require 'experiment_support/projective/decomposes_homography'
require 'experiment_support/random/normal'

class Homography
  attr_reader :name

  def initialize( mat, opts = {} )
    @mat = mat
    @name = opts[:name] || "ref"
  end

  def truth; @mat; end
  def h;     @mat; end

  def to_h
    { Homography: { name: name, h: @mat.to_a } }
  end
end

class PerturbedHomography < Homography
  include ExperimentSupport::DecomposesHomography 
  include ExperimentSupport::Random

  def initialize( name, ref, opts = {} )
    super ref, opts.merge({name: name})

    @ref = ref

    @roll_var = opts[:roll_var] || 0.0
    @pitch_var = opts[:pitch_var] || 0.0
    @yaw_var = opts[:yaw_var] || 0.0

    @roll_bias = opts[:roll_bias] || 0.0
    @pitch_bias = opts[:pitch_bias] || 0.0
    @yaw_bias = opts[:yaw_bias] || 0.0

    
    decomp = decompose_homography( ref )
    decomp = decomp.find { |hom|
      angles = hom.r.to_euler_angles_roll_pitch_yaw
      angles.roll.abs < 1.0 and angles.pitch.abs < 1.0
    }

    raise "Hm, couldn't decompose the homography" unless decomp

    roll_rand = Normal.new( @roll_bias, @roll_var )
    pitch_rand = Normal.new( @pitch_bias, @pitch_var )
    yaw_rand = Normal.new( @yaw_bias, @yaw_var )

    angles = decomp.r.to_euler_angles_roll_pitch_yaw
    angles.roll += (@roll_error = roll_rand.rand)
    angles.pitch += (@pitch_error = pitch_rand.rand)
    angles.yaw += (@yaw_error = yaw_rand.rand)
    new_h = DecomposedHomography.new( decomp.h, angles.to_rotation_matrix, decomp.n )

    @perturbed = new_h.recompose
  end

  def h; @perturbed; end
  
  def to_h
    h = { ref: @ref.to_a, 
          homography: @perturbed }

    [ :roll_var, :pitch_var, :yaw_var, 
      :roll_bias, :pitch_bias, :yaw_bias,
      :roll_error, :pitch_error, :yaw_error ].each { |key|
        h[ key ] = instance_variable_get( "@" + key.to_s )
      }

    { PerturbedHomography: h }
  end


end



