#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>

#include "matchers.h"

#ifndef __CVRICE_MATCHERS_COV_BRUTE_FORCE_MATCHERS_H__
#define __CVRICE_MATCHERS_COV_BRUTE_FORCE_MATCHERS_H__

namespace CVRice {
  using cv::Point2d;
  using cv::Matx33d;
  using cv::Matx22d;
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
      CovarianceBFMatcher( const Matx33d h, const Mat hcov, float weight = 1.0 );
      virtual ~CovarianceBFMatcher() {;}

      virtual std::vector<GeomDMatch> match( const FeatureSet &query, const FeatureSet &train );

    protected:
      Point2d map_lr( const Point2d &pt );
      Matx22d point_covariance( const Point2d &pt );
      double reproj_distance( const Point2d &q, const Point2d &t );

      std::vector< std::vector<cv::DMatch> > do_match( const Mat &query, const Mat &train );

      Matx33d _h;
      Matx<double, 8, 8> _hcov;
      double _weight;
      int _knnDepth;
  };

  class CovarianceBFRatioMatcher : public CovarianceBFMatcher {
    public:
      CovarianceBFRatioMatcher( const Matx33d h, const Mat hcov, float weight = 1.0, float ratio = 0.0 );
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
