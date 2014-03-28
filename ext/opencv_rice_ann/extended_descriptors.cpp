
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

#include <iostream>

ExtendedDescriptors::ExtendedDescriptors( KeyPointVector vector, const Mat descriptors, double weight )
: Descriptors( descriptors ), 
  _kps( vector ),
  _weight( weight )
{ 
  assert( _kps.size() == num_descriptors() );
}

ExtendedDescriptors::~ExtendedDescriptors( void )
{ ; }

// TODO:  specifying the type (CV_32F, CV_64F) of the output 
Mat ExtendedDescriptors::descriptors_to_mat( int type )
{
  type =  CV_32F;

  Mat out( num_descriptors(), descriptor_length() + 2, type );

  Mat first = _descriptors.colRange( 0, descriptor_length() );
  Mat converted;
  _descriptors.convertTo( converted, type );
  converted.copyTo( first );

  for( unsigned int r = 0; r < num_descriptors(); ++r ) {
    KeyPoint &feat( _kps[r] );

    switch(type) {
      case CV_32F:
        cout << "Feat: " << feat.pt.x << ' ' << feat.pt.y << endl;

        out.at<float>(r,descriptor_length()  ) = _weight * feat.pt.x;
        out.at<float>(r,descriptor_length()+1) = _weight * feat.pt.y;


        for( unsigned int i = 0; i < descriptor_length() + 2; ++i ) {
          cout << out.at<float>(r,i) << ' ';
        }
        cout << endl;
        break;
      case CV_64F:
        out.at<double>(r,descriptor_length()  ) = _weight * feat.pt.x;
        out.at<double>(r,descriptor_length()+1) = _weight * feat.pt.y;
        break;
    }

  }

  return out;
}

ExtendedDescriptors ExtendedDescriptors::warp_descriptors( const Mat h )
{
  KeyPointVector warped;
  Mat hf;
  h.convertTo( hf, CV_32F );

  for( KeyPointVector::iterator itr = _kps.begin(); itr != _kps.end(); ++itr ) {
    Point2f feat( (*itr).pt );

    float x = hf.at<float>(0,0)*feat.x + hf.at<float>(0,1)*feat.y + hf.at<float>(0,2);
    float y = hf.at<float>(1,0)*feat.x + hf.at<float>(1,1)*feat.y + hf.at<float>(1,2);
    float z = hf.at<float>(2,0)*feat.x + hf.at<float>(2,1)*feat.y + hf.at<float>(2,2);

    feat.x = x/z;
    feat.y = y/z;

    KeyPoint newkp( *itr );
    newkp.pt.x = feat.x;
    newkp.pt.y = feat.y;
    warped.push_back( newkp );
  }

  return ExtendedDescriptors( warped, _descriptors, _weight );
}


void init_extended_descriptors( Object &rb_mParent ) {
  Data_Type <ExtendedDescriptors> rc_cED = define_class_under<ExtendedDescriptors, Descriptors>( rb_mParent, "ExtendedDescriptors" )
    .define_constructor( Constructor<ExtendedDescriptors,KeyPointVector,const Mat,double>(), (Arg("keypoints"), Arg("descriptors"), Arg("weight") = 1) )
    .define_method( "descriptors_to_mat", &ExtendedDescriptors::descriptors_to_mat, (Arg("type") = 0 ) )
    .define_method( "warp_descriptors", &ExtendedDescriptors::warp_descriptors );
}
