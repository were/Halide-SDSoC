set VHLS_DIR "/work/zhang/common/tools/xilinx/SDSoC/2016.1/Vivado_HLS/2016.1/"
set IFLAGS "-std=c++0x -I../../include/ -I../../tools/ -I$VHLS_DIR/include"
set LFLAGS "../sds_support/halide_runtime.a -ldl -lpthread -lz -ltinfo"

open_project hls.prj

set_top "offload"
add_files -cflags $IFLAGS offload.cpp
add_files -tb -cflags $IFLAGS top.cpp
add_files -tb -cflags $IFLAGS cpu.cpp
add_files -tb -cflags $IFLAGS test.cpp

open_solution "solution1" -reset

set_part {xc7z020clg484-1}
create_clock -period 5

csim_design -O -ldflags ${LFLAGS}

csynth_design

#cosim_design -rtl verilog -trace_level all
#
# export the RTL design as a Xilix IP Catalog
export_design -format ip_catalog

exit
