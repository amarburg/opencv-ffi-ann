
class DirectorySet

  def initialize( me )
    @dirs = {}

    @dirs[:top] = Pathname.new( me ).parent.expand_path

    paths = []; topdir.descend{ |p| paths << p }
    @dirs[:workspace] = paths.reverse.find { |p| p.basename.to_s =~ /workspace/ }

    @dirs[:gtest] = workspace.join("gtest")
    raise "Couldn't find gtest" unless @dirs[:gtest].directory?

    @dirs[:gem] = Pathname.new( `rvm gemdir`.chomp )
    raise "Couldn't find gemdir \"#{gemdir}\"" unless @dirs[:gem].directory?

    @dirs[:rice] = gem_path( "rice" ).join('ruby','lib')
    raise "Couldn't find the Rice source files \"%s\"" % @dirs[:rice] unless @dirs[:rice]

    @dirs[:cvrice] = gem_path('opencv-rice')
    raise "Couldn't find opencv-rice" unless @dirs[:cvrice].directory?
  end

  def gem_path( gem )
    Pathname.new(Bundler.setup.gems[gem].first.bin_dir).parent
  end

  def [](a); @dirs[a]; end

  def topdir; @dirs[:top]; end
  def workspace; @dirs[:workspace]; end

  def ruby_cflags
    [ "-I#{rbconfig_dir('rubyhdrdir')}",
      "-I#{rbconfig_dir('rubyarchhdrdir')}" ]
  end

    def ruby_ldflags
      [ rbconfig('LIBRUBYARG') ]
    end

  def rbconfig(a) 
    RbConfig::CONFIG[a]
  end

  def rbconfig_dir( a )
    Pathname.new( rbconfig a )
  end
#
#  def set_env_paths
#    ENV['TOP_DIR'] = topdir.to_s
#    ENV['GTEST_DIR'] = gtest.to_s
#    ENV['RICE_DIR'] = dir(:rice).to_s
#    ENV['RB_LIB_ARGS'] = ruby_ldflags.join(' ')
#    ENV['RB_INCLUDE_ARGS'] = ruby_cflags.join(' ')
#    # TODO:  How to get ruby libdir automatically?
#    ENV['LD_LIBRARY_PATH'] = [rbconfig('rubylibdir') + "/lib", 
#                               cvrice.join('lib'),
#                               topdir.join("lib") ].join(':')
#  end
end

def dirs
  @dirs ||= DirectorySet.new(__FILE__)
end

dirs

