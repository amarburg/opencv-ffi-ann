          
class CVFFIBruteForceMatcher < Matcher

  def initialize(opts = {})
    super opts
  end

  ID = :brute_force
  def match( query, train, opts = {} )
    results = nil
    @match_time = Benchmark.measure {
      results = CVFFI::Matcher::brute_force_matcher( query.descriptors_to_mat( :CV_32F ), 
                                                    train.descriptors_to_mat( :CV_32F ), opts )
    }
    results
  end
end
 
