# frozen_string_literal: true

require_relative "lib/fastly/compute_edge/version"

Gem::Specification.new do |spec|
  spec.name = "fastly-compute_edge"
  spec.version = Fastly::ComputeEdge::VERSION
  spec.authors = ["Yuta Saito"]
  spec.email = ["kateinoigakukun@gmail.com"]

  spec.summary = "NO SUMMARY"
  spec.description = "NO DESCRIPTION"
  spec.license = "MIT"
  spec.required_ruby_version = ">= 2.6.0"

  spec.files = Dir.chdir(File.expand_path(__dir__)) do
    `git ls-files -z`.split("\x0").reject do |f|
      (f == __FILE__) || f.match(%r{\A(?:(?:test|spec|features)/|\.(?:git|travis|circleci)|appveyor)})
    end
  end
  spec.bindir = "exe"
  spec.executables = spec.files.grep(%r{\Aexe/}) { |f| File.basename(f) }
  spec.require_paths = ["lib"]
end
