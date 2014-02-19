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


class InterCameraHomography < Homography

  def initialize( x, xp, opts = {} )
    h_est = estimate_homography_from_cameras( x, xp )
    super h_est, opts
    @name = "photoscan"
  end

  def estimate_homography_from_cameras( x, xp )
    # n.b. this is the same as     xp.world_to_body     * x.world_to_body.inv
    inter = Photoscan::Transform.new xp.world_to_body.mat * x.body_to_world.mat

    # Given the camera-to-camera body transform, can I turn that into an estimated homography?
    #  Use the A - av^T formulation from H&Z
    #  v = equation for plane.  Assume plane is on X,y plane at z = 0
    xy_plane = Vector[ 0, 0, 1, 0]
    xy_plane_cam = x.world_to_body.inv.mat.transpose * xy_plane
    #xy_plane_cam.print( caption: "xy_plane_cam" )
    # in-homogenize it
    norm_plane = Matrix.rows [[ xy_plane_cam[0] / xy_plane_cam[3],
                                xy_plane_cam[1] / xy_plane_cam[3],
                                xy_plane_cam[2] / xy_plane_cam[3] ]]
    #norm_plane.print( caption: "xy_plane_cam un-homogenized" )

    h_est = xp.calibration.k * (inter.r_mat - inter.t_mat * norm_plane ) * x.calibration.k.inv
    h_est = h_est.map { |x| x / h_est[2,2] }

    #h_est.print("h_est")

    h_est
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
    h = { name: name,
          ref: @ref.to_a, 
          h: @perturbed.to_a }

    [ :roll_var, :pitch_var, :yaw_var, 
      :roll_bias, :pitch_bias, :yaw_bias,
      :roll_error, :pitch_error, :yaw_error ].each { |key|
        h[ key ] = instance_variable_get( "@" + key.to_s )
      }

    { PerturbedHomography: h }
  end


end



