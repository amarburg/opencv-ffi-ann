require "delegate"

class Result
  attr_reader :pair, :algo, :matches
  attr_reader :num_inliers, :accuracy

  def initialize( pair, algo, matches, tms )
    @pair = pair
    @algo = algo
    @matches = matches
    @tms = tms
  end

  def calculate_residuals( h )
    xp = Matrix.rows @matches.map { |pts| [pts.second.x, pts.second.y] }
    x = Matrix.rows @matches.map { |pts| [pts.first.x, pts.first.y] }

    CVFFI::Calib3d::computeHomographyReprojError( x, xp, h ).to_a
  end

  def calculate_inliers( h, threshold = 3.0 )
    @num_inliers = calculate_residuals(h).count { |res|
      res < threshold
    }
  end

  def frac_inliers
    @num_inliers.to_f / @matches.length
  end

  def pct_inliers; frac_inliers * 100.0; end

  def calculate_accuracy( ref )
    @accuracy = matches.count { |match| 
      ref_answer = ref.find_by_a_idx( match.a_idx )
      puts "Multiple answers from reference" if ref_answer.length > 1
      ref_answer = ref_answer.first

      match.b_idx == ref_answer.b_idx
    }
  end

  def frac_accuracy
    @accuracy.to_f / @matches.length
  end
  def pct_accuracy; frac_accuracy * 100.0; end

  def find_by_a_idx( idx )
    matches.select { |match| match.a_idx == idx }
  end
end

class ResultDb 

  ID = "result_rb"

  def initialize
    @results =  []
  end

  def add( pair, algo, matches, tms )
    @results.push Result.new( pair, algo, matches, tms )
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
        result.calculate_inliers( pair.homography ) if pair.homography
        result.calculate_accuracy( ref )

        puts_pre ID, "%20s %20s   % 4.2f  % 4.2f" % [ result.algo.name, result.pair.name, result.pct_inliers, result.pct_accuracy ]
      }
      


    }


  end
end

