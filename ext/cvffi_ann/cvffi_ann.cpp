
#include <rice/Class.hpp>
using namespace Rice;

#include "cvffi_ann.h"

void init_descriptors( Object & );
void init_extended_descriptors( Object & );

extern "C"
void Init_libcvffi_ann_c( void ) {
  Module rb_mCVFFI = define_module( "CVFFI" );
  Module rb_mANN   = define_module_under( rb_mCVFFI, "ANN" );
  Module rb_mBenchmarking = define_module_under( rb_mANN, "Benchmarking" );

  init_descriptors( rb_mBenchmarking );
  init_extended_descriptors( rb_mBenchmarking );

  init_flann_matcher( rb_mANN );
}
