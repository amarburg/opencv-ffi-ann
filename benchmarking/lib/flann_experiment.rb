
require "trollop"
#require "benchmark"

require "members"
require "print_now"

require "brute_force_matcher"
require "cvffi_brute_force_matcher"
require "flann_matcher"
require "extended_flann_matcher"
require "cvffi_flann_matcher"
require "match_bundle"
require "manual_geometry"

require "libcvffi_ann"

include CVFFI::ANN::Benchmarking


class FlannExperiment

  include FlannExperimentMembers

  attr_accessor :reference
  attr_reader :verbose
  alias :verbose? :verbose

  def initialize( opts = {} )
    @verbose = opts[:verbose] || false

    @reference = nil

    yield self if block_given?
  end

  def run
    image_pairs.each { |pair|

      algorithms.each { |algo|

        features_a = feature_library[pair.a]
        features_b = feature_library[pair.b]

        puts_pre "exp", "Running #{algo.name}" if opts[:verbose]
        matches = algo.match( features_a, features_b, homography: pair.homography )
        matches = Matches.new( matches, features_a, features_b )

        result_db.add( pair, algo, matches, algo.train_time, algo.match_time )
      }

    }


    result_db.calculate( @reference )
  end


  def self.from_opts( argvs = ARGV )

    opts = Trollop::options do
      opt :verbose, "Verbose output", :default => true
    end

    FlannExperiment.new( opts ) { |exp| yield exp if block_given? }
  end



  ID = :exp
  def dump
    puts_pre ID, "Has %d image %s" % [image_pairs.length,
                                      image_pairs.length == 1 ? "pair" : "pairs" ]

  end

end
