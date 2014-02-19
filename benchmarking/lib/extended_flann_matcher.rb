
class ExtendedKdTreeFlannMatcher < KdTreeFlannMatcher

  ID = :extended_flann

  def initialize( weight, opts = {} )
    super opts
    @weight = weight

    set_description "%s (w=%.2e)" % [name, @weight]
  end

  def match( query, train,  opts = {} )
    hom = opts[:homography] or raise "Homography not supplied to #{self.class.name}"


    results = nil

    matcher = make_matcher
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

class ExtendedKdTreeFlannRatioMatcher < KdTreeFlannRatioMatcher

  def initialize( weight, ratio = 1.4, opts = {} )
    super ratio
    @weight = weight
    set_description "%s (w=%.2e,r=%.1f)" % [name, @weight, @ratio]
  end

  def match( query, train,  opts = {} )
    hom = opts[:homography] or raise "Homography not supplied to #{self.class.name}"

    results = nil

    matcher = make_matcher
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


