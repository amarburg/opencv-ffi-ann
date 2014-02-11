require "image_pairs"
require "feature_library"
require "result_db"

module FlannExperimentMembers

  def image_pairs
    @image_pairs ||= ImagePairs.new
  end

  def add_pair( a, b, opts = {} )
    image_pairs.add( a, b, opts )
  end

  def feature_library
    @feature_lib ||= FeatureLibrary.new
  end

  def algorithms
    @algorithms ||= []
  end

def result_db
  @result_db ||= ResultDb.new
end


end