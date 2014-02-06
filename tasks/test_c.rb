## C++-language testing

desc "Run the C/C++ language test suite."

task :test_c => "test_c:run"
namespace :test_c do

  def make_in_test_c( cmd )
    sh "cd test_c && make " + cmd
  end

  task :run => :ext do
    make_in_test_c "test"
  end
  
  task :gdb => :ext do
    make_in_test_c "gdb"
  end

  task :clean do
    make_in_test_c "clean"
  end
end


