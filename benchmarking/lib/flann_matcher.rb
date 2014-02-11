require_relative "matcher"

require 'libcvffi_ann'

class FlannMatcher < Matcher

  ID = :flann

  def initialize( opts = {} )
    super opts
    @name = ID.to_s
  end

  def match( query, train, opts = {} )
    print_pre ID, "Matching %d features with %d features" % [query.length, train.length] 

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

class EnhancedFlannMatcher < FlannMatcher

  ID = :enhanced_flann

  def initialize( weight, opts = {} )
    super opts
    @weight = weight
    @name = ID.to_s
    @description = "%s (w=%f)" % [name, @weight]
  end

  def match( query, train,  opts = {} )
    hom = opts[:homography] or raise "Homography not supplied to EnhancedFlannMatcher."

    print_pre ID, "Matching %d features with %d features" % [query.length, train.length] 

    results = nil
    matcher = CVFFI::ANN::FlannMatcher.new

    @train_time = Benchmark.measure {
      t = EnhancedDescriptors.new(train,@weight).descriptors_to_mat( :CV_32F )
      matcher.train t
    }

    @match_time = Benchmark.measure {
      q = EnhancedDescriptors.new(query,@weight).warp_descriptors_to_mat( hom, :CV_32F )
      results = matcher.match q
    }

    puts " .. have %d putative matches" % results.length

    results
  end
end


