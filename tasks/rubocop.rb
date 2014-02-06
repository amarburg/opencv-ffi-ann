
desc "Run the Rubocop tool on lib/"
task :rubocop do
  sh "rubocop lib benchmarking"
end
