
#include "gtest/gtest.h"

#include "opencv_rice_ann/descriptors.h"
using namespace cv;


TEST( ExtendedDescriptors, WarpDescriptors )
{
  KeyPointVector kps;
  kps.push_back( KeyPoint( 3.2, 4.0, 1.0 ) );
  kps.push_back( KeyPoint( 6.0, -2.1, 1.0 ) );

  const int num_desc = 2;
  const int desc_len = 128;
  Mat descriptors( num_desc, desc_len, CV_32F );
  for( int r = 0; r < num_desc; ++r ) {
    for( int c = 0; c < desc_len; ++c ) {
      descriptors.at<float>(r,c) = 0.1 * (r+c);
    }
  }

  ExtendedDescriptors ed( kps, descriptors, 1.0 );

  ExtendedDescriptors warped( ed.warp_descriptors( Mat::eye( 3, 3, CV_32F ) ) );

  ASSERT_EQ( 2, warped.keypoints().size() );
  ASSERT_FLOAT_EQ( 3.2, warped.keypoints()[0].pt.x );
  ASSERT_FLOAT_EQ( 4.0, warped.keypoints()[0].pt.y );
  ASSERT_FLOAT_EQ( 6.0, warped.keypoint(1).pt.x );
  ASSERT_FLOAT_EQ( -2.1, warped.keypoint(1).pt.y );

  // Ensure descriptor comes through unchanged
  ASSERT_EQ( num_desc, warped.descriptors().rows );
  ASSERT_EQ( desc_len, warped.descriptors().cols );
  for( int r = 0; r < num_desc; ++r ) {
    for( int c = 0; c < desc_len; ++c ) {
      ASSERT_FLOAT_EQ( (0.5*(r+c)), warped.descriptors().at<float>(r,c) );
    }
  }
}

