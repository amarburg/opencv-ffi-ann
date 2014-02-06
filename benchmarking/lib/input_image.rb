
require "opencv-ffi-wrappers"


class InputImage

  def initialize( pathname, opts = {} )
    @pathname = Pathname.new( pathname ).expand_path
    raise "Can't find image #{@pathname}" unless @pathname.readable?

    @scale = opts[:scale] || nil
  end

  def to_s; @pathname.to_s; end

  def basename
    @pathname.basename.sub_ext("")
  end
  alias :name :basename

  def as_CvMat
    mat = CVFFI::IplImage.load( to_s ).to_CvMat
    if @scale
      mat.shrink( @scale )
    end

    mat
  end

  def image_size
    @size ||= as_mat.image_size
  end
end
