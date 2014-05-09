#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>

#include "matchers.h"

#ifndef __CVRICE_MATCHERS_ONLY_GEOMETRY_MATCHER_H__
#define __CVRICE_MATCHERS_ONLY_GEOMETRY_MATCHER_H__

namespace CVRice {

  using cv::Point2f;
  using cv::Matx33f;
  using cv::DMatch;

  class OnlyGeometryMatcher  {
    public:
      OnlyGeometryMatcher( const Matx33f h, const float threshold );
      virtual ~OnlyGeometryMatcher() {;}

      virtual std::vector<DMatch> match( const FeatureSet &query, const FeatureSet &train );

    protected:
      float residual_distance( const Point2f &q, const Point2f &t );
      Point2f map_rl( const Point2f &pt );

      Matx33f _hinv;
      float _threshold;
  };

};




#endif
