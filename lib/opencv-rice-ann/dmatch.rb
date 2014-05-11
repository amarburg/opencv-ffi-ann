

module CVRice

  class DMatch

    def select_by_h( features_q, features_t, h, threshold )
      ANN::dmatch_select_by_inliers( self, features_q, features_t, h, threshold )
    end

    def select_by_mask( mask )
      ANN::dmatch_select_by_mask( self, mask )
    end
  end
end
