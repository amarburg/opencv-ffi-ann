


class ImagePairs

  def initialize
    @pairs = []
  end

  def add( a, b )
    @pairs << ImagePair.new( a, b )
  end

  def length; @pairs.length; end

end


class ImagePair

  def initialize( a, b )
    @a = a
    @b = b
  end
end
