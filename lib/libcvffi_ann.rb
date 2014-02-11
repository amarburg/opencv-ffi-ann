
require "libcvffi_ann_c"

module CVFFI
  module ANN
    module Benchmarking

      class Descriptors
        # Rather than bring the CVFFI API into the C++ code,
        # the C++ code returns a FFI::Pointer to a CvMat
        # which can be re-wrapped in this function as an CvMat
        # (n.b. it needs to be a FFI::Struct type, not, for example Mat)
        alias :descriptors_to_mat_c :descriptors_to_mat
        def descriptors_to_mat( type = nil )
          ptr = descriptors_to_mat_c( type )
          CVFFI::CvMat.new ptr
        end

      end

      class EnhancedDescriptors
        alias :descriptors_to_mat_c :descriptors_to_mat
        def descriptors_to_mat( type = nil )
          ptr = descriptors_to_mat_c( type )
          CVFFI::CvMat.new ptr
        end
      end
    end
  end
end
