
require "sift_features"

class FeatureLibrary

  def initialize
    @lib = Hash.new { |h,k|
      h[k] = SiftFeatures::extract_features( k )
    }
  end

  def [](a); @lib[a]; end

  def keys; @lib.keys; end
end

