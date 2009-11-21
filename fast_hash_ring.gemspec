# -*- encoding: utf-8 -*-

Gem::Specification.new do |s|
  s.name = %q{fast_hash_ring}
  s.version = "0.1.1"

  s.required_rubygems_version = Gem::Requirement.new(">= 0") if s.respond_to? :required_rubygems_version=
  s.authors = ["Flinn"]
  s.date = %q{2009-11-21}
  s.email = %q{flinn@actsasflinn.com}
  s.extensions = ["ext/extconf.rb"]
  s.extra_rdoc_files = ["README.rdoc"]
  s.files = ["CREDITS", "LICENSE", "Rakefile", "README.rdoc", "ext/fast_hash_ring.c", "ext/fast_hash_ring.h", "ext/md5.c", "ext/md5.h", "spec/fast_hash_ring_spec.rb", "spec/spec_base.rb", "benchmarks/fast_hash_ring.rb", "ext/extconf.rb"]
  s.homepage = %q{http://github.com/actsasflinn/fast_hash_ring/}
  s.require_paths = ["ext"]
  s.rubygems_version = %q{1.3.5}
  s.summary = %q{A Ruby C Extension based on the pure Ruby HashRing gem}
  s.test_files = ["spec/spec_base.rb"]

  if s.respond_to? :specification_version then
    current_version = Gem::Specification::CURRENT_SPECIFICATION_VERSION
    s.specification_version = 3

    if Gem::Version.new(Gem::RubyGemsVersion) >= Gem::Version.new('1.2.0') then
    else
    end
  else
  end
end
