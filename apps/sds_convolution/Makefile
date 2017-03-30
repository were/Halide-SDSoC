IFLAGS=-I../../include/ -I../../tools/ -I../sds_support/include -I../sds_support
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

test: top.cpp cpu.cpp test.cpp offload.cpp
	$(CXX) -O1 -std=gnu++11 $(IFLAGS) $^ ../sds_support/halide_runtime.a -o $@ -lpthread -ldl
	

clean:
	rm -f generator ir.*.html cpu.* top.* offload.* test

copy:
	cp *.cpp ~/sds/
	cp *.h ~/sds/
