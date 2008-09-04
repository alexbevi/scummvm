#!/usr/bin/env ruby

# suitable for generating opcodes_nargs.data

exefile = ARGV[0]

File.open(exefile) do |f|
	f.seek(0x315)
	first = true
	(0..0xfd).each do |i|
		printf(",\n") if !first
		first = false
		code = f.read(4)
		code = code.unpack("vv")
		printf("/* opcode %02x [%04x] */ %d", i, code[0], code[1])
	end
end

printf("\n")
