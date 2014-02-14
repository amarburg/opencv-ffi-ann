require "benchmark"

class Matcher
  attr_reader :do_warp, :name
  attr_reader :train_time, :match_time

  def initialize( opts = {}  )
    @train_time = nil
    @match_time = nil
  end

  def name
    @name ||= self.class.name
  end

  def set_description(a)
    @description = a
  end

  def description
    @description || @name
  end

  def to_h
    {name: name, description: description }
  end
end


