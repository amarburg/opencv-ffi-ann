
class LoadedResults
  attr_reader :results

  include Enumerable

  def initialize( results = [] )
    @results = results
  end

  def length; @results.length; end

  def +(b)
    LoadedResults.new (@results + b.results)
  end

  def print
    @results.each(&:print)
  end

  ## Special case??

  def partition_by(&blk)
    keys = @results.map { |result| blk.call result }.uniq.sort!

    hash = {}
    keys.each { |kvalue|
      hash[kvalue] = LoadedResults.new @results.select { |result| blk.call( result ) == kvalue }
    }
    hash
  end

  def partition_by_algorithm
    partition_by { |result| result.algorithm.name }
  end

  def partition_by_pair
    partition_by { |result| result.pair.name }
  end

  def partition_by_homography
    partition_by { |result| result.homography.name }
  end

  def each
    if block_given?
      @results.each { |r| yield r }
    else
      @results.each
    end
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


