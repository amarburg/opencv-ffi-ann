#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>

#include "matchers.h"

#ifndef __CVRICE_MATCHERS_STRICTLY_GEOMETRY_MATCHER_H__
#define __CVRICE_MATCHERS_STRICTLY_GEOMETRY_MATCHER_H__

namespace CVRiceMatchers {

  using cv::Point2f;
  using cv::Matx33f;
  using cv::DMatch;

  class StrictlyGeometryMatcher  {
    public:
      StrictlyGeometryMatcher( const Matx33f h, const float threshold );
      virtual ~StrictlyGeometryMatcher() {;}

      virtual std::vector<DMatch> match( const FeatureSet &query, const FeatureSet &train );

    protected:
      float residual_distance( const Point2f &q, const Point2f &t );
      Point2f map_lr( const Point2f &pt );

      Matx33f _h;
      float _threshold;
  };

};




#endif
