
## Binaries 
# I call binaries from Rakefile so LD_LIBRARY_PATH can be set appropriately

def benchmark_bin_wrapper( name )
  bin_opts = %w(   --verbose )

  task name => :ext do
    ruby "benchmarking/#{name} " + bin_opts.join(' ')
  end

  task "#{name}_gdb".to_sym => :ext do
    ruby_gdb "benchmarking/#{name} " + bin_opts.join(' ')
  end
end

include Rake::DSL 

desc "Run one of the binaries to get the help string."
task :help => :ext do
  ruby "benchmarking/do_test --help"
end

benchmark_bin_wrapper :do_test
benchmark_bin_wrapper :do_self_test
benchmark_bin_wrapper :do_make_homographies

task :process_results do
    ruby "benchmarking/process_results"
end


