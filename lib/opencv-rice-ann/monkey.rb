
require 'libopencv_rice_ann'

module CVRice
  module ANN
    
    class Matcher
      alias_method :match_c, :match
      def match( query, train = nil )
        if train
          train_match( query, train )
        else
          match_c( query )
        end
      end
    end

    class FlannMatcher
      def self.new( type = :KD_TREE )
        case type
        when :KD_TREE
          KdTreeFlannMatcher.new
        when :KMEANS
          KMeansFlannMatcher.new
        else 
          raise "Hm, unknown type of Flann matcher.."
        end
      end
    end


# TODO.  Found I wasn't use this, just instantiating the class i wanted
# I'm sure this API is useful, but don't want to keep it up right now
#
#    # I'm sure this could be done more elegantly, maybe with the 
#    # constructor taking an enum.  But I can't wrap my head around
#    # converting class constants to enums right now...
#    class BruteForceMatcher
#      def self.new( norm, crosscheck = false )
#        case norm
#        when :L2SQR
#          L2SqrBruteForceMatcher.new( crosscheck )
#        else
#          L2BruteForceMatcher.new( crosscheck )
#        end
#      end
#    end
#
#    class BruteForceRatioMatcher
#      def self.new( ratio, crosscheck = true )
#        case norm
#        when :L2SQR
#          L2SqrBFRatioMatcher.new( ratio, crosscheck )
#        else
#          L2BFRatioMatcher.new( ratio, crosscheck )
#        end
#      end
#    end


  end
end
