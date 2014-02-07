
require "rake/clean"
require "rake/file_list"

CLEAN << FileList.new( "ext/**/*.o" )
CLEAN << FileList.new( "ext/**/Rakefile" )
CLEAN << FileList.new( "ext/**/*.so" )

CLEAN << FileList.new( "lib/*.so" )
