
module ResultCommon

  def total_time
    if @train_time
      @match_time + @train_time
    else
      @match_time
    end
  end

  def frac_inliers
    if num_matches > 0
      @num_inliers.to_f / num_matches
    else
      0.0
    end
  end

  def pct_inliers; frac_inliers * 100.0; end

  def frac_accuracy
    if num_matches > 0
      @accuracy.to_f / num_matches
    else
      0.0
    end
  end
  def pct_accuracy; frac_accuracy * 100.0; end

end


