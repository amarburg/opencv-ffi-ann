
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
  CovarianceBruteForceMatcher::CovarianceBruteForceMatcher( const Matx33d h, const Mat hcov, double weight  )
    : _h(h), _hcov(hcov.ptr<double>(0)), _weight( weight )
  {
    CV_Assert( hcov.type() == CV_64F );
  }

  Point2d CovarianceBruteForceMatcher::map_lr( const Point2d &pt )
  {
    Vec3d vec( pt.x, pt.y, 1 );
    Vec3d mapped = _h * vec;

    return Point2d( mapped[0]/mapped[2], mapped[1]/mapped[2] );
  }

  Matx22d CovarianceBruteForceMatcher::point_covariance( const Point2d &pt )
  {
    double h1x = _h(0,0)*pt.x + _h(0,1)*pt.y + _h(0,2),
           h2x = _h(1,0)*pt.x + _h(1,1)*pt.y + _h(1,2),
           h3x = _h(2,0)*pt.x + _h(2,1)*pt.y + _h(2,2);
    double h3x2 = h3x*h3x;

    double jd[] = { pt.x/h3x, pt.y/h3x, 1.0/h3x, 0, 0, 0, -pt.x*h1x/h3x2, -pt.y*h1x/h3x2,
                    0, 0, 0,   pt.x/h3x, pt.y/h3x, 1/h3x, -pt.x*h2x/h3x2, -pt.y*h2x/h3x2 };
    Matx<double, 2, 8> j( jd );
    Matx22d cov = j * _hcov * j.t();

    return cov.inv();
  }

  double CovarianceBruteForceMatcher::reproj_distance( const Point2d &q, const Point2d &t )
  {
    Point2d mapped = map_lr( q );
    Vec2d err( mapped.x - t.x, mapped.y - t.y );

    Matx<double,1,1> cost = err.t() * point_covariance( q ) * err;

    return cost(0,0);
  }

  vector<GeomDMatch> CovarianceBruteForceMatcher::match( const FeatureSet &query, 
      const FeatureSet &train )
  {
    vector< vector<DMatch> > dmatches;

    DescriptorMatcher *matcher =  new cv::BFMatcher( NORM_L2, false ); 
    matcher->knnMatch( query.desc, train.desc, dmatches, 50 );
    delete matcher;

    vector<GeomDMatch> out;

    for( vector< vector<DMatch> >::iterator itr = dmatches.begin();
        itr != dmatches.end(); ++itr ) {
      vector<DMatch> &matches( *itr );
      DMatch &best( *matches.begin() );

      double best_geom_dist = reproj_distance( query.kps[ best.queryIdx ].pt,
                                               train.kps[ best.trainIdx ].pt );
      double best_dist = best.distance + _weight * best_geom_dist;

      if( matches.size() == 1 ) {
        out.push_back( GeomDMatch( best, best_dist, best_geom_dist, _weight ) );
        continue;
      }

      int i = 1;
      for( vector<DMatch>::iterator itr = matches.begin()++;
          itr != matches.end(); ++itr, ++i ) {
        DMatch &dmatch( *itr );

        if( dmatch.distance > best_dist ) break;

        double geom_dist = reproj_distance( query.kps[ dmatch.queryIdx ].pt,
              train.kps[ dmatch.trainIdx ].pt );
        double dist = dmatch.distance + _weight * geom_dist;

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

};



