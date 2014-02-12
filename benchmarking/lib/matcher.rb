require "benchmark"

class Matcher
  attr_reader :name, :do_warp
  attr_reader :train_time, :match_time

  def initialize( opts = {} )
    @name = nil
    @description = nil
    @train_time = nil
    @match_time = nil
  end

  def describe
    @description || @name
  end

  def to_h
    {name: name}
  end
end


