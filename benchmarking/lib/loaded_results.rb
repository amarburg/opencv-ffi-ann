require 'result_common'

module Benchmark
  class Tms
    def self.from_h( h )
      return nil unless h

      # to_a and the constructor have arguments in slightly different order
      # to_a starts with the label, constructor ends with the label
      Tms.new( *(h["tms"].rotate) )
    end

    def to_h
      {tms: to_a }
    end
  end
end

class LoadedResults
  attr_reader :results

  def initialize( results = [] )
    @results = results
  end

  def +(b)
    LoadedResults.new (@results + b.results)
  end

  def print
    @results.each(&:print)
  end

  def self.from_h( h )
    raise "Can't find h[\"results\"]" unless h["results"]

    results = h["results"].map { |result|
      LoadedResult.from_h result
    }

    LoadedResults.new results
  end

  def self.from_file( file )
    from_h JSON.parse File.read file 
  end
end

class LoadedResult
  include ResultCommon

  attr_reader :pair, :algo, :num_matches
  attr_reader :train_time, :match_time

  def initialize( pair, algo, opts )
    @pair = pair
    @algo = algo

    @num_matches = opts[:num_matches]
    @num_inliers = opts[:num_inliers]
    @accuracy =    opts[:accuracy]
    @match_time = opts[:match_time]
    @train_time = opts[:train_time]
  end

  def print
    puts "%40s %20s %10s   % 6d % 4.2f  % 7.2f  %12s %12s %12s" % 
      [ algo.description, pair.name, pair.hom.name,
        num_matches, pct_inliers, pct_accuracy,
        (train_time ? ("% 8d" % (train_time.total*1e3)) : "--"),
        (match_time ? ("% 8d" % (match_time.total*1e3)) : "--"),
        ("% 8d" % (total_time.total*1e3))
    ]
  end

  def self.from_h( h )
    pair = LoadedPair.from_h( h["pair"] )
    algo = LoadedAlgorithm.from_h( h["algo"] )
    opts = {}
    opts[:num_matches] = h["num_matches"]
    opts[:num_inliers] = h["num_inliers"]
    opts[:accuracy]    = h["accuracy"]
    opts[:match_time]  = Benchmark::Tms::from_h( h["match_time"] )
    opts[:train_time]  = Benchmark::Tms::from_h( h["train_time"] )

    LoadedResult.new( pair, algo, opts )
  end

end

class LoadedPair
  attr_reader :a, :b, :hom
  def initialize( a, b, homography )
    @a =a
    @b = b
    @hom = homography
  end

  def name
      "%s--%s" % [a,b]
  end

  def self.from_h( h )
    LoadedPair.new h["a"], h["b"], LoadedHomography.from_h( h["homography"] )
  end
end

class LoadedAlgorithm
  attr_reader :name, :description
  def initialize( name, description )
  end

  def self.from_h( h )
    LoadedAlgorithm.new h["name"], h["description"]
  end
end

class LoadedHomography
  attr_reader :name
  def initialize( name, h )
    @name = name
    @mat = Matrix.rows h
  end

  def self.from_h( h )
    case h.keys.first
    when "Homography"
      val = h.values.first
      LoadedHomography.new val["name"], val["h"] 
    when "PerturbedHomography"
      val = h.values.first
      LoadedHomography.new val["name"], val["h"] 
    else
      puts "Don't know how to deal with a homography of type \"#{h.keys.first}\""
    end
  end
end
  


