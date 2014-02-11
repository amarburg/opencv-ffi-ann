require_relative "matcher"

require 'libcvffi_ann'

class FlannMatcher < Matcher

  ID = :flann

  def initialize( opts = {} )
    super opts
    @name = "flann"
  end

  def match( query, train, opts = {} )
    print_pre ID, "Matching %d features with %d features" % [query.length, train.length] 

    # Apparently FLANN only takes floats
    
    results = nil
    @match_time = Benchmark.measure { 
    results = CVFFI::Matcher::flann_matcher( query.descriptors_to_mat( :CV_32F ), 
                                            train.descriptors_to_mat( :CV_32F ) )
    }

    puts " .. have %d putative matches" % results.length

    results
  end
end

class EnhancedFlannMatcher < FlannMatcher

  ID = :enhanced_flann

  def initialize( weight, opts = {} )
    super opts
    @weight = weight
    @name = "enhanced_flann"
    @description = "enhanced_flann (w=%f)" % @weight
  end

  def match( query, train,  opts = {} )
    hom = opts[:homography] or raise "Homography not supplied to EnhancedFlannMatcher."

    print_pre ID, "Matching %d features with %d features" % [query.length, train.length] 

    results = nil
    @match_time = Benchmark.measure {
      q = EnhancedDescriptors.new(query,@weight).warp_descriptors_to_mat( hom, :CV_32F )
      t = EnhancedDescriptors.new(train,@weight).descriptors_to_mat( :CV_32F )

      # Apparently FLANN only takes floats
      results = CVFFI::Matcher::flann_matcher( q,t )
    }

    puts " .. have %d putative matches" % results.length

    results
  end
end


class RiceFlannMatcher < FlannMatcher
  ID = :rice_flann
  def initialize( opts = () )
    super opts
    @name = "rice_flann"
  end

  def match( query, train, opts = {} )
    print_pre ID, "Matching %d features with %d features" % [query.length, train.length] 

    # Apparently FLANN only takes floats

    results = nil
    matcher = CVFFI::ANN::FlannMatcher.new

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
