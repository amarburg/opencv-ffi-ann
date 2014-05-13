
#include <iostream>
#include <vector>
using std::vector;
using std::endl;
using std::cout;

#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
using namespace cv;

#include <rice/Class.hpp>
#include <rice/Constructor.hpp>
#include <rice/Array.hpp>
using namespace Rice;

#include "feature_set.h"

namespace CVRiceMatchers {

  KeyPointVector feature_set_kps( const FeatureSet &set ) { return set.kps; }
  cv::Mat feature_set_desc( const FeatureSet &set ) { return set.desc; }
  void FeatureSet::apply_intrinsics( const Matx33f &k )
  {
    Matx33f kinv = k.inv();

    for( KeyPointVector::iterator itr = kps.begin(); itr != kps.end(); ++itr ) {
      Vec3f v( (*itr).pt.x, (*itr).pt.y, 1 );
      Vec3f mapped = kinv * v;

      (*itr).pt.x = mapped[0]/mapped[2];
      (*itr).pt.y = mapped[1]/mapped[2];
    }
  }


  FeatureSetCovariance::FeatureSetCovariance( const KeyPointVector _kps, const cv::Mat _desc, const vector< cv::Matx22f > _cov )
    : FeatureSet( _kps, _desc ), covs( _cov )
  {;}
  vector< cv::Matx22f > feature_set_covariance( const FeatureSetCovariance &set ) { return set.covs; }


  void init_feature_set( Module &rb_module ) {

    define_class_under<FeatureSet>( rb_module, "FeatureSet" )
      .define_constructor( Constructor<FeatureSet,KeyPointVector,Mat>() )
      .define_method("kps", &feature_set_kps )
      .define_method("keypoints", &feature_set_kps )
      .define_method("descs", &feature_set_desc )
      .define_method("descriptors", &feature_set_desc )
      .define_method("apply_intrinsics", &FeatureSet::apply_intrinsics );

    define_class_under<FeatureSetCovariance,FeatureSet>( rb_module, "FeatureSetCovariance" )
      .define_constructor( Constructor<FeatureSetCovariance,KeyPointVector, Mat,vector< cv::Matx22f > >() )
      .define_method("covs", &feature_set_covariance );
  }

};
