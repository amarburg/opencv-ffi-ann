require "flann_matcher"

class CVFFIFlannMatcher < FlannMatcher
  ID = :cvffi_flann
  def initialize( opts = {} )
    super
  end

  def match( query, train, opts = {} )
    # Apparently FLANN only takes floats
    
    results = nil
    @match_time = Benchmark.measure { 
    results = CVFFI::Matcher::flann_matcher( query.descriptors_to_mat( :CV_32F ), 
                                            train.descriptors_to_mat( :CV_32F ) )
    }

    results
  end
end

class ExtendedCVFFIFlannMatcher < FlannMatcher

  ID = :extended_cvfi_flann

  def initialize( weight )
    super

    @weight = weight
    set_description "%s (w=%.2e)" % [name, weight]
  end

  def match( query, train,  opts = {} )
    hom = opts[:homography] or raise "Homography not supplied to #{self.class.name}"

    results = nil
    @match_time = Benchmark.measure {
      q = ExtendedDescriptors.new(query,@weight).warp_descriptors_to_mat( hom, :CV_32F )
      t = ExtendedDescriptors.new(train,@weight).descriptors_to_mat( :CV_32F )

      # Apparently FLANN only takes floats
      results = CVFFI::Matcher::flann_matcher( q,t )
    }

    results
  end
end


