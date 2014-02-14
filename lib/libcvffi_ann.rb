
require 'libcvffi_ann_c'

module CVFFI
  module ANN
    module Benchmarking
      # Monkey-patching additional functionality into the Rice class 
      # "Descriptors" in the extension library.
      #
      class Descriptors
        # Rather than bring the CVFFI API into the C++ code,
        # the C++ code returns a FFI::Pointer to a CvMat
        # which can be re-wrapped in this function as an CvMat
        # (n.b. it needs to be a FFI::Struct type, not, for example Mat)
        alias_method :descriptors_to_mat_c, :descriptors_to_mat
        def descriptors_to_mat( type = nil )
          ptr = descriptors_to_mat_c( type )
          CVFFI::CvMat.new ptr
        end
      end

      #
      #
      #
      class ExtendedDescriptors
        alias_method :descriptors_to_mat_c, :descriptors_to_mat
        def descriptors_to_mat( type = nil )
          ptr = descriptors_to_mat_c( type )
          CVFFI::CvMat.new ptr
        end

        alias_method :warp_descriptors_to_mat_c, :warp_descriptors_to_mat
        def warp_descriptors_to_mat( h, type = nil  )
          ptr = warp_descriptors_to_mat_c( h, type )
          CVFFI::CvMat.new ptr
        end
      end
    end

    #
    #
    #
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


    # I'm sure this could be done more elegantly, maybe with the 
    # constructor taking an enum.  But I can't wrap my head around
    # converting class constants to enums right now...
    class BruteForceMatcher
      def self.new( norm, crosscheck = false )
        case norm
        when :L2
          L2BruteForceMatcher.new( crosscheck )
        when :L2SQR
          L2SqrBruteForceMatcher.new( crosscheck )
        end
      end
    end

    class BruteForceRatioMatcher
      def self.new( ratio, crosscheck = true )
        L2BruteForceRatioMatcher.new( ratio, crosscheck )
      end
    end
  end
end
