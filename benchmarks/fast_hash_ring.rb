require 'benchmark'
require 'rubygems'
require 'faker'
require 'hash_ring'
require 'fast_hash_ring'

servers = []
16.times do |i|
  "127.0.0.#{i+1}:12345"
end

weights = {}
4.times do |i|
  weights["127.0.0.#{i+1}:12345"] = 8
end

keys = []
10_000.times do |i|
  keys << Faker::Name.name
end

Benchmark.benchmark(' ' * 20 + Benchmark::Tms::CAPTION, 20) do |b|
  h = HashRing.new(servers, weights)
  b.report('Hash Ring') do
    keys.each{ |key| server = h.get_node(key) }
  end

  fh = FastHashRing.new(servers, weights)
  b.report('Fash Hash Ring') do
    keys.each{ |key| server = fh.get_node(key) }
  end
end
