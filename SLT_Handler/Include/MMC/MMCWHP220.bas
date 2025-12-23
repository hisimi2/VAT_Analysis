Attribute VB_Name = "modMMCWin"


Global Const ST_NONE = &H0
Global Const ST_HOME_SWITCH = &H1
Global Const ST_POS_LIMIT = &H2
Global Const ST_NEG_LIMIT = &H4
Global Const ST_AMP_FAULT = &H8
Global Const ST_A_LIMIT = &H10
Global Const ST_V_LIMIT = &H20
Global Const ST_X_NEG_LIMIT = &H40
Global Const ST_X_POS_LIMIT = &H80
Global Const ST_ERROR_LIMIT = &H100
Global Const ST_PC_COMMAND = &H200
Global Const ST_OUT_OF_FRAMES = &H400
Global Const ST_AMP_POWER_ONOFF = &H800
Global Const ST_ABS_COMM_ERROR = &H1000
Global Const ST_INPOSITION_STATUS = &H2000
Global Const ST_RUN_STOP_COMMAND = &H4000
Global Const ST_COLLISION_STATE = &H8000

Global Const MAX_BD_NUM = 8
Global Const BD_AXIS_NUM = 8
Global Const GAIN_NUMBER = 5


Declare Function get_mmc_error Lib "MMCWHP220.dll" () As Integer
Declare Function get_version Lib "MMCWHP220.dll" () As Long
Declare Function get_axis_num Lib "MMCWHP220.dll" () As Integer
Declare Function get_bd_num Lib "MMCWHP220.dll" () As Integer
Declare Function get_velocity Lib "MMCWHP220.dll" (ByVal ax As Integer) As Integer

Declare Function mmc_initx Lib "MMCWHP220.dll" (ByVal nBdNum As Integer, lAddr As Long) As Integer
Declare Function version_chk Lib "MMCWHP220.dll" (ByVal bd As Integer, n As Integer) As Integer
Declare Function set_dpram_addr Lib "MMCWHP220.dll" (ByVal nBdNum As Integer, ByVal lAddr As Long) As Integer
Declare Function get_dpram_addr Lib "MMCWHP220.dll" (ByVal nBdNum As Integer, lAddr As Long) As Integer
Declare Function save_boot_frame Lib "MMCWHP220.dll" () As Integer

Declare Function mmc_axes Lib "MMCWHP220.dll" (ByVal bd As Integer, axes As Integer) As Integer
Declare Function mmc_all_axes Lib "MMCWHP220.dll" () As Integer
Declare Function get_stepper Lib "MMCWHP220.dll" (ByVal ax As Integer) As Integer
Declare Function set_stepper Lib "MMCWHP220.dll" (ByVal ax As Integer) As Integer
Declare Function set_servo Lib "MMCWHP220.dll" (ByVal ax As Integer) As Integer
Declare Function set_feedback Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer
Declare Function get_feedback Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer
Declare Function set_closed_loop Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer
Declare Function get_closed_loop Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer
Declare Function set_unipolar Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer
Declare Function get_unipolar Lib "MMCWHP220.dll" (ByVal ax As Integer) As Integer

Declare Function set_abs_encoder Lib "MMCWHP220.dll" (ByVal ax As Integer) As Integer
Declare Function set_micro_stepper Lib "MMCWHP220.dll" (ByVal ax As Integer) As Integer
Declare Function get_micro_stepper Lib "MMCWHP220.dll" (ByVal ax As Integer) As Integer

Declare Function mmcDelay Lib "MMCWHP220.dll" (ByVal l As Long) As Integer
Declare Function mmc_dwell Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal l As Long) As Integer
Declare Function mmc_io_trigger Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal bitNum As Integer, ByVal state As Integer) As Integer

Declare Function get_counter Lib "MMCWHP220.dll" (ByVal ax As Integer, f As Double) As Integer
Declare Function get_sync_position Lib "MMCWHP220.dll" (pos_m As Double, pos_s As Double) As Integer

Declare Function set_position Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal f As Double) As Integer
Declare Function get_position Lib "MMCWHP220.dll" (ByVal ax As Integer, f As Double) As Integer
Declare Function set_command Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal f As Double) As Integer
Declare Function get_command Lib "MMCWHP220.dll" (ByVal ax As Integer, f As Double) As Integer
Declare Function get_error Lib "MMCWHP220.dll" (ByVal ax As Integer, f As Double) As Integer
Declare Function get_com_velocity Lib "MMCWHP220.dll" (ByVal ax As Integer) As Integer
Declare Function get_act_velocity Lib "MMCWHP220.dll" (ByVal ax As Integer) As Integer

Declare Function in_sequence Lib "MMCWHP220.dll" (ByVal ax As Integer) As Integer
Declare Function in_motion Lib "MMCWHP220.dll" (ByVal ax As Integer) As Integer
Declare Function in_position Lib "MMCWHP220.dll" (ByVal ax As Integer) As Integer
Declare Function frames_left Lib "MMCWHP220.dll" (ByVal ax As Integer) As Integer
Declare Function motion_done Lib "MMCWHP220.dll" (ByVal ax As Integer) As Integer
Declare Function axis_done Lib "MMCWHP220.dll" (ByVal ax As Integer) As Integer
Declare Function axis_state Lib "MMCWHP220.dll" (ByVal ax As Integer) As Integer
Declare Function axis_source Lib "MMCWHP220.dll" (ByVal ax As Integer) As Integer
Declare Function axis_sourcex Lib "MMCWHP220.dll" (ByVal ax As Integer) As Long
Declare Function clear_status Lib "MMCWHP220.dll" (ByVal ax As Integer) As Integer
Declare Function frames_clear Lib "MMCWHP220.dll" (ByVal ax As Integer) As Integer

Declare Function map_axes Lib "MMCWHP220.dll" (ByVal n_axes As Integer, paxes As Integer) As Integer
Declare Function set_move_speed Lib "MMCWHP220.dll" (ByVal speed As Double) As Integer
Declare Function set_move_accel Lib "MMCWHP220.dll" (ByVal accel As Integer) As Integer
Declare Function set_arc_division Lib "MMCWHP220.dll" (ByVal dArc As Double) As Integer
Declare Function all_done Lib "MMCWHP220.dll" () As Integer
Declare Function move_2 Lib "MMCWHP220.dll" (ByVal X As Double, ByVal Y As Double) As Integer
Declare Function move_3 Lib "MMCWHP220.dll" (ByVal X As Double, ByVal Y As Double, ByVal z As Double) As Integer
Declare Function move_4 Lib "MMCWHP220.dll" (ByVal X As Double, ByVal Y As Double, ByVal z As Double, ByVal w As Double) As Integer
Declare Function move_n Lib "MMCWHP220.dll" (pos As Double) As Integer
Declare Function move_2ax Lib "MMCWHP220.dll" (ByVal ax1 As Integer, ByVal ax2 As Integer, ByVal X As Double, ByVal Y As Double, ByVal vel As Double, ByVal acc As Integer) As Integer
Declare Function move_3ax Lib "MMCWHP220.dll" (ByVal ax1 As Integer, ByVal ax2 As Integer, ByVal ax3 As Integer, ByVal X As Double, ByVal Y As Double, ByVal z As Double, ByVal vel As Double, ByVal acc As Integer) As Integer
Declare Function move_4ax Lib "MMCWHP220.dll" (ByVal ax1 As Integer, ByVal ax2 As Integer, ByVal ax3 As Integer, ByVal ax4 As Integer, ByVal X As Double, ByVal Y As Double, ByVal z As Double, ByVal w As Double, ByVal vel As Double, ByVal acc As Integer) As Integer
Declare Function move_nax Lib "MMCWHP220.dll" (ByVal axlen As Integer, axes As Integer, pos As Double, ByVal vel As Double, ByVal acc As Integer) As Integer
Declare Function move_2axgr Lib "MMCWHP220.dll" (ByVal gr As Integer, ByVal ax1 As Integer, ByVal ax2 As Integer, ByVal X As Double, ByVal Y As Double, ByVal vel As Double, ByVal acc As Integer) As Integer
Declare Function move_3axgr Lib "MMCWHP220.dll" (ByVal gr As Integer, ByVal ax1 As Integer, ByVal ax2 As Integer, ByVal ax3 As Integer, ByVal X As Double, ByVal Y As Double, ByVal z As Double, ByVal vel As Double, ByVal acc As Integer) As Integer
Declare Function move_4axgr Lib "MMCWHP220.dll" (ByVal gr As Integer, ByVal ax1 As Integer, ByVal ax2 As Integer, ByVal ax3 As Integer, ByVal ax4 As Integer, ByVal X As Double, ByVal Y As Double, ByVal z As Double, ByVal w As Double, ByVal vel As Double, ByVal acc As Integer) As Integer

Declare Function smove_2 Lib "MMCWHP220.dll" (ByVal X As Double, ByVal Y As Double) As Integer
Declare Function smove_3 Lib "MMCWHP220.dll" (ByVal X As Double, ByVal Y As Double, ByVal z As Double) As Integer
Declare Function smove_4 Lib "MMCWHP220.dll" (ByVal X As Double, ByVal Y As Double, ByVal z As Double, ByVal w As Double) As Integer
Declare Function smove_n Lib "MMCWHP220.dll" (pos As Double) As Integer
Declare Function smove_2ax Lib "MMCWHP220.dll" (ByVal ax1 As Integer, ByVal ax2 As Integer, ByVal X As Double, ByVal Y As Double, ByVal vel As Double, ByVal acc As Integer) As Integer
Declare Function smove_3ax Lib "MMCWHP220.dll" (ByVal ax1 As Integer, ByVal ax2 As Integer, ByVal ax3 As Integer, ByVal X As Double, ByVal Y As Double, ByVal z As Double, ByVal vel As Double, ByVal acc As Integer) As Integer
Declare Function smove_4ax Lib "MMCWHP220.dll" (ByVal ax1 As Integer, ByVal ax2 As Integer, ByVal ax3 As Integer, ByVal ax4 As Integer, ByVal X As Double, ByVal Y As Double, ByVal z As Double, ByVal w As Double, ByVal vel As Double, ByVal acc As Integer) As Integer
Declare Function smove_nax Lib "MMCWHP220.dll" (ByVal axlen As Integer, axes As Integer, pos As Double, ByVal vel As Double, ByVal acc As Integer) As Integer
Declare Function smove_2axgr Lib "MMCWHP220.dll" (ByVal gr As Integer, ByVal ax1 As Integer, ByVal ax2 As Integer, ByVal X As Double, ByVal Y As Double, ByVal vel As Double, ByVal acc As Integer) As Integer
Declare Function smove_3axgr Lib "MMCWHP220.dll" (ByVal gr As Integer, ByVal ax1 As Integer, ByVal ax2 As Integer, ByVal ax3 As Integer, ByVal X As Double, ByVal Y As Double, ByVal z As Double, ByVal vel As Double, ByVal acc As Integer) As Integer
Declare Function smove_4axgr Lib "MMCWHP220.dll" (ByVal gr As Integer, ByVal ax1 As Integer, ByVal ax2 As Integer, ByVal ax3 As Integer, ByVal ax4 As Integer, ByVal X As Double, ByVal Y As Double, ByVal z As Double, ByVal w As Double, ByVal vel As Double, ByVal acc As Integer) As Integer

Declare Function arc_2 Lib "MMCWHP220.dll" (ByVal xCenter As Double, ByVal yCenter As Double, ByVal angle As Double) As Integer
Declare Function arc_2ax Lib "MMCWHP220.dll" (ByVal ax1 As Integer, ByVal ax2 As Integer, ByVal x_center As Double, ByVal y_center As Double, ByVal angle As Double, ByVal vel As Double, ByVal acc As Integer) As Integer
Declare Function spl_line_move2 Lib "MMCWHP220.dll" (ppntxy As Double, ByVal vel As Double, ByVal acc As Integer) As Integer
Declare Function spl_line_move3 Lib "MMCWHP220.dll" (ppntxyz As Double, ByVal vel As Double, ByVal acc As Integer) As Integer
Declare Function spl_line_move2ax Lib "MMCWHP220.dll" (ByVal ax1 As Integer, ByVal ax2 As Integer, ppntxy As Double, ByVal vel As Double, ByVal acc As Integer) As Integer
Declare Function spl_line_move3ax Lib "MMCWHP220.dll" (ByVal ax1 As Integer, ByVal ax2 As Integer, ByVal ax3 As Integer, ppntxyz As Double, ByVal vel As Double, ByVal acc As Integer) As Integer

Declare Function spl_arc_move2 Lib "MMCWHP220.dll" (ByVal xc As Double, ByVal yc As Double, ppntxy As Double, ByVal vel As Double, ByVal acc As Integer, ByVal Dir As Integer) As Integer
Declare Function spl_arc_move3 Lib "MMCWHP220.dll" (ByVal xc As Double, ByVal yc As Double, ppntxyz As Double, ByVal vel As Double, ByVal acc As Integer, ByVal Dir As Integer) As Integer
Declare Function spl_arc_move2ax Lib "MMCWHP220.dll" (ByVal ax1 As Integer, ByVal ax2 As Integer, ByVal xc As Double, ByVal yc As Double, ppntxy As Double, ByVal vel As Double, ByVal acc As Integer, ByVal Dir As Integer) As Integer
Declare Function spl_arc_move3ax Lib "MMCWHP220.dll" (ByVal ax1 As Integer, ByVal ax2 As Integer, ByVal ax3 As Integer, ByVal xc As Double, ByVal yc As Double, ppntxyz As Double, ByVal vel As Double, ByVal acc As Integer, ByVal Dir As Integer) As Integer

Declare Function spl_move Lib "MMCWHP220.dll" (ByVal n As Integer, ByVal ax1 As Integer, ByVal ax2 As Integer, ByVal ax3 As Integer, ppntx As Double, ppnty As Double, ppntz As Double, ByVal vel As Double, ByVal acc As Integer) As Integer
Declare Function rect_move Lib "MMCWHP220.dll" (ByVal ax1 As Integer, ByVal ax2 As Integer, ppntxy As Double, ByVal vel As Double, ByVal acc As Integer) As Integer
Declare Function set_spl_auto_off Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal mode As Integer) As Integer

Declare Function start_move Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal pos As Double, ByVal vel As Double, ByVal acc As Integer) As Integer
Declare Function move Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal pos As Double, ByVal vel As Double, ByVal acc As Integer) As Integer
Declare Function start_r_move Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal pos As Double, ByVal vel As Double, ByVal acc As Integer) As Integer
Declare Function r_move Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal pos As Double, ByVal vel As Double, ByVal acc As Integer) As Integer
Declare Function start_s_move Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal pos As Double, ByVal vel As Double, ByVal acc As Integer) As Integer
Declare Function s_move Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal pos As Double, ByVal vel As Double, ByVal acc As Integer) As Integer
Declare Function start_rs_move Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal pos As Double, ByVal vel As Double, ByVal acc As Integer) As Integer
Declare Function rs_move Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal pos As Double, ByVal vel As Double, ByVal acc As Integer) As Integer
Declare Function start_t_move Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal pos As Double, ByVal vel As Double, ByVal acc As Integer, ByVal dcc As Integer) As Integer
Declare Function t_move Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal pos As Double, ByVal vel As Double, ByVal acc As Integer, ByVal dcc As Integer) As Integer
Declare Function start_ts_move Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal pos As Double, ByVal vel As Double, ByVal acc As Integer, ByVal dcc As Integer) As Integer
Declare Function ts_move Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal pos As Double, ByVal vel As Double, ByVal acc As Integer, ByVal dcc As Integer) As Integer
Declare Function start_tr_move Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal pos As Double, ByVal vel As Double, ByVal acc As Integer, ByVal dcc As Integer) As Integer
Declare Function tr_move Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal pos As Double, ByVal vel As Double, ByVal acc As Integer, ByVal dcc As Integer) As Integer
Declare Function start_trs_move Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal pos As Double, ByVal vel As Double, ByVal acc As Integer, ByVal dcc As Integer) As Integer
Declare Function trs_move Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal pos As Double, ByVal vel As Double, ByVal acc As Integer, ByVal dcc As Integer) As Integer
Declare Function start_t_move_all Lib "MMCWHP220.dll" (ByVal axlen As Integer, nax As Integer, nPos As Double, nVel As Double, nAcc As Integer, nDcc As Integer) As Integer
Declare Function t_move_all Lib "MMCWHP220.dll" (ByVal axlen As Integer, nax As Integer, nPos As Double, nVel As Double, nAcc As Integer, nDcc As Integer) As Integer
Declare Function start_ts_move_all Lib "MMCWHP220.dll" (ByVal axlen As Integer, nax As Integer, nPos As Double, nVel As Double, nAcc As Integer, nDcc As Integer) As Integer
Declare Function ts_move_all Lib "MMCWHP220.dll" (ByVal axlen As Integer, nax As Integer, nPos As Double, nVel As Double, nAcc As Integer, nDcc As Integer) As Integer

Declare Function start_move_all Lib "MMCWHP220.dll" (ByVal axlen As Integer, nax As Integer, nPos As Double, nVel As Double, nAcc As Integer) As Integer
Declare Function move_all Lib "MMCWHP220.dll" (ByVal axlen As Integer, nax As Integer, nPos As Double, nVel As Double, nAcc As Integer) As Integer
Declare Function start_s_move_all Lib "MMCWHP220.dll" (ByVal axlen As Integer, nax As Integer, nPos As Double, nVel As Double, nAcc As Integer) As Integer
Declare Function s_move_all Lib "MMCWHP220.dll" (ByVal axlen As Integer, nax As Integer, nPos As Double, nVel As Double, nAcc As Integer) As Integer
Declare Function wait_for_done Lib "MMCWHP220.dll" (ByVal ax As Integer) As Integer
Declare Function wait_for_all Lib "MMCWHP220.dll" (ByVal n As Integer, n As Integer) As Integer
Declare Function v_move Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal vel As Double, ByVal accel As Integer) As Integer

Declare Function set_positive_sw_limit Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal f As Double, ByVal n As Integer) As Integer
Declare Function get_positive_sw_limit Lib "MMCWHP220.dll" (ByVal ax As Integer, f As Double, n As Integer) As Integer
Declare Function set_negative_sw_limit Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal f As Double, ByVal n As Integer) As Integer
Declare Function get_negative_sw_limit Lib "MMCWHP220.dll" (ByVal ax As Integer, f As Double, n As Integer) As Integer

Declare Function get_accel_limit Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer
Declare Function set_accel_limit Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer
Declare Function fset_accel_limit Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer
Declare Function get_vel_limit Lib "MMCWHP220.dll" (ByVal ax As Integer, f As Double) As Integer
Declare Function set_vel_limit Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal f As Double) As Integer

Declare Function set_positive_limit Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer
Declare Function get_positive_limit Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer
Declare Function set_negative_limit Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer
Declare Function get_negative_limit Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer
Declare Function set_in_position Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal f As Double) As Integer
Declare Function get_in_position Lib "MMCWHP220.dll" (ByVal ax As Integer, f As Double) As Integer
Declare Function set_error_limit Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal f As Double, ByVal n As Integer) As Integer
Declare Function get_error_limit Lib "MMCWHP220.dll" (ByVal ax As Integer, f As Double, n As Integer) As Integer
Declare Function set_positive_level Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer
Declare Function get_positive_level Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer
Declare Function set_negative_level Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer
Declare Function get_negative_level Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer

Declare Function home_switch Lib "MMCWHP220.dll" (ByVal ax As Integer) As Integer
Declare Function pos_switch Lib "MMCWHP220.dll" (ByVal ax As Integer) As Integer
Declare Function neg_switch Lib "MMCWHP220.dll" (ByVal ax As Integer) As Integer
Declare Function amp_fault_switch Lib "MMCWHP220.dll" (ByVal ax As Integer) As Integer
Declare Function set_io Lib "MMCWHP220.dll" (ByVal bd As Integer, ByVal l As Long) As Integer
Declare Function get_io Lib "MMCWHP220.dll" (ByVal bd As Integer, l As Long) As Integer
Declare Function get_out_io Lib "MMCWHP220.dll" (ByVal bd As Integer, l As Long) As Integer
Declare Function set_bit Lib "MMCWHP220.dll" (ByVal i As Integer) As Integer
Declare Function reset_bit Lib "MMCWHP220.dll" (ByVal i As Integer) As Integer

Declare Function get_gain Lib "MMCWHP220.dll" (ByVal ax As Integer, coeff As Long) As Integer
Declare Function fget_gain Lib "MMCWHP220.dll" (ByVal ax As Integer, coeff As Long) As Integer
Declare Function set_gain Lib "MMCWHP220.dll" (ByVal ax As Integer, coeff As Long) As Integer
Declare Function fset_gain Lib "MMCWHP220.dll" (ByVal ax As Integer, coeff As Long) As Integer
Declare Function get_v_gain Lib "MMCWHP220.dll" (ByVal ax As Integer, coeff As Long) As Integer
Declare Function fget_v_gain Lib "MMCWHP220.dll" (ByVal ax As Integer, coeff As Long) As Integer
Declare Function set_v_gain Lib "MMCWHP220.dll" (ByVal ax As Integer, coeff As Long) As Integer
Declare Function fset_v_gain Lib "MMCWHP220.dll" (ByVal ax As Integer, coeff As Long) As Integer

Declare Function fset_p_integration Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer
Declare Function fset_v_integration Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer
Declare Function fget_p_integration Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer
Declare Function fget_v_integration Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer
Declare Function set_p_integration Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer
Declare Function get_p_integration Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer
Declare Function set_v_integration Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer
Declare Function get_v_integration Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer

Declare Function set_amp_enable Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer
Declare Function get_amp_enable Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer
Declare Function amp_fault_reset Lib "MMCWHP220.dll" (ByVal ax As Integer) As Integer
Declare Function amp_fault_set Lib "MMCWHP220.dll" (ByVal ax As Integer) As Integer
Declare Function set_amp_enable_level Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer
Declare Function get_amp_enable_level Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer
Declare Function fget_amp_enable_level Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer
Declare Function set_control Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer
Declare Function get_control Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer
Declare Function set_electric_gear Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal f As Double) As Integer
Declare Function get_electric_gear Lib "MMCWHP220.dll" (ByVal ax As Integer, f As Double) As Integer
Declare Function fget_electric_gear Lib "MMCWHP220.dll" (ByVal ax As Integer, f As Double) As Integer
Declare Function set_step_mode Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer
Declare Function get_step_mode Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer
Declare Function set_sync_map_axes Lib "MMCWHP220.dll" (ByVal max As Integer, ByVal sax As Integer) As Integer
Declare Function set_sync_control Lib "MMCWHP220.dll" (ByVal state As Integer) As Integer
Declare Function get_sync_control Lib "MMCWHP220.dll" (state As Integer) As Integer
Declare Function set_sync_gain Lib "MMCWHP220.dll" (ByVal gain As Integer) As Integer
Declare Function fset_sync_gain Lib "MMCWHP220.dll" (ByVal gain As Integer) As Integer
Declare Function get_sync_gain Lib "MMCWHP220.dll" (gain As Integer) As Integer
Declare Function fget_sync_gain Lib "MMCWHP220.dll" (gain As Integer) As Integer
Declare Function compensation_pos Lib "MMCWHP220.dll" (ByVal axlen As Integer, nax As Integer, nCdist As Double, nAcc As Integer) As Integer

Declare Function set_pulse_ratio Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer
Declare Function fset_pulse_ratio Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer
Declare Function get_pulse_ratio Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer

Declare Function set_stop Lib "MMCWHP220.dll" (ByVal ax As Integer) As Integer
Declare Function set_stop_rate Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer
Declare Function get_stop_rate Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer
Declare Function set_e_stop Lib "MMCWHP220.dll" (ByVal ax As Integer) As Integer
Declare Function set_e_stop_rate Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer
Declare Function get_e_stop_rate Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer

Declare Function set_home Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer
Declare Function get_home Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer
Declare Function set_home_level Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer
Declare Function get_home_level Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer
Declare Function set_index_required Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer
Declare Function get_index_required Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer

Declare Function io_interrupt_enable Lib "MMCWHP220.dll" (ByVal bdNum As Integer, ByVal state As Integer) As Integer
Declare Function io_interrupt_on_stop Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal state As Integer) As Integer
Declare Function io_interrupt_on_e_stop Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal state As Integer) As Integer
Declare Function io_interrupt_pcirq Lib "MMCWHP220.dll" (ByVal bdNum As Integer, ByVal state As Integer) As Integer
Declare Function io_interrupt_pcirq_eoi Lib "MMCWHP220.dll" (ByVal bdNum As Integer) As Integer

Declare Function set_amp_fault Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer
Declare Function get_amp_fault Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer
Declare Function set_amp_fault_level Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer
Declare Function get_amp_fault_level Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer
Declare Function fget_amp_fault_level Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer
Declare Function set_amp_reset_level Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer
Declare Function get_amp_reset_level Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer
Declare Function fget_amp_reset_level Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer

Declare Function get_analog Lib "MMCWHP220.dll" (ByVal channel As Integer, val As Integer) As Integer
Declare Function set_dac_output Lib "MMCWHP220.dll" (ByVal channel As Integer, ByVal volt As Long) As Integer
Declare Function get_dac_output Lib "MMCWHP220.dll" (ByVal channel As Integer, volt As Long) As Integer

Declare Function fset_stepper Lib "MMCWHP220.dll" (ByVal ax As Integer) As Integer
Declare Function fset_servo Lib "MMCWHP220.dll" (ByVal ax As Integer) As Integer
Declare Function fset_feedback Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer
Declare Function fset_closed_loop Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer
Declare Function fset_unipolar Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer

Declare Function fset_micro_stepper Lib "MMCWHP220.dll" (ByVal ax As Integer) As Integer
Declare Function fget_micro_stepper Lib "MMCWHP220.dll" (ByVal ax As Integer) As Integer

Declare Function fset_amp_fault Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer

Declare Function fset_control Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer
Declare Function fset_electric_gear Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal f As Double) As Integer
Declare Function fset_step_mode Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer

Declare Function fset_home Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer
Declare Function fset_index_required Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer

Declare Function fio_interrupt_enable Lib "MMCWHP220.dll" (ByVal bdNum As Integer, ByVal state As Integer) As Integer
Declare Function fio_interrupt_on_stop Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal state As Integer) As Integer
Declare Function fio_interrupt_on_e_stop Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal state As Integer) As Integer
Declare Function fio_interrupt_pcirq Lib "MMCWHP220.dll" (ByVal bdNum As Integer, ByVal state As Integer) As Integer

Declare Function fset_positive_sw_limit Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal f As Double, ByVal n As Integer) As Integer
Declare Function fset_negative_sw_limit Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal f As Double, ByVal n As Integer) As Integer
Declare Function fset_positive_limit Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer
Declare Function fset_negative_limit Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer
Declare Function fset_in_position Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal f As Double) As Integer
Declare Function fset_error_limit Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal f As Double, ByVal n As Integer) As Integer

Declare Function fset_stop_rate Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer
Declare Function fset_e_stop_rate Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer

Declare Function fget_stepper Lib "MMCWHP220.dll" (ByVal ax As Integer) As Integer
Declare Function fget_feedback Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer
Declare Function fget_closed_loop Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer
Declare Function fget_unipolar Lib "MMCWHP220.dll" (ByVal ax As Integer) As Integer

Declare Function fget_amp_fault Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer

Declare Function fget_control Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer
Declare Function fget_step_mode Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer

Declare Function fget_home Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer
Declare Function fget_index_required Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer

Declare Function fget_positive_sw_limit Lib "MMCWHP220.dll" (ByVal ax As Integer, f As Double, n As Integer) As Integer
Declare Function fget_negative_sw_limit Lib "MMCWHP220.dll" (ByVal ax As Integer, f As Double, n As Integer) As Integer
Declare Function fget_positive_limit Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer
Declare Function fget_negative_limit Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer
Declare Function fget_in_position Lib "MMCWHP220.dll" (ByVal ax As Integer, f As Double) As Integer
Declare Function fget_error_limit Lib "MMCWHP220.dll" (ByVal ax As Integer, f As Double, n As Integer) As Integer

Declare Function fget_stop_rate Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer
Declare Function fget_e_stop_rate Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer

Declare Function set_interpolation Lib "MMCWHP220.dll" (ByVal nlen As Integer, ax As Integer, delt_s As Long, ByVal flag As Integer) As Integer
Declare Function frames_interpolation Lib "MMCWHP220.dll" (ByVal ax As Integer) As Integer

Declare Function v_move_stop Lib "MMCWHP220.dll" (ByVal ax As Integer) As Integer
Declare Function set_inposition_level Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer
Declare Function fset_inposition_level Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer
Declare Function get_inposition_level Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer
Declare Function fget_inposition_level Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer

Declare Function controller_idle Lib "MMCWHP220.dll" (ByVal ax As Integer) As Integer
Declare Function controller_run Lib "MMCWHP220.dll" (ByVal ax As Integer) As Integer

Declare Function arm_latch Lib "MMCWHP220.dll" (ByVal bdNum As Integer, ByVal state As Integer) As Integer
Declare Function latch_status Lib "MMCWHP220.dll" (ByVal bdNum As Integer) As Integer
Declare Function get_latched_position Lib "MMCWHP220.dll" (ByVal ax As Integer, pos As Double) As Integer
Declare Function latch Lib "MMCWHP220.dll" (ByVal bdNum As Integer) As Integer

Declare Function set_timer Lib "MMCWHP220.dll" (ByVal bdNum As Integer, ByVal t As Long) As Integer
Declare Function get_timer Lib "MMCWHP220.dll" (ByVal bdNum As Integer, t As Long) As Integer

Declare Function fget_home_level Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer
Declare Function fget_positive_level Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer
Declare Function fget_negative_level Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer

Declare Function fset_amp_fault_level Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer
Declare Function fset_amp_reset_level Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer
Declare Function fset_amp_enable_level Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer
Declare Function fget_pulse_ratio Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer
Declare Function fset_home_level Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer
Declare Function fget_accel_limit Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer
Declare Function fget_vel_limit Lib "MMCWHP220.dll" (ByVal ax As Integer, f As Double) As Integer
Declare Function fset_vel_limit Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal f As Double) As Integer
Declare Function fset_positive_level Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer
Declare Function fset_negative_level Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer

Declare Function set_io_mode Lib "MMCWHP220.dll" (ByVal nBdNum As Integer, ByVal IoMode As Integer) As Integer
Declare Function fset_io_mode Lib "MMCWHP220.dll" (ByVal nBdNum As Integer, ByVal IoMode As Integer) As Integer
Declare Function get_io_mode Lib "MMCWHP220.dll" (ByVal nBdNum As Integer, IoMode As Integer) As Integer
Declare Function fget_io_mode Lib "MMCWHP220.dll" (ByVal nBdNum As Integer, IoMode As Integer) As Integer
Declare Function get_io_num Lib "MMCWHP220.dll" (ByVal ax As Integer, nval As Integer) As Integer

Declare Function set_analog_offset Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal volt As Integer) As Integer
Declare Function fset_analog_offset Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal volt As Integer) As Integer
Declare Function get_analog_offset Lib "MMCWHP220.dll" (ByVal ax As Integer, volt As Integer) As Integer
Declare Function fget_analog_offset Lib "MMCWHP220.dll" (ByVal ax As Integer, volt As Integer) As Integer

Declare Function set_inposition_required Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer
Declare Function fset_inposition_required Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer
Declare Function get_inposition_required Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer
Declare Function fget_inposition_required Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer

Declare Function fget_coordinate_direction Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer
Declare Function get_coordinate_direction Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer
Declare Function fset_coordinate_direction Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer
Declare Function set_coordinate_direction Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer

Declare Function fget_encoder_direction Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer
Declare Function get_encoder_direction Lib "MMCWHP220.dll" (ByVal ax As Integer, n As Integer) As Integer
Declare Function fset_encoder_direction Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer
Declare Function set_encoder_direction Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal n As Integer) As Integer

Declare Function get_axis_runstop Lib "MMCWHP220.dll" (ByVal bdNum As Integer, n As Integer) As Integer
Declare Function set_axis_runstop Lib "MMCWHP220.dll" (ByVal bdNum As Integer, ByVal n As Integer) As Integer

Declare Function set_endless_rotationax Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal mode As Integer, ByVal resolution As Integer) As Integer
Declare Function fset_endless_rotationax Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal mode As Integer, ByVal resolution As Integer) As Integer
Declare Function get_endless_rotationax Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal ax As Integer, mode As Integer) As Integer
Declare Function fget_endless_rotationax Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal ax As Integer, mode As Integer) As Integer

Declare Function set_endless_linearax Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal mode As Integer, ByVal resolution As Integer) As Integer
Declare Function fset_endless_linearax Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal mode As Integer, ByVal resolution As Integer) As Integer
Declare Function get_endless_linearax Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal ax As Integer, mode As Integer) As Integer
Declare Function fget_endless_linearax Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal ax As Integer, mode As Integer) As Integer
Declare Function set_endless_range Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal range As Double) As Integer
Declare Function fset_endless_range Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal range As Double) As Integer
Declare Function get_endless_range Lib "MMCWHP220.dll" (ByVal ax As Integer, range As Double) As Integer
Declare Function fget_endless_range Lib "MMCWHP220.dll" (ByVal ax As Integer, range As Double) As Integer

Declare Function set_linear_all_stop_flag Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal flag As Integer) As Integer
Declare Function get_linear_all_stop_flag Lib "MMCWHP220.dll" (ByVal ax As Integer, flag As Integer) As Integer

Declare Function get_command_rpm Lib "MMCWHP220.dll" (ByVal ax As Integer, rpm As Integer) As Integer
Declare Function get_encoder_rpm Lib "MMCWHP220.dll" (ByVal ax As Integer, rpm As Integer) As Integer

Declare Function set_amp_resolution Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal resolution As Integer) As Integer
Declare Function fset_amp_resolution Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal resolution As Integer) As Integer
Declare Function get_amp_resolution Lib "MMCWHP220.dll" (ByVal ax As Integer, resolution As Integer) As Integer
Declare Function fget_amp_resolution Lib "MMCWHP220.dll" (ByVal ax As Integer, resolution As Integer) As Integer

Declare Function set_collision_prevent_flag Lib "MMCWHP220.dll" (ByVal bdNum As Integer, ByVal mode As Integer) As Integer
Declare Function get_collision_prevent_flag Lib "MMCWHP220.dll" (ByVal bdNum As Integer, mode As Integer) As Integer
Declare Function set_collision_prevent Lib "MMCWHP220.dll" (ByVal max As Integer, ByVal sax As Integer, ByVal add_sub As Integer, ByVal non_equal As Integer, ByVal c_pos As Double) As Integer

Declare Function set_abs_encoder_type Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal ntype As Integer) As Integer
Declare Function get_abs_encoder_type Lib "MMCWHP220.dll" (ByVal ax As Integer, ntype As Integer) As Integer

Declare Function set_fast_read_encoder Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal status As Integer) As Integer
Declare Function get_fast_read_encoder Lib "MMCWHP220.dll" (ByVal ax As Integer, status As Integer) As Integer

Declare Function set_control_timer Lib "MMCWHP220.dll" (ByVal bdNum As Integer, ByVal ntime As Integer) As Integer
Declare Function fset_control_timer Lib "MMCWHP220.dll" (ByVal bdNum As Integer, ByVal ntime As Integer) As Integer
Declare Function get_control_timer Lib "MMCWHP220.dll" (ByVal bdNum As Integer, ntime As Integer) As Integer
Declare Function fget_control_timer Lib "MMCWHP220.dll" (ByVal bdNum As Integer, ntime As Integer) As Integer
Declare Function spl_move_data Lib "MMCWHP220.dll" (ByVal spl_num As Integer, ByVal nlen As Integer, ByVal ax1 As Integer, ByVal ax2 As Integer, ByVal ax3 As Integer, pnt1 As Double, pnt2 As Double, pnt3 As Double, ByVal vel As Double, ByVal acc As Integer) As Integer
Declare Function spl_movex Lib "MMCWHP220.dll" (ByVal splNum As Integer, ByVal ax1 As Integer, ByVal ax2 As Integer, ByVal ax3 As Integer) As Integer
Declare Function spl_arc_movenax Lib "MMCWHP220.dll" (ByVal axlen As Integer, axes As Integer, ByVal x_center As Double, ByVal y_center As Double, pnt As Double, ByVal vel As Double, ByVal acc As Integer, ByVal cdir As Integer) As Integer
Declare Function spl_line_movenax Lib "MMCWHP220.dll" (ByVal axlen As Integer, axes As Integer, pnt As Double, ByVal vel As Double, ByVal acc As Integer) As Integer

Declare Function set_mmc_led_num Lib "MMCWHP220.dll" (ByVal bdNum As Integer) As Integer
Declare Function get_mmc_led_num Lib "MMCWHP220.dll" (ByVal bdNum As Integer, ledNum As Integer) As Integer
Declare Function get_fast_position Lib "MMCWHP220.dll" (ByVal ax As Integer, pos As Double) As Integer

Declare Function set_encoder_ratioa Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal ratioa As Integer) As Integer
Declare Function fset_encoder_ratioa Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal ratioa As Integer) As Integer
Declare Function get_encoder_ratioa Lib "MMCWHP220.dll" (ByVal ax As Integer, ratioa As Integer) As Integer
Declare Function fget_encoder_ratioa Lib "MMCWHP220.dll" (ByVal ax As Integer, ratioa As Integer) As Integer

Declare Function set_encoder_ratiob Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal ratiob As Integer) As Integer
Declare Function fset_encoder_ratiob Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal ratiob As Integer) As Integer
Declare Function get_encoder_ratiob Lib "MMCWHP220.dll" (ByVal ax As Integer, ratiob As Integer) As Integer
Declare Function fget_encoder_ratiob Lib "MMCWHP220.dll" (ByVal ax As Integer, ratiob As Integer) As Integer

Declare Function set_analog_limit Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal voltage As Long) As Integer
Declare Function fset_analog_limit Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal voltage As Long) As Integer
Declare Function get_analog_limit Lib "MMCWHP220.dll" (ByVal ax As Integer, voltage As Long) As Integer
Declare Function fget_analog_limit Lib "MMCWHP220.dll" (ByVal ax As Integer, voltage As Long) As Integer

Declare Function set_position_lowpass_filter Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal HZ As Double) As Integer
Declare Function fset_position_lowpass_filter Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal HZ As Double) As Integer
Declare Function get_position_lowpass_filter Lib "MMCWHP220.dll" (ByVal ax As Integer, HZ As Double) As Integer
Declare Function fget_position_lowpass_filter Lib "MMCWHP220.dll" (ByVal ax As Integer, HZ As Double) As Integer

Declare Function set_velocity_lowpass_filter Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal HZ As Double) As Integer
Declare Function fset_velocity_lowpass_filter Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal HZ As Double) As Integer
Declare Function get_velocity_lowpass_filter Lib "MMCWHP220.dll" (ByVal ax As Integer, HZ As Double) As Integer
Declare Function fget_velocity_lowpass_filter Lib "MMCWHP220.dll" (ByVal ax As Integer, HZ As Double) As Integer

Declare Function set_position_notch_filter Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal HZ As Double) As Integer
Declare Function fset_position_notch_filter Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal HZ As Double) As Integer
Declare Function get_position_notch_filter Lib "MMCWHP220.dll" (ByVal ax As Integer, HZ As Double) As Integer
Declare Function fget_position_notch_filter Lib "MMCWHP220.dll" (ByVal ax As Integer, HZ As Double) As Integer

Declare Function set_velocity_notch_filter Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal HZ As Double) As Integer
Declare Function fset_velocity_notch_filter Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal HZ As Double) As Integer
Declare Function get_velocity_notch_filter Lib "MMCWHP220.dll" (ByVal ax As Integer, HZ As Double) As Integer
Declare Function fget_velocity_notch_filter Lib "MMCWHP220.dll" (ByVal ax As Integer, HZ As Double) As Integer

Declare Function set_mmc_parameter_init Lib "MMCWHP220.dll" (ByVal ax As Integer) As Integer
Declare Function axis_all_status Lib "MMCWHP220.dll" (ByVal ax As Integer, istatus As Integer, lstatus As Long, dstatus As Double) As Integer

Declare Function set_sensor_auto_off Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal offFlag As Integer) As Integer
Declare Function fset_sensor_auto_off Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal offFlag As Integer) As Integer
Declare Function get_sensor_auto_off Lib "MMCWHP220.dll" (ByVal ax As Integer, offFlag As Integer) As Integer
Declare Function fget_sensor_auto_off Lib "MMCWHP220.dll" (ByVal ax As Integer, offFlag As Integer) As Integer
Declare Function get_spline_move_num Lib "MMCWHP220.dll" (ByVal bdNum As Integer, num As Integer) As Integer

Declare Function set_servo_linear_flag Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal linearFlag As Integer) As Integer
Declare Function fset_servo_linear_flag Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal linearFlag As Integer) As Integer
Declare Function get_servo_linear_flag Lib "MMCWHP220.dll" (ByVal ax As Integer, linearFlag As Integer) As Integer
Declare Function fget_servo_linear_flag Lib "MMCWHP220.dll" (ByVal ax As Integer, linearFlag As Integer) As Integer

Declare Function spl_auto_line_move2ax Lib "MMCWHP220.dll" (ByVal ax1 As Integer, ByVal ax2 As Integer, pntxy As Double, ByVal vel As Double, ByVal acc As Integer, ByVal AutoOffFlag As Integer) As Integer
Declare Function spl_auto_line_move3ax Lib "MMCWHP220.dll" (ByVal ax1 As Integer, ByVal ax2 As Integer, ByVal ax3 As Integer, pntxy As Double, ByVal vel As Double, ByVal acc As Integer, ByVal AutoOffFlag As Integer) As Integer
Declare Function spl_auto_arc_move2ax Lib "MMCWHP220.dll" (ByVal ax1 As Integer, ByVal ax2 As Integer, ByVal xc As Double, ByVal yc As Double, pntxy As Double, ByVal vel As Double, ByVal acc As Integer, ByVal Dir As Integer, ByVal AutoOffFlag As Integer) As Integer
Declare Function spl_auto_arc_move3ax Lib "MMCWHP220.dll" (ByVal ax1 As Integer, ByVal ax2 As Integer, ByVal ax3 As Integer, ByVal xc As Double, ByVal yc As Double, pntxy As Double, ByVal vel As Double, ByVal acc As Integer, ByVal Dir As Integer, ByVal AutoOffFlag As Integer) As Integer
Declare Function spl_auto_arc_movenax Lib "MMCWHP220.dll" (ByVal nax As Integer, axes As Integer, ByVal xc As Double, ByVal yc As Double, pntxy As Double, ByVal vel As Double, ByVal acc As Integer, ByVal Dir As Integer, ByVal AutoOffFlag As Integer) As Integer
Declare Function spl_auto_line_movenax Lib "MMCWHP220.dll" (ByVal nax As Integer, axes As Integer, pntxy As Double, ByVal vel As Double, ByVal acc As Integer, ByVal AutoOffFlag As Integer) As Integer

Declare Function set_analog_direction Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal Dir As Integer) As Integer
Declare Function fset_analog_direction Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal Dir As Integer) As Integer
Declare Function get_analog_direction Lib "MMCWHP220.dll" (ByVal ax As Integer, Dir As Integer) As Integer
Declare Function fget_analog_direction Lib "MMCWHP220.dll" (ByVal ax As Integer, Dir As Integer) As Integer

Declare Function position_compare Lib "MMCWHP220.dll" (ByVal index_sel As Integer, ByVal index_num As Integer, ByVal bitNo As Integer, ByVal ax As Integer, ByVal latch As Integer, ByVal nfunction As Integer, ByVal pos As Double, ByVal time As Long) As Integer
Declare Function position_compare_enable Lib "MMCWHP220.dll" (ByVal bn As Integer, ByVal flag As Integer) As Integer
Declare Function position_compare_index_clear Lib "MMCWHP220.dll" (ByVal bn As Integer, ByVal index As Integer) As Integer

Declare Function spl_arc_deg_move2 Lib "MMCWHP220.dll" (ByVal x_center As Double, ByVal y_center As Double, pnt As Double, ByVal vel As Double, ByVal acc As Integer, ByVal cdir As Integer) As Integer
Declare Function spl_arc_deg_move3 Lib "MMCWHP220.dll" (ByVal x_center As Double, ByVal y_center As Double, pnt As Double, ByVal vel As Double, ByVal acc As Integer, ByVal cdir As Integer) As Integer

Declare Function spl_arc_deg_move2ax Lib "MMCWHP220.dll" (ByVal ax1 As Integer, ByVal ax2 As Integer, ByVal x_center As Double, ByVal y_center As Double, pnt As Double, ByVal vel As Double, ByVal acc As Integer, ByVal cdir As Integer) As Integer
Declare Function spl_arc_deg_move3ax Lib "MMCWHP220.dll" (ByVal ax1 As Integer, ByVal ax2 As Integer, ByVal ax3 As Integer, ByVal x_center As Double, ByVal y_center As Double, pnt As Double, ByVal vel As Double, ByVal acc As Integer, ByVal cdir As Integer) As Integer
Declare Function spl_arc_deg_movenax Lib "MMCWHP220.dll" (ByVal nlen As Integer, ax As Integer, ByVal x_center As Double, ByVal y_center As Double, pnt As Double, ByVal vel As Double, ByVal acc As Integer, ByVal cdir As Integer) As Integer

Declare Function spl_auto_arc_deg_move2ax Lib "MMCWHP220.dll" (ByVal ax1 As Integer, ByVal ax2 As Integer, ByVal x_center As Double, ByVal y_center As Double, pnt As Double, ByVal vel As Double, ByVal acc As Integer, ByVal cdir As Integer, ByVal auto_flag As Integer) As Integer
Declare Function spl_auto_arc_deg_move3ax Lib "MMCWHP220.dll" (ByVal ax1 As Integer, ByVal ax2 As Integer, ByVal ax3 As Integer, ByVal x_center As Double, ByVal y_center As Double, pnt As Double, ByVal vel As Double, ByVal acc As Integer, ByVal cdir As Integer, ByVal auto_flag As Integer) As Integer
Declare Function spl_auto_arc_deg_movenax Lib "MMCWHP220.dll" (ByVal nlen As Integer, ax As Integer, ByVal x_center As Double, ByVal y_center As Double, pnt As Double, ByVal vel As Double, ByVal acc As Integer, ByVal cdir As Integer, ByVal auto_flag As Integer) As Integer

Declare Function arc_3 Lib "MMCWHP220.dll" (ByVal x_center As Double, ByVal y_center As Double, ByVal angle As Double, pos As Double) As Integer
Declare Function arc_3ax Lib "MMCWHP220.dll" (ByVal ax1 As Integer, ByVal ax2 As Integer, ByVal ax3 As Integer, ByVal x_center As Double, ByVal y_center As Double, ByVal angle As Double, pos As Double, ByVal vel As Double, ByVal acc As Integer) As Integer

Declare Function set_encoder_filter_num Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal fn As Integer) As Integer
Declare Function fset_encoder_filter_num Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal fn As Integer) As Integer
Declare Function get_encoder_filter_num Lib "MMCWHP220.dll" (ByVal ax As Integer, fn As Integer) As Integer
Declare Function fget_encoder_filter_num Lib "MMCWHP220.dll" (ByVal ax As Integer, fn As Integer) As Integer
Declare Function get_mmc_init_chkx Lib "MMCWHP220.dll" () As Integer
Declare Function set_mmc_init_chkx Lib "MMCWHP220.dll" (ByVal bn As Integer, ByVal val As Integer) As Integer

Declare Function spl_line_move1 Lib "MMCWHP220.dll" (pntx As Double, ByVal vel As Double, ByVal acc As Integer) As Integer
Declare Function spl_line_move1ax Lib "MMCWHP220.dll" (ByVal ax As Integer, pntx As Double, ByVal vel As Double, ByVal acc As Integer) As Integer
Declare Function spl_auto_line_move1ax Lib "MMCWHP220.dll" (ByVal ax As Integer, pntx As Double, ByVal vel As Double, ByVal acc As Integer, ByVal AutoOffFlag As Integer) As Integer

Declare Function position_compare_interval Lib "MMCWHP220.dll" (ByVal Dir As Integer, ByVal ax As Integer, ByVal bitNo As Integer, ByVal startpos As Double, ByVal limitpos As Double, ByVal interval As Long, ByVal time As Long) As Integer
Declare Function set_control_timer_ax Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal time As Double) As Integer
Declare Function fset_control_timer_ax Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal time As Double) As Integer
Declare Function get_control_timer_ax Lib "MMCWHP220.dll" (ByVal ax As Integer, time As Double) As Integer
Declare Function fget_control_timer_ax Lib "MMCWHP220.dll" (ByVal ax As Integer, time As Double) As Integer

Declare Function position_io_onoff Lib "MMCWHP220.dll" (ByVal pos_num As Integer, ByVal bitNo As Integer, ByVal ax As Integer, ByVal pos As Double, ByVal encflag As Integer) As Integer
Declare Function position_io_allclear Lib "MMCWHP220.dll" (ByVal ax As Integer) As Integer
Declare Function position_io_clear Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal pos_num As Integer) As Integer

Declare Function set_friction_gain Lib "MMCWHP220.dll" (ByVal Axis As Integer, ByVal gain As Long) As Integer
Declare Function fset_friction_gain Lib "MMCWHP220.dll" (ByVal Axis As Integer, ByVal gain As Long) As Integer
Declare Function get_friction_gain Lib "MMCWHP220.dll" (ByVal Axis As Integer, gain As Long) As Integer
Declare Function fget_friction_gain Lib "MMCWHP220.dll" (ByVal Axis As Integer, gain As Long) As Integer
Declare Function set_friction_range Lib "MMCWHP220.dll" (ByVal axno As Integer, ByVal range As Double) As Integer
Declare Function fset_friction_range Lib "MMCWHP220.dll" (ByVal axno As Integer, ByVal range As Double) As Integer
Declare Function get_friction_range Lib "MMCWHP220.dll" (ByVal axno As Integer, range As Double) As Integer
Declare Function fget_friction_range Lib "MMCWHP220.dll" (ByVal axno As Integer, range As Double) As Integer

Declare Function set_mpg_velocity Lib "MMCWHP220.dll" (ByVal bdNum As Integer, ByVal mpg_vel As Integer) As Integer
Declare Function get_mpg_velocity Lib "MMCWHP220.dll" (ByVal bdNum As Integer, mpg_vel As Integer) As Integer
Declare Function set_mpg_enable Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal state As Integer) As Integer
Declare Function get_mpg_enable Lib "MMCWHP220.dll" (ByVal ax As Integer, state As Integer) As Integer

Declare Function set_enc_open_check_para Lib "MMCWHP220.dll" (ByVal bdNum As Integer, ByVal analog_ref As Integer, ByVal count_ref As Integer, ByVal tolerance As Integer) As Integer
Declare Function get_enc_open_check_para Lib "MMCWHP220.dll" (ByVal bdNum As Integer, analog_ref As Integer, count_ref As Integer, tolerance As Integer) As Integer
Declare Function set_mpg_quarter_counter Lib "MMCWHP220.dll" (ByVal state As Integer) As Integer

Declare Function set_system_io Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal OnOff As Integer) As Integer
Declare Function get_system_io Lib "MMCWHP220.dll" (ByVal ax As Integer, OnOff As Integer) As Integer
Declare Function set_amp_resolution32 Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal resolution As Long) As Integer
Declare Function fset_amp_resolution32 Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal resolution As Long) As Integer
Declare Function get_amp_resolution32 Lib "MMCWHP220.dll" (ByVal ax As Integer, resolution As Long) As Integer
Declare Function fget_amp_resolution32 Lib "MMCWHP220.dll" (ByVal ax As Integer, resolution As Long) As Integer

Declare Function set_collision_prevent_ax Lib "MMCWHP220.dll" (ByVal ax As Integer, enable As Integer, slave_ax As Integer, add_sub As Integer, non_equal As Integer, c_pos As Double, TP As Integer) As Integer
Declare Function get_collision_prevent_ax Lib "MMCWHP220.dll" (ByVal ax As Integer, enable As Integer) As Integer

Declare Function set_sync_control_ax Lib "MMCWHP220.dll" (ByVal ax As Integer, enable As Integer, master_ax As Integer, ByVal gain As Long) As Integer
Declare Function get_sync_control_ax Lib "MMCWHP220.dll" (ByVal ax As Integer, enable As Integer, master_ax As Integer, ByRef gain As Long) As Integer

Declare Function set_pause_control Lib "MMCWHP220.dll" (ByVal bn As Integer, enable As Integer, io_bit As Long) As Integer
Declare Function get_collision_position Lib "MMCWHP220.dll" (ByVal ax As Integer, position As Double) As Integer
Declare Function get_teachposition Lib "MMCWHP220.dll" (ByVal ax As Integer, position As Double) As Integer

Declare Function position_compare_reset Lib "MMCWHP220.dll" (ByVal index As Integer) As Integer
Declare Function position_compare_init Lib "MMCWHP220.dll" (ByVal index As Integer, ax1 As Integer, ax2 As Integer) As Integer
Declare Function position_compare_read Lib "MMCWHP220.dll" (ByVal index As Integer, ax As Integer, pos As Double) As Integer
Declare Function position_compare_bit Lib "MMCWHP220.dll" (ByVal bdNum As Integer, bitNum As Integer, OnOff As Integer) As Integer

Declare Function set_option_io Lib "MMCWHP220.dll" (ByVal port As Integer, ByVal value As Long) As Integer
Declare Function get_option_io Lib "MMCWHP220.dll" (ByVal port As Integer, value As Long) As Integer
Declare Function get_option_out_io Lib "MMCWHP220.dll" (ByVal port As Integer, value As Long) As Integer
' 1.51 added 2010.08.13
Declare Function get_option_io_bit Lib "MMCWHP220.dll" (ByVal port As Integer, ByVal bit As Integer, status As Integer) As Integer
' 1.51 added 2010.08.27
Declare Function get_option_out_io_bit Lib "MMCWHP220.dll" (ByVal port As Integer, ByVal bit As Integer, status As Integer) As Integer

Declare Function set_option_bit Lib "MMCWHP220.dll" (ByVal bitNo As Integer) As Integer
Declare Function reset_option_bit Lib "MMCWHP220.dll" (ByVal bitNo As Integer) As Integer

Declare Function option_fpga_version_chk Lib "MMCWHP220.dll" (ByVal bn As Integer, ByRef ver As Integer) As Integer
Declare Function HardwareReset Lib "MMCWHP220.dll" (ByVal bn As Integer) As Integer
Declare Function get_sync_map_axes Lib "MMCWHP220.dll" (ByRef Master As Integer, ByRef Slave As Integer) As Integer


Declare Function spl_line_movenax Lib "MMCWHP220.dll" (ByVal Length As Integer, ByRef ax As Integer, ByRef pnt As Double, ByVal vel As Integer, ByVal acc As Integer) As Integer
Declare Function spl_arc_movenax Lib "MMCWHP220.dll" (ByVal Length As Integer, ByRef ax As Integer, ByVal x_center As Double, ByVal y_center As Double, ByRef pnt As Double, ByVal vel As Integer, ByVal acc As Integer, ByVal cdir As Integer) As Integer
Declare Function set_event_sharing_axes Lib "MMCWHP220.dll" (ByVal ax1 As Integer, ByVal ax2 As Integer, ByVal state As Integer) As Integer
Declare Function get_event_sharing_axes Lib "MMCWHP220.dll" (ByVal bdNum As Integer, ByRef ax1 As Integer, ByRef ax2 As Integer, ByRef state As Integer) As Integer
Declare Function get_zphase_position Lib "MMCWHP220.dll" (ByVal ax As Integer, ByRef pos As Double) As Integer
Declare Function set_switch_status Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal state As Integer) As Integer
Declare Function homing_start Lib "MMCWHP220.dll" (ByVal ax As Integer) As Integer
Declare Function homing_stop Lib "MMCWHP220.dll" (ByVal ax As Integer) As Integer
Declare Function homing_process Lib "MMCWHP220.dll" (ByVal ax As Integer) As Integer
Declare Function set_homing_option Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal Timeout As Long, ByVal Config As Integer, ByVal Dir As Integer) As Integer
Declare Function get_homing_option Lib "MMCWHP220.dll" (ByVal ax As Integer, ByRef pTimeout As Long, ByRef pConfig As Integer, ByRef pDir As Integer) As Integer
Declare Function set_homing_profile Lib "MMCWHP220.dll" (ByVal ax As Integer, ByVal MainVel As Double, ByVal MainDec As Integer, ByVal RetVel As Double, ByVal RetDec As Integer, ByVal FineVel As Double, ByVal FineDec As Integer, ByVal MarkerVel As Double, ByVal MarkerDec As Integer) As Integer
Declare Function get_homing_profile Lib "MMCWHP220.dll" (ByVal ax As Integer, ByRef pMainVel As Double, ByRef pMainDec As Integer, ByRef pRetVel As Double, ByRef pRetDec As Integer, ByRef pFineVel As Double, ByRef pFineDec As Integer, ByRef pMarkerVel As Double, ByRef pMarkerDec As Integer) As Integer
Declare Function mmcsw_version_chk Lib "MMCWHP220.dll" (ByRef ver As Integer) As Integer
Declare Function set_mmcsw_version Lib "MMCWHP220.dll" (ByVal ver As Integer) As Integer
Declare Function AxisPowerOnCheck Lib "MMCWHP220.dll" (ByVal Axis As Integer) As Integer
Declare Function Find_Bd_Jnt Lib "MMCWHP220.dll" (ByVal ax As Integer, ByRef bn As Integer, ByRef jnt As Integer) As Integer
'       ival[0] = ComVel;       ival[1] = ActVel;       ival[2] = ComRpm;       ival[3] = ActRpm;       ival[4] = Control_Error;
'       lval[0] = Command_Pos;  lval[1] = Actual_Pos;   lval[2] = Encoder_Pos;
Declare Function axis_monitor_params Lib "MMCWHP220.dll" (ByVal ax As Integer, ByRef lval As Long, ByRef iVal As Integer) As Integer

'2.20 added 2014.01.07 joystick function
Declare Function set_joystick_enable Lib "MMCWHP220.dll" (ByVal bdNum As Integer, ByVal Enable As Integer) As Integer
Declare Function set_joystick_velocity Lib "MMCWHP220.dll" (ByVal bdNum As Integer, ByVal vel0 As Integer, ByVal vel1 As Integer, ByVal vel2 As Integer, ByVal vel3 As Integer) As Integer
Declare Function get_joystick_enable Lib "MMCWHP220.dll" (ByVal bdNum As Integer, ByRef Enable As Integer) As Integer
Declare Function get_joystick_velocity Lib "MMCWHP220.dll" (ByVal bdNum As Integer, ByRef vel0 As Integer, ByRef vel1 As Integer, ByRef vel2 As Integer, ByRef vel3 As Integer) As Integer
