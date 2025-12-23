// DlgCommand_Stacker.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgCommand_Stacker.h"
#include "afxdialogex.h"
#include "DlgCommand_Param.h"

// CDlgCommand_Stacker 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgCommand_Stacker, CDialogEx)

CDlgCommand_Stacker::CDlgCommand_Stacker(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCommand_Stacker::IDD, pParent)
{
	m_nSelectedSeqCmdId = eSeqCmd_None;
	m_nManuOper = 0;
	m_nStackerIdx = 0;
	ZeroMemory(m_nSelectedSeqCmdParam, sizeof(m_nSelectedSeqCmdParam));
}

CDlgCommand_Stacker::~CDlgCommand_Stacker()
{
	m_cBmpInitial.DeleteObject();
	m_cBmpLoad.DeleteObject();
	m_cBmpTrayDown.DeleteObject();
	m_cBmpTrayUp.DeleteObject();
	m_cBmpUnload.DeleteObject();
	m_cBmpTrayOut.DeleteObject();
	m_cBmpVibration.DeleteObject();
	m_cBmpClose.DeleteObject();
}

void CDlgCommand_Stacker::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	// initial
	DDX_Control(pDX, IDC_BUTTON_STACKER_CMD_INIT, m_btnInitial);
	DDX_Control(pDX, IDC_BUTTON_STACKER_CMD_LOAD, m_btnLoad);
	DDX_Control(pDX, IDC_BUTTON_STACKER_CMD_TRAYDOWN, m_btnTrayDown);
	DDX_Control(pDX, IDC_BUTTON_STACKER_CMD_TRAYUP, m_btnTrayUp);
	DDX_Control(pDX, IDC_BUTTON_STACKER_CMD_UNLOAD, m_btnUnload);
	DDX_Control(pDX, IDC_BUTTON_STACKER_CMD_TRAY_OUT, m_btnTrayOut);
	DDX_Control(pDX, IDC_BUTTON_STACKER_CMD_VIBRATION, m_btnVibration);
	// close
	DDX_Control(pDX, IDCANCEL, m_btnClose);
}


BEGIN_MESSAGE_MAP(CDlgCommand_Stacker, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_STACKER_CMD_INIT, &CDlgCommand_Stacker::OnBnClickedButtonStackerCmdInit)
	ON_BN_CLICKED(IDC_BUTTON_STACKER_CMD_LOAD, &CDlgCommand_Stacker::OnBnClickedButtonStackerCmdLoad)
	ON_BN_CLICKED(IDC_BUTTON_STACKER_CMD_TRAYDOWN, &CDlgCommand_Stacker::OnBnClickedButtonStackerCmdTraydown)
	ON_BN_CLICKED(IDC_BUTTON_STACKER_CMD_TRAYUP, &CDlgCommand_Stacker::OnBnClickedButtonStackerCmdTrayup)
	ON_BN_CLICKED(IDC_BUTTON_STACKER_CMD_UNLOAD, &CDlgCommand_Stacker::OnBnClickedButtonStackerCmdUnload)
	ON_BN_CLICKED(IDC_BUTTON_STACKER_CMD_VIBRATION, &CDlgCommand_Stacker::OnBnClickedButtonStackerCmdVibration)
	ON_BN_CLICKED(IDC_BUTTON_STACKER_CMD_TRAY_OUT, &CDlgCommand_Stacker::OnBnClickedButtonStackerCmdTrayOut)
END_MESSAGE_MAP()


// CDlgCommand_Stacker 메시지 처리기입니다.


BOOL CDlgCommand_Stacker::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CDlgCommand_Stacker::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// initial
	m_cBmpInitial.LoadBitmap(IDB_CMD_INITIAL);
	m_btnInitial.SetBitmap(m_cBmpInitial);

	m_cBmpLoad.LoadBitmap(IDB_CMD_LOAD);
	m_btnLoad.SetBitmap(m_cBmpLoad);

	m_cBmpTrayDown.LoadBitmap(IDB_CMD_TRAYDOWN);
	m_btnTrayDown.SetBitmap(m_cBmpTrayDown);

	m_cBmpTrayUp.LoadBitmap(IDB_CMD_TRAYUP);
	m_btnTrayUp.SetBitmap(m_cBmpTrayUp);

	m_cBmpUnload.LoadBitmap(IDB_CMD_UNLOAD);
	m_btnUnload.SetBitmap(m_cBmpUnload);

	m_cBmpTrayOut.LoadBitmap(IDB_CMD_TRAY_OUT);
	m_btnTrayOut.SetBitmap(m_cBmpTrayOut);


	m_cBmpVibration.LoadBitmap(IDB_CMD_VIBRATION);
	m_btnVibration.SetBitmap(m_cBmpVibration);

	// close
	m_cBmpClose.LoadBitmap( IDB_CMD_CLOSE );
	m_btnClose.SetBitmap(m_cBmpClose);

	if( m_nStackerIdx == eSTACKER_01 || m_nStackerIdx == eSTACKER_02 || m_nStackerIdx == eSTACKER_03 )
	{
		GetDlgItem( IDC_BUTTON_STACKER_CMD_VIBRATION )->ShowWindow( SW_HIDE );
	}

	if (m_nStackerIdx != eSTACKER_03)
	{
		GetDlgItem(IDC_BUTTON_STACKER_CMD_TRAY_OUT)->ShowWindow(SW_HIDE);
	}



	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgCommand_Stacker::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CDlgCommand_Stacker::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}


void CDlgCommand_Stacker::OnBnClickedButtonStackerCmdInit()
{
	m_nSelectedSeqCmdId = eSeqCmd_Stacker_Initial;
	CDialogEx::OnOK();
}


int CDlgCommand_Stacker::GetSelectedSeqCmdId()
{
	return m_nSelectedSeqCmdId;
}


void CDlgCommand_Stacker::OnBnClickedButtonStackerCmdLoad()
{
	m_nSelectedSeqCmdId = eSeqCmd_Stacker_Load;
	CDialogEx::OnOK();
}


void CDlgCommand_Stacker::OnBnClickedButtonStackerCmdTraydown()
{
	m_nSelectedSeqCmdId = eSeqCmd_Stacker_TrayDn;
	CDialogEx::OnOK();
}


void CDlgCommand_Stacker::OnBnClickedButtonStackerCmdTrayup()
{
	m_nSelectedSeqCmdId = eSeqCmd_Stacker_TrayUp;
	CDialogEx::OnOK();
}


void CDlgCommand_Stacker::OnBnClickedButtonStackerCmdUnload()
{
	m_nSelectedSeqCmdId = eSeqCmd_Stacker_Unload;
	CDialogEx::OnOK();
}


void CDlgCommand_Stacker::OnBnClickedButtonStackerCmdVibration()
{
	m_nSelectedSeqCmdId = eSeqCmd_Stacker_Vibration;
	CDialogEx::OnOK();
}


void CDlgCommand_Stacker::OnBnClickedButtonStackerCmdTrayOut()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nSelectedSeqCmdId = eSeqCmd_Stacker_TrayOut;
	CDialogEx::OnOK();
}
