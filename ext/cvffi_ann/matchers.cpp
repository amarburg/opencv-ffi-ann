
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
#include <rice/Array.hpp>
using namespace Rice;

#include "descriptors.h"
#include "to_from_ruby.h"


// Written as a "simplified wrapper" around the OpenCV flann toolset
// mostly because I don't know how (heavily templated) Rice will react 
// to the heavily templated OpenCV stuff..
//
// As always, the question is whether this is necessary or if the 
// base class can be wrapped directly...
//
class Matcher {
public:

  Matcher() : _matcher(NULL)  
  {;}
  virtual ~Matcher() 
  { if(_matcher) delete _matcher;}

  virtual void train( const Mat descriptors );
  virtual vector<DMatch> match( const Mat query, const Mat train );
  virtual vector<DMatch> match( const Mat query );

protected:

  DescriptorMatcher *_matcher;

};

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

class L2BruteForceMatcher : public Matcher {
public:
  L2BruteForceMatcher( bool crosscheck = false ) : Matcher()
  {  _matcher = new BFMatcher( NORM_L2, crosscheck ); }
};

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



class L2BruteForceRatioMatcher : public L2BruteForceMatcher {
public:
  L2BruteForceRatioMatcher( float ratio, bool crosscheck = false ) : L2BruteForceMatcher()
  { _ratio = ratio; }

  vector<DMatch> match( const Mat query, const Mat train )
  { return ratio_match( _matcher, query, train, _ratio ); }

protected:
  float _ratio;
};


class L2SqrBruteForceMatcher : public Matcher {
public:
  L2SqrBruteForceMatcher( bool crosscheck = false ) : Matcher()
  {  _matcher = new BFMatcher( NORM_L2SQR, crosscheck ); }
};

class KdTreeFlannMatcher : public Matcher {
public:
  KdTreeFlannMatcher() : Matcher()
  {_matcher = new FlannBasedMatcher( makePtr<flann::KDTreeIndexParams>() ); }
};

class KdTreeFlannRatioMatcher : public KdTreeFlannMatcher {
  public:
    KdTreeFlannRatioMatcher( float ratio ) : KdTreeFlannMatcher()
  { _ratio = ratio; }

    vector<DMatch> match( const Mat query, const Mat train )
    { return ratio_match( _matcher, query, train, _ratio ); }

    vector<DMatch> match( const Mat query )
    { return ratio_match( _matcher, query, _ratio ); }

  protected:
  float _ratio;
};

class KMeansFlannMatcher : public Matcher {
public:
  KMeansFlannMatcher() : Matcher()
  {_matcher = new FlannBasedMatcher( makePtr<flann::KMeansIndexParams>() ); }
};

class KMeansFlannRatioMatcher : public KMeansFlannMatcher {
  public:
    KMeansFlannRatioMatcher( float ratio ) : KMeansFlannMatcher()
  { _ratio = ratio; }

    vector<DMatch> match( const Mat query, const Mat train )
    { return ratio_match( _matcher, query, train, _ratio ); }

    vector<DMatch> match( const Mat query )
    { return ratio_match( _matcher, query, _ratio ); }

  protected:
  float _ratio;
};
//================================================


int dmatch_queryIdx( const DMatch &dm ) { return dm.queryIdx; }
int dmatch_trainIdx( const DMatch &dm ) { return dm.trainIdx; }
int dmatch_imgIdx( const DMatch &dm ) { return dm.imgIdx; }
float dmatch_distance( const DMatch &dm ) { return dm.distance; }

  template<>
Object to_ruby< vector<DMatch> >( vector<DMatch> const &matches )
{
  Array out;
  for( vector<DMatch>::const_iterator itr = matches.begin(); itr != matches.end(); itr++ )
  {
    out.push( to_ruby( *itr ) );
  }

  return out;
}

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

void init_flann_matcher( Object &rb_module ) {
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

  Data_Type <DMatch> rc_cDMatch = define_class_under<cv::DMatch>( rb_module, "DMatch" )
    .define_constructor( Constructor<cv::DMatch,int,int,float>() )
    .define_method( "queryIdx", &dmatch_queryIdx )
    .define_method( "trainIdx", &dmatch_trainIdx )
    .define_method( "distance", &dmatch_distance )
    .define_method( "imgIdx", &dmatch_imgIdx );


}
