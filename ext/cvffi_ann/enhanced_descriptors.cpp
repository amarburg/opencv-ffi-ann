
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

typedef feature SiftFeature;
typedef vector<feature> SiftFeatureVector;

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

    // TODO:  specifying type is broken 
    Mat *descriptors_to_mat( Symbol foo )
    {

      int type =  CV_32F;
      int descriptor_length = first().d;
      cout << endl << descriptor_length << " " << _features.size() << endl;
      Mat *out = new Mat( descriptor_length, _features.size(), type );

      // TODO:  Put in more efficient implementation...
      //if( out.isContinuous() ) {
      //
      //} else {

      // Inefficient way for now
      for( unsigned int i = 0; i < _features.size(); i++ ) {
        SiftFeature &feat = _features[i];

        for( int j = 0; j < descriptor_length; j++ ) {
          switch(type) {
            case CV_32F:
              out->at<float>(i,j) = feat.descr[j];
              break;
            case CV_64F:
              out->at<double>(i,j) = feat.descr[j];
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


// From FFI
struct AbstractMemory {
  char* address; // Use char* instead of void* to ensure adding to it works correctly
  long size;
  int flags;
  int typeSize;
};

struct Pointer {
  struct AbstractMemory memory;
  VALUE rbParent;
  char* storage; /* start of malloc area */
  bool autorelease;
  bool allocated;
};

template<>
SiftFeatureVector from_ruby<SiftFeatureVector>( Object obj )
{
  SiftFeatureVector vector;

  // For flexibility, it will either be a CvSeq of SiftKeypoints or 
  // an Array of same ...?
  //
  // Don't like these rb_eval_strings...
  if( obj.is_instance_of( rb_eval_string("Array") ) ) {
    cout << "populating from array" << endl;
  } else if( obj.is_instance_of( rb_eval_string("CVFFI::Features2D::SIFT::Results") ) ) {
    //cout << "populating from sequence array" << endl;

    // Sketchy...
    // seq.seq should be an CvSeq -> a (Nice)FFI::Struct wrapping a CvSeq 
    Object pointer( (obj.instance_eval("seq.seq.pointer")) );
    if( pointer.is_nil() ) rb_raise(rb_eTypeError, "Hm, the pointer is nil");

    Pointer *ptr;
    Data_Get_Struct( pointer.value(), struct Pointer, ptr );

    CvSeq *seq = reinterpret_cast<CvSeq *>(ptr->memory.address);

    cout << "Loading from CvSeq with " << seq->total << " elements" << endl;


    CvSeqReader reader;

    cvStartReadSeq( seq, &reader );
    for( int i = 0; i < seq->total; i++ ) {
      // Too many copies going on here ...
      SiftFeature feat;
      CV_READ_SEQ_ELEM( feat, reader );

      vector.push_back( feat );
    }

  } else { 
    rb_raise(rb_eTypeError, "Can't create enhanced descriptors from this type");
  }

  return vector;
}

void init_enhanced_descriptors( Object &rb_mBenchmarking ) {
  Data_Type <EnhancedDescriptors> rc_cED = define_class_under<EnhancedDescriptors>( rb_mBenchmarking, "EnhancedDescriptors" )
    .define_constructor( Constructor<EnhancedDescriptors,SiftFeatureVector>() )
    .define_method( "length", &EnhancedDescriptors::length )
    .define_method( "descriptors_to_mat", &EnhancedDescriptors::descriptors_to_mat );
}
