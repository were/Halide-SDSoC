<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>
<!DOCTYPE boost_serialization>
<boost_serialization signature="serialization::archive" version="14">
  <syndb class_id="0" tracking_level="0" version="0">
    <userIPLatency>-1</userIPLatency>
    <userIPName/>
    <cdfg class_id="1" tracking_level="1" version="0" object_id="_0">
      <name>offload</name>
      <ret_bitwidth>0</ret_bitwidth>
      <ports class_id="2" tracking_level="0" version="0">
        <count>2</count>
        <item_version>0</item_version>
        <item class_id="3" tracking_level="1" version="0" object_id="_1">
          <Value class_id="4" tracking_level="0" version="0">
            <Obj class_id="5" tracking_level="0" version="0">
              <type>1</type>
              <id>1</id>
              <name>p_p0</name>
              <fileName/>
              <fileDirectory/>
              <lineNumber>0</lineNumber>
              <contextFuncName/>
              <inlineStackInfo class_id="6" tracking_level="0" version="0">
                <count>0</count>
                <item_version>0</item_version>
              </inlineStackInfo>
              <originalName>_p0</originalName>
              <rtlName/>
              <coreName>RAM</coreName>
            </Obj>
            <bitwidth>8</bitwidth>
          </Value>
          <direction>0</direction>
          <if_type>1</if_type>
          <array_size>10000</array_size>
          <bit_vecs class_id="7" tracking_level="0" version="0">
            <count>0</count>
            <item_version>0</item_version>
          </bit_vecs>
        </item>
        <item class_id_reference="3" object_id="_2">
          <Value>
            <Obj>
              <type>1</type>
              <id>2</id>
              <name>p_offload</name>
              <fileName/>
              <fileDirectory/>
              <lineNumber>0</lineNumber>
              <contextFuncName/>
              <inlineStackInfo>
                <count>0</count>
                <item_version>0</item_version>
              </inlineStackInfo>
              <originalName>_offload</originalName>
              <rtlName/>
              <coreName>RAM</coreName>
            </Obj>
            <bitwidth>8</bitwidth>
          </Value>
          <direction>1</direction>
          <if_type>1</if_type>
          <array_size>10000</array_size>
          <bit_vecs>
            <count>0</count>
            <item_version>0</item_version>
          </bit_vecs>
        </item>
      </ports>
      <nodes class_id="8" tracking_level="0" version="0">
        <count>4</count>
        <item_version>0</item_version>
        <item class_id="9" tracking_level="1" version="0" object_id="_3">
          <Value>
            <Obj>
              <type>0</type>
              <id>7</id>
              <name>p_p0_to_offload_s0_stream_V</name>
              <fileName/>
              <fileDirectory/>
              <lineNumber>0</lineNumber>
              <contextFuncName/>
              <inlineStackInfo>
                <count>0</count>
                <item_version>0</item_version>
              </inlineStackInfo>
              <originalName/>
              <rtlName>p_p0_to_offload_s0_stream_V_U</rtlName>
              <coreName>FIFO</coreName>
            </Obj>
            <bitwidth>8</bitwidth>
          </Value>
          <oprand_edges>
            <count>1</count>
            <item_version>0</item_version>
            <item>15</item>
          </oprand_edges>
          <opcode>alloca</opcode>
        </item>
        <item class_id_reference="9" object_id="_4">
          <Value>
            <Obj>
              <type>0</type>
              <id>10</id>
              <name/>
              <fileName/>
              <fileDirectory/>
              <lineNumber>0</lineNumber>
              <contextFuncName/>
              <inlineStackInfo>
                <count>0</count>
                <item_version>0</item_version>
              </inlineStackInfo>
              <originalName/>
              <rtlName>offload_Loop_distributor_p0_1_proc_U0</rtlName>
              <coreName/>
            </Obj>
            <bitwidth>0</bitwidth>
          </Value>
          <oprand_edges>
            <count>3</count>
            <item_version>0</item_version>
            <item>17</item>
            <item>18</item>
            <item>19</item>
          </oprand_edges>
          <opcode>call</opcode>
        </item>
        <item class_id_reference="9" object_id="_5">
          <Value>
            <Obj>
              <type>0</type>
              <id>11</id>
              <name/>
              <fileName/>
              <fileDirectory/>
              <lineNumber>0</lineNumber>
              <contextFuncName/>
              <inlineStackInfo>
                <count>0</count>
                <item_version>0</item_version>
              </inlineStackInfo>
              <originalName/>
              <rtlName>offload_Loop_offload_s0_y_yi_proc_U0</rtlName>
              <coreName/>
            </Obj>
            <bitwidth>0</bitwidth>
          </Value>
          <oprand_edges>
            <count>5</count>
            <item_version>0</item_version>
            <item>21</item>
            <item>22</item>
            <item>23</item>
            <item>72</item>
            <item>73</item>
          </oprand_edges>
          <opcode>call</opcode>
        </item>
        <item class_id_reference="9" object_id="_6">
          <Value>
            <Obj>
              <type>0</type>
              <id>12</id>
              <name/>
              <fileName>offload.cpp</fileName>
              <fileDirectory>..</fileDirectory>
              <lineNumber>179</lineNumber>
              <contextFuncName>offload</contextFuncName>
              <inlineStackInfo>
                <count>1</count>
                <item_version>0</item_version>
                <item class_id="11" tracking_level="0" version="0">
                  <first>/work/zhang/common/tools/halide-working/Halide-SDSoC/apps/sds_simple</first>
                  <second class_id="12" tracking_level="0" version="0">
                    <count>1</count>
                    <item_version>0</item_version>
                    <item class_id="13" tracking_level="0" version="0">
                      <first class_id="14" tracking_level="0" version="0">
                        <first>offload.cpp</first>
                        <second>offload</second>
                      </first>
                      <second>179</second>
                    </item>
                  </second>
                </item>
              </inlineStackInfo>
              <originalName/>
              <rtlName/>
              <coreName/>
            </Obj>
            <bitwidth>0</bitwidth>
          </Value>
          <oprand_edges>
            <count>0</count>
            <item_version>0</item_version>
          </oprand_edges>
          <opcode>ret</opcode>
        </item>
      </nodes>
      <consts class_id="15" tracking_level="0" version="0">
        <count>3</count>
        <item_version>0</item_version>
        <item class_id="16" tracking_level="1" version="0" object_id="_7">
          <Value>
            <Obj>
              <type>2</type>
              <id>14</id>
              <name>empty</name>
              <fileName/>
              <fileDirectory/>
              <lineNumber>0</lineNumber>
              <contextFuncName/>
              <inlineStackInfo>
                <count>0</count>
                <item_version>0</item_version>
              </inlineStackInfo>
              <originalName/>
              <rtlName/>
              <coreName/>
            </Obj>
            <bitwidth>64</bitwidth>
          </Value>
          <const_type>0</const_type>
          <content>1</content>
        </item>
        <item class_id_reference="16" object_id="_8">
          <Value>
            <Obj>
              <type>2</type>
              <id>16</id>
              <name>offload_Loop_distributor_p0_1_proc</name>
              <fileName/>
              <fileDirectory/>
              <lineNumber>0</lineNumber>
              <contextFuncName/>
              <inlineStackInfo>
                <count>0</count>
                <item_version>0</item_version>
              </inlineStackInfo>
              <originalName/>
              <rtlName/>
              <coreName/>
            </Obj>
            <bitwidth>0</bitwidth>
          </Value>
          <const_type>6</const_type>
          <content>&lt;constant:offload_Loop__distributor_p0_1_proc&gt;</content>
        </item>
        <item class_id_reference="16" object_id="_9">
          <Value>
            <Obj>
              <type>2</type>
              <id>20</id>
              <name>offload_Loop_offload_s0_y_yi_proc</name>
              <fileName/>
              <fileDirectory/>
              <lineNumber>0</lineNumber>
              <contextFuncName/>
              <inlineStackInfo>
                <count>0</count>
                <item_version>0</item_version>
              </inlineStackInfo>
              <originalName/>
              <rtlName/>
              <coreName/>
            </Obj>
            <bitwidth>0</bitwidth>
          </Value>
          <const_type>6</const_type>
          <content>&lt;constant:offload_Loop__offload_s0_y_yi_proc&gt;</content>
        </item>
      </consts>
      <blocks class_id="17" tracking_level="0" version="0">
        <count>1</count>
        <item_version>0</item_version>
        <item class_id="18" tracking_level="1" version="0" object_id="_10">
          <Obj>
            <type>3</type>
            <id>13</id>
            <name>offload</name>
            <fileName/>
            <fileDirectory/>
            <lineNumber>0</lineNumber>
            <contextFuncName/>
            <inlineStackInfo>
              <count>0</count>
              <item_version>0</item_version>
            </inlineStackInfo>
            <originalName/>
            <rtlName/>
            <coreName/>
          </Obj>
          <node_objs>
            <count>4</count>
            <item_version>0</item_version>
            <item>7</item>
            <item>10</item>
            <item>11</item>
            <item>12</item>
          </node_objs>
        </item>
      </blocks>
      <edges class_id="19" tracking_level="0" version="0">
        <count>9</count>
        <item_version>0</item_version>
        <item class_id="20" tracking_level="1" version="0" object_id="_11">
          <id>15</id>
          <edge_type>1</edge_type>
          <source_obj>14</source_obj>
          <sink_obj>7</sink_obj>
        </item>
        <item class_id_reference="20" object_id="_12">
          <id>17</id>
          <edge_type>1</edge_type>
          <source_obj>16</source_obj>
          <sink_obj>10</sink_obj>
        </item>
        <item class_id_reference="20" object_id="_13">
          <id>18</id>
          <edge_type>1</edge_type>
          <source_obj>1</source_obj>
          <sink_obj>10</sink_obj>
        </item>
        <item class_id_reference="20" object_id="_14">
          <id>19</id>
          <edge_type>1</edge_type>
          <source_obj>7</source_obj>
          <sink_obj>10</sink_obj>
        </item>
        <item class_id_reference="20" object_id="_15">
          <id>21</id>
          <edge_type>1</edge_type>
          <source_obj>20</source_obj>
          <sink_obj>11</sink_obj>
        </item>
        <item class_id_reference="20" object_id="_16">
          <id>22</id>
          <edge_type>1</edge_type>
          <source_obj>7</source_obj>
          <sink_obj>11</sink_obj>
        </item>
        <item class_id_reference="20" object_id="_17">
          <id>23</id>
          <edge_type>1</edge_type>
          <source_obj>2</source_obj>
          <sink_obj>11</sink_obj>
        </item>
        <item class_id_reference="20" object_id="_18">
          <id>72</id>
          <edge_type>4</edge_type>
          <source_obj>10</source_obj>
          <sink_obj>11</sink_obj>
        </item>
        <item class_id_reference="20" object_id="_19">
          <id>73</id>
          <edge_type>4</edge_type>
          <source_obj>10</source_obj>
          <sink_obj>11</sink_obj>
        </item>
      </edges>
    </cdfg>
    <cdfg_regions class_id="21" tracking_level="0" version="0">
      <count>1</count>
      <item_version>0</item_version>
      <item class_id="22" tracking_level="1" version="0" object_id="_20">
        <mId>1</mId>
        <mTag>offload</mTag>
        <mType>0</mType>
        <sub_regions>
          <count>0</count>
          <item_version>0</item_version>
        </sub_regions>
        <basic_blocks>
          <count>1</count>
          <item_version>0</item_version>
          <item>13</item>
        </basic_blocks>
        <mII>-1</mII>
        <mDepth>-1</mDepth>
        <mMinTripCount>-1</mMinTripCount>
        <mMaxTripCount>-1</mMaxTripCount>
        <mMinLatency>10401</mMinLatency>
        <mMaxLatency>-1</mMaxLatency>
        <mIsDfPipe>1</mIsDfPipe>
        <mDfPipe class_id="23" tracking_level="1" version="0" object_id="_21">
          <port_list class_id="24" tracking_level="0" version="0">
            <count>0</count>
            <item_version>0</item_version>
          </port_list>
          <process_list class_id="25" tracking_level="0" version="0">
            <count>2</count>
            <item_version>0</item_version>
            <item class_id="26" tracking_level="1" version="0" object_id="_22">
              <type>0</type>
              <name>offload_Loop_distributor_p0_1_proc_U0</name>
              <ssdmobj_id>10</ssdmobj_id>
              <pins class_id="27" tracking_level="0" version="0">
                <count>2</count>
                <item_version>0</item_version>
                <item class_id="28" tracking_level="1" version="0" object_id="_23">
                  <port class_id="29" tracking_level="1" version="0" object_id="_24">
                    <name>p_p0</name>
                    <dir>2</dir>
                    <type>0</type>
                  </port>
                  <inst class_id="30" tracking_level="1" version="0" object_id="_25">
                    <type>0</type>
                    <name>offload_Loop_distributor_p0_1_proc_U0</name>
                    <ssdmobj_id>10</ssdmobj_id>
                  </inst>
                </item>
                <item class_id_reference="28" object_id="_26">
                  <port class_id_reference="29" object_id="_27">
                    <name>p_p0_to_offload_s0_stream_V</name>
                    <dir>0</dir>
                    <type>1</type>
                  </port>
                  <inst class_id_reference="30" object_id_reference="_25"/>
                </item>
              </pins>
            </item>
            <item class_id_reference="26" object_id="_28">
              <type>0</type>
              <name>offload_Loop_offload_s0_y_yi_proc_U0</name>
              <ssdmobj_id>11</ssdmobj_id>
              <pins>
                <count>2</count>
                <item_version>0</item_version>
                <item class_id_reference="28" object_id="_29">
                  <port class_id_reference="29" object_id="_30">
                    <name>p_p0_to_offload_s0_stream_V</name>
                    <dir>0</dir>
                    <type>0</type>
                  </port>
                  <inst class_id_reference="30" object_id="_31">
                    <type>0</type>
                    <name>offload_Loop_offload_s0_y_yi_proc_U0</name>
                    <ssdmobj_id>11</ssdmobj_id>
                  </inst>
                </item>
                <item class_id_reference="28" object_id="_32">
                  <port class_id_reference="29" object_id="_33">
                    <name>p_offload</name>
                    <dir>2</dir>
                    <type>1</type>
                  </port>
                  <inst class_id_reference="30" object_id_reference="_31"/>
                </item>
              </pins>
            </item>
          </process_list>
          <channel_list class_id="31" tracking_level="0" version="0">
            <count>1</count>
            <item_version>0</item_version>
            <item class_id="32" tracking_level="1" version="0" object_id="_34">
              <type>1</type>
              <name>p_p0_to_offload_s0_stream_V</name>
              <ssdmobj_id>7</ssdmobj_id>
              <ctype>0</ctype>
              <depth>1</depth>
              <bitwidth>8</bitwidth>
              <source class_id_reference="28" object_id="_35">
                <port class_id_reference="29" object_id="_36">
                  <name>in</name>
                  <dir>3</dir>
                  <type>0</type>
                </port>
                <inst class_id_reference="30" object_id_reference="_25"/>
              </source>
              <sink class_id_reference="28" object_id="_37">
                <port class_id_reference="29" object_id="_38">
                  <name>out</name>
                  <dir>3</dir>
                  <type>1</type>
                </port>
                <inst class_id_reference="30" object_id_reference="_31"/>
              </sink>
            </item>
          </channel_list>
          <net_list class_id="33" tracking_level="0" version="0">
            <count>0</count>
            <item_version>0</item_version>
          </net_list>
        </mDfPipe>
      </item>
    </cdfg_regions>
    <fsm class_id="34" tracking_level="1" version="0" object_id="_39">
      <states class_id="35" tracking_level="0" version="0">
        <count>4</count>
        <item_version>0</item_version>
        <item class_id="36" tracking_level="1" version="0" object_id="_40">
          <id>1</id>
          <operations class_id="37" tracking_level="0" version="0">
            <count>2</count>
            <item_version>0</item_version>
            <item class_id="38" tracking_level="1" version="0" object_id="_41">
              <id>7</id>
              <stage>1</stage>
              <latency>1</latency>
            </item>
            <item class_id_reference="38" object_id="_42">
              <id>10</id>
              <stage>2</stage>
              <latency>2</latency>
            </item>
          </operations>
        </item>
        <item class_id_reference="36" object_id="_43">
          <id>2</id>
          <operations>
            <count>1</count>
            <item_version>0</item_version>
            <item class_id_reference="38" object_id="_44">
              <id>10</id>
              <stage>1</stage>
              <latency>2</latency>
            </item>
          </operations>
        </item>
        <item class_id_reference="36" object_id="_45">
          <id>3</id>
          <operations>
            <count>1</count>
            <item_version>0</item_version>
            <item class_id_reference="38" object_id="_46">
              <id>11</id>
              <stage>2</stage>
              <latency>2</latency>
            </item>
          </operations>
        </item>
        <item class_id_reference="36" object_id="_47">
          <id>4</id>
          <operations>
            <count>8</count>
            <item_version>0</item_version>
            <item class_id_reference="38" object_id="_48">
              <id>3</id>
              <stage>1</stage>
              <latency>1</latency>
            </item>
            <item class_id_reference="38" object_id="_49">
              <id>4</id>
              <stage>1</stage>
              <latency>1</latency>
            </item>
            <item class_id_reference="38" object_id="_50">
              <id>5</id>
              <stage>1</stage>
              <latency>1</latency>
            </item>
            <item class_id_reference="38" object_id="_51">
              <id>6</id>
              <stage>1</stage>
              <latency>1</latency>
            </item>
            <item class_id_reference="38" object_id="_52">
              <id>8</id>
              <stage>1</stage>
              <latency>1</latency>
            </item>
            <item class_id_reference="38" object_id="_53">
              <id>9</id>
              <stage>1</stage>
              <latency>1</latency>
            </item>
            <item class_id_reference="38" object_id="_54">
              <id>11</id>
              <stage>1</stage>
              <latency>2</latency>
            </item>
            <item class_id_reference="38" object_id="_55">
              <id>12</id>
              <stage>1</stage>
              <latency>1</latency>
            </item>
          </operations>
        </item>
      </states>
      <transitions class_id="39" tracking_level="0" version="0">
        <count>3</count>
        <item_version>0</item_version>
        <item class_id="40" tracking_level="1" version="0" object_id="_56">
          <inState>1</inState>
          <outState>2</outState>
          <condition class_id="41" tracking_level="0" version="0">
            <id>0</id>
            <sop class_id="42" tracking_level="0" version="0">
              <count>1</count>
              <item_version>0</item_version>
              <item class_id="43" tracking_level="0" version="0">
                <count>0</count>
                <item_version>0</item_version>
              </item>
            </sop>
          </condition>
        </item>
        <item class_id_reference="40" object_id="_57">
          <inState>2</inState>
          <outState>3</outState>
          <condition>
            <id>1</id>
            <sop>
              <count>1</count>
              <item_version>0</item_version>
              <item>
                <count>0</count>
                <item_version>0</item_version>
              </item>
            </sop>
          </condition>
        </item>
        <item class_id_reference="40" object_id="_58">
          <inState>3</inState>
          <outState>4</outState>
          <condition>
            <id>2</id>
            <sop>
              <count>1</count>
              <item_version>0</item_version>
              <item>
                <count>0</count>
                <item_version>0</item_version>
              </item>
            </sop>
          </condition>
        </item>
      </transitions>
    </fsm>
    <res class_id="44" tracking_level="1" version="0" object_id="_59">
      <dp_component_resource class_id="45" tracking_level="0" version="0">
        <count>2</count>
        <item_version>0</item_version>
        <item class_id="46" tracking_level="0" version="0">
          <first>offload_Loop_distributor_p0_1_proc_U0 (offload_Loop_distributor_p0_1_proc)</first>
          <second class_id="47" tracking_level="0" version="0">
            <count>2</count>
            <item_version>0</item_version>
            <item class_id="48" tracking_level="0" version="0">
              <first>FF</first>
              <second>73</second>
            </item>
            <item>
              <first>LUT</first>
              <second>81</second>
            </item>
          </second>
        </item>
        <item>
          <first>offload_Loop_offload_s0_y_yi_proc_U0 (offload_Loop_offload_s0_y_yi_proc)</first>
          <second>
            <count>2</count>
            <item_version>0</item_version>
            <item>
              <first>FF</first>
              <second>71</second>
            </item>
            <item>
              <first>LUT</first>
              <second>80</second>
            </item>
          </second>
        </item>
      </dp_component_resource>
      <dp_expression_resource>
        <count>0</count>
        <item_version>0</item_version>
      </dp_expression_resource>
      <dp_fifo_resource>
        <count>1</count>
        <item_version>0</item_version>
        <item>
          <first>p_p0_to_offload_s0_stream_V_U</first>
          <second>
            <count>6</count>
            <item_version>0</item_version>
            <item>
              <first>(0Depth)</first>
              <second>1</second>
            </item>
            <item>
              <first>(1Bits)</first>
              <second>8</second>
            </item>
            <item>
              <first>(2Size:D*B)</first>
              <second>8</second>
            </item>
            <item>
              <first>BRAM</first>
              <second>0</second>
            </item>
            <item>
              <first>FF</first>
              <second>5</second>
            </item>
            <item>
              <first>LUT</first>
              <second>20</second>
            </item>
          </second>
        </item>
      </dp_fifo_resource>
      <dp_memory_resource>
        <count>0</count>
        <item_version>0</item_version>
      </dp_memory_resource>
      <dp_multiplexer_resource>
        <count>0</count>
        <item_version>0</item_version>
      </dp_multiplexer_resource>
      <dp_register_resource>
        <count>1</count>
        <item_version>0</item_version>
        <item>
          <first>offload_Loop_offload_s0_y_yi_proc_U0_ap_start</first>
          <second>
            <count>3</count>
            <item_version>0</item_version>
            <item>
              <first>(Bits)</first>
              <second>1</second>
            </item>
            <item>
              <first>(Consts)</first>
              <second>0</second>
            </item>
            <item>
              <first>FF</first>
              <second>1</second>
            </item>
          </second>
        </item>
      </dp_register_resource>
      <dp_component_map class_id="49" tracking_level="0" version="0">
        <count>2</count>
        <item_version>0</item_version>
        <item class_id="50" tracking_level="0" version="0">
          <first>offload_Loop_distributor_p0_1_proc_U0 (offload_Loop_distributor_p0_1_proc)</first>
          <second>
            <count>1</count>
            <item_version>0</item_version>
            <item>10</item>
          </second>
        </item>
        <item>
          <first>offload_Loop_offload_s0_y_yi_proc_U0 (offload_Loop_offload_s0_y_yi_proc)</first>
          <second>
            <count>1</count>
            <item_version>0</item_version>
            <item>11</item>
          </second>
        </item>
      </dp_component_map>
      <dp_expression_map>
        <count>0</count>
        <item_version>0</item_version>
      </dp_expression_map>
      <dp_fifo_map>
        <count>1</count>
        <item_version>0</item_version>
        <item>
          <first>p_p0_to_offload_s0_stream_V_U</first>
          <second>
            <count>1</count>
            <item_version>0</item_version>
            <item>62</item>
          </second>
        </item>
      </dp_fifo_map>
      <dp_memory_map>
        <count>0</count>
        <item_version>0</item_version>
      </dp_memory_map>
    </res>
    <node_label_latency class_id="51" tracking_level="0" version="0">
      <count>4</count>
      <item_version>0</item_version>
      <item class_id="52" tracking_level="0" version="0">
        <first>7</first>
        <second class_id="53" tracking_level="0" version="0">
          <first>0</first>
          <second>0</second>
        </second>
      </item>
      <item>
        <first>10</first>
        <second>
          <first>0</first>
          <second>1</second>
        </second>
      </item>
      <item>
        <first>11</first>
        <second>
          <first>2</first>
          <second>1</second>
        </second>
      </item>
      <item>
        <first>12</first>
        <second>
          <first>3</first>
          <second>0</second>
        </second>
      </item>
    </node_label_latency>
    <bblk_ent_exit class_id="54" tracking_level="0" version="0">
      <count>1</count>
      <item_version>0</item_version>
      <item class_id="55" tracking_level="0" version="0">
        <first>13</first>
        <second class_id="56" tracking_level="0" version="0">
          <first>0</first>
          <second>3</second>
        </second>
      </item>
    </bblk_ent_exit>
    <regions class_id="57" tracking_level="0" version="0">
      <count>1</count>
      <item_version>0</item_version>
      <item class_id="58" tracking_level="1" version="0" object_id="_60">
        <region_name>offload</region_name>
        <basic_blocks>
          <count>1</count>
          <item_version>0</item_version>
          <item>13</item>
        </basic_blocks>
        <nodes>
          <count>10</count>
          <item_version>0</item_version>
          <item>3</item>
          <item>4</item>
          <item>5</item>
          <item>6</item>
          <item>7</item>
          <item>8</item>
          <item>9</item>
          <item>10</item>
          <item>11</item>
          <item>12</item>
        </nodes>
        <anchor_node>-1</anchor_node>
        <region_type>16</region_type>
        <interval>0</interval>
        <pipe_depth>0</pipe_depth>
      </item>
    </regions>
    <dp_fu_nodes class_id="59" tracking_level="0" version="0">
      <count>3</count>
      <item_version>0</item_version>
      <item class_id="60" tracking_level="0" version="0">
        <first>40</first>
        <second>
          <count>1</count>
          <item_version>0</item_version>
          <item>7</item>
        </second>
      </item>
      <item>
        <first>44</first>
        <second>
          <count>2</count>
          <item_version>0</item_version>
          <item>10</item>
          <item>10</item>
        </second>
      </item>
      <item>
        <first>51</first>
        <second>
          <count>2</count>
          <item_version>0</item_version>
          <item>11</item>
          <item>11</item>
        </second>
      </item>
    </dp_fu_nodes>
    <dp_fu_nodes_expression class_id="62" tracking_level="0" version="0">
      <count>1</count>
      <item_version>0</item_version>
      <item class_id="63" tracking_level="0" version="0">
        <first>p_p0_to_offload_s0_stream_V_fu_40</first>
        <second>
          <count>1</count>
          <item_version>0</item_version>
          <item>7</item>
        </second>
      </item>
    </dp_fu_nodes_expression>
    <dp_fu_nodes_module>
      <count>2</count>
      <item_version>0</item_version>
      <item>
        <first>grp_offload_Loop_distributor_p0_1_proc_fu_44</first>
        <second>
          <count>2</count>
          <item_version>0</item_version>
          <item>10</item>
          <item>10</item>
        </second>
      </item>
      <item>
        <first>grp_offload_Loop_offload_s0_y_yi_proc_fu_51</first>
        <second>
          <count>2</count>
          <item_version>0</item_version>
          <item>11</item>
          <item>11</item>
        </second>
      </item>
    </dp_fu_nodes_module>
    <dp_fu_nodes_io>
      <count>0</count>
      <item_version>0</item_version>
    </dp_fu_nodes_io>
    <return_ports>
      <count>0</count>
      <item_version>0</item_version>
    </return_ports>
    <dp_mem_port_nodes class_id="64" tracking_level="0" version="0">
      <count>0</count>
      <item_version>0</item_version>
    </dp_mem_port_nodes>
    <dp_reg_nodes>
      <count>1</count>
      <item_version>0</item_version>
      <item>
        <first>58</first>
        <second>
          <count>1</count>
          <item_version>0</item_version>
          <item>7</item>
        </second>
      </item>
    </dp_reg_nodes>
    <dp_regname_nodes>
      <count>1</count>
      <item_version>0</item_version>
      <item>
        <first>p_p0_to_offload_s0_stream_V_reg_58</first>
        <second>
          <count>1</count>
          <item_version>0</item_version>
          <item>7</item>
        </second>
      </item>
    </dp_regname_nodes>
    <dp_reg_phi>
      <count>0</count>
      <item_version>0</item_version>
    </dp_reg_phi>
    <dp_regname_phi>
      <count>0</count>
      <item_version>0</item_version>
    </dp_regname_phi>
    <dp_port_io_nodes class_id="65" tracking_level="0" version="0">
      <count>0</count>
      <item_version>0</item_version>
    </dp_port_io_nodes>
    <port2core class_id="66" tracking_level="0" version="0">
      <count>2</count>
      <item_version>0</item_version>
      <item class_id="67" tracking_level="0" version="0">
        <first>1</first>
        <second>RAM</second>
      </item>
      <item>
        <first>2</first>
        <second>RAM</second>
      </item>
    </port2core>
    <node2core>
      <count>1</count>
      <item_version>0</item_version>
      <item>
        <first>7</first>
        <second>FIFO</second>
      </item>
    </node2core>
  </syndb>
</boost_serialization>
