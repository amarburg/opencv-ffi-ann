
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>

#include <rice/Object.hpp>

#include <opencv_rice/core/dmatch.h>
#include <opencv_rice/core/keypoint.h>

#ifndef __CVRICE_MATCHERS_FEATURE_SET_H__
#define __CVRICE_MATCHERS_FEATURE_SET_H__

namespace CVRiceMatchers {

  class FeatureSet {
    public:

      FeatureSet( const KeyPointVector kps_, const cv::Mat desc_ )
        : kps( kps_ ), desc( desc_ ) {;}

      int length( void ) const { return kps.size(); }

      void apply_intrinsics( const cv::Matx33f &k );

      KeyPointVector kps;
      cv::Mat desc;
  };

  class FeatureSetCovariance : public FeatureSet {
    public:

      FeatureSetCovariance( const KeyPointVector _kps, const cv::Mat _desc, const std::vector< cv::Matx22f > _cov );

      std::vector< cv::Matx22f > covs;
  };
                  

  void init_feature_set( Rice::Module &rb_module );

};

template <>
inline
std::vector< cv::Matx22f > from_ruby< std::vector< cv::Matx22f > >( Rice::Object o )
{
  Rice::Array arr(o);
  std::vector< cv::Matx22f > vec;

  for( Rice::Array::iterator itr = arr.begin(); itr != arr.end(); ++itr ) {
    vec.push_back( from_ruby<cv::Matx22f>( *itr ) );
  }
  return vec;
}

template<>
  inline
Rice::Object to_ruby< std::vector<cv::Matx22f> >( std::vector<cv::Matx22f> const &vec )
{
  return Rice::Array(vec.begin(), vec.end());
}

#endif
