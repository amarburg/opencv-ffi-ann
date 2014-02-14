
class ExtendedFlannMatcher < FlannMatcher

  ID = :extended_flann

  def initialize( weight, opts = {} )
    super opts
    @weight = weight

    set_description "%s (w=%.2e)" % [name, @weight]
  end

  def match( query, train,  opts = {} )
    hom = opts[:homography] or raise "Homography not supplied to #{self.class.name}"


    results = nil
    matcher = CVFFI::ANN::FlannMatcher.new

    @train_time = Benchmark.measure {
      t = ExtendedDescriptors.new(train,@weight).descriptors_to_mat( :CV_32F )
      matcher.train t
    }

    @match_time = Benchmark.measure {
      q = ExtendedDescriptors.new(query,@weight).warp_descriptors_to_mat( hom, :CV_32F )
      results = matcher.match q
    }

    results
  end

  def to_h
    h = super
    h[:opts] = { weight: @weight }
    h
  end
end


