# Include the hash_ring library
require 'fast_hash_ring'

# Helpers
module FastHashRingHelpers
  def random_string(length=50)
    (0...length).map{ ('a'..'z').to_a[rand(26)] }.join
  end
end
