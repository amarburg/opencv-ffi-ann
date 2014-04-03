require "bundler/gem_tasks"

require_relative "tasks/directories"
DirectorySet.new( __FILE__ )


task :default => [:test_c, :test]


Dir.glob("tasks/*").each { |f| require_relative Pathname.new(f).sub_ext('') }





