
require 'mkrf'

require_relative Pathname.new(ENV['TOP_DIR']).join( "tasks", "directories" )
dirs = DirectorySet.new Pathname.new(ENV['TOP_DIR']).join("Rakefile" )

sources = %w( enhanced_descriptors.cpp
              cvffi_ann.cpp )

Mkrf::Generator.new( 'libcvffi_ann_c', sources, compiler: "g++" ) do |gen|
  # TODO.  Shouldn't be fixed paths...
  #   (need to make "mkrf-rice" to correspond to "mkmf-rice"
  #
  gen.logger.level = Logger::DEBUG
  
  #gen.include_header  "rice/Class.hpp", '/home/aaron/.rvm/gems/ruby-2.0.0-p195/gems/rice-1.5.3/ruby/lib/include' 
  #gen.include_library "rice", "", "/home/aaron/.rvm/gems/ruby-2.0.0-p195/gems/rice-1.5.3/ruby/lib/lib"
  #gen.include_library "stdc++", "", "/usr/lib/x86_64-linux-gnu/"

  # The standard automatic library detection mechanism isn't well suited to
  # shared libraries of C++ code, so specify libraries manually for now
  #
  gen.cflags << [ "-ggdb",
                  "-I" + dirs[:cvffi_ext].join("ext").to_s,
                  "-I" + dirs[:rice].join("include").to_s ].join(' ')

  # n.b.  Libraries should be specified after the object files.  This 'objects' syntax
  # causes mkrf to place this text after the list of objects on the linker command line
  # (though before the other libs)
  gen.objects << [ "-L" + dirs[:rice].join("lib").to_s, "-lrice",
                  "-lopencv_core", "-lopencv_features2d",
                  "-lstdc++" ].join(' ')

  #               -lstdc++ 
  #               -L#{g2o_dir.join('lib')} -lg2o_core_d -lg2o_csparse_extension -lg2o_stuff -lg2o_solver_csparse 
  #               -L#{graph_mosaic_lib} -lgraph_mosaic ).join(' ')
end
