
require "opencv-ffi-wrappers"


class InputImage
  attr_reader :intrinsics

  def initialize( pathname, opts = {} )
    @pathname = Pathname.new( pathname ).expand_path
    raise "Can't find image #{@pathname}" unless @pathname.readable?

    @scale = opts[:scale] || nil
    
    @intrinsics = if opts[:intrinsics] 
                    case opts[:intrinsics]
                    when Array
                      Matrix.rows opts[:intrinsics]
                    else
                      opts[:intrinsics]
                    end
                  else
                    nil
                  end

  end

  def to_s; @pathname.to_s; end

  def basename
    @pathname.basename.sub_ext("")
  end
  alias :name :basename

  def kinv
    @kinv ||= if @intrinsics 
                @intrinsics.inv
              else
                nil
              end
  end

  def as_CvMat
    mat = CVFFI::IplImage.load( to_s ).to_CvMat
    if @scale
      mat.shrink( @scale )
    end

    mat
  end

  def image_size
    @size ||= as_CvMat.image_size
  end
end
