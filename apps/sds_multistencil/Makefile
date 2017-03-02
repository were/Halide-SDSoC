#VHLS_DIR := $(dir $(shell which vivado_hls))..
#VHLS_DIR=/scratch/common/tools/xilinx/Vivado_HLS/2015.4/
IFLAGS=-I../../include/ -I../../tools/ -I../sds_support #-I$(VHLS_DIR)/include
LFLAGS=-ldl -lpthread -lz #-ltinfo
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
	$(CXX) -O1 -std=gnu++11 $(IFLAGS) $^ ../sds_support/halide_runtime.a -o $@ -lpthread -ldl

clean:
	rm -f generator test
	rm -f ir.*.html *.o *.d
	rm -f app.*
	rm -f offload.*
	rm -f top.cpp top.h
	rm -f cpu.cpp cpu.h
