#pragma once

#define SZ_DM_NAME_5		_T("DM_TRAY_PP")

#define NDM5_PICKER_COUNT_X            (0x000)
#define NDM5_PICKER_COUNT_Y            (0x001)
#define NDM5_HAND_PITCH_MIN_X          (0x002)
#define NDM5_HAND_PITCH_MIN_Y          (0x003)
#define NDM5_HAND_PITCH_MAX_X          (0x004)
#define NDM5_HAND_PITCH_MAX_Y          (0x005)


#define NDM5_TrayPP_Buffer_Pitch_X	   (0x010)
#define NDM5_TrayPP_Buffer_Pitch_Y	   (0x011)

#define NDM5_SPD_TrayPP_1_X			       (0x50)	// NDM11_SPD_TrayPP_X		
#define NDM5_ACC_TrayPP_1_X			       (0x51)	// NDM11_ACC_TrayPP_X		
#define NDM5_DCC_TrayPP_1_X			       (0x52)	// NDM11_DCC_TrayPP_X		
#define NDM5_SPD_TrayPP_1_Y			       (0x53)	// NDM11_SPD_TrayPP_Y		
#define NDM5_ACC_TrayPP_1_Y			       (0x54)	// NDM11_ACC_TrayPP_Y		
#define NDM5_DCC_TrayPP_1_Y			       (0x55)	// NDM11_DCC_TrayPP_Y		
#define NDM5_SPD_TrayPP_1_Z			       (0x56)	// NDM11_SPD_TrayPP_Z		
#define NDM5_ACC_TrayPP_1_Z			       (0x57)	// NDM11_ACC_TrayPP_Z		
#define NDM5_DCC_TrayPP_1_Z			       (0x58)	// NDM11_DCC_TrayPP_Z		
#define NDM5_SPD_TrayPP_1_X_Pitch		   (0x59)	// NDM11_SPD_TrayPP_X_Pitch
#define NDM5_ACC_TrayPP_1_X_Pitch		   (0x5A)	// NDM11_ACC_TrayPP_X_Pitch
#define NDM5_DCC_TrayPP_1_X_Pitch		   (0x5B)	// NDM11_DCC_TrayPP_X_Pitch
#define NDM5_SPD_TrayPP_1_Y_Pitch		   (0x5C)	// NDM11_SPD_TrayPP_Y_Pitch
#define NDM5_ACC_TrayPP_1_Y_Pitch		   (0x5D)	// NDM11_ACC_TrayPP_Y_Pitch
#define NDM5_DCC_TrayPP_1_Y_Pitch		   (0x5E)	// NDM11_DCC_TrayPP_Y_Pitch

#define NDM5_SPD_TrayPP_2_X				   (0x60)	// NDM11_SPD_TrayPP_X		
#define NDM5_ACC_TrayPP_2_X				   (0x61)	// NDM11_ACC_TrayPP_X		
#define NDM5_DCC_TrayPP_2_X				   (0x62)	// NDM11_DCC_TrayPP_X		
#define NDM5_SPD_TrayPP_2_Y				   (0x63)	// NDM11_SPD_TrayPP_Y		
#define NDM5_ACC_TrayPP_2_Y				   (0x64)	// NDM11_ACC_TrayPP_Y		
#define NDM5_DCC_TrayPP_2_Y				   (0x65)	// NDM11_DCC_TrayPP_Y		
#define NDM5_SPD_TrayPP_2_Z				   (0x66)	// NDM11_SPD_TrayPP_Z		
#define NDM5_ACC_TrayPP_2_Z				   (0x67)	// NDM11_ACC_TrayPP_Z		
#define NDM5_DCC_TrayPP_2_Z				   (0x68)	// NDM11_DCC_TrayPP_Z		
#define NDM5_SPD_TrayPP_2_X_Pitch		   (0x69)	// NDM11_SPD_TrayPP_X_Pitch
#define NDM5_ACC_TrayPP_2_X_Pitch		   (0x6A)	// NDM11_ACC_TrayPP_X_Pitch
#define NDM5_DCC_TrayPP_2_X_Pitch		   (0x6B)	// NDM11_DCC_TrayPP_X_Pitch
#define NDM5_SPD_TrayPP_2_Y_Pitch		   (0x6C)	// NDM11_SPD_TrayPP_Y_Pitch
#define NDM5_ACC_TrayPP_2_Y_Pitch		   (0x6D)	// NDM11_ACC_TrayPP_Y_Pitch
#define NDM5_DCC_TrayPP_2_Y_Pitch		   (0x6E)	// NDM11_DCC_TrayPP_Y_Pitch



//Load Tray PP
#define NDM5_SPD_LD_TrayPP_X			   (0x100)	// NDM11_SPD_TrayPP_X		
#define NDM5_ACC_LD_TrayPP_X			   (0x101)	// NDM11_ACC_TrayPP_X		
#define NDM5_DCC_LD_TrayPP_X			   (0x102)	// NDM11_DCC_TrayPP_X		
#define NDM5_SPD_Gripper_Y				   (0x103)	// NDM11_SPD_TrayPP_Y		
#define NDM5_ACC_Gripper_Y			       (0x104)	// NDM11_ACC_TrayPP_Y		
#define NDM5_DCC_Gripper_Y			       (0x105)	// NDM11_DCC_TrayPP_Y		
#define NDM5_SPD_LD_TrayPP_Z			   (0x106)	// NDM11_SPD_TrayPP_Z		
#define NDM5_ACC_LD_TrayPP_Z			   (0x107)	// NDM11_ACC_TrayPP_Z		
#define NDM5_DCC_LD_TrayPP_Z			   (0x108)	// NDM11_DCC_TrayPP_Z		
#define NDM5_SPD_LD_TrayPP_X_Pitch		   (0x109)	// NDM11_SPD_TrayPP_X_Pitch
#define NDM5_ACC_LD_TrayPP_X_Pitch		   (0x10A)	// NDM11_ACC_TrayPP_X_Pitch
#define NDM5_DCC_LD_TrayPP_X_Pitch		   (0x10B)	// NDM11_DCC_TrayPP_X_Pitch
#define NDM5_SPD_LD_TrayPP_Y_Pitch		   (0x10C)	// NDM11_SPD_TrayPP_Y_Pitch
#define NDM5_ACC_LD_TrayPP_Y_Pitch		   (0x10D)	// NDM11_ACC_TrayPP_Y_Pitch
#define NDM5_DCC_LD_TrayPP_Y_Pitch		   (0x10E)	// NDM11_DCC_TrayPP_Y_Pitch

//Unload Tray PP
#define NDM5_SPD_UL_TrayPP_X			   (0x110)	// NDM11_SPD_TrayPP_X		
#define NDM5_ACC_UL_TrayPP_X			   (0x111)	// NDM11_ACC_TrayPP_X		
#define NDM5_DCC_UL_TrayPP_X			   (0x112)	// NDM11_DCC_TrayPP_X		
#define NDM5_SPD_UL_TrayPP_Y			   (0x113)	// NDM11_SPD_TrayPP_Y		
#define NDM5_ACC_UL_TrayPP_Y			   (0x114)	// NDM11_ACC_TrayPP_Y		
#define NDM5_DCC_UL_TrayPP_Y			   (0x115)	// NDM11_DCC_TrayPP_Y		
#define NDM5_SPD_UL_TrayPP_Z			   (0x116)	// NDM11_SPD_TrayPP_Z		
#define NDM5_ACC_UL_TrayPP_Z			   (0x117)	// NDM11_ACC_TrayPP_Z		
#define NDM5_DCC_UL_TrayPP_Z			   (0x118)	// NDM11_DCC_TrayPP_Z		
#define NDM5_SPD_UL_TrayPP_X_Pitch		   (0x119)	// NDM11_SPD_TrayPP_X_Pitch
#define NDM5_ACC_UL_TrayPP_X_Pitch		   (0x11A)	// NDM11_ACC_TrayPP_X_Pitch
#define NDM5_DCC_UL_TrayPP_X_Pitch		   (0x11B)	// NDM11_DCC_TrayPP_X_Pitch
#define NDM5_SPD_UL_TrayPP_Y_Pitch		   (0x11C)	// NDM11_SPD_TrayPP_Y_Pitch
#define NDM5_ACC_UL_TrayPP_Y_Pitch		   (0x11D)	// NDM11_ACC_TrayPP_Y_Pitch
#define NDM5_DCC_UL_TrayPP_Y_Pitch		   (0x11E)	// NDM11_DCC_TrayPP_Y_Pitch
		
#define NDM5_TrayDiv_X				   (0x120)	// NDM11_TrayDiv_X		
#define NDM5_TrayDiv_Y				   (0x121)	// NDM11_TrayDiv_Y		
#define NDM5_Tray_Start_X			   (0x122)	// NDM11_Tray_Start_X	
#define NDM5_Tray_Start_Y			   (0x123)	// NDM11_Tray_Start_Y	
#define NDM5_Tray_Pitch_X			   (0x124)	// NDM11_Tray_Pitch_X	
#define NDM5_Tray_Pitch_Y			   (0x125)	// NDM11_Tray_Pitch_Y	
#define NDM5_Tray_Thickness			   (0x126)	// NDM11_Tray_Thickness

#define NDM5_TrayPP_LdTbl_Pitch_X	   (0x127)	// NDM11_TrayPP_LdTbl_Pitch_X
#define NDM5_TrayPP_LdTbl_Pitch_Y	   (0x128)	// NDM11_TrayPP_LdTbl_Pitch_Y

#define NDM5_LDTrayPP_RetryCnt		   (0x129)	// NDM11_TrayPP_RetryCnt
#define NDM5_UDTrayPP_RetryCnt		   (0x12A)	// NDM11_TrayPP_RetryCnt

#define NDM5_TM_UDTrayPP_CylDownWait     (0x12B)	// NDM11_TM_TrayPP_CylDownWait
#define NDM5_TM_LDTrayPP_CylDownWait     (0x12C)	// NDM11_TM_TrayPP_CylDownWait

#define NDM5_TM_LDTrayPP_Vacuum		   (0x12D)	// NDM11_TM_TrayPP_Vacuum
#define NDM5_TM_UDTrayPP_Vacuum		   (0x12E)	// NDM11_TM_TrayPP_Vacuum

#define NDM5_TM_LDTrayPP_Blow			 (0x12F)	// NDM11_TM_TrayPP_Blow
#define NDM5_TM_UDTrayPP_Blow			 (0x130)	// NDM11_TM_TrayPP_Blow

#define NDM5_TM_TrayPP_Tray_End		(0x131)	// NDM11_TM_TrayPP_Tray_End

#define NDM5_TM_LDTrayPP_Pick_1Stage_Offset     (0x132)	// NDM5_TM_LDTrayPP_Pick_1Stage_Offset
#define NDM5_TM_LDTrayPP_Pick_1Stage_Speed      (0x133)	// NDM5_TM_LDTrayPP_Pick_1Stage_Speed

#define NDM5_TM_UDTrayPP_Linear_Offset			 (0x13A)	// NDM5_TM_UDTrayPP_Linear_Offset

#define NDM5_TM_LDTrayPP_Remind_Pick_Count      (0x140)	// NDM5_TM_LDTrayPP_Remind_Pick_Count


	









