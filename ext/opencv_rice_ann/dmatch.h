
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>

#include <rice/Object.hpp>

#include <opencv_rice/core/dmatch.h>
#include <opencv_rice/core/keypoint.h>

#ifndef __CVRICE_MATCHERS_DMATCH_H__
#define __CVRICE_MATCHERS_DMATCH_H__

namespace CVRiceMatchers {

  using std::vector;
  using cv::DMatch;
  using cv::Mat;
  using CVRice::DMatchVector;

  Mat dmatches_to_mat( const DMatchVector dmatches, const KeyPointVector kps, int which );
  DMatchVector dmatches_select_by_inliers( const DMatchVector dmatches, const KeyPointVector query, 
                                           const KeyPointVector train, const Mat &h, const double threshold );
  DMatchVector dmatches_select_by_mask( const DMatchVector dmatches, const Mat &mask );

  void init_dmatch_functions( Rice::Module &rb_module );

};

#endif
