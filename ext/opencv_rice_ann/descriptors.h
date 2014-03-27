
#include <vector>
using std::vector;

#include <opencv2/core.hpp>
#include <opencv_rice/core/core.h>

#ifndef __DESCRIPTORS_H__
#define __DESCRIPTORS_H__

class Descriptors {
  public:

    Descriptors( const cv::Mat descriptors );
    virtual ~Descriptors( void );

    int length( void ) { return _descriptors.rows; }
    int descriptor_length( void ) { return _descriptors.cols; }

    // TODO:  specifying the type (CV_32F, CV_64F) of the output 
    virtual cv::Mat descriptors_to_mat( int type = 0 );
    
  protected:

    cv::Mat _descriptors;

};

typedef vector<cv::KeyPoint> KeyPointVector;

class ExtendedDescriptors : public Descriptors {
  public:

    ExtendedDescriptors( KeyPointVector vector, const Mat descriptors, double weight = 1.0 );
    virtual ~ExtendedDescriptors( void );

    virtual Mat descriptors_to_mat( int type = 0 );
    cv::Mat warp_descriptors_to_mat( const cv::Mat h, int type );

  protected:

    std::vector< cv::KeyPoint > _kps;
    double _weight;
};


#endif
