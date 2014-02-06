
require "opencv-ffi-ext/features2d"
require "print_now"

module SiftFeatures

  ID = :sift

  def self.extract_features( image )
    mat = image.as_CvMat
    params = CVFFI::Features2D::SIFT::Params.new

    print_pre ID, "Extracting SIFT features from image which is %d x %d" % [mat.image_size.width, mat.image_size.height] 

    results = CVFFI::Features2D::SIFT.detect_describe( mat, params )

    puts "   ... extracted %d features" % results.length 

    results
  end
end
