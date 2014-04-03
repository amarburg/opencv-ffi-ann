
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


CovarianceBruteForceMatcher::CovarianceBruteForceMatcher( const Matx33d &h, const Mat &hcov  )
  : _h(h), _hcov(hcov)
{
  ;
}

Point2d CovarianceBruteForceMatcher::map_lr( const Point2d &pt )
{
  Vec3d vec( pt.x, pt.y, 1 );
  Vec3d mapped = _h * vec;

  return Point2d( mapped[0]/mapped[2], mapped[1]/mapped[2] );
}

Matx22d CovarianceBruteForceMatcher::point_covariance( const Point2d &pt )
{
  return Matx22d( 1, 0, 0, 1 );
}

double CovarianceBruteForceMatcher::reproj_distance( const Point2d &q, const Point2d &t )
{
  Point2d mapped = map_lr( q );
  Vec2d err( mapped.x - t.x,
      mapped.y - t.y );

  Matx<double,1,1> cost = err.t() * point_covariance( q ) * err;

  return cost(0,0);
}

vector<DMatch> CovarianceBruteForceMatcher::match( const FeatureSet &query, 
    const FeatureSet &train )
{
  vector< vector<DMatch> > dmatches;

  DescriptorMatcher *matcher =  new cv::BFMatcher( NORM_L2, false ); 
  matcher->knnMatch( query.desc, train.desc, dmatches, 50 );
  delete matcher;

  vector<DMatch> out;
  
  for( vector< vector<DMatch> >::iterator itr = dmatches.begin();
      itr != dmatches.end(); ++itr ) {
    vector<DMatch> &matches( *itr );
    DMatch &best( *matches.begin() );

    if( matches.size() == 1 ) {
      out.push_back( best );
      continue;
    }

    double best_dist = best.distance + reproj_distance( query.kps[ best.queryIdx ].pt,
                                                        train.kps[ best.trainIdx ].pt );



    for( vector<DMatch>::iterator itr = matches.begin()++;
        itr != matches.end(); ++itr ) {
      DMatch &dmatch( *itr );

      double dist = dmatch.distance + 
        reproj_distance( query.kps[ dmatch.queryIdx ].pt,
            train.kps[ dmatch.trainIdx ].pt );

      if( dist < best_dist ) {
        best = dmatch;
        best_dist = dist;
      }

    }

    out.push_back( best );

  }

  return out;
}

