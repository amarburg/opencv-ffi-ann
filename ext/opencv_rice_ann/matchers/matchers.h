
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>

#include <rice/Object.hpp>

#include <opencv_rice/core/dmatch.h>
#include <opencv_rice/core/keypoint.h>

#include "../feature_set.h"

#ifndef __CVRICE_MATCHERS_MATCHER_H__
#define __CVRICE_MATCHERS_MATCHER_H__

namespace CVRice {

  // Written as a "simplified wrapper" around the OpenCV flann toolset
  // mostly because I don't know how (heavily templated) Rice will react 
  // to the heavily templated OpenCV stuff..
  //
  // As always, the question is whether this is necessary or if the 
  // base class can be wrapped directly...
  //
  // TODO:  Consider if this intermediate class is necessary..
  class Matcher {
    public:

      Matcher() : _matcher(NULL)  
    {;}
      virtual ~Matcher() 
      { if(_matcher) delete _matcher;}

      virtual void train( const cv::Mat descriptors );
      virtual std::vector<cv::DMatch> match( const cv::Mat query, const cv::Mat train );
      virtual std::vector<cv::DMatch> match( const cv::Mat query );

    protected:

      std::vector<cv::DMatch> ratio_match( const cv::Mat query, float ratio );
      std::vector<cv::DMatch> ratio_match( const cv::Mat query, const cv::Mat train, float ratio );

      cv::DescriptorMatcher *_matcher;

  };

  std::vector<cv::DMatch> ratio_match( cv::DescriptorMatcher *matcher, const cv::Mat query, float ratio );
  std::vector<cv::DMatch> ratio_match( cv::DescriptorMatcher *matcher, const cv::Mat query, const cv::Mat train, float ratio );


  void init_matchers( Rice::Module &rb_module );

}

#endif
