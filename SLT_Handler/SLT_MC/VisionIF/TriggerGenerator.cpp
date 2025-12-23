#include "StdAfx.h"
#include "TriggerGenerator.h"

CTriggerGenerator::CTriggerGenerator()
{
	m_bConfig_2D_BarCode_Use = FALSE;
	m_nConfig_IO_Driver_Type = 0;
	m_dTblResolution = 0.0;
}

CTriggerGenerator::~CTriggerGenerator()
{
	DisConnect();
}

void CTriggerGenerator::SetTblResolution(double dTblResolution)
{
	m_dTblResolution = dTblResolution;
}

BOOL CTriggerGenerator::IsOpen()
{
	return serial_.IsOpen();
}

int CTriggerGenerator::Connect()
{
	m_bConfig_2D_BarCode_Use = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_2D_BarCode_Use);
	m_nConfig_IO_Driver_Type = g_IO.GetDriverType();
	
	if (m_nConfig_IO_Driver_Type  == _DIO_TYPE_SIMUL_ || m_bConfig_2D_BarCode_Use == FALSE)
	{
		return ERR_NO_ERROR;
	}

	if (serial_.IsOpen())
	{
		DisConnect();
	}

	// trigger generator com open
	char szDriverFileName[MAX_PATH] = { 0, };
	char szTemp[512] = { 0, };
	::GetCurrentDirectory(sizeof(szTemp), szTemp);
	sprintf_s(szDriverFileName, sizeof(szDriverFileName), "%s\\sys_data\\%s", szTemp, SZ_DRIVER_FILE_NAME);

	CConfigData ConfigData(szDriverFileName);
	int port_no = ConfigData.GetInt("TRIGGER CONFIG", "PORT NO", "0");

	CString port_no_str = _T("");
/*	int port_no = 11; //RS 232 Port*/
	port_no_str.Format(DEF_COMPORT, port_no);

	int nIdx = serial_.Open(port_no_str);

	if (!serial_.IsValid())
		return ERR_2DBARCODE_TRIGGER_CONNECT_FAIL;

	return ERR_NO_ERROR;
}

int CTriggerGenerator::DisConnect()
{
	if (m_nConfig_IO_Driver_Type == _DIO_TYPE_SIMUL_ || m_bConfig_2D_BarCode_Use == FALSE)
	{
		return ERR_NO_ERROR;
	}

	serial_.Purge();
	serial_.Close();

	return ERR_NO_ERROR;
}

int CTriggerGenerator::SetTriggerPos(int nTblIdx)
{
// 	BOOL is_use = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_2D_BarCode_Use);
// 	if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_ || is_use == FALSE)
// 		return ERR_NO_ERROR;

	if (serial_.IsOpen() == false)
		Connect();

	int ret = ERR_NO_ERROR;

	double trigger_pos0 = 0.0;
	double trigger_pos1 = 0.0;
	double trigger_interval = 0.0;
	int trigger_dir = 0;

// 	//Triger 설정 어떻게 되어 있는지 확인 필요.
// 	ASSERT(nTblIdx == eLoadTbl_2 || nTblIdx == eLoadTbl_3);

// trigger run
	UINT Ch1 = 0x01, Ch2 = 0x02, Ch3 = 0x04, ch4 = 0x08;
	serial_.SendWrite(0xF008, Ch1 | Ch2);
	//serial_.SendWrite(0xF008, Ch2 | Ch3);


	TRGCTRL trigger_ctrl;
	trigger_ctrl.Type = 0;			//0:DIFF, 1:TTL, 2:Virtual Encoder
	trigger_ctrl.Ch = 0;					//Encoder CH (0~3)
	trigger_ctrl.Multi = 4;					//Encoder Multi(1:x1, 2:x2, 4:x4)
	trigger_ctrl.CondFactor = 7;			//0:DI0 ~ 6:DI6, 7:High, 8:Low
	trigger_ctrl.OutputOperator = 0;		//0:AND, 1:OR, 2:XOR, 3:NAND
	trigger_ctrl.DirectionFactor = 0;		//0:DISABLE, 1:DI0 ~ 7:DI6, 8:CW, 9:CCW
	trigger_ctrl.PositionDirection = 1;		//0:DISABLE, 1:Positive, 2:Negative, 3:Bi-Direction
	trigger_ctrl.Correction = 0;			//0:DISABLE, 1:ENABLE
	trigger_ctrl.TriggerBase = 1;			//0:COUNT, 1:POSITION


	int ntriggerID;
	if (nTblIdx == eLoadTbl_2) {
		ntriggerID = 0;		//Trigger Ch
		trigger_ctrl.Ch = 1;		//Encoder Ch
	}
	else if (nTblIdx == eLoadTbl_3) {
		ntriggerID = 1;		//Trigger Ch
		trigger_ctrl.Ch = 2;     //Encoder Ch
	}

	serial_.SetTriggerControls(ntriggerID, trigger_ctrl);

	double dResolution = 4;
	GetTriggerInform(nTblIdx, trigger_pos0, trigger_pos1, trigger_interval, trigger_dir);
	serial_.SetTriggerPosition0(ntriggerID, trigger_pos0); //Start Pos
	serial_.SetTriggerPosition1(ntriggerID, trigger_pos1); //End Pos
	serial_.SetEncoderResolution(dResolution);
	serial_.SetEncoderDirection(ntriggerID, trigger_dir);
	serial_.SetTriggerGeneratorCycle(ntriggerID, (UINT)trigger_interval);


// 	GetTriggerInform(nTblIdx, trigger_pos0, trigger_pos1, trigger_interval, trigger_dir);
// 	serial_.SetTriggerPosition0(ntriggerID, trigger_pos0); //Start Pos
// 	serial_.SetTriggerPosition1(ntriggerID, trigger_pos1); //End Pos
// 	serial_.SetTriggerGeneratorCycle(ntriggerID, trigger_interval);
// 	serial_.SetEncoderDirection(ntriggerID, trigger_dir);

	serial_.SendWrite(0xF008, Ch1 | Ch2);

	//장비 Runt 시, Trigger Count 누적.
	BOOL bLotState = g_DMCont.m_dmEQP.GB(BDM0_SYS_LOT_STATUS);
	
	if (bLotState == eHandlerLotMode_OnGoing && g_TaskLoadTbl[nTblIdx].IsTriggerInitialize() == TRUE) {
		int nTriggerCnt = g_trigger_generator.GetTriggerCount(ntriggerID);
		g_pBarcode->AddTriggerTotalCount(ntriggerID, nTriggerCnt);
	}


	ClearTriggerCount(ntriggerID);

	serial_.SaveFlash();
	return ret;

// 	//loading table
// 	GetTriggerInform(1, trigger_pos0, trigger_pos1, trigger_interval, trigger_dir);
// 
// 	serial_.SetTriggerPosition0(0, trigger_pos0); //Start Pos
// 	serial_.SetTriggerPosition1(0, trigger_pos1); //End Pos
// 	serial_.SetTriggerGeneratorCycle(0, trigger_interval);
// 	serial_.SetEncoderDirection(0, trigger_dir);
// 
// 	GetTriggerInform(2, trigger_pos0, trigger_pos1, trigger_interval, trigger_dir);
// 
// 	serial_.SetTriggerPosition0(1, trigger_pos0);
// 	serial_.SetTriggerPosition1(1, trigger_pos1);
// 	serial_.SetTriggerGeneratorCycle(1, trigger_interval);
// 	serial_.SetEncoderDirection(1, trigger_dir);
// 
// 	serial_.SaveFlash();


}


int CTriggerGenerator::SetTriggerManulPos(int nTblIdx)
{
	if (serial_.IsOpen() == false)
		Connect();

	int ret = ERR_NO_ERROR;

	double trigger_pos0 = 0.0;
	double trigger_pos1 = 0.0;
	double trigger_interval = 0.0;
	int trigger_dir = 0;

	// trigger run
	UINT Ch1 = 0x01, Ch2 = 0x02, Ch3 = 0x04, ch4 = 0x08;
	serial_.SendWrite(0xF008, Ch1 | Ch2);
	//serial_.SendWrite(0xF008, Ch2 | Ch3);


	//Auto일 경우 Type은 0으로 고정.
	int nType = 0;
	int nCorrection = 0;

	if (g_TaskLoadTbl[nTblIdx].GetEqpCtrlMode() == eEqpCtrlMode_Manual) {
		nType = (nTblIdx == eLoadTbl_2) ? g_DMCont.m_dmShuttle.GN(NDM3_TRIGGER_BOARD_TYPE) : g_DMCont.m_dmShuttle.GN(NDM3_TRIGGER_BOARD_TYPE2);

		if (nType == 2)
			nCorrection = 1;
		else
			nCorrection = 0;
	}

	TRGCTRL trigger_ctrl;
	trigger_ctrl.Type = nType/*0*/;			//0:DIFF, 1:TTL, 2:Virtual Encoder
	trigger_ctrl.Ch = 0;					//Encoder CH (0~3)
	trigger_ctrl.Multi = 4;					//Encoder Multi(1:x1, 2:x2, 4:x4)
	trigger_ctrl.CondFactor = 7;			//0:DI0 ~ 6:DI6, 7:High, 8:Low
	trigger_ctrl.OutputOperator = 0;		//0:AND, 1:OR, 2:XOR, 3:NAND
	trigger_ctrl.DirectionFactor = 0;		//0:DISABLE, 1:DI0 ~ 7:DI6, 8:CW, 9:CCW
	trigger_ctrl.PositionDirection = 1;		//0:DISABLE, 1:Positive, 2:Negative, 3:Bi-Direction
	trigger_ctrl.Correction = nCorrection;			//0:DISABLE, 1:ENABLE
	trigger_ctrl.TriggerBase = 1;			//0:COUNT, 1:POSITION


	int ntriggerID;
	if (nTblIdx == eLoadTbl_2) {
		ntriggerID = 0;		//Trigger Ch
		trigger_ctrl.Ch = 1;		//Encoder Ch
	}
	else if (nTblIdx == eLoadTbl_3) {
		ntriggerID = 1;		//Trigger Ch
		trigger_ctrl.Ch = 2;     //Encoder Ch
	}

	serial_.SetTriggerControls(ntriggerID, trigger_ctrl);

	double dResolution = 4;
	GetTriggerInform(nTblIdx, trigger_pos0, trigger_pos1, trigger_interval, trigger_dir);
	serial_.SetTriggerPosition0(ntriggerID, trigger_pos0); //Start Pos
	serial_.SetTriggerPosition1(ntriggerID, trigger_pos1); //End Pos
	serial_.SetEncoderResolution(dResolution);
	serial_.SetEncoderDirection(ntriggerID, trigger_dir);
	serial_.SetTriggerGeneratorCycle(ntriggerID, (UINT)trigger_interval);


	//Board Type이 Virtual(2)만 해당.
	if (g_TaskLoadTbl[nTblIdx].GetEqpCtrlMode() == eEqpCtrlMode_Manual && nType == 2)
	{
		//if (nType == 2) //조건 항상 참
		//{
			int nFrequency = 0;
			if (nTblIdx == eLoadTbl_2) {
				nFrequency = g_DMCont.m_dmShuttle.GN(NDM3_TRIGGER_VIRTUAL_ENCODER_FREQUENCY);
			}
			else {
				nFrequency = g_DMCont.m_dmShuttle.GN(NDM3_TRIGGER_VIRTUAL_ENCODER_FREQUENCY2);
			}


			serial_.SetVirtualEncoderFrequency(trigger_ctrl.Ch/*ntriggerID*/, nFrequency);

		//}
//  		int nTest1 = 0;
//  		int nTest2 = 0;
//  		int nTest3 = 0;
//  		int nTest4 = 0;
//  
//  		int nfreq = serial_.GetVirtualEncoderFrequency(0, &nTest1);
//  		int nfreq1 = serial_.GetVirtualEncoderFrequency(1, &nTest2);
//  		int nfreq2 = serial_.GetVirtualEncoderFrequency(2, &nTest3);
//  		int nfreq3 = serial_.GetVirtualEncoderFrequency(3, &nTest4);
// 
// 		serial_.SetVirtualEncoderFrequency(1, nFrequency);
	}

// 	int ntest = 0;
// 	int nfreqStart = serial_.GetVirtualEncoderFrequency(0, &ntest);


// \
	//얘가 값 저장 하는 거라고?

 //	int nfreq = serial_.GetVirtualEncoderFrequency(0, &ntest);
// 	int nfreq1 = serial_.GetVirtualEncoderFrequency(1, ntest);
// 	int nfreq2 = serial_.GetVirtualEncoderFrequency(2, ntest);
// 	int nfreq3 = serial_.GetVirtualEncoderFrequency(3, ntest);

	//먼저 수행?
	//serial_.SaveFlash();

	serial_.SendWrite(0xF008, Ch1 | Ch2);

	ClearTriggerCount(ntriggerID);

	serial_.SaveFlash();
	return ret;
}

void CTriggerGenerator::SetStartTrigger(int nTableIdx)
{
	if (serial_.IsOpen() == false)
		Connect();

	if (!serial_.IsValid()) {
		int a = 0;
	}


	serial_.ClearTriggerCount(nTableIdx / 2); // Trigger 0, Trigger 1
	serial_.ClearEncoderPosition(nTableIdx / 2); // Encode Ch1, Ch2 아님, Trigger 0, Trigger 1

/*	serial_.SaveFlash();*/
}

void CTriggerGenerator::GetTriggerInform(int nTblIdx, double& trigger_pos0, double& trigger_pos1, double& trigger_interval, int& dir)
{
//	double dTblResolution = 5.599575;

	//1. trigger pos 0
	trigger_pos0 = g_DMCont.m_Teach.GetTeachPos(RID_LD_TBL_(nTblIdx), LOC_IDX_LD_TBL_TEST_SIDE_2D, POS_IDX_LD_TBL_Y);
	trigger_pos0 = trigger_pos0 / m_dTblResolution;

	//2. trigger pos 1
	//Rotate 이후 2D 촬영하기 때문에 X pitch로 계산
	int nLdTblPitchX = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Pitch_X);
	int nLdTblDivX = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);
	int nPos0 = g_DMCont.m_Teach.GetTeachPos(RID_LD_TBL_(nTblIdx), LOC_IDX_LD_TBL_TEST_SIDE_2D, POS_IDX_LD_TBL_Y);
	trigger_pos1 = nPos0 + (nLdTblPitchX*(nLdTblDivX - 1)) + 2000.0;
	trigger_pos1 = trigger_pos1 / m_dTblResolution;
	
	//3. trigger_interval(um).
	//trigger_interval = nLdTblPitchX;
	trigger_interval = nLdTblPitchX / m_dTblResolution;
// 	double dDistance_um = 0.0;
// 	double dSpeed_umpsec = 0.0;
// 	double dInterval_sec = 0.0;
// 
// 	//거리 = um
// 	dDistance_um = trigger_pos0 - trigger_pos1;
// 	
// 	//속력 = um/sec
// 	int nIndexSpd[4] = { NDM3_SPD_LoadTbl1_Y, NDM3_SPD_LoadTbl2_Y, NDM3_SPD_LoadTbl3_Y, NDM3_SPD_LoadTbl4_Y };
// 	double dRateSpd = (double)g_DMCont.m_dmShuttle.GN(nIndexSpd[nTblIdx]);
// 	double dMtSpd = g_TaskLoadTbl[nTblIdx].GetAxis(CTaskLoadTable::eAxisY)->GetSpeed();
// 	
// 	dSpeed_umpsec = dMtSpd * (dRateSpd / 100.0);
// 
// 	//시간 = sec
// 	dInterval_sec = dDistance_um / dSpeed_umpsec;
// 	//시간 단위 확인 후, 변경 필요.


	//4. dir
	dir = 0; // 0 : NORMAL , 1 : REVERSE
}

int CTriggerGenerator::GetTriggerCount(int nTriggerId)
{
 	if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_)
 		return 0;

	//실장비 검증 필요.
	int ntsetCnt = 0;

	UINT nCount;
	ntsetCnt = serial_.GetTriggerCount(nTriggerId, &nCount);

	// 	ULONG64 test;
	// 	serial_.GetTriggerCount64(nTriggerId, &test);


	return (int)nCount;
}

int CTriggerGenerator::GetTriggerCurPos(int nTriggerId)
{
	if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_)
		return 1;

	int nCurPosition = serial_.GetEncoderPosition(nTriggerId, 4, m_dTblResolution);
	return nCurPosition;
}

void CTriggerGenerator::ClearTriggerCount(int nTriggerId)
{
	serial_.ClearTriggerCount(nTriggerId);
}

void CTriggerGenerator::SetEndTrigger(bool _is_loader)
{
	
}