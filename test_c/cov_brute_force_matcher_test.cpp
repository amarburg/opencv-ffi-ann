
#include "gtest/gtest.h"

#include <opencv2/core.hpp>

#include "opencv_rice_ann/matchers/cov_brute_force_matchers.h"

using namespace cv;
using namespace CVRice;


TEST( CovarianceBFMatcher, point_covariance )
{
  CovarianceBFMatcher matcher( Matx33f( 1,0,0,0,1,0,0,0,1 ),  Mat(8,8,CV_64F)  );

  Point2f pt_a( 1.0, 1.0 );
  Matx22f cov_a = matcher.point_covariance( pt_a ); 
  Matx22f cov_a2 = matcher.point_covariance( pt_a );
  
}

