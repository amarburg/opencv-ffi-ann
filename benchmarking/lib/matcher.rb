require "benchmark"

class Matcher
  attr_reader :do_warp, :name
  attr_reader :train_time, :match_time

  def initialize( name, description = nil )
    @name = self.class.name
    @description = description || name
    @train_time = nil
    @match_time = nil
  end

  def set_name(a); @name = a; end
  def set_description(a); @description = a; end

  def describe
    @description || @name
  end

  def to_h
    {name: name}
  end
end


