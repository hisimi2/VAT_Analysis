#include "stdafx.h"
#include "TaskTrayUnloadPP.h"


CTaskTrayUnloadPP::CTaskTrayUnloadPP()
{
	SetRobotIdIdx(eTrayPP_2_UD);

	NDM_TrayPP_RetryCnt       = NDM5_UDTrayPP_RetryCnt;
	NDM_TM_TrayPP_CylDownWait = NDM5_TM_UDTrayPP_CylDownWait;
	NDM_TM_TrayPP_Vacuum      = NDM5_TM_UDTrayPP_Vacuum;
	NDM_TM_TrayPP_Blow        = NDM5_TM_UDTrayPP_Blow;

	m_eMyDeviceStageNum       = eDeviceStage_TRAY_PP_2;
	
	char row[4] = { 'U','U','U','U' };
	m_TrayPP_picker_setting.add_row(row, 4);
	m_TrayPP_picker_setting.add_row(row, 4);

	ZeroMemory(m_nHand_i_VacSen, sizeof(m_nHand_i_VacSen));
	ZeroMemory(m_nHand_o_Vac, sizeof(m_nHand_o_Vac));
	ZeroMemory(m_nHand_o_Blw, sizeof(m_nHand_o_Blw));
	ZeroMemory(m_nHand_o_Pick, sizeof(m_nHand_o_Pick));
	ZeroMemory(m_nHand_o_Reset, sizeof(m_nHand_o_Reset));


	// DI
	m_mapDI.SetAt(static_cast<int>(eDI::xC_TRAY_HAND_R1_VACUUM_SEN), "C_TRAY_HAND_R1_VACUUM_SEN");
	m_mapDI.SetAt(static_cast<int>(eDI::xC_TRAY_HAND_R2_VACUUM_SEN), "C_TRAY_HAND_R2_VACUUM_SEN");
	m_mapDI.SetAt(static_cast<int>(eDI::xC_TRAY_HAND_R3_VACUUM_SEN), "C_TRAY_HAND_R3_VACUUM_SEN");
	m_mapDI.SetAt(static_cast<int>(eDI::xC_TRAY_HAND_R4_VACUUM_SEN), "C_TRAY_HAND_R4_VACUUM_SEN");
	m_mapDI.SetAt(static_cast<int>(eDI::xC_TRAY_HAND_F1_VACUUM_SEN), "C_TRAY_HAND_F1_VACUUM_SEN");
	m_mapDI.SetAt(static_cast<int>(eDI::xC_TRAY_HAND_F2_VACUUM_SEN), "C_TRAY_HAND_F2_VACUUM_SEN");
	m_mapDI.SetAt(static_cast<int>(eDI::xC_TRAY_HAND_F3_VACUUM_SEN), "C_TRAY_HAND_F3_VACUUM_SEN");
	m_mapDI.SetAt(static_cast<int>(eDI::xC_TRAY_HAND_F4_VACUUM_SEN), "C_TRAY_HAND_F4_VACUUM_SEN");
	m_mapDI.SetAt(static_cast<int>(eDI::xC_TRAY_HAND_Y_SAFETY_AREA_SEN),  "C_TRAY_HAND_Y_SAFETY_SEN");
	

	// DO
	m_mapDO.SetAt(static_cast<int>(eDO::yC_TRAY_HAND_R1_VACUUM_RESET), "C_TRAY_HAND_R1_VACUUM_RESET");
	m_mapDO.SetAt(static_cast<int>(eDO::yC_TRAY_HAND_R2_VACUUM_RESET), "C_TRAY_HAND_R2_VACUUM_RESET");
	m_mapDO.SetAt(static_cast<int>(eDO::yC_TRAY_HAND_R3_VACUUM_RESET), "C_TRAY_HAND_R3_VACUUM_RESET");
	m_mapDO.SetAt(static_cast<int>(eDO::yC_TRAY_HAND_R4_VACUUM_RESET), "C_TRAY_HAND_R4_VACUUM_RESET");
	m_mapDO.SetAt(static_cast<int>(eDO::yC_TRAY_HAND_F1_VACUUM_RESET), "C_TRAY_HAND_F1_VACUUM_RESET");
	m_mapDO.SetAt(static_cast<int>(eDO::yC_TRAY_HAND_F2_VACUUM_RESET), "C_TRAY_HAND_F2_VACUUM_RESET");
	m_mapDO.SetAt(static_cast<int>(eDO::yC_TRAY_HAND_F3_VACUUM_RESET), "C_TRAY_HAND_F3_VACUUM_RESET");
	m_mapDO.SetAt(static_cast<int>(eDO::yC_TRAY_HAND_F4_VACUUM_RESET), "C_TRAY_HAND_F4_VACUUM_RESET");

	m_mapDO.SetAt(static_cast<int>(eDO::yC_TRAY_HAND_R1_VACUUM_SET), "C_TRAY_HAND_R1_VACUUM_SET");
	m_mapDO.SetAt(static_cast<int>(eDO::yC_TRAY_HAND_R2_VACUUM_SET), "C_TRAY_HAND_R2_VACUUM_SET");
	m_mapDO.SetAt(static_cast<int>(eDO::yC_TRAY_HAND_R3_VACUUM_SET), "C_TRAY_HAND_R3_VACUUM_SET");
	m_mapDO.SetAt(static_cast<int>(eDO::yC_TRAY_HAND_R4_VACUUM_SET), "C_TRAY_HAND_R4_VACUUM_SET");
	m_mapDO.SetAt(static_cast<int>(eDO::yC_TRAY_HAND_F1_VACUUM_SET), "C_TRAY_HAND_F1_VACUUM_SET");
	m_mapDO.SetAt(static_cast<int>(eDO::yC_TRAY_HAND_F2_VACUUM_SET), "C_TRAY_HAND_F2_VACUUM_SET");
	m_mapDO.SetAt(static_cast<int>(eDO::yC_TRAY_HAND_F3_VACUUM_SET), "C_TRAY_HAND_F3_VACUUM_SET");
	m_mapDO.SetAt(static_cast<int>(eDO::yC_TRAY_HAND_F4_VACUUM_SET), "C_TRAY_HAND_F4_VACUUM_SET");

	m_mapDO.SetAt(static_cast<int>(eDO::yC_TRAY_HAND_R1_BLOW), "C_TRAY_HAND_R1_BLOW");
	m_mapDO.SetAt(static_cast<int>(eDO::yC_TRAY_HAND_R2_BLOW), "C_TRAY_HAND_R2_BLOW");
	m_mapDO.SetAt(static_cast<int>(eDO::yC_TRAY_HAND_R3_BLOW), "C_TRAY_HAND_R3_BLOW");
	m_mapDO.SetAt(static_cast<int>(eDO::yC_TRAY_HAND_R4_BLOW), "C_TRAY_HAND_R4_BLOW");
	m_mapDO.SetAt(static_cast<int>(eDO::yC_TRAY_HAND_F1_BLOW), "C_TRAY_HAND_F1_BLOW");
	m_mapDO.SetAt(static_cast<int>(eDO::yC_TRAY_HAND_F2_BLOW), "C_TRAY_HAND_F2_BLOW");
	m_mapDO.SetAt(static_cast<int>(eDO::yC_TRAY_HAND_F3_BLOW), "C_TRAY_HAND_F3_BLOW");
	m_mapDO.SetAt(static_cast<int>(eDO::yC_TRAY_HAND_F4_BLOW), "C_TRAY_HAND_F4_BLOW");

	m_mapDO.SetAt(static_cast<int>(eDO::yC_TRAY_HAND_VAT_CAM_UP), "C_TRAY_HAND_VAT_CAM_UP");
	m_mapDO.SetAt(static_cast<int>(eDO::yC_TRAY_HAND_VAT_CAM_DN), "C_TRAY_HAND_VAT_CAM_DOWN");

	// Motor
	m_mapMotor.SetAt(eAxisX, "Axis X");
	m_mapMotor.SetAt(eAxisY, "Axis Y");
	m_mapMotor.SetAt(eAxisZ, "Axis Z");
	m_mapMotor.SetAt(eAxisPitchX, "Axis Pitch X");
	m_mapMotor.SetAt(eAxisPitchY, "Axis Pitch Y");

	m_bInitialized = FALSE;
	m_bSkipToAssign = FALSE;

	m_vDevLossX.reserve(30);
	m_vDevLossY.reserve(30);

	m_vDevLossX.clear();
	m_vDevLossY.clear();

	//VAT
	m_hLog = NULL;	
}



int CTaskTrayUnloadPP::Initialize(int nTrayPPIdx, char* szDriverFileName)
{
	SetRobotIdIdx(nTrayPPIdx);

	CConfigData ConfigData(szDriverFileName);
	char szSection[128] = { 0, };

	// LOG File Name
	char szLogFileName[512] = { 0, };
	char szBuff[512] = { 0, };
	char szDefaultValue[128] = { 0, };

	memset(szSection, 0x00, sizeof(szSection));
	sprintf_s(szSection, sizeof(szSection), "TASK TRAY UNLOAD PP CONFIG");

	memset(szDefaultValue, 0x00, sizeof(szDefaultValue));
	sprintf_s(szDefaultValue, sizeof(szDefaultValue), "TaskTrayUnloadPP.log");

	ConfigData.GetString(szSection, "Log File Name", szDefaultValue, szBuff);
	sprintf_s(szLogFileName, sizeof(szLogFileName), "%s%s", SZ_LOG_FILE_PATH, szBuff);
	m_Logger.SetFileName(szLogFileName);

	// Digital Input
	memset(szSection, 0x00, sizeof(szSection));
	sprintf_s(szSection, sizeof(szSection), "TASK TRAY UNLOAD PP CONFIG - DI");
	int nDiCount = m_mapDI.GetCount();	
	for (int i = 0; i < nDiCount; i++) {
		char szKey[128] = { 0, };
		CString strName;
		m_mapDI.Lookup(i, strName);
		sprintf_s(szKey, sizeof(szKey), "%s", strName.GetString());
		int nIdx = ConfigData.GetInt(szSection, szKey, "0");
		m_vX.push_back(nIdx);
	}

	// Digital Output
	memset(szSection, 0x00, sizeof(szSection));
	sprintf_s(szSection, sizeof(szSection), "TASK TRAY UNLOAD PP CONFIG - DO");
	int nDoCount = m_mapDO.GetCount();
	for (int i = 0; i < nDoCount; i++) {
		char szKey[128] = { 0, };
		CString strName;
		m_mapDO.Lookup(i, strName);
		sprintf_s(szKey, sizeof(szKey), "%s", strName.GetString());
		int nIdx = ConfigData.GetInt(szSection, szKey, "0");
		m_vY.push_back(nIdx);
	}

	// Motor
	memset(szSection, 0x00, sizeof(szSection));
	sprintf_s(szSection, sizeof(szSection), "TASK TRAY UNLOAD PP CONFIG - MOTOR");
	int nMotorCount = m_mapMotor.GetCount();
	for (int i = 0; i < nMotorCount; i++) {
		char szKey[128] = { 0, };
		CString strName;
		m_mapMotor.Lookup(i, strName);
		sprintf_s(szKey, sizeof(szKey), "%s", strName.GetString());
		int nIdx = ConfigData.GetInt(szSection, szKey, "-1");
		if (nIdx != -1) {
			OneAxis* pMotor = g_Motor[nIdx];
			m_vMotor.push_back(pMotor);
		}
		else {
			m_vMotor.push_back(nullptr);
		}
	}

	for (int y = 0; y < MAX_TRAY_PP_CNT_Y; y++) {
		for (int x = 0; x < MAX_TRAY_PP_CNT_X; x++) {
			m_nHand_i_VacSen[y][x] = static_cast<CTaskTrayBasePP::eDI>((int)eDI::xC_TRAY_HAND_R1_VACUUM_SEN   + (y*MAX_TRAY_PP_CNT_X) + x);
			m_nHand_o_Vac   [y][x] = static_cast<CTaskTrayBasePP::eDO>((int)eDO::yC_TRAY_HAND_R1_VACUUM_RESET + (y*MAX_TRAY_PP_CNT_X) + x);
			m_nHand_o_Reset [y][x] = static_cast<CTaskTrayBasePP::eDO>((int)eDO::yC_TRAY_HAND_R1_VACUUM_SET + (y*MAX_TRAY_PP_CNT_X) + x);
			m_nHand_o_Blw   [y][x] = static_cast<CTaskTrayBasePP::eDO>((int)eDO::yC_TRAY_HAND_R1_BLOW         + (y*MAX_TRAY_PP_CNT_X) + x);
		}
	}

	m_nVATvisionLED_o_ONOFF[eUpperCam] = static_cast<CTaskTrayBasePP::eDO>((int)eDO::yC_TRAY_HAND_VAT_CAM_UP);
	m_nVATvisionLED_o_ONOFF[eDownCam]  = static_cast<CTaskTrayBasePP::eDO>((int)eDO::yC_TRAY_HAND_VAT_CAM_DN);

	//VAT
	for (int i = VAT_PICKER_01; i < VAT_PICKER_MAX; i++) {
		m_nBbxmnpAxis[i] = eBMAxis_UNLOAD_TRAY_Picker01 + i;
	}

	return CBaseTaskManager::Initialize();
}

void CTaskTrayUnloadPP::Finalize()
{
	CBaseTaskManager::Finalize();
}

int CTaskTrayUnloadPP::OnEvent(const CBaseTaskManager* pSender, int nEventId, CParamList* pDataList /*= NULL*/)
{
	return ERR_NO_ERROR;
}

void CTaskTrayUnloadPP::OnOperatorCommand(int nOperatorCommand, WPARAM wParam, LPARAM lParam)
{
	switch (nOperatorCommand)
	{
	case TWMB_ID_SKIP:
	case TWMB_ID_P_SKIP:
	{
		SetProcessCommand(base_seq_cmd::eProcCmd_Skip);
	}break;
	}
}

int CTaskTrayUnloadPP::WhereIsMotorPosXY(eLocIdxTrayPP& pos_xy)
{
	// get need data
	OneAxis* pMotorX = this->GetAxis(eAxisX);
	OneAxis* pMotorY = this->GetAxis(eAxisY);

	bool bXCheckSkip = (pMotorX == nullptr);
	bool bYCheckSkip = (pMotorY == nullptr);

	// get all setplate teaching y pos
	double dTechPos_X = 0.0;
	double dTechPos_Y = 0.0;
	double dMargine = 500.0; // 0.5mm

	double dCurPosX = 0, dCurPosY=0;

	pMotorX->GetCurrentPos(dCurPosX); 
	pMotorY->GetCurrentPos(dCurPosY);

	// loading table width x height
	double dLoadingTableDiv[2] = { 0.0, };
	dLoadingTableDiv[0] = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);
	dLoadingTableDiv[1] = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);

	double dLoadingTablePitch[2] = { 0.0, };
	dLoadingTablePitch[0] = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Pitch_X);
	dLoadingTablePitch[1] = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Pitch_Y);

	double dLdTblWidth = dLoadingTableDiv[0] * dLoadingTablePitch[0]; // um
	double dLdTblHeight = dLoadingTableDiv[1] * dLoadingTablePitch[1]; // um

	for (int i = 0; i < eMaxLoadTblCount; i++)
	{
		std::vector<CPoint> vCmdPicker;
		vCmdPicker.push_back(CPoint(0, 0));

		dTechPos_X = GetTeachPos(static_cast<eLocIdxTrayPP>(LOC_IDX_TRAY_PP_TBL_1 + i), CPoint(0, 0), vCmdPicker, eZPOS_SAFETY, eAxisX);
		dTechPos_Y = GetTeachPos(static_cast<eLocIdxTrayPP>(LOC_IDX_TRAY_PP_TBL_1 + i), CPoint(0, 0), vCmdPicker, eZPOS_SAFETY, eAxisY);

		bool bRangeIn_X = false;
		bool bRangeIn_Y = false;

		if ((dCurPosX >= (dTechPos_X - dMargine)) && (dCurPosX <= (dTechPos_X + dLdTblWidth + dMargine)))
		{
			bRangeIn_X = true;
		}
		if ((dCurPosY <= (dTechPos_Y + dMargine)) && (dCurPosX >= (dTechPos_Y - dLdTblHeight - dMargine)))
		{
			bRangeIn_Y = true;
		}

		if (bRangeIn_X && bRangeIn_Y)
		{
			pos_xy = static_cast<eLocIdxTrayPP>(LOC_IDX_TRAY_PP_TBL_1 + i);
			return ERR_NO_ERROR;
		}
	}

	// c-tray table width x height
	double dCTrayDiv[2] = { 0.0, };
	dCTrayDiv[0] = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_X);
	dCTrayDiv[1] = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_Y);

	double dCTrayPitch[2] = { 0.0, };
	dCTrayPitch[0] = g_DMCont.m_dmTrayPP.GN(NDM5_Tray_Pitch_X);
	dCTrayPitch[1] = g_DMCont.m_dmTrayPP.GN(NDM5_Tray_Pitch_Y);

	double dTrayWidth  = dCTrayDiv[0] * dCTrayPitch[0]; // um
	double dTrayHeight = dCTrayDiv[1] * dCTrayPitch[1]; // um	

	pos_xy = LOC_IDX_TRAY_PP_Undefined;


	// 현재 위치가 Margine을 포함한 C-Tray 크기 영역 안에 있는지 확인 한다.
	for (int i = 0; i < eMaxStackerCount; i++)
	{
		if(i < LOC_IDX_TRAY_PP_CONV_4)
			continue;

		std::vector<CPoint> vCmdPicker;
		vCmdPicker.push_back(CPoint(0, 0));

		dTechPos_X = GetTeachPos(static_cast<eLocIdxTrayPP>(LOC_IDX_TRAY_PP_CONV_1 + i), CPoint(0, 0), vCmdPicker, eZPOS_SAFETY, eAxisX);
		dTechPos_Y = GetTeachPos(static_cast<eLocIdxTrayPP>(LOC_IDX_TRAY_PP_CONV_1 + i), CPoint(0, 0), vCmdPicker, eZPOS_SAFETY, eAxisY);

		bool bRangeIn_X = false;
		bool bRangeIn_Y = false;

		if ((dCurPosX >= (dTechPos_X - dMargine)) && (dCurPosX <= (dTechPos_X + dTrayWidth + dMargine)))
		{
			bRangeIn_X = true;
		}
		if ((dCurPosY <= (dTechPos_Y + dMargine)) && (dCurPosX >= (dTechPos_Y - dTrayHeight - dMargine)))
		{
			bRangeIn_Y = true;
		}

		if (bRangeIn_X && bRangeIn_Y)
		{
			pos_xy = static_cast<eLocIdxTrayPP>(LOC_IDX_TRAY_PP_CONV_1 + i);
			return ERR_NO_ERROR;
		}
	}

	return ERR_NO_ERROR;
}

void CTaskTrayUnloadPP::MakeLog(LPCTSTR fmt, ...)
{
	TCHAR tmpMsg[1024] = { 0, };
	if (fmt)
	{
		va_list argList;
		va_start(argList, fmt);
		_vstprintf_s(tmpMsg, fmt, argList);
		va_end(argList);
	}
	TWLOG.MakeLog(static_cast<int>(eLogID_TrayPP_2), Debug, "", __LINE__, NULL, tmpMsg);
}

void CTaskTrayUnloadPP::MakeVatLog(LPCTSTR fmt, ...)
{
	TCHAR tmpMsg[1024] = { 0, };
	if (fmt)
	{
		va_list argList;
		va_start(argList, fmt);
		_vstprintf_s(tmpMsg, fmt, argList);
		va_end(argList);
	}
	TWLOG.MakeLog(static_cast<int>(eLogID_VAT_Tray_2), Debug, "", __LINE__, NULL, tmpMsg);
}

BOOL CTaskTrayUnloadPP::GetPickerAlignStatus(int nX, int nY)
{
	BOOL bAlignStatus = FALSE;

	std::vector<std::vector<int>> v = { 
		{ BDM11_PICKER_UD_X1_Y1, BDM11_PICKER_UD_X2_Y1, BDM11_PICKER_UD_X3_Y1, BDM11_PICKER_UD_X4_Y1 },
		{ BDM11_PICKER_UD_X1_Y2, BDM11_PICKER_UD_X2_Y2, BDM11_PICKER_UD_X3_Y2, BDM11_PICKER_UD_X4_Y2 } };

	bAlignStatus = v[nY][nX];

	return bAlignStatus;
}


void CTaskTrayUnloadPP::GetPickerOffset(double &dOffsetX, double &dOffsetY, int nX, int nY)
{
	std::vector<std::vector<CPoint>> v = {
		{
			CPoint(DDM11_PKR_UD_X0_Y0_OFFSETX , DDM11_PKR_UD_X0_Y0_OFFSETY),
			CPoint(DDM11_PKR_UD_X1_Y0_OFFSETX , DDM11_PKR_UD_X1_Y0_OFFSETY),
			CPoint(DDM11_PKR_UD_X2_Y0_OFFSETX , DDM11_PKR_UD_X2_Y0_OFFSETY),
			CPoint(DDM11_PKR_UD_X3_Y0_OFFSETX , DDM11_PKR_UD_X3_Y0_OFFSETY)
		},
		{
			CPoint(DDM11_PKR_UD_X0_Y1_OFFSETX , DDM11_PKR_UD_X0_Y1_OFFSETY),
			CPoint(DDM11_PKR_UD_X1_Y1_OFFSETX , DDM11_PKR_UD_X1_Y1_OFFSETY),
			CPoint(DDM11_PKR_UD_X2_Y1_OFFSETX , DDM11_PKR_UD_X2_Y1_OFFSETY),
			CPoint(DDM11_PKR_UD_X3_Y1_OFFSETX , DDM11_PKR_UD_X3_Y1_OFFSETY)
		} };

	int x = v[nY][nX].x;
	int y = v[nY][nX].y;

	dOffsetX = g_DMCont.m_dmHandlerOpt.GD(x);
	dOffsetY = g_DMCont.m_dmHandlerOpt.GD(y);
}

void CTaskTrayUnloadPP::ThreadProc1()
{
	SetThreadInterval(eTHREAD_1, 50);
	while (GetThreadAliveFlag(eTHREAD_1))
	{
		if (GetPauseFlag(eTHREAD_1))
		{
			NEXT;
			continue;
		}

		for (int i = 0; i < MAX_THREAD_SCAN_COUNT; i++)
		{
			CBaseTaskManager::SeqCmdRunning();
		}
		NEXT;
	}
}

int CTaskTrayUnloadPP::ChkDI(eDI nIndex, eOnOffState bValue /*= eOn*/)
{
	return CTaskTrayBasePP::ChkDI(static_cast<CTaskTrayBasePP::eDI>(nIndex), bValue);
}

eOnOffState CTaskTrayUnloadPP::GetDI(eDI nIndex)
{
	return CTaskTrayBasePP::GetDI(static_cast<CTaskTrayBasePP::eDI>(nIndex));
}

int CTaskTrayUnloadPP::SetDO(eDO nIndex, eOnOffState eOnOff)
{
	return CTaskTrayBasePP::SetDO(static_cast<CTaskTrayBasePP::eDO>(nIndex), eOnOff);
}

bool CTaskTrayUnloadPP::is_safety_state()
{
	bool bSafetyPos = false;

	bool bSafetySensor = this->get_TrayUnload_Y_Safety_sen(DEF_ON) == TRUE;

	OneAxis* pMotorY = g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisY);
	BOOL bOrigin = pMotorY->IsOrgCompleted(nullptr);

	OneAxis* pMotorPitchY = g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisPitchY);

	if (bOrigin == TRUE) {

		double dLoadTablePitchY = (double)g_DMCont.m_dmTrayPP.GN(NDM5_TrayPP_LdTbl_Pitch_Y);

		double dCurPos_um = 0.0, dCurPitchY_um = 0.0;
		pMotorY->GetCurrentPos(dCurPos_um);
 		pMotorPitchY->GetCurrentPos(dCurPitchY_um);
 
		double dPosiLimitPitchY = pMotorPitchY->GetSwLimitPosi();

		double dWorkEnableMin_um = MIN_TRAY_PP_WORK_ENABLE_POS_um - (DEF_TRAY_PP_MAX_Y_PITCH_WIDE - dLoadTablePitchY - 10000.0);
		double dWorkEnablePitchY = dPosiLimitPitchY - (60000.0 - dLoadTablePitchY);
		if ((dWorkEnableMin_um < dCurPos_um && dCurPitchY_um < dWorkEnablePitchY) || MIN_TRAY_PP_WORK_ENABLE_POS_um < dCurPos_um) {
			bSafetyPos = true;
		}
	}

	if (bSafetySensor == true && bSafetyPos == true) {
		return true;
	}

	return false;
}

void CTaskTrayUnloadPP::ApplyVATOffset()
{

}

int CTaskTrayUnloadPP::chk_TrayUnload_Y_Safety_sen(BOOL bOnOff)
{
	eOnOffState sefty_y_on = bOnOff == DEF_ON ? eOn : eOff;

	return ChkDI(eDI::xC_TRAY_HAND_Y_SAFETY_AREA_SEN, sefty_y_on);
}

int CTaskTrayUnloadPP::get_TrayUnload_Y_Safety_sen(BOOL bOnOff)
{
	eOnOffState sefty_y_on = bOnOff == DEF_ON ? eOn : eOff;

	return GetDI(eDI::xC_TRAY_HAND_Y_SAFETY_AREA_SEN );
}

void CTaskTrayUnloadPP::ApplyBacklash()
{
	for (int i = eSpeed_5; i < eSpeed_MAX; i++) {
		g_Motor[eAXIS_TRAY_PP_2_X]->SetSpeedBacklash(i, g_VatBacklash.trayunloadpp.m_BacklashOffset_XY[i].x);
		g_Motor[eAXIS_TRAY_PP_2_Y]->SetSpeedBacklash(i, g_VatBacklash.trayunloadpp.m_BacklashOffset_XY[i].y);
	}
}

void CTaskTrayUnloadPP::MakeVatPreparePos(double & dX, double & dY, double dPrepareOffset)
{
	double dNegaLimitX = m_vMotor[CTaskTrayUnloadPP::eAxisX]->GetSwLimitNega();
	double dNegaLimitY = m_vMotor[CTaskTrayUnloadPP::eAxisY]->GetSwLimitNega();

	double dPreparePosX = dX - dPrepareOffset;
	double dPreparePosY = dY - dPrepareOffset;

	dX = (dPreparePosX <= dNegaLimitX) ? (dNegaLimitX) : (dPreparePosX);
	dY = (dPreparePosY <= dNegaLimitY) ? (dNegaLimitY) : (dPreparePosY);
}

void CTaskTrayUnloadPP::SetVatLogHandle(HWND hOwner)
{
	m_hLog = hOwner;
}

void CTaskTrayUnloadPP::WriteDisplayLog(const char * fmt, ...)
{
	char szBuff[MAX_BUFF] = { 0, };

	va_list va;
	va_start(va, fmt);
	vsprintf(szBuff, fmt, va);
	va_end(va);

	::SendMessage(m_hLog, UM_VAT_SEQ_MSG_PRINT_LOG, (WPARAM)VAT_HAND_UNLOAD_TRAYPP, (LPARAM)szBuff);
}