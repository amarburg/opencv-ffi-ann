
require "trollop"
require "members"
require "print_now"


class FlannExperiment

  attr_reader :verbose
  alias :verbose? :verbose

  def initialize( opts = {} )
    @verbose = opts[:verbose] || false

    yield self if block_given?
  end

  def self.from_opts( argvs = ARGV )

    opts = Trollop::options do
      opt :verbose, "Verbose output", :default => true
    end

    FlannExperiment.new( opts ) { |exp| yield exp if block_given? }
  end

  include FlannExperimentMembers


  ID = :exp
  def dump
    puts_pre ID, "Has %d image %s" % [image_pairs.length,
                                      image_pairs.length == 1 ? "pair" : "pairs" ]

  end

end
