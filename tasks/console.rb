
desc "Run a console"
task :console do
  require 'irb'
  require 'irb/completion'
  require 'libcvffi_ann'
  ARGV.clear
  IRB.start
end
