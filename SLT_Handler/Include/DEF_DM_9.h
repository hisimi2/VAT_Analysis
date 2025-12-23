#pragma once

#define SZ_DM_NAME_9		_T("DM_TRANSFER")

#define NDM9_HEAD_DISTANCE				      (0x000)


#define NDM9_SPD_Transfer_X		   (1)	// NDM11_SPD_Transfer_X
#define NDM9_ACC_Transfer_X		   (2)	// NDM11_ACC_Transfer_X
#define NDM9_DCC_Transfer_X		   (3)	// NDM11_DCC_Transfer_X

#define NDM9_Transfer_CylDownWait   (10)

#define NDM9_Transfer_source_conveyor_index			(20)
#define NDM9_Transfer_destination_conveyor_index	(21)

//============BOOL
#define BDM9_SLT_USE_TRASNFER_SEN_EXIST						(0x000)
#define BDM9_SLT_USE_TRASNFER_AREA_SEN_EXIST				(0x001)