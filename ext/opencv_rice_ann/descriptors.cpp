
#include <iostream>
#include <vector>
using std::vector;
using std::endl;
using std::cout;

#include <opencv2/core/core_c.h>
#include <opencv2/core.hpp>
using namespace cv;

#include <ruby.h>

#include <rice/Class.hpp>
#include <rice/Constructor.hpp>
#include <rice/Array.hpp>
using namespace Rice;

#include "descriptors.h"
#include "to_from_ruby.h"


Descriptors::Descriptors( const Mat descriptors )
  : _descriptors( descriptors )
{;}


Descriptors::~Descriptors( void )
{;}

// TODO:  specifying the type (CV_32F, CV_64F) of the output 
Mat Descriptors::descriptors_to_mat( int type )
{
  type =  CV_32F;
  Mat out;

  _descriptors.convertTo( out, type );
  return out;
}

void init_descriptors( Object &rb_mParent ) {
  Data_Type <Descriptors> rc_cED = define_class_under<Descriptors>( rb_mParent, "Descriptors" )
    .define_constructor( Constructor<Descriptors,const Mat>() )
    .define_method( "length", &Descriptors::length )
    .define_method( "descriptor_length", &Descriptors::descriptor_length )
    .define_method( "descriptors_to_mat", &Descriptors::descriptors_to_mat, (Arg("type") = 0) );
}

