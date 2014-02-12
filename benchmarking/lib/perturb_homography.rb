

require "experiment_support/projective/decomposes_homography"
require "experiment_support/random/normal"

class PerturbHomography
  extend ExperimentSupport::DecomposesHomography
  include ExperimentSupport::DecomposesHomography
  include ExperimentSupport::Random

  attr_accessor :true_homography, :homography, :name

  def initialize( h, name, opts = {} )
    @true_homography = h
    @name = name

    roll_var = opts[:roll_var] || 0.0
    pitch_var = opts[:pitch_var] || 0.0
    yaw_var = opts[:yaw_var] || 0.0
    
    decomp = decompose_homography( h )
    decomp = decomp.find { |hom|
      angles = hom.r.to_euler_angles_roll_pitch_yaw
      angles.roll.abs < 1.0 and angles.pitch.abs < 1.0
    }

    raise "Hm, couldn't decompose the homography" unless decomp

    roll_rand = Normal.new( 0.0, roll_var )
    pitch_rand = Normal.new( 0.0, pitch_var )
    yaw_rand = Normal.new( 0.0, yaw_var )

    angles = decomp.r.to_euler_angles_roll_pitch_yaw
    angles.roll += roll_rand.rand
    angles.pitch += pitch_rand.rand
    angles.yaw += yaw_rand.rand
    new_h = DecomposedHomography.new( decomp.h, angles.to_rotation_matrix, decomp.n )

    @homography = new_h.recompose
  end

end
