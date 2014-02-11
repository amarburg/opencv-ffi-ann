
#include <vector>
using std::vector;
#include <opencv-ffi/sift/sift.h>

#ifndef __DESCRIPTORS_H__
#define __DESCRIPTORS_H__

typedef feature                  SiftFeature;
typedef std::vector<SiftFeature> SiftFeatureVector;

class Descriptors {
  public:

    Descriptors( SiftFeatureVector vector );
    virtual ~Descriptors( void );

    int length( void ) { return _features.size(); }

    // TODO:  specifying the type (CV_32F, CV_64F) of the output 
    virtual Mat descriptors_to_mat( Symbol foo );
    
  protected:

    SiftFeature first( void ) { return *(_features.begin()); }

    SiftFeatureVector _features;

};

class EnhancedDescriptors : public Descriptors {
  public:

    EnhancedDescriptors( SiftFeatureVector vector, double weight = 1.0 );
    virtual ~EnhancedDescriptors( void );

    virtual Mat descriptors_to_mat( Symbol foo );

  protected:

    double _weight;
};


#endif
