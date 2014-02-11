
require_relative "matcher"

require "opencv-ffi-ext"


class BruteForceMatcher < Matcher

  def initialize(opts = {})
    super opts
    @name = ID.to_s
  end

  def matcher
    CVFFI::ANN::BruteForceMatcher.new( :L2 )
  end

  ID = :brute_force
  def match( query, train, opts = {} )
    print_pre ID, "Matching %d features with %d features" % [query.length, train.length] 

    results = nil
    @match_time = Benchmark.measure {
      results = matcher.match( query.descriptors_to_mat( :CV_32F ), 
                                  train.descriptors_to_mat( :CV_32F ) )
    }

    puts " .. have %d putative matches" % results.length

    results
  end
end

class L2SqrBruteForceMatcher < BruteForceMatcher

  ID = :l2_sqr_brute_force
  def initialize(opts = {})
    super opts
    @name = ID.to_s
  end

  def matcher
    CVFFI::ANN::BruteForceMatcher.new( :L2SQR )
  end

end
                 
