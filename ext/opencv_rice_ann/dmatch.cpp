
#include <iostream>
#include <vector>
#include <iostream>
using std::vector;
using std::endl;
using std::cout;

#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
using namespace cv;

#include <rice/Class.hpp>
#include <rice/Array.hpp>
using namespace Rice;

#include <ruby.h>

#include <opencv-rice/calibd3d/reproj.h>

namespace CVRice {

  Mat dmatches_to_mat( const vector<DMatch> dmatches, const KeyPointVector kps, int which )
  {
    if( (which != 0) and (which != 1) )
      rb_raise( rb_eArgError, "dmatches_to_mat takes 0 or 1" );

    Mat out( dmatches.size(), 2, CV_64F );
    double *dbl = out.ptr<double>(0);
    int count = 0;
    for( vector<DMatch>::const_iterator itr = dmatches.begin(); itr != dmatches.end(); ++itr, count+=2 ) {
      int idx = (which==0 ? (*itr).queryIdx : (*itr).trainIdx );

      dbl[count]   = kps[idx].pt.x;
      dbl[count+1] = kps[idx].pt.y;
    }
    return out;
  }

  vector<DMatch> dmatch_select_by_inliers( const vector<DMatch> &dmatches, const KeyPointVector, &query, const KeyPointVector &train, const Mat &h, const double threshold )
  {
    vector<DMatch> out;

    Mat query_mat = dmatches_to_mat( dmatches, query, 0 ),
        train_mat = dmatches_to_mat( dmatches, train, 0 );
    Mat reproj_error = cvHMaxReprojError( query_mat, train_mat, h );

    for( unsigned int i = 0; i < dmatches.length(); ++i ) {
      if( reproj_error.at<double>(i,0) < threshold ) 
        out.push_back( dmatches[i] );
    }

    return out;
  }

  vector<DMatch> dmatch_select_by_mask( const vector<DMatch> &dmatches, const Mat &mask )
  {
    vector<DMatch> out;
    for( unsigned int i = 0; i < dmatches.length(); ++i ) {
      if( mask.at<unsigned char>(i,0) != 0 ) 
        out.push_back( dmatches[i] );
    }

    return out;
  }

  void init_dmatch( Module &rb_module ) {
    //  Data_Type <cv::Mat> rc_cMat     = define_class_under<cv::Mat>( rb_module, "Mat" );

    rb_module.define_module_function( "dmatches_to_mat", &dmatches_to_mat )
      .define_module_function( "dmatch_select_by_inliers", &dmatch_select_by_inliers )
      .define_module_function( "dmatch_select_by_mask", &dmatch_select_by_mask );
 }

};


