
require "trollop"
require "benchmark"

require "members"
require "print_now"

require "brute_force_matcher"
require "flann_matcher"
require "match_bundle"

require "opencv-ffi-ext"

class HomographyExperiment

  include FlannExperimentMembers

  attr_accessor :reference
  attr_reader :verbose
  alias :verbose? :verbose

  def initialize( opts = {} )
    @verbose = opts[:verbose] || false

    yield self if block_given?
  end

  def run
    image_pairs.each { |pair|

      features_a = feature_library[pair.a]
      features_b = feature_library[pair.b]
      dmatches = BruteForceMatcher.new.match( features_a, features_b, ratio: 1.4 )

      matches = Matches.new( dmatches, features_a, features_b )

      params = CVFFI::Calib3d::FEstimatorParams.new( max_iters: 10e6,
                                                    confidence: 0.9999,
                                                    outlier_threshold: 3,
                                                    do_refine:  true )

      h_result = CVFFI::Calib3d.estimateHomography( matches.a_mat, matches.b_mat, params )

      puts_pre ID, "Homography calculation required %d iterations, with %d inliers (%.2f%%)" %
        [h_result.num_iters, h_result.count_inliers, 100.0 * h_result.frac_inliers ] if verbose?

      puts "Homography for pair %s - %s" % [pair.a.basename, pair.b.basename]
      h_result.h.print( format: :exp )

    }
  end


  def self.from_opts( argvs = ARGV )

    opts = Trollop::options do
      opt :verbose, "Verbose output", :default => true
    end

    HomographyExperiment.new( opts ) { |exp| yield exp if block_given? }
  end



  ID = :exp
  def dump
    puts_pre ID, "Has %d image %s" % [image_pairs.length,
                                      image_pairs.length == 1 ? "pair" : "pairs" ]

  end

end