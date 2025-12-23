unit MMCWFP39;

interface

 function get_mmc_error         : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function get_version       : Longint       ;stdcall;   external    'MMCWFP39.DLL';
 function get_axis_num      : Smallint      ;stdcall;   external    'MMCWFP39.DLL';
 function get_bd_num        : Smallint      ;stdcall;   external    'MMCWFP39.DLL';
 function get_velocity    ( ax : Smallint)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';


 function mmc_initx       ( nBdTatalNum : Smallint; arrlAddr : pointer)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function version_chk     ( bd : Smallint; iVer : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function set_dpram_addr      ( nBdNum : Smallint;  lAddr : Longint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function get_dpram_addr      ( nBdNum : Smallint; plAddr : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function save_boot_frame      : Smallint     ;stdcall;   external    'MMCWFP39.DLL';

 function mmc_axes    ( bd : Smallint; arriaxes : pointer)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function mmc_all_axes      : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function get_stepper     ( ax : Smallint)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function set_stepper     ( ax : Smallint)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function set_servo       ( ax : Smallint)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function set_feedback    ( ax : Smallint;  iDev : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function get_feedback    ( ax : Smallint; piDev : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function set_closed_loop     ( ax : Smallint;  iloop : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function get_closed_loop     ( ax : Smallint; piloop : pointer )   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function set_unipolar    ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function get_unipolar    ( ax : Smallint)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';

 function set_abs_encoder     ( ax : Smallint)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function set_micro_stepper       ( ax : Smallint)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function get_micro_stepper       ( ax : Smallint)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';

 function mmcDelay    ( l : Longint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function mmc_dwell       ( ax : Smallint;  l : Longint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function mmc_io_trigger      ( ax : Smallint;  nBitNo : Smallint;  nState : Smallint)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';

 function get_counter     ( ax : Smallint; pdCounter : pointer)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function get_sync_position       (pdPos_m : pointer; pdPos_s : pointer)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';

 function set_position    ( ax : Smallint;  f : Double)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function get_position    ( ax : Smallint; pdPos : pointer)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function set_command     ( ax : Smallint;  f : Double)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function get_command     ( ax : Smallint; pdCommand : pointer)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function get_error       ( ax : Smallint; pdErr : pointer)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function get_com_velocity    ( ax : Smallint)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function get_act_velocity    ( ax : Smallint)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';

 function in_sequence     ( ax : Smallint)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function in_motion       ( ax : Smallint)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function in_position     ( ax : Smallint)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function frames_left     ( ax : Smallint)   : Smallint ;stdcall;   external    'MMCWFP39.DLL';
 function motion_done     ( ax : Smallint)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function axis_done       ( ax : Smallint)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function axis_state      ( ax : Smallint)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function axis_source     ( ax : Smallint)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function axis_sourcex    ( ax : Smallint)   : Longint   ;stdcall;   external    'MMCWFP39.DLL';
 function clear_status    ( ax : Smallint)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function frames_clear    ( ax : Smallint)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';

 function map_axes    ( n_axes : Smallint; arripaxes : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function set_move_speed      ( speed : Double)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function set_move_accel      ( accel : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function set_arc_division    ( dArc : Double)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function all_done     : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function move_2      ( X : Double;  Y : Double)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function move_3      ( X : Double;  Y : Double;  z : Double)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function move_4      ( X : Double;  Y : Double;  z : Double;  w : Double)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function move_n      (arrdpos : pointer)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function move_2ax    ( ax1 : Smallint;  ax2 : Smallint;  X : Double;  Y : Double;  vel : Double;  acc : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function move_3ax    ( ax1 : Smallint;  ax2 : Smallint;  ax3 : Smallint;  X : Double;  Y : Double;  z : Double;  vel : Double;  acc : Smallint)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function move_4ax    ( ax1 : Smallint;  ax2 : Smallint;  ax3 : Smallint;  ax4 : Smallint;  X : Double;  Y : Double;  z : Double;  w : Double;  vel : Double;  acc : Smallint)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function move_nax    ( axLen : Smallint; arriaxes : pointer; arrdpos : pointer;  vel : Double;  acc : Smallint)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function move_2axgr      ( gr : Smallint;  ax1 : Smallint;  ax2 : Smallint;  X : Double;  Y : Double;  vel : Double;  acc : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function move_3axgr      ( gr : Smallint;  ax1 : Smallint;  ax2 : Smallint;  ax3 : Smallint;  X : Double;  Y : Double;  z : Double;  vel : Double;  acc : Smallint)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function move_4axgr      ( gr : Smallint;  ax1 : Smallint;  ax2 : Smallint;  ax3 : Smallint;  ax4 : Smallint;  X : Double;  Y : Double;  z : Double;  w : Double;  vel : Double;  acc : Smallint)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';

 function smove_2     ( X : Double;  Y : Double)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function smove_3     ( X : Double;  Y : Double;  z : Double)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function smove_4     ( X : Double;  Y : Double;  z : Double;  w : Double)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function smove_n     (arrdpos : pointer)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function smove_2ax       ( ax1 : Smallint;  ax2 : Smallint;  X : Double;  Y : Double;  vel : Double;  acc : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function smove_3ax       ( ax1 : Smallint;  ax2 : Smallint;  ax3 : Smallint;  X : Double;  Y : Double;  z : Double;  vel : Double;  acc : Smallint)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function smove_4ax       ( ax1 : Smallint;  ax2 : Smallint;  ax3 : Smallint;  ax4 : Smallint;  X : Double;  Y : Double;  z : Double;  w : Double;  vel : Double;  acc : Smallint)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function smove_nax       ( axLen : Smallint; arriaxes : pointer; arrdpos : pointer;  vel : Double;  acc : Smallint)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function smove_2axgr     ( gr : Smallint;  ax1 : Smallint;  ax2 : Smallint;  X : Double;  Y : Double;  vel : Double;  acc : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function smove_3axgr     ( gr : Smallint;  ax1 : Smallint;  ax2 : Smallint;  ax3 : Smallint;  X : Double;  Y : Double;  z : Double;  vel : Double;  acc : Smallint)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function smove_4axgr     ( gr : Smallint;  ax1 : Smallint;  ax2 : Smallint;  ax3 : Smallint;  ax4 : Smallint;  X : Double;  Y : Double;  z : Double;  w : Double;  vel : Double;  acc : Smallint)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';

 function arc_2       ( xCenter : Double;  yCenter : Double;  angle : Double)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function arc_2ax     ( ax1 : Smallint;  ax2 : Smallint;  x_center : Double;  y_center : Double;  angle : Double;  vel : Double;  acc : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function spl_line_move2      (arrdpntxy : pointer;  vel : Double;  acc : Smallint)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function spl_line_move3      (arrdpntxyz : pointer;  vel : Double;  acc : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function spl_line_move2ax    ( ax1 : Smallint;  ax2 : Smallint; arrdpntxy : pointer;  vel : Double;  acc : Smallint)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function spl_line_move3ax    ( ax1 : Smallint;  ax2 : Smallint;  ax3 : Smallint; arrdpntxyz : pointer;  vel : Double;  acc : Smallint)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';

 function spl_arc_move2       ( xc : Double;  yc : Double; arrdpntxy : pointer;  vel : Double;  acc : Smallint;  dir : Smallint)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function spl_arc_move3       ( xc : Double;  yc : Double; arrdpntxyz : pointer;  vel : Double;  acc : Smallint;  dir : Smallint)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function spl_arc_move2ax     ( ax1 : Smallint;  ax2 : Smallint;  xc : Double;  yc : Double; arrdpntxy : pointer;  vel : Double;  acc : Smallint;  dir : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function spl_arc_move3ax     ( ax1 : Smallint;  ax2 : Smallint;  ax3 : Smallint;  xc : Double;  yc : Double; arrdpntxyz : pointer;  vel : Double;  acc : Smallint;  dir : Smallint)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';

 function spl_move    ( n : Smallint;  ax1 : Smallint;  ax2 : Smallint;  ax3 : Smallint; arrdpntx : pointer; arrdpnty : pointer; arrdpntz : pointer;  vel : Double;  acc : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function rect_move       ( ax1 : Smallint;  ax2 : Smallint; arrdpntxy : pointer;  vel : Double;  acc : Smallint)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function spl_auto_off    ( ax : Smallint;  mode : Smallint)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';

 function start_move      ( ax : Smallint;  pos : Double;  vel : Double;  acc : Smallint)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function move    ( ax : Smallint;  pos : Double;  vel : Double;  acc : Smallint)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function start_r_move    ( ax : Smallint;  pos : Double;  vel : Double;  acc : Smallint)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function r_move      ( ax : Smallint;  pos : Double;  vel : Double;  acc : Smallint)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function start_s_move    ( ax : Smallint;  pos : Double;  vel : Double;  acc : Smallint)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function s_move      ( ax : Smallint;  pos : Double;  vel : Double;  acc : Smallint)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function start_rs_move       ( ax : Smallint;  pos : Double;  vel : Double;  acc : Smallint)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function rs_move     ( ax : Smallint;  pos : Double;  vel : Double;  acc : Smallint)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function start_t_move    ( ax : Smallint;  pos : Double;  vel : Double;  acc : Smallint;  dcc : Smallint)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function t_move      ( ax : Smallint;  pos : Double;  vel : Double;  acc : Smallint;  dcc : Smallint)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function start_ts_move       ( ax : Smallint;  pos : Double;  vel : Double;  acc : Smallint;  dcc : Smallint)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function ts_move     ( ax : Smallint;  pos : Double;  vel : Double;  acc : Smallint;  dcc : Smallint)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function start_tr_move       ( ax : Smallint;  pos : Double;  vel : Double;  acc : Smallint;  dcc : Smallint)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function tr_move     ( ax : Smallint;  pos : Double;  vel : Double;  acc : Smallint;  dcc : Smallint)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function start_trs_move      ( ax : Smallint;  pos : Double;  vel : Double;  acc : Smallint;  dcc : Smallint)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function trs_move    ( ax : Smallint;  pos : Double;  vel : Double;  acc : Smallint;  dcc : Smallint)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function start_t_move_all    ( axLen : Smallint; arriax : pointer; arrdPos : pointer; arrdVel : pointer; arriAcc : pointer; arriDcc : pointer)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function t_move_all      ( axLen : Smallint; arriax : pointer; arrdPos : pointer; arrdVel : pointer; arriAcc : pointer; arriDcc : pointer)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function start_ts_move_all   ( axLen : Smallint; arriax : pointer; arrdPos : pointer; arrdVel : pointer; arriAcc : pointer; arriDcc : pointer)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function ts_move_all     ( axLen : Smallint; arriax : pointer; arrdPos : pointer; arrdVel : pointer; arriAcc : pointer; arriDcc : pointer)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';


 function start_move_all      ( axLen : Smallint; arriax : pointer; arrdPos : pointer; arrdVel : pointer; arriAcc : pointer)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function move_all        ( axLen : Smallint; arriax : pointer; arrdPos : pointer; arrdVel : pointer; arriAcc : pointer)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function start_s_move_all    ( axLen : Smallint; arriax : pointer; arrdPos : pointer; arrdVel : pointer; arriAcc : pointer)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function s_move_all      ( axLen : Smallint; arriax : pointer; arrdPos : pointer; arrdVel : pointer; arriAcc : pointer)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function wait_for_done       ( ax : Smallint)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function wait_for_all    ( axNum : Smallint; arriaxes : pointer)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function v_move      ( ax : Smallint;  vel : Double;  accel : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';

 function set_positive_sw_limit       ( ax : Smallint;  f : Double;  n : Smallint)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function get_positive_sw_limit       ( ax : Smallint; pdPos : pointer; piSt : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function set_negative_sw_limit       ( ax : Smallint;  f : Double;  n : Smallint)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function get_negative_sw_limit       ( ax : Smallint; pdPos : pointer; piSt : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
            
 function get_accel_limit     ( ax : Smallint; piSt : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function set_accel_limit     ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function fset_accel_limit    ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function get_vel_limit       ( ax : Smallint; pdVel : pointer)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function set_vel_limit       ( ax : Smallint;  f : Double)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';

 function set_positive_limit      ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function get_positive_limit      ( ax : Smallint; piSt : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function set_negative_limit      ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function get_negative_limit      ( ax : Smallint; piSt : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function set_in_position     ( ax : Smallint;  f : Double)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function get_in_position     ( ax : Smallint; pdVal : pointer)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function set_error_limit     ( ax : Smallint;  f : Double;  n : Smallint)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function get_error_limit     ( ax : Smallint; pdErrL : pointer; piSt : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function set_positive_level      ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function get_positive_level      ( ax : Smallint; piSt : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function set_negative_level      ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function get_negative_level      ( ax : Smallint; piSt : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
            
 function home_switch     ( ax : Smallint)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function pos_switch      ( ax : Smallint)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function neg_switch      ( ax : Smallint)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function amp_fault_switch    ( ax : Smallint)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function set_io      ( bd : Smallint;  l : Longint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function get_io      ( bd : Smallint; plBits : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function get_out_io      ( bd : Smallint; plBits : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function set_bit     ( i : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function reset_bit       ( i : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
           
 function get_gain    ( ax : Smallint; arrlcoeff : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function fget_gain       ( ax : Smallint; arrlcoeff : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function set_gain    ( ax : Smallint; arrlcoeff : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function fset_gain       ( ax : Smallint; arrlcoeff : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function get_v_gain      ( ax : Smallint; arrlcoeff : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function fget_v_gain     ( ax : Smallint; arrlcoeff : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function set_v_gain      ( ax : Smallint; arrlcoeff : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function fset_v_gain     ( ax : Smallint; arrlcoeff : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';

 function fset_p_integration      ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function fset_v_integration      ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function fget_p_integration      ( ax : Smallint; pimode : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function fget_v_integration      ( ax : Smallint; pimode : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function set_p_integration       ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function get_p_integration       ( ax : Smallint; pimode : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function set_v_integration       ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function get_v_integration       ( ax : Smallint; pimode : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';

 function set_amp_enable      ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function get_amp_enable      ( ax : Smallint; piVal : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function amp_fault_reset     ( ax : Smallint)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function amp_fault_set       ( ax : Smallint)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function set_amp_enable_level    ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function get_amp_enable_level    ( ax : Smallint; piSt : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function fget_amp_enable_level       ( ax : Smallint; piSt : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function set_control     ( ax : Smallint;  iCon : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function get_control     ( ax : Smallint; piCon : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function set_electric_gear       ( ax : Smallint;  f : Double)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function get_electric_gear       ( ax : Smallint; pdVal : pointer)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function fget_electric_gear      ( ax : Smallint; pdVal : pointer)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function set_step_mode       ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function get_step_mode       ( ax : Smallint; pimode : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function set_sync_map_axes       ( max : Smallint;  sax : Smallint)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function set_sync_control    ( state : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function get_sync_control    (pistate : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function set_sync_gain       ( gain : Smallint)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function fset_sync_gain      ( gain : Smallint)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function get_sync_gain       (pigain : pointer)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function fget_sync_gain      (pigain : pointer)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function compensation_pos    ( axLen : Smallint; arriax : pointer; arrdCdist : pointer; arriAcc : pointer)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';

 function set_pulse_ratio     ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function fset_pulse_ratio    ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function get_pulse_ratio     ( ax : Smallint; piRatio : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';

 function set_stop    ( ax : Smallint)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function set_stop_rate       ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function get_stop_rate       ( ax : Smallint; pirate : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function set_e_stop      ( ax : Smallint)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function set_e_stop_rate     ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function get_e_stop_rate     ( ax : Smallint; pirate : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';

 function set_home    ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function get_home    ( ax : Smallint; piSt : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function set_home_level      ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function get_home_level      ( ax : Smallint; piSt : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function set_index_required      ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function get_index_required      ( ax : Smallint; piVal : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';

 function io_interrupt_enable (bdnum : Smallint;  state : Smallint) : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function io_interrupt_on_stop  (ax : Smallint;  state : Smallint) : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function io_interrupt_on_e_stop (ax : Smallint;  state : Smallint) : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function io_interrupt_pcirq ( bdnum : Smallint;  state : Smallint) : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function io_interrupt_pcirq_eoi  ( bdnum : Smallint) : Smallint  ;stdcall;   external    'MMCWFP39.DLL';

 function set_amp_fault       ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function get_amp_fault       ( ax : Smallint; piVal : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function set_amp_fault_level     ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function get_amp_fault_level     ( ax : Smallint; piSt : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function fget_amp_fault_level    ( ax : Smallint; piSt : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function set_amp_reset_level     ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function get_amp_reset_level     ( ax : Smallint; piSt : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function fget_amp_reset_level    ( ax : Smallint; piSt : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';

 function get_analog      ( channel : Smallint; piVal : pointer)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function set_dac_output      ( channel : Smallint;  volt : Longint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function get_dac_output      ( channel : Smallint; plvolt : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';

 function fset_stepper    ( ax : Smallint)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function fset_servo      ( ax : Smallint)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function fset_feedback       ( ax : Smallint;  iDev : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function fset_closed_loop    ( ax : Smallint;  iloop : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function fset_unipolar   ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';

 function fset_micro_stepper      ( ax : Smallint)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function fget_micro_stepper      ( ax : Smallint)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';

 function fset_amp_fault      ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';

 function fset_control    ( ax : Smallint;  iCon : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function fset_electric_gear      ( ax : Smallint;  f : Double)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function fset_step_mode      ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';

 function fset_home       ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function fset_index_required     ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';

 function fio_interrupt_enable (bdnum : Smallint;  state : Smallint) : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function fio_interrupt_on_stop  (ax : Smallint;  state : Smallint) : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function fio_interrupt_on_e_stop (ax : Smallint;  state : Smallint) : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function fio_interrupt_pcirq   ( bdnum : Smallint;  state : Smallint) : Smallint  ;stdcall;   external    'MMCWFP39.DLL';

 function fset_positive_sw_limit      ( ax : Smallint;  f : Double;  n : Smallint)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function fset_negative_sw_limit      ( ax : Smallint;  f : Double;  n : Smallint)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function fset_positive_limit     ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function fset_negative_limit     ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function fset_in_position    ( ax : Smallint;  f : Double)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function fset_error_limit    ( ax : Smallint;  f : Double;  n : Smallint)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';

 function fset_stop_rate      ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function fset_e_stop_rate    ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';

 function fget_stepper    ( ax : Smallint)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function fget_feedback       ( ax : Smallint; piDev : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function fget_closed_loop    ( ax : Smallint; piloop : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function fget_unipolar   ( ax : Smallint)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';

 function fget_amp_fault      ( ax : Smallint; piVal : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';

 function fget_control    ( ax : Smallint; piCon : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function fget_step_mode      ( ax : Smallint; pimode : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';

 function fget_home       ( ax : Smallint; piSt : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function fget_index_required     ( ax : Smallint; piVal : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';

 function fget_positive_sw_limit      ( ax : Smallint; pdPos : pointer; piSt : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function fget_negative_sw_limit      ( ax : Smallint; pdPos : pointer; piSt : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function fget_positive_limit     ( ax : Smallint; piSt : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function fget_negative_limit     ( ax : Smallint; piSt : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function fget_in_position    ( ax : Smallint; pdVal : pointer)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function fget_error_limit    ( ax : Smallint; pdErrL : pointer; piSt : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';

 function fget_stop_rate      ( ax : Smallint; pirate : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function fget_e_stop_rate    ( ax : Smallint; pirate : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';

 function set_interpolation       ( nLen : Smallint; arriax : pointer; arrlidelt_s : pointer;  flag : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function frames_interpolation    ( nax : Smallint)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';

 function v_move_stop     ( ax : Smallint   ): Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function set_inposition_level    ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function fset_inposition_level       ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function get_inposition_level    ( ax : Smallint; piSt : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function fget_inposition_level       ( ax : Smallint; piSt : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';

 function controller_idle     ( ax : Smallint)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function controller_run      ( ax : Smallint)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';

 function arm_latch       ( bdnum : Smallint;  state : Smallint)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function latch_status    ( bdnum : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function get_latched_position    ( ax : Smallint; pdPos : pointer)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function latch       ( bdnum : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';

 function set_timer (bdnum : Smallint; t : Longint)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function get_timer (bdnum : Smallint; pt : pointer)    : Smallint     ;stdcall;   external    'MMCWFP39.DLL';

 function fget_home_level     ( ax : Smallint; piSt : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function fget_positive_level     ( ax : Smallint; piSt : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function fget_negative_level     ( ax : Smallint; piSt : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';

 function fset_amp_fault_level    ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function fset_amp_reset_level    ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function fset_amp_enable_level       ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function fget_pulse_ratio    ( ax : Smallint; piRatio : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function fset_home_level     ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function fget_accel_limit    ( ax : Smallint; piSt : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function fget_vel_limit      ( ax : Smallint; pdVel : pointer)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function fset_vel_limit      ( ax : Smallint;  f : Double)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function fset_positive_level     ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function fset_negative_level     ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';

 function set_io_mode     ( nBdNum : Smallint;  IoMode : Smallint)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function fset_io_mode    ( nBdNum : Smallint;  IoMode : Smallint)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function get_io_mode     ( nBdNum : Smallint; piIoMode : pointer)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function fget_io_mode    ( nBdNum : Smallint; piIoMode : pointer)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function get_io_num      ( nax : Smallint; pival : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';

 function set_analog_offset       ( naxis : Smallint;  nvoltage : Smallint)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function fset_analog_offset      ( naxis : Smallint;  nvoltage : Smallint)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function get_analog_offset       ( naxis : Smallint; pivoltage : pointer)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function fget_analog_offset      ( naxis : Smallint; pivoltage : pointer)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';

 function fget_inposition_required    ( ax : Smallint; piVal : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function get_inposition_required     ( ax : Smallint; piVal : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function fset_inposition_required    ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function set_inposition_required     ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';

 function fget_coordinate_direction       ( ax : Smallint; piVal : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function get_coordinate_direction    ( ax : Smallint; piVal : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function fset_coordinate_direction       ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function set_coordinate_direction    ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';

 function fget_encoder_direction      ( ax : Smallint; piVal : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function get_encoder_direction       ( ax : Smallint; piVal : pointer)   : Smallint  ;stdcall;   external    'MMCWFP39.DLL';
 function fset_encoder_direction      ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';
 function set_encoder_direction       ( ax : Smallint;  n : Smallint)   : Smallint    ;stdcall;   external    'MMCWFP39.DLL';

 function get_axis_runstop    ( bdnum : Smallint; piVal : pointer)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function set_axis_runstop    ( bdnum : Smallint;  n : Smallint)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';

 function set_endless_rotationax      ( ax : Smallint;  mode : Smallint;  resolution : Smallint)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function fset_endless_rotationax     ( ax : Smallint;  mode : Smallint;  resolution : Smallint)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function get_endless_rotationax      ( ax : Smallint;  pimode : pointer)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function fget_endless_rotationax     ( ax : Smallint;  pimode : pointer)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';

 function set_endless_linearax    ( ax : Smallint;  mode : Smallint;  resolution : Smallint)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function fset_endless_linearax       ( ax : Smallint;  mode : Smallint;  resolution : Smallint)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function get_endless_linearax    ( ax : Smallint;   pimode : pointer)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function fget_endless_linearax       ( ax : Smallint;   pimode : pointer)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function set_endless_range       ( ax : Smallint;  range : Double)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function fset_endless_range      ( ax : Smallint;  range : Double)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function get_endless_range       ( ax : Smallint; pdrange : pointer)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';
 function fget_endless_range      ( ax : Smallint; pdrange : pointer)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';

 function set_linear_all_stop_flag    ( ax : Smallint;  flag : Smallint)   : Smallint     ;stdcall;   external    'MMCWFP39.DLL';
 function get_linear_all_stop_flag    ( ax : Smallint; piflag : pointer)   : Smallint   ;stdcall;   external    'MMCWFP39.DLL';

 Function get_command_rpm  ( ax : Smallint; irpm : pointer) : Smallint       ;stdcall;   external    'MMCWFP39.DLL';
 Function get_encoder_rpm ( ax : Smallint; irpm : pointer) : Smallint          ;stdcall;   external    'MMCWFP39.DLL';

 Function set_amp_resolution   ( ax : Smallint;  res : Smallint) : Smallint      ;stdcall;   external    'MMCWFP39.DLL';
 Function fset_amp_resolution  ( ax : Smallint;  res : Smallint) : Smallint       ;stdcall;   external    'MMCWFP39.DLL';
 Function get_amp_resolution  ( ax : Smallint; ires : pointer) : Smallint           ;stdcall;   external    'MMCWFP39.DLL';
 Function fget_amp_resolution  ( ax : Smallint; ires : pointer) : Smallint            ;stdcall;   external    'MMCWFP39.DLL';

 Function set_collision_prevent_flag ( bd_num : Smallint;  mode : Smallint) : Smallint        ;stdcall;   external    'MMCWFP39.DLL';
 Function get_collision_prevent_flag  ( bd_num : Smallint; imode : pointer) : Smallint               ;stdcall;   external    'MMCWFP39.DLL';
 Function set_collision_prevent  ( max : Smallint;  sax : Smallint;  add_sub : Smallint;  non_equal : Smallint;  c_pos : Double) : Smallint       ;stdcall;   external    'MMCWFP39.DLL';

 Function set_abs_encoder_type  ( ax : Smallint;  itype : Smallint) : Smallint      ;stdcall;   external    'MMCWFP39.DLL';
 Function get_abs_encoder_type  ( ax : Smallint; itype : pointer) : Smallint             ;stdcall;   external    'MMCWFP39.DLL';

 Function set_fast_read_encoder ( ax : Smallint;  status:Smallint):Smallint       ;stdcall;   external    'MMCWFP39.DLL';
 Function get_fast_read_encoder  ( ax : Smallint; istatus : pointer) : Smallint           ;stdcall;   external    'MMCWFP39.DLL';

 Function set_control_timer  ( bn : Smallint;  itime : Smallint) : Smallint           ;stdcall;   external    'MMCWFP39.DLL';
 Function fset_control_timer ( bn : Smallint;  itime : Smallint) : Smallint          ;stdcall;   external    'MMCWFP39.DLL';
 Function get_control_timer  ( bn : Smallint; itime : pointer) : Smallint                 ;stdcall;   external    'MMCWFP39.DLL';
 Function fget_control_timer  ( bn : Smallint; itime : pointer) : Smallint                ;stdcall;   external    'MMCWFP39.DLL';
 Function spl_move_data  ( spl_num : Smallint;  nlen : Smallint;  ax1 : Smallint;  ax2 : Smallint;  ax3 : Smallint; dpnt1 : pointer; dpnt2 : pointer; dpnt3 : pointer;  vel : Double;  acc : Smallint) : Smallint      ;stdcall;   external    'MMCWFP39.DLL';
 Function spl_movex  ( spl_num : Smallint;  ax1 : Smallint;  ax2 : Smallint;  ax3 : Smallint) : Smallint           ;stdcall;   external    'MMCWFP39.DLL';
 Function spl_arc_movenax  ( nlen : Smallint; arriax : pointer;  x_center : Double;  y_center : Double; dpnt : pointer;  vel : Double;  acc : Smallint;  cdir : Smallint) : Smallint      ;stdcall;   external    'MMCWFP39.DLL';
 Function spl_line_movenax ( nlen : Smallint; arriax : pointer; dpnt : pointer;  vel : Double;  acc : Smallint) : Smallint        ;stdcall;   external    'MMCWFP39.DLL';

 Function set_mmc_led_num ( bn : Smallint) : Smallint            ;stdcall;   external    'MMCWFP39.DLL';
 Function get_mmc_led_num ( bn : Smallint; iled_num : pointer) : Smallint      ;stdcall;   external    'MMCWFP39.DLL';
 Function get_fast_position  ( ax : Smallint; dpos : pointer) : Smallint         ;stdcall;   external    'MMCWFP39.DLL';

 Function set_encoder_ratioa  ( ax : Smallint;  ratioa : Smallint) : Smallint             ;stdcall;   external    'MMCWFP39.DLL';
 Function fset_encoder_ratioa  ( ax : Smallint;  ratioa : Smallint) : Smallint              ;stdcall;   external    'MMCWFP39.DLL';
 Function get_encoder_ratioa  ( ax : Smallint; iratioa : pointer) : Smallint                   ;stdcall;   external    'MMCWFP39.DLL';
 Function fget_encoder_ratioa  ( ax : Smallint; iratioa : pointer) : Smallint                   ;stdcall;   external    'MMCWFP39.DLL';

 Function set_encoder_ratiob ( ax : Smallint;  ratiob : Smallint) : Smallint               ;stdcall;   external    'MMCWFP39.DLL';
 Function fset_encoder_ratiob  ( ax : Smallint;  ratiob : Smallint) : Smallint             ;stdcall;   external    'MMCWFP39.DLL';
 Function get_encoder_ratiob  ( ax : Smallint; iratiob : pointer) : Smallint                    ;stdcall;   external    'MMCWFP39.DLL';
 Function fget_encoder_ratiob  ( ax : Smallint; iratiob : pointer) : Smallint                   ;stdcall;   external    'MMCWFP39.DLL';

 Function set_analog_limit  ( ax : Smallint;  voltage : Longint) : Smallint                   ;stdcall;   external    'MMCWFP39.DLL';
 Function fset_analog_limit  ( ax : Smallint;  voltage : Longint) : Smallint                  ;stdcall;   external    'MMCWFP39.DLL';
 Function get_analog_limit  ( ax : Smallint; lvoltage : pointer) : Smallint                         ;stdcall;   external    'MMCWFP39.DLL';
 Function fget_analog_limit ( ax : Smallint; lvoltage : pointer) : Smallint                         ;stdcall;   external    'MMCWFP39.DLL';

 Function set_position_lowpass_filter  ( ax : Smallint;  hz : Double) : Smallint          ;stdcall;   external    'MMCWFP39.DLL';
 Function fset_position_lowpass_filter  ( ax : Smallint;  hz : Double) : Smallint          ;stdcall;   external    'MMCWFP39.DLL';
 Function get_position_lowpass_filter ( ax : Smallint; dhz : pointer) : Smallint                  ;stdcall;   external    'MMCWFP39.DLL';
 Function fget_position_lowpass_filter ( ax : Smallint; dhz : pointer) : Smallint                 ;stdcall;   external    'MMCWFP39.DLL';

 Function set_velocity_lowpass_filter  ( ax : Smallint;  hz : Double) : Smallint            ;stdcall;   external    'MMCWFP39.DLL';
 Function fset_velocity_lowpass_filter  ( ax : Smallint;  hz : Double) : Smallint           ;stdcall;   external    'MMCWFP39.DLL';
 Function get_velocity_lowpass_filter  ( ax : Smallint; dhz : pointer) : Smallint                 ;stdcall;   external    'MMCWFP39.DLL';
 Function fget_velocity_lowpass_filter  ( ax : Smallint; dhz : pointer) : Smallint                ;stdcall;   external    'MMCWFP39.DLL';

 Function set_position_notch_filter ( ax : Smallint;  hz : Double) : Smallint              ;stdcall;   external    'MMCWFP39.DLL';
 Function fset_position_notch_filter  ( ax : Smallint;  hz : Double) : Smallint            ;stdcall;   external    'MMCWFP39.DLL';
 Function get_position_notch_filter ( ax : Smallint; dhz : pointer) : Smallint                     ;stdcall;   external    'MMCWFP39.DLL';
 Function fget_position_notch_filter ( ax : Smallint; dhz : pointer) : Smallint                    ;stdcall;   external    'MMCWFP39.DLL';

 Function set_velocity_notch_filter  ( ax : Smallint;  hz : Double) : Smallint             ;stdcall;   external    'MMCWFP39.DLL';
 Function fset_velocity_notch_filter  ( ax : Smallint;  hz : Double) : Smallint            ;stdcall;   external    'MMCWFP39.DLL';
 Function get_velocity_notch_filter  ( ax : Smallint; dhz : pointer) : Smallint                    ;stdcall;   external    'MMCWFP39.DLL';
 Function fget_velocity_notch_filter  ( ax : Smallint; dhz : pointer) : Smallint                   ;stdcall;   external    'MMCWFP39.DLL';

 Function set_mmc_parameter_init ( ax : Smallint) : Smallint                                      ;stdcall;   external    'MMCWFP39.DLL';
 Function axis_all_status(ax:Smallint ; arriState: pointer ; lState:pointer; arrdState:pointer) : Smallint     ;   stdcall;   external    'MMCWFP39.DLL';

 Function set_sensor_auto_off(ax:Smallint; off:Smallint) : Smallint;      stdcall;   external    'MMCWFP39.DLL';
 Function fset_sensor_auto_off(ax:Smallint; off:Smallint) : Smallint;       stdcall;   external    'MMCWFP39.DLL';
 Function get_sensor_auto_off(ax:Smallint; ioff:pointer) : Smallint;     stdcall;   external    'MMCWFP39.DLL';
 Function fget_sensor_auto_off(ax:Smallint; ioff:pointer) : Smallint;      stdcall;   external    'MMCWFP39.DLL';
 Function get_spline_move_num(bdNum:Smallint; inum:pointer) : Smallint;      stdcall;   external    'MMCWFP39.DLL';

 Function set_servo_linear_flag(ax:Smallint; flag:Smallint) : Smallint;      stdcall;   external    'MMCWFP39.DLL';
 Function fset_servo_linear_flag(ax:Smallint; flag:Smallint) : Smallint;      stdcall;   external    'MMCWFP39.DLL';
 Function get_servo_linear_flag(ax:Smallint; iflag:pointer) : Smallint;      stdcall;   external    'MMCWFP39.DLL';
 Function fget_servo_linear_flag(ax:Smallint; iflag:pointer) : Smallint;     stdcall;   external    'MMCWFP39.DLL';

 Function spl_auto_line_move2ax(ax1:Smallint;ax2:Smallint;arrdPos:pointer; vel:Double; acc:Smallint; offFlag:Smallint) : Smallint;        stdcall;   external    'MMCWFP39.DLL';
 Function spl_auto_line_move3ax(ax1:Smallint;ax2:Smallint;ax3:Smallint;arrdPos:pointer; vel:Double; acc:Smallint; offFlag:Smallint) : Smallint;        stdcall;   external    'MMCWFP39.DLL';
 Function spl_auto_arc_move2ax(ax1:Smallint;ax2:Smallint; cx:Double; cy:Double; arrdPos: pointer; vel:Double; acc:Smallint; dir:Smallint; offFlag:Smallint) : Smallint;      stdcall;   external    'MMCWFP39.DLL';
 Function spl_auto_arc_move3ax(ax1:Smallint;ax2:Smallint;ax3:Smallint; cx:Double; cy:Double; arrdPos: pointer; vel:Double; acc:Smallint; dir:Smallint; offFlag:Smallint) : Smallint;      stdcall;   external    'MMCWFP39.DLL';
 Function spl_auto_arc_movenax(axNum:Smallint; arriax:pointer; cx:Double; cy:Double; arrdPos: pointer; vel:Double; acc:Smallint; dir:Smallint; offFlag:Smallint) : Smallint;       stdcall;   external    'MMCWFP39.DLL';
 Function spl_auto_line_movenax(axNum:Smallint; arriax:pointer; arrdPos:Double; vel:Double; acc:Smallint; offFlag:Smallint) : Smallint;         stdcall;   external    'MMCWFP39.DLL';

 Function set_analog_direction ( ax : Smallint;  dir : Smallint) : Smallint ;     stdcall;   external    'MMCWFP39.DLL';
 Function fset_analog_direction  ( ax : Smallint;  dir : Smallint) : Smallint ;     stdcall;   external    'MMCWFP39.DLL';
 Function get_analog_direction  ( ax : Smallint; pidir : pointer) : Smallint ;     stdcall;   external    'MMCWFP39.DLL';
 Function fget_analog_direction  ( ax : Smallint; pidir : pointer) : Smallint ;     stdcall;   external    'MMCWFP39.DLL';


 Function position_compare  ( index_sel : Smallint;  index_num : Smallint;  bitNo : Smallint;  ax : Smallint;  latch : Smallint;  nfunction : Smallint;  pos : Double ;  time : Longint) : Smallint ;     stdcall;   external    'MMCWFP39.DLL';
 Function position_compare_enable  ( bn : Smallint;  flag : Smallint) : Smallint ;     stdcall;   external    'MMCWFP39.DLL';
 Function position_compare_index_clear  ( bn : Smallint;  index : Smallint): Smallint ;     stdcall;   external    'MMCWFP39.DLL';

 Function spl_arc_deg_move2  ( x_center : Double;  y_center : Double; pdpoint : pointer;  vel : Double;  acc : Smallint;  cdir : Smallint) : Smallint ;     stdcall;   external    'MMCWFP39.DLL';
 Function spl_arc_deg_move3  ( x_center : Double;  y_center : Double; pdpoint : pointer;  vel : Double;  acc : Smallint;  cdir : Smallint) : Smallint ;     stdcall;   external    'MMCWFP39.DLL';

 Function spl_arc_deg_move2ax  ( ax1 : Smallint;  ax2 : Smallint; x_center : Double;  y_center : Double; pdpoint : pointer;  vel : Double;  acc : Smallint;  cdir : Smallint) : Smallint ;     stdcall;   external    'MMCWFP39.DLL';
 Function spl_arc_deg_move3ax  ( ax1 : Smallint;  ax2 : Smallint;  ax3 : Smallint; x_center : Double;  y_center : Double; pdpoint : pointer;  vel : Double;  acc : Smallint;  cdir : Smallint) : Smallint ;     stdcall;   external    'MMCWFP39.DLL';
 Function spl_arc_deg_movenax  ( nlen : Smallint; piax : pointer; x_center : Double;  y_center : Double; pdpoint : pointer;  vel : Double;  acc : Smallint;  cdir : Smallint) : Smallint ;     stdcall;   external    'MMCWFP39.DLL';

 Function spl_auto_arc_deg_move2ax  ( ax1 : Smallint;  ax2 : Smallint; x_center : Double;  y_center : Double; pdpoint : pointer;  vel : Double;  acc : Smallint;  cdir : Smallint;  auto_flag : Smallint) : Smallint ;     stdcall;   external    'MMCWFP39.DLL';
 Function spl_auto_arc_deg_move3ax  ( ax1 : Smallint;  ax2 : Smallint;  ax3 : Smallint; x_center : Double;  y_center : Double; pdpoint : pointer;  vel : Double;  acc : Smallint;  cdir : Smallint;  auto_flag : Smallint) : Smallint ;     stdcall;   external    'MMCWFP39.DLL';
 Function spl_auto_arc_deg_movenax  ( nlen : Smallint; piax : pointer; x_center : Double;  y_center : Double; pdpoint : pointer;  vel : Double;  acc : Smallint;  cdir : Smallint;  auto_flag : Smallint) : Smallint ;     stdcall;   external    'MMCWFP39.DLL';

 Function arc_3  (x_center : Double;  y_center : Double;  angle : Double; pos : Double) : Smallint ;     stdcall;   external    'MMCWFP39.DLL';
 Function arc_3ax  (ax1 : Smallint;  ax2 : Smallint;  ax3 : Smallint; x_center : Double;  y_center : Double;  angle : Double; pdpos : pointer;  vel : Double;  acc : Smallint) : Smallint ;     stdcall;   external    'MMCWFP39.DLL';

 Function set_encoder_filter_num  (ax : Smallint;  fn : Smallint): Smallint ;     stdcall;   external    'MMCWFP39.DLL';
 Function fset_encoder_filter_num  (ax : Smallint; fn : Smallint) : Smallint ;     stdcall;   external    'MMCWFP39.DLL';
 Function get_encoder_filter_num  (ax : Smallint; pifn : pointer) : Smallint ;     stdcall;   external    'MMCWFP39.DLL';
 Function fget_encoder_filter_num  (ax : Smallint; pifn : pointer) : Smallint ;     stdcall;   external    'MMCWFP39.DLL';
 Function get_mmc_init_chkx  () : Smallint ;     stdcall;   external    'MMCWFP39.DLL';
 Function set_mmc_init_chkx  (bn : Smallint;  val : Smallint) : Smallint ;     stdcall;   external    'MMCWFP39.DLL';

 Function spl_line_move1  (arrdPos:pointer; vel:Double; acc:Smallint): Smallint ;     stdcall;   external    'MMCWFP39.DLL';
 Function spl_line_move1ax (ax:Smallint;arrdPos:pointer; vel:Double; acc:Smallint): Smallint ;     stdcall;   external    'MMCWFP39.DLL';
 Function spl_auto_line_move1ax  (ax:Smallint;arrdPos:pointer; vel:Double; acc:Smallint; offFlag:Smallint): Smallint ;     stdcall;   external    'MMCWFP39.DLL';

 Function position_compare_interval ( dir:Smallint;  ax:Smallint;  bitNo:Smallint;  startpos:Double; limitpos:Double;  interval: Longint; time : Longint): Smallint ;     stdcall;   external    'MMCWFP39.DLL';
 Function set_control_timer_ax ( ax:Smallint;  time :Double): Smallint ;     stdcall;   external    'MMCWFP39.DLL';
 Function fset_control_timer_ax ( ax:Smallint;  time :Double): Smallint ;     stdcall;   external    'MMCWFP39.DLL';
 Function get_control_timer_ax ( ax:Smallint;  pdtime:pointer): Smallint ;     stdcall;   external    'MMCWFP39.DLL';
 Function fget_control_timer_ax ( ax:Smallint;  pdtime:pointer): Smallint ;     stdcall;   external    'MMCWFP39.DLL';

 Function position_io_onoff ( pos_num:Smallint;  bitNo:Smallint;  ax:Smallint;  pos :Double;  encflag:Smallint): Smallint ;     stdcall;   external    'MMCWFP39.DLL';
 Function position_io_allclear ( ax:Smallint): Smallint ;     stdcall;   external    'MMCWFP39.DLL';
 Function position_io_clear ( ax:Smallint;  pos_num:Smallint): Smallint ;     stdcall;   external    'MMCWFP39.DLL';

 Function set_friction_gain ( axis:Smallint;  gain : Longint): Smallint ;     stdcall;   external    'MMCWFP39.DLL';
 Function fset_friction_gain ( axis:Smallint;  gain : Longint): Smallint ;     stdcall;   external    'MMCWFP39.DLL';
 Function get_friction_gain ( axis:Smallint;  plgain:pointer): Smallint ;     stdcall;   external    'MMCWFP39.DLL';
 Function fget_friction_gain ( axis:Smallint;  plgain:pointer): Smallint ;     stdcall;   external    'MMCWFP39.DLL';
 Function set_friction_range ( axno:Smallint;  range :Double): Smallint ;     stdcall;   external    'MMCWFP39.DLL';
 Function fset_friction_range ( axno:Smallint;  range :Double): Smallint ;     stdcall;   external    'MMCWFP39.DLL';
 Function get_friction_range ( axno:Smallint;  pdrange:pointer): Smallint ;     stdcall;   external    'MMCWFP39.DLL';
 Function fget_friction_range ( axno:Smallint;  pdrange:pointer): Smallint ;     stdcall;   external    'MMCWFP39.DLL';

 Function set_system_io( ax:Smallint; onoff:Smallint): Smallint ;     stdcall;   external    'MMCWFP39.DLL';
 Function get_system_io( ax:Smallint; pionoff:pointer): Smallint ;     stdcall;   external    'MMCWFP39.DLL';
 Function set_amp_resolution32( ax:Smallint; resolution:Longint): Smallint ;     stdcall;   external    'MMCWFP39.DLL';
 Function fset_amp_resolution32( ax:Smallint; resolution:Longint): Smallint ;     stdcall;   external    'MMCWFP39.DLL';
 Function get_amp_resolution32( ax:Smallint; plresolution:pointer): Smallint ;     stdcall;   external    'MMCWFP39.DLL';
 Function fget_amp_resolution32( ax::Smallint; plresolution:pointer): Smallint ;     stdcall;   external    'MMCWFP39.DLL';

 Function set_collision_prevent_ax( ax:Smallint; enable:Smallint;  slave_ax:Smallint;  add_sub:Smallint;  non_equal:Smallint;  c_pos:Double;  mtype:Smallint): Smallint ;     stdcall;   external    'MMCWFP39.DLL';
 Function get_collision_prevent_ax( ax:Smallint; pienable:pointer): Smallint ;     stdcall;   external    'MMCWFP39.DLL';
 Function set_sync_control_ax( ax:Smallint;  enable:Smallint;  master_ax:Smallint;  gain: Longint): Smallint ;     stdcall;   external    'MMCWFP39.DLL';
 Function get_sync_control_ax( ax:Smallint; pienable:pointer; pimaster_ax:pointer; plgain:pointer): Smallint ;     stdcall;   external    'MMCWFP39.DLL';

 Function set_pause_control( bn:Smallint; enable:Smallint; io_bit: Longint): Smallint ;     stdcall;   external    'MMCWFP39.DLL';

implementation

end.

