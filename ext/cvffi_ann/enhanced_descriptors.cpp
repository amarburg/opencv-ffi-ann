
#include <iostream>
#include <vector>
using std::vector;
using std::endl;
using std::cout;

#include <opencv2/core/core_c.h>
#include <opencv2/core.hpp>
using namespace cv;

#include <opencv-ffi/sift/sift.h>

#include <ruby.h>

#include <rice/Class.hpp>
#include <rice/Constructor.hpp>
#include <rice/Array.hpp>
using namespace Rice;

#include "enhanced_descriptors.h"
#include "to_from_ruby.h"


class EnhancedDescriptors {
  public:


    EnhancedDescriptors( SiftFeatureVector vector )
      : _features( vector )
    { 
      ;
    }

    ~EnhancedDescriptors( void )
    { ; }

    int length( void ) { return _features.size(); }

    // TODO:  specifying the type (CV_32F, CV_64F) of the output 
    Mat descriptors_to_mat( Symbol foo )
    {

      int type =  CV_32F;
      int descriptor_length = first().d;

      Mat out( _features.size(),descriptor_length, type );

      // TODO:  Put in more efficient implementation...
      //if( out.isContinuous() ) {
      //
      //} else {

      // Inefficient way for now
      for( unsigned int r = 0; r < _features.size(); r++ ) {
        SiftFeature &feat = _features[r];

        for( int c = 0; c < descriptor_length; c++ ) {
          switch(type) {
            case CV_32F:
              out.at<float>(r,c) = feat.descr[c];
              break;
            case CV_64F:
              out.at<double>(r,c) = feat.descr[c];
              break;
          }
        }
      }

      return out;
    }


  protected:

    SiftFeature first( void ) { return *(_features.begin()); }

    SiftFeatureVector _features;

};


void init_enhanced_descriptors( Object &rb_mBenchmarking ) {
  Data_Type <EnhancedDescriptors> rc_cED = define_class_under<EnhancedDescriptors>( rb_mBenchmarking, "EnhancedDescriptors" )
    .define_constructor( Constructor<EnhancedDescriptors,SiftFeatureVector>() )
    .define_method( "length", &EnhancedDescriptors::length )
    .define_method( "descriptors_to_mat", &EnhancedDescriptors::descriptors_to_mat );
}
