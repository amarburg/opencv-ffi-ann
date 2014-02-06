# coding: utf-8
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'opencv-ffi-ann/version'

Gem::Specification.new do |spec|
  spec.name          = "opencv-ffi-ann"
  spec.version       = CVFFI::ANN::VERSION
  spec.authors       = ["Aaron Marburg"]
  spec.email         = ["aaron.marburg@canterbury.ac.nz"]
  spec.summary       = %q{ANN extensions to OpenCV-FFI}
  spec.description   = %q{ANN extensions to OpenCV-FFI}
  spec.homepage      = ""
  spec.license       = "MIT"

  spec.files         = `git ls-files -z`.split("\x0")
  spec.executables   = spec.files.grep(%r{^bin/}) { |f| File.basename(f) }
  spec.test_files    = spec.files.grep(%r{^(test|spec|features)/})
  spec.require_paths = ["lib"]

  spec.extensions << "ext/cvffi_ann"

  spec.add_development_dependency "bundler", "~> 1.5"
  spec.add_development_dependency "rake"
  spec.add_development_dependency "yard"
  spec.add_development_dependency "rubocop"

  spec.add_dependency "rice"
end
