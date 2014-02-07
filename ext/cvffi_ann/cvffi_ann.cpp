
#include <rice/Class.hpp>
using namespace Rice;

#include "cvffi_ann.h"

void init_enhanced_descriptors( Object & );

extern "C"
void Init_libcvffi_ann_c( void ) {
  Module rb_mCVFFI = define_module( "CVFFI" );
  Module rb_mANN   = define_module_under( rb_mCVFFI, "ANN" );
  Module rb_mBenchmarking = define_module_under( rb_mANN, "Benchmarking" );

  init_enhanced_descriptors( rb_mBenchmarking );
}
