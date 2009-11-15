#!/usr/bin/env ruby

require 'pathname'
$root = Pathname(__FILE__).dirname

require 'rubygems'
require 'rake'
require 'rake/clean'
require 'rake/packagetask'
require 'rake/gempackagetask'
require 'rake/testtask'
require 'rake/rdoctask'

task :spec do
  load $root.join('spec', 'spec_base.rb')
end
task :default => [ :spec ]

CLEAN.include('pkg', 'tmp')

gemspec = Gem::Specification.new do |s|
  s.name = 'fast_hash_ring'
  s.version = '0.1'
  s.authors = [ 'Flinn' ]
  s.email = 'flinn@actsasflinn.com'
  s.homepage = 'http://github.com/actsasflinn/fast_hash_ring/'
  s.platform = Gem::Platform::RUBY
  s.summary = 'A Ruby C Extension based on the pure Ruby HashRing gem'
  s.require_path = 'ext'
  s.test_file = 'spec/spec_base.rb'
  s.has_rdoc = true
  s.extra_rdoc_files = %w{ README.rdoc }

  s.files = ['CREDITS',
             'LICENSE',
             'Rakefile',
             'README.rdoc'] +
             Dir['ext/**/*.[rb|c|h]'] +
             Dir['spec/**/*'] +
             Dir['benchmarks/**/*']
  s.extensions << "ext/extconf.rb"
end

task :gemspec do
  File.open('fast_hash_ring.gemspec', 'w') do |f|
    f.write(gemspec.to_ruby)
  end
end

Rake::GemPackageTask.new(gemspec) do |pkg|
  pkg.need_tar = true
end

Rake::PackageTask.new('fast_hash_ring', '0.1') do |pkg|
  pkg.need_zip = true
  pkg.package_files = FileList[
    'COPYING',
    'Rakefile',
    'README.rdoc',
    'ext/**/*',
    'spec/**/*',
    'benchmarks/**/*'
  ].to_a
  class << pkg
    def package_name
      "#{@name}-#{@version}-src"
    end
  end
end
