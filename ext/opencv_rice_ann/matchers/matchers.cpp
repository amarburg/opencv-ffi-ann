
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

void Matcher::train( const Mat descriptors )
{
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

//==============

vector<DMatch> ratio_match( DescriptorMatcher *matcher, const Mat query, float ratio )
{
  vector<vector<DMatch> > match_pairs;
  vector<DMatch> keep;
  matcher->knnMatch( query, match_pairs, 2 );

  for( vector<vector<DMatch> >::iterator itr = match_pairs.begin(); itr != match_pairs.end(); itr++ ) {
    vector<DMatch> &pair( *itr );
    bool do_keep = pair[1].distance > (ratio * pair[0].distance);
//printf("%f %f %s\n", pair[1].distance, pair[0].distance, (do_keep ? "yes" : "no") );
    if( do_keep ) keep.push_back(pair[0]);
  }
  return keep;
}

vector<DMatch> ratio_match( DescriptorMatcher *matcher, const Mat query, const Mat train, float ratio )
{
  vector<vector<DMatch> > match_pairs;
  vector<DMatch> keep;
  matcher->knnMatch( query, train, match_pairs, 2 );

  for( vector<vector<DMatch> >::iterator itr = match_pairs.begin(); itr != match_pairs.end(); itr++ ) {
    vector<DMatch> &pair( *itr );
    bool do_keep = pair[1].distance > (ratio * pair[0].distance);
    //printf("%f %f %s\n", pair[1].distance, pair[0].distance, (do_keep ? "yes" : "no") );
    if( do_keep ) keep.push_back(pair[0]);
  }
  return keep;
}


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

void init_matchers( Object &rb_module ) {
  //  Data_Type <cv::Mat> rc_cMat     = define_class_under<cv::Mat>( rb_module, "Mat" );

  Data_Type <Matcher> rc_cMatcher = define_class_under<Matcher>( rb_module, "Matcher" )
    .define_method( "match", match_using_existing(&Matcher::match) )
    .define_method( "train_match", train_match(&Matcher::match) );

  define_bf_matcher<L2BruteForceMatcher>( rb_module, "L2BruteForceMatcher" );
  define_bf_matcher<L2SqrBruteForceMatcher>( rb_module, "L2SqrBruteForceMatcher" );

  define_class_under<L2BruteForceRatioMatcher,L2BruteForceMatcher>( rb_module, "L2BruteForceRatioMatcher" )
    .define_constructor( Constructor<L2BruteForceRatioMatcher,float,bool>(), (Arg("ratio"), Arg("crosscheck") = false) )
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

}
