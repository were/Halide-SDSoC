#include "Halide.h"
#include <stdio.h>

using namespace Halide;

Var x("x"), y("y"), c("c");
Var xo("xo"), yo("yo"), xi("xi"), yi("yi");

uint8_t phase = 0;

class HalidePipeline {
public:
    ImageParam input;
    std::vector<Argument> args;
    //Param<uint8_t> phase; // One bit each for x and y phase
    Func padded;
    Func red, green, blue;
    Func demosaic, lowpass_x, lowpass_y, downsample;
    Func output, hw_output;
    Func neswNeighbors, diagNeighbors, vNeighbors, hNeighbors;

    HalidePipeline()
        : input(UInt(8), 2), padded("padded"),
          demosaic("demosaic"), lowpass_x("lowpass_x"),
          lowpass_y("lowpass_y"), downsample("downsample"),
          output("output"), hw_output("offload")
    {
        padded = BoundaryConditions::repeat_edge(input);
        //padded(x, y) = input(x+240, y+60);
        //padded(x, y) = input(x+200, y+40);

        // Now demosaic and try to get RGB back
        Func padded16;
        padded16(x, y) = cast<uint16_t>(padded(x, y));
        neswNeighbors(x, y) = cast<uint8_t>((padded16(x-1, y) + padded16(x+1, y) +
                                             padded16(x, y-1) + padded16(x, y+1))/4);
        diagNeighbors(x, y) = cast<uint8_t>((padded16(x-1, y-1) + padded16(x+1, y-1) +
                                             padded16(x-1, y+1) + padded16(x+1, y+1))/4);

        vNeighbors(x, y) = cast<uint8_t>((padded16(x, y-1) + padded16(x, y+1))/2);
        hNeighbors(x, y) = cast<uint8_t>((padded16(x-1, y) + padded16(x+1, y))/2);

        green(x, y) = select((y % 2) == (phase / 2),
             select((x % 2) == (phase % 2), neswNeighbors(x, y), padded(x, y)), // First row, RG
             select((x % 2) == (phase % 2), padded(x, y), neswNeighbors(x, y))); // Second row, GB

        red(x, y) = select((y % 2) == (phase / 2),
             select((x % 2) == (phase % 2), padded(x, y), hNeighbors(x, y)), // First row, RG
             select((x % 2) == (phase % 2), vNeighbors(x, y), diagNeighbors(x, y))); // Second row, GB

        blue(x, y) = select((y % 2) == (phase / 2),
             select((x % 2) == (phase % 2), diagNeighbors(x, y), vNeighbors(x, y)), // First row, RG
             select((x % 2) == (phase % 2), hNeighbors(x, y), padded(x, y))); // Second row, GB

        demosaic(c, x, y) = cast<uint8_t>(select(c == 0, red(x, y),
                                                 c == 1, green(x, y),
                                                 blue(x, y)));

        // lowpass filter before downsample
        lowpass_y(c, x, y) = cast<uint8_t>((cast<uint16_t>(demosaic(c, x, y)) +
                                            cast<uint16_t>(demosaic(c, x+1, y)))/2);
        lowpass_x(c, x, y) = cast<uint8_t>((cast<uint16_t>(lowpass_y(c, x, y)) +
                                            cast<uint16_t>(lowpass_y(c, x, y+1)))/2);

        //hw_output(c, x, y) = demosaic(c, x, y);
        hw_output(c, x, y) = lowpass_x(c, x, y);

        // downsample
        downsample(c, x, y) = hw_output(c, x*2, y*2);

        output(x, y, c) = downsample(c, x, y);
        //output(x, y, c) = hw_output(c, x, y);

        // common constraints
        output.bound(c, 0, 3);

        // Arguments
        args = {input};

        // We can generate slightly better code if we know the output is a whole number of tiles.
        output.bound(x, 0, 480).bound(y, 0, 640);
    }

    void compile_to_cpu() {

        output.compile_to_lowered_stmt("ir.cpu.html", {input}, HTML);
        output.compile_to_c("cpu.cpp", {input}, "cpu");
        output.compile_to_header("cpu.h", {input}, "cpu");
        std::cerr << "Compiled...\n";
    }

    void compile_to_hls() {
        std::cout << "\ncompiling HLS code..." << std::endl;


        padded.compute_root();
        hw_output.compute_root();

        hw_output.tile(x, y, xo, yo, xi, yi, 480, 640).reorder(c, xi, yi, xo, yo);

        hw_output.offload({demosaic.unroll(c), lowpass_x.unroll(c)}, xo).unroll(c);

        //output.print_loop_nest();
        // Create the target for HLS simulation
        output.compile_to_lowered_stmt("ir.hls.html", args, HTML);
        output.compile_to_sdsoc("top", args, "top");
    }
};

int main(int argc, char **argv) {
    if (argc != 1 && argc != 2) {
        std::cerr << "Usage: ./generator <target>\n";
        std::cerr << "By default, it is targetted to native CPU code.\n";
        return 1;
    }

    if (argc == 1 || !strcmp(argv[1], "CPU")) {
        HalidePipeline().compile_to_cpu();
    } else if (!strcmp(argv[1], "HLS")) {
        HalidePipeline().compile_to_hls();
    }
    return 0;
}
