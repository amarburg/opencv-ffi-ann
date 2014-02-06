require "delegate"

class Result
  def initialize( matches, tms )
    @matches = matches
    @tms = tms
  end
end

class ResultDb < DelegateClass( Hash )

  def initialize
    @results = Hash.new { |h,k| h[k] = {} }
    super @results
  end

  def add( pair, algo, matches, tms )
    @results[pair][algo] = Result.new( matches, tms )
  end
end

