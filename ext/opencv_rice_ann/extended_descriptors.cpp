
#include <iostream>
#include <vector>
using std::vector;
using std::endl;
using std::cout;

#include <opencv2/core/core_c.h>
#include <opencv2/core.hpp>
using namespace cv;

#include <rice/Class.hpp>
#include <rice/Constructor.hpp>
#include <rice/Array.hpp>
using namespace Rice;

#include "descriptors.h"
#include "to_from_ruby.h"


ExtendedDescriptors::ExtendedDescriptors( KeyPointVector vector, const Mat descriptors, double weight )
: Descriptors( descriptors ), 
  _kps( vector ),
  _weight( weight )
{ 
  ;
}

ExtendedDescriptors::~ExtendedDescriptors( void )
{ ; }

// TODO:  specifying the type (CV_32F, CV_64F) of the output 
Mat ExtendedDescriptors::descriptors_to_mat( int type )
{
  type =  CV_32F;

  Mat out( length(), descriptor_length() + 2, type );

Mat first = _descriptors.colRange( 0, descriptor_length() );
Mat converted;
_descriptors.convertTo( converted, type );
converted.copyTo( first );
  
  for( int r = 0; r < length(); ++r ) {
    KeyPoint &feat( _kps[r] );

    switch(type) {
      case CV_32F:
        out.at<float>(r,descriptor_length()  ) = _weight * feat.pt.x;
        out.at<float>(r,descriptor_length()+1) = _weight * feat.pt.y;
        break;
      case CV_64F:
        out.at<double>(r,descriptor_length()  ) = _weight * feat.pt.x;
        out.at<double>(r,descriptor_length()+1) = _weight * feat.pt.y;
        break;
    }

  }

  return out;
}

Mat ExtendedDescriptors::warp_descriptors_to_mat( const Mat h, int type )
{
  KeyPointVector warped;

  for( KeyPointVector::iterator itr = _kps.begin(); itr != _kps.end(); ++itr ) {
    Point feat( (*itr).pt );

    float x = h.at<float>(0,0)*feat.x + h.at<float>(0,1)*feat.y + h.at<float>(0,2);
    float y = h.at<float>(1,0)*feat.x + h.at<float>(1,1)*feat.y + h.at<float>(1,2);
    float z = h.at<float>(2,0)*feat.x + h.at<float>(2,1)*feat.y + h.at<float>(2,2);

    feat.x = x/z;
    feat.y = y/z;

    KeyPoint newkp( *itr );
    newkp.pt = feat;
    warped.push_back( newkp );
  }

  return ExtendedDescriptors( warped, _descriptors, _weight ).descriptors_to_mat( type );
}


void init_extended_descriptors( Object &rb_mParent ) {
  Data_Type <ExtendedDescriptors> rc_cED = define_class_under<ExtendedDescriptors, Descriptors>( rb_mParent, "ExtendedDescriptors" )
    .define_constructor( Constructor<ExtendedDescriptors,KeyPointVector,const Mat,double>(), (Arg("keypoints"), Arg("descriptors"), Arg("weight") = 1) )
    .define_method( "length", &Descriptors::length )
    .define_method( "descriptors_to_mat", &ExtendedDescriptors::descriptors_to_mat, (Arg("type") = 0 ) )
    .define_method( "warp_descriptors_to_mat", &ExtendedDescriptors::warp_descriptors_to_mat,
        (Arg("h"), Arg("type") = 0 ) );
}
