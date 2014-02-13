
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

#include "descriptors.h"
#include "to_from_ruby.h"


ExtendedDescriptors::ExtendedDescriptors( SiftFeatureVector vector, double weight )
: Descriptors( vector ), 
  _weight( weight )
{ 
  ;
}

ExtendedDescriptors::~ExtendedDescriptors( void )
{ ; }

// TODO:  specifying the type (CV_32F, CV_64F) of the output 
Mat ExtendedDescriptors::descriptors_to_mat( Symbol foo )
{

  int type =  CV_32F;
  int descriptor_length = first().d;

  Mat out( _features.size(), descriptor_length + 2, type );

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

    switch(type) {
      case CV_32F:
        out.at<float>(r,descriptor_length  ) = _weight * feat.x;
        out.at<float>(r,descriptor_length+1) = _weight * feat.y;
        break;
      case CV_64F:
        out.at<double>(r,descriptor_length  ) = _weight * feat.x;
        out.at<double>(r,descriptor_length+1) = _weight * feat.y;
        break;
    }

  }

  return out;
}

Mat ExtendedDescriptors::warp_descriptors_to_mat( const Mat h, Symbol type )
{
  SiftFeatureVector warped;

  for( SiftFeatureVector::iterator itr = _features.begin(); itr != _features.end(); itr++ ) {
    SiftFeature feat( *itr );

    float x = h.at<float>(0,0)*feat.x + h.at<float>(0,1)*feat.y + h.at<float>(0,2);
    float y = h.at<float>(1,0)*feat.x + h.at<float>(1,1)*feat.y + h.at<float>(1,2);
    float z = h.at<float>(2,0)*feat.x + h.at<float>(2,1)*feat.y + h.at<float>(2,2);

    feat.x = x/z;
    feat.y = y/z;

    warped.push_back( feat );
  }

  return ExtendedDescriptors( warped, _weight ).descriptors_to_mat( type );
}


void init_extended_descriptors( Object &rb_mBenchmarking ) {
  Data_Type <ExtendedDescriptors> rc_cED = define_class_under<ExtendedDescriptors, Descriptors>( rb_mBenchmarking, "ExtendedDescriptors" )
    .define_constructor( Constructor<ExtendedDescriptors,SiftFeatureVector,double>() )
    .define_method( "length", &Descriptors::length )
    .define_method( "descriptors_to_mat", &ExtendedDescriptors::descriptors_to_mat )
    .define_method( "warp_descriptors_to_mat", &ExtendedDescriptors::warp_descriptors_to_mat );
}
