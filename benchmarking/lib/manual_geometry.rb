
require_relative "brute_force_matcher"

require "opencv-ffi-ext"


class ManualGeometry < BruteForceMatcher

  ID = :manual_geometry

  def initialize(opts = {})
    super opts
    @threshold = opts[:threshold] || 9.0
    @name = ID.to_s
  end

  def match( query, train, opts = {} )
    hom = opts[:homography] or raise "Homography not supplied to EnhancedFlannMatcher."

    results = nil
    @match_time = Benchmark.measure {
      results = matcher.match( query.descriptors_to_mat( :CV_32F ), 
                              train.descriptors_to_mat( :CV_32F ) )


      x = Matrix.rows results.map { |match| 
        a = query[ match.queryIdx ]
        [a.x, a.y] }
      xp = Matrix.rows results.map { |match| 
        b = train[ match.trainIdx ]
        [b.x, b.y] }

      residuals = CVFFI::Calib3d::computeHomographyReprojError( x, xp, hom ).to_a

      results.select!.with_index { |match,idx|
        residuals[idx] < @threshold
      }
    }

    results
  end
end

