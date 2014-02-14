
require "input_image"
require_relative "homography"

class ImagePairs

  def initialize
    @pairs = []
    @library = Hash.new { |h,k|
      h[k] = InputImage.new k
    }
  end

  def add( a, b, opts = {} )
    pair= ImagePair.new( @library, a, b, opts )
    @pairs << pair
    pair
  end

  def clone( copy, opts = {} )
    add( copy.image_a, copy.image_b, opts )
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
  attr_reader :image_a, :image_b, :hom

  def initialize( image_library, a, b, opts = {} )
    @image_a = a
    @image_b = b
    @lib = image_library

    hom = opts[:h]
    case hom
    when Array
      @hom = Homography.new Matrix.rows hom 
    when Matrix
      @hom = Homography.new hom
    when Homography
      @hom = hom
    else
      puts "Don't know what to do with #{hom}"
    end
  end

  def a; @lib[image_a]; end
  def b; @lib[image_b]; end

  def name
      "%s--%s" % [a.basename, b.basename ]
  end

  def <=>(b)
    name <=> b.name 
  end

  def homography; @hom.h; end
  def true_homography; @hom.truth; end

  def to_h
    { a: image_a.basename,
      b: image_b.basename,
      homography: @hom.to_h }
  end
end

