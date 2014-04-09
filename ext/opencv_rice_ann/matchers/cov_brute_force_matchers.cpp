
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
             h3x = _h(2,0)*pt.x + _h(2,1)*pt.y + _h(2,2);
      float h3x2 = h3x*h3x;

      float jd[] = { pt.x/h3x, pt.y/h3x, 1.0f/h3x, 0, 0, 0, -pt.x*h1x/h3x2, -pt.y*h1x/h3x2,
                     0, 0, 0,   pt.x/h3x, pt.y/h3x, 1.0f/h3x, -pt.x*h2x/h3x2, -pt.y*h2x/h3x2 };
      Matx<float, 2, 8> j( jd );
      Matx22f cov = j * _hcov * j.t();

      Matx22f inv = cov.inv();
      return inv;
  }

  float CovarianceBFMatcher::reproj_distance( const Point2f &qmapped, const Matx22f &qcov, const Point2f &t )
  {
    Vec2f err( qmapped.x - t.x, qmapped.y - t.y );

    Matx<float,1,1> cost = err.t() * qcov * err;

    return cost(0,0);
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

    for( vector< vector<DMatch> >::iterator itr = dmatches.begin();
        itr != dmatches.end(); ++itr ) {
      vector<DMatch> &matches( *itr );
      DMatch &best( *matches.begin() );

      Point2f qmapped = map_lr( query.kps[ best.queryIdx ].pt );
      Matx22d qcov = point_covariance( query.kps[ best.queryIdx ].pt );

      float best_geom_dist = reproj_distance( qmapped, qcov, train.kps[ best.trainIdx ].pt );
      float best_dist = best.distance + _weight * best_geom_dist;

      if( matches.size() == 1 ) {
        out.push_back( GeomDMatch( best, best_dist, best_geom_dist, _weight ) );
        continue;
      }

      for( vector<DMatch>::iterator itr = matches.begin()++;
          itr != matches.end(); ++itr ) {
        DMatch &dmatch( *itr );

        if( dmatch.distance > best_dist ) break;

        float geom_dist = reproj_distance( qmapped, qcov, train.kps[ dmatch.trainIdx ].pt );
        float dist = dmatch.distance + _weight * geom_dist;

        if( dist < best_dist ) {
          best = dmatch;
          best_dist = dist;
          best_geom_dist = geom_dist;
        }

      }

      out.push_back( GeomDMatch( best, best_dist, best_geom_dist, _weight ) );

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

    for( vector< vector<DMatch> >::iterator itr = dmatches.begin();
        itr != dmatches.end(); ++itr ) {
      vector<DMatch> &matches( *itr );
      DMatch &best( *matches.begin() );

      Point2f qmapped = map_lr( query.kps[ best.queryIdx ].pt );
      Matx22d qcov = point_covariance( query.kps[ best.queryIdx ].pt );

      float best_geom_dist = reproj_distance( qmapped, qcov, train.kps[ best.trainIdx ].pt );
      float best_dist = best.distance + _weight * best_geom_dist;
      double second_best = 0;

      for( vector<DMatch>::iterator itr = matches.begin()+1;
          itr != matches.end(); ++itr ) {
        DMatch &dmatch( *itr );

        // TODO.  Figure out this heuristic with ratio included..
        //if( dmatch.distance > best_dist ) break;

        double geom_dist = reproj_distance( qmapped, qcov, train.kps[ dmatch.trainIdx ].pt );
        double dist = dmatch.distance + _weight * geom_dist;

        if( dist < best_dist ) {
          best = dmatch;
          second_best = best_dist;
          best_dist = dist;
          best_geom_dist = geom_dist;
        } else if ( (second_best == 0) || (dist < second_best) ) {
          second_best = dist;
        }

      }

      if( (second_best == 0) || ((best_dist * _ratio) < second_best) ) {
        out.push_back( GeomDMatch( best, best_dist, best_geom_dist, _weight ) );
      }

    }

    return out;
  }



};



