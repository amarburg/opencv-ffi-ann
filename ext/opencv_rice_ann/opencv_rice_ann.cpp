
#include <rice/Class.hpp>
#include <rice/to_from_ruby_defn.hpp>
using namespace Rice;

#include <opencv_rice/opencv_rice.h>

#include "opencv_rice_ann.h"
#include "matchers/matchers.h"
#include "feature_set.h"

void init_descriptors( Object & );
void init_extended_descriptors( Object & );

using namespace CVRice;

extern "C"
void Init_libopencv_rice_ann( void ) {
  Module rb_mCVRice = define_module( "CVRice" );
  Module rb_mANN   = define_module_under( rb_mCVRice, "ANN" );

  init_descriptors( rb_mANN );
  init_feature_set( rb_mCVRice );
  init_extended_descriptors( rb_mANN );
  init_matchers( rb_mANN );

//  cv::Mat m;
//  Object o = to_ruby<cv::Mat>( m );
//  from_ruby<cv::Mat>( o );

  //Init_libopencv_rice();
}
