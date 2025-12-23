#include "stdafx.h"
#include "SoftServoControl.h"
#include "ConfigData.h"
#include "strsafe.h"
#include "OneAxis_SoftServoEtherCAT.h"

SoftServoControl::SoftServoControl()
{
	memset(m_szInitFileName, 0x00, sizeof(m_szInitFileName));
}


SoftServoControl::~SoftServoControl()
{
	memset(m_szInitFileName, 0x00, sizeof(m_szInitFileName));
}

int SoftServoControl::CreateDev(TCHAR* szIniFileName, int nFileLen)
{
	int nDIO_Type;
	int nMotor_Type;
	// TODO STRIP : INI 파일이름 전달 받기
//	StringCchCopy(m_szInitFileName, _countof(m_szInitFileName), CString(__func__));
	StringCchCopy(m_szInitFileName, _countof(m_szInitFileName), szIniFileName);

	// TODO STRIP : ini 파일에서 읽어서 처리
	CConfigData ConfigData(m_szInitFileName);
	TCHAR szTemp[128] = { 0, };

	nDIO_Type = ConfigData.GetInt(_T("DIO CONFIG"), _T("Driver Type"), _T("0"));
	nMotor_Type = ConfigData.GetInt(_T("Motion Board"), _T("Type"), _T("0"));
	
	if (/*nMotor_Type != DEF_MOTION_TYPE_ECAT_SOFT_SERVO &&*/ nDIO_Type != _DIO_TYPE_SOFTSERVO_)
	{
		return -1;
	}

	// Restart the engine.
	int RestartEngine = g_Wmx3Lib.RestartEngine("C:\\Program Files\\SoftServo\\WMX3\\", INFINITE);
// 	g_Wmx3Lib.StopEngine();
// 	Sleep(1000);
	// Create device.
	//int RestartEngine = g_Wmx3Lib.StartEngine("C:\\Program Files\\SoftServo\\WMX3\\", INFINITE);

	RestartEngine = g_Wmx3Lib.CreateDevice("C:\\Program Files\\SoftServo\\WMX3\\", DeviceType::DeviceTypeNormal, INFINITE);
	RestartEngine = g_Wmx3Lib.SetDeviceName("device");

	g_Wmx3Lib.SetWatchdog(0);
	if (g_pWmx3Lib_cm == nullptr)
		g_pWmx3Lib_cm = new CoreMotion(&g_Wmx3Lib);

	if (g_pAdvMotion == nullptr)
		g_pAdvMotion = new AdvancedMotion(&g_Wmx3Lib);

	CoreMotionStatus cCoreMotionStatus;
	
	while(true)
	{
		g_pWmx3Lib_cm->GetStatus(&cCoreMotionStatus);
		Sleep(100);

		if (cCoreMotionStatus.engineState == wmx3Api::EngineState::Running ||
			cCoreMotionStatus.engineState == wmx3Api::EngineState::Communicating)
		{
			break;
		}
	} 
	// Communicating 상태가 아니면 계속 체크 한다.
	

	//모터에서 Watchdog을 확인 안 하도록 수정
	//g_Wmx3Lib.SetWatchdog(0);

	// Get created device state.
	wmx3Api::DevicesInfoA devInfo;
	g_Wmx3Lib.GetAllDevices(&devInfo);
	Sleep(5000);

	int nErrCode = SoftServoControl::StartCommunication();		 
	if (nErrCode != CoreMotionErrorCode::None)
	{
		CString text = _T("");
		text.Format(_T("Failed to initialize SoftServo Errcode = %d"), nErrCode);
		AfxMessageBox(text, MB_ICONERROR | MB_SYSTEMMODAL | MB_OK);
		//		ExitProcess(0);
		return -1;
	}

	return 0;
}

int SoftServoControl::StartCommunication()
{
	int nErrCode = (int)g_Wmx3Lib.StartCommunication(INFINITE);

// 	if (nErrCode != CoreMotionErrorCode::None)
// 		return false;

	return nErrCode;
}

void SoftServoControl::DestroyEtherCat(void)
{
	// 통신 연결 해제
	g_Wmx3Lib.StopCommunication(INFINITE);
	g_Wmx3Lib.CloseDevice();

 	if (g_pWmx3Lib_cm != NULL)
 	{
 		delete g_pWmx3Lib_cm;
 		g_pWmx3Lib_cm = NULL;
 	}
 
 	if (g_pAdvMotion != nullptr)
 	{
 		SAFE_DELETE(g_pAdvMotion);
 	}

 	if (g_pWmx3Lib_Io != NULL)
 	{
 		delete g_pWmx3Lib_Io;
 		g_pWmx3Lib_Io = NULL;
 	}
}

void SoftServoControl::CloseDevice()
{
	g_Wmx3Lib.StopCommunication(INFINITE);
	g_Wmx3Lib.CloseDevice();
}