#include "stdafx.h"
#include "OneAxis_SoftServoEtherCAT.h"
#include "Motion_Cont_Imp.h"
#include "DEF_ERROR_CODE.h"

#include "EcApi.h"
#include "IOApi.h"
#include "WMX3Api.h"
#include "CoreMotionApi.h"
#include "AdvancedMotionApi.h"




//#include "AdvancedMotionApi.h"
//#include "ApiBufferApi.h"
//#include "CompensationApi.h"
//#include "CoreMotionApi.h"
//#include "CyclicBufferApi.h"
//#include "EventApi.h"
//#include "IOApi.h"
//#include "LogApi.h"
//#include "UserMemoryApi.h"
//#include "WMX3Api.h"


//#pragma comment(lib, "..\\..\\..\\LibFiles\\SoftServoEtherCAT\\EcApi.lib")


// #pragma comment(lib, "..\\..\\..\\LibFiles\\SoftServoEtherCAT\\WMX3Api.lib")
// #pragma comment(lib, "..\\..\\..\\LibFiles\\SoftServoEtherCAT\\IMDll.lib")
// #pragma comment(lib, "..\\..\\..\\LibFiles\\SoftServoEtherCAT\\CoreMotionApi.lib")
// #pragma comment(lib, "..\\..\\..\\LibFiles\\SoftServoEtherCat\\AdvancedMotionApi.lib")

//#pragma comment(lib, "..\\..\\..\\LibFiles\\SoftServoEtherCat\\ApiBufferApi.lib")
//#pragma comment(lib, "..\\..\\..\\LibFiles\\SoftServoEtherCat\\CompensationApi.lib")
//#pragma comment(lib, "..\\..\\..\\LibFiles\\SoftServoEtherCat\\CyclicBufferApi.lib")
//#pragma comment(lib, "..\\..\\..\\LibFiles\\SoftServoEtherCat\\EventApi.lib")
//#pragma comment(lib, "..\\..\\..\\LibFiles\\SoftServoEtherCat\\IOApi.lib")
//#pragma comment(lib, "..\\..\\..\\LibFiles\\SoftServoEtherCat\\LogApi.lib")
//#pragma comment(lib, "..\\..\\..\\LibFiles\\SoftServoEtherCat\\UserMemoryApi.lib")

unsigned int ConvertModuleEncoder(char num[]) {
	int len = strlen(num);
	int base = 1;
	unsigned int temp = 0;
	for (int i = len - 1; i >= 0; i--) {
		if (num[i] >= '0' && num[i] <= '9') {
			temp += (num[i] - 48)*base;
			base = base * 16;
		}
		else if (num[i] >= 'A' && num[i] <= 'F') {
			temp += (num[i] - 55)*base;
			base = base * 16;
		}
	}
	return temp;
}



using namespace wmx3Api;
using namespace ecApi;

// WMX3Api         g_Wmx3Lib;	// 전역으로 처리
// Ecat			*g_pecApi;
// //EcAPI			*g_pecApi;// (&g_Wmx3Lib);
// CoreMotion      *g_pWmx3Lib_cm = nullptr;
// AdvancedMotion	*g_pAdvMotion = nullptr;

BOOL			g_bThreadStopFlag = FALSE;
CWinThread*	    m_pThreadRcv	  = NULL;
CEvent          m_evtThreadTerminatedRcv;
int OneAxis_SoftServoEtherCAT::m_nConnectSuccess = FALSE;

enum OrgSteps {
	eOrg_Unspecified = -1, // Origin() 에서 Completed된 후에는 이 값으로 설정한다.
	eOrg_Start = 0,
	eOrg_Start_Delay,
	eOrg_Wait,
};


BOOL ChkServoOn(int nPhysicalAxisNum)
{
	CoreMotionStatus CmStatus;
	g_pWmx3Lib_cm->GetStatus(&CmStatus);
	CoreMotionAxisStatus cmAxis = CmStatus.axesStatus[nPhysicalAxisNum];

	short res = 0;
	res = cmAxis.servoOn;
	//서보가 on된경우에는 AmpFault를 확인한다.

	return (BOOL)res;
}

/*=============================================================================
AUTHOR   : 황유빈
NAME     : 재정의ClearOrgFlag
PARAMS   :
RETURN   :
COMMENTS : Origin flag clear
==============================================================================*/
void OneAxis_SoftServoEtherCAT::ClearOrgFlag()
{
	g_pWmx3Lib_cm->home->SetHomeDone(m_nAxisNumPhy, 0);
	OneAxis::ClearOrgFlag();

	m_nOri_Step = eOrg_Unspecified;
}

BOOL OneAxis_SoftServoEtherCAT::ChkCommunication()
{
	return OneAxis_SoftServoEtherCAT::m_nConnectSuccess;
}

int  OneAxis_SoftServoEtherCAT::GetOnlineAxisCount()
{
	if (g_pecApi != nullptr)
	{
		EcMasterInfo master;
		g_pecApi->GetMasterInfo(1,&master);
		return (int)master.numOfSlaves;
	}

	return 0;
}

// 통신 연결 상태 갱신 하여 태스크 시스템에서 처리
void OneAxis_SoftServoEtherCAT::Motion_Error_Check(void)
{
	CoreMotionStatus CmStatus;

	int nChkErrorCode = (int)g_pWmx3Lib_cm->GetStatus(&CmStatus);

	if (nChkErrorCode != wmx3Api::ErrorCode::None)	// Error
	{
		//함수 호출 실패
	}

	// check commnunication error
	EngineStatus wmxEngineSts;			
	g_Wmx3Lib.GetEngineStatus(&wmxEngineSts);
	
	if (wmxEngineSts.state != wmx3Api::EngineState::T::Communicating)
	{
		//return 0;	
	}
	else
	{		
		for (int nAxis_num = 0; nAxis_num < 10; nAxis_num++)
		{
			CoreMotionAxisStatus cmAxis = CmStatus.axesStatus[nAxis_num];	// move 동작전 모터 상태 확인

			if (g_bThreadStopFlag == TRUE)
				break;

			if (1)		//RUN_STOP == RUN && OtherAction.GetRunInitComplete() == TRUE)
			{
				//Alarm state for each axis
				//m_wmxLib->GetStatus(&SSS_STATUS);							

				if (cmAxis.ampAlarm == TRUE )
				{
					// system에서 멈추도록 한다.
					//int AlarmCode = Get_AmpAlarm(axis_num);
					//sprintf(msg, "%s::Axis(No.%d) Was AMP_Alarm (CODE:%d) Please Check the Servo Drive", Motor[axis_num]->IsMotorName(), axis_num + 1, AlarmCode);
					//Error.StopMsg(msg, ErrMsg("0010"), M_ICONINFORMATION | M_OK, 2);
					//ClearAmpAlarm(axis_num);
				}
				else
				{
					//Handler Running 상태에서 Servo Off되어 있는지 Check 
					if (ChkServoOn(nAxis_num) == FALSE)
					{
						//sprintf(msg, "%s::Axis(No.%d) Was Servo Off", Motor[axis_num]->IsMotorName(), axis_num + 1);
						//Error.StopMsg(msg, ErrMsg("0010"), M_ICONINFORMATION | M_OK, 2);
					}
				}
			}
		}
	}
}

UINT OneAxis_SoftServoEtherCAT::SoftServoEtherCAT_Error_Polling(LPVOID lpVoid)
{
	//Handler Motion Error를 여기서 체크 
	while (g_bThreadStopFlag == FALSE)
	{
		// check motor 
		Motion_Error_Check();
		Sleep(100);
	}

	return 1;
}


void OneAxis_SoftServoEtherCAT::ComThreadCtrl(BOOL bRunStop)
{
	if (bRunStop == TRUE)
	{
		if (g_bThreadStopFlag == TRUE)
		{
			return;
		}
		m_pThreadRcv = AfxBeginThread(SoftServoEtherCAT_Error_Polling, NULL);
	}
	else
	{
		g_bThreadStopFlag = TRUE;
	
		int nCount = 0;
		if (m_pThreadRcv != NULL) {
			while (::WaitForSingleObject(m_evtThreadTerminatedRcv, 1) == WAIT_TIMEOUT) {
				nCount++;
				if (nCount > 500) {
					::TerminateThread(m_pThreadRcv->m_hThread, 0);
					break;
				}
			}
		}
		m_pThreadRcv = NULL;
	}
}

//==============================================================================
//
//==============================================================================

//==============================================================================
//
//==============================================================================

bool OneAxis_SoftServoEtherCAT::ApplyPlviModuleConfigration(bool bSet)
{
	m_bPlviModuleServo = bSet;
	if (bSet)
	{
		m_bMotorDisable = true;
		SetPlviCFG_REG(false);

		// --- setting pulse width
		unsigned int ErrorCode;
		unsigned char NewValue[2];
		ZeroMemory(NewValue, sizeof(NewValue));		
		// vision PLVI camera 인식 ms 는 최소 100um 가 필요하다
		// 1당 50us
		// 헌데 2byte라서 59999까지밖에 안먹는다.
		short PulseWidth = 1999; // 1999*50 = 99,950 us = 99ms
		memcpy(NewValue, (char*)&PulseWidth, 2);
		long lRes = g_pecApi->SdoDownload(DEF_SOFTSERVO_MASTER_ID, m_nAxisNumPhy, 0x2010, 6, sizeof(NewValue), NewValue, &ErrorCode, 1000);
		if (lRes != 0)
			return false;

		// --- setting pulse period
		// 310mm / 1800image = 0.172222
		// 1pusle module = 93.244 -> 172.22*93.244 = 16058.68 (소수점 버림)
		//ZeroMemory(NewValue, sizeof(NewValue));
		//short PulsePeriod = 469/*16058*/;
		//memcpy(NewValue, (char*)&PulsePeriod, 2);
		//lRes = g_pecApi->SdoDownload(m_nAxisNumPhy, 0x2010, 5, sizeof(NewValue), NewValue, &ErrorCode, 1000);
		//if (lRes != 0)
		//	return false;
	}
	return true;
}

// 모터 부하 확인
// 현재 FUJI만 구현 된 상태
int  OneAxis_SoftServoEtherCAT::GetTorque(BOOL bMaxTorque , double &dTorquePesentage)
{
	int nAddr = 0;

	if (m_strServoMaker.Compare(_T("FUJI")) == 0)	// FUJI
	{
		nAddr = 0x6077;	// 메뉴얼 상 명칭은 실효 토크 이지만 남동운 이사 실시간 토크라고 하였음.
	}
	else if (m_strServoMaker.Compare(_T("RSA")) == 0)	// RSA
	{
		return ERR_SUCCESS;

		if (bMaxTorque == TRUE)
		{
			nAddr = 0X2A2D;
		}
		else
		{
			nAddr = 0x2A2C;
		}
	}
	else
	{
		return ERR_SUCCESS;
	}

	unsigned int   ErrorCode;
	unsigned char  cTorqueVal[2];
	unsigned int   ActualSize2 = 0;
	unsigned int   nGetData = 0;
	short		   nTorqueShrot = 0;
	long lRes = g_pecApi->SdoUpload(DEF_SOFTSERVO_MASTER_ID, m_nAxisNumPhy, nAddr, 0, sizeof(cTorqueVal), cTorqueVal, &ActualSize2, &ErrorCode, 1000);

	if (lRes != 0)
		return ERR_FAILURE;
	else
	{
		CString strTorque;
		strTorque.Format("%02x%02x", cTorqueVal[1], cTorqueVal[0]);
		strTorque.MakeUpper();
		nGetData   = ConvertModuleEncoder((LPSTR)(LPCTSTR)strTorque);

		nTorqueShrot = (short)nGetData;
		dTorquePesentage = nTorqueShrot * 0.1;
		
		m_MtLog.LOG("torque val=%.1f", dTorquePesentage);
	}

	return ERR_SUCCESS;
}

int  OneAxis_SoftServoEtherCAT::GetPlviTriggerCount()
{
	// plvi 미사용
	unsigned int ErrorCode;
	unsigned char szTiggerCount[4];
	unsigned int ActualSize2 = 0;
	unsigned int nGetTriggerCount = 0;
	ZeroMemory(szTiggerCount, sizeof(szTiggerCount));
	g_pecApi->SdoUpload(DEF_SOFTSERVO_MASTER_ID,m_nAxisNumPhy, 0x2010, 2, sizeof(szTiggerCount), szTiggerCount, &ActualSize2, &ErrorCode, 1000);
	{
		CString strStart;
		strStart.Format("%02x%02x%02x%02x", szTiggerCount[3], szTiggerCount[2], szTiggerCount[1], szTiggerCount[0]);
		strStart.MakeUpper();
		nGetTriggerCount = ConvertModuleEncoder((LPSTR)(LPCTSTR)strStart);
		m_MtLog.LOG("TiggerCount=%ld", nGetTriggerCount);
	}

	return (int)nGetTriggerCount;
}

int OneAxis_SoftServoEtherCAT::SetPlviCFG_REG(bool bUse/*0=dis,1=enable*/, double dTrigerDistence_um/*start-end dist*/, double dStartOffset_um/*현위치에서 빼야할 거리*/, int nImageCount/*촬영 이미지 횟수*/)
{
	if (m_bPlviModuleServo == true)
	{		
		// plvi 미사용
		unsigned int ErrorCode;

		// plvi 미사용
		//unsigned int ErrorCode;
		unsigned char NewValue[2];
		ZeroMemory(NewValue, sizeof(NewValue));
		long lRes = g_pecApi->SdoDownload(DEF_SOFTSERVO_MASTER_ID,m_nAxisNumPhy, 0x2010, 1, sizeof(NewValue), NewValue, &ErrorCode, 1000);
		if (lRes != 0)
			return ERR_FAILURE;

		if (bUse == true)// plvi 사용
		{// 미사용으로 만든 후 setting 하고 enable 시켜야 한다.
			unsigned char cStartPos[4], cEnddPos[4];			
			unsigned int ActualSize = 0;

			// set period
			// vision cam 최소 period는 1.6ms
			ZeroMemory(NewValue, sizeof(NewValue));
			short PulsePeriod = (short)(((dTrigerDistence_um/1000) / nImageCount)*2778.89);
			memcpy(NewValue, (char*)&PulsePeriod, 2);
			lRes = g_pecApi->SdoDownload(DEF_SOFTSERVO_MASTER_ID,m_nAxisNumPhy, 0x2010, 5, sizeof(NewValue), NewValue, &ErrorCode, 1000);
			if (lRes != 0)
				return ERR_FAILURE;

			// -- get start pos
			ZeroMemory(cStartPos, sizeof(cStartPos));
			lRes = g_pecApi->SdoUpload(DEF_SOFTSERVO_MASTER_ID,m_nAxisNumPhy, 0x2010, 0x07, sizeof(cStartPos), cStartPos, &ActualSize, &ErrorCode, 1000);
			if (lRes != 0)
				return ERR_FAILURE;

			CString strStart;
			strStart.Format("%02x%02x%02x%02x", cStartPos[3], cStartPos[2], cStartPos[1], cStartPos[0]);
			strStart.MakeUpper();
			unsigned int nGetDataStart = ConvertModuleEncoder((LPSTR)(LPCTSTR)strStart);
			//unsigned int nGetDataEnd = nGetDataStart;
			// ------------------------------------------------------
			// y encoder 1바퀴 pulse = 250000
			// 2500 pulse(1회전) = 89.9637952
			// 250000 / 89.9637952(35.985517 * 2500) = 2778.89( 1mm 당 pulse 수 )
			// -------------------------------------------------------

			// -- set start pos
			nGetDataStart -= (unsigned int)((dStartOffset_um/1000.0) * 2778.89);
			ZeroMemory(cStartPos, sizeof(cStartPos));
			memcpy(cStartPos, (char*)&nGetDataStart, 4);
			lRes = g_pecApi->SdoDownload(DEF_SOFTSERVO_MASTER_ID,m_nAxisNumPhy, 0x2010, 0x03, sizeof(cStartPos), cStartPos, &ErrorCode, 1000);
			if (lRes != 0)
				return ERR_FAILURE;

			// -- set end pos
			unsigned int nGetDataEnd = nGetDataStart;
			nGetDataEnd -= (unsigned int)((dTrigerDistence_um /1000.0) * 2778.89);
			ZeroMemory(cEnddPos, sizeof(cEnddPos));
			memcpy(cEnddPos, (char*)&nGetDataEnd, 4);
			lRes = g_pecApi->SdoDownload(DEF_SOFTSERVO_MASTER_ID,m_nAxisNumPhy, 0x2010, 0x04, sizeof(cEnddPos), cEnddPos, &ErrorCode, 1000);
			if (lRes != 0)
				return ERR_FAILURE;

			Sleep(50);
			// -- set enable
			ZeroMemory(NewValue, sizeof(NewValue));
			NewValue[0] = 0x04;
			lRes = g_pecApi->SdoDownload(DEF_SOFTSERVO_MASTER_ID,m_nAxisNumPhy, 0x2010, 1, sizeof(NewValue), NewValue, &ErrorCode, 1000);
			if (lRes != 0)
				return ERR_FAILURE;
		}
	}

	return ERR_SUCCESS;
}

bool OneAxis_SoftServoEtherCAT::ApplyConfiguration(eAxisServoConfiguration Configuration, BOOL bUseStartEngine/*= FALSE*/)
{
	m_bPlviModuleServo = false;
	wmx3Api::EngineStatus Status;
	Status.state = wmx3Api::EngineState::Unknown;

	// RestartEngine 및 ReconnectCommunication은 어느 객체에서 사용해도 동일한 효과를 가진다.
	// Axis 객체가 여러 개라도, 여러 개 중 하나의 객체에 대해서만 단 1번 호출하면 된다.
	if (bUseStartEngine == TRUE)
	{
		if (Configuration & eAxisServoConfiguration::RestartEngine)
		{
			// wmx3Api::EngineStatus::state 가 Running 또는 Communicating 일 경우에는
			// Engine이 running 중인 경우이다. 또한 Communicating의 경우는 통신이 정상임을 의미한다.
			// 하지만 EMO를 눌러 Power가 나간 경우에도 해당 state는 변하지 않았고 그대로 Communicating 상태로 유지되었다.
			// 
			// Power 복구 후에 StopCommunication() -> StartCommunication() 후 정상 작동하였으므로 state를 참조하지 않기로 한다.

			//
			// Idle          - The platform engine is idle(this state indicates that the platform engine is not running).
			// Running       - The platform engine is running, but communication with the network is stopped.
			// Communicating - The platform engine is communicating with the network.
			// Shutdown      - The platform engine is shutting down.
			// Unknown       - Unknown engine state.
			// 

			//g_Wmx3Lib.GetEngineStatus(&Status);

			if (true) //(Status.state != wmx3Api::EngineState::Running && Status.state != wmx3Api::EngineState::Communicating)
			{
				// Restart the engine.
				g_Wmx3Lib.RestartEngine("C:\\Program Files\\SoftServo\\WMX3\\");
			}
			Sleep(5000);
		}

		// RestartEngine 및 ReconnectCommunication은 어느 객체에서 사용해도 동일한 효과를 가진다.
		// Axis 객체가 여러 개라도, 여러 개 중 하나의 객체에 대해서만 단 1번 호출하면 된다.
		if (Configuration & eAxisServoConfiguration::ReconnectCommunication)
		{
			// g_Wmx3Lib.GetEngineStatus(&Status);

			if (true) //(Status.state != wmx3Api::EngineState::Running && Status.state != wmx3Api::EngineState::Communicating)
			{
				// Restart the communication.
				g_Wmx3Lib.StopCommunication(INFINITE);

				if (!OneAxis_SoftServoEtherCAT::StartCommunication())
					return false;
			}
			Sleep(5000);
		}
		
	}
	

	int nPhysicalAxis = m_nAxisNumPhy;
	int nPulsePer1Round = (int)m_dPulsePerRound;
	double dInPosWidth = m_dHwInPosWidth;

	// Motor를 이동시키기 위한 setting을 여기에서 수행한다.
	// 아래 항목들은 Motor를 이동시키기 전에 기본적으로 setting되어 있어야만 한다.
	// => 기어비, in-pos width, 움직이는 방향.
	if (Configuration & eAxisServoConfiguration::BasicConfiguration)
	{
		long Result = 0;

		// gear비를 설정한다.
		// gearRatioNumerator = Motor Driver 단에서의 pulse (분해능). RSA 기준 1 << 23.
		// gearRatioDenominator = 사용자가 설정하고자 하는 1회전당 pulse.
		double dGearRatioNumerator = 0.0;
		if (m_strServoMaker.Compare(_T("FUJI")) == 0)	// FUJI
		{
			dGearRatioNumerator = 1 << 24;
		}
		else if (m_strServoMaker.Compare(_T("EZI_SERVO")) == 0)	// ezi servo 공통
		{
			dGearRatioNumerator = 10000.0;
		}
		else if (m_strServoMaker.Compare(_T("RSA_TRI")) == 0)
		{
			dGearRatioNumerator = 250000.0;
		}
		else // RSA
		{
			dGearRatioNumerator = 1 << 23;
		}			

		double dGearRatioDenominator = nPulsePer1Round;
		Result = g_pWmx3Lib_cm->config->SetGearRatio(nPhysicalAxis, dGearRatioNumerator, dGearRatioDenominator);
		m_MtLog.LOG(_T("SetGearRatio (nPhyAxis: %d, dGearRatioNumerator: %lf, dGearRatioDenominator: %lf) = %d"),
			nPhysicalAxis, dGearRatioNumerator, dGearRatioDenominator, Result);

		if (Result != 0)
		{
			m_MtLog.LOG(_T("SetGearRatio failed, nPhyAxis: %d"), nPhysicalAxis);
			m_bMotorDisable = true;
			return false;
		}

		// in-pos width를 설정한다.
		// 적용 시, -width에서 +width까지의 범위 내에 있으면 in-position은 true.

		Config::FeedbackParam cmParam;
		Result = g_pWmx3Lib_cm->config->GetFeedbackParam(nPhysicalAxis, &cmParam);
		m_MtLog.LOG(_T("GetFeedbackParam (nPhyAxis: %d) = %d"), nPhysicalAxis, Result);

		if (Result != 0)
		{
			m_MtLog.LOG(_T("GetFeedbackParam failed, nPhyAxis: %d"), nPhysicalAxis);
			m_bMotorDisable = true;
			return false;
		}

		cmParam.inPosWidth = dInPosWidth;
		cmParam.delayedPosSetWidth = dInPosWidth;
		cmParam.delayedPosSetMilliseconds = 20.0;		// 범위내에 50ms 유지 해야 설정 하도록 수정. 
		Result = g_pWmx3Lib_cm->config->SetFeedbackParam(nPhysicalAxis, &cmParam);
		m_MtLog.LOG(_T("SetFeedbackParam (nPhyAxis: %d) { dInPosWidth: %lf } = %d {delayedPosMilliseconds = %d}"), 
			nPhysicalAxis, dInPosWidth, Result, cmParam.delayedPosSetMilliseconds);

		if (Result != 0)
		{
			m_MtLog.LOG(_T("SetFeedbackParam failed, nPhyAxis: %d"), nPhysicalAxis);
			m_bMotorDisable = true;
			return false;
		}


		int nHwMoveDirection = m_nMMC_Coordinate_dir;

		unsigned char PrevValue = 0, CurrValue = 0, NewValue = 0;
		unsigned int ActualSize = 0;
		unsigned int ErrorCode;

		ASSERT(nHwMoveDirection == 0 || nHwMoveDirection == 1);

		// TBL_AXIS_PARAM.MMC_COORDINATE_DIR의 값이...
		// 0일 경우(정방향) : SDO 0x2002, subindex 3->Write 0x00,
		// 1일 경우(역방향) : SDO 0x2002, subindex 3->Write 0x01
		NewValue = !!nHwMoveDirection;

		//
		// SDO Read/Write로 설정을 변경한다.
		//

		Result = g_pecApi->ChangeSlaveState(DEF_SOFTSERVO_MASTER_ID, nPhysicalAxis, EcStateMachine::Preop/*StateMachine::Preop*/, (int *)&ErrorCode);

		m_MtLog.LOG(_T("ChangeSlaveState: PhyAxis %d, Result %d, ErrorCode %d"), nPhysicalAxis, Result, ErrorCode);
		if (Result != 0)
		{
			AfxMessageBox(_T("ChangeSlaveState failed (Preop)"), MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
			m_bMotorDisable = true;
			return false;
		}
		
		if (m_strServoMaker.Compare(_T("FUJI")) == 0)	// fuji  공통
		{
			unsigned char cPreValue[4] = { 0, };
			unsigned char cNewVal[4] = { 0, };
			unsigned char cCurrentVal[4]  = { 0, };

			unsigned char cPreValueFault[2] = { 0, };
			unsigned char cNewValFault[2] = { 0, };
			unsigned char cCurrentValFault[2] = { 0, };

			cNewVal[0] = (nHwMoveDirection == 0 ? 0 : 1);

			//2호기 CY Fault 제거 
// 			g_pecApi->TxPdoWrite()
// 				g_pecApi->PdoRead()
// 			Result = g_pecApi->SdoUpload(DEF_SOFTSERVO_MASTER_ID, nPhysicalAxis, 0x6040, 0, sizeof(cPreValueFault), cPreValueFault, &ActualSize, &ErrorCode, 1000);
// 			m_MtLog.LOG(_T("CY SdoUpload: PhyAxis %d, Result %d, ErrorCode %d, PrevValue 0x%x, ActualSize %d"),
// 				nPhysicalAxis, Result, ErrorCode, cPreValueFault[0], ActualSize);
// 			if (Result != 0)
// 			{
// 				AfxMessageBox(_T("SdoUpload failed"), MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
// 				m_bMotorDisable = true;
// 				return false;
// 			}
// 			cNewValFault[0] = 128;
// 			Result = g_pecApi->SdoDownload(DEF_SOFTSERVO_MASTER_ID, nPhysicalAxis, 0x6040, 0, sizeof(cNewValFault), cNewValFault, &ErrorCode, 1000);
// 			m_MtLog.LOG(_T("CY SdoDownload: PhyAxis %d, Result %d, ErrorCode %d, NewValue 0x%x"),
// 				nPhysicalAxis, Result, ErrorCode, cNewValFault[0]);
// 			if (Result != 0)
// 			{
// 				AfxMessageBox(_T("CY SdoDownload failed"), MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
// 				m_bMotorDisable = true;
// 				return false;
// 			}
// 
// 			Result = g_pecApi->SdoUpload(DEF_SOFTSERVO_MASTER_ID, nPhysicalAxis, 0x6040, 0, sizeof(cCurrentValFault), cCurrentValFault, &ActualSize, &ErrorCode, 1000);
// 			m_MtLog.LOG(_T("SdoUpload: PhyAxis %d, Result %d, ErrorCode %d, PrevValue 0x%x, ActualSize %d"),
// 				nPhysicalAxis, Result, ErrorCode, cCurrentValFault[0], ActualSize);
// 			if (Result != 0)
// 			{
// 				AfxMessageBox(_T("CY SdoUpload failed"), MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
// 				m_bMotorDisable = true;
// 				return false;
// 			}
// 			if (cNewValFault[0] != cCurrentValFault[0])
// 			{
// 				AfxMessageBox(_T("CY SdoUpload result mismatch"), MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
// 				m_bMotorDisable = true;
// 				return false;
// 			}

// 			unsigned int  waitTime = 3000;
// 			unsigned int  errCode = 0;
// 			unsigned char data[4];
// 			unsigned int  mask = 0xFFFFFFFF;
// 			int Value = 128;
// 
// 			data[0] = (byte)(Value & 0xff);
// 			data[1] = (byte)((Value & 0xff00) >> 8);
// 			data[2] = (byte)((Value & 0xff0000) >> 16);
// 			data[3] = (byte)((Value & 0xff000000) >> 24);
// 
// 			g_pecApi->TxPdoWrite(DEF_SOFTSERVO_MASTER_ID, 0x6040, 0, 0x2, data, mask);
// 
// 			g_pecApi->ClearTxPdoWrite(DEF_SOFTSERVO_MASTER_ID, 0x6040, 0);

			// cw/ccw 설정 파라미터 확인 중
			Result = g_pecApi->SdoUpload(DEF_SOFTSERVO_MASTER_ID, nPhysicalAxis, 0x5001, 4, sizeof(cPreValue), cPreValue, &ActualSize, &ErrorCode, 1000);
			m_MtLog.LOG(_T("SdoUpload: PhyAxis %d, Result %d, ErrorCode %d, PrevValue 0x%x, ActualSize %d"),
				nPhysicalAxis, Result, ErrorCode, cPreValue[0], ActualSize);
			if (Result != 0)
			{
				AfxMessageBox(_T("SdoUpload failed"), MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
				m_bMotorDisable = true;
				return false;
			}

			Result = g_pecApi->SdoDownload(DEF_SOFTSERVO_MASTER_ID, nPhysicalAxis, 0x5001, 4, sizeof(cNewVal), cNewVal, &ErrorCode, 1000);
			m_MtLog.LOG(_T("SdoDownload: PhyAxis %d, Result %d, ErrorCode %d, NewValue 0x%x"),
				nPhysicalAxis, Result, ErrorCode, cNewVal[0]);
			if (Result != 0)
			{
				AfxMessageBox(_T("SdoDownload failed"), MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
				m_bMotorDisable = true;
				return false;
			}

			Result = g_pecApi->SdoUpload(DEF_SOFTSERVO_MASTER_ID, nPhysicalAxis, 0x5001, 4, sizeof(cCurrentVal), cCurrentVal, &ActualSize, &ErrorCode, 1000);
			m_MtLog.LOG(_T("SdoUpload: PhyAxis %d, Result %d, ErrorCode %d, PrevValue 0x%x, ActualSize %d"),
				nPhysicalAxis, Result, ErrorCode, cCurrentVal[0], ActualSize);
			if (Result != 0)
			{
				AfxMessageBox(_T("SdoUpload failed"), MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
				m_bMotorDisable = true;
				return false;
			}
			if (cNewVal[0] != cCurrentVal[0])
			{
				AfxMessageBox(_T("SdoUpload result mismatch"), MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
				m_bMotorDisable = true;
				return false;
			}
		}
		else if (m_strServoMaker.Compare(_T("EZI_SERVO")) == 0)	// ezi servo 공통
		{
			// ezi servo 회전수 설정
			unsigned char cSetPulse_1motorSpin[4] = { 0, };
			unsigned char cPrePulse_1motorSpin[4] = { 0, };
			unsigned char cCurPulse_1motorSpin[4] = { 0, };

			ZeroMemory(cSetPulse_1motorSpin, sizeof(cSetPulse_1motorSpin));
			ZeroMemory(cPrePulse_1motorSpin, sizeof(cPrePulse_1motorSpin));
			ZeroMemory(cCurPulse_1motorSpin, sizeof(cCurPulse_1motorSpin));

			// 10000 pulse로 설정 한다.
			cSetPulse_1motorSpin[1] = 39;
			cSetPulse_1motorSpin[0] = 16;

			// 모터 회전수
			Result = g_pecApi->SdoUpload(DEF_SOFTSERVO_MASTER_ID,nPhysicalAxis, 0x200c, 0, sizeof(cPrePulse_1motorSpin), cPrePulse_1motorSpin, &ActualSize, &ErrorCode, 1000);
			m_MtLog.LOG(_T("SdoUpload: PhyAxis %d, Result %d, ErrorCode %d, PrevValue 0x%x, ActualSize %d"),
				nPhysicalAxis, Result, ErrorCode, cPrePulse_1motorSpin, ActualSize);
			if (Result != 0)
			{
				AfxMessageBox(_T("SdoUpload failed"), MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
				m_bMotorDisable = true;
				return false;
			}

			Result = g_pecApi->SdoDownload(DEF_SOFTSERVO_MASTER_ID,nPhysicalAxis, 0x200c, 0, sizeof(cSetPulse_1motorSpin), cSetPulse_1motorSpin, &ErrorCode, 1000);
			m_MtLog.LOG(_T("SdoDownload: PhyAxis %d, Result %d, ErrorCode %d, NewValue 0x%x"),
				nPhysicalAxis, Result, ErrorCode, cSetPulse_1motorSpin);
			if (Result != 0)
			{
				AfxMessageBox(_T("SdoDownload failed"), MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
				m_bMotorDisable = true;
				return false;
			}

			Result = g_pecApi->SdoUpload(DEF_SOFTSERVO_MASTER_ID,nPhysicalAxis, 0x200c, 0, sizeof(cCurPulse_1motorSpin), cCurPulse_1motorSpin, &ActualSize, &ErrorCode, 1000);
			m_MtLog.LOG(_T("SdoUpload: PhyAxis %d, Result %d, ErrorCode %d, PrevValue 0x%x, ActualSize %d"),
				nPhysicalAxis, Result, ErrorCode, cCurPulse_1motorSpin, ActualSize);
			if (Result != 0)
			{
				AfxMessageBox(_T("SdoUpload failed"), MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
				m_bMotorDisable = true;
				return false;
			}
			
			// motor 방향 설정 ( Index : 607E, SubIndex 0 )
			NewValue = (nHwMoveDirection == 0 ? 0 : 128);
			Result = g_pecApi->SdoUpload(DEF_SOFTSERVO_MASTER_ID,nPhysicalAxis, 0x607e, 0, sizeof(PrevValue), &PrevValue, &ActualSize, &ErrorCode, 1000);
			m_MtLog.LOG(_T("SdoUpload: PhyAxis %d, Result %d, ErrorCode %d, PrevValue 0x%x, ActualSize %d"),
				nPhysicalAxis, Result, ErrorCode, PrevValue, ActualSize);
			if (Result != 0)
			{
				AfxMessageBox(_T("SdoUpload failed"), MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
				m_bMotorDisable = true;
				return false;
			}
			Result = g_pecApi->SdoDownload(DEF_SOFTSERVO_MASTER_ID, nPhysicalAxis, 0x607e, 0, sizeof(NewValue), &NewValue, &ErrorCode, 1000);
			m_MtLog.LOG(_T("SdoDownload: PhyAxis %d, Result %d, ErrorCode %d, NewValue 0x%x"),
				nPhysicalAxis, Result, ErrorCode, NewValue);
			if (Result != 0)
			{
				AfxMessageBox(_T("SdoDownload failed"), MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
				m_bMotorDisable = true;
				return false;
			}
			Result = g_pecApi->SdoUpload(DEF_SOFTSERVO_MASTER_ID,nPhysicalAxis, 0x607e, 0, sizeof(CurrValue), &CurrValue, &ActualSize, &ErrorCode, 1000);
			m_MtLog.LOG(_T("SdoUpload: PhyAxis %d, Result %d, ErrorCode %d, PrevValue 0x%x, ActualSize %d"),
				nPhysicalAxis, Result, ErrorCode, CurrValue, ActualSize);
			if (Result != 0)
			{
				AfxMessageBox(_T("SdoUpload failed"), MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
				m_bMotorDisable = true;
				return false;
			}
			if (NewValue != CurrValue)
			{
				AfxMessageBox(_T("SdoUpload result mismatch"), MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
				m_bMotorDisable = true;
				return false;
			}

		}
		else if (m_strServoMaker.Compare(_T("RSA")) == 0)	// RSA 공통
		{
			unsigned char cPreValue[4] = { 0, };
			unsigned char cNewVal[4] = { 0, };
			unsigned char cCurrentVal[4] = { 0, };

			Result = g_pecApi->SdoUpload(DEF_SOFTSERVO_MASTER_ID,nPhysicalAxis, 0x2002, 3, sizeof(PrevValue), &PrevValue, &ActualSize, &ErrorCode, 1000);
			m_MtLog.LOG(_T("SdoUpload: PhyAxis %d, Result %d, ErrorCode %d, PrevValue 0x%x, ActualSize %d"),
				nPhysicalAxis, Result, ErrorCode, PrevValue, ActualSize);
			if (Result != 0)
			{
				AfxMessageBox(_T("SdoUpload failed"), MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
				m_bMotorDisable = true;
				return false;
			}
			unsigned char charoverload = '1';
			Result = g_pecApi->SdoDownload(DEF_SOFTSERVO_MASTER_ID, nPhysicalAxis, 0x2417, 0, 4, &charoverload, &ErrorCode, 1000);
			if (Result != 0)
			{
				AfxMessageBox(_T("SdoDownload Set torque load rate time failed"), MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
				m_bMotorDisable = true;
				return false;
			}

			Result = g_pecApi->SdoDownload(DEF_SOFTSERVO_MASTER_ID,nPhysicalAxis, 0x2002, 3, sizeof(NewValue), &NewValue, &ErrorCode, 1000);
			m_MtLog.LOG(_T("SdoDownload: PhyAxis %d, Result %d, ErrorCode %d, NewValue 0x%x"),
				nPhysicalAxis, Result, ErrorCode, NewValue);
			if (Result != 0)
			{
				AfxMessageBox(_T("0x2002 SdoDownload failed"), MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
				m_bMotorDisable = true;
				return false;
			}

			Result = g_pecApi->SdoUpload(DEF_SOFTSERVO_MASTER_ID,nPhysicalAxis, 0x2002, 3, sizeof(CurrValue), &CurrValue, &ActualSize, &ErrorCode, 1000);
			m_MtLog.LOG(_T("SdoUpload: PhyAxis %d, Result %d, ErrorCode %d, PrevValue 0x%x, ActualSize %d"),
				nPhysicalAxis, Result, ErrorCode, CurrValue, ActualSize);
			if (Result != 0)
			{
				AfxMessageBox(_T("0x2002 SdoUpload failed"), MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
				m_bMotorDisable = true;
				return false;
			}
			if (NewValue != CurrValue)
			{
				AfxMessageBox(_T("0x2002 SdoUpload result mismatch"), MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
				m_bMotorDisable = true;
				return false;
			}


			if (nPhysicalAxis >= eAXIS_LOAD_TABLE_1_ROTATE && nPhysicalAxis <= eAXIS_LOAD_TABLE_4_ROTATE)
			{
				Result = g_pecApi->SdoUpload(DEF_SOFTSERVO_MASTER_ID, nPhysicalAxis, 0x2314, 0, sizeof(cPreValue), cPreValue, &ActualSize, &ErrorCode, 1000);
				m_MtLog.LOG(_T("SdoUpload: PhyAxis %d, Result %d, ErrorCode %d, PrevValue 0x%x, ActualSize %d"),
					nPhysicalAxis, Result, ErrorCode, cPreValue[0], ActualSize);
				if (Result != 0)
				{
					AfxMessageBox(_T("0x2314 SdoUpload failed"), MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
					m_bMotorDisable = true;
					return false;
				}
				
				cNewVal[0] = 0xAA;
				cNewVal[1] = 0xAA;
				cNewVal[2] = 0x0A;

				Result = g_pecApi->SdoDownload(DEF_SOFTSERVO_MASTER_ID, nPhysicalAxis, 0x2314, 0, sizeof(cNewVal), cNewVal, &ErrorCode, 1000);
				m_MtLog.LOG(_T("SdoDownload: PhyAxis %d, Result %d, ErrorCode %d, PrevValue 0x%x, ActualSize %d"),
					nPhysicalAxis, Result, ErrorCode, cNewVal[0], ActualSize);
				if (Result != 0)
				{
					AfxMessageBox(_T("0x2314 SdoDownload Set torque load rate time failed"), MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
					m_bMotorDisable = true;
					return false;
				}

				Result = g_pecApi->SdoUpload(DEF_SOFTSERVO_MASTER_ID, nPhysicalAxis, 0x2314, 0, sizeof(cCurrentVal), cCurrentVal, &ActualSize, &ErrorCode, 1000);
				m_MtLog.LOG(_T("SdoUpload: PhyAxis %d, Result %d, ErrorCode %d, PrevValue 0x%x, ActualSize %d"),
					nPhysicalAxis, Result, ErrorCode, cCurrentVal[0], ActualSize);
				if (Result != 0)
				{
					AfxMessageBox(_T("0x2314 SdoUpload failed"), MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
					m_bMotorDisable = true;
					return false;
				}
				if (memcmp(cNewVal, cCurrentVal, sizeof(cNewVal)) != ERR_NO_ERROR)
				{
					AfxMessageBox(_T("0x2314 SdoUpload result mismatch"), MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
					m_bMotorDisable = true;
					return false;
				}
			}
		}

		Result = g_pecApi->ChangeSlaveState(DEF_SOFTSERVO_MASTER_ID,nPhysicalAxis, EcStateMachine::Op/*StateMachine::Op*/, (int *)&ErrorCode);
		m_MtLog.LOG(_T("ChangeSlaveState: PhyAxis %d, Result %d, ErrorCode %d"), nPhysicalAxis, Result, ErrorCode);
		if (Result != 0)
		{
			AfxMessageBox(_T("ChangeSlaveState failed (Op)"), MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
			m_bMotorDisable = true;
			return false;
		}

		// SDO 설정이 실패했을 경우 해당 motor를 절대로 움직여서는 안 된다. 해당 motor를 disable 시킨다.
		m_bMotorDisable = false;
	}

	// Motor를 동작시키기 위해 필요하지만, 설정하지 않아도 Motor는 움직인다.
	// => Limit switch 설정.
	if (Configuration & eAxisServoConfiguration::ExtendedConfiguration)
	{
		//// Limit switch type을 설정한다.
		//// Motor가 움직이려는 방향으로 limit switch가 감지되었을 경우 감속하여 정지한다.
		//// 정지까지의 시간은 DEF_SOFTSERVO_LIMIT_SWITCH_HIT_DECEL_TIME_SEC 에 정의되어 있다. 단위는 sec.
		//Config::LimitParam LimitParam;
		//
		//long Result = g_pWmx3Lib_cm->config->GetLimitParam(nPhysicalAxis, &LimitParam);
		//m_MtLog.LOG(_T("GetLimitParam (nPhyAxis: %d) = %d"), nPhysicalAxis, Result);
		//
		//if (Result != 0)
		//{
		//	m_MtLog.LOG(_T("GetLimitParam failed, nPhyAxis: %d"), nPhysicalAxis);
		//	m_bMotorDisable = true;
		//	return false;
		//}
		////LimitParam.lsType = Config::LimitSwitchType::Dec;
		////LimitParam.lsDec = m_dCmdVelocity / (DEF_SOFTSERVO_LIMIT_SWITCH_HIT_DECEL_TIME_SEC * (m_dScaleFactor + m_dScaleFactorOffset));
		//
		////SoftLimit으로 변경 
		////Limit pos Pulse 로 설정 
		//LimitParam.softLimitType = Config::LimitSwitchType::Dec;
		//LimitParam.softLimitNegativePos = m_dSwLimit_Nega_um / (m_dScaleFactor + m_dScaleFactorOffset);
		//LimitParam.softLimitPositivePos = m_dSwLimit_Posi_um / (m_dScaleFactor + m_dScaleFactorOffset);
		//
		//
		//m_MtLog.LOG(_T("PhyAxis: %d, Velocity: %lf, LimitSwitchHitDecelTime: %lf, ScaleFactor: %lf, ScaleFactorOffset: %lf"),
		//	nPhysicalAxis, m_dCmdVelocity, DEF_SOFTSERVO_LIMIT_SWITCH_HIT_DECEL_TIME_SEC, m_dScaleFactor, m_dScaleFactorOffset);
		//
		//Result = g_pWmx3Lib_cm->config->SetLimitParam(nPhysicalAxis, &LimitParam);
		//m_MtLog.LOG(_T("SetLimitParam (nPhyAxis: %d) { lsType: %d, lsDec: %lf } = %d"), 
		//	nPhysicalAxis, LimitParam.lsType, LimitParam.lsDec, Result);
		//
		//if (Result != 0)
		//{
		//	m_MtLog.LOG(_T("SetLimitParam failed, nPhyAxis: %d"), nPhysicalAxis);
		//	m_bMotorDisable = true;
		//	return false;
		//}

		Config::LimitParam LimitParam;
		// fuji motor는 
		if (m_strServoMaker.Compare(_T("FUJI")) == 0)	// FUJI
		{
			long Result = g_pWmx3Lib_cm->config->GetLimitParam(nPhysicalAxis, &LimitParam);
			m_MtLog.LOG(_T("GetLimitParam (nPhyAxis: %d) = %d"), nPhysicalAxis, Result);

			if (Result != 0)
			{
				m_MtLog.LOG(_T("GetLimitParam failed, nPhyAxis: %d"), nPhysicalAxis);
				m_bMotorDisable = true;
				return false;
			}

			LimitParam.lsType = Config::LimitSwitchType::Dec;
			LimitParam.lsDec = m_dCmdVelocity / (DEF_SOFTSERVO_LIMIT_SWITCH_HIT_DECEL_TIME_SEC * (m_dScaleFactor + m_dScaleFactorOffset));
			
			Result = g_pWmx3Lib_cm->config->SetLimitParam(nPhysicalAxis, &LimitParam);
			m_MtLog.LOG(_T("SetLimitParam (nPhyAxis: %d) { lsType: %d, lsDec: %lf } = %d"), 
				nPhysicalAxis, LimitParam.lsType, LimitParam.lsDec, Result);
			
			if (Result != 0)
			{
				m_MtLog.LOG(_T("SetLimitParam failed, nPhyAxis: %d"), nPhysicalAxis);
				m_bMotorDisable = true;
				return false;
			}
		}


		bool bSetLimit = SetSWLimit(m_dSwLimit_Posi_um, m_dSwLimit_Nega_um);
		if (bSetLimit == false) {
			return false;
		}
	}

	return true;
}

bool OneAxis_SoftServoEtherCAT::InitBoard(
	// AxisMap[AxisPhysicalNumber] = { PulsePer1Round, InPosWidth } 
	std::map<int, ST_MOTION_INIT_PARAM>& AxisMap)
{
	ASSERT(AxisMap.size() < 128);

	CLogger Logger;
	CString strLogPath = SZ_LOG_FILE_PATH;
	strLogPath += _T("SoftServoEtherCAT_InitBoard.log");
	Logger.SetFileName(strLogPath);

	Logger.LOG(_T("%s called"), _T(__FUNCTION__));


	if (g_pecApi == nullptr)
		g_pecApi = new /*EcAPI*/Ecat(&g_Wmx3Lib);

	// Restart the engine.
// 	g_Wmx3Lib.RestartEngine("C:\\Program Files\\SoftServo\\WMX3\\", INFINITE);
// 	//g_Wmx3Lib.StartEngine("C:\\Program Files\\SoftServo\\WMX3\\", INFINITE);
// 
// 	// Create device.
// 	g_Wmx3Lib.CreateDevice("C:\\Program Files\\SoftServo\\WMX3\\", DeviceType::DeviceTypeNormal, INFINITE);
// 	g_Wmx3Lib.SetDeviceName("SLT_MC");
// 
// 	//모터에서 Watchdog을 확인 안 하도록 수정
// 	g_Wmx3Lib.SetWatchdog(0);

	// Get created device state.
// 	wmx3Api::DevicesInfoA devInfo;
// 	g_Wmx3Lib.GetAllDevices(&devInfo);
// 
// 	if (!OneAxis_SoftServoEtherCAT::StartCommunication())
// 	{
// 		AfxMessageBox(_T("Failed to initialize SoftServo"), MB_ICONERROR | MB_SYSTEMMODAL | MB_OK);
// //		ExitProcess(0);
// 	}


	CoreMotionStatus CmStatus;
	g_pWmx3Lib_cm->GetStatus(&CmStatus);


	OneAxis_SoftServoEtherCAT::m_nConnectSuccess = TRUE;

	return true;
}

bool OneAxis_SoftServoEtherCAT::Close()
{
	ComThreadCtrl(FALSE);	// thread 해제
	
	// 통신 연결 해제

	g_Wmx3Lib.StopCommunication(INFINITE);
	g_Wmx3Lib.CloseDevice();

	g_Wmx3Lib.SetWatchdog(0);
	
	if (g_pWmx3Lib_cm != NULL)
	{
		delete g_pWmx3Lib_cm;
		g_pWmx3Lib_cm = NULL;
	}

	if (g_pAdvMotion != nullptr)
	{
		SAFE_DELETE(g_pAdvMotion);
	}

	return true;
}

bool OneAxis_SoftServoEtherCAT::StartCommunication()
{
	int nErrCode = (int)g_Wmx3Lib.StartCommunication(INFINITE);

	if (nErrCode != CoreMotionErrorCode::None)
		return false;

	return true;
}
double OneAxis_SoftServoEtherCAT::GetReadActualTorque_PDO(int nAxisNo)
{
	CoreMotionStatus CmStatus;
	g_pWmx3Lib_cm->GetStatus(&CmStatus);
	CoreMotionAxisStatus cmAxis = CmStatus.axesStatus[nAxisNo];
	return cmAxis.actualTorque;
}
int OneAxis_SoftServoEtherCAT::ReadActualPeakTorqueData(int nAxisNo, _eTorqueType eType)
{

	unsigned char chValue[4]; //MAX Size
	std::fill_n(chValue, 4, 0);
	int nArrSize = 0;
	int index = 0;
	CString strType;
	// 	if (eType == _eTorqueType::CURRENT_ACTUAL_TORQUE)
	// 	{
	// 		index = 0x6074;
	// 		strType = "CURRENT ACTUAL TORQUE";
	// 		nArrSize = 4;
	// 	}
	if (eType == _eTorqueType::MAX_ACTUAL_TORQUE)
	{
		index = 0x6077;
		strType = "MAX ACTUAL TORQUE";
		nArrSize = 2;
	}
	if (eType == _eTorqueType::CURRENT_PEAK_TORQUE)
	{
		index = 0x2A2c; //추후...
		strType = "CURRENT PEAK TORQUE";
		nArrSize = 4;
	}
	if (eType == _eTorqueType::MAX_PEAK_TORQUE)
	{
		index = 0x2A2D;
		strType = "MAX PEAK TORQUE";
		nArrSize = 4;
	}

	unsigned int ActualSize = 0;
	unsigned int ErrorCode;
	long Result = g_pecApi->SdoUpload(nAxisNo, index, 0, nArrSize, chValue, &ActualSize, &ErrorCode, 1000);
	if (Result != 0)
	{
		m_MtLog.LOG(_T("Fail to Read Torque: PhyAxis %d, Result %d, ErrorCode %d, Type %s"), nAxisNo, Result, ErrorCode, strType);
		return 0;
	}
	return ConvertToValidData_Torque(chValue, nArrSize, eType);
}
string OneAxis_SoftServoEtherCAT::decimalToBinary(int Navlue)
{

	// C++ integers are already in 2-complements so you only have to 
	// extract the bits.
	if ((Navlue < -128) || (Navlue >= 128)) {
		return "";
	}
	string res = "";
	for (int i = 0; i < 8; ++i) {
		res = string(1, (char)(((Navlue >> i) & 1) + '0')) + res;
	}
	return res;
}

int OneAxis_SoftServoEtherCAT::binaryToDecimal(string strValue)
{
	// The most significant bit is the signal
	int result = -((int)(strValue[0] - '0'));

	for (int i = 1; i < strValue.size(); ++i) {
		result = 2 * result + ((int)(strValue[i] - '0'));
	}
	return result;
}
int OneAxis_SoftServoEtherCAT::ConvertToValidData_Torque(unsigned char Data[], int nSizeArr, _eTorqueType eType)
{
	int nResult = 0;
	for (int i = 0; i < nSizeArr; i++)
	{
		nResult |= (int)(Data[i] << (8 * i));
	}

	if (nResult < 0)
	{
		nResult = 0;
	}
	if (eType == CURRENT_ACTUAL_TORQUE ||
		eType == MAX_ACTUAL_TORQUE)
	{
		if (nResult > 3600) // 3400 -> 340% 임. 350% 넘으면 Alarm발생함
		{
			// 			int nRawInt = 0;
			// 			std::memcpy(&nRawInt, Data, sizeof(int));
			// 			m_MtLog.LOG(_T("byte %c =[0]"), Data[0]);
			// 			m_MtLog.LOG(_T("byte %c =[1]"), Data[1]);
			string binary = decimalToBinary(Data[0]);
			nResult = binaryToDecimal(binary);
		}
		else
		{
			nResult = nResult / 10;
		}
	}

	return nResult;
}

//==============================================================================
//
//==============================================================================
OneAxis_SoftServoEtherCAT::OneAxis_SoftServoEtherCAT()
{
	m_bPlviModuleServo = false;
	m_nOri_Flag = eMotorOrgFlag_NG;
	m_nOri_Step = eOrg_Unspecified;
	// LOG File Name
	char szLogFileName[ 512 ] = { 0, };
	sprintf_s(szLogFileName, sizeof(szLogFileName), _T("%sMotor.log"), SZ_LOG_FILE_PATH);
	//StringCchPrintf( szLogFileName, _countof( szLogFileName ), _T( "%sMotor.log" ), SZ_LOG_FILE_PATH );
	m_MtLog.SetFileName(szLogFileName);
}

//==============================================================================
//
//==============================================================================
OneAxis_SoftServoEtherCAT::~OneAxis_SoftServoEtherCAT()
{
	ServoOff();
}


/*=============================================================================
  AUTHOR   : 김선진
  NAME     : Move
  PARAMS   : move_pos    = 움직이고자 하는 위치		[um    ] (미크론 단위로 scale_factor로 나누어야 실제 pulse값이 됨)
             speed       = 움직이려는 축의 속도      [um/sec]
			 accel       = 움직이려는 축의 가속도		[msec  ]
			 dccel       = 움직이려는 축의 감속도		[msec  ]
  RETURN   :  0          = OK
              1          = Board Init Fail or 해당 축 disable
             -1          = Origin이 안 잡힌 상태
			 -2          = Servo가 On이 안되어 있는 상태 	SLT_MC_D.exe!CTransferCmd_Initial::State_Executing() 줄 252	C++

			 -3          = AMP FAULT 상태
			 -4          = Limit를 벗어나는 상태
			 -5          = Limit Pulse를 넘는 상태 ( 모터 RPM Over )
  COMMENTS : 해당 축을 움직이려는 함수 ( Motion Done을 확인해야 함 )
==============================================================================*/
int OneAxis_SoftServoEtherCAT::Move(double a_dPos_um, double a_dSpeed_umps, int a_nAccel, int a_nDccel, BOOL bApplyBacklash, _eMotorIntertlock nInterlockType /*= _eMotorIntertlock::eMotorInterlock_Operation*/)
{
	if( m_cbInterLock_Move != NULL && nInterlockType != _eMotorIntertlock::eMotorInterlock_All_Ignore ) {
		//ERR_CODE nInterLockErr = m_cbInterLock_Move( a_dPos_um, a_dSpeed_umps, a_nAccel, a_nDccel, nInterlockType, bPcdBacklashMode);

		// 파라미터값 확인 필요 
		ERR_CODE nInterLockErr = m_cbInterLock_Move(a_dPos_um, a_dSpeed_umps, a_nAccel, a_nDccel);
		if( nInterLockErr != ERR_SUCCESS ) {
			return nInterLockErr;
		}
	}

	if (OneAxis_SoftServoEtherCAT::ChkCommunication() != TRUE)
	{
		return ERR_MOTOR_SYS_NOT_CONNECTED;	// API 함수 호출
	}

	if (!GetServoStat()) {
		return ERR_MOTOR_CMN_SERVO_OFF;
	}

	if (m_eMotorSyncType == eMotorSyncType_Slave)
		return ERR_MOTOR_CMN_IN_SLAVE_MODE;		// 이부분은 Motor가 Slave로 설정 되어 있다는 Message로 변경한다.

	CoreMotionStatus CmStatus;
	int nChkErrorCode = (int)g_pWmx3Lib_cm->GetStatus(&CmStatus);
	CoreMotionAxisStatus cmAxis = CmStatus.axesStatus[m_nAxisNumPhy];	// move 동작전 모터 상태 확인

	if (!g_pMotion_Cont_Imp->GetInitBoardStatus()) {
		return ERR_MOTOR_SYS_BOARD_INITIALIZTION_FAILURE;
	}

	if (m_bMotorDisable) {
		return ERR_MOTOR_CMN_DISABLED;
	}

	if (cmAxis.ampAlarm == TRUE)
	{
		// return cmAxis.ampAlarmCode;	// servo driver maker alarm 확인
		return ERR_MOTOR_CMN_AMP_FAULT;
	}

	//원점 Chk
	if(nInterlockType != _eMotorIntertlock::eMotorInterlock_All_Ignore )
	{
		if (!cmAxis.homeDone)			// 원점 상태
		{
			return ERR_MOTOR_CMN_NOT_ORG_CHECKED;
		}
	}



	ClearStatus();


	if (m_bDegeeCtrl == TRUE)	// (각)도 단위로
	{
		a_dPos_um /= 1000.0;
	}

	// 20190405 ihc. backlash 적용 
	if (bApplyBacklash == true)
	{	
		double dBacklashOffset = 0.0;
		if(m_nAxisNumPhy == eAXIS_TEST_PP_1_X || m_nAxisNumPhy == eAXIS_TEST_PP_1_X2)
			dBacklashOffset = GetBacklashOffset_TestPP_X1(a_dPos_um, a_dSpeed_umps);
		else
			dBacklashOffset = GetBacklashOffset(a_dPos_um, a_dSpeed_umps);

		CString strBacklashLog;
		strBacklashLog.Format(_T("%s[PhyNo %d] [BACKLASH APPLY] MovePos: %.3fmm, Offset: %.3fmm"), m_szAxisName, m_nAxisNumPhy, a_dPos_um / 1000, dBacklashOffset / 1000);
		m_MtLog.LOG(strBacklashLog);
		a_dPos_um += dBacklashOffset;
	}


	if (nInterlockType != eMotorInterlock_All_Ignore)
	{
		// 움직이고자 하는 위치가 s/w limit를 벗어나는 위치인지를 확인
		if (a_dPos_um > m_dSwLimit_Posi_um) {
			return ERR_MOTOR_CMN_SOFTWARE_POSITIVE_LIMIT_HIT;
		}
		if (a_dPos_um < m_dSwLimit_Nega_um) {
			return ERR_MOTOR_CMN_SOFTWARE_NEGATIVE_LIMIT_HIT;
		}
	}
	

	if (a_dSpeed_umps <= 0) {	// 속도 설정 안했을 시
		a_dSpeed_umps = m_dCmdVelocity;
	}
	if (a_nAccel <= 0) {	// 가속 설정 안했을 시
		a_nAccel = m_nCmdAcc;
	}
	if (a_nDccel <= 0) {	// 감속 설정 안했을 시
		a_nDccel = m_nCmdDcc;
	}
	if (a_dSpeed_umps == 1) {
		a_dSpeed_umps = m_dCmdVelocity;
		a_nAccel = 100;
		a_nDccel = 100;
	}
	// 최악의 경우 방지
	if (a_nAccel <= 100) {	// 가속 들어가지 않는거에 대비 하여
		a_nAccel = 100;
	}
	if (a_nDccel <= 100) {	// 감속 들어가지 않는거에 대비 하여
		a_nDccel = 100;
	}

	if (m_bDegeeCtrl == TRUE)	// (각)도 단위로
	{
		if (a_dSpeed_umps > 720)	// 회전축은 이동속도가 초당 90도 보다 빠르면 안되어서 제한함.
		{
			a_dSpeed_umps = 720;
		}
	}

	//double dTargetPos_pulse = (double)m_nDir_Move * a_dPos_um / (m_dScaleFactor + m_dScaleFactorOffset);	//pulse
	//double dSpeed_pps = a_dSpeed_umps;
	//int    nAccel_mmc = a_nAccel / MMC_ACC_TIME_UNIT;
	//int    nDccel_mmc = a_nDccel / MMC_ACC_TIME_UNIT;

	double	dMovePos = a_dPos_um / 1000.0;
	SaveMotorMoveLog(a_dPos_um, dMovePos, 0);	// mm 단위로 저장

	m_dTarget_pos_um = a_dPos_um;	//이동할 위치
	m_dLast_pos = m_dTarget_pos_um;

// 	if (bAutoRunning == TRUE)
// 	{
// 		m_dLast_pos = m_dTarget_pos_um;
// 	}

	double dVelocityPps = a_dSpeed_umps / (m_dScaleFactor + m_dScaleFactorOffset); // pulse로 변환

	Motion::PosCommand posCommand = Motion::PosCommand();

	posCommand.target						= a_dPos_um / (m_dScaleFactor + m_dScaleFactorOffset);	// pulse로 변환
	posCommand.axis							= m_nAxisNumPhy;
	
// 	if(m_nAxisNumPhy == eAXIS_TEST_PP_1_X
// 		|| m_nAxisNumPhy == eAXIS_TEST_PP_1_X2
// 		|| m_nAxisNumPhy == eAXIS_TEST_PP_2_X
// 		|| m_nAxisNumPhy == eAXIS_TEST_PP_2_X2)
// 	{
// 		posCommand.profile.type					= ProfileType::SCurve;
// 	}
	
	posCommand.profile.velocity				= dVelocityPps;
	posCommand.profile.startingVelocity		= 0;
	posCommand.profile.endVelocity			= 0;
	posCommand.profile.acc					= dVelocityPps * 1000 / a_nAccel;
	posCommand.profile.dec					= dVelocityPps * 1000 / a_nDccel;
	posCommand.profile.jerkAcc				= 1000;
	posCommand.profile.jerkAccRatio			= 0.1;
	posCommand.profile.jerkDec				= 1000;
	posCommand.profile.jerkDecRatio			= 0.1;

	int nErrorCode = (int)g_pWmx3Lib_cm->motion->StartPos(&posCommand);

	if (nErrorCode != CoreMotionErrorCode::None)
	{
		CString strLog;
		strLog.Format(_T("%s[PhyNo %d] ErrorCode: %d, MovePos: %.3fmm, posCommand { target %.3f, axis %d, vel %f, acc %f, dcc %f }"),
			m_szAxisName, m_nAxisNumPhy, nErrorCode, a_dPos_um / 1000,
			posCommand.target, posCommand.axis, posCommand.profile.velocity, posCommand.profile.acc, posCommand.profile.dec);
		m_MtLog.LOG(strLog);
	}

	return NormalizeError(nErrorCode);




	//int nCnt = frames_left(m_nAxisNumPhy); 받은 커맨드 털기
	//return NoInterlock_Move( a_dPos_um, a_dSpeed_umps, a_nAccel, a_nDccel, bAutoRunning, nInterlockType);	// 20190405 ihc. backlash test 여부 전달
}


//TWSL421 현재 사용 안함.
ERR_CODE OneAxis_SoftServoEtherCAT::NoInterlock_Move(double a_dPos_um, double a_dSpeed_umps, int a_nAccel, int a_nDccel, BOOL bAutoRunning /*= FALSE*/,_eMotorIntertlock nInterlockType /*= _eMotorIntertlock::eMotorInterlock_Operation*/)
{
	if (OneAxis_SoftServoEtherCAT::ChkCommunication() != TRUE)
	{
		return ERR_MOTOR_SYS_NOT_CONNECTED;	// API 함수 호출
	}
	
	CoreMotionStatus CmStatus;
	int nChkErrorCode = (int)g_pWmx3Lib_cm->GetStatus(&CmStatus);
	CoreMotionAxisStatus cmAxis = CmStatus.axesStatus[m_nAxisNumPhy];	// move 동작전 모터 상태 확인

	if (!g_pMotion_Cont_Imp->GetInitBoardStatus()) {
		return ERR_MOTOR_SYS_BOARD_INITIALIZTION_FAILURE;
	}

	if (m_bDegeeCtrl == TRUE)	// (각)도 단위로
	{
		a_dPos_um /= 1000.0;
	}

	if (m_bMotorDisable) {
		return ERR_MOTOR_CMN_DISABLED;
	}

	//원점 Chk
	if (!cmAxis.homeDone)			// 원점 상태
	{
		return ERR_MOTOR_CMN_NOT_ORG_CHECKED;
	}
// 	if (nInterlockType != eMotorInterlock_ForceMove)
// 	{
// 		if (!cmAxis.homeDone)			// 원점 상태
// 		{
// 			return ERR_MOTOR_CMN_NOT_ORG_CHECKED;
// 		}
// 	}
		
	if (!GetServoStat()) {
		return ERR_MOTOR_CMN_SERVO_OFF;
	}
	if (m_eMotorSyncType == eMotorSyncType_Slave)
		return ERR_MOTOR_CMN_IN_SLAVE_MODE;		// 이부분은 Motor가 Slave로 설정 되어 있다는 Message로 변경한다.

	ClearStatus();
	
	// 20190405 ihc. backlash 적용 
	if (m_bApplyBacklash == true)
	{
		double dBacklashOffset = 0.0;
		if (m_nAxisNumPhy == eAXIS_TEST_PP_1_X || m_nAxisNumPhy == eAXIS_TEST_PP_1_X2)
			dBacklashOffset = GetBacklashOffset_TestPP_X1(a_dPos_um, a_dSpeed_umps);
		else
			dBacklashOffset = GetBacklashOffset(a_dPos_um, a_dSpeed_umps);

		CString strBacklashLog;	
		strBacklashLog.Format(_T("%s[PhyNo %d] [BACKLASH APPLY] MovePos: %.3fmm, Offset: %.3fmm"), m_szAxisName, m_nAxisNumPhy, a_dPos_um / 1000, dBacklashOffset / 1000);
		m_MtLog.LOG(strBacklashLog);
		a_dPos_um += dBacklashOffset;
	}
	
	else if ( 0 )//  im hyen chang item low priority // eApplyBacklashCondition == _eApplyBacklashCondition::eApplyBacklashCondition_BacklashTest)
	{
		double dBacklashOffset = 0.0;
		if (m_nAxisNumPhy == eAXIS_TEST_PP_1_X || m_nAxisNumPhy == eAXIS_TEST_PP_1_X2)
			dBacklashOffset = GetBacklashOffset_TestPP_X1(a_dPos_um, a_dSpeed_umps);
		else
			dBacklashOffset = GetBacklashOffset(a_dPos_um, a_dSpeed_umps);

		CString strBacklashLog;
		strBacklashLog.Format(_T("%s[PhyNo %d] [BACKLASH APPLY] MovePos: %.3fmm, Offset: %.3fmm"), m_szAxisName, m_nAxisNumPhy, a_dPos_um / 1000, dBacklashOffset / 1000);
		m_MtLog.LOG(strBacklashLog);
		a_dPos_um += dBacklashOffset;
	}

	if (cmAxis.ampAlarm == TRUE)
	{
		// return cmAxis.ampAlarmCode;	// servo driver maker alarm 확인
		return ERR_MOTOR_CMN_AMP_FAULT;
	}

	if (nInterlockType != eMotorInterlock_ForceMove)
	{
		// 움직이고자 하는 위치가 s/w limit를 벗어나는 위치인지를 확인
		if (a_dPos_um > m_dSwLimit_Posi_um) {
			return ERR_MOTOR_CMN_SOFTWARE_POSITIVE_LIMIT_HIT;
		}
		if (a_dPos_um < m_dSwLimit_Nega_um) {
			return ERR_MOTOR_CMN_SOFTWARE_NEGATIVE_LIMIT_HIT;
		}
	}

	if (a_dSpeed_umps <= 0) {	// 속도 설정 안했을 시
		a_dSpeed_umps = m_dCmdVelocity;
	}
	if (a_nAccel <= 0) {	// 가속 설정 안했을 시
		a_nAccel = m_nCmdAcc;
	}
	if (a_nDccel <= 0) {	// 감속 설정 안했을 시
		a_nDccel = m_nCmdDcc;
	}
	if (a_dSpeed_umps == 1) {
		a_dSpeed_umps = m_dCmdVelocity;
		a_nAccel = 100;
		a_nDccel = 100;
	}
	// 최악의 경우 방지
	if (a_nAccel <= 100) {	// 가속 들어가지 않는거에 대비 하여
		a_nAccel = 100;
	}
	if (a_nDccel <= 100) {	// 감속 들어가지 않는거에 대비 하여
		a_nDccel = 100;
	}

	//double dTargetPos_pulse = (double)m_nDir_Move * a_dPos_um / (m_dScaleFactor + m_dScaleFactorOffset);	//pulse
	//double dSpeed_pps = a_dSpeed_umps;
	//int    nAccel_mmc = a_nAccel / MMC_ACC_TIME_UNIT;
	//int    nDccel_mmc = a_nDccel / MMC_ACC_TIME_UNIT;
	
	double	dMovePos = a_dPos_um / 1000.0;
	SaveMotorMoveLog(a_dPos_um, dMovePos, 0 );	// mm 단위로 저장
	
	m_dTarget_pos_um = a_dPos_um;	//이동할 위치

	if (bAutoRunning == TRUE)
	{
		m_dLast_pos = m_dTarget_pos_um;
	}
	
	double dVelocityPps = a_dSpeed_umps / (m_dScaleFactor + m_dScaleFactorOffset); // pulse로 변환

	Motion::PosCommand posCommand = Motion::PosCommand();

	posCommand.target						= a_dPos_um / (m_dScaleFactor + m_dScaleFactorOffset);	// pulse로 변환
	posCommand.axis							= m_nAxisNumPhy;
	posCommand.profile.velocity				= dVelocityPps;
	posCommand.profile.startingVelocity		= 0;
	posCommand.profile.endVelocity			= 0;
	posCommand.profile.acc					= dVelocityPps * 1000 / a_nAccel;
	posCommand.profile.dec					= dVelocityPps * 1000 / a_nDccel;
	posCommand.profile.jerkAcc				= 1000;
	posCommand.profile.jerkAccRatio			= 0.5;
	posCommand.profile.jerkDec				= 1000;
	posCommand.profile.jerkDecRatio			= 0.5;
		
	int nErrorCode = (int)g_pWmx3Lib_cm->motion->StartPos(&posCommand);

	if (nErrorCode != CoreMotionErrorCode::None)
	{
		CString strLog;
		strLog.Format(_T("%s[PhyNo %d] ErrorCode: %d, MovePos: %.3fmm, posCommand { target %.3f, axis %d, vel %f, acc %f, dcc %f }"), 
			m_szAxisName, m_nAxisNumPhy, nErrorCode, a_dPos_um / 1000, 
			posCommand.target, posCommand.axis, posCommand.profile.velocity, posCommand.profile.acc, posCommand.profile.dec);
		m_MtLog.LOG(strLog);
	}

	return NormalizeError(nErrorCode);
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : R_Move
  PARAMS   : move_pos = 움직이고자 하는 위치		[um    ] (미크론 단위로 scale_factor로 나누어야 실제 pulse값이 됨)
             speed    = 움직이려는 축의 속도		[um/sec]		
			 bPcdBacklashMode = Pcd,Backlash 측정 시 본 함수가 호출된 경우 True
  RETURN   :  0       = OK
              1       = Board Init Fail or 해당 축 disable
             -1       = Origin이 안 잡힌 상태
			 -2       = Servo가 On이 안되어 있는 상태
			 -3       = AMP FAULT 상태
			 -4       = Limit를 벗어나는 상태
			 -5       = Limit Pulse를 넘는 상태 ( 모터 RPM Over )
  COMMENTS : 현재 위치부터 설정 값 만큼(move_pos) 이동 한다.
==============================================================================*/
ERR_CODE OneAxis_SoftServoEtherCAT::R_Move(double a_dPos_um, double a_dSpeed_umps, int nAcc_msec, int nDec_msec, BOOL bPcdBacklashMode)
{
	if (OneAxis_SoftServoEtherCAT::ChkCommunication() != TRUE)
	{
		return ERR_MOTOR_SYS_NOT_CONNECTED;
	}
	
	if (!g_pMotion_Cont_Imp->GetInitBoardStatus()) {
		return ERR_MOTOR_SYS_BOARD_INITIALIZTION_FAILURE;
	}
	if (m_bMotorDisable) {
		return ERR_MOTOR_CMN_DISABLED;
	}

	CoreMotionStatus CmStatus;

	int nChkErrorCode = (int)g_pWmx3Lib_cm->GetStatus(&CmStatus);

	CoreMotionAxisStatus cmAxis = CmStatus.axesStatus[m_nAxisNumPhy];	// move 동작전 모터 상태 확인
	if (!cmAxis.homeDone)			// 원점 상태
	{
		return ERR_MOTOR_CMN_NOT_ORG_CHECKED;
	}

	if(!GetServoStat())
	{
		return ERR_MOTOR_CMN_SERVO_OFF;
	}

	if (m_eMotorSyncType == eMotorSyncType_Slave)
		return ERR_MOTOR_CMN_IN_SLAVE_MODE;		// 이부분은 Motor가 Slave로 설정 되어 있다는 Message로 변경한다.
		
	// 움직이고자 하는 위치가 s/w limit를 벗어나는 위치인지를 확인 
	
	double dTargetPos_um = 0.0;
	GetCurrentPos(dTargetPos_um);
	dTargetPos_um += a_dPos_um;

	if (dTargetPos_um > m_dSwLimit_Posi_um)
		return ERR_MOTOR_CMN_SOFTWARE_POSITIVE_LIMIT_HIT;
	if (dTargetPos_um < m_dSwLimit_Nega_um)
		return ERR_MOTOR_CMN_SOFTWARE_NEGATIVE_LIMIT_HIT;

	ClearStatus();
		
	if (cmAxis.ampAlarm == TRUE)
	{
		return ERR_MOTOR_CMN_AMP_FAULT;
//		return cmAxis.ampAlarmCode;	// servo driver maker alarm 확인
	}

	if( m_cbInterLock_Move != NULL ){
		//ERR_CODE nInterLockErr = m_cbInterLock_Move(dTargetPos_um, a_dSpeed_umps, m_nCmdAcc, m_nCmdAcc, _eMotorIntertlock::eMotorInterlock_Operation, bPcdBacklashMode);
		ERR_CODE nInterLockErr = m_cbInterLock_Move(dTargetPos_um, a_dSpeed_umps, m_nCmdAcc, m_nCmdAcc);
		if( nInterLockErr != ERR_SUCCESS ){
			return nInterLockErr;
		}
	}

	if( nAcc_msec < 40 ) nAcc_msec = 40;
	if( nDec_msec < 40 ) nDec_msec = 40;

	if(g_pMotion_Cont_Imp->GetInitBoardStatus())
	{
		Motion::PosCommand posCommand = Motion::PosCommand();

		double dVelocityPps = a_dSpeed_umps / (m_dScaleFactor + m_dScaleFactorOffset);	// pulse로 변환

		posCommand.target = a_dPos_um / (m_dScaleFactor + m_dScaleFactorOffset);	// pulse로 변환
		posCommand.axis = m_nAxisNumPhy;
		posCommand.profile.velocity = dVelocityPps;
		posCommand.profile.startingVelocity = 0;
		posCommand.profile.endVelocity = 0;
		posCommand.profile.acc = dVelocityPps * 1000 / nAcc_msec; //2500; //a_nAccel;
		posCommand.profile.dec = dVelocityPps * 1000 / nDec_msec;
		posCommand.profile.jerkAcc = 1000;
		posCommand.profile.jerkAccRatio = 0.5;
		posCommand.profile.jerkDec = 1000;
		posCommand.profile.jerkDecRatio = 0.5;

		//가감속을 ms단위에서 Pulse 단위로 변경해주자...
		//Acc = abs((int)(Vel * 1000 / Acc));
		//Dec = abs((int)(Vel * 1000 / Dec));

		// Move to Position
		//m_wmxLib->basicMotion->StartPos(axis_num, move_pos, Vel, Acc, Dec);
		//iErrCode = g_Wmx3Lib.jerkRatioMotion->StartPos(axis_num, ProfileType::JerkLimited, move_pos, Vel, Acc, Dec, 0.5, 0.5, 0, 0);

		// [NOTE] 검증 필요
		int nErrorCode = (int)g_pWmx3Lib_cm->motion->StartMov(&posCommand);

		//SaveMotorMoveLog(dTargetPos_pulse, dMovePos_mm, 1);
		m_dLast_pos = m_dTarget_pos_um = dTargetPos_um;
		//TRACE("start_tr_move(%d, %f, %f, %d)\n", m_nAxisNumPhy, dTargetPos_pulse, dSpeed_pps, nAccel_mmc, nDecel_mmc);

		ERR_CODE Error = NormalizeError(nErrorCode);

		if (Error != ERR_SUCCESS)	// Error
		{
			return Error;//함수 호출 실패
		}
		//if (nErrorCode != MMC_OK) {
		//	return ERR_MOTOR_CMN_BOARD_API_FAILURE;
		//}
	}
	return ERR_SUCCESS;
}


/*=============================================================================
  AUTHOR   : 김선진
  NAME     : Origin
  PARAMS   : None
  RETURN   :  0       = Origin 진행 중.
             -1       = Origin이 잡혀 있는 상태
			 0>       = Error
                        Origin 시 문제가 있는 상태
			            Servo가 On이 안되어 있는 상태
			            AMP FAULT 상태
			            Limit를 벗어나는 상태
			            Limit Pulse를 넘는 상태 ( 모터 RPM Over )
  COMMENTS : 현재 위치부터 설정 값 만큼(move_pos) 이동 한다.
==============================================================================*/
ERR_CODE OneAxis_SoftServoEtherCAT::Origin()
{
	ERR_CODE Error = ERR_SUCCESS;
	if (OneAxis_SoftServoEtherCAT::ChkCommunication() != TRUE)
	{
		return ERR_MOTOR_SYS_NOT_CONNECTED;
	}

	if (!g_pMotion_Cont_Imp->GetInitBoardStatus()) {
		return ERR_MOTOR_SYS_BOARD_INITIALIZTION_FAILURE;
	}
	if (m_bMotorDisable) {
		return ERR_MOTOR_CMN_DISABLED;
	}
	if (!IsServoOn()) {
		return ERR_MOTOR_CMN_SERVO_OFF;
	}
	if (m_cbInterLock_Org != nullptr) {
		int nInterLockErr = m_cbInterLock_Org();
		if (nInterLockErr != 0) {
			return nInterLockErr;
		}
	}

	if (!m_nCmdAcc_Ori || !m_nCmdDcc)
		return ERR_MOTOR_CMN_ORIGIN_FAILURE;

	// Cmd 에서 계속 호출됨. Origin 완료 되도 
	ERR_CODE eErrCode = ERR_SUCCESS;
	BOOL bOrgComplet = IsOrgCompleted(&eErrCode);

	if (bOrgComplet == TRUE) {
		return ERR_SUCCESS;
	}

	if (m_nOri_Step == eOrg_Unspecified)
	{
		// 마지막으로 Origin() 완료된 후 처음부터 진행할 경우
		m_nOri_Step = eOrg_Start;
	}

	switch (m_nOri_Step)
	{
	case eOrg_Start:
		{
			double dGearRatioNumerator = 0;
			double dGearRatioDenominator = 0;
			int nErrorCode = ErrorCode::None;

			nErrorCode = g_pWmx3Lib_cm->config->GetGearRatio(m_nAxisNumPhy, &dGearRatioNumerator, &dGearRatioDenominator);
			if (nErrorCode != ErrorCode::None)
			{
				m_nOri_Step = eOrg_Unspecified;
				return ERR_MOTOR_CMN_ORIGIN_FAILURE;
			}

			int nPulsePerRound = 0;
			if (m_nDouble_chk_um == DEF_ON) {
				int nCalGearRation = 0;
				if (m_strServoMaker.Compare(_T("FUJI")) == 0)	// FUJI
				{
					nCalGearRation = 1 << 24;
				}
				else if (m_strServoMaker.Compare(_T("EZI_SERVO")) == 0)	// ezi servo 공통
				{
					nCalGearRation = 10000;
				}
				else if (m_strServoMaker.Compare(_T("RSA_TRI")) == 0)
				{
					nCalGearRation = 250000;
				}
				else // RSA
				{
					nCalGearRation = 1 << 23;
				}
				nPulsePerRound = (int)(dGearRatioDenominator * nCalGearRation / dGearRatioNumerator);
			}
			
			if (m_bDegeeCtrl == TRUE)	// (각)도 단위로
			{
				if (m_dCmdVelocity_Ori > 90)	// 회전축은 이동속도가 초당 90도 보다 빠르면 안되어서 제한함.
				{
					m_dCmdVelocity_Ori = 90;
				}
			}

			double dVelocityPps = m_dCmdVelocity_Ori / (m_dScaleFactor + m_dScaleFactorOffset);

// 			//Rotate 속도 계산.
// 			if (m_bDegeeCtrl == TRUE) {
// 				double dBufvelocityPps = dVelocityPps;    // pulse/sec
// 				dVelocityPps = m_dScaleFactor * dBufvelocityPps; //   1degree/ 1sec로 환산.
// 			}


			// 가속 / 감속 동일하게 
			// 업체에서 보통은 속도에 10배 가량을 쓴다고 함.
			double dOriginAcc = dVelocityPps * 1000 / m_nCmdAcc_Ori;			
			double dOriginDcc = dVelocityPps * 1000 / m_nCmdAcc_Ori;//m_nCmdDcc;

			// Motor Homing 전에 원하는 값으로 Setting한다.
			Config::HomeParam pParam;
			g_pWmx3Lib_cm->home->SetHomeDone(m_nAxisNumPhy, 0);
			nErrorCode = g_pWmx3Lib_cm->config->GetHomeParam(m_nAxisNumPhy, &pParam);
			if (nErrorCode != ErrorCode::None)
			{
				m_nOri_Step = eOrg_Unspecified;
				return ERR_MOTOR_CMN_ORIGIN_FAILURE;
			}

			pParam.homeType = Config::HomeType::T::HSHS; // HSHS다 현재 Home 두번 감지하는 것
			pParam.clearHomeDoneOnCommStop = true;
			pParam.clearHomeDoneOnServoOff = true;
			pParam.homingVelocitySlow = dVelocityPps / 3.0;
			pParam.homingVelocityFast = dVelocityPps;
			pParam.homingVelocitySlowAcc = dOriginAcc / 5.0;
			pParam.homingVelocitySlowDec = dOriginDcc / 5.0;
			pParam.homingVelocityFastAcc = dOriginAcc;
			pParam.homingVelocityFastDec = dOriginDcc;
			// Homing 시 Home 1회 감지 후 반대방향으로 1바퀴 이동 필요 (Backlash 대응)
			pParam.homingReverseDistance = nPulsePerRound;
			pParam.homeShiftVelocity = dVelocityPps/2.0;
			pParam.homeShiftAcc = dOriginAcc/2.0;
			pParam.homeShiftDec = dOriginDcc/2.0;
			pParam.homeShiftDistance = 0;

			// TBL_AXIS_PARAM 내의 DIR_ZR이 < 0일 경우 - Negative, > 0일 경우 - Positive
			ASSERT(m_nDir_Ori != 0);
			pParam.homeDirection = m_nDir_Ori < 0 ? Config::HomeDirection::Negative : Config::HomeDirection::Positive;

			nErrorCode = g_pWmx3Lib_cm->config->SetHomeParam(m_nAxisNumPhy, &pParam);
			if (nErrorCode != ErrorCode::None)
			{
				m_nOri_Step = eOrg_Unspecified;
				return ERR_MOTOR_CMN_ORIGIN_FAILURE;
			}
			
			nErrorCode = (int)g_pWmx3Lib_cm->home->StartHome(m_nAxisNumPhy);
			ERR_CODE Error = NormalizeError(nErrorCode);

			if (Error != ERR_SUCCESS)
			{
				m_nOri_Step = eOrg_Unspecified;
				return ERR_MOTOR_CMN_ORIGIN_FAILURE;
			}

			// Homing 중이다.
			m_nOri_Flag = eMotorOrgFlag_ING;
			m_nOri_Step = eOrg_Wait;
		}break;

	case eOrg_Wait:
		{
			if (m_bDegeeCtrl == TRUE) {
			
			}


			ERR_CODE eErrorCode = ERR_SUCCESS;
			int nErr = GetOriginCheck(&eErrorCode);
			if (nErr == TRUE )
			{
				m_nOri_Step = eOrg_Unspecified;
				m_nOri_Flag = eMotorOrgFlag_OK;
				return ERR_SUCCESS;
			}
			else
			{
				if (eErrorCode == ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					return ERR_SUCCESS;
				}

				Error = eErrorCode;

				if (Error != ERR_SUCCESS)
				{
					m_nOri_Step = eOrg_Unspecified;
					m_nOri_Flag = eMotorOrgFlag_NG;
				}
			}
		}break;

	default:
		ASSERT(false);
	}

	return Error;
}
ERR_CODE OneAxis_SoftServoEtherCAT::GetOriginCheck(ERR_CODE *pError/* = NULL*/)
{
	ERR_CODE Error = ERR_SUCCESS;
	BOOL bResult = FALSE;
	CoreMotionStatus CmStatus;

	if (OneAxis_SoftServoEtherCAT::ChkCommunication() != TRUE)
		Error = ERR_MOTOR_SYS_NOT_CONNECTED;
	else if (!g_pMotion_Cont_Imp->GetInitBoardStatus())
		Error = ERR_MOTOR_SYS_BOARD_INITIALIZTION_FAILURE;
	else if (m_bMotorDisable)
		Error = ERR_MOTOR_CMN_DISABLED;

	if (Error == ERR_SUCCESS)
	{
		g_pWmx3Lib_cm->GetStatus(&CmStatus);
		auto& Status = CmStatus.axesStatus[m_nAxisNumPhy];
#if 1
		// 
		// Homing 중일 때:
		// opState == OperationState::Home || homeState != HomeState::Idle
		// 
		// Homing이 끝났을 때:
		// opState == OperationState::Idle && homeState == HomeState::Idle && homeDone == true
		// 
		// 그 외의 경우는 Error 처리.
		// 

		//
		// HomeState OpState HomeDone
		// [566810859] 0, 3, 0
		// [566810859] 0, 3, 0
		// [566810859] 12, 3, 0
		// ...
		// [566811328] 12, 3, 0
		// [566811328] 18, 3, 0
		// ...
		// [566811968] 18, 3, 0
		// [566811968] 23, 3, 0
		// [566812109] 23, 3, 0
		// [566812109] 0, 0, 1
		// ***** HOME DONE *****
		//

		if (Status.opState == OperationState::Home || Status.homeState != HomeState::Idle)
		{
			Error = ERR_MOTOR_CMN_IN_MOVE_STATE;
		}
		else if (Status.opState == OperationState::Idle && Status.homeState == HomeState::Idle)
		{
			if (Status.homeDone == true)
			{
				Error = ERR_SUCCESS;
				bResult = TRUE;
			}
			else
			{
				if (m_nOri_Flag == eMotorOrgFlag_ING)
				{
					if (Status.ampAlarm == true)
					{
						Error = ERR_MOTOR_CMN_AMP_FAULT;
					}
					else if (Status.servoOn == false)
					{
						Error = ERR_MOTOR_CMN_SERVO_OFF;
					}
					else if (Status.homeError != HomeError::NoError)
					{
						Error = ERR_MOTOR_CMN_ORIGIN_FAILURE;
					}
					else
					{
						// Alarm도 아니고 ServoOff도 아니고 HomeError도 아니다.
						// 이런 경우는 어떤 경우인지 확인이 필요함.
						Error = ERR_FAILURE;
					}

					m_nOri_Flag = eMotorOrgFlag_NG;
				}
				else
				{
					// Motor는 움직이지 않는데 homing done 상태가 아니라면 그건 원점이 잡히지 않은 상태이다.
					Error = ERR_MOTOR_CMN_NOT_ORG_CHECKED;
				}
			}
		}
		else
		{
			// Homing 중도 아니고 Idle도 아니다. 예를 들면 Motor Move 중일 때가 그렇다.
			if (Status.homeDone == true)
			{
				Error = ERR_SUCCESS;
				bResult = TRUE;
			}
			else
			{
				Error = ERR_MOTOR_CMN_NOT_ORG_CHECKED;
			}
		}

#else
		if (Status.homeState == HomeState::Idle)
		{
			if (Status.homeDone)
			{
				Error = ERR_SUCCESS;
				bResult = TRUE;
			}
			else if (Status.ampAlarm)
			{
				m_nOri_Flag = eMotorOrgFlag_NG;
				Error = ERR_MOTOR_CMN_AMP_FAULT;
			}
			else if (!Status.servoOn)
				Error = ERR_MOTOR_CMN_SERVO_OFF;
			else if (Status.homeError != HomeError::NoError)
			{
				Error = ERR_MOTOR_CMN_ORIGIN_FAILURE;
			}
			//else
			// [FIXME][imsi] Homing 중에 AMP fault도 아니며
			//               Servo off도 아니며
			//               Home error도 아닐 경우에는 이 값을 반환한다
			//               오류가 없는 경우에는 이 값이 반환되어서는 안 됨.
			//               수정 필요.
			//Error = ERR_MOTOR_CMN_NOT_ORG_CHECKED;
			else
			{
				Error = ERR_MOTOR_CMN_IN_MOVE_STATE;
			}
		}
		else
		{
			Error = ERR_MOTOR_CMN_IN_MOVE_STATE;
		}
#endif
	}

	if (pError != NULL)
	{
		*pError = Error;
	}
	
	return bResult;
}
/*=============================================================================
  AUTHOR   : 김선진
  NAME     : OriginWhile
  PARAMS   : None
  RETURN   : 0   = 해당 축 사용 안함 or Board open fail or 원점 체크 완료.
			 0>  = 원점 잡기 실패
  COMMENTS : 해당 축의 원점 잡기 ( motion done 확인 )
==============================================================================*/
ERR_CODE OneAxis_SoftServoEtherCAT::OriginWhile()
{
	ERR_CODE Error = Origin();

	if (Error != ERR_SUCCESS)
		return Error;

	CoreMotionAxisStatus cmAxis;

	do 
	{
		CoreMotionStatus CmStatus;

		int nResult = g_pWmx3Lib_cm->GetStatus(&CmStatus);
		ASSERT(nResult == ErrorCode::None);

		CoreMotionAxisStatus cmAxis = CmStatus.axesStatus[m_nAxisNumPhy];
		Sleep(1);
	} while (cmAxis.homeState != HomeState::Idle && 
		cmAxis.opState != OperationState::Idle && 
		!cmAxis.homeDone);

	if (cmAxis.homeError != HomeError::NoError || !cmAxis.homeDone)
		Error = ERR_MOTOR_CMN_ORIGIN_FAILURE;
	else
		Error = ERR_SUCCESS;

	return Error;
}
/*=============================================================================
AUTHOR   : 나명진  [2015-09-24]
NAME     :
PARAMS   :
RETURN   :
COMMENTS : 내부적으로 
==============================================================================*/
BOOL OneAxis_SoftServoEtherCAT::IsOrgCompletedInMotionController()
{

	CoreMotionStatus CmStatus;
	g_pWmx3Lib_cm->GetStatus(&CmStatus);
	CoreMotionAxisStatus cmAxis = CmStatus.axesStatus[m_nAxisNumPhy];

	if ( cmAxis.homeState == TRUE)
		return TRUE;

	return FALSE;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-24]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
BOOL OneAxis_SoftServoEtherCAT::IsOrgCompleted(ERR_CODE *pError)
{	
	ERR_CODE eErrCode = ERR_SUCCESS;
	BOOL bOrgChk= GetOriginCheck(&eErrCode);

	if (pError)
		*pError = eErrCode;

	if (bOrgChk == TRUE)
	{
		if (m_nOri_Flag != eMotorOrgFlag_OK)
		{
			bOrgChk = FALSE;

			if (m_nOri_Flag == eMotorOrgFlag_ING && m_nOri_Step == eOrg_Wait)
			{
				// Origin()을 호출한 후에 Home check가 완료되었으나 아직 m_nOri_Flag에는 반영되지 않은 상태
				eErrCode = ERR_MOTOR_CMN_ORG_CALL;
			}
			else
			{
				eErrCode = ERR_MOTOR_CMN_NOT_ORG_CHECKED;
			}

			if (pError != NULL)
			{
				*pError = eErrCode;
			}
		}
	}
	return bOrgChk;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : Origin
  PARAMS   : None
  RETURN   :  dCalcPos= 목표 위치에 대한 position 값 [um]
              1       = Board Init Fail or 해당 축 disable or Origin이 잡혀 있는 상태
             
  COMMENTS : 목표 위치에 대한 position 값 확인.
==============================================================================*/
ERR_CODE OneAxis_SoftServoEtherCAT::GetCommandPos(double& dPos)
{
	if (OneAxis_SoftServoEtherCAT::ChkCommunication() != TRUE)
	{
		return ERR_MOTOR_SYS_NOT_CONNECTED;
	}

	if (!g_pMotion_Cont_Imp->GetInitBoardStatus()) {
		return ERR_MOTOR_SYS_BOARD_INITIALIZTION_FAILURE;
	}
	if (m_bMotorDisable) {
		return ERR_MOTOR_CMN_DISABLED;
	}

	CoreMotionStatus CmStatus;
	g_pWmx3Lib_cm->GetStatus(&CmStatus);
	CoreMotionAxisStatus cmAxis = CmStatus.axesStatus[m_nAxisNumPhy];
	
	double dReturnPos = 0.0;
	if(g_pMotion_Cont_Imp->GetInitBoardStatus()){
		dReturnPos = cmAxis.posCmd;
	}

	dPos = (double)m_nDir_Move * dReturnPos * (m_dScaleFactor + m_dScaleFactorOffset);

	return ERR_SUCCESS;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : MotionDone
  PARAMS   : None
  RETURN   :   
				0		= 동작 중.
				1       = 해당 동작 완료 or 해당 축 사용 안함 or Board open fail
				-1      = AMP Fault 인 경우
  COMMENTS : 해당 축이 목표하는 위치에 도착 완료 확인.
==============================================================================*/
ERR_CODE OneAxis_SoftServoEtherCAT::MotionDone()
{
	if (OneAxis_SoftServoEtherCAT::ChkCommunication() != TRUE)
	{
		return ERR_MOTOR_SYS_NOT_CONNECTED;
	}

	if (!g_pMotion_Cont_Imp->GetInitBoardStatus()) {
		return ERR_MOTOR_SYS_BOARD_INITIALIZTION_FAILURE;
	}
	if (m_bMotorDisable) {
		return ERR_MOTOR_CMN_DISABLED;
	}

	CoreMotionStatus CmStatus;
	g_pWmx3Lib_cm->GetStatus(&CmStatus);
	CoreMotionAxisStatus cmAxis = CmStatus.axesStatus[m_nAxisNumPhy];
	
	if (cmAxis.ampAlarm == TRUE)// amp fault 발생
	{
		LogMtStrage(m_nAxisNumPhy);
		g_pWmx3Lib_cm->axisControl->SetServoOn(m_nAxisNumPhy, 0);	// servo off
		g_pWmx3Lib_cm->home->SetHomeDone(m_nAxisNumPhy, 0);

		return ERR_MOTOR_CMN_AMP_FAULT;
	}

	//
	// opState가 Idle이고 inPos가 true여야만 한다.
	// 단, inPos는 Servo Off시에는 off이므로 Servo Off일 때는 무시한다.
	//
//	if (cmAxis.motionComplete == TRUE)
	if (cmAxis.opState == OperationState::Idle && 
		((cmAxis.servoOn && cmAxis.inPos && cmAxis.delayedPosSet ) || !cmAxis.servoOn))
	{
		m_dActTime = m_tm.CompareTime();
		return ERR_SUCCESS;
	}

// 	CString strLog = _T("");
// 		strLog.Format("[Motion Done][Axis : %d][opState : %d] [ServoOn : %d] [Inposition : %d] [delayedPos Set : %d]", 
// 			m_nAxisNumPhy, (int)cmAxis.opState, (int)cmAxis.servoOn, (int)cmAxis.inPos, (int)cmAxis.delayedPosSet);
// 	
// 		m_MtLog.LOG(strLog);

	return ERR_MOTOR_CMN_IN_MOVE_STATE;
	//return -1로 변경 or Tast 해당 Alarm으로 변경
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : GetaxisdoneORfree
  PARAMS   : None
  RETURN   :  0        = 움직이는 상태
              1        = 동작 완료 or 해당 축 Ampfault
  COMMENTS : 해당 축의 동작이 완료 되었거나, AmpFault 상태 일 경우 (memory에서 Door 열기 위한 체크 인듯..)
==============================================================================*/
bool OneAxis_SoftServoEtherCAT::IsAxisDoneOrFree()
{
	if(GetServoStat())
	{
		if(MotionDone() == 1)
			return true;
		else
			return false;
	}

	return true;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : AxisDone
  PARAMS   : None
  RETURN   :  
				0       = 움직이는 상태
				1       = 해당 축 동작 완료 or 해당 축 사용 안함 or Board open fail
				-1      = 해당 축 Timeover or Amp fault
				other	= interlock error code
  COMMENTS : 해당 축의 동작이 완료 되었거나, AmpFault 상태 일 경우 (memory에서 Door 열기 위한 체크 인듯..)
==============================================================================*/
ERR_CODE OneAxis_SoftServoEtherCAT::AxisDone()
{
	if (OneAxis_SoftServoEtherCAT::ChkCommunication() != TRUE)
	{
		return ERR_MOTOR_SYS_NOT_CONNECTED;
	}

	if (!g_pMotion_Cont_Imp->GetInitBoardStatus()) {
		return ERR_MOTOR_SYS_BOARD_INITIALIZTION_FAILURE;
	}
	if (m_bMotorDisable) {
		return ERR_MOTOR_CMN_DISABLED;
	}

	CoreMotionStatus CmStatus;
	g_pWmx3Lib_cm->GetStatus(&CmStatus);
	CoreMotionAxisStatus cmAxis = CmStatus.axesStatus[m_nAxisNumPhy];
	
	if (cmAxis.ampAlarm == true)
	{
		SetStop();
		ClearStatus();
		g_pWmx3Lib_cm->home->SetHomeDone(m_nAxisNumPhy, 0);
		return ERR_MOTOR_CMN_AMP_FAULT;
	}

	// motion done check
	if (cmAxis.opState != OperationState::Idle || !cmAxis.inPos || !cmAxis.delayedPosSet)
		return ERR_MOTOR_CMN_IN_MOVE_STATE;

	ERR_CODE Error = ERR_SUCCESS;
	if (GetOriginCheck(&Error) != TRUE)	// 원점 체크 진행 여부 확인
	{
		return Error;
	}

	// inposition check
	FramesClear();

	double dEncoder_um = 0.0;
	GetCurrentPos(dEncoder_um);


	//double dLimitValueNega = GetSwLimitNega();
	//double dLimitValuePosi = GetSwLimitPosi();

	//double dLimitDifferenceNega = fabs(dEncoder_um - m_dTarget_pos_um);
	//double dLimitDifferencePosi = fabs(dEncoder_um - m_dTarget_pos_um);


	if (dEncoder_um < m_dTarget_pos_um)					// postive 방향으로 이동 하여  positive 센서 확인
	{
		if (IsPositiveSensorOn() == true)
		{
			return ERR_MOTOR_CMN_HARDWARE_POSITIVE_LIMIT_HIT;
		}
	}
	else												// negative 방향으로 이동 하여 negative 센서 확인 
	{
		if (IsNegativeSensorOn() == true)
		{
			return ERR_MOTOR_CMN_HARDWARE_NEGATIVE_LIMIT_HIT;
		}
	}

	double dPosErrAbs_um = fabs(dEncoder_um - m_dTarget_pos_um);
	//minimum control range
	double dMinimumControlRange = (m_dScaleFactor + m_dScaleFactorOffset) * MOTOR_INPOSITION_PULSE_COUNT;// ((m_dScaleFactor + m_dScaleFactorOffset) > MOTOR_INPOSITION_um) ? (m_dScaleFactor + m_dScaleFactorOffset) * 4 : /*MOTOR_INPOSITION_um*/20.0;
	//기존에 Inpos 범위를 10um이였는데, inpos 패스 범위를 최소 1 um pps로 설정함.
	if (dPosErrAbs_um >= dMinimumControlRange)
	{
		//SetCommandPos(dEncoder_um);
		Sleep(10);
		ClearStatus();
		Sleep(10);
		Error = Move(m_dTarget_pos_um, m_dCmdVelocity*0.5, 100, 100, _eMotorIntertlock::eMotorInterlock_Operation);
		Sleep(10);

		if (Error == ERR_SUCCESS)	// 해당 함수는 inposition 성공해야 되므로 success를 반환 하면 안된다.
		{
			return ERR_MOTOR_CMN_IN_MOVE_STATE;
		}

		return Error;
	}

	return ERR_SUCCESS;
}

ERR_CODE OneAxis_SoftServoEtherCAT::ChkPremovePos()
{
	double dEncoder_um = 0.0;
	GetCurrentPos(dEncoder_um);

	double dPosErrAbs_um = fabs(dEncoder_um - m_dTarget_pos_um);

	if (dPosErrAbs_um <= PREMOVE_POSITION_GAB)
	{
		return ERR_SUCCESS;
	}

	return ERR_FAILURE;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : MoveWhile
  PARAMS   : move_pos = 움직이고자 하는 위치 (미크론 단위로 scale_factor로 나누어야 실제 pulse값이 됨)
             speed    = 움직이려는 축의 속도      (PPS 단위)
             accel    = 움직이려는 축의 가속도
             dccel    = 움직이려는 축의 감속도
  RETURN   :  1       = Board Init Fail or 해당 축 disable or OK
             -1       = AMP FAULT 상태 or Move Time Over
  COMMENTS : 해당 축을 움직이려는 함수 ( Motion Done 확인 불필요 )
==============================================================================*/
ERR_CODE OneAxis_SoftServoEtherCAT::MoveWhile(double a_dPos, double a_dSpeed, int a_nAccel, int a_nDccel)
{
	if (OneAxis_SoftServoEtherCAT::ChkCommunication() != TRUE)
	{
		return ERR_MOTOR_SYS_NOT_CONNECTED;
	}

	if (!g_pMotion_Cont_Imp->GetInitBoardStatus()) {
		return ERR_MOTOR_SYS_BOARD_INITIALIZTION_FAILURE;
	}
	if (m_bMotorDisable) {
		return ERR_MOTOR_CMN_DISABLED;
	}

	double encoder    = 0.0;
	double err_pos    = 0.0;
	double st_encoder = 0.0, ed_encoder = 0.0;
	int cd_encoder    = 0;
	int encoder_retry = 0;
	int err_retry     = 0;

	CTimerTickCheck tt(30,TRUE);

	ERR_CODE Error = ERR_SUCCESS;

	GetCurrentPos(st_encoder);

	for(int nRetryCnt = 0; nRetryCnt < 10; nRetryCnt++)
	{
		ERR_CODE nMoveResult = Move(a_dPos, a_dSpeed, a_nAccel, a_nDccel);

		while(nMoveResult == ERR_SUCCESS)
		{
			Error = MotionDone();

			if(Error == ERR_MOTOR_CMN_AMP_FAULT)	// AMP FAULT
			{
				LogMtStrage(m_nAxisNumPhy, a_dPos, a_dSpeed);
				SetStop();
				ClearStatus();

				return Error;
			}

			if(Error == 1)	         // Motion Done
			{
				break;
			}

			if(tt.CheckOverTime())	// Time Over Check
			{
				LogMtStrage(m_nAxisNumPhy, a_dPos, a_dSpeed);
				SetStop();
				ClearStatus();

				return Error;
			}

			NEXT;
		}

		err_pos = (m_dScaleFactor + m_dScaleFactorOffset) * MOTOR_INPOSITION_PULSE_COUNT;//(MOTOR_INPOSITION_um); // by jhjo[150424] Inposition 누락된 부분 수정
		GetCurrentPos(encoder);

		if( (encoder > a_dPos - err_pos) && (encoder < a_dPos + err_pos))
		{
			return ERR_SUCCESS;
		}		

		ClearStatus();
	}	

	SetStop();
	ClearStatus();

	return ERR_FAILURE;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : R_MoveWhile
  PARAMS   : pos      = 움직이고자 하는 거리
             speed    = 움직이려는 축의 속도      (PPS 단위)
  RETURN   : nResult(-1) = Ampfault 및 이동 불가 사항
             nResult( 1) = 정상 동작 완료 or 해당 축 사용 안함 or Board open fail
  COMMENTS : 현재 위치에서 pos 입력치 만큼 이동 시키는 함수.
==============================================================================*/
ERR_CODE OneAxis_SoftServoEtherCAT::R_MoveWhile(double a_dPos, double a_dSpeed)
{
	if (OneAxis_SoftServoEtherCAT::ChkCommunication() != TRUE)
	{
		return ERR_MOTOR_SYS_NOT_CONNECTED;
	}

	if (!g_pMotion_Cont_Imp->GetInitBoardStatus()) {
		return ERR_MOTOR_SYS_BOARD_INITIALIZTION_FAILURE;
	}
	if (m_bMotorDisable) {
		return ERR_MOTOR_CMN_DISABLED;
	}

	double Goal = 0.0;
	GetCurrentPos(Goal);

	Goal += a_dPos;	// 거리 + 현재위치

	ERR_CODE Error = ERR_SUCCESS;
	Error = MoveWhile(Goal, a_dSpeed);

	return Error;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : V_Move
  PARAMS   : pos      = 움직이고자 하는 거리
             speed    = 움직이려는 축의 속도      (PPS 단위)
  RETURN   : -1       = Amp fault or Servo Off
              0       = OK
			  1       = 해당 축 사용 안함 or Board open fail
  COMMENTS : 현재 위치에서 pos 입력치 만큼 이동 시키는 함수.
==============================================================================*/
ERR_CODE OneAxis_SoftServoEtherCAT::V_Move(int nDir, double dSpeed_umps, int nAccDec_msec)
{
	CString strLog = _T("");

	if (OneAxis_SoftServoEtherCAT::ChkCommunication() != TRUE)
	{
		strLog.Format(_T("Axis = %s Motor System Not Connected"), m_szAxisName);
		return ERR_MOTOR_SYS_NOT_CONNECTED;
	}

	if (!g_pMotion_Cont_Imp->GetInitBoardStatus()) {
		strLog.Format(_T("Axis = %s Board Status Failure"), m_szAxisName);
		m_MtLog.LOG(strLog);
		return ERR_MOTOR_SYS_BOARD_INITIALIZTION_FAILURE;
	}

	if (m_bMotorDisable) {
		strLog.Format(_T("Axis = %s Motor Disable Error"), m_szAxisName);
		m_MtLog.LOG(strLog);
		return ERR_MOTOR_CMN_DISABLED;
	}

	if (!GetServoStat()) {
		strLog.Format(_T("Axis = %s Motor Servo Off Status"), m_szAxisName);
		m_MtLog.LOG(strLog);
		return ERR_MOTOR_CMN_SERVO_OFF;
	}

	CoreMotionStatus CmStatus;
	CoreMotionAxisStatus cmAxis = CmStatus.axesStatus[m_nAxisNumPhy];

	if (cmAxis.ampAlarm == TRUE)// Amp Fault Check
	{
		strLog.Format(_T("Axis = %s Motor Amp Fault Status"), m_szAxisName);
		m_MtLog.LOG(strLog);
		return ERR_MOTOR_CMN_AMP_FAULT;
	}

	if (!cmAxis.homeDone)// Origin Check
	{
		strLog.Format(_T("Axis = %s Motor Not Origin Check"), m_szAxisName);
		m_MtLog.LOG(strLog);
		return ERR_MOTOR_CMN_NOT_ORG_CHECKED;
	}

 	if (m_cbInterLock_Jog != NULL) {
 		int nInterLockErr = m_cbInterLock_Jog(dSpeed_umps, nAccDec_msec, nAccDec_msec);
 		if (nInterLockErr != ERR_NO_ERROR) {
 			V_MoveStop();
 			strLog.Format(_T("Axis = %s Interlock Error"), m_szAxisName);
 			m_MtLog.LOG(strLog);
 			return nInterLockErr;
 		}
 	}

	int nRet = JogMove(nDir, dSpeed_umps, nAccDec_msec);
	if (nRet != ERR_SUCCESS)
	{
		CParamList AlarmData;
		strLog.Format("%s Axis Jog Move command error", m_szAxisName); //name 가지고 오는지 확인 필요.
		m_MtLog.LOG(strLog);
		return ERR_MOTOR_CMN_DISABLED; //[작업 필요] 에러 변경 필요.
	}

	ClearStatus();
		
	return ERR_SUCCESS;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : V_MoveStop
  PARAMS   : None
  RETURN   : 0       = OK
			 1       = 해당 축 사용 안함 or Board open fail
  COMMENTS : v_move를 Stop
==============================================================================*/
ERR_CODE OneAxis_SoftServoEtherCAT::V_MoveStop()
{
	if (OneAxis_SoftServoEtherCAT::ChkCommunication() != TRUE)
	{
		return ERR_MOTOR_SYS_NOT_CONNECTED;
	}

	if (!g_pMotion_Cont_Imp->GetInitBoardStatus()) {
		return ERR_MOTOR_SYS_BOARD_INITIALIZTION_FAILURE;
	}
	if (m_bMotorDisable) {
		return ERR_MOTOR_CMN_DISABLED;
	}

	int nRet = (int)g_pWmx3Lib_cm->motion->Stop(m_nAxisNumPhy);

	// stop시, 감속값 넣을 수 있음 [4/27/2023 dohyeong.kim]
	//	int nRet = (int)g_pWmx3Lib_cm->motion->Stop(m_nAxisNumPhy, 100);

	SaveMotorMoveLog(0, 0, 2);

	if (nRet != wmx3Api::ErrorCode::None) {
		return ERR_MOTOR_CMN_BOARD_API_FAILURE;
	}

	return NormalizeError(nRet);
//	return ERR_SUCCESS;
}

ERR_CODE OneAxis_SoftServoEtherCAT::V_MoveCheckInterlock(double speed, int accel)
{
	if (m_cbInterLock_Jog != NULL) {
		int nInterLockErr = m_cbInterLock_Jog(speed, accel, accel);
		if (nInterLockErr != ERR_NO_ERROR) {
			V_MoveStop();
			return nInterLockErr;
		}
	}

	return 0;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : ServoOn
  PARAMS   : None
  RETURN   : 0       = OK or 이미 servo on 상태
			 1       = 해당 축 사용 안함 or Board open fail
  COMMENTS : Amp를 Enable 시키고 현재 위치를 다시 넣어준다.
==============================================================================*/
ERR_CODE OneAxis_SoftServoEtherCAT::ServoOn()
{
	CoreMotionStatus CmStatus;
	g_pWmx3Lib_cm->GetStatus(&CmStatus);
	CoreMotionAxisStatus cmAxis = CmStatus.axesStatus[m_nAxisNumPhy];

	if (OneAxis_SoftServoEtherCAT::ChkCommunication() != TRUE)
	{
		return ERR_MOTOR_SYS_NOT_CONNECTED;
	}
	if (!g_pMotion_Cont_Imp->GetInitBoardStatus()) {
		return ERR_MOTOR_SYS_BOARD_INITIALIZTION_FAILURE;
	}
	if (m_bMotorDisable) {
		return ERR_MOTOR_CMN_DISABLED;
	}

	if(GetServoStat() == TRUE){
		return ERR_SUCCESS;
	}

	if (m_eMotorSyncType == eMotorSyncType_Slave)
		return ERR_MOTOR_CMN_IN_SLAVE_MODE;
	if (cmAxis.ampAlarm)
	{
		//알람 발생되어 있으면 HomeDone False로 변경
		g_pWmx3Lib_cm->home->SetHomeDone(m_nAxisNumPhy, 0);
	}
	g_pWmx3Lib_cm->axisControl->SetServoOn(m_nAxisNumPhy, TRUE);
	// Servo가 꺼지면 자동으로 Home Flag Off되도록 수정
	Config::HomeParam  pParam;
	g_pWmx3Lib_cm->config->GetHomeParam(m_nAxisNumPhy, &pParam);
	pParam.clearHomeDoneOnServoOff = true;
	g_pWmx3Lib_cm->config->SetHomeParam(m_nAxisNumPhy, &pParam);
	//

	//m_MotorLogger.LOG("%s[%d] Amp Eanble True", m_szAxisName, m_nAxisNumPhy);

	//현재의 위치를 다시 넣는다.
	double dCurrPos = 0.0;
	GetCurrentPos(dCurrPos);



	SetCommandPos(dCurrPos);

	if (m_eMotorSyncType == eMotorSyncType_Master)
	{
		g_pWmx3Lib_cm->axisControl->SetServoOn(m_nSlaveNo, TRUE);
	}

	return ERR_SUCCESS;
}

void OneAxis_SoftServoEtherCAT::SetAxisInitInfo(char* szAxisName, 
								   int num, 
								   double scale, 
								   int Zr_direction, 
								   int encoder_dir, 
								   int move_dir, 
								   int a_nEncoderType, 
								   int coordinate_dir, 
								   int pulse_dir, 
								   int double_chk_pulse,
								   MOTOR_TYPE _motor_type,
								   MOTOR_SYNC_TYPE _sync_type,
	                               int nSlaveNo,
								   BOOL bDegreeCtrl)
{
	//호출 확인.
	sprintf_s(m_szAxisName, sizeof(m_szAxisName), szAxisName);
	//StringCchCopy( m_szAxisName, _countof( m_szAxisName ), szAxisName );
	m_nAxisNumPhy     = num; 
	m_dScaleFactor = scale; 
	m_nDir_Ori     = Zr_direction;
	m_nMMC_Encoder_dir   = encoder_dir;
	m_nDir_Move    = move_dir;
	m_nEncoderType = a_nEncoderType;
	m_nMMC_Coordinate_dir = coordinate_dir;
	m_nDir_Pluse = 0; //pulse_dir;
	m_nDouble_chk_um = double_chk_pulse;
	m_eMotorType = _motor_type;
	m_eMotorSyncType = _sync_type;
	m_nSlaveNo = nSlaveNo;
	m_bDegeeCtrl = bDegreeCtrl;
	
	//fset_encoder_direction( m_nAxisNumPhy, m_nMMC_Encoder_dir );
	//fset_coordinate_direction( m_nAxisNumPhy, m_nMMC_Coordinate_dir );
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : ServoOff
  PARAMS   : None
  RETURN   : 0       = OK 
			 1       = 해당 축 사용 안함 or Board open fail
  COMMENTS : Amp를 disable 시킨다.
==============================================================================*/
ERR_CODE OneAxis_SoftServoEtherCAT::ServoOff()
{
	if (OneAxis_SoftServoEtherCAT::ChkCommunication() != TRUE)
	{
		return ERR_MOTOR_SYS_NOT_CONNECTED;
	}

	if (!g_pMotion_Cont_Imp->GetInitBoardStatus()) {
		return ERR_MOTOR_SYS_BOARD_INITIALIZTION_FAILURE;
	}
	if (m_bMotorDisable) {
		return ERR_MOTOR_CMN_DISABLED;
	}

	if (m_eMotorSyncType == eMotorSyncType_Slave)
		return ERR_MOTOR_CMN_IN_SLAVE_MODE;
	// 사용자가 Servo Off를 임의로 했기 때문에 Servo Off시 HomeDone 해제 안 되도록 수정
	int nResult = ERR_SUCCESS;
	if (g_pWmx3Lib_cm != nullptr)
	{
		Config::HomeParam  pParam;
		g_pWmx3Lib_cm->config->GetHomeParam(m_nAxisNumPhy, &pParam);
		pParam.clearHomeDoneOnServoOff = false;
		g_pWmx3Lib_cm->config->SetHomeParam(m_nAxisNumPhy, &pParam);
		//
		nResult = (int)g_pWmx3Lib_cm->axisControl->SetServoOn(m_nAxisNumPhy, FALSE);

		if (m_eMotorSyncType == eMotorSyncType_Master)
			nResult = (int)g_pWmx3Lib_cm->axisControl->SetServoOn(m_nSlaveNo, FALSE);
	}
	//m_MotorLogger.LOG("%s[%d] Amp Enable False", m_szAxisName, m_nAxisNumPhy);

	return NormalizeError(nResult);
}


/*=============================================================================
  AUTHOR   : 김선진
  NAME     : Reset
  PARAMS   : None
  RETURN   : 0       = OK 
			 1       = 해당 축 사용 안함 or Board open fail
  COMMENTS : Mmc Board 의 한축을 리셋한다.
==============================================================================*/
ERR_CODE OneAxis_SoftServoEtherCAT::AlarmReset()
{
	if (OneAxis_SoftServoEtherCAT::ChkCommunication() != TRUE)
	{
		return ERR_MOTOR_SYS_NOT_CONNECTED;
	}

	CoreMotionStatus CmStatus;
	g_pWmx3Lib_cm->GetStatus(&CmStatus);
	CoreMotionAxisStatus cmAxis = CmStatus.axesStatus[m_nAxisNumPhy];
	if (cmAxis.ampAlarm)
	{
		//알람 발생되어 있으면 HomeDone False로 변경
		g_pWmx3Lib_cm->home->SetHomeDone(m_nAxisNumPhy, 0);
	}
	if (!g_pMotion_Cont_Imp->GetInitBoardStatus()) {
		return ERR_MOTOR_SYS_BOARD_INITIALIZTION_FAILURE;
	}
	if (m_bMotorDisable) {
		return ERR_MOTOR_CMN_DISABLED;
	}

	g_pWmx3Lib_cm->axisControl->ClearAmpAlarm(m_nAxisNumPhy);
	g_pWmx3Lib_cm->axisControl->ClearAxisAlarm(m_nAxisNumPhy);
	Sleep(10);

	return ERR_SUCCESS;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : Reset
  PARAMS   : None
  RETURN   : 0       = OK 
			 1       = 해당 축 사용 안함 or Board open fail
			-1       = 해당 축이 Origin이 잡혀 있지 않을 경우
  COMMENTS : 현재 amp가 on or off 인지를 리턴..
==============================================================================*/
BOOL OneAxis_SoftServoEtherCAT::GetServoStat()
{
	if (OneAxis_SoftServoEtherCAT::ChkCommunication() != TRUE)
	{
		return FALSE;
	}

	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable)
		return FALSE;
	
	CoreMotionStatus CmStatus;
	g_pWmx3Lib_cm->GetStatus(&CmStatus);
	CoreMotionAxisStatus cmAxis = CmStatus.axesStatus[m_nAxisNumPhy];

	if (!cmAxis.servoOn)
		return FALSE;

	//서보가 on된경우에는 AmpFault를 확인한다.
	if (cmAxis.ampAlarm == TRUE)// amp fault 발생
	{
		g_pWmx3Lib_cm->axisControl->SetServoOn(m_nAxisNumPhy, 0);	// servo off
		g_pWmx3Lib_cm->home->SetHomeDone(m_nAxisNumPhy, 0);
		//res = -1;
		//m_MotorLogger.LOG("%s[%d] Amp Enable False", m_szAxisName, m_nAxisNumPhy);
		return FALSE;
	}
	
	return TRUE;
}

//typedef  short (__stdcall* FN_GET_AMP )( short, short* );

bool OneAxis_SoftServoEtherCAT::IsServoOn()
{
	if (OneAxis_SoftServoEtherCAT::ChkCommunication() != TRUE)
	{
		return false;
	}

	if( !g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable )
		return false;
	
	CoreMotionStatus CmStatus;
	g_pWmx3Lib_cm->GetStatus(&CmStatus);
	CoreMotionAxisStatus cmAxis = CmStatus.axesStatus[m_nAxisNumPhy];

	bool bResult = cmAxis.servoOn;
	//서보가 on된경우에는 AmpFault를 확인한다.
	if (bResult)
	{//만일 AmpFault 이면 서보를 off 한다.

		if (cmAxis.ampAlarm)
		{
			g_pWmx3Lib_cm->axisControl->SetServoOn(m_nAxisNumPhy, 0);	// servo off
			g_pWmx3Lib_cm->home->SetHomeDone(m_nAxisNumPhy, 0);
			//m_MotorLogger.LOG("%s[%d] Amp Enable False", m_szAxisName, m_nAxisNumPhy);
			bResult = false;
		}
	}

	return bResult;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : GetHomeSensor
  PARAMS   : None
  RETURN   : TRUE   = 해당 축 사용 안함 or Board open fail or Home sensor가 감지 됨.    
			 FALSE  = Home sensor 감지 안됨.
  COMMENTS : Home Sensor 가 감지 되는지 확인.
==============================================================================*/
bool OneAxis_SoftServoEtherCAT::IsHomeSensorOn()
{
	if (OneAxis_SoftServoEtherCAT::ChkCommunication() != TRUE)
	{
		return false;
	}

	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return false;
	}

	CoreMotionStatus CmStatus;
	g_pWmx3Lib_cm->GetStatus(&CmStatus);
	CoreMotionAxisStatus cmAxis = CmStatus.axesStatus[m_nAxisNumPhy];

	return cmAxis.homeSwitch;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : GetPositiveSensor
  PARAMS   : None
  RETURN   : TRUE   = 해당 축 사용 안함 or Board open fail or +Limit가 감지 됨
			 FALSE  = +Limit가 감지 안됨.
  COMMENTS : + 방향의 Limit Sensor의 상태를 읽어온다.
==============================================================================*/
bool OneAxis_SoftServoEtherCAT::IsPositiveSensorOn()
{
	if (OneAxis_SoftServoEtherCAT::ChkCommunication() != TRUE)
	{
		return false;
	}
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return 1;
	}
	if (m_eMotorSyncType == eMotorSyncType_Slave) {
		return 0;
	}

	CoreMotionStatus CmStatus;
	g_pWmx3Lib_cm->GetStatus(&CmStatus);
	CoreMotionAxisStatus cmAxis = CmStatus.axesStatus[m_nAxisNumPhy];

//	if (m_nMMC_Coordinate_dir == 0) // 정방향
//		return cmAxis.positiveLS;
//	else if (m_nMMC_Coordinate_dir == 1) // 역방향
//		return cmAxis.negativeLS;
//
//	ASSERT(false);

	return cmAxis.positiveLS;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : GetNegativeSensor
  PARAMS   : None
  RETURN   : TRUE   = 해당 축 사용 안함 or Board open fail or -Limit가 감지 됨
			 FALSE  = -Limit가 감지 안됨.
  COMMENTS : - 방향의 Limit Sensor의 상태를 읽어온다.
==============================================================================*/
bool OneAxis_SoftServoEtherCAT::IsNegativeSensorOn()
{
	if (OneAxis_SoftServoEtherCAT::ChkCommunication() != TRUE)
	{
		return false;
	}
	if (!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable) {
		return 1;
	}
	if (m_eMotorSyncType == eMotorSyncType_Slave) {
		return 0;
	}

	CoreMotionStatus CmStatus;
	g_pWmx3Lib_cm->GetStatus(&CmStatus);
	CoreMotionAxisStatus cmAxis = CmStatus.axesStatus[m_nAxisNumPhy];

	//	 if (m_nMMC_Coordinate_dir == 0) // 정방향
	//	 	return cmAxis.negativeLS;
	//	 else if (m_nMMC_Coordinate_dir == 1) // 역방향
	//	 	return cmAxis.positiveLS;
	//
	//	ASSERT(false);

	return cmAxis.negativeLS;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : ClearStatus
  PARAMS   : None
  RETURN   : TRUE   = 해당 축 사용 안함 or Board open fail or 해당 축 EVENT 해제 완료.
			 FALSE  = 해당 축 EVENT 해제 실패
  COMMENTS : 해당 축의 발생된 EVENT를 해제 하고, 다음명령부터 실행 한다
==============================================================================*/
ERR_CODE OneAxis_SoftServoEtherCAT::ClearStatus()
{
	if (OneAxis_SoftServoEtherCAT::ChkCommunication() != TRUE)
	{
		return ERR_MOTOR_SYS_NOT_CONNECTED;
	}

	if (!g_pMotion_Cont_Imp->GetInitBoardStatus()) {
		return ERR_MOTOR_SYS_BOARD_INITIALIZTION_FAILURE;
	}
	if (m_bMotorDisable) {
		return ERR_MOTOR_CMN_DISABLED;
	}

	return ERR_SUCCESS;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : SetStop
  PARAMS   : None
  RETURN   :	( 0 ) 정상 작업					///< lwy 20150819
				( 1 ) init 실패 or 축 사용 안함. ///< lwy 20150819
  COMMENTS : 지정된 축의 STOP_EVENT를 생성, 축의 이동을 멈춘다.
==============================================================================*/
ERR_CODE OneAxis_SoftServoEtherCAT::SetStop()
{
	if (OneAxis_SoftServoEtherCAT::ChkCommunication() != TRUE)
	{
		return ERR_MOTOR_SYS_NOT_CONNECTED;
	}

	if (!g_pMotion_Cont_Imp->GetInitBoardStatus()) {
		return ERR_MOTOR_SYS_BOARD_INITIALIZTION_FAILURE;
	}
	if (m_bMotorDisable) {
		return ERR_MOTOR_CMN_DISABLED;
	}

	int nRet = (int)g_pWmx3Lib_cm->motion->Stop(m_nAxisNumPhy);

	return NormalizeError(nRet);
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : SetStop
  PARAMS   : None
  RETURN   :	( 0 ) 정상 작업
				( 1 ) init 실패 or 축 사용 안함.
  COMMENTS : 지정된 축의 STOP_EVENT를 생성, 축의 이동을 멈춘다.
==============================================================================*/
ERR_CODE	OneAxis_SoftServoEtherCAT::SetSlowStop()
{
	if (OneAxis_SoftServoEtherCAT::ChkCommunication() != TRUE)
	{
		return ERR_MOTOR_SYS_NOT_CONNECTED;
	}

	if (!g_pMotion_Cont_Imp->GetInitBoardStatus()) {
		return ERR_MOTOR_SYS_BOARD_INITIALIZTION_FAILURE;
	}
	if (m_bMotorDisable) {
		return ERR_MOTOR_CMN_DISABLED;
	}

	CoreMotionStatus CmStatus;
	g_pWmx3Lib_cm->GetStatus(&CmStatus);
	CoreMotionAxisStatus cmAxis = CmStatus.axesStatus[m_nAxisNumPhy];

	int nCmdDcc = 30;
	int nRet = (int)g_pWmx3Lib_cm->motion->ExecTimedStop(m_nAxisNumPhy, nCmdDcc);

	int nRet1 = 1;
	if (m_eMotorSyncType == eMotorSyncType_Master) {
		nRet1 = (int)g_pWmx3Lib_cm->motion->ExecTimedStop(m_nSlaveNo, nCmdDcc);
	}
	
	//AlarmReset();

	ERR_CODE Error = NormalizeError(nRet);
	if (Error != ERR_SUCCESS)
		Error = NormalizeError(nRet1);
	if (Error != 0)
	{
		m_MtLog.LOG(_T("%s[%d] Axis Set Slow Stop Err : %d nRet : %d nRet1 : %d nCmdDcc : %d"), m_szAxisName, m_nAxisNumPhy, Error, nRet, nRet1, nCmdDcc);
	}
	return Error;
}


/*=============================================================================
  AUTHOR   : 김선진
  NAME     : SetStop
  PARAMS   : stop_acc = 감속 시간 (stop_acc * 10msec)
  RETURN   : 
  COMMENTS : 지정된 축의 STOP_EVENT 수행시 감속시간을 지정
==============================================================================*/
void OneAxis_SoftServoEtherCAT::SetStopRate(double stop_dcc)
{
	//if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
	//	return;
	//}
	//
	//set_stop_rate(m_nAxisNumPhy, (int)stop_dcc); //(stop_acc * 10msec)
	////m_MotorLogger.LOG("%s[%d] Axis Set Stop Rate", m_szAxisName, m_nAxisNumPhy);	
	//
	//if (m_eMotorSyncType == eMotorSyncType_Master) {
	//	set_stop_rate(m_nSlaveNo, (int)stop_dcc); //(stop_acc * 10msec)
	//}
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : SetPos
  PARAMS   : posit = 목표 위치
  RETURN   : 
  COMMENTS : 축의 목표 위치를 지정
==============================================================================*/
void OneAxis_SoftServoEtherCAT::SetCommandPos(double a_dPos)
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){	
		return;
	}

	if(a_dPos != 0.0)
	{
//		if( m_nEncoderType == eEncoderType_4Ch_NoPulseDir ){
		a_dPos = (double)m_nDir_Move * a_dPos / (m_dScaleFactor + m_dScaleFactorOffset);
// 		}else{
// 			a_dPos = (double)m_nDir_Move * (double)m_nDir_Pulse * a_dPos / (m_dScaleFactor + m_dScaleFactorOffset);
// 		}
	}

	CoreMotionStatus CmStatus;
	g_pWmx3Lib_cm->GetStatus(&CmStatus);
	CoreMotionAxisStatus cmAxis = CmStatus.axesStatus[m_nAxisNumPhy];

	int nErrCode = (int)g_pWmx3Lib_cm->home->SetFeedbackPos(m_nAxisNumPhy, a_dPos);
	//m_MotorLogger.LOG("%s[%d] Set Command [%0.2f Pulse]", m_szAxisName, m_nAxisNumPhy, a_dPos);

	//return nErrCode;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : FramesClear
  PARAMS   : 
  RETURN   : 
  COMMENTS :  Handler 에서 MMC보드의 Frame Buffer를 Clear
==============================================================================*/
void OneAxis_SoftServoEtherCAT::FramesClear()
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return;
	}

	//frames_clear(m_nAxisNumPhy);

	//if (m_eMotorSyncType == eMotorSyncType_Master) {
	//	frames_clear(m_nSlaveNo);
	//}
	//m_MotorLogger.LOG("%s[%d] Axis Frame Clear", m_szAxisName, m_nAxisNumPhy);
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : SetHome
  PARAMS   : active
			  -> NO_EVENT    (0) = ignore a condition
              -> STOP_EVENT  (1) = generate a stop event
              -> E_STOP_EVENT(2) = generate an e_stop event
              -> ABORT_EVENT (3) = disable PID control, and disable the amplifier
  RETURN   : 
  COMMENTS :  Home Sensor를 만났을때 동작할 EVENT를 지정
==============================================================================*/
void OneAxis_SoftServoEtherCAT::SetHome(int active)
{
	//if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
	//	return;
	//}
	//
	//set_home(m_nAxisNumPhy, active);
	//
	//if (m_eMotorSyncType == eMotorSyncType_Master) {
	//	set_home(m_nSlaveNo, active);
	//}
	//m_MotorLogger.LOG("%s[%d] Axis Set Home [Active = %d]", m_szAxisName, m_nAxisNumPhy, active);
}

void OneAxis_SoftServoEtherCAT::SetPositiveLimit(int nAction)
{
	//if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
	//	return;
	//}
	//set_positive_limit(m_nAxisNumPhy, nAction);
}

void OneAxis_SoftServoEtherCAT::SetNegativeLimit(int nAction)
{
	//if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
	//	return;
	//}
	//set_negative_limit(m_nAxisNumPhy, nAction);
}


/*=============================================================================
  AUTHOR   : 김선진
  NAME     : GetEncoder
  PARAMS   : None
  RETURN   : 1         = 해당 축 사용 안함 or Board open fail
             dCalcPos  = 위치의 Encoder 값	[um]
  COMMENTS :  축의 실제위치(현재위치)를 읽어 들인다
==============================================================================*/
int OneAxis_SoftServoEtherCAT::GetCurrentPos(double &dPos)
{
	double dCurPos = 0.0;
// 	if (!g_pMotion_Cont_Imp->GetInitBoardStatus()) {
// 		return ERR_MOTOR_SYS_BOARD_INITIALIZTION_FAILURE;
// 	}	
// 
// 	if (m_bMotorDisable) {
// 		return ERR_MOTOR_CMN_DISABLED;
// 	}

	CoreMotionStatus CmStatus;
	g_pWmx3Lib_cm->GetStatus(&CmStatus);
	CoreMotionAxisStatus cmAxis = CmStatus.axesStatus[m_nAxisNumPhy];

	double dActualPos = cmAxis.actualPos;
	dPos = dActualPos * (m_dScaleFactor + m_dScaleFactorOffset);

	return dCurPos;

// 	if (m_bPlviModuleServo)
// 	{
// 		// subindex 07h, data type DINT(4byte)
// 		unsigned char PrevValue[4];
// 		unsigned int ActualSize = 0;
// 		unsigned int ErrorCode;
// 
// 		long Result = g_pecApi->SdoUpload(m_nAxisNumPhy, 0x2010, 0x07
// 			, sizeof(PrevValue), PrevValue
// 			, &ActualSize, &ErrorCode, 1000);
// 		if (Result != 0)
// 		{
// 			m_MtLog.LOG(_T("%s[PhyNo %d][Cur Err result=%d][Err code=%d]"), m_szAxisName, m_nAxisNumPhy, Result, ErrorCode);
// 			return ERR_FAILURE;
// 		}
// 
// 		CString str;
// 		str.Format("%02x%02x%02x%02x", PrevValue[3], PrevValue[2], PrevValue[1], PrevValue[0]);
// 		str.MakeUpper();
// 		unsigned int nGetData = ConvertModuleEncoder((LPSTR)(LPCTSTR)str);
// 		dCalPos = nGetData;
// 	}
// 	else
// 	{
// 		if (m_bMotorDisable) {
// 			return ERR_MOTOR_CMN_DISABLED;
// 		}
// 
// 		CoreMotionStatus CmStatus;
// 		g_pWmx3Lib_cm->GetStatus(&CmStatus);
// 		CoreMotionAxisStatus cmAxis = CmStatus.axesStatus[m_nAxisNumPhy];
// 
// 		double dActualPos = cmAxis.actualPos;
// 		dCalPos = dActualPos * (m_dScaleFactor + m_dScaleFactorOffset);
// 	}

}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : SetEncoder
  PARAMS   : pos : 위치
  RETURN   : 
  COMMENTS : 축의 실제위치 및 목표위치를 지정

  ** set_position(ax, 0) 할 경우 command와 position이 모두 '0'으로 되나, 
     '0'이 아닌 값으로 하게 될 경우 command는 지정한 값이 되지만, 
	 position은 지정값의 4체배된 값이 표시된다. 
	 서보가 입력대비 출력되는 엔코더 값이 4체배의 연산관계를 가지지 않는 경우는 문제의 소지가 있으므로 "주의" 해야 합니다.
==============================================================================*/
void OneAxis_SoftServoEtherCAT::SetCurrentPos(double pos)
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return;
	}

	CoreMotionStatus CmStatus;
	g_pWmx3Lib_cm->GetStatus(&CmStatus);
	CoreMotionAxisStatus cmAxis = CmStatus.axesStatus[m_nAxisNumPhy];

	if(pos != 0.0)
	{
		pos = (double)m_nDir_Move * (double)pos / (m_dScaleFactor + m_dScaleFactorOffset);
	}

	g_pWmx3Lib_cm->home->SetFeedbackPos(m_nAxisNumPhy, pos);
	//m_MotorLogger.LOG("%s[%d] Axis Set Encoder [%0.2f Pulse]", m_szAxisName, m_nAxisNumPhy, pos);
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : SetEncoder
  PARAMS   : pos : 위치
  RETURN   : 1 : AMP Fault 
             0 : OK or 해당 축 사용 안함 or Board open fail
  COMMENTS : AMP Fault 상태 확인
==============================================================================*/
bool OneAxis_SoftServoEtherCAT::IsAmpFault()
{
	if (OneAxis_SoftServoEtherCAT::ChkCommunication() != TRUE)
	{
		return false;
	}

	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return false;
	}

	CoreMotionStatus CmStatus;
	g_pWmx3Lib_cm->GetStatus(&CmStatus);
	CoreMotionAxisStatus cmAxis = CmStatus.axesStatus[m_nAxisNumPhy];

	int r_val = cmAxis.ampAlarm;

	if(r_val == TRUE )
	{
		g_pWmx3Lib_cm->home->SetHomeDone(m_nAxisNumPhy, 0);
		LogMtStrage(m_nAxisNumPhy);

		return true;
	}

	int r_val1 = -1;
	if (m_eMotorSyncType == eMotorSyncType_Master) {

		cmAxis = CmStatus.axesStatus[m_nSlaveNo];
		r_val1 = cmAxis.ampAlarm;

		if (r_val1 == TRUE)
		{
			g_pWmx3Lib_cm->home->SetHomeDone(m_nSlaveNo, 0);
			LogMtStrage(m_nSlaveNo);
			return true;
		}
	}

	return false;
}

/*=============================================================================
  AUTHOR   : 이원용
  NAME     : GetErrorValue
  PARAMS   : None
  RETURN   : double
  COMMENTS : 현재 위치와 목표위치의 차이를 리턴.
==============================================================================*/
double		OneAxis_SoftServoEtherCAT::GetErrorValue()
{
	//if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
	//	return 1;
	//}
	//
	//double dErrorValue;
	//
	//get_error( m_nAxisNumPhy, &dErrorValue );

	//return dErrorValue;

	return 0.0;
}

// OneAxis virtual

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : GetVelocity
  PARAMS   : 
  RETURN   : vel = 해당 축의 motor parameter 설정 치 (pps)
  COMMENTS : Motor Parameter에 설정 된 speed
==============================================================================*/
double OneAxis_SoftServoEtherCAT::GetVelocity()
{
	CoreMotionStatus CmStatus;
	g_pWmx3Lib_cm->GetStatus(&CmStatus);
	CoreMotionAxisStatus cmAxis = CmStatus.axesStatus[m_nAxisNumPhy];
	
	return cmAxis.velocityCmd;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : GetAcc
  PARAMS   : 
  RETURN   : acc = 해당 축의 motor parameter 설정 치 (pps)
  COMMENTS : Motor Parameter에 설정 된 Accel
==============================================================================*/
int OneAxis_SoftServoEtherCAT::GetAcc()
{
	return m_nCmdAcc;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : GetDcc
  PARAMS   : 
  RETURN   : dcc = 해당 축의 motor parameter 설정 치 (pps)
  COMMENTS : Motor Parameter에 설정 된 Decel
==============================================================================*/
int OneAxis_SoftServoEtherCAT::GetDcc()
{
	return m_nCmdDcc;
}

// virtual OneAxis
void OneAxis_SoftServoEtherCAT::SetPulseDirection(int zdir, int pdir, int mdir, BOOL bEnType)
{
	// Softservo의 경우 m_nDir_Pulse를 사용하지 않는다.
	m_nDir_Ori		= zdir;
	//m_nDir_Pulse	= pdir;
	m_nDir_Move		= mdir;
	m_nDir_Pluse = 0;//pdir;

	//this->Set_Reset_Level(bEnType);
	//m_MotorLogger.LOG("%s[%d] Axis Reset Level [Encode Type = %d]", m_szAxisName, m_nAxisNumPhy, bEnType);
	/*
	m_EncoderType=1;
	*/
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : GetDcc
  PARAMS   : bType : 1 = HIGH, 0 = LOW
  RETURN   : dcc = 해당 축의 motor parameter 설정 치 (pps)
  COMMENTS : Motor Parameter에 설정 된 Decel
==============================================================================*/
void OneAxis_SoftServoEtherCAT::Set_Reset_Level(BOOL bType)
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return;
	}
	
	m_bResetLevel=bType;

	//set_amp_reset_level(m_nAxisNumPhy, bType);
	//m_MotorLogger.LOG("%s[%d] Axis Amp Reset Level [Level = %d]", m_szAxisName, m_nAxisNumPhy, bType);
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : RepeatMove
  PARAMS   : move_pos = 움직일 거리
             rspeed   = 속도
			 racc     = 가속
			 rdcc     = 감속
  RETURN   : 1 : 해당 축 사용 안함 or Board open fail
            -1 : 원점 안잡힌 상태 or 움직일 수 없는 상황 or AMP fault or Limit 위치를 벗어난 상황
  COMMENTS : 설정 거리 만큼의 반복 동작을 위한 함수
==============================================================================*/
ERR_CODE OneAxis_SoftServoEtherCAT::RepeatMove(double move_pos, double rspeed,int racc,int rdcc)
{
	#pragma message(__PRAGMA_MSG("Strange Return Value"))
	//return 1;
	return ERR_FAILURE;

	if (OneAxis_SoftServoEtherCAT::ChkCommunication() != TRUE)
	{
		return ERR_MOTOR_SYS_NOT_CONNECTED;
	}

	if (!g_pMotion_Cont_Imp->GetInitBoardStatus()) {
		return ERR_MOTOR_SYS_BOARD_INITIALIZTION_FAILURE;
	}
	if (m_bMotorDisable) {
		return ERR_MOTOR_CMN_DISABLED;
	}

	CoreMotionStatus CmStatus;
	g_pWmx3Lib_cm->GetStatus(&CmStatus);
	CoreMotionAxisStatus cmAxis = CmStatus.axesStatus[m_nAxisNumPhy];

	if(!IsOrgCompleted(nullptr)){
		return ERR_MOTOR_CMN_NOT_ORG_CHECKED;
	}

	if(!GetServoStat())
	{
		return ERR_MOTOR_CMN_SERVO_OFF;
	}

	if (!g_pMotion_Cont_Imp->GetInitBoardStatus()) {
		return ERR_MOTOR_SYS_BOARD_INITIALIZTION_FAILURE;
	}
	if (m_bMotorDisable) {
		return ERR_MOTOR_CMN_DISABLED;
	}

	ClearStatus();

	bool bIsAmpFault = IsAmpFault();
	if(bIsAmpFault){
		return ERR_MOTOR_CMN_AMP_FAULT;
	}

	// 움직이고자 하는 위치가 s/w limit를 벗어나는 위치인지를 확인
	if (move_pos > m_dSwLimit_Posi_um)
		return ERR_MOTOR_CMN_SOFTWARE_POSITIVE_LIMIT_HIT;
	if(move_pos < m_dSwLimit_Nega_um)
		return ERR_MOTOR_CMN_SOFTWARE_NEGATIVE_LIMIT_HIT;

	if(rspeed<=0)
		rspeed = m_dCmdVelocity;

	if (m_bDegeeCtrl == TRUE)	// (각)도 단위로
	{
		if (rspeed > 90)	// 회전축은 이동속도가 초당 90도 보다 빠르면 안되어서 제한함.
		{
			rspeed = 90;
		}
	}

// 20190405 ihc. 조건수정
	if (racc <= 0 || racc > 500)
		racc = m_nCmdAcc;
// 20190405 ihc. 조건수정
	if (rdcc <= 0 || rdcc > 500)
		rdcc = m_nCmdDcc;

	m_tm.StartTimer();	// Repeat Motion Done check time

	ASSERT((m_dScaleFactor + m_dScaleFactorOffset) != 0.0);

	double dTargetPos	= (double)m_nDir_Move * move_pos / (m_dScaleFactor + m_dScaleFactorOffset);
	double dMovePos		= move_pos / 1000.0;
	double dSpeed_pps	= rspeed / (m_dScaleFactor + m_dScaleFactorOffset);

	SaveMotorMoveLog(dTargetPos, dMovePos, 0);

	Motion::PosCommand posCommand = Motion::PosCommand();

	posCommand.target					= dTargetPos;	// pulse
	posCommand.axis						= m_nAxisNumPhy;
	posCommand.profile.velocity			= dSpeed_pps;
	posCommand.profile.startingVelocity = 0;
	posCommand.profile.endVelocity		= 0;
	posCommand.profile.acc				= dSpeed_pps * 1000 / racc;
	posCommand.profile.dec				= dSpeed_pps * 1000 / rdcc;
	posCommand.profile.jerkAcc			= 1000;
	posCommand.profile.jerkAccRatio		= 0.5;
	posCommand.profile.jerkDec			= 1000;
	posCommand.profile.jerkDecRatio		= 0.5;

	//start_ts_move(m_nAxisNumPhy, dTargetPos, rspeed, racc, rdcc);
	int nErrorCode = (int)g_pWmx3Lib_cm->motion->StartPos(&posCommand);

	return NormalizeError(nErrorCode);
}


ERR_CODE OneAxis_SoftServoEtherCAT::JogMove( int nDir, double dSpeed_umps, int nAccDec, BOOL bPcdBacklashMode/*=FALSE*/ )
{
// 	//if(!g_pMotion_Cont_Imp->GetInitBoardStatus() ){
// 	//	return ERR_MOTOR_SYS_BOARD_INITIALIZTION_FAILURE;
// 	//}	
// 	//if( m_bMotorDisable ){
// 	//	return ERR_MOTOR_CMN_DISABLED;
// 	//}
// 	//if(!GetServoStat()){
// 	//	return ERR_MOTOR_CMN_SERVO_OFF;
// 	//}
// 
	if (OneAxis_SoftServoEtherCAT::ChkCommunication() != TRUE)
	{
		return ERR_MOTOR_SYS_NOT_CONNECTED;
	}
	
	CoreMotionStatus CmStatus;
	g_pWmx3Lib_cm->GetStatus(&CmStatus);
	CoreMotionAxisStatus cmAxis = CmStatus.axesStatus[m_nAxisNumPhy];

	double dMovePos_um = 0.0;

	if( nDir > 0 ) dMovePos_um = m_dSwLimit_Posi_um;
	else dMovePos_um		   = m_dSwLimit_Nega_um;

	ClearStatus();

	if( dSpeed_umps <= 0 )
		dSpeed_umps = 10;

	if( nAccDec < 500 )
		nAccDec = 500;

// 	if (m_bDegeeCtrl == TRUE) {
// 		dMovePos_um = dMovePos_um / 1000;
// 	}

	if (m_bDegeeCtrl == TRUE)	// (각)도 단위로
	{
		if (dSpeed_umps > 90)	// 회전축은 이동속도가 초당 90도 보다 빠르면 안되어서 제한함.
		{
			dSpeed_umps = 90;
		}
	}


	// 움직이고자 하는 위치가 s/w limit를 벗어나는 위치인지를 확인
	if (dMovePos_um > m_dSwLimit_Posi_um) {
		return ERR_MOTOR_CMN_SOFTWARE_POSITIVE_LIMIT_HIT;
	}
	if (dMovePos_um < m_dSwLimit_Nega_um) {
		return ERR_MOTOR_CMN_SOFTWARE_NEGATIVE_LIMIT_HIT;
	}

	double dTargetPos_pulse = (dMovePos_um / (m_dScaleFactor + m_dScaleFactorOffset)); //pulse

	double dSpeed_pps = dSpeed_umps / (m_dScaleFactor + m_dScaleFactorOffset);
	//Rotate 속도 계산.
	if (m_bDegeeCtrl == TRUE) {
		double dBufvelocityPps = dSpeed_pps;    // pulse/sec
		dSpeed_pps = m_dScaleFactor * dBufvelocityPps; //   1degree/ 1sec로 환산.
	}



		//20190403 ihc. 위치변경(본 함수 내 아랫부분에 있음). speed, accel, dcel 값 계산 이후 
	if (m_cbInterLock_Jog != NULL) {
		ERR_CODE nInterLockErr = m_cbInterLock_Jog(nDir, dSpeed_pps, bPcdBacklashMode);
				
		if (nInterLockErr != ERR_SUCCESS) {
			return nInterLockErr;
		}
	}

	SaveMotorMoveLog( dTargetPos_pulse, dMovePos_um, 2);

// 	if (m_bDegeeCtrl == TRUE)
// 	{
// 		dTargetPos_pulse  *= m_nDir_Move;
// 	}





	Motion::PosCommand posCommand = Motion::PosCommand();

	posCommand.target					= dTargetPos_pulse;	// pulse로 변화 해야 하는지 확인
	posCommand.axis						= m_nAxisNumPhy;
	posCommand.profile.velocity			= dSpeed_pps;
	posCommand.profile.startingVelocity = 0;
	posCommand.profile.endVelocity		= 0;
	posCommand.profile.acc				= dSpeed_pps * 1000 / nAccDec;
	posCommand.profile.dec				= dSpeed_pps * 1000 / nAccDec;
	posCommand.profile.jerkAcc			= 1000;
	posCommand.profile.jerkAccRatio		= 0.5;
	posCommand.profile.jerkDec			= 1000;
	posCommand.profile.jerkDecRatio		= 0.5;

	// [NOTE] 검증 필요
	int nErrorCode = (int)g_pWmx3Lib_cm->motion->StartPos(&posCommand);

	return NormalizeError(nErrorCode);
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : _source
  PARAMS   : 
  RETURN   : 1 : 해당 축 사용 안함 or Board open fail
            ret: 1이면 정상
  COMMENTS : MMC보드로 부터 축의 현재상태를 읽어들인다, amp on/off 인지
==============================================================================*/
int OneAxis_SoftServoEtherCAT::_source()
{
	//if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
	//	return 1;
	//}
	//
	//INT ret = axis_source(m_nAxisNumPhy);
	return 1;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : sethomelevel
  PARAMS   : 
  RETURN   : 1 : OK
  COMMENTS : Home Sensor의 Active Level 상태를 HIGH(1) 또는 LOW(0)로 지정한다
==============================================================================*/
int OneAxis_SoftServoEtherCAT::SetHomeLevel(BOOL bStatus)
{
	//set_home_level(m_nAxisNumPhy, bStatus);
	//m_MotorLogger.LOG("%s[%d] Axis Home Level [Level = %d]", m_szAxisName, m_nAxisNumPhy, bStatus);
	return ERR_SUCCESS;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : CheckPowerOffStat
  PARAMS   : 
  RETURN   : FALSE : OK
             TRUE  : Amp Fault
  COMMENTS : servo 상태를 확인 and Servo의 전원이 꺼진것을 체크하여 org 하는 용도
==============================================================================*/
BOOL OneAxis_SoftServoEtherCAT::CheckPowerOffStat()
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return FALSE;
	}

	if (IsAmpFault() == TRUE)
	{
		return TRUE;
	}

	return FALSE;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : GetSpeed
  PARAMS   : 
  RETURN   : 1 : mmc board init fail or 해당 축 disable
             speed : 저장 된 속도 값( um / sec )
  COMMENTS : parameter에 설정 된 속도값을 불러온다. ( um / sec )
==============================================================================*/
double OneAxis_SoftServoEtherCAT::GetSpeed()
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return 1;
	}

	double dSpeed = m_dCmdVelocity;

	if( dSpeed < 10.0 )
		dSpeed = 10.0;

	return dSpeed;
}

double OneAxis_SoftServoEtherCAT::GetManualMoveSpeed_High()
{
	double dHighSpdPulse = GetSpeed() * 0.5;
	return dHighSpdPulse;
}

double OneAxis_SoftServoEtherCAT::GetManualMoveSpeed_Middle()
{
	double dMiddleSpdPulse = GetSpeed() * 0.25;
	return dMiddleSpdPulse;
}

double OneAxis_SoftServoEtherCAT::GetManualMoveSpeed_Low()
{
	double dLowSpdPulse = GetSpeed() * 0.05;

	dLowSpdPulse = 1000.0 / (m_dScaleFactor + m_dScaleFactorOffset);		// 초당 1mm 이동
	return dLowSpdPulse;
}

/************************************************************************/
/* 프로그램 시작시에 한번만 호출 한다.                                  */
/************************************************************************/

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : Set_disable
  PARAMS   : bDisable = Motor disable 상태 인가에 대한 parameter
  RETURN   : 
  COMMENTS : 해당 축의 사용 유/무 setting
==============================================================================*/
void OneAxis_SoftServoEtherCAT::Set_disable(BOOL bDisable)
{
	m_bMotorDisable = bDisable;
}


/*=============================================================================
  AUTHOR   : 김선진
  NAME     : IsDisable
  PARAMS   : 
  RETURN   : bDisable = Motor disable 상태 인가에 대한 parameter
  COMMENTS : 해당 축의 사용 유/무 setting
==============================================================================*/
bool OneAxis_SoftServoEtherCAT::IsDisable()
{
	return !!m_bMotorDisable;
}



/*=============================================================================
  AUTHOR   : 김선진
  NAME     : MotorStrageCheck
  PARAMS   : ST_MOTORSTATUS = 축에 대한 정보
  RETURN   : None
  COMMENTS : 서보모터 로그 and 충돌한 축에 정보 먼저(Target, Speed, 감속, 가속)  기록 나머지 축 정보도 기록
==============================================================================*/
void OneAxis_SoftServoEtherCAT::MotorStrageCheck(ST_MOTORSTATUS* lpstMotorData) // 
{
	/*
	CString strData;

	strData.Format("------------motor Jam Log start %s--------------", _MotorNameStr[lpstMotorData->iAxisNum]);
	g_CDailyProduct.SaveMotorErrLogFileString(strData);

	// by hyb [141011]: 서보모터 로그 표시 수정 <start>
	strData.Format("[T Pos %.2fmm] [E Pos %.2fmm] [cmd Pos %.2fmm] [Spd %.2f pps] [Acc %d] [Dcc %d] [Amp Fault %d]",
		lpstMotorData->dTarget/1000,lpstMotorData->dEncoder/1000,lpstMotorData->dCommand/1000,lpstMotorData->dMotorSpeed, lpstMotorData->dAcc, lpstMotorData->dDcc, lpstMotorData->bServoStatus);
	g_CDailyProduct.SaveMotorErrLogFileString(strData);

	// By jhjang [150227] Add Motor Amp Fault Log <start>
	strData.Format("Motor Status [in_Sequence : %d] [in_motion : %d] [in_position : %d] [frames_left : %d] [motion_done : %d] [axis_done : %d] [Command RPM : %d]"
		,lpstMotorData->iInsequence, lpstMotorData->iInmotion, lpstMotorData->iInposition, lpstMotorData->iFramesleft, lpstMotorData->iMotiondone, lpstMotorData->iAxisdone, lpstMotorData->sEncoderRPM);
	// By jhjang [150227] Add Motor Amp Fault Log <end>

	g_CDailyProduct.SaveMotorErrLogFileString(strData);

	// by hyb [141011]: 서보모터 로그 표시 수정 <end>

	strData.Format("------------motor Jam Log end %s--------------", _MotorNameStr[lpstMotorData->iAxisNum]);
	g_CDailyProduct.SaveMotorErrLogFileString(strData);
	*/
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : AmpFaultCheckFunction
  PARAMS   : None
  RETURN   : 0 : mmc board init fail or 해당 축 disable or Amp Fault
             1 : OK
  COMMENTS : Origin flag clear
==============================================================================*/
BOOL OneAxis_SoftServoEtherCAT::AmpFaultCheckFunction()
{
	if (OneAxis_SoftServoEtherCAT::ChkCommunication() != TRUE)
	{
		return FALSE;
	}

	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable)
	{
		return FALSE;
	}

	ClearStatus();

	int r_val = IsAmpFault();

	if(r_val == TRUE)	// amp fault
	{
		LogMtStrage(m_nAxisNumPhy);

		return FALSE;
	}

	return TRUE;
}


/*=============================================================================
  AUTHOR   : 김선진
  NAME     : AmpFaultCheckFunction
  PARAMS   : dTargetPos : target posisition
             dMovePos   : Move   positioin
  RETURN   : None
  COMMENTS : Motor 이동 시 Log를 기록한다.
==============================================================================*/
void OneAxis_SoftServoEtherCAT::SaveMotorMoveLog(double dTargetPos_pulse, double dMovePos_mm, int nLogMode)
{
	 //Motor Log 남기지 않도록 주석처리 20190110 김현진
	// Move
	double dCurrentPos = 0.0;
	GetCurrentPos(dCurrentPos);
	if(nLogMode == 0){
		m_MtLog.LOG(_T("%s[PhyNo %d] [MOVE]: [Current Pos: %0.3f] [Target Pos: %0.3f] [Move Pos: %0.3fmm]"),m_szAxisName, m_nAxisNumPhy, dCurrentPos,dTargetPos_pulse, dMovePos_mm );
	}
	// R Move
	else if(nLogMode == 1){
		m_MtLog.LOG(_T("%s[PhyNo %d] [R_MOVE]: [Target Pos: %0.3f] [Move Pos: %0.3fmm]"),m_szAxisName, m_nAxisNumPhy, dTargetPos_pulse, dMovePos_mm );
	}
	// V Move
	else if(nLogMode == 2){
		m_MtLog.LOG(_T("%s[PhyNo %d] [V_MOVE]: [Speed ( pps ): %0.3f]"),m_szAxisName, m_nAxisNumPhy, dTargetPos_pulse );
	}	
}

/*==============================================================================
DESCRIPT : Load 및 Parameter 저장하는 부분에서 호출
KEY WORD : // By jhjang [150225] Save Motor Move Log
ARGUMENT : dPosLimit : [um]
           dNegLimit : [um]
		   bDisable  : 0 = SW Limit이 유효함.
                       1 = Orgin 진행 시에 sw limit을 무시 한다.
RETURN   : None
==============================================================================*/
bool OneAxis_SoftServoEtherCAT::SetSWLimit(double dPosLimit_um, double dNegLimit_um, BOOL bDisable)
{
	double dPosLimitPulse	= 0.0;
	double dNegLimitPulse	= 0.0;

	m_dSwLimit_Posi_um = dPosLimit_um;
	m_dSwLimit_Nega_um = dNegLimit_um;

	if (m_bDegeeCtrl == TRUE)
	{
		dPosLimitPulse = dPosLimit_um / (m_dScaleFactor + m_dScaleFactorOffset);
		dNegLimitPulse = dNegLimit_um / (m_dScaleFactor + m_dScaleFactorOffset);
	}
	else
	{
		if (m_nDir_Move == 1)
		{
			dPosLimitPulse = dPosLimit_um / (m_dScaleFactor + m_dScaleFactorOffset);
			dNegLimitPulse = dNegLimit_um / (m_dScaleFactor + m_dScaleFactorOffset);
		}
		else
		{
			dPosLimitPulse = (double)m_nDir_Move * dNegLimit_um / (m_dScaleFactor + m_dScaleFactorOffset);
			dNegLimitPulse = (double)m_nDir_Move * dPosLimit_um / (m_dScaleFactor + m_dScaleFactorOffset);
		}
	}

	m_dPosLimitPulse = dPosLimitPulse;
	m_dNegLimitPulse = dNegLimitPulse;
	
	if (g_pWmx3Lib_cm != nullptr)
	{
		Config::LimitParam LimitParam;

		long Result = g_pWmx3Lib_cm->config->GetLimitParam(m_nAxisNumPhy, &LimitParam);
		m_MtLog.LOG(_T("GetLimitParam (nPhyAxis: %d) = %d"), m_nAxisNumPhy, Result);

		if (Result != 0)
		{
			m_MtLog.LOG(_T("GetLimitParam failed, nPhyAxis: %d"), m_nAxisNumPhy);
			m_bMotorDisable = true;
			return false;
		}

		LimitParam.softLimitType = Config::LimitSwitchType::Dec;

		// 일반 Running
		if (bDisable == FALSE)
		{
			LimitParam.softLimitType = Config::LimitSwitchType::Dec;
		}
		// 원점체크 시작 시
		else if (bDisable == TRUE)
		{
			LimitParam.softLimitType = Config::LimitSwitchType::None;
		}

		LimitParam.softLimitNegativePos = m_dNegLimitPulse;
		LimitParam.softLimitPositivePos = m_dPosLimitPulse;


		m_MtLog.LOG(_T("PhyAxis: %d, Set softlimit Disable %s, Negative pos = %.5fmm, Positive pos = %.5fmm"),
			m_nAxisNumPhy, (bDisable == TRUE ? _T("True") : _T("False")), m_dSwLimit_Nega_um * 1000.0, m_dSwLimit_Posi_um * 1000.0);

		Result = g_pWmx3Lib_cm->config->SetLimitParam(m_nAxisNumPhy, &LimitParam);
		m_MtLog.LOG(_T("SetLimitParam (nPhyAxis: %d) { lsType: %d, lsDec: %lf } = %d"),
			m_nAxisNumPhy, LimitParam.lsType, LimitParam.lsDec, Result);

		if (Result != 0)
		{
			m_MtLog.LOG(_T("SetLimitParam failed, nPhyAxis: %d"), m_nAxisNumPhy);
			m_bMotorDisable = true;
			return false;
		}
	}
	

	return true;
	//m_MotorLogger.LOG("Set %s[%d] Axis Software Limit : [Pos %0.2f Pulse] [Neg %0.2f Pulse]", m_szAxisName, m_nAxisNumPhy, dPosLimitPulse, dNegLimitPulse);
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-08-26]
  NAME     : 
  PARAMS   : 0=disable, 1=enable
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void OneAxis_SoftServoEtherCAT::EnableSWLimit(BOOL bEnable)
{
	SetSWLimit(m_dSwLimit_Posi_um, m_dSwLimit_Nega_um, !bEnable);
}



/*=============================================================================
  AUTHOR   : 이원용
  NAME     : InSequence
  PARAMS   : None
  RETURN   : bool
  COMMENTS : 축이 명령을 수행중 일때 TRUE(1)을 돌려줌
==============================================================================*/
bool OneAxis_SoftServoEtherCAT::InSequence()
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return true;
	}

	CoreMotionStatus CmStatus;
	g_pWmx3Lib_cm->GetStatus(&CmStatus);
	CoreMotionAxisStatus cmAxis = CmStatus.axesStatus[m_nAxisNumPhy];

	bool bInposition = false;
	if (cmAxis.inPos && cmAxis.delayedPosSet)
	{
		bInposition = true;
	}

	if (!cmAxis.servoOn)
		bInposition = true;

	return cmAxis.opState != OperationState::Idle || !bInposition;
}

/*=============================================================================
  AUTHOR   : 이원용
  NAME     : InMotion
  PARAMS   : None
  RETURN   : bool 
  COMMENTS : 축이 동작중일때 TRUE(1)을 돌려줌
==============================================================================*/
bool OneAxis_SoftServoEtherCAT::InMotion()
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return true;
	}

	CoreMotionStatus CmStatus;
	g_pWmx3Lib_cm->GetStatus(&CmStatus);
	CoreMotionAxisStatus cmAxis = CmStatus.axesStatus[m_nAxisNumPhy];
	bool bInposition = false;
	if (cmAxis.inPos && cmAxis.delayedPosSet)
	{
		bInposition = true;
	}
	
	if (!cmAxis.servoOn)
		bInposition = true;

	return cmAxis.opState != OperationState::Idle || !bInposition; //!cmAxis.motionComplete;
}

/*=============================================================================
  AUTHOR   : 이원용
  NAME     : InPosition
  PARAMS   : None
  RETURN   : bool
  COMMENTS : 위치오차가 In_Position범위 내일때 TRUE(1)을 돌려줌
==============================================================================*/
bool OneAxis_SoftServoEtherCAT::InPosition()
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return true;
	}
	
	CoreMotionStatus CmStatus;
	g_pWmx3Lib_cm->GetStatus(&CmStatus);
	CoreMotionAxisStatus cmAxis = CmStatus.axesStatus[m_nAxisNumPhy];
	
	bool bInPosition = false;

	if(cmAxis.inPos == true && cmAxis.delayedPosSet == true)
	{
		bInPosition = true;
	}

	return bInPosition;
}

/*=============================================================================
  AUTHOR   : 이원용
  NAME     : IsAxisDone
  PARAMS   : None
  RETURN   : bool
  COMMENTS : motion_done && in_position 일때 TRUE(1)을 돌려줌
==============================================================================*/
bool OneAxis_SoftServoEtherCAT::IsAxisDone()
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return true;
	}

	CoreMotionStatus CmStatus;
	g_pWmx3Lib_cm->GetStatus(&CmStatus);
	CoreMotionAxisStatus cmAxis = CmStatus.axesStatus[m_nAxisNumPhy];

//	int nRet = 1;
//
//	int nInposSts	= cmAxis.inPos;
//	int nMotionDone = cmAxis.motionComplete;
//
//	if (nInposSts != TRUE)
//	{
//		nRet = 0;
//	}
//
//	if (nMotionDone != TRUE)
//	{
//		nRet = 0;
//	}
//
//	if (m_eMotorSyncType == eMotorSyncType_Master) {
//		cmAxis = CmStatus.axesStatus[m_nSlaveNo];
//
//		nInposSts = cmAxis.inPos;
//		nMotionDone = cmAxis.motionComplete;
//		
//		if (nInposSts != TRUE)
//		{
//			nRet = 0;
//		}
//
//		if (nMotionDone != TRUE)
//		{
//			nRet = 0;
//		}
//	}
//
//	return nRet;

	bool bAxisDone = true;

	if (m_eMotorSyncType == eMotorSyncType_Master)
		cmAxis = CmStatus.axesStatus[m_nSlaveNo];

	if (cmAxis.opState != OperationState::Idle || !cmAxis.inPos || !cmAxis.delayedPosSet)
		bAxisDone = false;

	return bAxisDone;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
bool OneAxis_SoftServoEtherCAT::IsMotorStop()		//모터 동작 체크용
{
	if( InSequence() || InMotion() )
		return false;

	return true;
}


//동작 상태
bool OneAxis_SoftServoEtherCAT::IsPositionDone()	//위치 체크용
{
	//if( InMotion() && InPosition() )
	//{
	//	double dCmdPos = GetCommandPos();
	//	double dCurPos = GetCurrentPos();
	//	double dDifPos = abs(dCmdPos-dCurPos);
	//	double dInPos =  0;
	//	get_in_position(m_nAxisNumPhy, &dInPos);
	//	
	//	if( dDifPos<= dInPos )
	//		return true;
	//}

	return false;
}

/*=============================================================================
  RETURN   : 상태정보
==============================================================================*/
int	OneAxis_SoftServoEtherCAT::GetAxisStatus()
{
	CoreMotionStatus CmStatus;
	g_pWmx3Lib_cm->GetStatus(&CmStatus);
	CoreMotionAxisStatus cmAxis = CmStatus.axesStatus[m_nAxisNumPhy];

	int nRet = 0;
	if (cmAxis.ampAlarm == TRUE)
	{
		nRet = cmAxis.ampAlarmCode;
	}
	return nRet;
}

ERR_CODE OneAxis_SoftServoEtherCAT::OriginStart()		//Switch 형식의 오리진 시작
{
//	if (!g_pMotion_Cont_Imp->GetInitBoardStatus()) {
//		return ERR_MOTOR_SYS_BOARD_INITIALIZTION_FAILURE;
//	}
//	if (m_bMotorDisable) {
//		return ERR_MOTOR_CMN_DISABLED;
//	}
//
//	m_bOri_Stop = FALSE;
//	m_nOri_Flag = eMotorOrgFlag_ING;
//
//	return ERR_SUCCESS;

	return Origin();
}

int	OneAxis_SoftServoEtherCAT::GetNoControlAxis()	//제어하고 있는 축수
{

	//int nRet=0;
	//nRet = mmc_all_axes();
	//return nRet;

	return 0;
}

double OneAxis_SoftServoEtherCAT::GetTargetPos(bool bMM)
{
	double dReturnTaget = 0.0;
	if (bMM == true)	// um -> mm 단위로 반환 시킨다.
	{
		dReturnTaget = m_dTarget_pos_um / 1000.0;
	}
	return m_dTarget_pos_um;
}

ERR_CODE OneAxis_SoftServoEtherCAT::SetEmergencyStop()
{
	if (!g_pMotion_Cont_Imp->GetInitBoardStatus()) {
		return ERR_MOTOR_SYS_BOARD_INITIALIZTION_FAILURE;
	}
	if (m_bMotorDisable) {
		return ERR_MOTOR_CMN_DISABLED;
	}

	int nErrCode = g_pWmx3Lib_cm->motion->ExecQuickStop(m_nAxisNumPhy);
	if(nErrCode != wmx3Api::ErrorCode::None)
	{
		g_pWmx3Lib_cm->home->SetHomeDone(m_nAxisNumPhy, 0);
	}

	//AlarmReset();

	return NormalizeError(nErrCode);
}

void OneAxis_SoftServoEtherCAT::LogMtStrage( int nAxisNumPhy, double dPos_um, double dSpeed )
{
	// Motor Log 남기지 않도록 주석처리 20190110 김현진
	//ST_MOTORSTATUS stMotorData;
	//stMotorData.dAcc            = GetAcc();
	//stMotorData.dDcc			= GetDcc();
	//stMotorData.dCommand		= GetCommandPos();
	//stMotorData.dEncoder		= GetCurrentPos();
	//stMotorData.bServoStatus	= GetServoOn();
	//stMotorData.iInsequence		= in_sequence(nAxisNumPhy);
	//stMotorData.iInmotion		= in_motion(nAxisNumPhy);
	//stMotorData.iInposition		= in_position(nAxisNumPhy);
	//stMotorData.iFramesleft		= frames_left(nAxisNumPhy);
	//stMotorData.iMotiondone		= motion_done(nAxisNumPhy);
	//stMotorData.iAxisdone		= axis_done(nAxisNumPhy);
	//stMotorData.iAxisNum		= nAxisNumPhy;
	//stMotorData.dTarget			= dPos_um;
	//stMotorData.dMotorSpeed		= dSpeed;
	//get_command_rpm(m_nAxisNumPhy, &stMotorData.sEncoderRPM);
	//
	//CString strData = _T("");
	//strData.Format(_T("motor[%s][T Pos %.2fmm][E Pos %.2fmm][cmd Pos %.2fmm][Spd: %.2f pps][Acc: %d][Dcc: %d][AmpFault: %d]"),
	//	m_szAxisName,
	//	stMotorData.dTarget/1000.0,
	//	stMotorData.dEncoder/1000.0,
	//	stMotorData.dCommand/1000.0,
	//	stMotorData.dMotorSpeed, 
	//	stMotorData.dAcc, 
	//	stMotorData.dDcc, 
	//	stMotorData.bServoStatus
	//);
	//
	//m_MtLog.LOG(strData);
	//
	//strData.Format(_T("[in_Sequence: %d][in_motion: %d] [in_position : %d] [frames_left : %d] [motion_done : %d] [axis_done : %d] [Cmd RPM : %d]"),
	//	stMotorData.iInsequence,
	//	stMotorData.iInmotion, 
	//	stMotorData.iInposition, 
	//	stMotorData.iFramesleft, 
	//	stMotorData.iMotiondone, 
	//	stMotorData.iAxisdone, 
	//	stMotorData.sEncoderRPM);
	//m_MtLog.LOG(strData);
}

double OneAxis_SoftServoEtherCAT::GetScaleFactor()
{
	return m_dScaleFactor;
}

void OneAxis_SoftServoEtherCAT::SetScaleFactor(double dScaleFactor)
{
	m_dScaleFactor = dScaleFactor;
}

double OneAxis_SoftServoEtherCAT::GetMotorInposition()
{
	double dInpositon = (m_dScaleFactor + m_dScaleFactorOffset) * MOTOR_INPOSITION_PULSE_COUNT;// (m_dScaleFactor + m_dScaleFactorOffset > MOTOR_INPOSITION_um) ? (m_dScaleFactor + m_dScaleFactorOffset) * 4 : 20.0;
	return dInpositon;
}

void OneAxis_SoftServoEtherCAT::set_sync_axis_map_setting(int nMaster, int nSlave)
{
	//set_sync_map_axes(nMaster, nSlave);
	//set_sync_control_setting(TRUE);
	//frames_clear(nMaster);
	return;
}

void OneAxis_SoftServoEtherCAT::set_sync_control_setting(BOOL nAction)
{
	//set_sync_control(nAction);
	return;
}

ERR_CODE OneAxis_SoftServoEtherCAT::SplineMove(std::vector<_stSpline> SplineMoveCmd, int nAxisNumPhy1, int nAxisNumPhy2)
{
	if (g_pAdvMotion == nullptr /*|| pPathIntplCommand == nullptr*/)
	{
		ASSERT(0);
	}
	
	AdvMotion::PathIntplCommand advMotionCmd = AdvMotion::PathIntplCommand();

	int nPointCnt = SplineMoveCmd.size();
	advMotionCmd.numPoints = nPointCnt;
	advMotionCmd.axis[0] = nAxisNumPhy1;
	advMotionCmd.axis[1] = nAxisNumPhy2;

	int nIdx = 0;
	for each (auto var in SplineMoveCmd)
	{
		advMotionCmd.type[nIdx] = AdvMotion::PathIntplSegmentType::Linear;
		advMotionCmd.profile[nIdx].type = ProfileType::JerkRatio;
		// Speed
		advMotionCmd.profile[nIdx].velocity = var.dSpeed;		
		// AC
		advMotionCmd.profile[nIdx].acc = var.dAcc;
		// DC
		advMotionCmd.profile[nIdx].dec = var.dDec;
		advMotionCmd.profile[nIdx].jerkAccRatio = 0.5;
		advMotionCmd.profile[nIdx].jerkDecRatio = 0.5;

		advMotionCmd.target[0][nIdx] = var.dTarget[0];
		advMotionCmd.target[1][nIdx] = var.dTarget[1];
		nIdx++;
	}

	for (int i = 0; i < nPointCnt -1; i++)
	{
		advMotionCmd.autoSmoothRadius[i] = 1000;
	}

	advMotionCmd.enableAutoSmooth = 1;
	advMotionCmd.enableConstProfile = 1;

	//return g_pAdvMotion->advMotion->StartPathIntplPos(&advMotionCmd);
	return 0;
}

ERR_CODE OneAxis_SoftServoEtherCAT::SetAmpFaultLevel(BOOL bStatus)
{
	//return set_amp_fault_level( m_nAxisNumPhy, bStatus);

	return ERR_SUCCESS;
}
ERR_CODE OneAxis_SoftServoEtherCAT::SetAmpInpositionLevel(BOOL bStatus)
{
	
	//return set_inposition_level(m_nAxisNumPhy, bStatus);

	return ERR_SUCCESS;
}

ERR_CODE OneAxis_SoftServoEtherCAT::SetAmpNegativeLimitLevel(BOOL bStatus)
{
	//return set_negative_level(m_nAxisNumPhy,bStatus);

	return ERR_SUCCESS;
}
ERR_CODE OneAxis_SoftServoEtherCAT::SetAmpPositiveLimitLevel(BOOL bStatus)
{
	//return set_positive_level(m_nAxisNumPhy,bStatus);

	return ERR_SUCCESS;
}

bool OneAxis_SoftServoEtherCAT::CheckLimitClose()
{
	double dCurPos = 0.0;
	GetCurrentPos(dCurPos);

	// 움직이고자 하는 위치가 s/w limit를 벗어나는 위치인지를 확인
	if (dCurPos > m_dSwLimit_Nega_um && dCurPos < m_dSwLimit_Posi_um) {
		return false;
	}

	return true;
}

void OneAxis_SoftServoEtherCAT::SetScaleFactorOffset(double dOffset)
{
	m_dScaleFactorOffset = dOffset;
}

double OneAxis_SoftServoEtherCAT::GetScaleFactorOffset()
{
	return m_dScaleFactorOffset;
}


ERR_CODE OneAxis_SoftServoEtherCAT::NormalizeError(int Error)
{
	// softservo error를 ERR_CODE로 변환
	switch (Error)
	{
	case CoreMotionErrorCode::None: return ERR_SUCCESS;
	case CoreMotionErrorCode::InvalidAxis: return ERR_MOTOR_CMN_AMP_FAULT;
	case CoreMotionErrorCode::ServoOnError: return ERR_MOTOR_SYS_SERVO_ON_FAILED;
	case CoreMotionErrorCode::AxisAmpAlarm: return ERR_MOTOR_CMN_AMP_FAULT;
	case CoreMotionErrorCode::ServoOff: return ERR_MOTOR_CMN_SERVO_OFF;

	case CoreMotionErrorCode::AxisHardLimit:
		return IsPositiveSensorOn() ? 
			ERR_MOTOR_CMN_HARDWARE_POSITIVE_LIMIT_HIT : 
			ERR_MOTOR_CMN_HARDWARE_NEGATIVE_LIMIT_HIT;
	}

	return ERR_MOTOR_CMN_BOARD_API_FAILURE;
}

// double OneAxis_SoftServoEtherCAT::GetBacklashOffset(double dMovePos)
// {
// 	double dOffset = 0.0;
// 
// 	if (m_dPrevMovePos != dMovePos && m_dTarget_pos_um != dMovePos)
// 	{
// 		//150mm를 기준으로 150mm 보다 작은 이동 거리는 Off-Set을 Liner하게 구하여 누적 시켜준다.
// 		if (fabs(m_dPrevMovePos - dMovePos) < m_dBacklashMoveDist)
// 		{
// 			if (m_dBacklashMoveDist == 0.0)	// 20190415 ihc. 0나누기 방지
// 			{
// 				//150mm보다 작은 경우 150mm를 기준으로 %를 계산 하여 적용
// 				dOffset = m_arrBackLashOffset[m_nMotorSpdRateIdx] * (fabs(m_dPrevMovePos - dMovePos) / 1.0);
// 			}
// 			else
// 			{
// 				//150mm보다 작은 경우 150mm를 기준으로 %를 계산 하여 적용
// 				dOffset = m_arrBackLashOffset[m_nMotorSpdRateIdx] * (fabs(m_dPrevMovePos - dMovePos) / m_dBacklashMoveDist);
// 			}
// 		}
// 		else
// 		{
// 			dOffset = m_arrBackLashOffset[m_nMotorSpdRateIdx];
// 		}
// 
// 		// 현재 위치에서 플러스 방향으로 가는 경우
// 		if (m_dPrevMovePos < dMovePos)
// 		{
// 			//off_set = -off_set;
// 			//플러스 방향인 경우 무조건 Off-Set을 "0"해서 Test 진행
// 			//해당 내용 관련하여 확정된 내용 없음
// 			dOffset = 0.0;
// 			m_arrPrevBackLashOffset[m_nMotorSpdRateIdx] = 0.0;
// 		}
// 
// 		if (fabs(m_arrPrevBackLashOffset[m_nMotorSpdRateIdx] + dOffset) > fabs(m_arrBackLashOffset[m_nMotorSpdRateIdx]))
// 		{
// 			m_arrPrevBackLashOffset[m_nMotorSpdRateIdx] = m_arrBackLashOffset[m_nMotorSpdRateIdx];
// 		}
// 		else
// 		{
// 			m_arrPrevBackLashOffset[m_nMotorSpdRateIdx] = dOffset;
// 		}
// 
// 		m_dPrevMovePos = dMovePos;
// 	}
// 	else
// 	{
// 		m_arrPrevBackLashOffset[m_nMotorSpdRateIdx] = 0.0;
// 	}
// 	//	}
// 
// 	if (isnan(m_arrPrevBackLashOffset[m_nMotorSpdRateIdx]) || isinf(m_arrPrevBackLashOffset[m_nMotorSpdRateIdx]))
// 	{
// 		m_arrPrevBackLashOffset[m_nMotorSpdRateIdx] = 0.0;
// 	}
// 	return m_arrPrevBackLashOffset[m_nMotorSpdRateIdx];
// }


double OneAxis_SoftServoEtherCAT::GetBacklashDataAboutSpeed(double dspeed_umps)
{
	int nSpeedRate = 0;
	// 현재 속도를 백분율로 계산. (0~100%)
	double dMax_speed_umps = GetSpeed();
	nSpeedRate = (int)((dspeed_umps / dMax_speed_umps) * 100.0);

	const int nSpdMinIndex = 0;
	const int nSpdMaxIndex = 10;
	int nSpeedIdx = 0;

	// Spped Rate가 계산되어 넘어 오기 때문에 딱 맞지 않음
	if (nSpeedRate < 10)
	{
		if (nSpeedRate % 10 > 7)
			nSpeedRate = 10;
		else
			nSpeedRate = 5;
	}
	else {
		if (nSpeedRate % 10 > 7)
		{
			nSpeedRate = nSpeedRate + (10 - (nSpeedRate % 10));
		}
		else
		{
			nSpeedRate = nSpeedRate - (nSpeedRate % 10);
		}
	}

	if (nSpeedRate == 5) {
		nSpeedIdx = nSpdMinIndex;
	}
	else {
		nSpeedIdx = nSpeedRate / 10;
	}

	if (nSpeedIdx > nSpdMaxIndex || nSpeedIdx <= 0) {
		nSpeedIdx = nSpdMinIndex;
	}

	double dBacklashOffset = m_vBacklashOffset_XY[nSpeedIdx];

	//if( fabs( dBacklashOffset ) > 1000.0 )
	//{
	//	// 과도한 Offset 방지. 1 mm 초과하는 경우 0 리턴.
	//	return 0.0;
	//}

	return dBacklashOffset;
}

double OneAxis_SoftServoEtherCAT::GetBacklashParameterData(double dMoveDistance, double dspeed_umps)
{
	int nSpeedRate = 0;
	double dDistanceRate = 0.0;
	int nDistanceIdx = 0;
	// 현재 속도를 백분율로 계산. (0~100%)
	double dMax_speed_umps = GetSpeed();
	nSpeedRate = (int)((dspeed_umps / dMax_speed_umps) * 100.0);

	const int nSpdMinIndex = 0;
	const int nSpdMaxIndex = 10;
	int nSpeedIdx = 0;

	// Spped Rate가 계산되어 넘어 오기 때문에 딱 맞지 않음
	if (nSpeedRate < 10)
	{
		if (nSpeedRate % 10 > 7)
			nSpeedRate = 10;
		else
			nSpeedRate = 5;
	}
	else {
		if (nSpeedRate % 10 > 7)
		{
			nSpeedRate = nSpeedRate + (10 - (nSpeedRate % 10));
		}
		else
		{
			nSpeedRate = nSpeedRate - (nSpeedRate % 10);
		}
	}

	if (nSpeedRate == 5) {
		nSpeedIdx = nSpdMinIndex;
	}
	else {
		nSpeedIdx = nSpeedRate / 10;
	}

	if (nSpeedIdx > nSpdMaxIndex || nSpeedIdx <= 0) {
		nSpeedIdx = nSpdMinIndex;
	}

	nDistanceIdx = (int)(dMoveDistance / 10000.0);
	dDistanceRate = (dMoveDistance / 10000.0) - (double)nDistanceIdx;

	if (nDistanceIdx > 14)
	{
		nDistanceIdx = 14;
		dDistanceRate = 1.0;
	}

	double dSectionMaxBacklashOffset = m_dBacklashParameterOffset_XY[nDistanceIdx][nSpeedIdx];
	double dSectionMinBacklashOffset = 0.0;

	if (nDistanceIdx != 0)
	{
		dSectionMinBacklashOffset = m_dBacklashParameterOffset_XY[nDistanceIdx - 1][nSpeedIdx];
	}

	double dBacklashOffset = dSectionMinBacklashOffset + ((dSectionMaxBacklashOffset - dSectionMinBacklashOffset) * dDistanceRate);

	return dBacklashOffset;
}

void OneAxis_SoftServoEtherCAT::SetSpeedBacklash(int nSpdIdx, double dValue)
{
	m_vBacklashOffset_XY[nSpdIdx] = dValue;
}

void OneAxis_SoftServoEtherCAT::SetParameterBacklash(int nDistanceIdx, int nSpdIdx, double dValue)
{
	m_dBacklashParameterOffset_XY[nDistanceIdx][nSpdIdx] = dValue;
}

double OneAxis_SoftServoEtherCAT::GetBacklashOffset_TestPpX(double dMovePos, double dPPS)
{
	// 현재 속도에 맞는 Backlash Offset Data Load.
	double dTargetDistance_ApplyBefore = fabs(m_dBefore_BacklashPos - dMovePos);
	double dTargetDistance_ApplyAfter = fabs(m_dAfter_BacklashPos - dMovePos);
	double dCheckYMove = dTargetDistance_ApplyAfter - fabs(m_dMem_BacklashOffset);
	double dMotorSacale = m_dScaleFactor * m_dServo_Pulse;
	double dNewDistanceRate_Pulley = dTargetDistance_ApplyBefore / dMotorSacale;
	double dBacklashOffset = 0.0;

	if (m_dAfter_BacklashPos > dMovePos && dCheckYMove > FLT_EPSILON) // - 방향으로 이동할 때, Backlash 감소. 
	{
		m_dDistanceRate -= dNewDistanceRate_Pulley;

		if (m_dDistanceRate < 0.0)
		{
			m_dDistanceRate = 0.0;
		}

		if (dNewDistanceRate_Pulley > 1.0)
		{
			dNewDistanceRate_Pulley = 1.0;
		}

		dTargetDistance_ApplyBefore = dNewDistanceRate_Pulley * 150000.0;

		dBacklashOffset = m_dMem_BacklashOffset - GetBacklashParameterData(dTargetDistance_ApplyBefore, dPPS);

		if (dBacklashOffset < 0.0)
		{
			dBacklashOffset = 0.0;
		}

		m_dMem_BacklashOffset = dBacklashOffset;
	}
	else if (dTargetDistance_ApplyAfter == 0.0) // Backlash Offset 적용 후 추가 이동하지 않도록 함.
	{
		dBacklashOffset = 0.0;
	}
	else // + 방향으로 이동할 때, Backlash 증가. 
	{
		m_dDistanceRate += dNewDistanceRate_Pulley;

		if (m_dDistanceRate > 1.0)
		{
			m_dDistanceRate = 1.0;
		}

		dTargetDistance_ApplyBefore = m_dDistanceRate * 150000.0;

		dBacklashOffset = GetBacklashParameterData(dTargetDistance_ApplyBefore, dPPS);

		m_dMem_BacklashOffset = dBacklashOffset;
	}

	m_dBefore_BacklashPos = dMovePos; // 처음 Target Pos 저장
	m_dAfter_BacklashPos = dMovePos + dBacklashOffset;
	return dBacklashOffset;
}

double OneAxis_SoftServoEtherCAT::GetBacklashOffset(double dMovePos, double dPPS)
{
	// 현재 속도에 맞는 Backlash Offset Data Load.
	double dBacklashOffset = GetBacklashDataAboutSpeed(dPPS);

	double dTargetDistance_ApplyBefore = fabs(m_dBefore_BacklashPos - dMovePos);
	double dTargetDistance_ApplyAfter = fabs(m_dAfter_BacklashPos - dMovePos);
	double dCheckYMove = dTargetDistance_ApplyAfter - fabs(m_dMem_BacklashOffset);
	double dMotorSacale = m_dScaleFactor * m_dServo_Pulse;
	double dNewDistanceRate = dTargetDistance_ApplyBefore / dMotorSacale;

	if (m_dAfter_BacklashPos < dMovePos && dCheckYMove > FLT_EPSILON)
	{
		m_dDistanceRate -= dNewDistanceRate;
		if (m_dDistanceRate < 0.0)
		{
			m_dDistanceRate = 0.0;
		}

		dBacklashOffset *= m_dDistanceRate; // 100mm를 기준으로 Backlash Offset 비율로 적용.

		m_dMem_BacklashOffset = dBacklashOffset;
	}
	else if (dTargetDistance_ApplyAfter == 0.0) // Backlash Offset 적용 후 추가 이동하지 않도록 함.
	{
		dBacklashOffset = 0.0;
	}
	else
	{
		m_dDistanceRate += dNewDistanceRate;
		if (m_dDistanceRate > 1)
		{
			m_dDistanceRate = 1;
		}

		dBacklashOffset *= m_dDistanceRate; // 100mm를 기준으로 Backlash Offset 비율로 적용.

		m_dMem_BacklashOffset = dBacklashOffset;
	}

	m_dBefore_BacklashPos = dMovePos; // 처음 Target Pos 저장
	m_dAfter_BacklashPos = dMovePos + dBacklashOffset;
	return dBacklashOffset;
}

double OneAxis_SoftServoEtherCAT::GetBacklashOffset_TestPP_X1(double dMovePos, double dPPS)
{
	// 현재 속도에 맞는 Backlash Offset Data Load.
	double dBacklashOffset = GetBacklashDataAboutSpeed(dPPS);

	double dTargetDistance_ApplyBefore = fabs(m_dBefore_BacklashPos - dMovePos);
	double dTargetDistance_ApplyAfter = fabs(m_dAfter_BacklashPos - dMovePos);
	double dCheckYMove = dTargetDistance_ApplyAfter - fabs(m_dMem_BacklashOffset);
	double dMotorSacale = m_dScaleFactor * m_dServo_Pulse;
	double dNewDistanceRate = dTargetDistance_ApplyBefore / dMotorSacale;

	if (m_dAfter_BacklashPos > dMovePos && dCheckYMove > FLT_EPSILON)
	{
		m_dDistanceRate -= dNewDistanceRate;
		if (m_dDistanceRate < 0.0)
		{
			m_dDistanceRate = 0.0;
		}

		dBacklashOffset *= m_dDistanceRate; // 100mm를 기준으로 Backlash Offset 비율로 적용.

		m_dMem_BacklashOffset = dBacklashOffset;
	}
	else if (dTargetDistance_ApplyAfter == 0.0) // Backlash Offset 적용 후 추가 이동하지 않도록 함.
	{
		dBacklashOffset = 0.0;
	}
	else
	{
		m_dDistanceRate += dNewDistanceRate;
		if (m_dDistanceRate > 1)
		{
			m_dDistanceRate = 1;
		}

		dBacklashOffset *= m_dDistanceRate; // 100mm를 기준으로 Backlash Offset 비율로 적용.

		m_dMem_BacklashOffset = dBacklashOffset;
	}

	m_dBefore_BacklashPos = dMovePos; // 처음 Target Pos 저장
	m_dAfter_BacklashPos = dMovePos + dBacklashOffset;
	return dBacklashOffset;
}
