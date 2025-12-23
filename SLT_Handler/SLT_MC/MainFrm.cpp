
// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "SLT_MC.h"

#include "MainFrm.h"
#include "DEF_LIB_IPC_SOCK.h"
#include "DlgAlarmMsgBox.h"
#include "DlgAlarmMsgBoxDoor.h"

#include "DlgManu.h"
#include "DlgManuMotor.h"
#include "DlgManuMotorTeaching.h"
#include "DlgManuTesterIF.h"
#include "DlgSetupSignalTower.h"
#include "../Include/OcxFiles/spreadsheet.h"
#include "afxwin.h"
#include "TeachingFile.h"
#include "RamTeaching.h"
#include "SocketCount.h"
//#include "VisionAutoTeaching/Z Auto Teaching/VAT_ZAutoTeachingDlg.h"
#include "DlgBarcodeSetting.h"

#include "VisionAutoTeaching/MainDlg/VAT_MainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_MESSAGE(UM_ALARM_REPORT, OnReceivedAlarm)
    ON_MESSAGE(UM_GUI_COMM_DLG_EVENT, OnGuiEvent)
    ON_MESSAGE(UM_GUI_COMM_FINALIZE_PGM, OnGuiEventClose)
	ON_MESSAGE(UM_GUI_MULTI_ORIGIN, OnMultiOriginEvent)
	ON_WM_WINDOWPOSCHANGED()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
	m_bManualCmdExcute = FALSE;
	m_pDlgHomeCheckStatus = NULL;
}

CMainFrame::~CMainFrame()
{
	if (m_pDlgHomeCheckStatus != NULL)
	{
		m_pDlgHomeCheckStatus->DestroyWindow();
		delete m_pDlgHomeCheckStatus;
		m_pDlgHomeCheckStatus = NULL;
	}
}



int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

// 	if (!m_wndStatusBar.Create(this) ||
// 		!m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT)))
// 	{
// 		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
// 		return -1;      // 만들지 못했습니다.
// 	}
// 	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	HANDLE hEvent;

	CString strAppName(AfxGetAppName());
	hEvent = CreateEvent(NULL, FALSE, TRUE, AfxGetAppName());

	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		AfxMessageBox("SLT MC Program is already execute!! please check it!!\r\n");
		CloseHandle(hEvent);
		return -1;
	}

	char szVer[100] = {0,};
	char szTitle[100] = {0,};

	sprintf_s(szTitle, sizeof(szTitle), "SLT_MC_D.exe");
	CWnd *pWnd = FindWindow(NULL, szTitle);
	if (pWnd != NULL) {
		AfxMessageBox("MC_D Program is already execute!!!!\r\n");
		CloseHandle(hEvent);
		return -1;
	}

	sprintf_s(szTitle, sizeof(szTitle), "SLT_MC.exe");
	pWnd = FindWindow(NULL, szTitle);
	if (pWnd != NULL) {
		AfxMessageBox("MC Program is already execute!!!!\r\n");
		CloseHandle(hEvent);
		return -1;
	}

#ifdef _DEBUG
    sprintf_s(szTitle, sizeof(szTitle), "SLT_MC_D.exe");
#else
    sprintf_s(szTitle, sizeof(szTitle), "SLT_MC.exe");
#endif

	
	SetWindowText(szTitle);
/*
	int x=GUI_SIZE_X,y=GUI_SIZE_Y;
	int w=GUI_SIZE_W,h=GUI_SIZE_H;
	SetWindowPos(&wndTopMost, x, y, w, h, SWP_SHOWWINDOW);
*/

	MoveToTrayIcon();


	SetTimer(0, DEF_SENSOR_DISP_TM, NULL);

// 	m_pDlgHomeCheckStatus = new CDlgHomeCheckStatus(this);
// 	m_pDlgHomeCheckStatus->Create(IDD_DIALG_HOME_CHECK_STATUS, this);
// 	m_pDlgHomeCheckStatus->ShowWindow(SW_HIDE);

	CloseHandle(hEvent);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
 	cs.style &= ~FWS_ADDTOTITLE; 
 	cs.style &= ~WS_MAXIMIZEBOX;
	cs.style &= ~WS_MINIMIZEBOX;
	cs.style = WS_DLGFRAME;

	if(cs.hMenu!=NULL)
	{
		::DestroyMenu(cs.hMenu);      // delete menu if loaded
		cs.hMenu = NULL;              // no menu for this window
	}

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기

void CMainFrame::MoveToTrayIcon()
{
	m_objTaskTrayIcon.hWnd = this->m_hWnd;
	m_objTaskTrayIcon.uID = 1;
	m_objTaskTrayIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	m_objTaskTrayIcon.uCallbackMessage = WM_MOUSEMOVE;
	m_objTaskTrayIcon.hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);//this->GetIcon(false);

	memset(m_objTaskTrayIcon.szTip, 0, sizeof(m_objTaskTrayIcon.szTip));
	TCHAR szTitle[128] = {0,};
	sprintf_s(szTitle, sizeof(szTitle), _T("SLT_MC"));
	memcpy(m_objTaskTrayIcon.szTip, szTitle, strlen(szTitle));

	m_objTaskTrayIcon.cbSize = sizeof(m_objTaskTrayIcon);

	Shell_NotifyIcon(NIM_ADD, &m_objTaskTrayIcon);
	this->ShowWindow(SW_HIDE);
}

void CMainFrame::ReleaseTrayIcon()
{
	NOTIFYICONDATA data;
	data.cbSize = sizeof(NOTIFYICONDATA);
	data.hWnd   = this->m_hWnd;
	data.uID    = 1;
	data.hIcon  = this->GetIcon(false);

	Shell_NotifyIcon(NIM_DELETE, &data);
}


void CMainFrame::OnDestroy()
{
	CFrameWnd::OnDestroy();

    Shell_NotifyIcon(NIM_DELETE, &m_objTaskTrayIcon);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CMainFrame::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(point.x)
	{
	case WM_LBUTTONDBLCLK :
/*
		int x=GUI_SIZE_X,y=GUI_SIZE_Y;
		int w=GUI_SIZE_W,h=GUI_SIZE_H;
// 		x = (::GetSystemMetrics(SM_CXSCREEN)/2)-(w/2);
// 		y = (::GetSystemMetrics(SM_CYSCREEN)/2)-(h/2);
		SetWindowPos(&wndTopMost, x, y, w, h, SWP_SHOWWINDOW);
*/
		ShowWindow(SW_SHOW);
		ReleaseTrayIcon();
		break;
	}

	CFrameWnd::OnMouseMove(nFlags, point);
}


void CMainFrame::OnClose()
{
	CFrameWnd::OnClose();
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-10-02]
  NAME     : 
  PARAMS   : wParam : ALID
             lParam : CParamList* AlarmData
  RETURN   : 
  COMMENTS : 
==============================================================================*/
LRESULT CMainFrame::OnReceivedAlarm(WPARAM wParam, LPARAM lParam)
{
	ST_ALARM_RPT_OBJ* pstAlarmObj = (ST_ALARM_RPT_OBJ*)wParam;
	_tERROR_MSG tErrorMsg;
	g_GuiComm.Send_AlarmReport(pstAlarmObj->nAlid, eALST_SET);

	g_DB.SELECT_ERROR_MSG_TBL(pstAlarmObj->nAlid, tErrorMsg);
	
	const bool bDoor = (pstAlarmObj->nAlid == ERR_CAN_NOT_COVER_LOCK);

	CDialog* pAlarmMsgBox = bDoor ? 
		(CDialog*)new CDlgAlarmMsgBoxDoor(pstAlarmObj, (CParamList*)lParam, this) : 
		(CDialog*)new CDlgAlarmMsgBox(pstAlarmObj, (CParamList*)lParam, this);

	int nDlgId = bDoor ? IDD_ALARM_MSG_DOOR : IDD_ALARM_MSG;

	CWnd* pWnd = this;
	if (g_DMCont.m_dmVAT.GB(BDM14_VAT_TEACHING_USE) == TRUE) {
		HWND hWnd = ::FindWindow(NULL, _T("Vision Auto Teaching"));
		pWnd = CWnd::FromHandle(hWnd);
		if (pWnd == NULL)
			pWnd = this;
	}

	pAlarmMsgBox->Create(nDlgId, pWnd);

	if (g_DMCont.m_dmEQP.GN(NDM0_MAIN_POWER_CONTROLER_CONNETION_STATUS) != eCommReconnectOK)
	{
		CRect rect;
		pAlarmMsgBox->GetWindowRect(rect);
		pAlarmMsgBox->MoveWindow(100, 150, rect.Width(), rect.Height());
	}

	pAlarmMsgBox->ShowWindow(SW_SHOW);

//원래 방법
// 		CDlgAlarmMsgBox* pAlarmMsgBox = new CDlgAlarmMsgBox(pstAlarmObj, (CParamList*)lParam, this);
// 		pAlarmMsgBox->Create(IDD_ALARM_MSG, this);
// 		pAlarmMsgBox->ShowWindow(SW_SHOW);
//	}
	
	return 0L;
}

LRESULT CMainFrame::OnGuiEvent(WPARAM wParma, LPARAM lParam)
{
    switch(wParma)
    {
    case eDlgProcCmd_Position:
        {
		if (g_DMCont.m_dmEQP.GB(BDM0_OPEN_SLT_UI_DLG_HISTORY) == FALSE)
		{
			CDlgManu dlg;
			dlg.DoModal();
		}
      
        }break;
    case eDlgProcCmd_IoMonitorDIO:
        {
            g_IO.ShowDialog_IO_All();
        }break;
    case eDlgProcCmd_IoMonitorAIO:
        {
            g_AIO.ShowDialog_IO_All();
        }break;
    case eDlgProcCmd_MotorMonitor:
        {
			if (g_DMCont.m_dmEQP.GB(BDM0_OPEN_SLT_UI_DLG_HISTORY) == FALSE)
			{
				CDlgManuMotor dlg;
				dlg.DoModal();
			}
        }break;
    case eDlgProcCmd_TesterIfTest:
        {
			if (g_DMCont.m_dmEQP.GB(BDM0_OPEN_SLT_UI_DLG_HISTORY) == FALSE)
			{
				int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
				if (nTesterOption != eTester_Agent)
				{
					CDlgManuTesterIF dlg;
					dlg.DoModal();
				}
			}
        }break;
    case eDlgProcCmd_Alignment:
        {
			if (g_DMCont.m_dmEQP.GB(BDM0_OPEN_SLT_UI_DLG_HISTORY) == FALSE)
			{
				CDlgManuMotorTeaching dlg;
				dlg.DoModal();
			}
        }break;
    case eDlgProcCmd_SignalTower:
        {
			if (g_DMCont.m_dmEQP.GB(BDM0_OPEN_SLT_UI_DLG_HISTORY) == FALSE)
			{
				CDlgSetupSignalTower dlg;
				dlg.DoModal();
			}
        }break;
	case eDlgProcCmd_SocketContactCount:
		{
			if (g_DMCont.m_dmEQP.GB(BDM0_OPEN_SLT_UI_DLG_HISTORY) == FALSE)
			{
				CSocketCount dlg;
				dlg.DoModal();
			}
		}break;
	case eDlgProcCmd_ServoMotor:
		{
			if (g_DMCont.m_dmEQP.GB(BDM0_OPEN_SLT_UI_DLG_HISTORY) == FALSE)
			{
				CDlgManuMotor dlg;
				dlg.DoModal();
			}
		}break;
	case eDlgProcCmd_TesterInfo:
		{
			int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);

			CString strTestScript = _T("");
			CString strSWVersion = _T("");
			char szBuffer[256] = { 0, };

			g_DMCont.m_dmHandlerOpt.GS(SDM0_TESTER_INFOR_PROGRAM, szBuffer, sizeof(szBuffer));
			strTestScript.Format("%s", szBuffer);

			g_DMCont.m_dmHandlerOpt.GS(SDM0_TESTER_INFOR_VERSION, szBuffer, sizeof(szBuffer));
			strSWVersion.Format("%s", szBuffer);

			int nStationID = -1;
			for (int c = 0; c < MAX_TESTER_EACH_SOCKET; c++) {

				nStationID = c + 1;
				if (g_AppleTestIF.IsConnected(nStationID) == TRUE) {
					CTesterStatus cTesterStatus = g_AppleTestIF.GetTesterStatus(nStationID);

					CString strAPLScript = strTestScript.MakeUpper();
					CString strTesterScript = cTesterStatus.m_sTest_Script_Info.MakeUpper();

					//if (strcmp(cTesterStatus.m_sTest_Script, strTestScript) != 0)

					if (nProjectOption == DEF_PROJECT_CAMPBELTOWN) // Carpenter 패키지는 Tester에서 Test_Script 정보를 받아서 처리 하기 때문에 체크 시점 변경
					{
						if (strcmp(strAPLScript, strTesterScript) != 0) {
							CParamList AlarmData;
							AlarmData.m_fnAddItemFormat("Tester Script : %s | APL Script : %s", cTesterStatus.m_sTest_Script_Info, strTestScript);
							g_Error.AlarmReport(ERR_TESTER_INFO_TESTER_SCRIPT_MISSMATCHE, AlarmData, NULL);
							break;
						}
						if (strcmp(cTesterStatus.m_sSW_Version, strSWVersion) != 0) {
							CParamList AlarmData;
							AlarmData.m_fnAddItemFormat("Tester Version : %s, APL Version : %s", cTesterStatus.m_sSW_Version, strSWVersion);
							g_Error.AlarmReport(ERR_TESTER_INFO_VERSION_MISSMATCHE, AlarmData, NULL);
							break;
						}
					}			
				}
			}




		}break;

	case eDlgProcCmd_LotInfoSave:
	{
		int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);

		int nRunMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_Run);
		if (nRunMode == eRUN_MODE_ONLINE) 
		{
			if (nProjectOption == DEF_PROJECT_CARPENTERS) // Carpenter 패키지는 LotInfo Save 시 Script Information을 보내준다.
			{
				g_AppleTestIF.m_TesterMgr.SendQueryScriptInfromation();
			}
			else if (nProjectOption == DEF_PROJECT_WATT) {

				CString strTPRev;
				char szTP_Rev[MAX_PATH] = { 0, };
				g_DMCont.m_dmHandlerOpt.GS(SDM11_TP_REV, szTP_Rev, MAX_PATH);
				strTPRev.Format("%s", szTP_Rev);

				CString strTPProg;
				char szTP_Prog[MAX_PATH] = { 0, };
				g_DMCont.m_dmHandlerOpt.GS(SDM11_ProgramName, szTP_Prog, MAX_PATH);
				strTPProg.Format("%s", szTP_Prog);

				CString strRetest;
				char szRetest[MAX_PATH] = { 0, };
				g_DMCont.m_dmHandlerOpt.GS(SDM11_ScheduleNo, szRetest, MAX_PATH);
				strRetest.Format("%s", szRetest);
				
				CString strHostName;
				char szHostName[MAX_PATH] = { 0, };
				g_DMCont.m_dmEQP.GS(SDM0_HANDER_ID, szHostName, sizeof(szHostName));
				strHostName.Format("%s", szHostName);

				CString strDevice;
				char szDevice[MAX_PATH] = { 0, };
				g_DMCont.m_dmHandlerOpt.GS(SDM11_DeviceName, szDevice, sizeof(szDevice));
				strDevice.Format("%s", szDevice);

				// TP Revision
				if (strTPRev.IsEmpty() == TRUE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("APL Info TP_REV Empty");
					g_Error.AlarmReport(ERR_LOT_INFO_EMPTY, AlarmData, NULL);
					break;
				}

				// TP Program
				if (strTPProg.IsEmpty() == TRUE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("APL Info TP_PROG Empty");
					g_Error.AlarmReport(ERR_LOT_INFO_EMPTY, AlarmData, NULL);
					break;
				}

				// Process
				if (strRetest.IsEmpty() == TRUE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("APL Info Retest Empty");
					g_Error.AlarmReport(ERR_LOT_INFO_EMPTY, AlarmData, NULL);
					break;
				}

				// Handler ID
				if (strHostName.IsEmpty() == TRUE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("APL Info Handler ID Empty");
					g_Error.AlarmReport(ERR_LOT_INFO_EMPTY, AlarmData, NULL);
					break;
				}

				// Device
				if (strDevice.IsEmpty() == TRUE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("APL Info Device Empty");
					g_Error.AlarmReport(ERR_LOT_INFO_EMPTY, AlarmData, NULL);
					break;
				}

				g_AppleTestIF.m_TesterMgr.SendQuerySetTestMode(eEqpCtrlMode_Manual);
				Sleep(100);
				g_AppleTestIF.m_TesterMgr.SendQuerySetAPLInfo(eEqpCtrlMode_Manual);
			}
		}
	}break;
	case eDlgProcCmd_VAT:
		{
			if (g_DMCont.m_dmVAT.GB(BDM14_VAT_UI_OPEN_HSITORY) == FALSE)
			{

				CVAT_MainDlg dlg;
				dlg.DoModal();

// 				CVAT_ZAutoTeachingDlg dlg;
// 				dlg.DoModal();
			}

		}break;
	case eDlgProcCmd_TrayView:
		{
			//Stacker 공유 메모리 저장.
			for (int i = 0; i < eMaxStackerCount; i++)
			{
				BOOL bRearExist = g_TaskStacker[i].GetExistTray_RearPos_Sen(DEF_EXIST) == ERR_NO_ERROR ? TRUE : FALSE;
				BOOL bFrontExist = g_TaskStacker[i].GetExistTray_FrontPos_Sen(DEF_EXIST) == ERR_NO_ERROR ? TRUE : FALSE;

				g_DMCont.m_dmStacker.SB(BDM7_SLT_USE_STACKER1_REAR_SEN_EXIST + i, bRearExist);	 //rear
				g_DMCont.m_dmStacker.SB(BDM7_SLT_USE_STACKER1_FRONT_SEN_EXIST + i, bFrontExist); //front
			}

			//Feeder 공유 메모리 저장.
			BOOL bFeederExist = g_TaskTrayFeeder.GetTrayExistSensor(); //B접
			g_DMCont.m_dmFeeder.SB(BDM10_SLT_USE_FEEDER_SEN_EXIST, bFeederExist);

			//Transfer 공유 메모리 저장.
			BOOL bTransferExist = g_TaskTransfer.GetDI(CTaskTransfer::xTRANSFER_TRAY_EXIST_SEN);
			g_DMCont.m_dmTransfer.SB(BDM9_SLT_USE_TRASNFER_SEN_EXIST, !bTransferExist);

			//Transfer Area 공유 메모리 저장
			BOOL bTransferAreaExist = (g_TaskTrayFeeder.GetTrayExist_TransferArea_Sen(eExist) == ERR_NO_ERROR) ? TRUE : FALSE;
			g_DMCont.m_dmTransfer.SB(BDM9_SLT_USE_TRASNFER_AREA_SEN_EXIST, bTransferAreaExist);

		}break;
	case eDlgProcCmd_BatchFileErr:
		{
			char szBatchFilePath[MAX_PATH] = { 0, };
			g_DMCont.m_dmHandlerOpt.GS(SDM11_BatchFile_Path, szBatchFilePath, sizeof(szBatchFilePath));

			char szBatPath[MAX_PATH] = { 0, };

			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Failed to execute Batch File. = %s", szBatchFilePath);
			g_Error.AlarmReport(ERR_TESTER_BATCH_FILE_NOT_EXIST, AlarmData, NULL);
		}break;
	case eDlgProcCmd_BarcodeSetting:
		{
			if (g_DMCont.m_dmEQP.GB(BDM0_OPEN_SLT_UI_DLG_HISTORY) == FALSE)
			{
				DlgBarcodeSetting dlg;
				dlg.DoModal();
			}
		}break;
    }


    return 0L;
}

LRESULT CMainFrame::OnGuiEventClose(WPARAM wParma, LPARAM lParam)
{
    Sleep( 1000 );
    //::AfxGetMainWnd()->SendMessage(WM_COMMAND,ID_APP_EXIT);
    SendMessage (WM_CLOSE, 0,0);

    return 0L;
}

void CMainFrame::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CFrameWnd::OnWindowPosChanged(lpwndpos);

	// 20161008_mjna_imsi >>
	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
	// 20161008_mjna_imsi <<
}

LRESULT CMainFrame::OnMultiOriginEvent(WPARAM wParma, LPARAM lParam)
{
	switch (wParma)
	{
		case eMultiOriginExecuting:
		{
			if (g_MulitOrg.GetSeqCmdStatus(eEqpCtrlMode_Manual) != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				AfxMessageBox(" manual command is Excuting!! please wait working finish!!!!");
				break;
			}

			int nErr = g_TaskSystemCtrl.CheckEqpConditionForMultiOrg();
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, &g_TaskSystemCtrl);
				break;
			}

			int nInterlockErr = g_TaskSystemCtrl.CheckManualable();
			if (nInterlockErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				g_Error.AlarmReport(nInterlockErr, AlarmData, NULL);
				break;
			}

			if (m_bManualCmdExcute || g_DMCont.m_dmHandlerOpt.GN(NDM11_MULTIORIGIN_HISTORY) == eMulti_Origin_Start) {
				AfxMessageBox(" manual command is Excuting!! please wait working finish!!!!");
				break;
			}

			int nRet = AfxMessageBox("Do you want Multi Origin Axis All Start?????", MB_YESNO);
			if (nRet == IDYES) {
				g_DMCont.m_dmHandlerOpt.SN(NDM11_MULTIORIGIN_HISTORY, eMulti_Origin_Start);
				CMultiOrgCmd_Axis* pSeqCmd = new CMultiOrgCmd_Axis;
				pSeqCmd->m_nCmdStage = eMultiOrgType_AxisAll;
				g_TaskSystemCtrl.SendCommand(g_MulitOrg, pSeqCmd, eEqpCtrlMode_Manual);

// 				if (m_pDlgHomeCheckStatus->IsWindowVisible() == FALSE) {
// 					m_pDlgHomeCheckStatus->ShowWindow(SW_SHOW);
// 				}
			}

			double dForcePerPin = g_DMCont.m_dmHandlerOpt.GD(DDM11_DvcContactForce_GF);
			if (dForcePerPin <= 0.0) {
				dForcePerPin = 0.0;
			}
			for (int i = 0; i < eMaxPressUnitCount; i++)
			{
				g_TaskRegulator[i].do_SetForcePerPin(dForcePerPin);
			}
			//g_DMCont.m_dmHandlerOpt.SB(BDM11_MULTIORIGIN_HISTORY, FALSE);

		}break;

		case eMultiOriginStop:
		{
			CBaseTaskManager::SendProcessCommand(base_seq_cmd::eProcCmd_Pause, eEqpCtrlMode_Manual, NULL);

			if (m_pDlgHomeCheckStatus != NULL)
			{
				if (m_pDlgHomeCheckStatus->IsWindowVisible() == TRUE)
					m_pDlgHomeCheckStatus->ShowWindow(SW_HIDE);
			}
			
			g_DMCont.m_dmHandlerOpt.SN(NDM11_MULTIORIGIN_HISTORY, eMulti_Origin_End);

		}break;
	}

	return 0L;
}



void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	BOOL bBusySts = FALSE;
	BOOL bAlarmSts = FALSE;

	int nCntBusy = 0;
	int nCntAlarm = 0;

	if (g_TaskTransfer.GetSeqCmdStatus(eEqpCtrlMode_Manual) != CBaseSeqCmd::eSeqCmdStatus_Ready) {
		bBusySts = TRUE;
	}
	else if (g_TaskTransfer.GetSeqCmdStatus(eEqpCtrlMode_Manual) == CBaseSeqCmd::eSeqCmdStatus_Alarm) {
		bAlarmSts = TRUE;
	}


	if (g_TaskTrayFeeder.GetSeqCmdStatus(eEqpCtrlMode_Manual) != CBaseSeqCmd::eSeqCmdStatus_Ready) {
		bBusySts = TRUE;
	}
	else if (g_TaskTrayFeeder.GetSeqCmdStatus(eEqpCtrlMode_Manual) == CBaseSeqCmd::eSeqCmdStatus_Alarm) {
		bAlarmSts = TRUE;
	}

	CTaskTrayBasePP* pTaskTrayPP[] = { &g_TaskTrayLoadPP, &g_TaskTrayUnloadPP };
	for (int nIdx = 0; nIdx < eMaxTrayPPCount; nIdx++) {

		if (pTaskTrayPP[nIdx]->GetSeqCmdStatus(eEqpCtrlMode_Manual) != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			bBusySts = TRUE;
		}
		else if (pTaskTrayPP[nIdx]->GetSeqCmdStatus(eEqpCtrlMode_Manual) == CBaseSeqCmd::eSeqCmdStatus_Alarm) {
			bAlarmSts = TRUE;
		}
	}

	for (int nIdx = 0; nIdx < eMaxStackerCount; nIdx++) {
		if (g_TaskStacker[nIdx].GetSeqCmdStatus(eEqpCtrlMode_Manual) != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			nCntBusy++;
		}
		else if (g_TaskStacker[nIdx].GetSeqCmdStatus(eEqpCtrlMode_Manual) == CBaseSeqCmd::eSeqCmdStatus_Alarm) {
			nCntAlarm++;
		}
	}

	for (int nIdx = 0; nIdx < eMaxPressUnitCount; nIdx++) {
		if (g_TaskPressUnit[nIdx].GetSeqCmdStatus(eEqpCtrlMode_Manual) != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			nCntBusy++;
		}
		else if (g_TaskPressUnit[nIdx].GetSeqCmdStatus(eEqpCtrlMode_Manual) == CBaseSeqCmd::eSeqCmdStatus_Alarm) {
			nCntAlarm++;
		}
	}

	for (int nIdx = 0; nIdx < eMaxTestPPCount; nIdx++) {
		if (g_TaskTestPP[nIdx].GetSeqCmdStatus(eEqpCtrlMode_Manual) != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			nCntBusy++;
		}
		else if (g_TaskTestPP[nIdx].GetSeqCmdStatus(eEqpCtrlMode_Manual) == CBaseSeqCmd::eSeqCmdStatus_Alarm) {
			nCntAlarm++;
		}
	}

	for (int nIdx = 0; nIdx < eMaxLoadTblCount; nIdx++) {
		if (g_TaskLoadTbl[nIdx].GetSeqCmdStatus(eEqpCtrlMode_Manual) != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			nCntBusy++;
		}
		else if (g_TaskLoadTbl[nIdx].GetSeqCmdStatus(eEqpCtrlMode_Manual) == CBaseSeqCmd::eSeqCmdStatus_Alarm) {
			nCntAlarm++;
		}
	}
	
	if (g_MulitOrg.GetSeqCmdStatus(eEqpCtrlMode_Manual) != CBaseSeqCmd::eSeqCmdStatus_Ready) {
		nCntBusy++;
	}
	else if (g_MulitOrg.GetSeqCmdStatus(eEqpCtrlMode_Manual) == CBaseSeqCmd::eSeqCmdStatus_Alarm) {
		nCntAlarm++;
	}

	if (nCntBusy > 0) {
		bBusySts = TRUE;
	}
	if (nCntAlarm > 0) {
		bAlarmSts = TRUE;
	}

	CString strCurPos;

	if (nIDEvent == 0)
	{
		KillTimer(0);

	
		if (bBusySts) {
			m_bManualCmdExcute = TRUE;
		}
		else {
			m_bManualCmdExcute = FALSE;
		}

		SetTimer(0, DEF_SENSOR_DISP_TM, NULL);

	}

	CWnd::OnTimer(nIDEvent);
}
