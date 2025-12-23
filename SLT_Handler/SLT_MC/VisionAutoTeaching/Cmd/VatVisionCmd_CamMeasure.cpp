#include "stdafx.h"
#include "VatVisionCmd_CamMeasure.h"
#include <unordered_map>

CVatVisionCmd_CamMeasure::CVatVisionCmd_CamMeasure(int _nHand) : m_nHand(_nHand)
{
	StringCchCopy(m_szCmdName, _countof(m_szCmdName), CString(__func__));

	//m_nVisionPC = -1;
	//m_nDataId = -1;
	m_vDataId.clear();
	m_nStatus = -1;
}


CVatVisionCmd_CamMeasure::~CVatVisionCmd_CamMeasure()
{

}


int CVatVisionCmd_CamMeasure::CheckParam()
{
// 	//nDataId
// 	for (auto nDataId : m_vDataId) {
// 		ASSERT((VISION_DATAID_VAT_LD_UPPER <= nDataId && nDataId <= VISION_DATAID_VAT_TEST_1_LOWER) ||
// 			(VISION_DATAID_VAT_TEST_2_LD_UPPER <= nDataId && nDataId <= VISION_DATAID_VAT_TEST_2_LOWER));
// 	}
// 
// 	//nStatus
// 	ASSERT((VISION_STATUS_PLATE_JIG <= m_nStatus && m_nStatus <= VISION_STATUS_SOCKET) || m_nStatus == VISION_STATUS_AUTO_FOCUS);
// 
// 	//nHand
// 	ASSERT(VAT_HAND_LOAD_TRAYPP <= m_nHand && m_nHand <= VAT_HAND_UNLOAD_TESTPP2);


// 	if (VISION_DATAID_VAT_LD_UPPER <= m_nDataId && m_nDataId <= VISION_DATAID_VAT_TEST_1_LOWER) {
// 		m_nVisionPC = VAT_VISION_PC_1;
// 	}
// 	else {
// 		m_nVisionPC = VAT_VISION_PC_2;
// 	}

	m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_MIDDLE);
	m_tcTimeout.StartTimer();

	return ERR_NO_ERROR;
}


int CVatVisionCmd_CamMeasure::OnStop()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Aborting);
	return ERR_NO_ERROR;
}


int CVatVisionCmd_CamMeasure::OnRetry()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}


int CVatVisionCmd_CamMeasure::OnSkip()
{
	ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_AlarmComplete);
	return ERR_NO_ERROR;
}


int CVatVisionCmd_CamMeasure::OnPause()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Pause);
	return ERR_NO_ERROR;
}


int CVatVisionCmd_CamMeasure::OnResume()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}


BOOL CVatVisionCmd_CamMeasure::State_Setup()
{
	return CBaseSeqCmd::State_Setup();
}


BOOL CVatVisionCmd_CamMeasure::State_Executing()
{
	CBaseTaskManager* pTask = (CBaseTaskManager*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTask->MakeVatLog(_T("[CMD: %s] [EXECUTE] [Step=%s]"), m_szCmdName, act_string(m_nStep));
	}
	m_nStep_Pre = m_nStep;

	m_nLast_Executing_Step = m_nStep;
	switch (m_nStep)
	{
	case vision_target_measure_start:
	{
		m_nStep = vision_target_send_measure;
	}break;
	case vision_target_send_measure:
	{
		for (auto nDataId : m_vDataId) {
			int nCamNo = ConvertDataIdToVatCam(nDataId);

			if (m_simulCtrl._IsSimul()) {
				g_VatVisionMgr[nCamNo].SetReceiveState(VAT_RCV_STATUS_IDLE);
			}
			else {
				//Vision Measure
				g_VatVisionMgr[nCamNo].SendMeasure(nDataId, m_nStatus);
			}

		}

		m_tcTimeout.SetCheckTime(TIME_OUT_MIDDLE * 2);
		m_tcTimeout.ResetStartTime();

		m_nStep = vision_target_receive_wait;

		//[Simulation]
		if (m_simulCtrl._IsSimul()) {
			//SendMeasure 이후, Simul Delay Time 조절
			int nWaitTime = (m_nStatus == VISION_STATUS_AUTO_FOCUS) ? 6000 : 3000;
			m_simulCtrl._WaitSeqControl(nWaitTime);
			

			//Cam Rcv 상태 Success로 변경.
			for (auto nDataId : m_vDataId) {
				int nCamNo = ConvertDataIdToVatCam(nDataId);
				g_VatVisionMgr[nCamNo].SetReceiveState(VAT_RCV_STATUS_SUCCESS);
			}
		}
	}break;
	case vision_target_receive_wait:
	{
		//[1]Time Out
		//  [9/22/2025 dohyeong.kim]Time Out일 경우, SetReceiveState Error로 표기 고려하기
		if (m_tcTimeout.CheckOverTime()) {
			CString strErrCamNo = _T("");
			for (auto nDataId : m_vDataId) {
				int nCamNo = ConvertDataIdToVatCam(nDataId);
				int nRcvStatus = g_VatVisionMgr[nCamNo].GetReceiveState();
				if (nRcvStatus == VAT_RCV_STATUS_IDLE) {
					strErrCamNo += ConvertCamIdToString(nCamNo);
					strErrCamNo += _T(",");
				}
			}
	
			CString strStatus = ConvertStatusToString(m_nStatus);

			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("[%s] [Cam : %s] ", strStatus, strErrCamNo);			
			ReportAlarm(ERR_VAT_VISION_TIME_OUT, AlarmData);
			break;
		}

		//[2]Rcv Check
		int nRcvDone = 0;
		BOOL bDisconnect = FALSE;

		for (auto nDataId : m_vDataId) {
			int nCamNo = ConvertDataIdToVatCam(nDataId);
			int nRcvStatus = g_VatVisionMgr[nCamNo].GetReceiveState();

			//Status 4개 - Success/Error/Disconnect/Idle
			if (nRcvStatus == VAT_RCV_STATUS_DISCONNECT) {		
				CString strCamNo = ConvertCamIdToString(nCamNo);
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("[Cam : %s]", strCamNo);
				ReportAlarm(ERR_VAT_VISION_DISCONNECT, AlarmData);
				bDisconnect = TRUE;
				break;
			}

			if (nRcvStatus != VAT_RCV_STATUS_IDLE) {
				nRcvDone++;
			}
		}

		if (nRcvDone == m_vDataId.size()) {
			m_nStep = vision_target_receive_error_chk;
		}
	}break;
	/*
	Vision Rcv Status 'Success' or 'Error'
	*/
	case vision_target_receive_error_chk:
	{
		int nErrCnt = 0;

		for (auto nDataId : m_vDataId) {
			int nCamNo = ConvertDataIdToVatCam(nDataId);
			int nRcvStatus = g_VatVisionMgr[nCamNo].GetReceiveState();

			//ASSERT(nRcvStatus == VAT_RCV_STATUS_SUCCESS || nRcvStatus == VAT_RCV_STATUS_ERROR);

			if (nRcvStatus == VAT_RCV_STATUS_ERROR)
			{
				nErrCnt++;

				CString strCamNo = ConvertCamIdToString(nCamNo);
				CString strStatus = ConvertStatusToString(m_nStatus);

				ST_VAT_RCV_DATA stRcvData;
				g_VatVisionMgr[nCamNo].GetReceiveData(stRcvData);

				int nErr = ConvertVisionErrIdxToAlarmIdx(stRcvData.nError);

				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("[%s] [%s] ", strStatus, strCamNo);
				ReportAlarm(nErr, AlarmData);
			}

			//Simulation 처리.
			m_simulCtrl._SetVisionRcvData(nCamNo, m_nStatus);
		}

		if (nErrCnt > 0) {
			break;
		}

		m_nStep = vision_data_save;		
	}break;
	case vision_data_save:
	{
		for (auto nDataId : m_vDataId) {
			SaveMeasureData(m_nHand, nDataId, m_nStatus);

			int nCamNo = ConvertDataIdToVatCam(nDataId);

			m_simulCtrl._WaitSeqControl(2000);

			g_VatVisionMgr[nCamNo].SetReceiveState(VAT_RCV_STATUS_INIT); //상태 초기화
		}

		m_nStep = vision_target_measure_end;
	}break;
	case vision_target_measure_end:
	{
		ReportCmdEnd(eSeqCmdStatus_NormalComplete);
	}break;
	default:
	{
		m_nStep = 0;
	}break;
	}

	return CBaseSeqCmd::State_Executing();
}


// BOOL CVatVisionCmd_CamMeasure::State_Executing()
// {
// 	CBaseTaskManager* pTask = (CBaseTaskManager*)m_pReceive;
// 	if (m_nStep_New != -1) {
// 		m_nStep = m_nStep_New;
// 		m_nStep_New = -1;
// 	}
// 	if (m_nStep_Pre != m_nStep) {
// 		pTask->MakeVatLog(_T("[CMD: %s] [EXECUTE] [Step=%s]"), m_szCmdName, act_string(m_nStep));
// 	}
// 	m_nStep_Pre = m_nStep;
// 
// 	m_nLast_Executing_Step = m_nStep;
// 	switch (m_nStep)
// 	{
// 		case vision_target_measure_start:
// 		{
// 			m_nStep = vision_target_send_measure;
// 		}break;
// 		case vision_target_send_measure:
// 		{
// 			//Vision Measure
// 			g_VatVisionMgr[m_nCamNo].SendMeasure(m_nDataId, m_nStatus);
// 
// 			m_tcTimeout.SetCheckTime(TIME_OUT_MIDDLE);
// 			m_tcTimeout.ResetStartTime();
// 
// 			m_nStep = vision_target_receive_wait;
// 		}break;
// 		case vision_target_receive_wait:
// 		{
// 			//Time Out
// 			if (m_tcTimeout.CheckOverTime()) {
// 				CString strCamNo = ConvertCamIdToString(m_nCamNo);
// 				CString strStatus = ConvertStatusToString(m_nStatus);
// 
// 				CParamList AlarmData;
// 				AlarmData.m_fnAddItemFormat("[%s] [Cam No : %s] ", strStatus, strCamNo);
// 				int nErr = (m_nVisionPC == VAT_VISION_PC_1) ? ERR_VAT_VISION_PC_1_TIME_OUT : ERR_VAT_VISION_PC_2_TIME_OUT;
// 				ReportAlarm(nErr, AlarmData);
// 				break;
// 			}
// 
// 			int nRcvStatus = g_VatVisionMgr[m_nCamNo].GetReceiveState();
// 
// 			//Status 4개 - Success/Error/Disconnect/Idle
// 			if (nRcvStatus == VAT_RCV_STATUS_DISCONNECT) {
// 				CParamList AlarmData;
// 				ReportAlarm(ERR_VAT_VISION_PC_1_DISCONNECT, AlarmData);
// 				break;
// 			}
// 
// 			if (nRcvStatus != VAT_RCV_STATUS_IDLE) {
// 				m_nStep = vision_target_receive_error_chk;
// 			}
// 		}break;
// 		/*
// 		Vision Rcv Status 'Success' or 'Error'
// 		*/
// 		case vision_target_receive_error_chk:
// 		{
// 			int nRcvStatus = g_VatVisionMgr[m_nCamNo].GetReceiveState();
// 
// 			ASSERT(nRcvStatus == VAT_RCV_STATUS_SUCCESS || nRcvStatus == VAT_RCV_STATUS_ERROR);
// 
// 			if (nRcvStatus == VAT_RCV_STATUS_ERROR)
// 			{
// 				CString strCamNo = ConvertCamIdToString(m_nCamNo);
// 				CString strStatus = ConvertStatusToString(m_nStatus);
// 
// 				ST_VAT_RCV_DATA stRcvData;
// 				g_VatVisionMgr[m_nCamNo].GetReceiveData(stRcvData);
// 
// 				int nErr = ConvertVisionErrIdxToAlarmIdx(stRcvData.nError);
// 
// 				CParamList AlarmData;
// 				AlarmData.m_fnAddItemFormat("[%s] [Cam No : %s] ", strStatus, strCamNo);
// 				ReportAlarm(nErr, AlarmData);
// 				break;
// 			}
// 
// 			m_nStep = vision_data_save;
// 		}break;
// 		case vision_data_save:
// 		{
// 			SaveMeasureData(m_nHand, m_nDataId, m_nStatus);
// 
// 			g_VatVisionMgr[m_nCamNo].SetReceiveState(VAT_RCV_STATUS_INIT); //상태 초기화
// 
// 			m_nStep = vision_target_measure_end;
// 		}break;
// 		case vision_target_measure_end:
// 		{
// 			ReportCmdEnd(eSeqCmdStatus_NormalComplete);
// 		}break;
// 		default:
// 		{
// 			m_nStep = 0;
// 		}break;
// 	}
// 
// 	return CBaseSeqCmd::State_Executing();
// }


//1000
BOOL CVatVisionCmd_CamMeasure::State_Pause()
{
	CBaseTaskManager* pTask = (CBaseTaskManager*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTask->MakeVatLog("[CMD: %s] [PAUSE] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
	case 1000:
	{

	}break;
	default:
	{
		m_nStep = 1000;
	}break;
	}

	return CBaseSeqCmd::State_Pause();
}


//2000
BOOL CVatVisionCmd_CamMeasure::State_Recovery()
{
	CBaseTaskManager* pTask = (CBaseTaskManager*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTask->MakeVatLog("[CMD: %s] [RECOVERY] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
		case 2000:
		{
			if (m_nStatus == VISION_STATUS_FIND_Z_MEASURE_PICKER) {
				//재측정 필요함.
				SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_NormalComplete);
			}
			else {
				SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Executing);
			}
			break;

			//SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Executing, m_nLast_Executing_Step);



			int nMeasureDone = 0;
			for (auto nDataId : m_vDataId) {
				int nCamNo = ConvertDataIdToVatCam(nDataId);
				int nRcvStatus = g_VatVisionMgr[nCamNo].GetReceiveState();
				if (m_nLast_Executing_Step >= vision_data_save && nRcvStatus == VAT_RCV_STATUS_INIT) //Lower Target Measure End
				{
					nMeasureDone++;
				}
			}

			if (nMeasureDone == m_vDataId.size()) {
				SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_NormalComplete);
			}
			else {
				m_nStep++;
			}
		}break;
		case 2001:
		{
			std::unordered_map<int, int> rcv_status_cnt;

			for (auto nDataId : m_vDataId) {
				int nCamNo = ConvertDataIdToVatCam(nDataId);
				int nRcvStatus = g_VatVisionMgr[nCamNo].GetReceiveState();
				rcv_status_cnt[nRcvStatus]++;
			}

			// 순서 변경 금지. [8/28/2025 dohyeong.kim]
			// 해당 순번으로 Rcv 상태 하나라도 존재 하면, Last Step 변경.
			if (rcv_status_cnt[VAT_RCV_STATUS_INIT]) {
				//Send Measure 보내기 전
				m_nLast_Executing_Step = vision_target_send_measure;
			}
			else if (rcv_status_cnt[VAT_RCV_STATUS_IDLE] || rcv_status_cnt[VAT_RCV_STATUS_DISCONNECT]) {
				//Send Measure 보낸 후, Rcv 받기 전.
				//Disconnect도 해당 case에서 check.
				m_tcTimeout.SetCheckTime(TIME_OUT_MIDDLE);
				m_tcTimeout.ResetStartTime();

				m_nLast_Executing_Step = vision_target_receive_wait;
			}
			else { //rcv_status -> SUCCESS or FAIL
				m_nLast_Executing_Step = vision_target_receive_error_chk;
			}

			SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Executing, m_nLast_Executing_Step);
		}break;
		default:
		{
			m_nStep = 2000;
		}break;
	}

	return CBaseSeqCmd::State_Recovery();
}


BOOL CVatVisionCmd_CamMeasure::State_NormalComplete()
{
	return CBaseSeqCmd::State_NormalComplete();
}


BOOL CVatVisionCmd_CamMeasure::State_Alarm()
{
	return CBaseSeqCmd::State_Alarm();
}


BOOL CVatVisionCmd_CamMeasure::State_AlarmComplete()
{
	return CBaseSeqCmd::State_AlarmComplete();
}


BOOL CVatVisionCmd_CamMeasure::State_Aborting()
{
	ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_AbortComplete); // 명령 진행 중 STOP 명령을 받아 명령이 중단 됨.
	return CBaseSeqCmd::State_Aborting();
}


BOOL CVatVisionCmd_CamMeasure::State_AbortComplete()
{
	return CBaseSeqCmd::State_AbortComplete();
}


CString CVatVisionCmd_CamMeasure::act_string(int act)
{
	BEGIN_CASE_TO_STR(act);

	CASE_TO_STR(vision_target_measure_start);

	CASE_TO_STR(vision_target_send_measure);
	CASE_TO_STR(vision_target_receive_wait);
	CASE_TO_STR(vision_target_receive_error_chk);
	CASE_TO_STR(vision_data_save);

	CASE_TO_STR(vision_target_measure_end);

	ELSE_CASE(_T("unknown_step"));
	END_CASE_TO_STR();
}