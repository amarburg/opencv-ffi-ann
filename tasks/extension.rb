
require "mkrf/rakehelper"
require "rake/file_list"

## Extension library...
setup_extension('opencv_rice_ann', 'libopencv_rice_ann' )

desc "Build all extension libraries"
task :ext => [ :libopencv_rice_ann ]

lib_deps = FileList['ext/opencv_rice_ann/*.h',
                    'ext/opencv_rice_ann/*.cpp',
                    'ext/opencv_rice_ann/**/*.h', 
                    'ext/opencv_rice_ann/**/*.cpp', 
                    'ext/opencv_rice_ann/mkrf_conf.rb']

task :libopencv_rice_ann => lib_deps

