
class LoadedPair
  attr_reader :a, :b, :homography

  def initialize( a, b, homography )
    @a =a
    @b = b
    @homography = homography
  end

  def name
      "%s--%s" % [Pathname.new(a).basename,Pathname.new(b).basename]
  end

  def self.from_h( h )
    LoadedPair.new h["a"], h["b"], LoadedHomography.from_h( h["homography"] )
  end

  def <=>(b)
    name <=> b.name
  end
end


