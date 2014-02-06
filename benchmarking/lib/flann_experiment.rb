
require "trollop"
require "benchmark"

require "members"
require "print_now"

require "brute_force_matcher"
require "flann_matcher"


class FlannExperiment

  include FlannExperimentMembers

  attr_reader :verbose
  alias :verbose? :verbose

  def initialize( opts = {} )
    @verbose = opts[:verbose] || false

    yield self if block_given?
  end

  def run

    image_pairs.each { |pair|

      algorithms.each { |algo|

        matches = nil
        tms = Benchmark::measure { 
          matches = algo.match( feature_library[pair.a], 
                               feature_library[pair.b] )
        }

        result_db.add( pair, algo, matches, tms )

      }

    }
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
