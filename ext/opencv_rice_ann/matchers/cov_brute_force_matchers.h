#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>

#include "matchers.h"

#ifndef __CVRICE_MATCHERS_COV_BRUTE_FORCE_MATCHERS_H__
#define __CVRICE_MATCHERS_COV_BRUTE_FORCE_MATCHERS_H__

namespace CVRice {
  using cv::Point2f;
  using cv::Mat;
  using cv::Matx;
  using cv::Matx33d;
  using cv::Matx33f;
  using cv::Matx22f;
  using cv::DMatch;


  class GeomDMatch : public cv::DMatch {
    public:

      GeomDMatch( const DMatch &dm, float _totalDistance, float _geomDistance, float _weight )
        : cv::DMatch( dm ), descDistance( dm.distance ), geomDistance( _geomDistance ), weight( _weight )
      { distance = _totalDistance; }

      float descDistance;
      float geomDistance;
      float weight;
  };

  inline float get_geomdmatch_descdistance( const GeomDMatch dm ) { return dm.descDistance; };
  inline float get_geomdmatch_geomdistance( const GeomDMatch dm ) { return dm.geomDistance; };
  inline float get_geomdmatch_weight( const GeomDMatch dm ) { return dm.weight; };


  class CovarianceBFMatcher  {
    public:
      CovarianceBFMatcher( const Matx33f h, const Mat hcov, float weight = 1.0 );
      virtual ~CovarianceBFMatcher() {;}

      virtual std::vector<GeomDMatch> match( const FeatureSet &query, const FeatureSet &train );

      // TODO.  The right thing to do is make this friend to the test class
      // The simple thing to do is make it public.
      Matx22f point_covariance( const Point2f pt );

    protected:
      Point2f map_lr( const Point2f pt );
      cv::Vec2f error( const Point2f &qmapped, const Point2f &t );
      float reproj_distance( const cv::Vec2f &err, const Matx22f &qcov );

      std::vector< std::vector<cv::DMatch> > do_match( const Mat &query, const Mat &train );

      Matx33f _h;
      Matx<float, 8, 8> _hcov;

      float _weight;
      int _knnDepth;
  };

  class CovarianceBFRatioMatcher : public CovarianceBFMatcher {
    public:
      CovarianceBFRatioMatcher( const Matx33f h, const Mat hcov, float weight = 1.0, float ratio = 0.0 );
      virtual ~CovarianceBFRatioMatcher() {;}

      virtual std::vector<GeomDMatch> match( const FeatureSet &query, const FeatureSet &train );

    protected:
      float _ratio;
  };

};


template<>
  inline
Rice::Object to_ruby< std::vector<CVRice::GeomDMatch> >( std::vector<CVRice::GeomDMatch> const &matches )
{
  Rice::Array out;
  for( std::vector<CVRice::GeomDMatch>::const_iterator itr = matches.begin(); itr != matches.end(); itr++ )
  {
    out.push( to_ruby( *itr ) );
  }

  return out;
}



#endif
