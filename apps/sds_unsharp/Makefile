IFLAGS=-I ../../include/ -I ../../tools/ -I ../../apps/support/
LFLAGS=-ldl -lpthread -lz
CFLAGS=-fno-rtti -std=c++11
CXX=g++

generator: halide_generator.cpp
	$(CXX) $(CFLAGS) $(IFLAGS) $< ../../lib/libHalide.a -o $@ $(LFLAGS)
	#c++ -std=c++11 -I ../../include/ -I ../../tools/ -I ../../apps/support/ -fno-rtti halide_generator.cpp ../../lib/libHalide.a -o generator  -ldl -lpthread -lz

hls: generator
	HL_DEBUG_CODEGEN=3 ./generator HLS

cpu: generator
	HL_DEBUG_CODEGEN=3 ./generator CPU

clean:
	rm -f generator
	rm -f ir.*.html
	rm -f app.*
	rm -f offload.*
