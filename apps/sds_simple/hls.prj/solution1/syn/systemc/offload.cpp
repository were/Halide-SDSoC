// ==============================================================
// RTL generated by Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC
// Version: 2016.1
// Copyright (C) 1986-2016 Xilinx, Inc. All Rights Reserved.
// 
// ===========================================================

#include "offload.h"
#include "AESL_pkg.h"

using namespace std;

namespace ap_rtl {

const sc_lv<14> offload::ap_const_lv14_0 = "00000000000000";
const sc_logic offload::ap_const_logic_0 = sc_dt::Log_0;
const sc_lv<8> offload::ap_const_lv8_0 = "00000000";
const sc_logic offload::ap_const_logic_1 = sc_dt::Log_1;
const bool offload::ap_true = true;

offload::offload(sc_module_name name) : sc_module(name), mVcdFile(0) {
    offload_Loop_distributor_p0_1_proc_U0 = new offload_Loop_distributor_p0_1_proc("offload_Loop_distributor_p0_1_proc_U0");
    offload_Loop_distributor_p0_1_proc_U0->ap_clk(ap_clk);
    offload_Loop_distributor_p0_1_proc_U0->ap_rst(ap_rst);
    offload_Loop_distributor_p0_1_proc_U0->ap_start(offload_Loop_distributor_p0_1_proc_U0_ap_start);
    offload_Loop_distributor_p0_1_proc_U0->ap_done(offload_Loop_distributor_p0_1_proc_U0_ap_done);
    offload_Loop_distributor_p0_1_proc_U0->ap_continue(offload_Loop_distributor_p0_1_proc_U0_ap_continue);
    offload_Loop_distributor_p0_1_proc_U0->ap_idle(offload_Loop_distributor_p0_1_proc_U0_ap_idle);
    offload_Loop_distributor_p0_1_proc_U0->ap_ready(offload_Loop_distributor_p0_1_proc_U0_ap_ready);
    offload_Loop_distributor_p0_1_proc_U0->p_p0_address0(offload_Loop_distributor_p0_1_proc_U0_p_p0_address0);
    offload_Loop_distributor_p0_1_proc_U0->p_p0_ce0(offload_Loop_distributor_p0_1_proc_U0_p_p0_ce0);
    offload_Loop_distributor_p0_1_proc_U0->p_p0_q0(p_p0_q0);
    offload_Loop_distributor_p0_1_proc_U0->p_p0_to_offload_s0_stream_V_din(offload_Loop_distributor_p0_1_proc_U0_p_p0_to_offload_s0_stream_V_din);
    offload_Loop_distributor_p0_1_proc_U0->p_p0_to_offload_s0_stream_V_full_n(p_p0_to_offload_s0_stream_V_full_n);
    offload_Loop_distributor_p0_1_proc_U0->p_p0_to_offload_s0_stream_V_write(offload_Loop_distributor_p0_1_proc_U0_p_p0_to_offload_s0_stream_V_write);
    offload_Loop_offload_s0_y_yi_proc_U0 = new offload_Loop_offload_s0_y_yi_proc("offload_Loop_offload_s0_y_yi_proc_U0");
    offload_Loop_offload_s0_y_yi_proc_U0->ap_clk(ap_clk);
    offload_Loop_offload_s0_y_yi_proc_U0->ap_rst(ap_rst);
    offload_Loop_offload_s0_y_yi_proc_U0->ap_start(offload_Loop_offload_s0_y_yi_proc_U0_ap_start);
    offload_Loop_offload_s0_y_yi_proc_U0->ap_done(offload_Loop_offload_s0_y_yi_proc_U0_ap_done);
    offload_Loop_offload_s0_y_yi_proc_U0->ap_continue(offload_Loop_offload_s0_y_yi_proc_U0_ap_continue);
    offload_Loop_offload_s0_y_yi_proc_U0->ap_idle(offload_Loop_offload_s0_y_yi_proc_U0_ap_idle);
    offload_Loop_offload_s0_y_yi_proc_U0->ap_ready(offload_Loop_offload_s0_y_yi_proc_U0_ap_ready);
    offload_Loop_offload_s0_y_yi_proc_U0->p_p0_to_offload_s0_stream_V_dout(p_p0_to_offload_s0_stream_V_dout);
    offload_Loop_offload_s0_y_yi_proc_U0->p_p0_to_offload_s0_stream_V_empty_n(p_p0_to_offload_s0_stream_V_empty_n);
    offload_Loop_offload_s0_y_yi_proc_U0->p_p0_to_offload_s0_stream_V_read(offload_Loop_offload_s0_y_yi_proc_U0_p_p0_to_offload_s0_stream_V_read);
    offload_Loop_offload_s0_y_yi_proc_U0->p_offload_address0(offload_Loop_offload_s0_y_yi_proc_U0_p_offload_address0);
    offload_Loop_offload_s0_y_yi_proc_U0->p_offload_ce0(offload_Loop_offload_s0_y_yi_proc_U0_p_offload_ce0);
    offload_Loop_offload_s0_y_yi_proc_U0->p_offload_we0(offload_Loop_offload_s0_y_yi_proc_U0_p_offload_we0);
    offload_Loop_offload_s0_y_yi_proc_U0->p_offload_d0(offload_Loop_offload_s0_y_yi_proc_U0_p_offload_d0);
    p_p0_to_offload_s0_stream_V_U = new FIFO_offload_p_p0_to_offload_s0_stream_V("p_p0_to_offload_s0_stream_V_U");
    p_p0_to_offload_s0_stream_V_U->clk(ap_clk);
    p_p0_to_offload_s0_stream_V_U->reset(ap_rst);
    p_p0_to_offload_s0_stream_V_U->if_read_ce(ap_var_for_const0);
    p_p0_to_offload_s0_stream_V_U->if_write_ce(ap_var_for_const0);
    p_p0_to_offload_s0_stream_V_U->if_din(offload_Loop_distributor_p0_1_proc_U0_p_p0_to_offload_s0_stream_V_din);
    p_p0_to_offload_s0_stream_V_U->if_full_n(p_p0_to_offload_s0_stream_V_full_n);
    p_p0_to_offload_s0_stream_V_U->if_write(offload_Loop_distributor_p0_1_proc_U0_p_p0_to_offload_s0_stream_V_write);
    p_p0_to_offload_s0_stream_V_U->if_dout(p_p0_to_offload_s0_stream_V_dout);
    p_p0_to_offload_s0_stream_V_U->if_empty_n(p_p0_to_offload_s0_stream_V_empty_n);
    p_p0_to_offload_s0_stream_V_U->if_read(offload_Loop_offload_s0_y_yi_proc_U0_p_p0_to_offload_s0_stream_V_read);

    SC_METHOD(thread_ap_clk_no_reset_);
    dont_initialize();
    sensitive << ( ap_clk.pos() );

    SC_METHOD(thread_ap_done);
    sensitive << ( ap_sig_hs_done );

    SC_METHOD(thread_ap_idle);
    sensitive << ( offload_Loop_distributor_p0_1_proc_U0_ap_idle );
    sensitive << ( offload_Loop_offload_s0_y_yi_proc_U0_ap_idle );

    SC_METHOD(thread_ap_ready);
    sensitive << ( offload_Loop_distributor_p0_1_proc_U0_ap_ready );

    SC_METHOD(thread_ap_sig_hs_continue);

    SC_METHOD(thread_ap_sig_hs_done);
    sensitive << ( offload_Loop_offload_s0_y_yi_proc_U0_ap_done );

    SC_METHOD(thread_ap_sig_hs_ready);
    sensitive << ( offload_Loop_distributor_p0_1_proc_U0_ap_ready );

    SC_METHOD(thread_offload_Loop_distributor_p0_1_proc_U0_ap_continue);

    SC_METHOD(thread_offload_Loop_distributor_p0_1_proc_U0_ap_start);
    sensitive << ( ap_start );

    SC_METHOD(thread_offload_Loop_offload_s0_y_yi_proc_U0_ap_continue);

    SC_METHOD(thread_p_offload_address0);
    sensitive << ( offload_Loop_offload_s0_y_yi_proc_U0_p_offload_address0 );

    SC_METHOD(thread_p_offload_address1);

    SC_METHOD(thread_p_offload_ce0);
    sensitive << ( offload_Loop_offload_s0_y_yi_proc_U0_p_offload_ce0 );

    SC_METHOD(thread_p_offload_ce1);

    SC_METHOD(thread_p_offload_d0);
    sensitive << ( offload_Loop_offload_s0_y_yi_proc_U0_p_offload_d0 );

    SC_METHOD(thread_p_offload_d1);

    SC_METHOD(thread_p_offload_we0);
    sensitive << ( offload_Loop_offload_s0_y_yi_proc_U0_p_offload_we0 );

    SC_METHOD(thread_p_offload_we1);

    SC_METHOD(thread_p_p0_address0);
    sensitive << ( offload_Loop_distributor_p0_1_proc_U0_p_p0_address0 );

    SC_METHOD(thread_p_p0_address1);

    SC_METHOD(thread_p_p0_ce0);
    sensitive << ( offload_Loop_distributor_p0_1_proc_U0_p_p0_ce0 );

    SC_METHOD(thread_p_p0_ce1);

    SC_METHOD(thread_p_p0_d0);

    SC_METHOD(thread_p_p0_d1);

    SC_METHOD(thread_p_p0_we0);

    SC_METHOD(thread_p_p0_we1);

    SC_THREAD(thread_hdltv_gen);
    sensitive << ( ap_clk.pos() );

    SC_THREAD(thread_ap_var_for_const0);

    offload_Loop_offload_s0_y_yi_proc_U0_ap_start = SC_LOGIC_0;
    static int apTFileNum = 0;
    stringstream apTFilenSS;
    apTFilenSS << "offload_sc_trace_" << apTFileNum ++;
    string apTFn = apTFilenSS.str();
    mVcdFile = sc_create_vcd_trace_file(apTFn.c_str());
    mVcdFile->set_time_unit(1, SC_PS);
    if (1) {
#ifdef __HLS_TRACE_LEVEL_PORT__
    sc_trace(mVcdFile, p_p0_address0, "(port)p_p0_address0");
    sc_trace(mVcdFile, p_p0_ce0, "(port)p_p0_ce0");
    sc_trace(mVcdFile, p_p0_d0, "(port)p_p0_d0");
    sc_trace(mVcdFile, p_p0_q0, "(port)p_p0_q0");
    sc_trace(mVcdFile, p_p0_we0, "(port)p_p0_we0");
    sc_trace(mVcdFile, p_p0_address1, "(port)p_p0_address1");
    sc_trace(mVcdFile, p_p0_ce1, "(port)p_p0_ce1");
    sc_trace(mVcdFile, p_p0_d1, "(port)p_p0_d1");
    sc_trace(mVcdFile, p_p0_q1, "(port)p_p0_q1");
    sc_trace(mVcdFile, p_p0_we1, "(port)p_p0_we1");
    sc_trace(mVcdFile, p_offload_address0, "(port)p_offload_address0");
    sc_trace(mVcdFile, p_offload_ce0, "(port)p_offload_ce0");
    sc_trace(mVcdFile, p_offload_d0, "(port)p_offload_d0");
    sc_trace(mVcdFile, p_offload_q0, "(port)p_offload_q0");
    sc_trace(mVcdFile, p_offload_we0, "(port)p_offload_we0");
    sc_trace(mVcdFile, p_offload_address1, "(port)p_offload_address1");
    sc_trace(mVcdFile, p_offload_ce1, "(port)p_offload_ce1");
    sc_trace(mVcdFile, p_offload_d1, "(port)p_offload_d1");
    sc_trace(mVcdFile, p_offload_q1, "(port)p_offload_q1");
    sc_trace(mVcdFile, p_offload_we1, "(port)p_offload_we1");
    sc_trace(mVcdFile, ap_clk, "(port)ap_clk");
    sc_trace(mVcdFile, ap_rst, "(port)ap_rst");
    sc_trace(mVcdFile, ap_done, "(port)ap_done");
    sc_trace(mVcdFile, ap_start, "(port)ap_start");
    sc_trace(mVcdFile, ap_ready, "(port)ap_ready");
    sc_trace(mVcdFile, ap_idle, "(port)ap_idle");
#endif
#ifdef __HLS_TRACE_LEVEL_INT__
    sc_trace(mVcdFile, offload_Loop_distributor_p0_1_proc_U0_ap_start, "offload_Loop_distributor_p0_1_proc_U0_ap_start");
    sc_trace(mVcdFile, offload_Loop_distributor_p0_1_proc_U0_ap_done, "offload_Loop_distributor_p0_1_proc_U0_ap_done");
    sc_trace(mVcdFile, offload_Loop_distributor_p0_1_proc_U0_ap_continue, "offload_Loop_distributor_p0_1_proc_U0_ap_continue");
    sc_trace(mVcdFile, offload_Loop_distributor_p0_1_proc_U0_ap_idle, "offload_Loop_distributor_p0_1_proc_U0_ap_idle");
    sc_trace(mVcdFile, offload_Loop_distributor_p0_1_proc_U0_ap_ready, "offload_Loop_distributor_p0_1_proc_U0_ap_ready");
    sc_trace(mVcdFile, offload_Loop_distributor_p0_1_proc_U0_p_p0_address0, "offload_Loop_distributor_p0_1_proc_U0_p_p0_address0");
    sc_trace(mVcdFile, offload_Loop_distributor_p0_1_proc_U0_p_p0_ce0, "offload_Loop_distributor_p0_1_proc_U0_p_p0_ce0");
    sc_trace(mVcdFile, offload_Loop_distributor_p0_1_proc_U0_p_p0_to_offload_s0_stream_V_din, "offload_Loop_distributor_p0_1_proc_U0_p_p0_to_offload_s0_stream_V_din");
    sc_trace(mVcdFile, offload_Loop_distributor_p0_1_proc_U0_p_p0_to_offload_s0_stream_V_write, "offload_Loop_distributor_p0_1_proc_U0_p_p0_to_offload_s0_stream_V_write");
    sc_trace(mVcdFile, offload_Loop_offload_s0_y_yi_proc_U0_ap_start, "offload_Loop_offload_s0_y_yi_proc_U0_ap_start");
    sc_trace(mVcdFile, offload_Loop_offload_s0_y_yi_proc_U0_ap_done, "offload_Loop_offload_s0_y_yi_proc_U0_ap_done");
    sc_trace(mVcdFile, offload_Loop_offload_s0_y_yi_proc_U0_ap_continue, "offload_Loop_offload_s0_y_yi_proc_U0_ap_continue");
    sc_trace(mVcdFile, offload_Loop_offload_s0_y_yi_proc_U0_ap_idle, "offload_Loop_offload_s0_y_yi_proc_U0_ap_idle");
    sc_trace(mVcdFile, offload_Loop_offload_s0_y_yi_proc_U0_ap_ready, "offload_Loop_offload_s0_y_yi_proc_U0_ap_ready");
    sc_trace(mVcdFile, offload_Loop_offload_s0_y_yi_proc_U0_p_p0_to_offload_s0_stream_V_read, "offload_Loop_offload_s0_y_yi_proc_U0_p_p0_to_offload_s0_stream_V_read");
    sc_trace(mVcdFile, offload_Loop_offload_s0_y_yi_proc_U0_p_offload_address0, "offload_Loop_offload_s0_y_yi_proc_U0_p_offload_address0");
    sc_trace(mVcdFile, offload_Loop_offload_s0_y_yi_proc_U0_p_offload_ce0, "offload_Loop_offload_s0_y_yi_proc_U0_p_offload_ce0");
    sc_trace(mVcdFile, offload_Loop_offload_s0_y_yi_proc_U0_p_offload_we0, "offload_Loop_offload_s0_y_yi_proc_U0_p_offload_we0");
    sc_trace(mVcdFile, offload_Loop_offload_s0_y_yi_proc_U0_p_offload_d0, "offload_Loop_offload_s0_y_yi_proc_U0_p_offload_d0");
    sc_trace(mVcdFile, ap_sig_hs_continue, "ap_sig_hs_continue");
    sc_trace(mVcdFile, p_p0_to_offload_s0_stream_V_full_n, "p_p0_to_offload_s0_stream_V_full_n");
    sc_trace(mVcdFile, p_p0_to_offload_s0_stream_V_dout, "p_p0_to_offload_s0_stream_V_dout");
    sc_trace(mVcdFile, p_p0_to_offload_s0_stream_V_empty_n, "p_p0_to_offload_s0_stream_V_empty_n");
    sc_trace(mVcdFile, ap_sig_hs_done, "ap_sig_hs_done");
    sc_trace(mVcdFile, ap_sig_hs_ready, "ap_sig_hs_ready");
#endif

    }
    mHdltvinHandle.open("offload.hdltvin.dat");
    mHdltvoutHandle.open("offload.hdltvout.dat");
}

offload::~offload() {
    if (mVcdFile) 
        sc_close_vcd_trace_file(mVcdFile);

    mHdltvinHandle << "] " << endl;
    mHdltvoutHandle << "] " << endl;
    mHdltvinHandle.close();
    mHdltvoutHandle.close();
    delete offload_Loop_distributor_p0_1_proc_U0;
    delete offload_Loop_offload_s0_y_yi_proc_U0;
    delete p_p0_to_offload_s0_stream_V_U;
}

void offload::thread_ap_var_for_const0() {
    ap_var_for_const0 = ap_const_logic_1;
}

void offload::thread_ap_clk_no_reset_() {
    if ( ap_rst.read() == ap_const_logic_1) {
        offload_Loop_offload_s0_y_yi_proc_U0_ap_start = ap_const_logic_0;
    } else {
        offload_Loop_offload_s0_y_yi_proc_U0_ap_start = ap_const_logic_1;
    }
}

void offload::thread_ap_done() {
    ap_done = ap_sig_hs_done.read();
}

void offload::thread_ap_idle() {
    if ((esl_seteq<1,1,1>(offload_Loop_distributor_p0_1_proc_U0_ap_idle.read(), ap_const_logic_1) && 
         esl_seteq<1,1,1>(ap_const_logic_1, offload_Loop_offload_s0_y_yi_proc_U0_ap_idle.read()))) {
        ap_idle = ap_const_logic_1;
    } else {
        ap_idle = ap_const_logic_0;
    }
}

void offload::thread_ap_ready() {
    ap_ready = offload_Loop_distributor_p0_1_proc_U0_ap_ready.read();
}

void offload::thread_ap_sig_hs_continue() {
    ap_sig_hs_continue = ap_const_logic_1;
}

void offload::thread_ap_sig_hs_done() {
    if (esl_seteq<1,1,1>(ap_const_logic_1, offload_Loop_offload_s0_y_yi_proc_U0_ap_done.read())) {
        ap_sig_hs_done = ap_const_logic_1;
    } else {
        ap_sig_hs_done = ap_const_logic_0;
    }
}

void offload::thread_ap_sig_hs_ready() {
    ap_sig_hs_ready = offload_Loop_distributor_p0_1_proc_U0_ap_ready.read();
}

void offload::thread_offload_Loop_distributor_p0_1_proc_U0_ap_continue() {
    offload_Loop_distributor_p0_1_proc_U0_ap_continue = ap_const_logic_1;
}

void offload::thread_offload_Loop_distributor_p0_1_proc_U0_ap_start() {
    offload_Loop_distributor_p0_1_proc_U0_ap_start = ap_start.read();
}

void offload::thread_offload_Loop_offload_s0_y_yi_proc_U0_ap_continue() {
    offload_Loop_offload_s0_y_yi_proc_U0_ap_continue = ap_const_logic_1;
}

void offload::thread_p_offload_address0() {
    p_offload_address0 = offload_Loop_offload_s0_y_yi_proc_U0_p_offload_address0.read();
}

void offload::thread_p_offload_address1() {
    p_offload_address1 = ap_const_lv14_0;
}

void offload::thread_p_offload_ce0() {
    p_offload_ce0 = offload_Loop_offload_s0_y_yi_proc_U0_p_offload_ce0.read();
}

void offload::thread_p_offload_ce1() {
    p_offload_ce1 = ap_const_logic_0;
}

void offload::thread_p_offload_d0() {
    p_offload_d0 = offload_Loop_offload_s0_y_yi_proc_U0_p_offload_d0.read();
}

void offload::thread_p_offload_d1() {
    p_offload_d1 = ap_const_lv8_0;
}

void offload::thread_p_offload_we0() {
    p_offload_we0 = offload_Loop_offload_s0_y_yi_proc_U0_p_offload_we0.read();
}

void offload::thread_p_offload_we1() {
    p_offload_we1 = ap_const_logic_0;
}

void offload::thread_p_p0_address0() {
    p_p0_address0 = offload_Loop_distributor_p0_1_proc_U0_p_p0_address0.read();
}

void offload::thread_p_p0_address1() {
    p_p0_address1 = ap_const_lv14_0;
}

void offload::thread_p_p0_ce0() {
    p_p0_ce0 = offload_Loop_distributor_p0_1_proc_U0_p_p0_ce0.read();
}

void offload::thread_p_p0_ce1() {
    p_p0_ce1 = ap_const_logic_0;
}

void offload::thread_p_p0_d0() {
    p_p0_d0 = ap_const_lv8_0;
}

void offload::thread_p_p0_d1() {
    p_p0_d1 = ap_const_lv8_0;
}

void offload::thread_p_p0_we0() {
    p_p0_we0 = ap_const_logic_0;
}

void offload::thread_p_p0_we1() {
    p_p0_we1 = ap_const_logic_0;
}

void offload::thread_hdltv_gen() {
    const char* dump_tv = std::getenv("AP_WRITE_TV");
    if (!(dump_tv && string(dump_tv) == "on")) return;

    wait();

    mHdltvinHandle << "[ " << endl;
    mHdltvoutHandle << "[ " << endl;
    int ap_cycleNo = 0;
    while (1) {
        wait();
        const char* mComma = ap_cycleNo == 0 ? " " : ", " ;
        mHdltvoutHandle << mComma << "{"  <<  " \"p_p0_address0\" :  \"" << p_p0_address0.read() << "\" ";
        mHdltvoutHandle << " , " <<  " \"p_p0_ce0\" :  \"" << p_p0_ce0.read() << "\" ";
        mHdltvoutHandle << " , " <<  " \"p_p0_d0\" :  \"" << p_p0_d0.read() << "\" ";
        mHdltvinHandle << mComma << "{"  <<  " \"p_p0_q0\" :  \"" << p_p0_q0.read() << "\" ";
        mHdltvoutHandle << " , " <<  " \"p_p0_we0\" :  \"" << p_p0_we0.read() << "\" ";
        mHdltvoutHandle << " , " <<  " \"p_p0_address1\" :  \"" << p_p0_address1.read() << "\" ";
        mHdltvoutHandle << " , " <<  " \"p_p0_ce1\" :  \"" << p_p0_ce1.read() << "\" ";
        mHdltvoutHandle << " , " <<  " \"p_p0_d1\" :  \"" << p_p0_d1.read() << "\" ";
        mHdltvinHandle << " , " <<  " \"p_p0_q1\" :  \"" << p_p0_q1.read() << "\" ";
        mHdltvoutHandle << " , " <<  " \"p_p0_we1\" :  \"" << p_p0_we1.read() << "\" ";
        mHdltvoutHandle << " , " <<  " \"p_offload_address0\" :  \"" << p_offload_address0.read() << "\" ";
        mHdltvoutHandle << " , " <<  " \"p_offload_ce0\" :  \"" << p_offload_ce0.read() << "\" ";
        mHdltvoutHandle << " , " <<  " \"p_offload_d0\" :  \"" << p_offload_d0.read() << "\" ";
        mHdltvinHandle << " , " <<  " \"p_offload_q0\" :  \"" << p_offload_q0.read() << "\" ";
        mHdltvoutHandle << " , " <<  " \"p_offload_we0\" :  \"" << p_offload_we0.read() << "\" ";
        mHdltvoutHandle << " , " <<  " \"p_offload_address1\" :  \"" << p_offload_address1.read() << "\" ";
        mHdltvoutHandle << " , " <<  " \"p_offload_ce1\" :  \"" << p_offload_ce1.read() << "\" ";
        mHdltvoutHandle << " , " <<  " \"p_offload_d1\" :  \"" << p_offload_d1.read() << "\" ";
        mHdltvinHandle << " , " <<  " \"p_offload_q1\" :  \"" << p_offload_q1.read() << "\" ";
        mHdltvoutHandle << " , " <<  " \"p_offload_we1\" :  \"" << p_offload_we1.read() << "\" ";
        mHdltvinHandle << " , " <<  " \"ap_rst\" :  \"" << ap_rst.read() << "\" ";
        mHdltvoutHandle << " , " <<  " \"ap_done\" :  \"" << ap_done.read() << "\" ";
        mHdltvinHandle << " , " <<  " \"ap_start\" :  \"" << ap_start.read() << "\" ";
        mHdltvoutHandle << " , " <<  " \"ap_ready\" :  \"" << ap_ready.read() << "\" ";
        mHdltvoutHandle << " , " <<  " \"ap_idle\" :  \"" << ap_idle.read() << "\" ";
        mHdltvinHandle << "}" << std::endl;
        mHdltvoutHandle << "}" << std::endl;
        ap_cycleNo++;
    }
}

}

