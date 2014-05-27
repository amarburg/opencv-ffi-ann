
#include <iostream>
#include <vector>
#include <iostream>
using std::vector;
using std::endl;
using std::cout;

#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
using namespace cv;

#include <rice/Module.hpp>
using namespace Rice;

#include "dmatch.h"

#include <opencv_rice/core/keypoint.h>
#include <opencv_rice/calib3d/reproj.h>


namespace CVRiceMatchers {
  
  using namespace CVRice;

  Mat dmatches_to_mat( const DMatchVector dmatches, const KeyPointVector kps, int which )
  {
    if( (which != 0) and (which != 1) )
      rb_raise( rb_eArgError, "dmatches_to_mat takes 0 or 1" );

    Mat out( dmatches.size(), 2, CV_64F );
    double *dbl = out.ptr<double>(0);
    int count = 0;
    for( DMatchVector::const_iterator itr = dmatches.begin(); itr != dmatches.end(); ++itr, count+=2 ) {
      int idx = (which==0 ? (*itr).queryIdx : (*itr).trainIdx );

      dbl[count]   = kps[idx].pt.x;
      dbl[count+1] = kps[idx].pt.y;
    }
    return out;
  }

  DMatchVector dmatches_select_inliers_by_h( const DMatchVector dmatches, 
      const KeyPointVector query, 
      const KeyPointVector train, 
      const Mat &h, const double threshold )
  {
    if( dmatches.size() == 0 ) return DMatchVector();

    DMatchVector out;

    Mat query_mat = dmatches_to_mat( dmatches, query, 0 ),
        train_mat = dmatches_to_mat( dmatches, train, 1 );
    Mat reproj_error = CVRice::cvHMaxReprojError( query_mat, train_mat, h );

    for( unsigned int i = 0; i < dmatches.size(); ++i ) {
      if( reproj_error.at<double>(i,0) < threshold ) 
        out.push_back( dmatches[i] );
    }

    return out;
  }

  DMatchVector dmatches_select_inliers_by_mask( const DMatchVector dmatches, const Mat &mask )
  {
    if( dmatches.size() == 0 ) return DMatchVector();
    if( dmatches.size() == 4 ) return dmatches;
    
    // If there's no solution, findHomography will return mask unchanged...
    if( mask.rows == 0 && mask.cols == 0 ) return DMatchVector();

    Mat mask8u;
    mask.convertTo( mask8u, CV_8U );
    if( mask.type() != CV_8U ) rb_raise( rb_eArgError, "Inlier mask not correct type (%d).", mask.type() );

    DMatchVector out;
    for( unsigned int i = 0; i < dmatches.size(); ++i ) {
      if( mask8u.at<unsigned char>(i,0) != 0 ) 
        out.push_back( dmatches[i] );
    }

    return out;
  }

  void init_dmatch_functions( Module &rb_module ) {
    rb_module.define_module_function( "dmatches_to_mat", &dmatches_to_mat )
      .define_module_function( "dmatches_select_by_h", &dmatches_select_inliers_by_h )
      .define_module_function( "dmatches_select_inliers_by_h", &dmatches_select_inliers_by_h )
      .define_module_function( "dmatches_select_by_mask", &dmatches_select_inliers_by_mask )
      .define_module_function( "dmatches_select_inliers_by_mask", &dmatches_select_inliers_by_mask );
 }

};
