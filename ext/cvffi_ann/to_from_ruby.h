#include <opencv2/core.hpp>

#include <rice/Class.hpp>

#include "descriptors.h"

#ifndef __TO_FROM_RUBY_H__
#define __TO_FROM_RUBY_H__


template<>
SiftFeatureVector from_ruby<SiftFeatureVector>( Rice::Object obj );

template<>
Rice::Object to_ruby<cv::Mat>( cv::Mat const &m );


#endif
