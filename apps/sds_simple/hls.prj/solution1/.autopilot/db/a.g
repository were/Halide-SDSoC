#!/bin/sh
lli=${LLVMINTERP-lli}
exec $lli \
    /work/zhang/common/tools/halide-working/Halide-SDSoC/apps/sds_simple/hls.prj/solution1/.autopilot/db/a.g.bc ${1+"$@"}
