IFLAGS=-I../../include/ -I../../tools/ -I ../../apps/support/ -I../sds_support -I../../bin/
LFLAGS=-ldl -lpthread -lz
CFLAGS=-fno-rtti -std=c++11
CXX=g++

generator: halide_generator.cpp
	$(CXX) $(CFLAGS) $(IFLAGS) $< ../../lib/libHalide.a -o $@ $(LFLAGS)
	#c++ -std=c++11 -I ../../include/ -I ../../tools/ -I ../../apps/support/ -fno-rtti halide_generator.cpp ../../lib/libHalide.a -o generator  -ldl -lpthread -lz

top.cpp top.h offload.cpp offload.h: generator
	HL_DEBUG_CODEGEN=3 ./generator HLS

cpu.cpp cpu.h: generator
	HL_DEBUG_CODEGEN=3 ./generator CPU


test: cpu.cpp test.cpp top.cpp offload.cpp
	$(CXX) $(CFLAGS) $(IFLAGS) $^ ../sds_support/halide_runtime.a -o $@ #$(LFLAGS)

clean:
	rm -f generator test
	rm -f ir.*.html
	rm -f app.*
	rm -f offload.*
	rm -f top.cpp top.h
	rm -f cpu.cpp cpu.h
