

  # n.b.:  The homography estimation finds b = Ha
  #
  class Match 

    def initialize( dmatch, features_a, features_b )
      fa = features_a[ dmatch.queryIdx ]
      fb = features_b[ dmatch.trainIdx ]

      @ax = fa.x
      @ay = fa.y
      @bx = fb.x
      @by = fb.y

      @dmatch = dmatch
    end

    def a_idx; @dmatch.queryIdx; end
    def b_idx; @dmatch.trainIdx; end

    def <=>(b)
      to_a <=> b.to_a
    end

    def to_a
      [@ax, @ay, @bx, @by]
    end

    # TODO:  Sort this type conversion nonsense out...
    #
    # Start the gratuitous type conversions..
    def first; CVFFI::Point.new( @ax, @ay ); end
    def second; CVFFI::Point.new( @bx, @by ); end
    def points; [@first, @second]; end
  end


  #
  # Convenience class for passing around a pair of matches, matches, and pair of features
  #
  # Must remember .. Matches is supposed to be _temporary_ because it contains
  # references to all of the features and etc.
  class Matches

    include Enumerable

    attr_reader :dmatches, :features_a, :features_b

    def initialize( dmatches, features_a, features_b )
      @dmatches = dmatches
      @features_a = features_a
      @features_b = features_b
    end

    alias :matches :dmatches

    def a_mat
      Matrix.rows( @dmatches.map { |match|
        q = @features_a[ match.queryIdx ]
        [ q.x, q.y ]
      } ).to_Mat
    end

    def b_mat
      Matrix.rows( @dmatches.map { |match|
        t = @features_b[ match.trainIdx ]
        [ t.x, t.y ]
      } ).to_Mat
    end
    def cvmats; [a_mat, b_mat]; end
    def length; @dmatches.length; end
    
    def arr
      @arr ||= Array.new( @dmatches.length ) { |i| Match.new( @dmatches[i], @features_a, @features_b ) }
    end

    def each
      if block_given?
        arr.each { |m| yield m }
      else
        arr.each
      end
    end
  end


