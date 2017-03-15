set C_TypeInfoList {{ 
"offload" : [[], { "return": [[], "void"]} , [{"ExternC" : 0}], [ {"_p0": [[], {"array": ["0", [10000]]}] }, {"_offload": [[], {"array": ["0", [10000]]}] }],[],""], 
"0": [ "uint8_t", {"typedef": [[[], {"scalar": "unsigned char"}],""]}]
}}
set moduleName offload
set isCombinational 0
set isDatapathOnly 0
set isPipelined 1
set pipeline_type dataflow
set FunctionProtocol ap_ctrl_hs
set isOneStateSeq 0
set C_modelName {offload}
set C_modelType { void 0 }
set C_modelArgList {
	{ p_p0 int 8 regular {array 10000 { 1 3 } 1 1 }  }
	{ p_offload int 8 regular {array 10000 { 0 3 } 0 1 }  }
}
set C_modelArgMapList {[ 
	{ "Name" : "p_p0", "interface" : "memory", "bitwidth" : 8, "direction" : "READONLY", "bitSlice":[{"low":0,"up":7,"cElement": [{"cName": "_p0","cData": "unsigned char","bit_use": { "low": 0,"up": 7},"cArray": [{"low" : 0,"up" : 9999,"step" : 1}]}]}]} , 
 	{ "Name" : "p_offload", "interface" : "memory", "bitwidth" : 8, "direction" : "WRITEONLY", "bitSlice":[{"low":0,"up":7,"cElement": [{"cName": "_offload","cData": "unsigned char","bit_use": { "low": 0,"up": 7},"cArray": [{"low" : 0,"up" : 9999,"step" : 1}]}]}]} ]}
# RTL Port declarations: 
set portNum 26
set portList { 
	{ p_p0_address0 sc_out sc_lv 14 signal 0 } 
	{ p_p0_ce0 sc_out sc_logic 1 signal 0 } 
	{ p_p0_d0 sc_out sc_lv 8 signal 0 } 
	{ p_p0_q0 sc_in sc_lv 8 signal 0 } 
	{ p_p0_we0 sc_out sc_logic 1 signal 0 } 
	{ p_p0_address1 sc_out sc_lv 14 signal 0 } 
	{ p_p0_ce1 sc_out sc_logic 1 signal 0 } 
	{ p_p0_d1 sc_out sc_lv 8 signal 0 } 
	{ p_p0_q1 sc_in sc_lv 8 signal 0 } 
	{ p_p0_we1 sc_out sc_logic 1 signal 0 } 
	{ p_offload_address0 sc_out sc_lv 14 signal 1 } 
	{ p_offload_ce0 sc_out sc_logic 1 signal 1 } 
	{ p_offload_d0 sc_out sc_lv 8 signal 1 } 
	{ p_offload_q0 sc_in sc_lv 8 signal 1 } 
	{ p_offload_we0 sc_out sc_logic 1 signal 1 } 
	{ p_offload_address1 sc_out sc_lv 14 signal 1 } 
	{ p_offload_ce1 sc_out sc_logic 1 signal 1 } 
	{ p_offload_d1 sc_out sc_lv 8 signal 1 } 
	{ p_offload_q1 sc_in sc_lv 8 signal 1 } 
	{ p_offload_we1 sc_out sc_logic 1 signal 1 } 
	{ ap_clk sc_in sc_logic 1 clock -1 } 
	{ ap_rst sc_in sc_logic 1 reset -1 active_high_sync } 
	{ ap_done sc_out sc_logic 1 predone -1 } 
	{ ap_start sc_in sc_logic 1 start -1 } 
	{ ap_ready sc_out sc_logic 1 ready -1 } 
	{ ap_idle sc_out sc_logic 1 done -1 } 
}
set NewPortList {[ 
	{ "name": "p_p0_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":14, "type": "signal", "bundle":{"name": "p_p0", "role": "address0" }} , 
 	{ "name": "p_p0_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "p_p0", "role": "ce0" }} , 
 	{ "name": "p_p0_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "p_p0", "role": "d0" }} , 
 	{ "name": "p_p0_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "p_p0", "role": "q0" }} , 
 	{ "name": "p_p0_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "p_p0", "role": "we0" }} , 
 	{ "name": "p_p0_address1", "direction": "out", "datatype": "sc_lv", "bitwidth":14, "type": "signal", "bundle":{"name": "p_p0", "role": "address1" }} , 
 	{ "name": "p_p0_ce1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "p_p0", "role": "ce1" }} , 
 	{ "name": "p_p0_d1", "direction": "out", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "p_p0", "role": "d1" }} , 
 	{ "name": "p_p0_q1", "direction": "in", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "p_p0", "role": "q1" }} , 
 	{ "name": "p_p0_we1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "p_p0", "role": "we1" }} , 
 	{ "name": "p_offload_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":14, "type": "signal", "bundle":{"name": "p_offload", "role": "address0" }} , 
 	{ "name": "p_offload_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "p_offload", "role": "ce0" }} , 
 	{ "name": "p_offload_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "p_offload", "role": "d0" }} , 
 	{ "name": "p_offload_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "p_offload", "role": "q0" }} , 
 	{ "name": "p_offload_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "p_offload", "role": "we0" }} , 
 	{ "name": "p_offload_address1", "direction": "out", "datatype": "sc_lv", "bitwidth":14, "type": "signal", "bundle":{"name": "p_offload", "role": "address1" }} , 
 	{ "name": "p_offload_ce1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "p_offload", "role": "ce1" }} , 
 	{ "name": "p_offload_d1", "direction": "out", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "p_offload", "role": "d1" }} , 
 	{ "name": "p_offload_q1", "direction": "in", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "p_offload", "role": "q1" }} , 
 	{ "name": "p_offload_we1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "p_offload", "role": "we1" }} , 
 	{ "name": "ap_clk", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "clock", "bundle":{"name": "ap_clk", "role": "default" }} , 
 	{ "name": "ap_rst", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "reset", "bundle":{"name": "ap_rst", "role": "default" }} , 
 	{ "name": "ap_done", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "predone", "bundle":{"name": "ap_done", "role": "default" }} , 
 	{ "name": "ap_start", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "start", "bundle":{"name": "ap_start", "role": "default" }} , 
 	{ "name": "ap_ready", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "ready", "bundle":{"name": "ap_ready", "role": "default" }} , 
 	{ "name": "ap_idle", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "done", "bundle":{"name": "ap_idle", "role": "default" }}  ]}

set RtlHierarchyInfo {[
	{"Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "", "Child" : ["1", "2", "3"], "CDFG" : "offload", "VariableLatency" : "1", "AlignedPipeline" : "0", "UnalignedPipeline" : "0", "ProcessNetwork" : "1", "Combinational" : "0", "ControlExist" : "1",
		"Port" : [
		{"Name" : "p_p0", "Type" : "Memory", "Direction" : "I", "BlockSignal" : [], "SubConnect" : [
			{"SubInst" : "offload_Loop_distributor_p0_1_proc_U0", "Port" : "p_p0"}]}, 
		{"Name" : "p_offload", "Type" : "Memory", "Direction" : "O", "BlockSignal" : [], "SubConnect" : [
			{"SubInst" : "offload_Loop_offload_s0_y_yi_proc_U0", "Port" : "p_offload"}]}],
		"WaitState" : [],
		"SubBlockPort" : []},
	{"Level" : "1", "Path" : "`AUTOTB_DUT_INST.offload_Loop_distributor_p0_1_proc_U0", "Parent" : "0", "Child" : [], "CDFG" : "offload_Loop_distributor_p0_1_proc", "VariableLatency" : "1", "AlignedPipeline" : "0", "UnalignedPipeline" : "0", "ProcessNetwork" : "0", "Combinational" : "0", "ControlExist" : "1",
		"Port" : [
		{"Name" : "p_p0", "Type" : "Memory", "Direction" : "I", "BlockSignal" : [], "SubConnect" : []}, 
		{"Name" : "p_p0_to_offload_s0_stream_V", "Type" : "Fifo", "Direction" : "O", "BlockSignal" : [
			{"Name" : "p_p0_to_offload_s0_stream_V_blk_n", "Type" : "RtlSignal"}], "SubConnect" : []}],
		"WaitState" : [],
		"SubBlockPort" : []},
	{"Level" : "1", "Path" : "`AUTOTB_DUT_INST.offload_Loop_offload_s0_y_yi_proc_U0", "Parent" : "0", "Child" : [], "CDFG" : "offload_Loop_offload_s0_y_yi_proc", "VariableLatency" : "1", "AlignedPipeline" : "0", "UnalignedPipeline" : "0", "ProcessNetwork" : "0", "Combinational" : "0", "ControlExist" : "1",
		"Port" : [
		{"Name" : "p_p0_to_offload_s0_stream_V", "Type" : "Fifo", "Direction" : "I", "BlockSignal" : [
			{"Name" : "p_p0_to_offload_s0_stream_V_blk_n", "Type" : "RtlSignal"}], "SubConnect" : []}, 
		{"Name" : "p_offload", "Type" : "Memory", "Direction" : "O", "BlockSignal" : [], "SubConnect" : []}],
		"WaitState" : [],
		"SubBlockPort" : []},
	{"Level" : "1", "Path" : "`AUTOTB_DUT_INST.p_p0_to_offload_s0_stream_V_U", "Parent" : "0", "Child" : []}]}

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "10401", "Max" : "10401"}
	, {"Name" : "Interval", "Min" : "10402", "Max" : "10402"}
]}

set Spec2ImplPortList { 
	p_p0 { ap_memory {  { p_p0_address0 mem_address 1 14 }  { p_p0_ce0 mem_ce 1 1 }  { p_p0_d0 mem_din 1 8 }  { p_p0_q0 mem_dout 0 8 }  { p_p0_we0 mem_we 1 1 }  { p_p0_address1 mem_address 1 14 }  { p_p0_ce1 mem_ce 1 1 }  { p_p0_d1 mem_din 1 8 }  { p_p0_q1 mem_dout 0 8 }  { p_p0_we1 mem_we 1 1 } } }
	p_offload { ap_memory {  { p_offload_address0 mem_address 1 14 }  { p_offload_ce0 mem_ce 1 1 }  { p_offload_d0 mem_din 1 8 }  { p_offload_q0 mem_dout 0 8 }  { p_offload_we0 mem_we 1 1 }  { p_offload_address1 mem_address 1 14 }  { p_offload_ce1 mem_ce 1 1 }  { p_offload_d1 mem_din 1 8 }  { p_offload_q1 mem_dout 0 8 }  { p_offload_we1 mem_we 1 1 } } }
}

set busDeadlockParameterList { 
}

# RTL port scheduling information:
set fifoSchedulingInfoList { 
}

# RTL bus port read request latency information:
set busReadReqLatencyList { 
}

# RTL bus port write response latency information:
set busWriteResLatencyList { 
}

# RTL array port load latency information:
set memoryLoadLatencyList { 
}
