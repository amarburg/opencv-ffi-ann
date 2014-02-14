require_relative "matcher"

require 'libcvffi_ann'

class FlannMatcher < Matcher
end

class KdTreeFlannMatcher < FlannMatcher

  ID = :kdtree_flann

  def flann_type
    :KD_TREE
  end

  def make_matcher
    CVFFI::ANN::FlannMatcher.new( flann_type )
  end

  def match( query, train, opts = {} )

    results = nil
    
matcher = make_matcher
    @train_time = Benchmark.measure {
      matcher.train train.descriptors_to_mat( :CV_32F ) 
    }
    @match_time = Benchmark.measure { 
      results = matcher.match query.descriptors_to_mat( :CV_32F ) 
    }

    results
  end
end

class KMeansFlannMatcher < KdTreeFlannMatcher
  ID = :kmeans_flann

  def flann_type; :KMEANS; end
end

class KdTreeFlannRatioMatcher < KdTreeFlannMatcher
  def initialize( ratio = 1.4 )
    @ratio = ratio
  end

  def make_matcher
    CVFFI::ANN::KdTreeFlannRatioMatcher.new( @ratio )
  end

  def to_h
    h = super
    h[:opts] = { ratio: @ratio }
    h
  end
end

class KMeansFlannRatioMatcher < KMeansFlannMatcher
  def initialize( ratio = 1.4 )
    @ratio = ratio
  end

  def make_matcher
    CVFFI::ANN::KMeansFlannRatioMatcher.new( @ratio )
  end
  def to_h
    h = super
    h[:opts] = { ratio: @ratio }
    h
  end
end


