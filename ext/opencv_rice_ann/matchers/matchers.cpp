
#include <iostream>
#include <vector>
using std::vector;
using std::endl;
using std::cout;

#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
using namespace cv;

#include <rice/Class.hpp>
#include <rice/Constructor.hpp>
#include <rice/Array.hpp>
using namespace Rice;

#include "descriptors.h"
#include "to_from_ruby.h"

#include "matchers.h"
#include "brute_force_matchers.h"
#include "flann_matchers.h"
#include "cov_brute_force_matchers.h"
#include "only_geometry_matcher.h"
using namespace CVRice;

#include <iostream>

#include <ruby.h>

void Matcher::train( const Mat descriptors )
{

//  cout << "Train: " << endl;
//        for( unsigned int i = 0; i < descriptors.cols; ++i ) {
//          cout << descriptors.at<float>(0,i) << ' ';
//        }
//        cout << endl;

  vector<Mat> train_desc;
  train_desc.push_back( descriptors );

  _matcher->add( train_desc );
  _matcher->train();
}

vector<DMatch> Matcher::match( const Mat query, const Mat train )
{
  vector<DMatch> matches;
  _matcher->match( query, train, matches );
  return matches;
}

vector<DMatch> Matcher::match( const Mat query )
{
  vector<DMatch> matches;
  _matcher->match( query, matches );
  return matches;
}

static vector<DMatch> filter_ratio_matches( vector< vector<DMatch> > const &match_pairs, float ratio )
{
  vector<DMatch> keep;
  for( vector<vector<DMatch> >::const_iterator itr = match_pairs.begin(); itr != match_pairs.end(); itr++ ) {
    const vector<DMatch> &pair( *itr );
    bool do_keep = pair[1].distance > (ratio * pair[0].distance);
    //printf("%f %f %s\n", pair[1].distance, pair[0].distance, (do_keep ? "yes" : "no") );
    if( do_keep ) keep.push_back(pair[0]);
  }
  return keep;
}

vector<DMatch> Matcher::ratio_match( const Mat query, float ratio )
{
  vector<vector<DMatch> > match_pairs;
  _matcher->knnMatch( query, match_pairs, 2 );
  return filter_ratio_matches( match_pairs, ratio );
}

vector<DMatch> Matcher::ratio_match( const Mat query, const Mat train, float ratio )
{
  vector<vector<DMatch> > match_pairs;

//  cout << "Ratio_match: " << endl;
//        for( unsigned int i = 0; i < query.cols; ++i ) {
//          cout << query.at<float>(0,i) << ' ';
//        }
//        cout << endl;

  _matcher->knnMatch( query, train, match_pairs, 2 );
  return filter_ratio_matches( match_pairs, ratio );
}


KeyPointVector feature_set_kps( const FeatureSet &set ) { return set.kps; }
cv::Mat feature_set_desc( const FeatureSet &set ) { return set.desc; }
void FeatureSet::apply_intrinsics( const Matx33f &k )
{
  Matx33f kinv = k.inv();

  for( KeyPointVector::iterator itr = kps.begin(); itr != kps.end(); ++itr ) {
    Vec3f v( (*itr).pt.x, (*itr).pt.y, 1 );
    Vec3f mapped = kinv * v;

    (*itr).pt.x = mapped[0]/mapped[2];
    (*itr).pt.y = mapped[1]/mapped[2];
  }
}


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


//==============
//
// From the Rice documentation
typedef vector<DMatch> (Matcher::*train_match)( const Mat, const Mat );
typedef vector<DMatch> (Matcher::*match_using_existing)( const Mat);

template<class T>
void define_bf_matcher( Object  &rb_module, const char *name )
{
  define_class_under<T,Matcher>( rb_module, name )
    .define_constructor( Constructor<T,bool>(), Arg("crosscheck") = false )
    .define_method( "match", train_match(&Matcher::match) );
}

template<class T>
void define_train_matcher(Object &rb_module, const char *name )
{
  define_class_under<T,Matcher>( rb_module, name )
    .define_constructor( Constructor<T>() )
    .define_method( "train", &Matcher::train )
    .define_method( "match", match_using_existing(&Matcher::match) )
    .define_method( "train_match", train_match(&Matcher::match) );
}

void init_matchers( Module &rb_module ) {
  //  Data_Type <cv::Mat> rc_cMat     = define_class_under<cv::Mat>( rb_module, "Mat" );

  rb_module.define_module_function( "dmatches_to_mat", &dmatches_to_mat );

  Data_Type <Matcher> rc_cMatcher = define_class_under<Matcher>( rb_module, "Matcher" )
    .define_method( "match", match_using_existing(&Matcher::match) )
    .define_method( "train_match", train_match(&Matcher::match) );

  define_bf_matcher<L2BruteForceMatcher>( rb_module, "L2BruteForceMatcher" );
  define_bf_matcher<L2SqrBruteForceMatcher>( rb_module, "L2SqrBruteForceMatcher" );

  define_class_under<L2BFRatioMatcher,L2BruteForceMatcher>( rb_module, "L2BFRatioMatcher" )
    .define_constructor( Constructor<L2BFRatioMatcher,float,bool>(), (Arg("ratio"), Arg("crosscheck") = false) )
    .define_method( "match", train_match(&Matcher::match) );

  define_class_under<L2SqrBFRatioMatcher,L2SqrBruteForceMatcher>( rb_module, "L2SqrBFRatioMatcher" )
    .define_constructor( Constructor<L2SqrBFRatioMatcher,float,bool>(), (Arg("ratio"), Arg("crosscheck") = false) )
    .define_method( "match", train_match(&Matcher::match) );

  define_train_matcher<KdTreeFlannMatcher>( rb_module, "KdTreeFlannMatcher" );
  define_class_under<KdTreeFlannRatioMatcher,KdTreeFlannMatcher>( rb_module, "KdTreeFlannRatioMatcher")
    .define_constructor( Constructor<KdTreeFlannRatioMatcher,float>() )
    .define_method( "train", &Matcher::train )
    .define_method( "match", match_using_existing(&Matcher::match) )
    .define_method( "train_match", train_match(&Matcher::match) );

  define_train_matcher<KMeansFlannMatcher>( rb_module, "KMeansFlannMatcher" );
  define_class_under<KMeansFlannRatioMatcher,KMeansFlannMatcher>( rb_module, "KMeansFlannRatioMatcher")
    .define_constructor( Constructor<KMeansFlannRatioMatcher,float>() )
    .define_method( "train", &Matcher::train )
    .define_method( "match", match_using_existing(&Matcher::match) )
    .define_method( "train_match", train_match(&Matcher::match) );



  define_class_under<CovarianceBFMatcher>( rb_module, "CovarianceBFMatcher" )
    .define_constructor( Constructor<CovarianceBFMatcher,Matx33f,Mat,float>())
    .define_method( "match", &CovarianceBFMatcher::match );

  define_class_under<CovarianceBFRatioMatcher,CovarianceBFMatcher>( rb_module, "CovarianceBFRatioMatcher" )
    .define_constructor( Constructor<CovarianceBFRatioMatcher,Matx33f,Mat,float,float>())
    .define_method( "match", &CovarianceBFRatioMatcher::match );

  define_class_under<OnlyGeometryMatcher>( rb_module, "OnlyGeometryMatcher" )
    .define_constructor( Constructor<OnlyGeometryMatcher,Matx33f,float>())
    .define_method( "match", &OnlyGeometryMatcher::match );


  define_class_under<FeatureSet>( rb_module, "FeatureSet" )
    .define_constructor( Constructor<FeatureSet,KeyPointVector,Mat>() )
    .define_method("kps", &feature_set_kps )
    .define_method("descriptors", &feature_set_desc )
    .define_method("apply_intrinsics", &FeatureSet::apply_intrinsics );

  define_class_under<GeomDMatch, DMatch>( rb_module, "GeomDMatch" )
    .define_method( "geom_distance", &get_geomdmatch_geomdistance )
    .define_method( "desc_distance", &get_geomdmatch_descdistance )
    .define_method( "weight", &get_geomdmatch_weight );
}
