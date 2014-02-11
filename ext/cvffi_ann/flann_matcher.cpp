
#include <iostream>
#include <vector>
using std::vector;
using std::endl;
using std::cout;

#include <opencv2/core/core_c.h>
#include <opencv2/core.hpp>
using namespace cv;

#include <opencv2/flann.hpp>

#include <rice/Class.hpp>
#include <rice/Constructor.hpp>
using namespace Rice;

#include "descriptors.h"



// Written as a "simplified wrapper" around the OpenCV flann toolset
// mostly because I don't know how (heavily templated) Rice will react 
// to the heavily templated OpenCV stuff..
//
//
class FlannMatcher {
public:

  FlannMatcher();
  ~FlannMatcher() {;}

  vector<DMatch> match( const Mat &query, const Mat &train );

  protected:

  FlannBasedMatcher _matcher;

  };


FlannMatcher::FlannMatcher( void )
{;}

vector<DMatch> FlannMatcher::match( const Mat &query, const Mat &train )
{;}


void init_flann_matcher( Object &rb_mBenchmarking ) {
  Data_Type <FlannMatcher> rc_cFlann = define_class_under<FlannMatcher>( rb_mBenchmarking, "FlannMatcher" )
    .define_constructor( Constructor<FlannMatcher>() )
    .define_method( "match", &FlannMatcher::match );
}
