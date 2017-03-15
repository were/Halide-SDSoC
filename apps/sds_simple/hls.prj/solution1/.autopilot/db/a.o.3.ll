; ModuleID = '/work/zhang/common/tools/halide-working/Halide-SDSoC/apps/sds_simple/hls.prj/solution1/.autopilot/db/a.o.3.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@offload_str = internal unnamed_addr constant [8 x i8] c"offload\00" ; [#uses=1 type=[8 x i8]*]
@llvm_global_ctors_1 = appending global [1 x void ()*] [void ()* @_GLOBAL__I_a] ; [#uses=0 type=[1 x void ()*]*]
@llvm_global_ctors_0 = appending global [1 x i32] [i32 65535] ; [#uses=0 type=[1 x i32]*]
@ap_fifo_str = internal unnamed_addr constant [8 x i8] c"ap_fifo\00" ; [#uses=3 type=[8 x i8]*]
@p_p0_to_offload_s0_stream_OC_V_s = internal unnamed_addr constant [27 x i8] c"_p0_to_offload_s0_stream.V\00" ; [#uses=1 type=[27 x i8]*]
@p_str1811 = private unnamed_addr constant [17 x i8] c"_offload_s0_x_xi\00", align 1 ; [#uses=3 type=[17 x i8]*]
@p_str1810 = private unnamed_addr constant [17 x i8] c"_offload_s0_y_yi\00", align 1 ; [#uses=3 type=[17 x i8]*]
@p_str1807 = private unnamed_addr constant [18 x i8] c"_distributor_p0_0\00", align 1 ; [#uses=3 type=[18 x i8]*]
@p_str1806 = private unnamed_addr constant [18 x i8] c"_distributor_p0_1\00", align 1 ; [#uses=3 type=[18 x i8]*]
@p_str1805 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1 ; [#uses=3 type=[1 x i8]*]
@p_str = internal unnamed_addr constant [1 x i8] zeroinitializer ; [#uses=14 type=[1 x i8]*]

; [#uses=0]
define void @offload([10000 x i8]* %p_p0, [10000 x i8]* %p_offload) nounwind uwtable {
codeRepl:
  call void (...)* @_ssdm_op_SpecDataflowPipeline(i32 -1, [1 x i8]* @p_str1805) nounwind, !dbg !68 ; [debug line = 135:1]
  call void (...)* @_ssdm_op_SpecBitsMap([10000 x i8]* %p_p0) nounwind, !map !79
  call void (...)* @_ssdm_op_SpecBitsMap([10000 x i8]* %p_offload) nounwind, !map !85
  call void (...)* @_ssdm_op_SpecTopModule([8 x i8]* @offload_str) nounwind
  %p_p0_to_offload_s0_stream_V = alloca i8, align 1 ; [#uses=5 type=i8*]
  %empty = call i32 (...)* @_ssdm_op_SpecChannel([27 x i8]* @p_p0_to_offload_s0_stream_OC_V_s, i32 1, [1 x i8]* @p_str, [1 x i8]* @p_str, i32 1, i32 1, i8* %p_p0_to_offload_s0_stream_V, i8* %p_p0_to_offload_s0_stream_V) nounwind ; [#uses=0 type=i32]
  %empty_4 = call i32 (...)* @_ssdm_op_SpecInterface(i8* %p_p0_to_offload_s0_stream_V, [8 x i8]* @ap_fifo_str, i32 0, i32 0, i32 0, i32 0, [1 x i8]* @p_str, [1 x i8]* @p_str, [1 x i8]* @p_str, i32 2, i32 2, i32 16, i32 16, [1 x i8]* @p_str) nounwind ; [#uses=0 type=i32]
  call void @llvm.dbg.value(metadata !{[10000 x i8]* %p_p0}, i64 0, metadata !89), !dbg !93 ; [debug line = 133:10] [debug variable = _p0]
  call void @llvm.dbg.value(metadata !{[10000 x i8]* %p_offload}, i64 0, metadata !94), !dbg !95 ; [debug line = 134:10] [debug variable = _offload]
  call fastcc void @offload_Loop__distributor_p0_1_proc([10000 x i8]* %p_p0, i8* %p_p0_to_offload_s0_stream_V) nounwind
  call fastcc void @offload_Loop__offload_s0_y_yi_proc(i8* %p_p0_to_offload_s0_stream_V, [10000 x i8]* %p_offload) nounwind
  ret void, !dbg !96                              ; [debug line = 179:1]
}

; [#uses=13]
declare void @llvm.dbg.value(metadata, i64, metadata) nounwind readnone

; [#uses=1]
declare void @llvm.dbg.declare(metadata, metadata) nounwind readnone

; [#uses=1]
define weak void @_ssdm_op_Write.ap_fifo.volatile.i8P(i8*, i8) {
entry:
  %empty = call i8 @_autotb_FifoWrite_i8(i8* %0, i8 %1) ; [#uses=0 type=i8]
  ret void
}

; [#uses=1]
define weak void @_ssdm_op_SpecTopModule(...) {
entry:
  ret void
}

; [#uses=4]
define weak i32 @_ssdm_op_SpecRegionEnd(...) {
entry:
  ret i32 0
}

; [#uses=4]
define weak i32 @_ssdm_op_SpecRegionBegin(...) {
entry:
  ret i32 0
}

; [#uses=2]
define weak void @_ssdm_op_SpecPipeline(...) nounwind {
entry:
  ret void
}

; [#uses=4]
define weak i32 @_ssdm_op_SpecLoopTripCount(...) {
entry:
  ret i32 0
}

; [#uses=4]
define weak void @_ssdm_op_SpecLoopName(...) nounwind {
entry:
  ret void
}

; [#uses=3]
define weak i32 @_ssdm_op_SpecInterface(...) {
entry:
  ret i32 0
}

; [#uses=1]
define weak void @_ssdm_op_SpecDataflowPipeline(...) nounwind {
entry:
  ret void
}

; [#uses=1]
define weak i32 @_ssdm_op_SpecChannel(...) {
entry:
  ret i32 0
}

; [#uses=2]
define weak void @_ssdm_op_SpecBitsMap(...) {
entry:
  ret void
}

; [#uses=1]
define weak i8 @_ssdm_op_Read.ap_fifo.volatile.i8P(i8*) {
entry:
  %empty = call i8 @_autotb_FifoRead_i8(i8* %0)   ; [#uses=1 type=i8]
  ret i8 %empty
}

; [#uses=1]
declare i8 @_autotb_FifoWrite_i8(i8*, i8)

; [#uses=1]
declare i8 @_autotb_FifoRead_i8(i8*)

; [#uses=1]
declare void @_GLOBAL__I_a() nounwind section ".text.startup"

; [#uses=1]
define internal fastcc void @offload_Loop__offload_s0_y_yi_proc(i8* %p_p0_to_offload_s0_stream_V, [10000 x i8]* nocapture %p_offload) {
newFuncRoot:
  %empty = call i32 (...)* @_ssdm_op_SpecInterface(i8* %p_p0_to_offload_s0_stream_V, [8 x i8]* @ap_fifo_str, i32 0, i32 0, i32 0, i32 0, [1 x i8]* @p_str, [1 x i8]* @p_str, [1 x i8]* @p_str, i32 2, i32 2, i32 16, i32 16, [1 x i8]* @p_str) ; [#uses=0 type=i32]
  br label %.preheader

.exitStub:                                        ; preds = %.preheader
  ret void

.preheader:                                       ; preds = %0, %newFuncRoot
  %p_offload_s0_y_yi = phi i7 [ %p_offload_s0_y_yi_1, %0 ], [ 0, %newFuncRoot ] ; [#uses=2 type=i7]
  %phi_mul = phi i14 [ %next_mul, %0 ], [ 0, %newFuncRoot ] ; [#uses=2 type=i14]
  %next_mul = add i14 %phi_mul, 100               ; [#uses=1 type=i14]
  %exitcond4 = icmp eq i7 %p_offload_s0_y_yi, -28, !dbg !97 ; [#uses=1 type=i1] [debug line = 160:31]
  %empty_5 = call i32 (...)* @_ssdm_op_SpecLoopTripCount(i64 100, i64 100, i64 100) nounwind ; [#uses=0 type=i32]
  %p_offload_s0_y_yi_1 = add i7 %p_offload_s0_y_yi, 1, !dbg !99 ; [#uses=1 type=i7] [debug line = 160:61]
  br i1 %exitcond4, label %.exitStub, label %2, !dbg !97 ; [debug line = 160:31]

; <label>:0                                       ; preds = %1
  %empty_6 = call i32 (...)* @_ssdm_op_SpecRegionEnd([17 x i8]* @p_str1810, i32 %tmp_2) nounwind, !dbg !100 ; [#uses=0 type=i32] [debug line = 178:2]
  call void @llvm.dbg.value(metadata !{i7 %p_offload_s0_y_yi_1}, i64 0, metadata !102), !dbg !99 ; [debug line = 160:61] [debug variable = _offload_s0_y_yi]
  br label %.preheader, !dbg !99                  ; [debug line = 160:61]

; <label>:1                                       ; preds = %3, %2
  %p_offload_s0_x_xi = phi i7 [ 0, %2 ], [ %p_offload_s0_x_xi_1, %3 ] ; [#uses=3 type=i7]
  %p_offload_s0_x_xi_cast8 = zext i7 %p_offload_s0_x_xi to i14, !dbg !104 ; [#uses=1 type=i14] [debug line = 163:32]
  %exitcond = icmp eq i7 %p_offload_s0_x_xi, -28, !dbg !104 ; [#uses=1 type=i1] [debug line = 163:32]
  %empty_7 = call i32 (...)* @_ssdm_op_SpecLoopTripCount(i64 100, i64 100, i64 100) nounwind ; [#uses=0 type=i32]
  %p_offload_s0_x_xi_1 = add i7 %p_offload_s0_x_xi, 1, !dbg !106 ; [#uses=1 type=i7] [debug line = 163:62]
  br i1 %exitcond, label %0, label %3, !dbg !104  ; [debug line = 163:32]

; <label>:2                                       ; preds = %.preheader
  call void (...)* @_ssdm_op_SpecLoopName([17 x i8]* @p_str1810) nounwind, !dbg !107 ; [debug line = 161:3]
  %tmp_2 = call i32 (...)* @_ssdm_op_SpecRegionBegin([17 x i8]* @p_str1810) nounwind, !dbg !107 ; [#uses=1 type=i32] [debug line = 161:3]
  br label %1, !dbg !104                          ; [debug line = 163:32]

; <label>:3                                       ; preds = %1
  call void (...)* @_ssdm_op_SpecLoopName([17 x i8]* @p_str1811) nounwind, !dbg !108 ; [debug line = 164:4]
  %tmp_8 = call i32 (...)* @_ssdm_op_SpecRegionBegin([17 x i8]* @p_str1811) nounwind, !dbg !108 ; [#uses=1 type=i32] [debug line = 164:4]
  call void (...)* @_ssdm_op_SpecPipeline(i32 1, i32 1, i32 1, i32 0, [1 x i8]* @p_str1805) nounwind, !dbg !110 ; [debug line = 165:1]
  %p_271 = call i8 @_ssdm_op_Read.ap_fifo.volatile.i8P(i8* %p_p0_to_offload_s0_stream_V), !dbg !111 ; [#uses=1 type=i8] [debug line = 131:9@170:17]
  call void @llvm.dbg.value(metadata !{i8 %p_271}, i64 0, metadata !172), !dbg !170 ; [debug line = 170:17] [debug variable = _271]
  %p_273 = add i14 %phi_mul, %p_offload_s0_x_xi_cast8, !dbg !173 ; [#uses=1 type=i14] [debug line = 173:43]
  %tmp_6 = zext i14 %p_273 to i64, !dbg !174      ; [#uses=1 type=i64] [debug line = 175:5]
  call void @llvm.dbg.value(metadata !{i14 %p_273}, i64 0, metadata !175), !dbg !173 ; [debug line = 173:43] [debug variable = _273]
  call void @llvm.dbg.value(metadata !{i8 %p_271}, i64 0, metadata !177), !dbg !178 ; [debug line = 174:33] [debug variable = _274]
  %p_offload_addr = getelementptr [10000 x i8]* %p_offload, i64 0, i64 %tmp_6, !dbg !174 ; [#uses=1 type=i8*] [debug line = 175:5]
  store i8 %p_271, i8* %p_offload_addr, align 1, !dbg !174 ; [debug line = 175:5]
  %empty_8 = call i32 (...)* @_ssdm_op_SpecRegionEnd([17 x i8]* @p_str1811, i32 %tmp_8) nounwind, !dbg !179 ; [#uses=0 type=i32] [debug line = 177:3]
  call void @llvm.dbg.value(metadata !{i7 %p_offload_s0_x_xi_1}, i64 0, metadata !180), !dbg !106 ; [debug line = 163:62] [debug variable = _offload_s0_x_xi]
  br label %1, !dbg !106                          ; [debug line = 163:62]
}

; [#uses=1]
define internal fastcc void @offload_Loop__distributor_p0_1_proc([10000 x i8]* nocapture %p_p0, i8* %p_p0_to_offload_s0_stream_V) {
newFuncRoot:
  %empty = call i32 (...)* @_ssdm_op_SpecInterface(i8* %p_p0_to_offload_s0_stream_V, [8 x i8]* @ap_fifo_str, i32 0, i32 0, i32 0, i32 0, [1 x i8]* @p_str, [1 x i8]* @p_str, [1 x i8]* @p_str, i32 2, i32 2, i32 16, i32 16, [1 x i8]* @p_str) ; [#uses=0 type=i32]
  call void @llvm.dbg.declare(metadata !{i8* %p_p0_to_offload_s0_stream_V}, metadata !181), !dbg !185 ; [debug line = 136:23] [debug variable = _p0_to_offload_s0_stream.V]
  br label %0

.preheader.exitStub:                              ; preds = %0
  ret void

; <label>:0                                       ; preds = %1, %newFuncRoot
  %p_distributor_p0_1_0_i_i = phi i7 [ 0, %newFuncRoot ], [ %p_distributor_p0_1, %1 ] ; [#uses=2 type=i7]
  %phi_mul = phi i14 [ 0, %newFuncRoot ], [ %next_mul, %1 ] ; [#uses=2 type=i14]
  %next_mul = add i14 %phi_mul, 100               ; [#uses=1 type=i14]
  %exitcond6_i_i = icmp eq i7 %p_distributor_p0_1_0_i_i, -28, !dbg !186 ; [#uses=1 type=i1] [debug line = 139:32]
  %empty_9 = call i32 (...)* @_ssdm_op_SpecLoopTripCount(i64 100, i64 100, i64 100) nounwind ; [#uses=0 type=i32]
  %p_distributor_p0_1 = add i7 %p_distributor_p0_1_0_i_i, 1, !dbg !188 ; [#uses=1 type=i7] [debug line = 139:63]
  br i1 %exitcond6_i_i, label %.preheader.exitStub, label %3, !dbg !186 ; [debug line = 139:32]

; <label>:1                                       ; preds = %2
  %empty_10 = call i32 (...)* @_ssdm_op_SpecRegionEnd([18 x i8]* @p_str1806, i32 %tmp) nounwind, !dbg !189 ; [#uses=0 type=i32] [debug line = 158:2]
  call void @llvm.dbg.value(metadata !{i7 %p_distributor_p0_1}, i64 0, metadata !191) nounwind, !dbg !188 ; [debug line = 139:63] [debug variable = _distributor_p0_1]
  br label %0, !dbg !188                          ; [debug line = 139:63]

; <label>:2                                       ; preds = %4, %3
  %p_distributor_p0_0_0_i_i = phi i7 [ 0, %3 ], [ %p_distributor_p0_0, %4 ] ; [#uses=3 type=i7]
  %p_distributor_p0_0_0_i_i_cast = zext i7 %p_distributor_p0_0_0_i_i to i14, !dbg !192 ; [#uses=1 type=i14] [debug line = 142:33]
  %exitcond5_i_i = icmp eq i7 %p_distributor_p0_0_0_i_i, -28, !dbg !192 ; [#uses=1 type=i1] [debug line = 142:33]
  %empty_11 = call i32 (...)* @_ssdm_op_SpecLoopTripCount(i64 100, i64 100, i64 100) nounwind ; [#uses=0 type=i32]
  %p_distributor_p0_0 = add i7 %p_distributor_p0_0_0_i_i, 1, !dbg !194 ; [#uses=1 type=i7] [debug line = 142:64]
  br i1 %exitcond5_i_i, label %1, label %4, !dbg !192 ; [debug line = 142:33]

; <label>:3                                       ; preds = %0
  call void (...)* @_ssdm_op_SpecLoopName([18 x i8]* @p_str1806) nounwind, !dbg !195 ; [debug line = 140:3]
  %tmp = call i32 (...)* @_ssdm_op_SpecRegionBegin([18 x i8]* @p_str1806) nounwind, !dbg !195 ; [#uses=1 type=i32] [debug line = 140:3]
  br label %2, !dbg !192                          ; [debug line = 142:33]

; <label>:4                                       ; preds = %2
  call void (...)* @_ssdm_op_SpecLoopName([18 x i8]* @p_str1807) nounwind, !dbg !196 ; [debug line = 143:4]
  %tmp_4 = call i32 (...)* @_ssdm_op_SpecRegionBegin([18 x i8]* @p_str1807) nounwind, !dbg !196 ; [#uses=1 type=i32] [debug line = 143:4]
  call void (...)* @_ssdm_op_SpecPipeline(i32 1, i32 1, i32 1, i32 0, [1 x i8]* @p_str1805) nounwind, !dbg !198 ; [debug line = 144:1]
  %p_268 = add i14 %phi_mul, %p_distributor_p0_0_0_i_i_cast, !dbg !199 ; [#uses=1 type=i14] [debug line = 150:44]
  %tmp_i = zext i14 %p_268 to i64, !dbg !201      ; [#uses=1 type=i64] [debug line = 151:29]
  call void @llvm.dbg.value(metadata !{i14 %p_268}, i64 0, metadata !202) nounwind, !dbg !199 ; [debug line = 150:44] [debug variable = _268]
  %p_p0_addr = getelementptr [10000 x i8]* %p_p0, i64 0, i64 %tmp_i, !dbg !201 ; [#uses=1 type=i8*] [debug line = 151:29]
  %p_269 = load i8* %p_p0_addr, align 1, !dbg !201 ; [#uses=1 type=i8] [debug line = 151:29]
  call void @llvm.dbg.value(metadata !{i8 %p_269}, i64 0, metadata !203) nounwind, !dbg !201 ; [debug line = 151:29] [debug variable = _269]
  call void @llvm.dbg.value(metadata !{i8 %p_269}, i64 0, metadata !204) nounwind, !dbg !205 ; [debug line = 153:33] [debug variable = _270]
  call void @llvm.dbg.value(metadata !{i8 %p_269}, i64 0, metadata !206) nounwind, !dbg !209 ; [debug line = 145:31@154:5] [debug variable = tmp]
  call void @_ssdm_op_Write.ap_fifo.volatile.i8P(i8* %p_p0_to_offload_s0_stream_V, i8 %p_269), !dbg !211 ; [debug line = 146:9@154:5]
  %empty_12 = call i32 (...)* @_ssdm_op_SpecRegionEnd([18 x i8]* @p_str1807, i32 %tmp_4) nounwind, !dbg !212 ; [#uses=0 type=i32] [debug line = 157:3]
  call void @llvm.dbg.value(metadata !{i7 %p_distributor_p0_0}, i64 0, metadata !213) nounwind, !dbg !194 ; [debug line = 142:64] [debug variable = _distributor_p0_0]
  br label %2, !dbg !194                          ; [debug line = 142:64]
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
!68 = metadata !{i32 135, i32 1, metadata !69, null}
!69 = metadata !{i32 786443, metadata !70, i32 134, i32 31, metadata !71, i32 0} ; [ DW_TAG_lexical_block ]
!70 = metadata !{i32 786478, i32 0, metadata !71, metadata !"offload", metadata !"offload", metadata !"_Z7offloadPhS_", metadata !71, i32 132, metadata !72, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 false, null, null, null, metadata !77, i32 134} ; [ DW_TAG_subprogram ]
!71 = metadata !{i32 786473, metadata !"offload.cpp", metadata !"/work/zhang/common/tools/halide-working/Halide-SDSoC/apps/sds_simple", null} ; [ DW_TAG_file_type ]
!72 = metadata !{i32 786453, i32 0, metadata !"", i32 0, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !73, i32 0, i32 0} ; [ DW_TAG_subroutine_type ]
!73 = metadata !{null, metadata !74, metadata !74}
!74 = metadata !{i32 786447, null, metadata !"", null, i32 0, i64 64, i64 64, i64 0, i32 0, metadata !75} ; [ DW_TAG_pointer_type ]
!75 = metadata !{i32 786454, null, metadata !"uint8_t", metadata !71, i32 49, i64 0, i64 0, i64 0, i32 0, metadata !76} ; [ DW_TAG_typedef ]
!76 = metadata !{i32 786468, null, metadata !"unsigned char", null, i32 0, i64 8, i64 8, i64 0, i32 0, i32 8} ; [ DW_TAG_base_type ]
!77 = metadata !{metadata !78}
!78 = metadata !{i32 786468}                      ; [ DW_TAG_base_type ]
!79 = metadata !{metadata !80}
!80 = metadata !{i32 0, i32 7, metadata !81}
!81 = metadata !{metadata !82}
!82 = metadata !{metadata !"_p0", metadata !83, metadata !"unsigned char", i32 0, i32 7}
!83 = metadata !{metadata !84}
!84 = metadata !{i32 0, i32 9999, i32 1}
!85 = metadata !{metadata !86}
!86 = metadata !{i32 0, i32 7, metadata !87}
!87 = metadata !{metadata !88}
!88 = metadata !{metadata !"_offload", metadata !83, metadata !"unsigned char", i32 0, i32 7}
!89 = metadata !{i32 786689, metadata !70, metadata !"_p0", null, i32 133, metadata !90, i32 0, i32 0} ; [ DW_TAG_arg_variable ]
!90 = metadata !{i32 786433, null, metadata !"", null, i32 0, i64 0, i64 0, i32 0, i32 0, metadata !75, metadata !91, i32 0, i32 0} ; [ DW_TAG_array_type ]
!91 = metadata !{metadata !92}
!92 = metadata !{i32 786465, i64 0, i64 9999}     ; [ DW_TAG_subrange_type ]
!93 = metadata !{i32 133, i32 10, metadata !70, null}
!94 = metadata !{i32 786689, metadata !70, metadata !"_offload", null, i32 134, metadata !90, i32 0, i32 0} ; [ DW_TAG_arg_variable ]
!95 = metadata !{i32 134, i32 10, metadata !70, null}
!96 = metadata !{i32 179, i32 1, metadata !69, null}
!97 = metadata !{i32 160, i32 31, metadata !98, null}
!98 = metadata !{i32 786443, metadata !69, i32 160, i32 2, metadata !71, i32 6} ; [ DW_TAG_lexical_block ]
!99 = metadata !{i32 160, i32 61, metadata !98, null}
!100 = metadata !{i32 178, i32 2, metadata !101, null}
!101 = metadata !{i32 786443, metadata !98, i32 161, i32 2, metadata !71, i32 7} ; [ DW_TAG_lexical_block ]
!102 = metadata !{i32 786688, metadata !98, metadata !"_offload_s0_y_yi", metadata !71, i32 160, metadata !103, i32 0, i32 0} ; [ DW_TAG_auto_variable ]
!103 = metadata !{i32 786468, null, metadata !"int", null, i32 0, i64 32, i64 32, i64 0, i32 0, i32 5} ; [ DW_TAG_base_type ]
!104 = metadata !{i32 163, i32 32, metadata !105, null}
!105 = metadata !{i32 786443, metadata !101, i32 163, i32 3, metadata !71, i32 8} ; [ DW_TAG_lexical_block ]
!106 = metadata !{i32 163, i32 62, metadata !105, null}
!107 = metadata !{i32 161, i32 3, metadata !101, null}
!108 = metadata !{i32 164, i32 4, metadata !109, null}
!109 = metadata !{i32 786443, metadata !105, i32 164, i32 3, metadata !71, i32 9} ; [ DW_TAG_lexical_block ]
!110 = metadata !{i32 165, i32 1, metadata !109, null}
!111 = metadata !{i32 131, i32 9, metadata !112, metadata !170}
!112 = metadata !{i32 786443, metadata !113, i32 129, i32 63, metadata !115, i32 11} ; [ DW_TAG_lexical_block ]
!113 = metadata !{i32 786478, i32 0, metadata !114, metadata !"read", metadata !"read", metadata !"_ZN3hls6streamIhE4readEv", metadata !115, i32 129, metadata !116, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 false, null, null, metadata !156, metadata !77, i32 129} ; [ DW_TAG_subprogram ]
!114 = metadata !{i32 786489, null, metadata !"hls", metadata !115, i32 69} ; [ DW_TAG_namespace ]
!115 = metadata !{i32 786473, metadata !"/work/zhang/common/tools/xilinx/SDSoC/2016.1/Vivado_HLS/2016.1/common/technology/autopilot/hls_stream.h", metadata !"/work/zhang/common/tools/halide-working/Halide-SDSoC/apps/sds_simple", null} ; [ DW_TAG_file_type ]
!116 = metadata !{i32 786453, i32 0, metadata !"", i32 0, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !117, i32 0, i32 0} ; [ DW_TAG_subroutine_type ]
!117 = metadata !{metadata !76, metadata !118}
!118 = metadata !{i32 786447, i32 0, metadata !"", i32 0, i32 0, i64 64, i64 64, i64 0, i32 64, metadata !119} ; [ DW_TAG_pointer_type ]
!119 = metadata !{i32 786434, metadata !114, metadata !"stream<unsigned char>", metadata !115, i32 79, i64 8, i64 8, i32 0, i32 0, null, metadata !120, i32 0, null, metadata !168} ; [ DW_TAG_class_type ]
!120 = metadata !{metadata !121, metadata !122, metadata !125, metadata !131, metadata !136, metadata !140, metadata !144, metadata !149, metadata !154, metadata !155, metadata !156, metadata !157, metadata !160, metadata !161, metadata !164}
!121 = metadata !{i32 786445, metadata !119, metadata !"V", metadata !115, i32 163, i64 8, i64 8, i64 0, i32 0, metadata !76} ; [ DW_TAG_member ]
!122 = metadata !{i32 786478, i32 0, metadata !119, metadata !"stream", metadata !"stream", metadata !"", metadata !115, i32 83, metadata !123, i1 false, i1 false, i32 0, i32 0, null, i32 256, i1 false, null, null, i32 0, metadata !77, i32 83} ; [ DW_TAG_subprogram ]
!123 = metadata !{i32 786453, i32 0, metadata !"", i32 0, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !124, i32 0, i32 0} ; [ DW_TAG_subroutine_type ]
!124 = metadata !{null, metadata !118}
!125 = metadata !{i32 786478, i32 0, metadata !119, metadata !"stream", metadata !"stream", metadata !"", metadata !115, i32 86, metadata !126, i1 false, i1 false, i32 0, i32 0, null, i32 256, i1 false, null, null, i32 0, metadata !77, i32 86} ; [ DW_TAG_subprogram ]
!126 = metadata !{i32 786453, i32 0, metadata !"", i32 0, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !127, i32 0, i32 0} ; [ DW_TAG_subroutine_type ]
!127 = metadata !{null, metadata !118, metadata !128}
!128 = metadata !{i32 786447, null, metadata !"", null, i32 0, i64 64, i64 64, i64 0, i32 0, metadata !129} ; [ DW_TAG_pointer_type ]
!129 = metadata !{i32 786470, null, metadata !"", null, i32 0, i64 0, i64 0, i64 0, i32 0, metadata !130} ; [ DW_TAG_const_type ]
!130 = metadata !{i32 786468, null, metadata !"char", null, i32 0, i64 8, i64 8, i64 0, i32 0, i32 6} ; [ DW_TAG_base_type ]
!131 = metadata !{i32 786478, i32 0, metadata !119, metadata !"stream", metadata !"stream", metadata !"", metadata !115, i32 91, metadata !132, i1 false, i1 false, i32 0, i32 0, null, i32 257, i1 false, null, null, i32 0, metadata !77, i32 91} ; [ DW_TAG_subprogram ]
!132 = metadata !{i32 786453, i32 0, metadata !"", i32 0, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !133, i32 0, i32 0} ; [ DW_TAG_subroutine_type ]
!133 = metadata !{null, metadata !118, metadata !134}
!134 = metadata !{i32 786448, null, null, null, i32 0, i64 0, i64 0, i64 0, i32 0, metadata !135} ; [ DW_TAG_reference_type ]
!135 = metadata !{i32 786470, null, metadata !"", null, i32 0, i64 0, i64 0, i64 0, i32 0, metadata !119} ; [ DW_TAG_const_type ]
!136 = metadata !{i32 786478, i32 0, metadata !119, metadata !"operator=", metadata !"operator=", metadata !"_ZN3hls6streamIhEaSERKS1_", metadata !115, i32 94, metadata !137, i1 false, i1 false, i32 0, i32 0, null, i32 257, i1 false, null, null, i32 0, metadata !77, i32 94} ; [ DW_TAG_subprogram ]
!137 = metadata !{i32 786453, i32 0, metadata !"", i32 0, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !138, i32 0, i32 0} ; [ DW_TAG_subroutine_type ]
!138 = metadata !{metadata !139, metadata !118, metadata !134}
!139 = metadata !{i32 786448, null, null, null, i32 0, i64 0, i64 0, i64 0, i32 0, metadata !119} ; [ DW_TAG_reference_type ]
!140 = metadata !{i32 786478, i32 0, metadata !119, metadata !"operator>>", metadata !"operator>>", metadata !"_ZN3hls6streamIhErsERh", metadata !115, i32 101, metadata !141, i1 false, i1 false, i32 0, i32 0, null, i32 256, i1 false, null, null, i32 0, metadata !77, i32 101} ; [ DW_TAG_subprogram ]
!141 = metadata !{i32 786453, i32 0, metadata !"", i32 0, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !142, i32 0, i32 0} ; [ DW_TAG_subroutine_type ]
!142 = metadata !{null, metadata !118, metadata !143}
!143 = metadata !{i32 786448, null, null, null, i32 0, i64 0, i64 0, i64 0, i32 0, metadata !76} ; [ DW_TAG_reference_type ]
!144 = metadata !{i32 786478, i32 0, metadata !119, metadata !"operator<<", metadata !"operator<<", metadata !"_ZN3hls6streamIhElsERKh", metadata !115, i32 105, metadata !145, i1 false, i1 false, i32 0, i32 0, null, i32 256, i1 false, null, null, i32 0, metadata !77, i32 105} ; [ DW_TAG_subprogram ]
!145 = metadata !{i32 786453, i32 0, metadata !"", i32 0, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !146, i32 0, i32 0} ; [ DW_TAG_subroutine_type ]
!146 = metadata !{null, metadata !118, metadata !147}
!147 = metadata !{i32 786448, null, null, null, i32 0, i64 0, i64 0, i64 0, i32 0, metadata !148} ; [ DW_TAG_reference_type ]
!148 = metadata !{i32 786470, null, metadata !"", null, i32 0, i64 0, i64 0, i64 0, i32 0, metadata !76} ; [ DW_TAG_const_type ]
!149 = metadata !{i32 786478, i32 0, metadata !119, metadata !"empty", metadata !"empty", metadata !"_ZNK3hls6streamIhE5emptyEv", metadata !115, i32 112, metadata !150, i1 false, i1 false, i32 0, i32 0, null, i32 256, i1 false, null, null, i32 0, metadata !77, i32 112} ; [ DW_TAG_subprogram ]
!150 = metadata !{i32 786453, i32 0, metadata !"", i32 0, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !151, i32 0, i32 0} ; [ DW_TAG_subroutine_type ]
!151 = metadata !{metadata !152, metadata !153}
!152 = metadata !{i32 786468, null, metadata !"bool", null, i32 0, i64 8, i64 8, i64 0, i32 0, i32 2} ; [ DW_TAG_base_type ]
!153 = metadata !{i32 786447, i32 0, metadata !"", i32 0, i32 0, i64 64, i64 64, i64 0, i32 64, metadata !135} ; [ DW_TAG_pointer_type ]
!154 = metadata !{i32 786478, i32 0, metadata !119, metadata !"full", metadata !"full", metadata !"_ZNK3hls6streamIhE4fullEv", metadata !115, i32 117, metadata !150, i1 false, i1 false, i32 0, i32 0, null, i32 256, i1 false, null, null, i32 0, metadata !77, i32 117} ; [ DW_TAG_subprogram ]
!155 = metadata !{i32 786478, i32 0, metadata !119, metadata !"read", metadata !"read", metadata !"_ZN3hls6streamIhE4readERh", metadata !115, i32 123, metadata !141, i1 false, i1 false, i32 0, i32 0, null, i32 256, i1 false, null, null, i32 0, metadata !77, i32 123} ; [ DW_TAG_subprogram ]
!156 = metadata !{i32 786478, i32 0, metadata !119, metadata !"read", metadata !"read", metadata !"_ZN3hls6streamIhE4readEv", metadata !115, i32 129, metadata !116, i1 false, i1 false, i32 0, i32 0, null, i32 256, i1 false, null, null, i32 0, metadata !77, i32 129} ; [ DW_TAG_subprogram ]
!157 = metadata !{i32 786478, i32 0, metadata !119, metadata !"read_nb", metadata !"read_nb", metadata !"_ZN3hls6streamIhE7read_nbERh", metadata !115, i32 136, metadata !158, i1 false, i1 false, i32 0, i32 0, null, i32 256, i1 false, null, null, i32 0, metadata !77, i32 136} ; [ DW_TAG_subprogram ]
!158 = metadata !{i32 786453, i32 0, metadata !"", i32 0, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !159, i32 0, i32 0} ; [ DW_TAG_subroutine_type ]
!159 = metadata !{metadata !152, metadata !118, metadata !143}
!160 = metadata !{i32 786478, i32 0, metadata !119, metadata !"write", metadata !"write", metadata !"_ZN3hls6streamIhE5writeERKh", metadata !115, i32 144, metadata !145, i1 false, i1 false, i32 0, i32 0, null, i32 256, i1 false, null, null, i32 0, metadata !77, i32 144} ; [ DW_TAG_subprogram ]
!161 = metadata !{i32 786478, i32 0, metadata !119, metadata !"write_nb", metadata !"write_nb", metadata !"_ZN3hls6streamIhE8write_nbERKh", metadata !115, i32 150, metadata !162, i1 false, i1 false, i32 0, i32 0, null, i32 256, i1 false, null, null, i32 0, metadata !77, i32 150} ; [ DW_TAG_subprogram ]
!162 = metadata !{i32 786453, i32 0, metadata !"", i32 0, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !163, i32 0, i32 0} ; [ DW_TAG_subroutine_type ]
!163 = metadata !{metadata !152, metadata !118, metadata !147}
!164 = metadata !{i32 786478, i32 0, metadata !119, metadata !"size", metadata !"size", metadata !"_ZN3hls6streamIhE4sizeEv", metadata !115, i32 157, metadata !165, i1 false, i1 false, i32 0, i32 0, null, i32 256, i1 false, null, null, i32 0, metadata !77, i32 157} ; [ DW_TAG_subprogram ]
!165 = metadata !{i32 786453, i32 0, metadata !"", i32 0, i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !166, i32 0, i32 0} ; [ DW_TAG_subroutine_type ]
!166 = metadata !{metadata !167, metadata !118}
!167 = metadata !{i32 786468, null, metadata !"unsigned int", null, i32 0, i64 32, i64 32, i64 0, i32 0, i32 7} ; [ DW_TAG_base_type ]
!168 = metadata !{metadata !169}
!169 = metadata !{i32 786479, null, metadata !"__STREAM_T__", metadata !76, null, i32 0, i32 0} ; [ DW_TAG_template_type_parameter ]
!170 = metadata !{i32 170, i32 17, metadata !171, null}
!171 = metadata !{i32 786443, metadata !109, i32 167, i32 2, metadata !71, i32 10} ; [ DW_TAG_lexical_block ]
!172 = metadata !{i32 786688, metadata !171, metadata !"_271", metadata !71, i32 170, metadata !75, i32 0, i32 0} ; [ DW_TAG_auto_variable ]
!173 = metadata !{i32 173, i32 43, metadata !171, null}
!174 = metadata !{i32 175, i32 5, metadata !171, null}
!175 = metadata !{i32 786688, metadata !171, metadata !"_273", metadata !71, i32 173, metadata !176, i32 0, i32 0} ; [ DW_TAG_auto_variable ]
!176 = metadata !{i32 786454, null, metadata !"int32_t", metadata !71, i32 39, i64 0, i64 0, i64 0, i32 0, metadata !103} ; [ DW_TAG_typedef ]
!177 = metadata !{i32 786688, metadata !171, metadata !"_274", metadata !71, i32 174, metadata !75, i32 0, i32 0} ; [ DW_TAG_auto_variable ]
!178 = metadata !{i32 174, i32 33, metadata !171, null}
!179 = metadata !{i32 177, i32 3, metadata !109, null}
!180 = metadata !{i32 786688, metadata !105, metadata !"_offload_s0_x_xi", metadata !71, i32 163, metadata !103, i32 0, i32 0} ; [ DW_TAG_auto_variable ]
!181 = metadata !{i32 790529, metadata !182, metadata !"_p0_to_offload_s0_stream.V", null, i32 136, metadata !183, i32 0, i32 0} ; [ DW_TAG_auto_variable_field ]
!182 = metadata !{i32 786688, metadata !69, metadata !"_p0_to_offload_s0_stream", metadata !71, i32 136, metadata !119, i32 0, i32 0} ; [ DW_TAG_auto_variable ]
!183 = metadata !{i32 786438, metadata !114, metadata !"stream<unsigned char>", metadata !115, i32 79, i64 8, i64 8, i32 0, i32 0, null, metadata !184, i32 0, null, metadata !168} ; [ DW_TAG_class_field_type ]
!184 = metadata !{metadata !121}
!185 = metadata !{i32 136, i32 23, metadata !69, null}
!186 = metadata !{i32 139, i32 32, metadata !187, null}
!187 = metadata !{i32 786443, metadata !69, i32 139, i32 2, metadata !71, i32 1} ; [ DW_TAG_lexical_block ]
!188 = metadata !{i32 139, i32 63, metadata !187, null}
!189 = metadata !{i32 158, i32 2, metadata !190, null}
!190 = metadata !{i32 786443, metadata !187, i32 140, i32 2, metadata !71, i32 2} ; [ DW_TAG_lexical_block ]
!191 = metadata !{i32 786688, metadata !187, metadata !"_distributor_p0_1", metadata !71, i32 139, metadata !103, i32 0, i32 0} ; [ DW_TAG_auto_variable ]
!192 = metadata !{i32 142, i32 33, metadata !193, null}
!193 = metadata !{i32 786443, metadata !190, i32 142, i32 3, metadata !71, i32 3} ; [ DW_TAG_lexical_block ]
!194 = metadata !{i32 142, i32 64, metadata !193, null}
!195 = metadata !{i32 140, i32 3, metadata !190, null}
!196 = metadata !{i32 143, i32 4, metadata !197, null}
!197 = metadata !{i32 786443, metadata !193, i32 143, i32 3, metadata !71, i32 4} ; [ DW_TAG_lexical_block ]
!198 = metadata !{i32 144, i32 1, metadata !197, null}
!199 = metadata !{i32 150, i32 44, metadata !200, null}
!200 = metadata !{i32 786443, metadata !197, i32 146, i32 2, metadata !71, i32 5} ; [ DW_TAG_lexical_block ]
!201 = metadata !{i32 151, i32 29, metadata !200, null}
!202 = metadata !{i32 786688, metadata !200, metadata !"_268", metadata !71, i32 150, metadata !176, i32 0, i32 0} ; [ DW_TAG_auto_variable ]
!203 = metadata !{i32 786688, metadata !200, metadata !"_269", metadata !71, i32 151, metadata !75, i32 0, i32 0} ; [ DW_TAG_auto_variable ]
!204 = metadata !{i32 786688, metadata !200, metadata !"_270", metadata !71, i32 153, metadata !75, i32 0, i32 0} ; [ DW_TAG_auto_variable ]
!205 = metadata !{i32 153, i32 33, metadata !200, null}
!206 = metadata !{i32 786688, metadata !207, metadata !"tmp", metadata !115, i32 145, metadata !76, i32 0, i32 0} ; [ DW_TAG_auto_variable ]
!207 = metadata !{i32 786443, metadata !208, i32 144, i32 79, metadata !115, i32 12} ; [ DW_TAG_lexical_block ]
!208 = metadata !{i32 786478, i32 0, metadata !114, metadata !"write", metadata !"write", metadata !"_ZN3hls6streamIhE5writeERKh", metadata !115, i32 144, metadata !145, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 false, null, null, metadata !160, metadata !77, i32 144} ; [ DW_TAG_subprogram ]
!209 = metadata !{i32 145, i32 31, metadata !207, metadata !210}
!210 = metadata !{i32 154, i32 5, metadata !200, null}
!211 = metadata !{i32 146, i32 9, metadata !207, metadata !210}
!212 = metadata !{i32 157, i32 3, metadata !197, null}
!213 = metadata !{i32 786688, metadata !193, metadata !"_distributor_p0_0", metadata !71, i32 142, metadata !103, i32 0, i32 0} ; [ DW_TAG_auto_variable ]
