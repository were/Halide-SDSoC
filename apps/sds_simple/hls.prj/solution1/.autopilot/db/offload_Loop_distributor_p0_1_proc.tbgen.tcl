set moduleName offload_Loop_distributor_p0_1_proc
set isCombinational 0
set isDatapathOnly 0
set isPipelined 0
set pipeline_type none
set FunctionProtocol ap_ctrl_hs
set isOneStateSeq 0
set C_modelName {offload_Loop__distributor_p0_1_proc}
set C_modelType { void 0 }
set C_modelArgList {
	{ p_p0 int 8 regular {array 10000 { 1 3 } 1 1 }  }
	{ p_p0_to_offload_s0_stream_V int 8 regular {fifo 1 volatile }  }
}
set C_modelArgMapList {[ 
	{ "Name" : "p_p0", "interface" : "memory", "bitwidth" : 8, "direction" : "READONLY"} , 
 	{ "Name" : "p_p0_to_offload_s0_stream_V", "interface" : "fifo", "bitwidth" : 8, "direction" : "WRITEONLY"} ]}
# RTL Port declarations: 
set portNum 13
set portList { 
	{ ap_clk sc_in sc_logic 1 clock -1 } 
	{ ap_rst sc_in sc_logic 1 reset -1 active_high_sync } 
	{ ap_start sc_in sc_logic 1 start -1 } 
	{ ap_done sc_out sc_logic 1 predone -1 } 
	{ ap_continue sc_in sc_logic 1 continue -1 } 
	{ ap_idle sc_out sc_logic 1 done -1 } 
	{ ap_ready sc_out sc_logic 1 ready -1 } 
	{ p_p0_address0 sc_out sc_lv 14 signal 0 } 
	{ p_p0_ce0 sc_out sc_logic 1 signal 0 } 
	{ p_p0_q0 sc_in sc_lv 8 signal 0 } 
	{ p_p0_to_offload_s0_stream_V_din sc_out sc_lv 8 signal 1 } 
	{ p_p0_to_offload_s0_stream_V_full_n sc_in sc_logic 1 signal 1 } 
	{ p_p0_to_offload_s0_stream_V_write sc_out sc_logic 1 signal 1 } 
}
set NewPortList {[ 
	{ "name": "ap_clk", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "clock", "bundle":{"name": "ap_clk", "role": "default" }} , 
 	{ "name": "ap_rst", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "reset", "bundle":{"name": "ap_rst", "role": "default" }} , 
 	{ "name": "ap_start", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "start", "bundle":{"name": "ap_start", "role": "default" }} , 
 	{ "name": "ap_done", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "predone", "bundle":{"name": "ap_done", "role": "default" }} , 
 	{ "name": "ap_continue", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "continue", "bundle":{"name": "ap_continue", "role": "default" }} , 
 	{ "name": "ap_idle", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "done", "bundle":{"name": "ap_idle", "role": "default" }} , 
 	{ "name": "ap_ready", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "ready", "bundle":{"name": "ap_ready", "role": "default" }} , 
 	{ "name": "p_p0_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":14, "type": "signal", "bundle":{"name": "p_p0", "role": "address0" }} , 
 	{ "name": "p_p0_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "p_p0", "role": "ce0" }} , 
 	{ "name": "p_p0_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "p_p0", "role": "q0" }} , 
 	{ "name": "p_p0_to_offload_s0_stream_V_din", "direction": "out", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "p_p0_to_offload_s0_stream_V", "role": "din" }} , 
 	{ "name": "p_p0_to_offload_s0_stream_V_full_n", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "p_p0_to_offload_s0_stream_V", "role": "full_n" }} , 
 	{ "name": "p_p0_to_offload_s0_stream_V_write", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "p_p0_to_offload_s0_stream_V", "role": "write" }}  ]}

set RtlHierarchyInfo {[
	{"Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "", "Child" : [], "CDFG" : "offload_Loop_distributor_p0_1_proc", "VariableLatency" : "1", "AlignedPipeline" : "0", "UnalignedPipeline" : "0", "ProcessNetwork" : "0", "Combinational" : "0", "ControlExist" : "1",
		"Port" : [
		{"Name" : "p_p0", "Type" : "Memory", "Direction" : "I", "BlockSignal" : [], "SubConnect" : []}, 
		{"Name" : "p_p0_to_offload_s0_stream_V", "Type" : "Fifo", "Direction" : "O", "BlockSignal" : [
			{"Name" : "p_p0_to_offload_s0_stream_V_blk_n", "Type" : "RtlSignal"}], "SubConnect" : []}],
		"WaitState" : [],
		"SubBlockPort" : []}]}

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "10401", "Max" : "10401"}
	, {"Name" : "Interval", "Min" : "10401", "Max" : "10401"}
]}

set Spec2ImplPortList { 
	p_p0 { ap_memory {  { p_p0_address0 mem_address 1 14 }  { p_p0_ce0 mem_ce 1 1 }  { p_p0_q0 mem_dout 0 8 } } }
	p_p0_to_offload_s0_stream_V { ap_fifo {  { p_p0_to_offload_s0_stream_V_din fifo_data 1 8 }  { p_p0_to_offload_s0_stream_V_full_n fifo_status 0 1 }  { p_p0_to_offload_s0_stream_V_write fifo_update 1 1 } } }
}
