
require "sift_features"

class FeatureLibrary
  attr_accessor :do_normalize

  ID = :feature_library
  def initialize
    @do_normalize = false
    @lib = Hash.new { |h,k|
      features = SiftFeatures::extract_features( k )

      if @do_normalize

        norm = [k.image_size.height, k.image_size.width].max
        puts_pre ID, "Normalizing by %d" % norm
        features.each { |feat|
          feat.x /= norm
          feat.y /= norm
        }
      end

      h[k] = features
    }
  end

  def [](a); @lib[a]; end

  def keys; @lib.keys; end
end

