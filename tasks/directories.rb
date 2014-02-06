
class DirectorySet

  def initialize( rakefile )
    @dirs = {}

    @dirs[:top] = Pathname.new(rakefile).expand_path.parent

    paths = []; topdir.descend{ |p| paths << p }
    @dirs[:workspace] = paths.reverse.find { |p| p.basename.to_s =~ /workspace/ }

    @dirs[:gtest] = workspace.join("gtest")
    raise "Couldn't find gtest" unless gtest.directory?

    @dirs[:gem] = Pathname.new( `rvm gemdir`.chomp )
    raise "Couldn't find gemdir \"#{gemdir}\"" unless gemdir.directory?

    @dirs[:rice] = @dirs[:gem].join(*%w(gems rice-1.6.0 ruby lib))
    raise "Couldn't find the RICE source files \"%s\"" % @dirs[:rice] unless @dirs[:rice]


    set_env_paths
  end

  def dir( a )
    @dirs[a]
  end

  def [](a)
    @dirs[a]
  end

  # Convenience macros
  def topdir; dir( :top ); end
  def gtest; dir( :gtest ); end
  def workspace; dir( :workspace ); end
  def gemdir; dir(:gem);end

  def rbconfig(a) 
    RbConfig::CONFIG[a]
  end

  def set_env_paths
    ENV['TOP_DIR'] = topdir.to_s
    ENV['GTEST_DIR'] = gtest.to_s
    ENV['RICE_DIR'] = dir(:rice).to_s
    ENV['RB_LIB_ARGS'] = rbconfig('LIBRUBYARG')
    ENV['RB_INCLUDE_ARGS'] = %W{ -I#{rbconfig('rubyhdrdir')}/ruby
                                 -I#{rbconfig('rubyarchhdrdir')}/ruby }.join(' ')

    # TODO:  How to get ruby libdir automatically?
    ENV['LD_LIBRARY_PATH'] = %W( -I${rbconfig('rubylibdir')}/lib ).join(':')
  end
end


