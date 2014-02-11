
require "input_image"


class ImagePairs

  def initialize
    @pairs = []
  end

  def add( a, b, opts = {} )
    @pairs << ImagePair.new( a, b, opts )
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

  def name
    "%s--%s" % [a.basename, b.basename]
  end
end
