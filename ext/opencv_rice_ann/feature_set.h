
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>

#include <rice/Object.hpp>

#include <opencv_rice/core/dmatch.h>
#include <opencv_rice/core/keypoint.h>

#ifndef __CVRICE_MATCHERS_FEATURE_SET_H__
#define __CVRICE_MATCHERS_FEATURE_SET_H__

namespace CVRice {

  class FeatureSet {
    public:

      FeatureSet( const KeyPointVector kps_, const cv::Mat desc_ )
        : kps( kps_ ), desc( desc_ ) {;}

      int length( void ) const { return kps.size(); }

      void apply_intrinsics( const cv::Matx33f &k );

      KeyPointVector kps;
      cv::Mat desc;
  };

  void init_feature_set( Rice::Module &rb_module );

};

#endif
