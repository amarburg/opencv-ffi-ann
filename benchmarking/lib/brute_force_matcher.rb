
require_relative "matcher"

require "opencv-ffi-ext"


class BruteForceMatcher < Matcher

  ID = :brute_force
  def initialize( opts = {} )
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
  def initialize( opts = {} )
    super 
  end

  def matcher
    CVFFI::ANN::BruteForceMatcher.new( :L2SQR )
  end

end

class BruteForceRatioMatcher < BruteForceMatcher

  ID = :brute_force_ratio
  def initialize( opts = {} )
    super
    @ratio = opts[:ratio] || 1.4
    set_description "%s (r=%.1f)" % [name, @ratio]
  end

  def match_opts; { ratio: @ratio }; end

  def to_h
    h = super
    h[:opts] = { ratio: @ratio }
    h
  end
end


 class ExtendedBruteForceMatcher < BruteForceMatcher

   ID = :extended_brute_force

   def initialize( weight, opts = {} )
     super opts
     @weight = weight
    set_description "%s (w=%.2e)" % [name, @weight]
   end

  def match( query, train, opts = {} )
    hom = opts[:homography] or raise "Homography not supplied to #{self.class.name}"
    results = nil
    @match_time = Benchmark.measure {
      t = ExtendedDescriptors.new(train,@weight).descriptors_to_mat( :CV_32F )
      q = ExtendedDescriptors.new(query,@weight).warp_descriptors_to_mat( hom, :CV_32F )
      results = matcher.match( q, t )
    }

    results
  end

  def to_h
    h = super
    h[:opts] = { weight: @weight }
    h
  end
end

                
