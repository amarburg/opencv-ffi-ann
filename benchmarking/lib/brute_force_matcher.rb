
require_relative "matcher"

require "opencv-ffi-ext"

class BruteForceMatcher < Matcher

  def initialize(opts = {})
    super opts.merge(name: "brute_force")
  end

  ID = :brute_force
  def match( query, train )
    print_pre ID, "Matching %d features with %d features" % [query.length, train.length] 

    results = CVFFI::Matcher::brute_force_matcher( query.descriptors_to_mat( :CV_32F ), 
                                                  train.descriptors_to_mat( :CV_32F ) )

    puts " .. have %d putative matches" % results.length

    results
  end
end
                  
