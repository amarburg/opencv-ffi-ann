
#include "matchers.h"

#ifndef __CVRICE_MATCHERS_FLANN_MATCHERS_H__
#define __CVRICE_MATCHERS_FLANN_MATCHERS_H__


class KdTreeFlannMatcher : public Matcher {
  public:
    KdTreeFlannMatcher() 
      : Matcher()
    {_matcher = new FlannBasedMatcher( makePtr<flann::KDTreeIndexParams>() ); }
};

class KdTreeFlannRatioMatcher : public KdTreeFlannMatcher {
  public:
    KdTreeFlannRatioMatcher( float ratio ) 
      : KdTreeFlannMatcher()
    { _ratio = ratio; }

    vector<DMatch> match( const Mat query, const Mat train )
    { return ratio_match( query, train, _ratio ); }

    vector<DMatch> match( const Mat query )
    { return ratio_match( query, _ratio ); }

  protected:
    float _ratio;
};

class KMeansFlannMatcher : public Matcher {
  public:
    KMeansFlannMatcher() 
      : Matcher()
    {_matcher = new FlannBasedMatcher( makePtr<flann::KMeansIndexParams>() ); }
};

class KMeansFlannRatioMatcher : public KMeansFlannMatcher {
  public:
    KMeansFlannRatioMatcher( float ratio ) 
      : KMeansFlannMatcher() { _ratio = ratio; }

    vector<DMatch> match( const Mat query, const Mat train )
    { return ratio_match( query, train, _ratio ); }

    vector<DMatch> match( const Mat query )
    { return ratio_match( query, _ratio ); }

  protected:
    float _ratio;
};


#endif

