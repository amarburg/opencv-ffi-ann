
require "input_image"


class ImagePairs

  def initialize
    @pairs = []
  end

  def add( a, b )
    @pairs << ImagePair.new( a, b )
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
  attr_reader :a, :b

  def initialize( a, b )
    @a = InputImage.new a
    @b = InputImage.new b
  end
end
