## C++-language testing

require_relative "directories"

desc "Run the C/C++ language test suite."

task :test_c => "test_c:run"
namespace :test_c do

  test_app = "test_c/test_app"
  srcs     = Dir.glob('test_c/*.cpp')
  objs     = srcs.map { |f| Pathname.new(f).sub_ext('.o').to_s }

  cpp      = 'g++'
  cflags   = ['-ggdb',
              "-I#{dirs.gtest}/include"]
  ldflags  = cflags
  libs  = ["-L#{dirs.gtest}/build", '-lgtest', '-lpthread']

  file test_app => [:ext] + objs do
    sh [ cpp,  *ldflags, 
         '-o', test_app, *objs, *libs ].join(' ')
  end

  srcs.each { |src|
    obj = Pathname.new(src).sub_ext('.o').to_s
    file obj => src do
      sh [ cpp, '-c', *cflags, '-o', obj, src ].join(' ')
    end
  }

  task :run => test_app do
    sh test_app
  end

  task :gdb => test_app do
    sh "gdb #{test_app}"
  end

  clean_files = %w( test_c/*.o
        test_c/test_app )

  task :clean do
    clean_files.each { |path|
      Dir.glob( path ).each { |file|
        File.unlink( file )
      }
    }
  end
end

