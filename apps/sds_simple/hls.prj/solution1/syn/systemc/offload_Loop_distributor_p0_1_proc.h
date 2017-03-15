// ==============================================================
// RTL generated by Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC
// Version: 2016.1
// Copyright (C) 1986-2016 Xilinx, Inc. All Rights Reserved.
// 
// ===========================================================

#ifndef _offload_Loop_distributor_p0_1_proc_HH_
#define _offload_Loop_distributor_p0_1_proc_HH_

#include "systemc.h"
#include "AESL_pkg.h"


namespace ap_rtl {

struct offload_Loop_distributor_p0_1_proc : public sc_module {
    // Port declarations 13
    sc_in_clk ap_clk;
    sc_in< sc_logic > ap_rst;
    sc_in< sc_logic > ap_start;
    sc_out< sc_logic > ap_done;
    sc_in< sc_logic > ap_continue;
    sc_out< sc_logic > ap_idle;
    sc_out< sc_logic > ap_ready;
    sc_out< sc_lv<14> > p_p0_address0;
    sc_out< sc_logic > p_p0_ce0;
    sc_in< sc_lv<8> > p_p0_q0;
    sc_out< sc_lv<8> > p_p0_to_offload_s0_stream_V_din;
    sc_in< sc_logic > p_p0_to_offload_s0_stream_V_full_n;
    sc_out< sc_logic > p_p0_to_offload_s0_stream_V_write;


    // Module declarations
    offload_Loop_distributor_p0_1_proc(sc_module_name name);
    SC_HAS_PROCESS(offload_Loop_distributor_p0_1_proc);

    ~offload_Loop_distributor_p0_1_proc();

    sc_trace_file* mVcdFile;

    sc_signal< sc_logic > ap_done_reg;
    sc_signal< sc_lv<4> > ap_CS_fsm;
    sc_signal< sc_logic > ap_sig_cseq_ST_st1_fsm_0;
    sc_signal< bool > ap_sig_22;
    sc_signal< sc_logic > p_p0_to_offload_s0_stream_V_blk_n;
    sc_signal< sc_logic > ap_reg_ppiten_pp0_it2;
    sc_signal< sc_logic > ap_reg_ppiten_pp0_it0;
    sc_signal< sc_logic > ap_reg_ppiten_pp0_it1;
    sc_signal< sc_lv<1> > exitcond5_i_i_reg_162;
    sc_signal< sc_lv<1> > ap_reg_ppstg_exitcond5_i_i_reg_162_pp0_iter1;
    sc_signal< sc_lv<7> > p_distributor_p0_0_0_i_i_reg_93;
    sc_signal< sc_lv<14> > next_mul_fu_104_p2;
    sc_signal< sc_lv<14> > next_mul_reg_148;
    sc_signal< sc_logic > ap_sig_cseq_ST_st2_fsm_1;
    sc_signal< bool > ap_sig_60;
    sc_signal< sc_lv<1> > exitcond6_i_i_fu_110_p2;
    sc_signal< sc_lv<7> > p_distributor_p0_1_fu_116_p2;
    sc_signal< sc_lv<7> > p_distributor_p0_1_reg_157;
    sc_signal< sc_lv<1> > exitcond5_i_i_fu_126_p2;
    sc_signal< sc_logic > ap_sig_cseq_ST_pp0_stg0_fsm_2;
    sc_signal< bool > ap_sig_72;
    sc_signal< bool > ap_sig_76;
    sc_signal< sc_lv<7> > p_distributor_p0_0_fu_132_p2;
    sc_signal< sc_lv<14> > p_268_fu_138_p2;
    sc_signal< sc_lv<14> > p_268_reg_171;
    sc_signal< sc_lv<7> > p_distributor_p0_1_0_i_i_reg_70;
    sc_signal< bool > ap_sig_104;
    sc_signal< sc_logic > ap_sig_cseq_ST_st6_fsm_3;
    sc_signal< bool > ap_sig_112;
    sc_signal< sc_lv<14> > phi_mul_reg_81;
    sc_signal< sc_lv<64> > tmp_i_fu_144_p1;
    sc_signal< sc_lv<14> > p_distributor_p0_0_0_i_i_cast_fu_122_p1;
    sc_signal< sc_lv<4> > ap_NS_fsm;
    static const sc_logic ap_const_logic_1;
    static const sc_logic ap_const_logic_0;
    static const sc_lv<4> ap_ST_st1_fsm_0;
    static const sc_lv<4> ap_ST_st2_fsm_1;
    static const sc_lv<4> ap_ST_pp0_stg0_fsm_2;
    static const sc_lv<4> ap_ST_st6_fsm_3;
    static const sc_lv<32> ap_const_lv32_0;
    static const sc_lv<1> ap_const_lv1_1;
    static const sc_lv<1> ap_const_lv1_0;
    static const sc_lv<32> ap_const_lv32_1;
    static const sc_lv<32> ap_const_lv32_2;
    static const sc_lv<7> ap_const_lv7_0;
    static const sc_lv<32> ap_const_lv32_3;
    static const sc_lv<14> ap_const_lv14_0;
    static const sc_lv<14> ap_const_lv14_64;
    static const sc_lv<7> ap_const_lv7_64;
    static const sc_lv<7> ap_const_lv7_1;
    // Thread declarations
    void thread_ap_clk_no_reset_();
    void thread_ap_done();
    void thread_ap_idle();
    void thread_ap_ready();
    void thread_ap_sig_104();
    void thread_ap_sig_112();
    void thread_ap_sig_22();
    void thread_ap_sig_60();
    void thread_ap_sig_72();
    void thread_ap_sig_76();
    void thread_ap_sig_cseq_ST_pp0_stg0_fsm_2();
    void thread_ap_sig_cseq_ST_st1_fsm_0();
    void thread_ap_sig_cseq_ST_st2_fsm_1();
    void thread_ap_sig_cseq_ST_st6_fsm_3();
    void thread_exitcond5_i_i_fu_126_p2();
    void thread_exitcond6_i_i_fu_110_p2();
    void thread_next_mul_fu_104_p2();
    void thread_p_268_fu_138_p2();
    void thread_p_distributor_p0_0_0_i_i_cast_fu_122_p1();
    void thread_p_distributor_p0_0_fu_132_p2();
    void thread_p_distributor_p0_1_fu_116_p2();
    void thread_p_p0_address0();
    void thread_p_p0_ce0();
    void thread_p_p0_to_offload_s0_stream_V_blk_n();
    void thread_p_p0_to_offload_s0_stream_V_din();
    void thread_p_p0_to_offload_s0_stream_V_write();
    void thread_tmp_i_fu_144_p1();
    void thread_ap_NS_fsm();
};

}

using namespace ap_rtl;

#endif
