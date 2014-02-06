
desc "Run yardoc on the code (output in doc/)"
task :yardoc do
  sh "yardoc lib/**/*.rb ext/**/*.cpp"
end
