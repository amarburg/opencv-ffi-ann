# coding: utf-8
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'opencv-rice-ann/version'

Gem::Specification.new do |spec|
  spec.name          = "opencv-rice-ann"
  spec.version       = CVRice::ANN::VERSION
  spec.authors       = ["Aaron Marburg"]
  spec.email         = ["aaron.marburg@canterbury.ac.nz"]
  spec.summary       = %q{ANN extensions to OpenCV-Rice}
  spec.description   = %q{ANN extensions to OpenCV-Rice}
  spec.homepage      = ""
  spec.license       = "MIT"

  spec.files         = `git ls-files -z`.split("\x0")
  spec.executables   = spec.files.grep(%r{^bin/}) { |f| File.basename(f) }
  spec.test_files    = spec.files.grep(%r{^(test|spec|features)/})
  spec.require_paths = ["lib"]

  spec.extensions << "ext/opencv_rice_ann"

  spec.add_development_dependency "bundler", "~> 1.5"
  spec.add_development_dependency "rake"
  spec.add_development_dependency "yard"
  spec.add_development_dependency "rubocop"
  spec.add_development_dependency "minitest"

  spec.add_dependency "rice"
  spec.add_dependency "opencv-rice"
end
