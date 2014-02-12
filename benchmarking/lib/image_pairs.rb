
require "input_image"


class ImagePairs

  def initialize
    @pairs = []
  end

  def add( a, b, opts = {} )
    p = ImagePair.new( a, b, opts )
    add_pair( p )
  end

  def add_pair( pair )
    @pairs << pair
    pair
  end

  def length; @pairs.length; end

  def each
    if block_given?
      @pairs.each { |pair| yield pair }
    else
      @pairs.each
    end
  end

end


class ImagePair
  attr_reader :a, :b, :homography

  def initialize( a, b, opts = {} )
    @a = InputImage.new a
    @b = InputImage.new b

    hom = opts[:h]
    if hom
      @homography = Matrix.rows hom 
    end
  end

  alias :true_homography :homography

  def name
    "%s--%s" % [a.basename, b.basename]
  end

  def <=>(b)
    name <=> b.name 
  end
end

class ImagePairPerturbed
  attr_reader :a, :b

  def initialize( pair, opts = {} )
    @a = pair.a
    @b = pair.b

    @hom = opts[:h]
  end

  def homography
    @hom.homography
  end

  def true_homography
    @hom.true_homography
  end

  def name
    "%s--%s--%s" % [a.basename, b.basename, @hom.name]
  end

  def <=>(b)
    name <=> b.name 
  end
end
