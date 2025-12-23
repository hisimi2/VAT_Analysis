#pragma once

#define SZ_DM_NAME_10		_T("DM_FEEDER")

//============================================================ BOOL
#define BDM10_SLT_USE_FEEDER_SEN_EXIST							(0x000)
#define BDM10_FEEDER_TRAY_END									(0x001)

//============================================================ NUMERIC
//Feeder
#define NDM10_SPD_Feeder_Y				    (0x000)	// NDM11_SPD_TrayPP_X		
#define NDM10_ACC_Feeder_Y				    (0x001)	// NDM11_ACC_TrayPP_X		
#define NDM10_DCC_Feeder_Y				    (0x002)	// NDM11_DCC_TrayPP_X	
#define NDM10_FEEDER_UPDN_WAITTIME			(0x003)
#define NDM10_FEEDER_CLAMP_WAITTIME			(0x004)
#define NDM10_FEEDER_ALIGN_WAITTIME			(0x005)

//============================================================ DOUBLE
#define DDM10_FEEDER_STEP_OFFSET			(0x000)

//============================================================ STRING
// #define SDM13_2DID_STRING_LIST1						(0x000)
/*#define SDM13_2DID_STRING_LIST1024					(0x400)*/