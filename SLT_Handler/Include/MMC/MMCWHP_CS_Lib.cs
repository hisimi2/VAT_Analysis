using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.Runtime.InteropServices;
using System.Security.Permissions;


namespace MMCWHPNET
{

    /// <summary>
    /// Library Class
    /// </summary>
    /// 
    public partial class MMCLib
    {
        // DLL name should be specified what you have.
        public const string MMC_DLL_NAME = "mmcwhp220.dll";
        // axis_source status
        public enum MMCSTS : int
        {
            // MMC Status (Source)
            #region ENUM_MMCSTS
            /*-----------------------------------------------------------
            *	Limit Vlaue
            *---------------------------------------------------------*/
            ST_NONE = 0x0000,
            ST_HOME_SWITCH = 0x0001,
            ST_POS_LIMIT = 0x0002,
            ST_NEG_LIMIT = 0x0004,
            ST_AMP_FAULT = 0x0008,
            ST_A_LIMIT = 0x0010,
            ST_V_LIMIT = 0x0020,
            ST_X_NEG_LIMIT = 0x0040,
            ST_X_POS_LIMIT = 0x0080,
            ST_ERROR_LIMIT = 0x0100,
            ST_PC_COMMAND = 0x0200,
            ST_OUT_OF_FRAMES = 0x0400,
            ST_AMP_POWER_ONOFF = 0x0800,
            ST_ABS_COMM_ERROR = 0x1000,
            ST_INPOSITION_STATUS = 0x2000,
            ST_RUN_STOP_COMMAND = 0x4000,
            ST_COLLISION_STATE = 0x8000,
            #endregion
        }
        // MMC Definition
        public enum MMCDef
        {
            // MMC Definition
            #region ENUM_MMCDef
            /*-----------------------------------------------------------
            *	On and Off
            *---------------------------------------------------------*/
            ON = 1,
            OFF = 0,

            /*-----------------------------------------------------------
            *	True and False
            *---------------------------------------------------------*/
            TRUE    = 1,
            FALSE   = 0,

            /*-----------------------------------------------------------
            *	High and Low
            *---------------------------------------------------------*/
            HIGH    = 1,
            LOW     = 0,
            /*-----------------------------------------------------------
            *	Coordinate  Direction
            *---------------------------------------------------------*/
            CORD_CCW    = 1,
            CORD_CW     = 0,
            /*-----------------------------------------------------------
            *	Encoder  Direction
            *---------------------------------------------------------*/
            ENCODER_CCW     = 1,
            ENCODER_CW      = 0,
            /*-----------------------------------------------------------
            *	Event defines
            *---------------------------------------------------------*/
            NO_EVENT        = 0, 	/* ignore a condition */
            STOP_EVENT      = 1,	/* generate a stop event */
            E_STOP_EVENT    = 2, 	/* generate an e_stop event */
            ABORT_EVENT     = 3, 	/* disable PID control, and disable the amplifier */
            /*-----------------------------------------------------------
            *	Digital Filter Defines
            *---------------------------------------------------------*/
            GAIN_NUMBER = 5,	/* elements expected get/set_filter(...) */
            GA_P        = 0,	/* proportional gain */
            GA_I        = 1,	/* integral gain */
            GA_D        = 2,	/* derivative gain-damping term */
            GA_F        = 3,	/* velocity feed forward */
            GA_ILIMIT   = 4,	/* integration summing limit */
            /*-----------------------------------------------------------
            *	Error Defines
            *---------------------------------------------------------*/
            MAX_ERROR_CODE_NUM = 20,
            MAX_ERROR_LEN = 80,	/* maximum length for error massage string */
            MMC_OK                          = 0,	/* no problems */
            MMC_NOT_INITIALIZED             = 1,	/* be sure to call mmc_init(...) */
            MMC_TIMEOUT_ERR                 = 2,	/* DPRAM Communication Error */
            MMC_INVALID_AXIS                = 3,	/* axis out of range or not configured */
            MMC_ILLEGAL_ANALOG              = 4,	/* analog channel < 0 or > 4 */
            MMC_ILLEGAL_IO                  = 5,	/* illegal I/O port */
            MMC_ILLEGAL_PARAMETER           = 6,	/* move with zero accel or velocity */
            MMC_NO_MAP                      = 7, 	/* The map_axes(...) funcation has not been called */
            MMC_AMP_FAULT                   = 8, 	/* amp fault occured */
            MMC_ON_MOTION                   = 9, 	/* Motion is not completed */
            MMC_NON_EXIST                   = 10,	/* MMC Board is not exist */
            MMC_BOOT_OPEN_ERROR             = 11,	/* MMC Boot File Read/Write Error*/
            MMC_CHKSUM_OPEN_ERROR           = 12,	/* MMC Checksum File Read/Write Error*/
            MMC_WINNT_DRIVER_OPEN_ERROR     = 13,	/* MMC Windows NT Driver Open Error*/
            MMC_EVENT_OCCUR_ERROR           = 14,
            MMC_AMP_POWER_OFF               = 15,
            MMC_DATA_DIRECTORY_OPEN_ERROR   = 16,
            MMC_INVALID_CPMOTION_GROUP      = 17,
            MMC_VELOCITY_ILLEGAL_PARAMETER  = 18,	/* move with zero accel or velocity */
            MMC_ACCEL_ILLEGAL_PARAMETER     = 19,	/* move with zero accel or velocity */
            FUNC_ERR                        = -1,	/* Function Error				*/

            //homing api return
            MMC_HOMING_DONE	   = 0,
            MMC_HOMING_ERROR   = 1,
            MMC_HOMING_WORKING = 2,
            MMC_HOMING_TIMEOUT = 3,
            MMC_HOMING_STOP    = 4,

            /*---------------------------------------------------------------------------*/
            /* General Defines                                                           */
            /*---------------------------------------------------------------------------*/
            POSITIVE        = 1,
            NEGATIVE        = 0,

            /*-----------------------------------------------------------
            *	Motor Type
            *---------------------------------------------------------*/
            SERVO_MOTOR     = 0,
            STEPPER         = 1,
            MICRO_STEPPER   = 2,

            /*-----------------------------------------------------------
            *	Feedback Configuration
            *---------------------------------------------------------*/
            FB_ENCODER      = 0,
            FB_UNIPOLAR     = 1,
            FB_BIPOLAR      = 2,

            /*-----------------------------------------------------------
            *	Control_Loop Method
            *---------------------------------------------------------*/
            OPEN_LOOP       = 0,
            CLOSED_LOOP     = 1,
            SEMI_LOOP       = 2,

            /*-----------------------------------------------------------
            *	Active Level
            *---------------------------------------------------------*/
            LOW_ACTIVE = 0,
            HIGH_ACTIVE = 1,

            /*-----------------------------------------------------------
            *	Set Reset
            *---------------------------------------------------------*/
            MMC_Set = 0,
            MMC_Reset = 1,

            /*-----------------------------------------------------------
            *	Control Method
            *---------------------------------------------------------*/
            V_CONTROL       = 0,
            T_CONTROL       = 1,

            IN_STANDING     = 0,
            IN_ALWAYS       = 1,

            TWO_PULSE       = 0,
            SIGN_PULSE      = 1,

            UNIPOLAR = 0,
            BIPOLAR = 1,

            /*-----------------------------------------------------------
            *	Limit Vlaue
            *---------------------------------------------------------*/
            MMC_ACCEL_LIMIT     =  25000,
            MMC_VEL_LIMIT       =  5000000,
            MMC_POS_SW_LIMIT    =  2147483640,
            MMC_NEG_SW_LIMIT    = -2147483640,
            MMC_ERROR_LIMIT     =  35000,
            MMC_PULSE_RATIO     = 8
            #endregion
        }
    }
}


namespace MMCWHPNET
{
    // MMC Library APIs
    public partial class MMCLib
    {
        //MMC API Functions
        #region Public Function
        [DllImport(MMC_DLL_NAME)]
        // This is an additional API to get monitoring variable of an axis.
        // lval size is 3, ival size is 5, 
        // lval[0] = command_pos
        // lval[1] = actual_pos
        // lval[2] = Encoder_pos
        // ival[0] = Comvel
        // ival[1] = Actvel
        // ival[2] = ComRPM
        // ival[3] = ActRPM
        // ival[4] = Control_Error
        public static extern short axis_monitor_params(short ax, [Out] int[] lval, [Out]  short[] ival);

        [DllImport(MMC_DLL_NAME)]
        public static extern short get_bd_num();
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_axis_num();
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_velocity(short ax);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_version();
        [DllImport(MMC_DLL_NAME)]
        public static extern short Find_Bd_Jnt(short ax, ref short board, ref short joint);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_enc_open_check_para(short board, short analog_ref, short count_ref, short tolerance);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_enc_open_check_para(short board, ref short analog_ref, ref short count_ref, ref short tolerance);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_counter(short ax, ref double pos);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_sync_position(ref double pos_m, ref double pos_s);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_position(short ax, double pos);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_position(short ax, ref double pos);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_fast_position(short ax, ref double pos);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_command(short ax, double pos);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_command(short ax, ref double pos);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_error(short ax, ref double error);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_com_velocity(short ax);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_act_velocity(short ax);
        [DllImport(MMC_DLL_NAME)]
        public static extern short arm_latch(short bn, short state);
        [DllImport(MMC_DLL_NAME)]
        public static extern short latch_status(short bn);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_latched_position(short ax, ref double pos);
        [DllImport(MMC_DLL_NAME)]
        public static extern short latch(short bn);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_command_rpm(short ax, ref short rpm_val);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_encoder_rpm(short ax, ref short rpm_val);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_abs_encoder_type(short ax, short type);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_abs_encoder_type(short ax, ref short type);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_amp_resolution(short ax, short resolution);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_amp_resolution(short ax, short resolution);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_amp_resolution(short ax, ref short resolution);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_amp_resolution(short ax, ref short resolution);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_amp_resolution32(short ax, int resolution);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_amp_resolution32(short ax, int resolution);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_amp_resolution32(short ax, ref int resolution);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_amp_resolution32(short ax, ref int resolution);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_collision_prevent_flag(short bd_num, short mode);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_collision_prevent_flag(short bd_num, ref short mode);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_collision_prevent(short max, short sax, short add_sub, short non_equal, double c_pos);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_collision_prevent_ax(short ax, short enable, short slave_ax, short add_sub, short non_equal, double c_pos, short type);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_collision_prevent_ax(short ax, ref short enable);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_collision_position(short ax, ref double position);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_teachposition(short ax, ref double position);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_fast_read_encoder(short ax, short status);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_fast_read_encoder(short ax, ref short status);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_encoder_ratioa(short ax, short ratioa);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_encoder_ratioa(short ax, short ratioa);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_encoder_ratioa(short ax, ref short ratioa);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_encoder_ratioa(short ax, ref short ratioa);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_encoder_ratiob(short ax, short ratiob);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_encoder_ratiob(short ax, short ratiob);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_encoder_ratiob(short ax, ref short ratiob);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_encoder_ratiob(short ax, ref short ratiob);
        [DllImport(MMC_DLL_NAME)]
        public static extern short save_boot_frame();
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_abs_encoder(short ax);
        [DllImport(MMC_DLL_NAME)]
        public static extern short mmc_axes(short bdNum, ref short axes);
        [DllImport(MMC_DLL_NAME)]
        public static extern short mmc_all_axes();
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_stepper(short ax);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_stepper(short ax);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_micro_stepper(short ax);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_micro_stepper(short ax);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_stepper(short ax);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_stepper(short ax);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_micro_stepper(short ax);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_micro_stepper(short ax);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_servo(short ax);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_servo(short ax);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_feedback(short ax, short device);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_feedback(short ax, short device);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_feedback(short ax, ref short device);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_feedback(short ax, ref short device);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_closed_loop(short ax, short loop);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_closed_loop(short ax, short loop);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_closed_loop(short ax, ref short loop);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_closed_loop(short ax, ref short loop);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_unipolar(short ax, short state);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_unipolar(short ax, short state);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_unipolar(short ax);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_unipolar(short ax);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_axis_runstop(short bd_num, short mode);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_axis_runstop(short bd_num, ref short mode);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_coordinate_direction(short ax, short direc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_coordinate_direction(short ax, short direc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_coordinate_direction(short ax, ref short direc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_coordinate_direction(short ax, ref short direc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_encoder_direction(short ax, short direc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_encoder_direction(short ax, short direc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_encoder_direction(short ax, ref short direc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_encoder_direction(short ax, ref short direc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_amp_fault(short ax, short act);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_amp_fault(short ax, short act);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_amp_fault(short ax, ref short act);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_amp_fault(short ax, ref short act);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_amp_fault_level(short ax, short level);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_amp_fault_level(short ax, short level);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_amp_fault_level(short ax, ref short level);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_amp_fault_level(short ax, ref short level);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_amp_reset_level(short ax, short level);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_amp_reset_level(short ax, short level);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_amp_reset_level(short ax, ref short level);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_amp_reset_level(short ax, ref short level);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_inposition_level(short ax, short level);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_inposition_level(short ax, short level);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_inposition_required(short ax, short inposflag);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_inposition_required(short ax, short inposflag);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_inposition_level(short ax, ref short level);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_inposition_level(short ax, ref short level);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_inposition_required(short ax, ref short inposflag);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_inposition_required(short ax, ref short inposflag);
        [DllImport(MMC_DLL_NAME)]
        public static extern short controller_idle(short ax);
        [DllImport(MMC_DLL_NAME)]
        public static extern short controller_run(short ax);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_encoder_filter_num(short ax, short fn);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_encoder_filter_num(short ax, short fn);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_encoder_filter_num(short ax, ref short fn);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_encoder_filter_num(short ax, ref short fn);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_system_io(short ax, short onoff);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_system_io(short ax, ref short onoff);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_analog(short channel, ref short value);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_dac_output(short ax, short voltage);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_dac_output(short ax, ref short voltage);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_analog_offset(short ax, short voltage);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_analog_offset(short ax, short voltage);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_analog_offset(short ax, ref short voltage);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_analog_offset(short ax, ref short voltage);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_analog_limit(short ax, int voltage);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_analog_limit(short ax, int voltage);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_analog_limit(short ax, ref int voltage);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_analog_limit(short ax, ref int voltage);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_position_lowpass_filter(short ax, double hz);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_position_lowpass_filter(short ax, double hz);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_position_lowpass_filter(short ax, ref double hz);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_position_lowpass_filter(short ax, ref double hz);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_velocity_lowpass_filter(short ax, double hz);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_velocity_lowpass_filter(short ax, double hz);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_velocity_lowpass_filter(short ax, ref double hz);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_velocity_lowpass_filter(short ax, ref double hz);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_position_notch_filter(short ax, double hz);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_position_notch_filter(short ax, double hz);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_position_notch_filter(short ax, ref double hz);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_position_notch_filter(short ax, ref double hz);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_velocity_notch_filter(short ax, double hz);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_velocity_notch_filter(short ax, double hz);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_velocity_notch_filter(short ax, ref double hz);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_velocity_notch_filter(short ax, ref double hz);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_analog_direction(short ax, short dac_dir);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_analog_direction(short ax, short dac_dir);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_analog_direction(short ax, ref short dac_dir);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_analog_direction(short ax, ref short dac_dir);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_amp_enable_level(short ax, short level);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_amp_enable_level(short ax, short level);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_amp_enable_level(short ax, ref short level);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_amp_enable_level(short ax, ref short level);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_amp_enable(short ax, short state);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_amp_enable(short ax, ref short state);
        [DllImport(MMC_DLL_NAME)]
        public static extern short amp_fault_reset(short ax);
        [DllImport(MMC_DLL_NAME)]
        public static extern short amp_fault_set(short ax);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_control(short ax, short control);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_control(short ax, short control);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_pulse_ratio(short ax, short pgratio);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_pulse_ratio(short ax, short pgratio);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_pulse_ratio(short ax, ref short pgratio);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_pulse_ratio(short ax, ref short pgratio);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_control(short ax, ref short control);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_control(short ax, ref short control);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_electric_gear(short ax, double ratio);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_electric_gear(short ax, double ratio);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_electric_gear(short ax, ref double ratio); // KSK comment Need to check return type
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_electric_gear(short ax, ref double ratio);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_step_mode(short ax, short mode);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_step_mode(short ax, short mode);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_step_mode(short ax, ref short mode);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_step_mode(short ax, ref short mode);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_sync_map_axes(short Master, short Slave);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_sync_control(short condition);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_sync_control(ref short condition);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_sync_gain(int syncgain);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_sync_gain(int syncgain);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_sync_gain(ref int syncgain);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_sync_gain(ref int syncgain);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_sync_control_ax(short ax, short enable, short master_ax, int gain);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_sync_control_ax(short ax, ref short enable, ref short master_ax, [Out] int[] gain);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_pause_control(short bn, short enable, int io_bit);
        [DllImport(MMC_DLL_NAME)]
        public static extern short compensation_pos(short len, short[] ax, double[] pos, short[] acc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short version_chk(short bn, ref short ver);
        [DllImport(MMC_DLL_NAME)]
        public static extern short motion_fpga_version_chk(short bn, ref short ver);
        [DllImport(MMC_DLL_NAME)]
        public static extern short option_fpga_version_chk(short bn, ref short ver);
        [DllImport(MMC_DLL_NAME)]
        public static extern short mmcsw_version_chk(ref short ver);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_mmcsw_version(short bn);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_mmc_led_num(short bn);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_mmc_led_num(short bn, ref short led_num);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_mmc_parameter_init(short ax);
        [DllImport(MMC_DLL_NAME)]
        public static extern short mmc_dwell(short ax, int duration);
        [DllImport(MMC_DLL_NAME)]
        public static extern short mmc_io_trigger(short ax, short bitNo, short state);
        [DllImport(MMC_DLL_NAME)]
        public static extern short mmcDelay(int duration);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_gain(short ax, [Out] int[] coeff);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_gain(short ax, [Out] int[] coeff);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_gain(short ax, int[] coeff);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_gain(short ax, int[] coeff);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_v_gain(short ax, [Out] int[] coeff);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_v_gain(short ax, [Out] int[] coeff);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_v_gain(short ax, int[] coeff);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_v_gain(short ax, int[] coeff);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_p_integration(short ax, short mode);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_p_integration(short ax, short mode);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_p_integration(short ax, ref short mode);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_p_integration(short ax, ref short mode);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_v_integration(short ax, short mode);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_v_integration(short ax, short mode);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_v_integration(short ax, ref short mode);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_v_integration(short ax, ref short mode);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_friction_gain(short ax, int gain);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_friction_gain(short ax, int gain);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_friction_gain(short ax, ref int gain);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_friction_gain(short ax, ref int gain);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_friction_range(short ax, double range);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_friction_range(short ax, double range);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_friction_range(short ax, ref double range);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_friction_range(short ax, ref double range);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_home(short ax, short act);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_home(short ax, short act);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_home(short ax, ref short act);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_home(short ax, ref short act);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_home_level(short ax, short level);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_home_level(short ax, short level);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_home_level(short ax, ref short level);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_home_level(short ax, ref short level);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_index_required(short ax, short index);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_index_required(short ax, short index);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_index_required(short ax, ref short index);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_index_required(short ax, ref short index);
        [DllImport(MMC_DLL_NAME)]
        public static extern uint get_linear_address(short bd_num);   // for flashrom down;
        [DllImport(MMC_DLL_NAME)]
        public static extern System.IntPtr _error_message(short code);

        // C# function for error message
        public static short error_message(short code, ref string message)
        {
            MMCString mmcstr = new MMCString();
            if (code < 0 || code > Convert.ToInt16(MMCDef.MAX_ERROR_CODE_NUM)) 
            {
                code = Convert.ToInt16(MMCDef.MAX_ERROR_CODE_NUM);
            }
            message = mmcstr.str_Error[code];
            return 0;
        }
        [DllImport(MMC_DLL_NAME)]
        public static extern short mmc_initx(short len, long[] dp_addr);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_mmc_init_chkx();
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_mmc_init_chkx(short bn, short val);
        [DllImport(MMC_DLL_NAME)]
        public static extern short io_interrupt_enable(short bn, short state);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fio_interrupt_enable(short bn, short state);
        [DllImport(MMC_DLL_NAME)]
        public static extern short io_interrupt_on_stop(short ax, short state);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fio_interrupt_on_stop(short ax, short state);
        [DllImport(MMC_DLL_NAME)]
        public static extern short io_interrupt_on_e_stop(short ax, short state);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fio_interrupt_on_e_stop(short ax, short state);
        [DllImport(MMC_DLL_NAME)]
        public static extern short io_interrupt_pcirq(short bn, short state);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fio_interrupt_pcirq(short bn, short state);
        [DllImport(MMC_DLL_NAME)]
        public static extern short io_interrupt_pcirq_eoi(short bn);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_timer(short bn, int time);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_timer(short bn, ref int time);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_control_timer(short bn, short time);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_control_timer(short bn, short time);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_control_timer(short bn, ref short time);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_control_timer(short bn, ref short time);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_control_timer_ax(short ax, double time);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_control_timer_ax(short ax, double time);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_control_timer_ax(short ax, ref double time);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_control_timer_ax(short ax, ref double time);
        [DllImport(MMC_DLL_NAME)]
        public static extern short frames_interpolation(short ax);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_interpolation(short Len, short[] ax, int[] idelt_s, short flag);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_io_num(short ax, ref short val);
        [DllImport(MMC_DLL_NAME)]
        public static extern short home_switch(short ax);
        [DllImport(MMC_DLL_NAME)]
        public static extern short pos_switch(short ax);
        [DllImport(MMC_DLL_NAME)]
        public static extern short neg_switch(short ax);
        [DllImport(MMC_DLL_NAME)]
        public static extern short amp_fault_switch(short ax);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_io(short port, int value);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_option_io(short port, int value);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_io(short port, ref int value);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_option_io(short port, ref int value);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_option_io_bit(short port, short bit, ref short status);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_option_out_io_bit(short port, short bit, ref short status);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_out_io(short port, ref int value);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_option_out_io(short port, ref int value);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_bit(short bitNo);
        [DllImport(MMC_DLL_NAME)]
        public static extern short reset_bit(short bitNo);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_option_bit(short bitNo);
        [DllImport(MMC_DLL_NAME)]
        public static extern short reset_option_bit(short bitNo);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_io_mode(short bd_num, short mode);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_io_mode(short bd_num, short mode);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_io_mode(short bd_num, ref short mode);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_io_mode(short bd_num, ref short mode);
        [DllImport(MMC_DLL_NAME)]
        public static extern short position_compare(short index_sel, short index_num, short bitNo, short ax1, short ax2, short latch, short function, short out_mode, double pos, int time);
        [DllImport(MMC_DLL_NAME)]
        public static extern short position_compare_reset(short bn);
        [DllImport(MMC_DLL_NAME)]
        public static extern short position_compare_interval(short dir, short ax, short bitNo, double startpos, double limitpos, int interval, int time);
        [DllImport(MMC_DLL_NAME)]
        public static extern short position_compare_enable(short bn, short flag);
        [DllImport(MMC_DLL_NAME)]
        public static extern short position_compare_index_clear(short bn, short index);
        [DllImport(MMC_DLL_NAME)]
        public static extern short position_compare_init(short index_sel, short ax1, short ax2);
        [DllImport(MMC_DLL_NAME)]
        public static extern short position_compare_read(short index_sel, short ax, ref double pos);
        [DllImport(MMC_DLL_NAME)]
        public static extern short position_compare_bit(short bdNum, short bitNum, short OnOff);
        [DllImport(MMC_DLL_NAME)]
        public static extern short position_io_onoff(short pos_num, short bitNo, short ax, double pos, short encflag);
        [DllImport(MMC_DLL_NAME)]
        public static extern short position_io_allclear(short ax);
        [DllImport(MMC_DLL_NAME)]
        public static extern short position_io_clear(short ax, short pos_num);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_positive_sw_limit(short ax, double limit, short action);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_positive_sw_limit(short ax, double limit, short action);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_positive_sw_limit(short ax, ref double limit, ref short action);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_positive_sw_limit(short ax, ref double limit, ref short action);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_negative_sw_limit(short ax, double limit, short action);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_negative_sw_limit(short ax, double limit, short action);

        /// <summary>
        /// [Function] Get Positive S/W Limit and Event
        /// </summary>
        /// <param name="ax">축 번호</param>
        /// <param name="limit"></param>
        /// <param name="action"></param>
        /// <returns>System.Int16</returns>
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_negative_sw_limit(short ax, ref double limit, ref short action);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_negative_sw_limit(short ax, ref double limit, ref short action);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_accel_limit(short ax, ref short limit);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_accel_limit(short ax, ref short limit);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_accel_limit(short ax, short limit);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_accel_limit(short ax, short limit);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_vel_limit(short ax, ref double limit);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_vel_limit(short ax, ref double limit);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_vel_limit(short ax, double limit);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_vel_limit(short ax, double limit);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_positive_limit(short ax, short act);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_positive_limit(short ax, short act);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_positive_limit(short ax, ref short act);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_positive_limit(short ax, ref short act);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_negative_limit(short ax, short act);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_negative_limit(short ax, short act);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_negative_limit(short ax, ref short act);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_negative_limit(short ax, ref short act);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_in_position(short ax, double pos);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_in_position(short ax, double pos);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_in_position(short ax, ref double pos);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_in_position(short ax, ref double pos);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_error_limit(short ax, double limit, short action);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_error_limit(short ax, double limit, short action);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_error_limit(short ax, ref double limit, ref short action);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_error_limit(short ax, ref double limit, ref short action);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_positive_level(short ax, short level);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_positive_level(short ax, short level);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_positive_level(short ax, ref short level);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_positive_level(short ax, ref short level);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_negative_level(short ax, short level);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_negative_level(short ax, short level);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_negative_level(short ax, ref short level);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_negative_level(short ax, ref short level);
        [DllImport(MMC_DLL_NAME)]
        public static extern short AxisPowerOnCheck(short ax);
        [DllImport(MMC_DLL_NAME)]
        public static extern short start_move(short ax, double pos, double vel, short acc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short move(short ax, double pos, double vel, short acc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short start_r_move(short ax, double pos, double vel, short acc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short r_move(short ax, double pos, double vel, short acc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short start_s_move(short ax, double pos, double vel, short acc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short s_move(short ax, double pos, double vel, short acc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short start_rs_move(short ax, double pos, double vel, short acc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short rs_move(short ax, double pos, double vel, short acc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short start_t_move(short ax, double pos, double vel, short acc, short dcc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short t_move(short ax, double pos, double vel, short acc, short dcc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short start_ts_move(short ax, double pos, double vel, short acc, short dcc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short ts_move(short ax, double pos, double vel, short acc, short dcc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short start_tr_move(short ax, double pos, double vel, short acc, short dcc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short tr_move(short ax, double pos, double vel, short acc, short dcc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short start_trs_move(short ax, double pos, double vel, short acc, short dcc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short trs_move(short ax, double pos, double vel, short acc, short dcc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short start_move_all(short len, short[] ax, double[] pos, double[] vel, short[] acc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short move_all(short len, short[] ax, double[] pos, double[] vel, short[] acc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short start_s_move_all(short len, short[] ax, double[] pos, double[] vel, short[] acc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short s_move_all(short len, short[] ax, double[] pos, double[] vel, short[] acc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short start_t_move_all(short len, short[] ax, double[] pos, double[] vel, short[] acc, short[] dcc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short t_move_all(short len, short[] ax, double[] pos, double[] vel, short[] acc, short[] dcc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short start_ts_move_all(short len, short[] ax, double[] pos, double[] vel, short[] acc, short[] dcc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short ts_move_all(short len, short[] ax, double[] pos, double[] vel, short[] acc, short[] dcc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short wait_for_done(short ax);
        [DllImport(MMC_DLL_NAME)]
        public static extern short wait_for_all(short len, short[] ax);
        [DllImport(MMC_DLL_NAME)]
        public static extern short v_move(short ax, double vel, short acc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short v_move_stop(short ax);
        [DllImport(MMC_DLL_NAME)]
        public static extern short map_axes(short n_axes, short[] map_array);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_move_speed(double speed);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_move_accel(short acc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_arc_division(double degrees);
        [DllImport(MMC_DLL_NAME)]
        public static extern short all_done();
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_spl_auto_off(short bd_num, short mode);
        [DllImport(MMC_DLL_NAME)]
        public static extern short move_2(double x, double y);
        [DllImport(MMC_DLL_NAME)]
        public static extern short move_2ax(short ax1, short ax2, double x, double y, double vel, short acc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short move_2axgr(short gr, short ax1, short ax2, double x, double y, double vel, short acc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short move_3(double x, double y, double z);
        [DllImport(MMC_DLL_NAME)]
        public static extern short move_3ax(short ax1, short ax2, short ax3, double x, double y, double z, double vel, short acc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short move_3axgr(short gr, short ax1, short ax2, short ax3, double x, double y, double z, double vel, short acc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short move_4(double x, double y, double z, double w);
        [DllImport(MMC_DLL_NAME)]
        public static extern short move_4ax(short ax1, short ax2, short ax3, short ax4, double x, double y, double z, double w, double vel, short acc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short move_4axgr(short gr, short ax1, short ax2, short ax3, short ax4, double x, double y, double z, double w, double vel, short acc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short move_n(double[] x);
        [DllImport(MMC_DLL_NAME)]
        public static extern short move_nax(short len, short[] ax, double[] pos, double vel, short acc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short smove_2(double x, double y);
        [DllImport(MMC_DLL_NAME)]
        public static extern short smove_2ax(short ax1, short ax2, double x, double y, double vel, short acc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short smove_2axgr(short gr, short ax1, short ax2, double x, double y, double vel, short acc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short smove_3(double x, double y, double z);
        [DllImport(MMC_DLL_NAME)]
        public static extern short smove_3ax(short ax1, short ax2, short ax3, double x, double y, double z, double vel, short acc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short smove_3axgr(short gr, short ax1, short ax2, short ax3, double x, double y, double z, double vel, short acc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short smove_4(double x, double y, double z, double w);
        [DllImport(MMC_DLL_NAME)]
        public static extern short smove_4ax(short ax1, short ax2, short ax3, short ax4, double x, double y, double z, double w, double vel, short acc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short smove_4axgr(short gr, short ax1, short ax2, short ax3, short ax4, double x, double y, double z, double w, double vel, short acc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short smove_n(double[] x);
        [DllImport(MMC_DLL_NAME)]
        public static extern short smove_nax(short len, short[] ax, double[] pos, double vel, short acc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short arc_2(double x_center, double y_center, double angle);
        [DllImport(MMC_DLL_NAME)]
        public static extern short arc_2ax(short ax1, short ax2, double x_center, double y_center, double angle, double vel, short acc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short arc_3(double x_center, double y_center, double angle, double[] pos);
        [DllImport(MMC_DLL_NAME)]
        public static extern short arc_3ax(short ax1, short ax2, short ax3, double x_center, double y_center, double angle, double[] pos, double vel, short acc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short spl_line_move1(double[] pnt, double vel, short acc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short spl_line_move1ax(short ax1, double[] pnt, double vel, short acc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short spl_auto_line_move1ax(short ax1, double[] pnt, double vel, short acc, short auto_flag);
        [DllImport(MMC_DLL_NAME)]
        public static extern short spl_line_move2(double[] pnt, double vel, short acc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short spl_line_move2ax(short ax1, short ax2, double[] pnt, double vel, short acc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short spl_auto_line_move2ax(short ax1, short ax2, double[] pnt, double vel, short acc, short auto_flag);
        [DllImport(MMC_DLL_NAME)]
        public static extern short spl_line_move3(double[] pnt, double vel, short acc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short spl_line_move3ax(short ax1, short ax2, short ax3, double[] pnt, double vel, short acc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short spl_auto_line_move3ax(short ax1, short ax2, short ax3, double[] pnt, double vel, short acc, short auto_flag);
        [DllImport(MMC_DLL_NAME)]
        public static extern short spl_line_movenax(short len, short[] ax, double[] pnt, double vel, short acc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short spl_auto_line_movenax(short len, short[] ax, double[] pnt, double vel, short acc, short auto_flag);
        [DllImport(MMC_DLL_NAME)]
        public static extern short spl_arc_move2(double x_center, double y_center, double[] pnt, double vel, short acc, short cdir);
        [DllImport(MMC_DLL_NAME)]
        public static extern short spl_arc_deg_move2(double x_center, double y_center, double[] pnt, double vel, short acc, short cdir);
        [DllImport(MMC_DLL_NAME)]
        public static extern short spl_arc_move2ax(short ax1, short ax2, double x_center, double y_center, double[] pnt, double vel, short acc, short cdir);
        [DllImport(MMC_DLL_NAME)]
        public static extern short spl_arc_deg_move2ax(short ax1, short ax2, double x_center, double y_center, double[] pnt, double vel, short acc, short cdir);
        [DllImport(MMC_DLL_NAME)]
        public static extern short spl_auto_arc_move2ax(short ax1, short ax2, double x_center, double y_center, double[] pnt, double vel, short acc, short cdir, short auto_flag);
        [DllImport(MMC_DLL_NAME)]
        public static extern short spl_auto_arc_deg_move2ax(short ax1, short ax2, double x_center, double y_center, double[] pnt, double vel, short acc, short cdir, short auto_flag);
        [DllImport(MMC_DLL_NAME)]
        public static extern short spl_arc_move3(double x_center, double y_center, double[] pnt, double vel, short acc, short cdir);
        [DllImport(MMC_DLL_NAME)]
        public static extern short spl_arc_deg_move3(double x_center, double y_center, double[] pnt, double vel, short acc, short cdir);
        [DllImport(MMC_DLL_NAME)]
        public static extern short spl_arc_move3ax(short ax1, short ax2, short ax3, double x_center, double y_center, double[] pnt, double vel, short acc, short cdir);
        [DllImport(MMC_DLL_NAME)]
        public static extern short spl_arc_deg_move3ax(short ax1, short ax2, short ax3, double x_center, double y_center, double[] pnt, double vel, short acc, short cdir);
        [DllImport(MMC_DLL_NAME)]
        public static extern short spl_auto_arc_move3ax(short ax1, short ax2, short ax3, double x_center, double y_center, double[] pnt, double vel, short acc, short cdir, short auto_flag);
        [DllImport(MMC_DLL_NAME)]
        public static extern short spl_auto_arc_deg_move3ax(short ax1, short ax2, short ax3, double x_center, double y_center, double[] pnt, double vel, short acc, short cdir, short auto_flag);
        [DllImport(MMC_DLL_NAME)]
        public static extern short spl_arc_movenax(short len, short[] ax, double x_center, double y_center, double[] pnt, double vel, short acc, short cdir);
        [DllImport(MMC_DLL_NAME)]
        public static extern short spl_arc_deg_movenax(short len, short[] ax, double x_center, double y_center, double[] pnt, double vel, short acc, short cdir);
        [DllImport(MMC_DLL_NAME)]
        public static extern short spl_auto_arc_movenax(short len, short[] ax, double x_center, double y_center, double[] pnt, double vel, short acc, short cdir, short auto_flag);
        [DllImport(MMC_DLL_NAME)]
        public static extern short spl_auto_arc_deg_movenax(short len, short[] ax, double x_center, double y_center, double[] pnt, double vel, short acc, short cdir, short auto_flag);
        [DllImport(MMC_DLL_NAME)]
        public static extern short rect_move(short ax1, short ax2, double[] pnt, double vel, short acc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short spl_move_data(short spl_num, short len, short ax1, short ax2, short ax3, double[] pnt1, double[] pnt2, double[] pnt3, double vel, short acc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short spl_movex(short spl_num, short ax1, short ax2, short ax3);
        [DllImport(MMC_DLL_NAME)]
        public static extern short spl_move(short len, short ax1, short ax2, short ax3, double[] pnt1, double[] pnt2, double[] pnt3, double vel, short acc);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_spline_move_num(short bd_num, ref short num);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_sensor_auto_off(short ax, short off);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_sensor_auto_off(short ax, short off);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_sensor_auto_off(short ax, ref short off);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_sensor_auto_off(short ax, ref short off);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_servo_linear_flag(short ax, short l_flag);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_servo_linear_flag(short ax, short l_flag);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_servo_linear_flag(short ax, ref short l_flag);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_servo_linear_flag(short ax, ref short l_flag);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_mpg_velocity(short mpg_vel);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_mpg_velocity(short ax, ref short mpg_vel);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_mpg_enable(short ax, short state);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_mpg_enable(short ax, ref short state);
        [DllImport(MMC_DLL_NAME)]
        public static extern short in_sequence(short ax);
        [DllImport(MMC_DLL_NAME)]
        public static extern short in_motion(short ax);
        [DllImport(MMC_DLL_NAME)]
        public static extern short in_position(short ax);
        [DllImport(MMC_DLL_NAME)]
        public static extern short motion_done(short ax);
        [DllImport(MMC_DLL_NAME)]
        public static extern short axis_done(short ax);
        [DllImport(MMC_DLL_NAME)]
        public static extern short axis_state(short ax);
        [DllImport(MMC_DLL_NAME)]
        public static extern short axis_source(short ax);
        [DllImport(MMC_DLL_NAME)]
        public static extern long axis_sourcex(short ax);
        [DllImport(MMC_DLL_NAME)]
        public static extern short clear_status(short ax);
        [DllImport(MMC_DLL_NAME)]
        public static extern short frames_clear(short ax);
        [DllImport(MMC_DLL_NAME)]
        public static extern short frames_left(short ax);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_endless_rotationax(short ax, short status, short resolution);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_endless_rotationax(short ax, short status, short resolution);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_endless_rotationax(short ax, ref short status);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_endless_rotationax(short ax, ref short status);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_endless_linearax(short ax, short status, short resolution);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_endless_linearax(short ax, short status, short resolution);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_endless_linearax(short ax, ref short status);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_endless_linearax(short ax, ref short status);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_endless_range(short ax, double range);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_endless_range(short ax, double range);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_endless_range(short ax, ref double range);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_endless_range(short ax, ref double range);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_linear_all_stop_flag(short bd_num, short mode);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_linear_all_stop_flag(short bd_num, ref short mode);
        [DllImport(MMC_DLL_NAME)]
        public static extern short axis_all_status(short ax, [Out] short[] istatus, [Out] int[] lstatus, [Out] double[] dstatus);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_stop(short ax);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_stop_rate(short ax, short rate);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_stop_rate(short ax, short rate);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_stop_rate(short ax, ref short rate);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_stop_rate(short ax, ref short rate);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_e_stop(short ax);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_e_stop_rate(short ax, short rate);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fset_e_stop_rate(short ax, short rate);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_e_stop_rate(short ax, ref short rate);
        [DllImport(MMC_DLL_NAME)]
        public static extern short fget_e_stop_rate(short ax, ref short rate);
        //2.20 added for joystick funtion
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_joystick_enable(short bdNum, short Enable);
        [DllImport(MMC_DLL_NAME)]
        public static extern short set_joystick_velocity(short bdNum, short vel0, short vel1, short vel2, short vel3);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_joystick_enable(short bdNum, ref short Enable);
        [DllImport(MMC_DLL_NAME)]
        public static extern short get_joystick_velocity(short bdNum, ref short vel0, ref short vel1, ref short vel2, ref short vel3);

        #endregion

    }
}

namespace MMCWHPNET
{
    // MMC String class
    public class MMCString
    {
        // This string table is for option selection.
        #region MMC_string
        public string[] str_Event = 
                        { 
                            "No-Event", //0
                            "Stop",     //1
                            "E-Stop",   //2
                            "Abort"     //3
                        };
        public string[] str_OnOff = 
                        { 
                            "Off",      //0
                            "On"        //1
                        };
        public string[] str_TrueFalse = 
                        { 
                            "False",    //0
                            "True"      //1
                        };
        public string[] str_HighLow = 
                        { 
                            "Low",      //0
                            "High"      //1
                        };
        public string[] str_ActiveLevel = 
                        { 
                            "Low Active",       //0
                            "High Active"       //1
                        };
        public string[] str_Direction = 
                        { 
                            "Clockwise",        //0
                            "Counterclockwise"  //1
                        };
        public string[] str_Gains = 
                        { 
                            "Proportional gain",        //0
                            "Integral gain",            //1
                            "Derivative gain",          //2
                            "Velocity feed forward",    //3 
                            "Integration summing limit" //4 
                        };
        public string[] str_Sign = 
                        {   
                            "Negative",         //0
                            "Positive"          //1
                        };
        public string[] str_MotorType = 
                        { 
                            "Servo",            //0
                            "Stepper",          //1
                            "Micro Stepper"     //2
                        };
        public string[] str_Feedback = 
                        { 
                            "Encoder",          //0
                            "Analog",           //1
                            "Bi-Analog"         //2
                        };
        public string[] str_Polar = 
                        {
                            "Unipolar",         //0
                            "Bipolar"           //1
                        };
        public string[] str_EncoderType = 
                        { 
                            "Open",             //0
                            "Closed",           //1
                            "Semi"              //2
                        };
        public string[] str_ControlType = 
                        { 
                            "Velocity",         //0
                            "Torque"            //1
                        };
        public string[] str_ControlMode = 
                        { 
                            "Standing",         //0
                            "Always"            //1
                        };
        public string[] str_StepMode = 
                        { 
                            "CW and CCW",       //0
                            "Sign and Pulse"    //1
                        };
        public string[] str_Voltage = 
                        {
                            "5",                //0
                            "10"                //1
                        };
        public string[] str_SetReset = 
                        { 
                            "Reset",            //0
                            "Set"               //1
                        };
        public string[] str_Error = 
                        { 
                          "No Error",                                       //0
                          "Boot Memory has been corrupted",                 //1
                          "DPRAM Communication Error",                      //2
                          "Non Existent Axis",                              //3
                          "Illegal Analog Input Channel",                   //4
                          "Invalid I/O Port",                               //5
                          "Illegal Parameter",                              //6
                          "Not Define Map Axis",                            //7
                          "AMP Fault Occured",                              //8
                          "Motion is not completed",                        //9
                          "MMC Board is not exist",                         //10
                          "MMC Boot File Read/Write Error",                 //11
                          "MMC Checksum File Read/Write Error",             //12
                          "MMC Windows NT Driver Open Error",               //13
                          "Event Occured",                                  //14
                          "AMP Drive Power Off Status",                     //15
                          "MMC Data File Save Directory Open Error",        //16
                          "MMC Invalid CPMOTION Group",                     //17
                          "MMC Velocity Illigal Parameter",                 //18
                          "MMC Accel Illigal Parameter",                    //19
                          "MMC Invalid Error Code"                          //20
                        };
        public string[] str_Source =
                        {
                          "ST_NONE        : No problem",                      //0
                          "ST_HOME_SWITCH : Sensing Home Position Sensor",    //1
                          "ST_POS_LIMIT   : Sensing Positive Limit Sensor",   //2
                          "ST_NEG_LIMIT   : Sensing Negative Limit Sensor",   //3
                          "ST_AMP_FALUE   : AMP Fault",                       //4
                          "ST_A_LIMIT     : Acceleration Limit Over",         //5
                          "ST_V_LIMIT     : Velocity Limit Over",             //6
                          "ST_X_NEG_LIMIT : Negative Position Limit Over",    //7
                          "ST_X_POS_LIMIT : Positive Positino Limit Over",    //8
                          "ST_ERROR_LIMIT : Error Limit Over",                //9
                          "ST_PC_COMMAND  : Event occur (set_stop, set_e_stop, set_liner_all_stop)", //10
                          "ST_OUT_OF_FRAME: Frame Buffer is full",            //11
                          "ST_AMP_POWR_OFF: AMP is disabled",                 //12
                          "ST_ABS_COMM_ERR: ABS Encoder communication error", //13
                          "ST_INPOS_ST    : In Position detected",            //14
                          "ST_RUN_STOP_CMD: RUN STOP Error",                  //15
                          "ST_COLLISION_ST: Collision prevent Error"          //16
                        };
        #endregion
    }
}