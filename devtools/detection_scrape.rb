#!/usr/bin/env ruby
#
# Scrape the ScummVM engines for detection entries or detection tables.
# Once located, extract the relevant game entries to build a list of
# titles that are currently supported
#
# NOTE
# This currently only works on Unix-like systems
#
def replace_target(path, target)
  p = path.split('/')
  p[-1] = target
  return p.join('/').to_s
end

target        = "detection.cpp"
fallback      = "detection_tables.h"
search_term   = "static const PlainGameDescriptor "
fallback_term = "const PlainGameDescriptor "

# XXX assume we're running from the /devtools directory, so we need
# to start searching from the parent folder
files = %x[find '../' -name '#{target}'].split("\n")

files.each do |f|
  offset  = %x[grep -n "#{search_term}" #{f}].split(':')[0].to_i
  
  if offset == 0    
    f = replace_target(f, fallback)
    offset = %x[grep -n "#{search_term}" #{f}].split(':')[0].to_i if File.exists?(f)
    # kyra
    offset = %x[grep -n "#{fallback_term}" #{f}].split(':')[0].to_i if offset == 0    
  end
  
  if offset > 0
    terminator1  = %x[sed -n '#{offset + 1}, $ p' '#{f}' | grep -n '0, 0'].split(':')[0].to_i
    terminator2  = %x[sed -n '#{offset + 1}, $ p' '#{f}' | grep -n ';'].split(':')[0].to_i
    length = (terminator1 < terminator2) ? terminator1 : terminator2
    content = %x[sed -n '#{offset + 1}, #{offset + length - 1} p' #{f}] 
    
    puts f
    puts content
    puts "\n"
  end
end

