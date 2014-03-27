
desc "Run a console"
task :console do
#  require 'irb'
#  require 'irb/completion'
#  require 'opencv-rice-ann'
#  ARGV.clear

  sh "bundle exec irb -r opencv-rice-ann "
#  IRB.start
end
