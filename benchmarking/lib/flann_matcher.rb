require_relative "brute_force_matcher"

class FLANNMatcher < Matcher

  def initialize( opts = {} )
    super "flann"
  end

  ID = :flann
  def match( query, train )
    print_pre ID, "Matching %d features with %d features" % [query.length, train.length] if @verbose

    #desc = query.descriptors_to_mat(:CV_32F).to_Matrix
    #p desc.row_vectors.at(2)
    #p 128.times.map { |i| query.at(2).descriptor[i] }.join(',')

    # Apparently FLANN only takes floats
    results = CVFFI::Matcher::flann_matcher( query.descriptors_to_mat( :CV_32F ), 
                                            train.descriptors_to_mat( :CV_32F ), ratio: 1.4 )

    puts " .. have %d putative matches" % results.length

    results
  end
end
                  
