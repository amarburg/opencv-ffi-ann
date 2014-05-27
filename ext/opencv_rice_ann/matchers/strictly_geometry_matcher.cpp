
#include <iostream>
#include <vector>
using std::vector;
using std::endl;
using std::cout;

#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
using namespace cv;

#include "strictly_geometry_matcher.h"

#include <iostream>

namespace CVRiceMatchers {

  // TODO.  What if hcov isn't CV_64F?
  StrictlyGeometryMatcher::StrictlyGeometryMatcher( const Matx33f h, const float threshold ) 
    : _h(h), _threshold( threshold*threshold )
  {
  }

  Point2f StrictlyGeometryMatcher::map_lr( const Point2f &pt )
  {
    Vec3f vec( pt.x, pt.y, 1 );
    Vec3f mapped = _h * vec;

    return Point2f( mapped[0]/mapped[2], mapped[1]/mapped[2] );
  }

  float StrictlyGeometryMatcher::residual_distance( const Point2f &q, const Point2f &t )
  {
    return ((q.x-t.x)*(q.x-t.x)) + ((q.y-t.y)*(q.y-t.y));
  }

  vector<DMatch> StrictlyGeometryMatcher::match( const FeatureSet &query, const FeatureSet &train )
  {
    vector<DMatch> out;

    vector<Point2f> qmapped;
    for( KeyPointVector::const_iterator qitr = query.kps.begin(); qitr != query.kps.end(); ++qitr ) {
      qmapped.push_back( map_lr( (*qitr).pt ) );
    }

    for( unsigned int qidx = 0; qidx < query.kps.size(); ++qidx ) {

      const Point2f &qpt( qmapped[qidx] );

      float best_dist = residual_distance( qpt, train.kps[0].pt );
      int best_idx = 0;

      for( unsigned int tidx = 0; tidx < train.kps.size(); ++tidx ) {
        // Insert early-escape heuristics here...

        float dist = residual_distance( qpt, train.kps[tidx].pt );
        if( dist < best_dist ) {
          best_dist = dist;
          best_idx = tidx;
        }
      }


//      if( best_dist < _threshold )
        out.push_back( DMatch( qidx, best_idx, best_dist ) );

    }

    return out;
  }

//  vector<DMatch> StrictlyGeometryMatcher::match( const FeatureSet &query, const FeatureSet &train )
//  {
//    vector<DMatch> out;
//
//    vector<Point2f> tmapped;
//    for( KeyPointVector::const_iterator titr = train.kps.begin(); titr != train.kps.end(); ++titr ) {
//      tmapped.push_back( map_rl( (*titr).pt ) );
//    }
//
//    for( unsigned int qidx = 0; qidx < query.kps.size(); ++qidx ) {
//
//      const Point2f &qpt( query.kps[qidx].pt );
//
//      float best_dist = residual_distance( qpt, tmapped[0] );
//      int best_idx = 0;
//
//      for( unsigned int tidx = 0; tidx < train.kps.size(); ++tidx ) {
//        // Insert early-escape heuristics here...
//
//        float dist = residual_distance( qpt, tmapped[tidx] );
//        if( dist < best_dist ) {
//          best_dist = dist;
//          best_idx = tidx;
//        }
//      }
//
//
////      if( best_dist < _threshold )
//        out.push_back( DMatch( qidx, best_idx, best_dist ) );
//
//    }
//
//    return out;
//  }


};



