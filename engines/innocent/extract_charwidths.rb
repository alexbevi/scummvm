#!/usr/bin/env ruby

exefile = ARGV[0]

File.open(exefile) do |f|
	f.seek(0xcdcc)
	first = true
	(0...(0xd8-0x7c)).each do |i|
		printf(", ") if !first
		first = false
		code = f.read(1)
		code = code.unpack("C")
		printf("%d", code[0])
	end
end

printf("\n")
