require "rake/testtask"

module FileUtils
  # Run a Ruby interpreter with the given arguments.
  #
  # Example:
  #   ruby %{-pe '$_.upcase!' <README}
  #
  def ruby_gdb(*args, &block)
    options = (Hash === args.last) ? args.pop : {}
    if args.length > 1
      sh(*(["gdb --args"] + [RUBY] + args + [options]), &block)
    else
      sh("gdb --args #{RUBY} #{args.first}", options, &block)
    end
  end

end

module Rake

  class TestTask
    # Create the tasks defined by this task lib.
    
    alias :define_super :define
    def define
      define_super

      desc "Run tests" + (@name == :test ? "" : " for #{@name}") + " under gdb"
      task (@name.to_s + "_gdb").to_sym do
        FileUtilsExt.verbose(@verbose) do
          args =
            "#{ruby_opts_string} #{run_code} " +
            "#{file_list_string} #{option_list}"
          ruby_gdb args do |ok, status|
            if !ok && status.respond_to?(:signaled?) && status.signaled?
              raise SignalException.new(status.termsig)
            elsif !ok
              fail "Command failed with status (#{status.exitstatus}): " +
                "[ruby #{args}]"
            end
          end
        end
      end

      self
    end

  end
end
