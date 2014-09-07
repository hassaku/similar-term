# coding: utf-8
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'similar_term/version'

Gem::Specification.new do |spec|
  spec.name          = "similar-term"
  spec.version       = SimilarTerm::VERSION
  spec.authors       = ["hassaku"]
  spec.email         = ["hassaku.apps@gmail.com"]
  spec.description   = %q{List similar terms by word2vec.}
  spec.summary       = %q{List similar terms by word2vec.}
  spec.homepage      = ""
  spec.license       = "MIT"
  spec.extensions    = %w[ext/similar_term/extconf.rb]

  spec.files         = `git ls-files`.split($/)
  spec.executables   = spec.files.grep(%r{^bin/}) { |f| File.basename(f) }
  spec.test_files    = spec.files.grep(%r{^(test|spec|features)/})
  spec.require_paths = ["lib"]

  spec.add_development_dependency "bundler", "~> 1.3"
  spec.add_development_dependency "rake"
end
