
class EnhancedFlannMatcher < FlannMatcher

  ID = :enhanced_flann

  def initialize( weight, opts = {} )
    super opts
    @weight = weight
    @prescaler = opts[:prescaler] || 1.0
    @name = ID.to_s
    @description = "%s (w=%.2e)" % [name, @weight]
  end

  def match( query, train,  opts = {} )
    hom = opts[:homography] or raise "Homography not supplied to EnhancedFlannMatcher."

    print_pre ID, "Matching %d features with %d features" % [query.length, train.length] 

    results = nil
    matcher = CVFFI::ANN::FlannMatcher.new

    @train_time = Benchmark.measure {
      t = EnhancedDescriptors.new(train,@weight * @prescaler).descriptors_to_mat( :CV_32F )
      matcher.train t
    }

    @match_time = Benchmark.measure {
      q = EnhancedDescriptors.new(query,@weight * @prescaler).warp_descriptors_to_mat( hom, :CV_32F )
      results = matcher.match q
    }

    puts " .. have %d putative matches" % results.length

    results
  end
end


