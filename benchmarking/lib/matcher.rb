require "benchmark"

class Matcher
  attr_reader :do_warp, :name
  attr_reader :train_time, :match_time
  attr_reader :name

  def initialize( opts = {}  )
    @name = self.class.name 
    @train_time = nil
    @match_time = nil
  end

  def set_description(a)
    @description = a
  end

  def description
    @description || @name
  end

  def to_h
    {name: name}
  end
end


