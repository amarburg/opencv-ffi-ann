require 'delegate'
require 'json'
require 'loaded_results'
require 'result_common'

class Result
  attr_reader :pair, :algo, :matches
  attr_reader :num_inliers, :accuracy
  attr_reader :train_time, :match_time

  include ResultCommon

  def initialize( pair, algo, matches, train_time, match_time )
    @pair = pair
    @algo = algo
    @matches = matches
    @train_time = train_time
    @match_time = match_time
  end

  def num_matches; @matches.length; end

  def calculate_residuals( h )
    if @matches.length > 0
      xp = Matrix.rows @matches.map { |pts| [pts.second.x, pts.second.y] }
      x = Matrix.rows @matches.map { |pts| [pts.first.x, pts.first.y] }

      CVFFI::Calib3d::computeHomographyReprojError( x, xp, h ).to_a
    else
      []
    end
  end

  def calculate_inliers( h, threshold = 3.0 )
    @num_inliers = calculate_residuals(h).count { |res|
      res < threshold
    }
  end

  def calculate_accuracy( ref )
    @accuracy = matches.count { |match| 
      ref_answer = ref.find_by_a_idx( match.a_idx )
      puts "Multiple answers from reference" if ref_answer.length > 1
      ref_answer = ref_answer.first

      match.b_idx == ref_answer.b_idx
    }
  end

  def find_by_a_idx( idx )
    matches.select { |match| match.a_idx == idx }
  end

  def to_h
    { pair: pair.to_h,
      algo: algo.to_h,
      num_matches: @matches.length,
      num_inliers: @num_inliers,
      accuracy: @accuracy,
      match_time: @match_time.to_h,
      train_time: (@train_time ? @train_time.to_h : nil ) }
  end
end

class ResultDb 

  ID = "result_rb"

  def initialize
    @results =  []
  end

  def add( pair, algo, matches, train_time, match_time )
    @results.push Result.new( pair, algo, matches,  train_time, match_time )
  end

  def image_pairs
    @results.map { |result| result.pair }.uniq!.sort!
  end

  def by_pair( pair )
    @results.select {|result| result.pair == pair}
  end

  def calculate( ref_algorithm )
    puts_pre ID, "Using \"%s\" as reference algorithm" % ref_algorithm.name


    image_pairs.each { |pair|
      puts_pre ID, "Calculating statistics for image pair %s" % pair.name

      # Find reference results
      ref = by_pair(pair).select { |result| result.algo == ref_algorithm }

      if ref.length > 1
        puts_pre ID, "Found more than one reference result for image pair %s" % pair.name
        next
      end

      if ref.length < 0
        puts_pre ID, "Unable to find reference results for image pair %s" % pair.name
        next
      end

      ref = ref.first

      by_pair(pair).each { |result|
        result.calculate_inliers( pair.true_homography ) if pair.true_homography
        result.calculate_accuracy( ref )
      }
      


    }

    self
  end

  def save( filename )
    File.write filename, JSON.pretty_generate( to_h )
  end

  def to_h
    { results: @results.map { |result| result.to_h } }
  end

end

