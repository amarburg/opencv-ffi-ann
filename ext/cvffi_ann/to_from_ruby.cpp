
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
#include <rice/Array.hpp>
using namespace Rice;

#include <ffi_c/Pointer.h>

#include "to_from_ruby.h"

using namespace cv;

template<class T>
T *ptr_from_ffi_struct( Object obj )
{
    Object pointer( obj.call( rb_intern("pointer") ) );
    Pointer *ptr;
    Data_Get_Struct( pointer.value(), struct Pointer, ptr );
    return reinterpret_cast<T *>(ptr->memory.address);
}

template<>
SiftFeatureVector from_ruby<SiftFeatureVector>( Object obj )
{
  SiftFeatureVector vector;

  // For flexibility, it will either be a CvSeq of SiftKeypoints or 
  // an Array of same ...?
  //
  // TODO: Don't like these rb_eval_strings...
  if( obj.is_instance_of( rb_eval_string("Array") ) ) {
    ;
  } else if( obj.is_instance_of( rb_eval_string("CVFFI::Features2D::SIFT::Results") ) ) {

    // seq.seq should be an CvSeq -> a (Nice)FFI::Struct wrapping a CvSeq 
    CvSeq *seq = ptr_from_ffi_struct<CvSeq>( obj.instance_eval("seq.seq" ) );
    
    CvSeqReader reader;
    cvStartReadSeq( seq, &reader );
    for( int i = 0; i < seq->total; i++ ) {
      // Too many copies going on here ...
      SiftFeature feat;
      CV_READ_SEQ_ELEM( feat, reader );

      vector.push_back( feat );
    }

  } else { 
    rb_raise(rb_eTypeError, "Can't create a SiftFeatureVector descriptors from this type");
  }

  return vector;
}


// Generic function turn Mat into a FFI::Pointer
template<>
Object to_ruby<Mat>( Mat const &m )
{
  // Need to make independent copy
  CvMat *cvmat = cvCreateMat( m.rows, m.cols, m.type() );
  CvMat mat = m;
  cvCopy( &mat, cvmat );

  VALUE ptr = rbffi_Pointer_NewInstance( cvmat );

  return ptr;
}


// TODO:  Could this be done more efficiently?
template<>
Mat from_ruby<Mat>( Object obj )
{
  Mat mat;

  if( obj.is_instance_of( rb_eval_string("Matrix") ) ) {
    Array arr( rb_funcall( obj, rb_intern("to_a"), 0 ) );
    Array first_row( arr[0] );

    mat.create( arr.size(), first_row.size(), CV_32F );

    for( unsigned int r = 0; r < arr.size(); r++ ) { 
      Array row( arr[r] );
      for( unsigned int c = 0; c < row.size(); c++ ) {
        mat.at<float>(r,c) = NUM2DBL( row[c].value() );
      }
    }
  } else if( obj.is_instance_of( rb_eval_string("CVFFI::CvMat" ) ) ) {
    CvMat *cvmat = ptr_from_ffi_struct<CvMat>( obj );

    return mat = cvarrToMat( cvmat );

  } else {
    rb_raise( rb_eTypeError, "Can't convert this type of object to a Mat" );
  }

  return mat;
}

