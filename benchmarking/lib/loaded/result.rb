
class LoadedResult
  include ResultCommon

  attr_reader :pair, :algo, :num_matches
  attr_reader :train_time, :match_time
  alias_method :algorithm, :algo

  def initialize( pair, algo, opts )
    @pair = pair
    @algo = algo

    @num_matches = opts[:num_matches]
    @num_inliers = opts[:num_inliers]
    @accuracy =    opts[:accuracy]
    @match_time = opts[:match_time]
    @train_time = opts[:train_time]
  end

  def homography; pair.homography; end

  def print
    puts "%40s %20s %10s   % 6d % 4.2f  % 7.2f  %12s %12s %12s" % 
      [ algo.description, pair.name, pair.homography.name,
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

