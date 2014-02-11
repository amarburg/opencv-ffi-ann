require_relative "matcher"

class FlannMatcher < Matcher

  ID = :flann

  def initialize( opts = {} )
    super ({ name: "flann" }.merge opts)
  end

  def match( query, train, opts = {} )
    print_pre ID, "Matching %d features with %d features" % [query.length, train.length] 

    #desc = query.descriptors_to_mat(:CV_32F).to_Matrix
    #p desc.row_vectors.at(2)
    #p 128.times.map { |i| query.at(2).descriptor[i] }.join(',')

    # Apparently FLANN only takes floats
    results = CVFFI::Matcher::flann_matcher( query.descriptors_to_mat( :CV_32F ), 
                                            train.descriptors_to_mat( :CV_32F ) )

    puts " .. have %d putative matches" % results.length

    results
  end
end

class EnhancedFlannMatcher < FlannMatcher

  ID = :enhanced_flann

  def initialize( weight, opts = {} )
    super ({ name: "enhanced_flann" }.merge(opts))

    @weight = weight
  end

  def match( query, train,  opts = {} )
    hom = opts[:homography] or raise "Homography not supplied to EnhancedFlannMatcher."

    print_pre ID, "Matching %d features with %d features" % [query.length, train.length] 

    q = EnhancedDescriptors.new(query,@weight).descriptors_to_mat( :CV_32F )
    t = EnhancedDescriptors.new(train,@weight).descriptors_to_mat( :CV_32F )

    # Apparently FLANN only takes floats
    results = CVFFI::Matcher::flann_matcher( q,t )

    puts " .. have %d putative matches" % results.length

    results
  end
end
                  
