#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>

#include "matchers.h"

#ifndef __CVRICE_MATCHERS_COV_BRUTE_FORCE_MATCHERS_H__
#define __CVRICE_MATCHERS_COV_BRUTE_FORCE_MATCHERS_H__

  using cv::Point2d;
  using cv::Matx33d;
  using cv::Matx22d;
  using cv::DMatch;


class CovarianceBruteForceMatcher  {
  public:
    CovarianceBruteForceMatcher( const Matx33d &h, const Mat &hcov  );

    std::vector<DMatch> match( const FeatureSet &query, const FeatureSet &train );

  protected:
    Point2d map_lr( const Point2d &pt );
    Matx22d point_covariance( const Point2d &pt );
    double reproj_distance( const Point2d &q, const Point2d &t );

    Matx33d _h;
    Mat _hcov;
};


#endif
