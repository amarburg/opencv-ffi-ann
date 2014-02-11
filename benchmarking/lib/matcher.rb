
class Matcher
  attr_reader :name, :do_warp

  def initialize( opts = {} )
    @name = opts[:name] || "unknown"
    @do_warp = opts[:do_warp] || false
  end

end


