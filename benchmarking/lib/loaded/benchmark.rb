
module Benchmark
  class Tms
    def self.from_h( h )
      return nil unless h

      # to_a and the constructor have arguments in slightly different order
      # to_a starts with the label, constructor ends with the label
      Tms.new( *(h["tms"].rotate) )
    end

    def to_h
      {tms: to_a }
    end
  end
end

