IFLAGS=-I ../../include/ -I ../../tools/ -I ../../apps/support/
LFLAGS=-ldl -lpthread -lz
CFLAGS=-fno-rtti -std=c++11
CXX=g++

generator: halide_generator.cpp
	$(CXX) $(CFLAGS) $(IFLAGS) $^ ../../lib/libHalide.a -o $@ $(LFLAGS)

cpu blur.o blur.h: generator
	HL_DEBUG_CODEGEN=3 ./generator CPU

hls: generator
	HL_DEBUG_CODEGEN=3 ./generator HLS

test: test.cpp blur.o
	$(CXX) $(CFLAGS) $(IFLAGS) $^ ../../lib/libHalide.a -o $@ $(LFLAGS)

all: cpu test

clean:
	rm -f generator ir.html app.* offload.*  blur.* test
