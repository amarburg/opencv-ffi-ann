
require "mkrf/rakehelper"
require "rake/file_list"

## Extension library...
setup_extension('cvffi_ann', 'libcvffi_ann_c' )

desc "Build all extension libraries"
task :ext => [ :libcvffi_ann_c ]

task :libcvffi_ann_c => FileList['ext/cvffi_ann/**/*.h', 'ext/cvffi_ann/**/*.cpp', 'ext/cvffi_ann/mkrf_conf.rb']

