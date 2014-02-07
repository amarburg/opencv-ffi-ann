
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
    raise "Couldn't find the Rice source files \"%s\"" % @dirs[:rice] unless @dirs[:rice]

    @dirs[:cvffi_ext] = @dirs[:workspace].join('opencv-ffi','opencv-ffi-ext')
    raise "Couldn't find opencv-ffi-ext" unless @dirs[:cvffi_ext]


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

  def rbconfig_dir( a )
    Pathname.new( rbconfig a )
  end

  def set_env_paths
    ENV['TOP_DIR'] = topdir.to_s
    ENV['GTEST_DIR'] = gtest.to_s
    ENV['RICE_DIR'] = dir(:rice).to_s
    ENV['RB_LIB_ARGS'] = rbconfig('LIBRUBYARG')
    ENV['RB_INCLUDE_ARGS'] = [ "-I" + rbconfig_dir('rubyhdrdir').join("ruby").to_s,
                               "-I" + rbconfig_dir('rubyarchhdrdir').join("ruby").to_s ].join(' ')

    # TODO:  How to get ruby libdir automatically?
    ENV['LD_LIBRARY_PATH'] = [ rbconfig('rubylibdir') + "/lib",
                               topdir.join("lib") ].join(':')
  end
end


