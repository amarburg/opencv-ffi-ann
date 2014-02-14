require 'result_common'

require_relative 'loaded/results'
require_relative 'loaded/result'
require_relative 'loaded/pair'
require_relative 'loaded/benchmark'

class LoadedAlgorithm
  attr_reader :name, :description, :opts
  def initialize( name, description, opts = {} )
    @name = name
    @description = description
    
    @opts = {}
    opts.each_pair { |k,v| @opts[k.to_sym] = v } if opts
  end

  def self.from_h( h )
    LoadedAlgorithm.new h["name"], h["description"], h["opts"]
  end
end

class LoadedHomography
  attr_reader :name, :opts
  def initialize( name, h )
    @name = name
    @mat = Matrix.rows h
    @opts = {}
  end

  def self.from_h( h )
    case h.keys.first
    when "Homography"
      val = h.values.first
      LoadedHomography.new val["name"], val["h"] 
    when "PerturbedHomography"
      val = h.values.first

      opts = {}
      LoadedPerturbedHomography.keys.each { |key|
opts[ key ] = val[ key.to_s ]
      }

      LoadedPerturbedHomography.new val["name"], val["h"], val["ref"], opts
    else
      puts "Don't know how to deal with a homography of type \"#{h.keys.first}\""
    end
  end
end
  
class LoadedPerturbedHomography < LoadedHomography

  def initialize( name, h, ref, opts = {} )
    super name, h

    self.class.keys.each { |key|
      instance_variable_set "@#{key}", opts[key]
      @opts[key] = opts[key]
      self.class.send( :define_method, key, Proc.new { instance_variable_get( "@#{key}" ) } )
    }
  end

  def self.keys
    [ :roll_var, :pitch_var, :yaw_var, 
      :roll_bias, :pitch_bias, :yaw_bias,
      :roll_error, :pitch_error, :yaw_error ]
  end


end

