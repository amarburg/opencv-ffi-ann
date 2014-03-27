require "bundler/gem_tasks"

require_relative "tasks/directories"
DirectorySet.new( __FILE__ )


task :default => [:test, :test_c]


Dir.glob("tasks/*").each { |f| require_relative Pathname.new(f).sub_ext('') }





