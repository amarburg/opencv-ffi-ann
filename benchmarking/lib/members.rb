require "image_pairs"

module FlannExperimentMembers

  def image_pairs
    @image_pairs ||= ImagePairs.new
  end

  def add_pair( a, b )
    image_pairs.add( a, b )
  end

  def feature_library
    @feature_lib ||= FeatureLibrary.new
  end

end
