
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

    unsigned int num_descriptors( void ) { return _descriptors.rows; }
    unsigned int descriptor_length( void ) { return _descriptors.cols; }

    // TODO:  specifying the type (CV_32F, CV_64F) of the output 
    virtual cv::Mat descriptors_to_mat( int type = 0 );

    const cv::Mat &descriptors() { return _descriptors; }
    
  protected:

    cv::Mat _descriptors;

};

typedef vector<cv::KeyPoint> KeyPointVector;

class ExtendedDescriptors : public Descriptors {

  public:

    ExtendedDescriptors( KeyPointVector vector, const cv::Mat descriptors, double weight = 1.0 );
    virtual ~ExtendedDescriptors( void );

    virtual cv::Mat descriptors_to_mat( int type = 0 );
    ExtendedDescriptors warp_descriptors( const cv::Mat h );

    const KeyPointVector &keypoints() { return _kps; }
    const cv::KeyPoint &keypoint( int idx ) { return _kps[idx]; }

  protected:

    std::vector< cv::KeyPoint > _kps;
    double _weight;
};


#endif
