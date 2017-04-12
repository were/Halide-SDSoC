#set VHLS_DIR "/work/zhang/common/tools/xilinx/SDSoC/2016.1/Vivado_HLS/2016.1/"
#set VHLS_DIR "/scratch/common/tools/xilinx/Vivado/2015.4/"
set IFLAGS "-I../../include/ -I../../tools/ -I../sds_support"
set LFLAGS "$::env(RUN_PATH)/../sds_support/halide_runtime.a -ldl -lpthread -lz -ltinfo"

open_project hls.prj

set_top "offload"
add_files -cflags "-std=gnu++0x $IFLAGS" offload.cpp
add_files -tb -cflags "-std=gnu++0x $IFLAGS" top.cpp
add_files -tb -cflags "-std=gnu++0x $IFLAGS" cpu.cpp
add_files -tb -cflags "-std=gnu++0x $IFLAGS" test.cpp

open_solution "solution1" -reset

set_part {xc7z020clg484-1}
create_clock -period 5

csim_design -O -ldflags ${LFLAGS}

csynth_design

cosim_design -rtl verilog -trace_level all -ldflags ${LFLAGS}

#
# export the RTL design as a Xilix IP Catalog
export_design -evaluate verilog -flow impl

exit
