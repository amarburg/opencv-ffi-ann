#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>

#include "matchers.h"

#ifndef __CVRICE_MATCHERS_BRUTE_FORCE_MATCHERS_H__
#define __CVRICE_MATCHERS_BRUTE_FORCE_MATCHERS_H__

namespace CVRiceMatchers {

  class L2BruteForceMatcher : public Matcher {
    public:
      L2BruteForceMatcher( bool crosscheck = false ) 
        : Matcher() {  _matcher = new cv::BFMatcher( NORM_L2, crosscheck ); }
  };

  class L2BFRatioMatcher : public L2BruteForceMatcher {
    public:
      L2BFRatioMatcher( float ratio, bool crosscheck = false )
        : L2BruteForceMatcher() { _ratio = ratio; }

      vector<cv::DMatch> match( const cv::Mat query, const cv::Mat train ) 
      { return ratio_match( query, train, _ratio ); }

    protected:
      float _ratio;
  };


  class L2SqrBruteForceMatcher : public Matcher {
    public:
      L2SqrBruteForceMatcher( bool crosscheck = false ) : Matcher()
    {  _matcher = new cv::BFMatcher( NORM_L2SQR, crosscheck ); }
  };

  class L2SqrBFRatioMatcher : public L2SqrBruteForceMatcher {
    public:
      L2SqrBFRatioMatcher( float ratio, bool crosscheck = false ) 
        : L2SqrBruteForceMatcher( crosscheck ), _ratio(ratio*ratio) {;}

      vector<cv::DMatch> match( const cv::Mat query, const cv::Mat train ) 
      { return ratio_match( query, train, _ratio ); }

    protected:
      float _ratio;
  };

};




#endif
