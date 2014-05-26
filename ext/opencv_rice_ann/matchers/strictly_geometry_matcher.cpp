
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
    : _hinv(h.inv()), _threshold( threshold )
  {
  }

  Point2f StrictlyGeometryMatcher::map_rl( const Point2f &pt )
  {
    Vec3f vec( pt.x, pt.y, 1 );
    Vec3f mapped = _hinv * vec;

    return Point2f( mapped[0]/mapped[2], mapped[1]/mapped[2] );
  }

  float StrictlyGeometryMatcher::residual_distance( const Point2f &q, const Point2f &t )
  {
    return ((q.x-t.x)*(q.x-t.x)) + ((q.y-t.y)*(q.y-t.y));
  }

  vector<DMatch> StrictlyGeometryMatcher::match( const FeatureSet &query, const FeatureSet &train )
  {
    vector<DMatch> out;

    vector<Point2f> tmapped;
    for( KeyPointVector::const_iterator titr = train.kps.begin(); titr != train.kps.end(); ++titr ) {
      tmapped.push_back( map_rl( (*titr).pt ) );
    }

    int qidx = 0;
    for( KeyPointVector::const_iterator qitr = query.kps.begin(); qitr != query.kps.end(); ++qitr, ++qidx ) {

      vector<Point2f>::const_iterator titr = tmapped.begin();

      float best_dist = residual_distance( (*qitr).pt, (*titr) );
      int best_idx = 0, idx = 0;

      for( ++titr, ++idx; titr != tmapped.end(); ++titr, ++idx ) {
        // Insert early-escape heuristics here...

        float dist = residual_distance( (*qitr).pt, (*titr) );
        if( dist < best_dist ) {
          best_dist = dist;
          best_idx = idx;
        }
      }


//      if( best_dist < _threshold )
        out.push_back( DMatch( qidx, best_idx, best_dist ) );

    }

    return out;
  }


};



