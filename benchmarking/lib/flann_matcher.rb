require_relative "brute_force_matcher"

class FlannMatcher < Matcher

  def initialize( opts = {} )
    super ({ name: "flann" }.merge opts)
  end

  ID = :flann
  def match( query, train )
    print_pre ID, "Matching %d features with %d features" % [query.length, train.length] 

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

class EnhancedFlannMatcher < FlannMatcher

  def initialize( opts = {} )
    super ({ do_warp: true, name: "enhanced_flann" }.merge(opts))
  end

  ID = :enhanced_flann
  def match( query, train )
    print_pre ID, "Matching %d features with %d features" % [query.length, train.length] 

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
                  
