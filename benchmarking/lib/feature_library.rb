

module Mosaicker

  class FeatureLibrary

    def initialize
      @lib = {}
    end

    def add( image, features )
      @lib[ image ] = features
    end

    def [](a); @lib[a]; end

    def keys; @lib.keys; end
  end

end
