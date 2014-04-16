
#include <iostream>
#include <vector>
using std::vector;
using std::endl;
using std::cout;

#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
using namespace cv;

#include <rice/Class.hpp>
#include <rice/Constructor.hpp>
#include <rice/Array.hpp>
using namespace Rice;

#include "cov_brute_force_matchers.h"

#include <iostream>

namespace CVRice {

  // TODO.  What if hcov isn't CV_64F?
  CovarianceBFMatcher::CovarianceBFMatcher( const Matx33f h, const Mat hcov, float weight  )
    : _h(h), _weight( weight ), _knnDepth( 500 )
  {
    CV_Assert( hcov.type() == CV_64F );

    // Convert from Mat to Matx33f?
    for( int r = 0; r < 8; ++r ) {
      for( int c = 0; c < 8; ++c ) {
        _hcov(r,c) = (float)hcov.at<double>(r,c);
      }
    }
  }

  Point2f CovarianceBFMatcher::map_lr( const Point2f pt )
  {
    Vec3f vec( pt.x, pt.y, 1 );
    Vec3f mapped = _h * vec;

    return Point2f( mapped[0]/mapped[2], mapped[1]/mapped[2] );
  }

  Matx22f CovarianceBFMatcher::point_covariance( const Point2f pt )
  {
      float h1x = _h(0,0)*pt.x + _h(0,1)*pt.y + _h(0,2),
             h2x = _h(1,0)*pt.x + _h(1,1)*pt.y + _h(1,2),
             h3x = 1.0/(_h(2,0)*pt.x + _h(2,1)*pt.y + _h(2,2));
      float h3x2 = h3x*h3x;

      float jd[] = { pt.x*h3x, pt.y*h3x, h3x, 0, 0, 0,   -pt.x*h1x*h3x2, -pt.y*h1x*h3x2,
                     0, 0, 0,   pt.x*h3x, pt.y*h3x, h3x, -pt.x*h2x*h3x2, -pt.y*h2x*h3x2 };
      Matx<float, 2, 8> j( jd );
      Matx22f cov = j * _hcov * j.t();
      return cov.inv();
  }

  Vec2f CovarianceBFMatcher::error( const Point2f &qmapped, const Point2f &t )
  {
    return Vec2f( fabs(qmapped.x - t.x), fabs(qmapped.y - t.y) );
  }


  float CovarianceBFMatcher::reproj_distance( const Vec2f &err, const Matx22f &qcov )
  {

    Matx<float,1,1> cost = err.t() * qcov * err;
    return cost(0,0);
  }

float CovarianceBFMatcher::map_and_reproj_distance( const Point2f &q, const Point2f &t, const Matx22f &qcov )
  {
    return reproj_distance( error( map_lr( q ), t ), qcov );
  }


  std::vector< std::vector<cv::DMatch> > CovarianceBFMatcher::do_match( const Mat &query, const Mat &train )
  {
    vector< vector<DMatch> > dmatches;

    DescriptorMatcher *matcher =  new cv::BFMatcher( NORM_L2SQR, false ); 
    matcher->knnMatch( query, train, dmatches, _knnDepth );
    delete matcher;

    return dmatches;
  }


  vector<GeomDMatch> CovarianceBFMatcher::match( const FeatureSet &query, 
      const FeatureSet &train )
  {
    vector< vector<DMatch> > dmatches = do_match( query.desc, train.desc );
    vector<GeomDMatch> out;

    #pragma omp parallel for
    for( unsigned int i = 0; i < dmatches.size(); i++ ) {
      vector<DMatch> &matches( dmatches[i] );
      DMatch &first( *matches.begin() );

      Matx22f qcov = point_covariance( query.kps[ first.queryIdx ].pt );
      GeomDMatch best( first, 
          map_and_reproj_distance( query.kps[ first.queryIdx ].pt,
                                   train.kps[ first.trainIdx ].pt, qcov ), _weight );

      if( matches.size() == 1 ) {
        out.push_back(  best );
        continue;
      }

      for( unsigned int j = 0; j < matches.size(); ++j ) {
        DMatch &dmatch( matches[j] );

        //if( dmatch.distance > best.descDistance ) break;

        GeomDMatch current( dmatch, 
            map_and_reproj_distance( query.kps[ dmatch.queryIdx ].pt,
                                     train.kps[ dmatch.trainIdx ].pt, qcov ), _weight );

        if( current.distance < best.distance ) best = current;

      }

# pragma omp critical
      out.push_back( best );

    }

    return out;
  }


  //== CovarianceBFRatioMatcher =================
  CovarianceBFRatioMatcher::CovarianceBFRatioMatcher( const Matx33f h, const Mat hcov, float weight, float ratio  )
    : CovarianceBFMatcher( h, hcov, weight ), _ratio(ratio * ratio)
  {;}


  vector<GeomDMatch> CovarianceBFRatioMatcher::match( const FeatureSet &query, const FeatureSet &train )
  {
    vector< vector<DMatch> > dmatches = do_match( query.desc, train.desc );

    vector<GeomDMatch> out;

#pragma omp parallel for 
    for( unsigned int i = 0; i < dmatches.size(); i++ ) {
      vector<DMatch> &matches( dmatches[i] );
      DMatch &first( *matches.begin() );

      Matx22f qcov = point_covariance( query.kps[ first.queryIdx ].pt );
      GeomDMatch best( first, 
          map_and_reproj_distance( query.kps[ first.queryIdx ].pt,
                                   train.kps[ first.trainIdx ].pt, qcov ), _weight ),
                 second( best );

      for( unsigned int j = 1; j < matches.size(); ++j )  {
        DMatch &dmatch( matches[j] );

        // TODO.  Figure out this heuristic with ratio included..
        //if( dmatch.distance > best_dist ) break;

        GeomDMatch current( dmatch, 
            map_and_reproj_distance( query.kps[ dmatch.queryIdx ].pt,
              train.kps[ dmatch.trainIdx ].pt, qcov ), _weight );

        if( current.distance < best.distance ) {
          second = best;
          best = current;
        } else if ( second == best ) {
          second = current;
        }

      }

#pragma omp critical
      if( do_accept_dmatch( best, second ) ) out.push_back( best );

    }

    return out;
  }


  bool CovarianceBFRatioMatcher::do_accept_dmatch( const GeomDMatch &best, const GeomDMatch &second )
  {
    return  (second == best) ||
      ((best.distance * _ratio) < second.distance);
  }


  //== CovarianceIndependentRatioMatcher =================
  CovarianceIndependentRatioMatcher::CovarianceIndependentRatioMatcher( const Matx33f h, const Mat hcov, float weight, 
      float desc_ratio, float geom_ratio)
    : CovarianceBFRatioMatcher( h, hcov, weight, desc_ratio ), 
    _descRatio( desc_ratio * desc_ratio ), 
    _geomRatio( geom_ratio * geom_ratio )
  {;}


  bool CovarianceIndependentRatioMatcher::do_accept_dmatch( const GeomDMatch &best, const GeomDMatch &second )
  {
    return (second == best) || 
      (((best.geomDistance * _geomRatio) < second.geomDistance) &&
       ((best.descDistance * _descRatio) < second.descDistance));
  }



};



