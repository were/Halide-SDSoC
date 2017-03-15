; ModuleID = '/work/zhang/common/tools/halide-working/Halide-SDSoC/apps/sds_simple/hls.prj/solution1/.autopilot/db/a.o.2.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@offload_str = internal unnamed_addr constant [8 x i8] c"offload\00"
@llvm_global_ctors_1 = appending global [1 x void ()*] [void ()* @_GLOBAL__I_a]
@llvm_global_ctors_0 = appending global [1 x i32] [i32 65535]
@ap_fifo_str = internal unnamed_addr constant [8 x i8] c"ap_fifo\00"
@p_p0_to_offload_s0_stream_OC_V_s = internal unnamed_addr constant [27 x i8] c"_p0_to_offload_s0_stream.V\00"
@p_str1811 = private unnamed_addr constant [17 x i8] c"_offload_s0_x_xi\00", align 1
@p_str1810 = private unnamed_addr constant [17 x i8] c"_offload_s0_y_yi\00", align 1
@p_str1807 = private unnamed_addr constant [18 x i8] c"_distributor_p0_0\00", align 1
@p_str1806 = private unnamed_addr constant [18 x i8] c"_distributor_p0_1\00", align 1
@p_str1805 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@p_str = internal unnamed_addr constant [1 x i8] zeroinitializer

define void @offload([10000 x i8]* %p_p0, [10000 x i8]* %p_offload) nounwind uwtable {
codeRepl:
  call void (...)* @_ssdm_op_SpecDataflowPipeline(i32 -1, [1 x i8]* @p_str1805) nounwind
  call void (...)* @_ssdm_op_SpecBitsMap([10000 x i8]* %p_p0) nounwind, !map !68
  call void (...)* @_ssdm_op_SpecBitsMap([10000 x i8]* %p_offload) nounwind, !map !74
  call void (...)* @_ssdm_op_SpecTopModule([8 x i8]* @offload_str) nounwind
  %p_p0_to_offload_s0_stream_V = alloca i8, align 1
  %empty = call i32 (...)* @_ssdm_op_SpecChannel([27 x i8]* @p_p0_to_offload_s0_stream_OC_V_s, i32 1, [1 x i8]* @p_str, [1 x i8]* @p_str, i32 1, i32 1, i8* %p_p0_to_offload_s0_stream_V, i8* %p_p0_to_offload_s0_stream_V) nounwind
  %empty_4 = call i32 (...)* @_ssdm_op_SpecInterface(i8* %p_p0_to_offload_s0_stream_V, [8 x i8]* @ap_fifo_str, i32 0, i32 0, i32 0, i32 0, [1 x i8]* @p_str, [1 x i8]* @p_str, [1 x i8]* @p_str, i32 2, i32 2, i32 16, i32 16, [1 x i8]* @p_str) nounwind
  call fastcc void @offload_Loop__distributor_p0_1_proc([10000 x i8]* %p_p0, i8* %p_p0_to_offload_s0_stream_V) nounwind
  call fastcc void @offload_Loop__offload_s0_y_yi_proc(i8* %p_p0_to_offload_s0_stream_V, [10000 x i8]* %p_offload) nounwind
  ret void
}

declare void @llvm.dbg.value(metadata, i64, metadata) nounwind readnone

declare void @llvm.dbg.declare(metadata, metadata) nounwind readnone

define weak void @_ssdm_op_Write.ap_fifo.volatile.i8P(i8*, i8) {
entry:
  %empty = call i8 @_autotb_FifoWrite_i8(i8* %0, i8 %1)
  ret void
}

define weak void @_ssdm_op_SpecTopModule(...) {
entry:
  ret void
}

define weak i32 @_ssdm_op_SpecRegionEnd(...) {
entry:
  ret i32 0
}

define weak i32 @_ssdm_op_SpecRegionBegin(...) {
entry:
  ret i32 0
}

define weak void @_ssdm_op_SpecPipeline(...) nounwind {
entry:
  ret void
}

define weak i32 @_ssdm_op_SpecLoopTripCount(...) {
entry:
  ret i32 0
}

define weak void @_ssdm_op_SpecLoopName(...) nounwind {
entry:
  ret void
}

define weak i32 @_ssdm_op_SpecInterface(...) {
entry:
  ret i32 0
}

define weak void @_ssdm_op_SpecDataflowPipeline(...) nounwind {
entry:
  ret void
}

define weak i32 @_ssdm_op_SpecChannel(...) {
entry:
  ret i32 0
}

define weak void @_ssdm_op_SpecBitsMap(...) {
entry:
  ret void
}

define weak i8 @_ssdm_op_Read.ap_fifo.volatile.i8P(i8*) {
entry:
  %empty = call i8 @_autotb_FifoRead_i8(i8* %0)
  ret i8 %empty
}

declare i8 @_autotb_FifoWrite_i8(i8*, i8)

declare i8 @_autotb_FifoRead_i8(i8*)

declare void @_GLOBAL__I_a() nounwind section ".text.startup"

define internal fastcc void @offload_Loop__offload_s0_y_yi_proc(i8* %p_p0_to_offload_s0_stream_V, [10000 x i8]* nocapture %p_offload) {
newFuncRoot:
  %empty = call i32 (...)* @_ssdm_op_SpecInterface(i8* %p_p0_to_offload_s0_stream_V, [8 x i8]* @ap_fifo_str, i32 0, i32 0, i32 0, i32 0, [1 x i8]* @p_str, [1 x i8]* @p_str, [1 x i8]* @p_str, i32 2, i32 2, i32 16, i32 16, [1 x i8]* @p_str)
  br label %.preheader

.exitStub:                                        ; preds = %.preheader
  ret void

.preheader:                                       ; preds = %0, %newFuncRoot
  %p_offload_s0_y_yi = phi i7 [ %p_offload_s0_y_yi_1, %0 ], [ 0, %newFuncRoot ]
  %phi_mul = phi i14 [ %next_mul, %0 ], [ 0, %newFuncRoot ]
  %next_mul = add i14 %phi_mul, 100
  %exitcond4 = icmp eq i7 %p_offload_s0_y_yi, -28
  %empty_5 = call i32 (...)* @_ssdm_op_SpecLoopTripCount(i64 100, i64 100, i64 100) nounwind
  %p_offload_s0_y_yi_1 = add i7 %p_offload_s0_y_yi, 1
  br i1 %exitcond4, label %.exitStub, label %2

; <label>:0                                       ; preds = %1
  %empty_6 = call i32 (...)* @_ssdm_op_SpecRegionEnd([17 x i8]* @p_str1810, i32 %tmp_2) nounwind
  br label %.preheader

; <label>:1                                       ; preds = %3, %2
  %p_offload_s0_x_xi = phi i7 [ 0, %2 ], [ %p_offload_s0_x_xi_1, %3 ]
  %p_offload_s0_x_xi_cast8 = zext i7 %p_offload_s0_x_xi to i14
  %exitcond = icmp eq i7 %p_offload_s0_x_xi, -28
  %empty_7 = call i32 (...)* @_ssdm_op_SpecLoopTripCount(i64 100, i64 100, i64 100) nounwind
  %p_offload_s0_x_xi_1 = add i7 %p_offload_s0_x_xi, 1
  br i1 %exitcond, label %0, label %3

; <label>:2                                       ; preds = %.preheader
  call void (...)* @_ssdm_op_SpecLoopName([17 x i8]* @p_str1810) nounwind
  %tmp_2 = call i32 (...)* @_ssdm_op_SpecRegionBegin([17 x i8]* @p_str1810) nounwind
  br label %1

; <label>:3                                       ; preds = %1
  call void (...)* @_ssdm_op_SpecLoopName([17 x i8]* @p_str1811) nounwind
  %tmp_8 = call i32 (...)* @_ssdm_op_SpecRegionBegin([17 x i8]* @p_str1811) nounwind
  call void (...)* @_ssdm_op_SpecPipeline(i32 1, i32 1, i32 1, i32 0, [1 x i8]* @p_str1805) nounwind
  %p_271 = call i8 @_ssdm_op_Read.ap_fifo.volatile.i8P(i8* %p_p0_to_offload_s0_stream_V)
  %p_273 = add i14 %phi_mul, %p_offload_s0_x_xi_cast8
  %tmp_6 = zext i14 %p_273 to i64
  %p_offload_addr = getelementptr [10000 x i8]* %p_offload, i64 0, i64 %tmp_6
  store i8 %p_271, i8* %p_offload_addr, align 1
  %empty_8 = call i32 (...)* @_ssdm_op_SpecRegionEnd([17 x i8]* @p_str1811, i32 %tmp_8) nounwind
  br label %1
}

define internal fastcc void @offload_Loop__distributor_p0_1_proc([10000 x i8]* nocapture %p_p0, i8* %p_p0_to_offload_s0_stream_V) {
newFuncRoot:
  %empty = call i32 (...)* @_ssdm_op_SpecInterface(i8* %p_p0_to_offload_s0_stream_V, [8 x i8]* @ap_fifo_str, i32 0, i32 0, i32 0, i32 0, [1 x i8]* @p_str, [1 x i8]* @p_str, [1 x i8]* @p_str, i32 2, i32 2, i32 16, i32 16, [1 x i8]* @p_str)
  br label %0

.preheader.exitStub:                              ; preds = %0
  ret void

; <label>:0                                       ; preds = %1, %newFuncRoot
  %p_distributor_p0_1_0_i_i = phi i7 [ 0, %newFuncRoot ], [ %p_distributor_p0_1, %1 ]
  %phi_mul = phi i14 [ 0, %newFuncRoot ], [ %next_mul, %1 ]
  %next_mul = add i14 %phi_mul, 100
  %exitcond6_i_i = icmp eq i7 %p_distributor_p0_1_0_i_i, -28
  %empty_9 = call i32 (...)* @_ssdm_op_SpecLoopTripCount(i64 100, i64 100, i64 100) nounwind
  %p_distributor_p0_1 = add i7 %p_distributor_p0_1_0_i_i, 1
  br i1 %exitcond6_i_i, label %.preheader.exitStub, label %3

; <label>:1                                       ; preds = %2
  %empty_10 = call i32 (...)* @_ssdm_op_SpecRegionEnd([18 x i8]* @p_str1806, i32 %tmp) nounwind
  br label %0

; <label>:2                                       ; preds = %4, %3
  %p_distributor_p0_0_0_i_i = phi i7 [ 0, %3 ], [ %p_distributor_p0_0, %4 ]
  %p_distributor_p0_0_0_i_i_cast = zext i7 %p_distributor_p0_0_0_i_i to i14
  %exitcond5_i_i = icmp eq i7 %p_distributor_p0_0_0_i_i, -28
  %empty_11 = call i32 (...)* @_ssdm_op_SpecLoopTripCount(i64 100, i64 100, i64 100) nounwind
  %p_distributor_p0_0 = add i7 %p_distributor_p0_0_0_i_i, 1
  br i1 %exitcond5_i_i, label %1, label %4

; <label>:3                                       ; preds = %0
  call void (...)* @_ssdm_op_SpecLoopName([18 x i8]* @p_str1806) nounwind
  %tmp = call i32 (...)* @_ssdm_op_SpecRegionBegin([18 x i8]* @p_str1806) nounwind
  br label %2

; <label>:4                                       ; preds = %2
  call void (...)* @_ssdm_op_SpecLoopName([18 x i8]* @p_str1807) nounwind
  %tmp_4 = call i32 (...)* @_ssdm_op_SpecRegionBegin([18 x i8]* @p_str1807) nounwind
  call void (...)* @_ssdm_op_SpecPipeline(i32 1, i32 1, i32 1, i32 0, [1 x i8]* @p_str1805) nounwind
  %p_268 = add i14 %phi_mul, %p_distributor_p0_0_0_i_i_cast
  %tmp_i = zext i14 %p_268 to i64
  %p_p0_addr = getelementptr [10000 x i8]* %p_p0, i64 0, i64 %tmp_i
  %p_269 = load i8* %p_p0_addr, align 1
  call void @_ssdm_op_Write.ap_fifo.volatile.i8P(i8* %p_p0_to_offload_s0_stream_V, i8 %p_269)
  %empty_12 = call i32 (...)* @_ssdm_op_SpecRegionEnd([18 x i8]* @p_str1807, i32 %tmp_4) nounwind
  br label %2
}

!opencl.kernels = !{!0, !7, !13, !7, !7, !19, !19, !22, !7, !24, !24, !7, !27, !7, !7, !7, !31, !31, !33, !33, !35, !7, !7, !7, !38, !24, !24, !7, !41, !7, !7, !7, !43, !43, !45, !45, !47, !7, !7, !7, !7, !7, !7, !49, !7, !7, !7, !7, !19, !19, !22, !49, !7, !19, !19, !22, !49, !7, !51, !51, !51, !7, !51, !51, !51, !7, !51, !51, !51, !7, !51, !51, !51, !51, !51, !51, !51, !51, !51, !7, !51, !51, !51, !7, !51, !51, !51, !7, !51, !51, !51, !7, !51, !51, !51, !7, !51, !51, !51, !51, !51, !51, !56, !56, !56, !7, !51, !51, !51, !7, !51, !51, !51, !7, !51, !51, !51, !7, !56, !56, !56, !7, !56, !56, !56, !7, !7, !7, !7, !7, !7}
!hls.encrypted.func = !{}
!llvm.map.gv = !{!61}

!0 = metadata !{null, metadata !1, metadata !2, metadata !3, metadata !4, metadata !5, metadata !6}
!1 = metadata !{metadata !"kernel_arg_addr_space", i32 1, i32 1}
!2 = metadata !{metadata !"kernel_arg_access_qual", metadata !"none", metadata !"none"}
!3 = metadata !{metadata !"kernel_arg_type", metadata !"uint8_t*", metadata !"uint8_t*"}
!4 = metadata !{metadata !"kernel_arg_type_qual", metadata !"", metadata !""}
!5 = metadata !{metadata !"kernel_arg_name", metadata !"_p0", metadata !"_offload"}
!6 = metadata !{metadata !"reqd_work_group_size", i32 1, i32 1, i32 1}
!7 = metadata !{null, metadata !8, metadata !9, metadata !10, metadata !11, metadata !12, metadata !6}
!8 = metadata !{metadata !"kernel_arg_addr_space"}
!9 = metadata !{metadata !"kernel_arg_access_qual"}
!10 = metadata !{metadata !"kernel_arg_type"}
!11 = metadata !{metadata !"kernel_arg_type_qual"}
!12 = metadata !{metadata !"kernel_arg_name"}
!13 = metadata !{null, metadata !14, metadata !15, metadata !16, metadata !17, metadata !18, metadata !6}
!14 = metadata !{metadata !"kernel_arg_addr_space", i32 0}
!15 = metadata !{metadata !"kernel_arg_access_qual", metadata !"none"}
!16 = metadata !{metadata !"kernel_arg_type", metadata !"const uchar &"}
!17 = metadata !{metadata !"kernel_arg_type_qual", metadata !""}
!18 = metadata !{metadata !"kernel_arg_name", metadata !"din"}
!19 = metadata !{null, metadata !14, metadata !15, metadata !20, metadata !17, metadata !21, metadata !6}
!20 = metadata !{metadata !"kernel_arg_type", metadata !"double"}
!21 = metadata !{metadata !"kernel_arg_name", metadata !"v"}
!22 = metadata !{null, metadata !14, metadata !15, metadata !20, metadata !17, metadata !23, metadata !6}
!23 = metadata !{metadata !"kernel_arg_name", metadata !"d"}
!24 = metadata !{null, metadata !14, metadata !15, metadata !25, metadata !17, metadata !26, metadata !6}
!25 = metadata !{metadata !"kernel_arg_type", metadata !"int"}
!26 = metadata !{metadata !"kernel_arg_name", metadata !"op"}
!27 = metadata !{null, metadata !28, metadata !2, metadata !29, metadata !4, metadata !30, metadata !6}
!28 = metadata !{metadata !"kernel_arg_addr_space", i32 0, i32 0}
!29 = metadata !{metadata !"kernel_arg_type", metadata !"const ap_int_base<1, false> &", metadata !"const ap_int_base<54, true> &"}
!30 = metadata !{metadata !"kernel_arg_name", metadata !"op", metadata !"op2"}
!31 = metadata !{null, metadata !14, metadata !15, metadata !32, metadata !17, metadata !26, metadata !6}
!32 = metadata !{metadata !"kernel_arg_type", metadata !"const ap_int_base<54, true> &"}
!33 = metadata !{null, metadata !14, metadata !15, metadata !34, metadata !17, metadata !26, metadata !6}
!34 = metadata !{metadata !"kernel_arg_type", metadata !"const ap_int_base<1, false> &"}
!35 = metadata !{null, metadata !14, metadata !15, metadata !36, metadata !17, metadata !37, metadata !6}
!36 = metadata !{metadata !"kernel_arg_type", metadata !"const ap_int_base<55, true> &"}
!37 = metadata !{metadata !"kernel_arg_name", metadata !"op2"}
!38 = metadata !{null, metadata !28, metadata !2, metadata !39, metadata !4, metadata !40, metadata !6}
!39 = metadata !{metadata !"kernel_arg_type", metadata !"const ap_int_base<11, false> &", metadata !"int"}
!40 = metadata !{metadata !"kernel_arg_name", metadata !"op", metadata !"i_op"}
!41 = metadata !{null, metadata !28, metadata !2, metadata !42, metadata !4, metadata !30, metadata !6}
!42 = metadata !{metadata !"kernel_arg_type", metadata !"const ap_int_base<11, false> &", metadata !"const ap_int_base<32, true> &"}
!43 = metadata !{null, metadata !14, metadata !15, metadata !44, metadata !17, metadata !26, metadata !6}
!44 = metadata !{metadata !"kernel_arg_type", metadata !"const ap_int_base<32, true> &"}
!45 = metadata !{null, metadata !14, metadata !15, metadata !46, metadata !17, metadata !26, metadata !6}
!46 = metadata !{metadata !"kernel_arg_type", metadata !"const ap_int_base<11, false> &"}
!47 = metadata !{null, metadata !14, metadata !15, metadata !48, metadata !17, metadata !37, metadata !6}
!48 = metadata !{metadata !"kernel_arg_type", metadata !"const ap_int_base<33, true> &"}
!49 = metadata !{null, metadata !14, metadata !15, metadata !20, metadata !17, metadata !50, metadata !6}
!50 = metadata !{metadata !"kernel_arg_name", metadata !"pf"}
!51 = metadata !{null, metadata !52, metadata !15, metadata !53, metadata !54, metadata !55, metadata !6}
!52 = metadata !{metadata !"kernel_arg_addr_space", i32 1}
!53 = metadata !{metadata !"kernel_arg_type", metadata !"char*"}
!54 = metadata !{metadata !"kernel_arg_type_qual", metadata !"const"}
!55 = metadata !{metadata !"kernel_arg_name", metadata !"str"}
!56 = metadata !{null, metadata !57, metadata !2, metadata !58, metadata !59, metadata !60, metadata !6}
!57 = metadata !{metadata !"kernel_arg_addr_space", i32 1, i32 0}
!58 = metadata !{metadata !"kernel_arg_type", metadata !"char*", metadata !"signed char"}
!59 = metadata !{metadata !"kernel_arg_type_qual", metadata !"const", metadata !""}
!60 = metadata !{metadata !"kernel_arg_name", metadata !"str", metadata !"radix"}
!61 = metadata !{metadata !62, [1 x i32]* @llvm_global_ctors_0}
!62 = metadata !{metadata !63}
!63 = metadata !{i32 0, i32 31, metadata !64}
!64 = metadata !{metadata !65}
!65 = metadata !{metadata !"llvm.global_ctors.0", metadata !66, metadata !"", i32 0, i32 31}
!66 = metadata !{metadata !67}
!67 = metadata !{i32 0, i32 0, i32 1}
!68 = metadata !{metadata !69}
!69 = metadata !{i32 0, i32 7, metadata !70}
!70 = metadata !{metadata !71}
!71 = metadata !{metadata !"_p0", metadata !72, metadata !"unsigned char", i32 0, i32 7}
!72 = metadata !{metadata !73}
!73 = metadata !{i32 0, i32 9999, i32 1}
!74 = metadata !{metadata !75}
!75 = metadata !{i32 0, i32 7, metadata !76}
!76 = metadata !{metadata !77}
!77 = metadata !{metadata !"_offload", metadata !72, metadata !"unsigned char", i32 0, i32 7}
