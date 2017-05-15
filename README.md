Halide-SDSoC README
===================
This project is aimed to reducing the complexity of image processing specialized hardware,
which uses Halide as frontend to generate SDSoC-friendly C++ code, and SDSoC (Software defined system on chip)
as backend to deploy frontend-generated code to FPGA-based heterogeneous SoCs.
All the modifications are based on Halide master branch's `51de3bff95d4c6116cafe4c511b6956a5cc864b8` commitment.
Sorry, 

Building Halide-SDSoC
=====================
First, you should go to [original Halide repository](https://github.com/halide/Halide) to see how origin Halide is built and installed.
Installation is nearly identical to Halide with the following differences:
  * When building LLVM both X86 and ARM targets MUST be enabled.
  * Vivado HLS (later than 2016.2 recommanded, or cosim cannot be run properly) is needed for cosim on the included apps.
  * SDSoC (Latest SDSoC/SDx is recommanded, although I did all the experiments under SDSoC 2016.2.) is needed for synthesizing the included apps.

#### Enable Simulation
For csim and SDSoC compilation, we need the Halide runtime library. SDSoC especially requires the 32-bit ARM target of the library.
The following commands can be used to build Halide and the library from the root directory. Note that you need a separate 32-bit build for the ARM lib.

You may use the following command to compile a host runtime library:

    % make enable_sim # to build runtime library and test the main features

Testing the HLS features may take a while to finish. If you do not like it, you may delete the `test_sds` dependency in `enable_sim` target.

After this is done, you can go to `apps` to play with some benchmarks. All the applications under directory `apps/sds_*/` except `sds_support` are benchmarks.
Originally, there are four files under each directory:
  * `halide_generator.cpp`: The Halide AOT generator to generate CPU C++ code and HLS C++ code.
  * `test.cpp`: The testbench, the main function of generated files.
  * `sdsoc_build`: The directory for building SDSoC target.
  * `Makefile`: The Makefile linked to `../sds_support/Makefile.in`.

You may use following commands to play with each benchmark:
    % make test  # build test.exe
    % ./test.exe # after building run compiled executable to see the result of csim
    % make hls   # see the result of cosim and export_design

After typing `make test`, besides `test.exe`, there are 8 more files generated:
  * `cpu.{h/cpp}`: The CPU C++ code. The baseline design.
  * `top.{h/cpp}`: The software function which invokes FPGA. I apologize that the name of the software function is confusing.
  * `offload.{h/cpp}`: Hardware function deployed on FPGA.
  * `gen.{hls/cpu}.log`: The compiling log of both target.

#### Enable SDSoC Target
In the root directory, you may use the following command to compile runtime library for sdsoc:
*(This is untested on our server. Becasue some unknown error, this command cannot be run properly on the server.
What we did is to compile Halide locally and run it locally. At last, we scp it to the server.
If you can run it properly, tell me your settings of LLVM.)*

    % make enable_sdsoc

After runtime library generation is done, and with `cpu.{cpp/h}`, `top.{cpp/h}`, and `offload.{cpp/h}` in the directory,
you may go to `sdsoc_build` directory and type `make all` to get FPGA deployment, which may take a while to finish.
Right now, our `Makefile` is targeted to Zed (Also, right now, only Zed target is tested. If anything wrong with the generated
result target to FPGA board other than Zed, ~~I think~~ it MUST be SDSoC's bug).
If you want to target to other boards, you need to modify the Makefile in `../../sds_support/Makefile.sdsoc.in`.

When SDSoC compilation is done, you may get a directory whose name is `sd_card`. Copy everything under this directory to
the FPGA board's `/mnt/` and reboot the FPGA board to reconfigure it. Then you may run the heterogeneous application on
FPGA board.

