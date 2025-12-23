// TrayViewStacker.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "TrayViewStacker.h"
#include "afxdialogex.h"

#define STATUS_ON		(COLOR_GREEN)
#define STATUS_OFF		(COLOR_LLIGHTGRAY)

// CTrayViewStacker 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTrayViewStacker, CDialogEx)

CTrayViewStacker::CTrayViewStacker(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_TRAYVIEW_STACKER, pParent)
{
	int nFrontConveyorExistCtrlId[eMaxStackerCount] = {
		IDC_FRONT_STACKER1_SEN,IDC_FRONT_STACKER2_SEN,IDC_FRONT_STACKER3_SEN,
		IDC_FRONT_STACKER4_SEN,IDC_FRONT_STACKER5_SEN,IDC_FRONT_STACKER6_SEN,
		IDC_FRONT_STACKER7_SEN
	};
	memcpy(m_nFrontConveyorExistCtrlId, nFrontConveyorExistCtrlId, sizeof(int) * eMaxStackerCount);

	int nFrontConveyorDataCtrlId[eMaxStackerCount] = {
		IDC_FRONT_STACKER1_DATA,IDC_FRONT_STACKER2_DATA,IDC_FRONT_STACKER3_DATA,
		IDC_FRONT_STACKER4_DATA,IDC_FRONT_STACKER5_DATA,IDC_FRONT_STACKER6_DATA,
		IDC_FRONT_STACKER7_DATA
	};
	memcpy(m_nFrontConveyorDataCtrlId, nFrontConveyorDataCtrlId, sizeof(int) * eMaxStackerCount);

	int nRearConveyorExistCtrlId[eMaxStackerCount] = {
		IDC_REAR_STACKER1_SEN,IDC_REAR_STACKER2_SEN,IDC_REAR_STACKER3_SEN,
		IDC_REAR_STACKER4_SEN,IDC_REAR_STACKER5_SEN,IDC_REAR_STACKER6_SEN,
		IDC_REAR_STACKER7_SEN
	};
	memcpy(m_nRearConveyorExistCtrlId, nRearConveyorExistCtrlId, sizeof(int) * eMaxStackerCount);

	int nRearConveyorDataCtrlId[eMaxStackerCount] = {
		IDC_REAR_STACKER1_DATA,IDC_REAR_STACKER2_DATA,IDC_REAR_STACKER3_DATA,
		IDC_REAR_STACKER4_DATA,IDC_REAR_STACKER5_DATA,IDC_REAR_STACKER6_DATA,
		IDC_REAR_STACKER7_DATA
	};
	memcpy(m_nRearConveyorDataCtrlId, nRearConveyorDataCtrlId, sizeof(int) * eMaxStackerCount);	
}

CTrayViewStacker::~CTrayViewStacker()
{
}

void CTrayViewStacker::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	for (int i = 0; i < eMaxStackerCount; i++) {
		DDX_Control(pDX, m_nFrontConveyorExistCtrlId[i], m_editFrontConveyorExist[i]);
		DDX_Control(pDX, m_nFrontConveyorDataCtrlId[i], m_editFrontConveyorData[i]);
		DDX_Control(pDX, m_nRearConveyorExistCtrlId[i], m_editRearConveyorExist[i]);
		DDX_Control(pDX, m_nRearConveyorDataCtrlId[i], m_editRearConveyorData[i]);
	}

	DDX_Control(pDX, IDC_FEEDER_SEN, m_editFeederExist);
	DDX_Control(pDX, IDC_FEEDER_DATA, m_editFeederData);
	DDX_Control(pDX, IDC_TRANSFER_SEN, m_editTransferExist);
	DDX_Control(pDX, IDC_TRANSFER_DATA, m_editTransferData);
	DDX_Control(pDX, IDC_TRANSFER_AREA_SEN, m_editTransferAreaExist);
	DDX_Control(pDX, IDC_TRANSFER_AREA_DATA, m_editTransferAreaData);
}


BEGIN_MESSAGE_MAP(CTrayViewStacker, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CTrayViewStacker 메시지 처리기입니다.

BOOL CTrayViewStacker::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CTrayViewStacker::OnDestroy()
{
	KillTimer(0);

	CDialogEx::OnDestroy();
}

BOOL CTrayViewStacker::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	SendMcComm();

	/////////////conveyor#1 ~ #7
	for (int i = 0; i < eMaxStackerCount; i++)
	{
		GetDlgItem(m_nFrontConveyorExistCtrlId[i])->SetWindowText(_T("sensor"));
		GetDlgItem(m_nFrontConveyorDataCtrlId[i])->SetWindowText(_T("data"));
		GetDlgItem(m_nRearConveyorExistCtrlId[i])->SetWindowText(_T("sensor"));
		GetDlgItem(m_nRearConveyorDataCtrlId[i])->SetWindowText(_T("data"));

		//Font & Text Color
		m_editFrontConveyorExist[i].SetFont("Fixedsys", 17);
		m_editFrontConveyorExist[i].SetTextColor(COLOR_BBLACK);

		m_editFrontConveyorData[i].SetFont("Fixedsys", 17);
		m_editFrontConveyorData[i].SetTextColor(COLOR_BBLACK);

		m_editRearConveyorExist[i].SetFont("Fixedsys", 17);
		m_editRearConveyorExist[i].SetTextColor(COLOR_BBLACK);

		m_editRearConveyorData[i].SetFont("Fixedsys", 17);
		m_editRearConveyorData[i].SetTextColor(COLOR_BBLACK);

		//BackGround Color
		m_editFrontConveyorExist[i].SetBackColor(STATUS_OFF);
		m_editFrontConveyorData[i].SetBackColor(STATUS_OFF);
		m_editRearConveyorExist[i].SetBackColor(STATUS_OFF);
		m_editRearConveyorData[i].SetBackColor(STATUS_OFF);
	}

	///////////////Feeder & Transfer
	GetDlgItem(IDC_FEEDER_SEN)->SetWindowText(_T("sensor"));
	GetDlgItem(IDC_FEEDER_DATA)->SetWindowText(_T("data"));
	GetDlgItem(IDC_TRANSFER_SEN)->SetWindowText(_T("sensor"));
	GetDlgItem(IDC_TRANSFER_DATA)->SetWindowText(_T("data"));
	GetDlgItem(IDC_TRANSFER_AREA_SEN)->SetWindowText(_T("sensor"));
	GetDlgItem(IDC_TRANSFER_AREA_DATA)->SetWindowText(_T("data"));

	//Font & Text Color
	m_editFeederExist.SetFont("Fixedsys", 17);
	m_editFeederExist.SetTextColor(COLOR_BBLACK);

	m_editFeederData.SetFont("Fixedsys", 17);
	m_editFeederData.SetTextColor(COLOR_BBLACK);

	m_editTransferExist.SetFont("Fixedsys", 17);
	m_editTransferExist.SetTextColor(COLOR_BBLACK);

	m_editTransferData.SetFont("Fixedsys", 17);
	m_editTransferData.SetTextColor(COLOR_BBLACK);

	m_editTransferAreaExist.SetFont("Fixedsys", 17);
	m_editTransferAreaExist.SetTextColor(COLOR_BBLACK);

	m_editTransferAreaData.SetFont("Fixedsys", 17);
	m_editTransferAreaData.SetTextColor(COLOR_BBLACK);

	//BackGround Color
	m_editFeederExist.SetBackColor(STATUS_OFF);
	m_editFeederData.SetBackColor(STATUS_OFF);
	m_editTransferExist.SetBackColor(STATUS_OFF);
	m_editTransferData.SetBackColor(STATUS_OFF);
	m_editTransferAreaExist.SetBackColor(STATUS_OFF);
	m_editTransferAreaData.SetBackColor(STATUS_OFF);

	UpdateStatus();

	SetTimer(0, 500, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CTrayViewStacker::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case 0:
		{
		KillTimer(0);
		UpdateStatus();
		SetTimer(0, 500, NULL);
		}break;
	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CTrayViewStacker::UpdateStatus()
{
	SendMcComm();
	COLORREF color = STATUS_OFF;

	for (int i = 0; i < eMaxStackerCount; i++)
	{
		
		//========================Sensor=========================================================
		//Conveyor Front
		color = g_DMCont.m_dmStacker.GB(BDM7_SLT_USE_STACKER1_FRONT_SEN_EXIST + i) == TRUE ? STATUS_ON : STATUS_OFF;
		if (beforeStacker_F_Sen_color[i] != color)
		{
			beforeStacker_F_Sen_color[i] = color;
			m_editFrontConveyorExist[i].SetBackColor(color);
		}
	
		
		//Conveyor Rear
		color = g_DMCont.m_dmStacker.GB(BDM7_SLT_USE_STACKER1_REAR_SEN_EXIST + i) == TRUE ? STATUS_ON : STATUS_OFF;
		if (beforeStacker_R_Sen_color[i] != color)
		{
			beforeStacker_R_Sen_color[i] = color;
			m_editRearConveyorExist[i].SetBackColor(color);
		}


		//========================================================================================

		//========================Data============================================================
		//Conveyor Front
		color = g_DvcDataMem.GetTrayExist(eDeviceStage_STACKER_BTM_1 + i) == TRUE ? STATUS_ON : STATUS_OFF;
		if (beforeStacker_F_Data_color[i] != color)
		{
			beforeStacker_F_Data_color[i] = color;
			m_editFrontConveyorData[i].SetBackColor(color);
		}

		//Conveyor Rear
		color = g_DvcDataMem.GetTrayExist(eDeviceStage_STACKER_CONV_1 + i) == TRUE ? STATUS_ON : STATUS_OFF;
		if (beforeStacker_R_Data_color[i] != color)
		{
			beforeStacker_R_Data_color[i] = color;
			m_editRearConveyorData[i].SetBackColor(color);
		}



		//========================================================================================

	}

	//========================Sensor=========================================================
	//Feeder
	color = g_DMCont.m_dmFeeder.GB(BDM10_SLT_USE_FEEDER_SEN_EXIST) == TRUE ? STATUS_ON : STATUS_OFF;
	if (beforeViewStacker_Sen_color[eViewTrayFeeder] != color)
	{
		beforeViewStacker_Sen_color[eViewTrayFeeder] = color;
		m_editFeederExist.SetBackColor(color);
	}
	
	//Transfer
	color = g_DMCont.m_dmCTrayXfer.GB(BDM9_SLT_USE_TRASNFER_SEN_EXIST) == TRUE ? STATUS_ON : STATUS_OFF;
	if (beforeViewStacker_Sen_color[eViewTransfer] != color)
	{
		beforeViewStacker_Sen_color[eViewTransfer] = color;
		m_editTransferExist.SetBackColor(color);
	}

	//Transfer Area
	color = g_DMCont.m_dmCTrayXfer.GB(BDM9_SLT_USE_TRASNFER_AREA_SEN_EXIST) == TRUE ? STATUS_ON : STATUS_OFF;
	if (beforeViewStacker_Sen_color[eViewTransferArea] != color)
	{
		beforeViewStacker_Sen_color[eViewTransferArea] = color;
		m_editTransferAreaExist.SetBackColor(color);
	}
// 
// 	//========================Data============================================================
// 	//Feeder
	color = g_DvcDataMem.GetTrayExist(eDeviceStage_TRAY_FEEDER) == TRUE ? STATUS_ON : STATUS_OFF;
	if (beforeViewStacker_Data_color[eViewTrayFeeder] != color)
	{
		beforeViewStacker_Data_color[eViewTrayFeeder] = color;
		m_editFeederData.SetBackColor(color);
	}

	//Transfer
	color = g_DvcDataMem.GetTrayExist(eDeviceStage_Transfer) == TRUE ? STATUS_ON : STATUS_OFF;
	if (beforeViewStacker_Data_color[eViewTransfer] != color)
	{
		beforeViewStacker_Data_color[eViewTransfer] = color;
		m_editTransferData.SetBackColor(color);
	}

	//Transfer Area
	color = g_DvcDataMem.GetTrayExist(eDeviceStage_STACKER_CONV_3_TRANSFER_AREA) == TRUE ? STATUS_ON : STATUS_OFF;
	if (beforeViewStacker_Data_color[eViewTransferArea] != color)
	{
		beforeViewStacker_Data_color[eViewTransferArea] = color;
		m_editTransferAreaData.SetBackColor(color);
	}
}

void CTrayViewStacker::SendMcComm()
{
	char szProcName[32] = { 0, };
#ifdef _DEBUG
	sprintf_s(szProcName, sizeof(szProcName), "SLT_MC_D.exe");
#else
	sprintf_s(szProcName, sizeof(szProcName), "SLT_MC.exe");
#endif
	HWND hWnd = ::FindWindow(NULL, szProcName);
	BOOL bMCConnect = g_McComm.IsConnected();

	if (bMCConnect == DEF_ON && hWnd)
	{
		g_McComm.OnSendDlgShow(eDlgProcCmd_TrayView);
	}
}