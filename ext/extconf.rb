# ENV["RC_ARCHS"] = `uname -m`.chomp if `uname -sr` =~ /^Darwin/
#
# require 'mkmf'
#

if RUBY_PLATFORM =~ /darwin/
  ENV["RC_ARCHS"] = `uname -m`.chomp if `uname -sr` =~ /^Darwin/

  ENV["RC_ARCHS"] = "x86_64" if RUBY_PLATFORM =~ /darwin10.0/

  # On PowerPC the defaults are fine
  ENV["RC_ARCHS"] = '' if `uname -m` =~ /^Power Macintosh/
end

# Loads mkmf which is used to make makefiles for Ruby extensions
require 'mkmf'

# Give it a name
extension_name = 'fast_hash_ring'

dir_config("fast_hash_ring")

# NOTE: use GCC flags unless Visual C compiler is used
$CFLAGS << ' -Wall ' unless RUBY_PLATFORM =~ /mswin/

if RUBY_VERSION < '1.8.6'
  $CFLAGS << ' -DRUBY_LESS_THAN_186'
end

create_makefile 'fast_hash_ring'
