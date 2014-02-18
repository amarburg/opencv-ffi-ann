
require "sift_features"

class FeatureLibrary
  attr_accessor :do_normalize

  ID = :feature_library
  def initialize
    @do_normalize = false
    @lib = Hash.new { |h,img|
      features = SiftFeatures::extract_features( img )

#      if img.intrinsics
#        puts_pre ID, "Applying intrinsic calibration"
#
#        height = img.image_size.height
#        features.each { |feat|
#          f = img.kinv * Vector[feat.x, feat.y, 1.0]
#
#          feat.x = f[0] / f[2]
#          feat.y = f[1] / f[2]
#        }
#
      if @do_normalize

        norm = [img.image_size.height, img.image_size.width].max
        puts_pre ID, "Normalizing by %d" % norm
        features.each { |feat|
          feat.x /= norm
          feat.y /= norm
        }
      end

      h[img] = features
    }
  end

  def [](a); @lib[a]; end

  def keys; @lib.keys; end
end

