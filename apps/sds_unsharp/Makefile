IFLAGS=-I../../include/ -I../../tools/ -I../sds_support/include -I../sds_support
LFLAGS=-ldl -lpthread -lz
CFLAGS=-fno-rtti -std=c++11
CXX=g++

generator: halide_generator.cpp
	$(CXX) $(CFLAGS) $(IFLAGS) $< ../../lib/libHalide.a -o $@ $(LFLAGS)

top.cpp top.h offload.cpp offload.h: generator
	HL_DEBUG_CODEGEN=3 ./generator HLS

cpu.cpp cpu.h: generator
	HL_DEBUG_CODEGEN=3 ./generator CPU

test: cpu.cpp test.cpp offload.cpp top.cpp
	$(CXX) -O1 -std=gnu++11 $(IFLAGS) $^ ../sds_support/halide_runtime.a -o $@ -lpthread -ldl

clean:
	rm -f generator test
	rm -f ir.*.html *.o *.d
	rm -f app.*
	rm -f offload.*
	rm -f top.cpp top.h
	rm -f cpu.cpp cpu.h

copy:
	cp *.cpp ~/sds/
	cp *.h ~/sds/
