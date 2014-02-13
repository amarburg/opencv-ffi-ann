require_relative "matcher"

require 'libcvffi_ann'

class FlannMatcher < Matcher

end

class KdtreeFlannMatcher < FlannMatcher

  ID = :kdtree_flann

  def initialize
    super 
  end

  def flann_type
    :KD_TREE
  end

  def match( query, train, opts = {} )

    results = nil
    matcher = CVFFI::ANN::FlannMatcher.new( flann_type )

    @train_time = Benchmark.measure {
      matcher.train train.descriptors_to_mat( :CV_32F ) 
    }
    @match_time = Benchmark.measure { 
      results = matcher.match query.descriptors_to_mat( :CV_32F ) 
    }

    results
  end
end

class KMeansFlannMatcher < KdtreeFlannMatcher
  ID = :kmeans_flann

  def flann_type; :KMEANS; end
end




