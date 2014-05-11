
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>

#include <rice/Object.hpp>

#include <opencv_rice/core/dmatch.h>
#include <opencv_rice/core/keypoint.h>

#include "../feature_set.h"

#ifndef __CVRICE_MATCHERS_DMATCH_H__
#define __CVRICE_MATCHERS_DMATCH_H__

namespace CVRice {

  void init_dmatch( Rice::Module &rb_module );

}

#endif
