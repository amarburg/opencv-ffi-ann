
class Matcher
  attr_reader :name, :do_warp

  def initialize( opts = {} )
    @name = opts[:name] || "unknown"
  end

end


