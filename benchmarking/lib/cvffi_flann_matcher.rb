require "flann_matcher"

class CVFFIFlannMatcher < FlannMatcher
  ID = :cvffi_flann
  def initialize( opts = () )
    super opts
    @name = "cvffi_flann"
  end

  def match( query, train, opts = {} )
    print_pre ID, "Matching %d features with %d features" % [query.length, train.length] 

    # Apparently FLANN only takes floats
    
    results = nil
    @match_time = Benchmark.measure { 
    results = CVFFI::Matcher::flann_matcher( query.descriptors_to_mat( :CV_32F ), 
                                            train.descriptors_to_mat( :CV_32F ) )
    }

    puts " .. have %d putative matches" % results.length

    results
  end
end

class EnhancedCVFFIFlannMatcher < FlannMatcher

  ID = :enhanced_cvfi_flann

  def initialize( weight, opts = {} )
    super opts
    @weight = weight
    @name = ID.to_s
    @description = "%s (w=%f)" % [name, @weight]
  end

  def match( query, train,  opts = {} )
    hom = opts[:homography] or raise "Homography not supplied to EnhancedFlannMatcher."

    print_pre ID, "Matching %d features with %d features" % [query.length, train.length] 

    results = nil
    @match_time = Benchmark.measure {
      q = EnhancedDescriptors.new(query,@weight).warp_descriptors_to_mat( hom, :CV_32F )
      t = EnhancedDescriptors.new(train,@weight).descriptors_to_mat( :CV_32F )

      # Apparently FLANN only takes floats
      results = CVFFI::Matcher::flann_matcher( q,t )
    }

    puts " .. have %d putative matches" % results.length

    results
  end
end


