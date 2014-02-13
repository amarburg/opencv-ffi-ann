
require_relative "matcher"

require "opencv-ffi-ext"


class BruteForceMatcher < Matcher

  ID = :brute_force
  def initialize( name = ID.to_s, description = nil )
    super
  end

  def matcher
    CVFFI::ANN::BruteForceMatcher.new( :L2 )
  end

  def match( query, train, opts = {} )
    results = nil
    @match_time = Benchmark.measure {
      results = matcher.match( query.descriptors_to_mat( :CV_32F ), 
                              train.descriptors_to_mat( :CV_32F ) )
    }

    results
  end

  def match_opts; {}; end
end

class L2SqrBruteForceMatcher < BruteForceMatcher

  ID = :l2_sqr_brute_force
  def initialize( name = ID.to_s, description = nil )
    super 
  end

  def matcher
    CVFFI::ANN::BruteForceMatcher.new( :L2SQR )
  end

end

class BruteForceRatioMatcher < BruteForceMatcher

  ID = :brute_force_ratio
  def initialize(ratio = 1.4, name = ID.to_s, description = nil)
    super name, description
    @ratio = ratio
  end

  def match_opts; { ratio: @ratio }; end
end


 class EnhancedBruteForceMatcher < BruteForceMatcher

   ID = :enhanced_brute_force

   def initialize( weight, name = ID.to_s, description = nil )
     super name, description
     @weight = weight
    set_description "%s (w=%.2e)" % [name, @weight]
   end

  def match( query, train, opts = {} )
    print_pre ID, "Matching %d features with %d features" % [query.length, train.length] 

    hom = opts[:homography] or raise "Homography not supplied to EnhancedBruteForceMatcher"
    results = nil
    @match_time = Benchmark.measure {
      t = EnhancedDescriptors.new(train,@weight).descriptors_to_mat( :CV_32F )
      q = EnhancedDescriptors.new(query,@weight).warp_descriptors_to_mat( hom, :CV_32F )
      results = matcher.match( q, t )
    }

    puts " .. have %d putative matches" % results.length

    results
  end
end

                
