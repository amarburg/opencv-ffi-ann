
require_relative "matcher"

require "opencv-ffi-ext"

class BruteForceMatcher < Matcher

  def initialize(opts = {})
    super opts
    @name = "brute_force"
  end

  ID = :brute_force
  def match( query, train, opts = {} )
    print_pre ID, "Matching %d features with %d features" % [query.length, train.length] 

    results = nil
    @match_time = Benchmark.measure {
      results = CVFFI::Matcher::brute_force_matcher( query.descriptors_to_mat( :CV_32F ), 
                                                    train.descriptors_to_mat( :CV_32F ), opts )
    }

    puts " .. have %d putative matches" % results.length

    results
  end
end
                  
