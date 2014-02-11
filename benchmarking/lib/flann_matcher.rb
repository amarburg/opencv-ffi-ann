require_relative "matcher"

require 'libcvffi_ann'

class FlannMatcher < Matcher

  ID = :flann

  def initialize( opts = {} )
    super opts
    @name =  "kdtree_flann"
  end

  def flann_type
    :KD_TREE
  end

  def match( query, train, opts = {} )
    print_pre ID, "Matching %d features with %d features" % [query.length, train.length] 

    results = nil
    matcher = CVFFI::ANN::FlannMatcher.new( flann_type )

    @train_time = Benchmark.measure {
      matcher.train train.descriptors_to_mat( :CV_32F ) 
    }
    @match_time = Benchmark.measure { 
      results = matcher.match query.descriptors_to_mat( :CV_32F ) 
    }

    puts " .. have %d putative matches" % results.length

    results
  end
end

class KMeansFlannMatcher < FlannMatcher
  def initialize( opts = {} )
    super opts
    @name = "kmeans_flann"
  end

  def flann_type; :KMEANS; end
end




