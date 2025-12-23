// DlgCommand_PressUnit.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgCommand_PressUnit.h"
#include "afxdialogex.h"


// CDlgCommand_PressUnit 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgCommand_PressUnit, CDialogEx)

CDlgCommand_PressUnit::CDlgCommand_PressUnit(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCommand_PressUnit::IDD, pParent)
{
	m_nSelectedSeqCmdId = 0;
	m_nManuOper = 0;
}

CDlgCommand_PressUnit::~CDlgCommand_PressUnit()
{
	m_cBmpInitial.DeleteObject();
	m_cBmpMovePos.DeleteObject();
	m_cBmpAutoHeightCal.DeleteObject();
	m_cBmpSocketCleaning.DeleteObject();
	m_cBmpClose.DeleteObject();
}

void CDlgCommand_PressUnit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	// initial
	DDX_Control(pDX, IDC_BUTTON_PRESS_UNIT_CMD_INIT, m_btnInitial);
	// move pos
	DDX_Control(pDX, IDC_BUTTON_PRESS_UNIT_CMD_MOVE_POS, m_btnMovePos);
	DDX_Control(pDX, IDC_BUTTON_PRESS_UNIT_CMD_AUTOHEIGHTCAL, m_btnAutoHeightCal);
	DDX_Control(pDX, IDC_BUTTON_PRESS_UNIT_CMD_SOCKETCLEAN, m_btnSocketCleaning);
	// close
	DDX_Control(pDX, IDCANCEL, m_btnClose);
}


BEGIN_MESSAGE_MAP(CDlgCommand_PressUnit, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_PRESS_UNIT_CMD_MOVE_POS, &CDlgCommand_PressUnit::OnBnClickedButtonPressUnitCmdMovePos)
	ON_BN_CLICKED(IDC_BUTTON_PRESS_UNIT_CMD_INIT, &CDlgCommand_PressUnit::OnBnClickedButtonPressUnitCmdInit)
	ON_BN_CLICKED(IDC_BUTTON_PRESS_UNIT_CMD_SOCKETCLEAN, &CDlgCommand_PressUnit::OnBnClickedButtonPressUnitCmdSocketclean)
	ON_BN_CLICKED(IDC_BUTTON_PRESS_UNIT_CMD_AUTOHEIGHTCAL, &CDlgCommand_PressUnit::OnBnClickedButtonPressUnitCmdAutoheightcal)
	ON_BN_CLICKED( IDCANCEL, &CDlgCommand_PressUnit::OnBnClickedCancel )
END_MESSAGE_MAP()


// CDlgCommand_PressUnit 메시지 처리기입니다.


BOOL CDlgCommand_PressUnit::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CDlgCommand_PressUnit::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// initial
	m_cBmpInitial.LoadBitmap(IDB_CMD_INITIAL);
	m_btnInitial.SetBitmap(m_cBmpInitial);
	// move pos
	m_cBmpMovePos.LoadBitmap(IDB_CMD_MOVEPOS);
	m_btnMovePos.SetBitmap(m_cBmpMovePos);

	m_cBmpAutoHeightCal.LoadBitmap(IDB_CMD_AUTOHEIGHTCAL);
	m_btnAutoHeightCal.SetBitmap(m_cBmpAutoHeightCal);

	m_cBmpSocketCleaning.LoadBitmap(IDB_CMD_SOCKETCLEANING);
	m_btnSocketCleaning.SetBitmap(m_cBmpSocketCleaning);
	// close
	m_cBmpClose.LoadBitmap(IDB_CMD_CLOSE);
	m_btnClose.SetBitmap(m_cBmpClose);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgCommand_PressUnit::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CDlgCommand_PressUnit::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}


void CDlgCommand_PressUnit::OnBnClickedButtonPressUnitCmdMovePos()
{
	m_nSelectedSeqCmdId = eSeqCmd_PressUnit_MovePos;
	CDialogEx::OnOK();
}


void CDlgCommand_PressUnit::OnBnClickedButtonPressUnitCmdInit()
{
	m_nSelectedSeqCmdId = eSeqCmd_PressUnit_Initial;
	CDialogEx::OnOK();
}

void CDlgCommand_PressUnit::OnBnClickedButtonPressUnitCmdSocketclean()
{
	m_nSelectedSeqCmdId = eSeqCmd_PressUnit_SocketCleanContact;
	CDialogEx::OnOK();
}


void CDlgCommand_PressUnit::OnBnClickedButtonPressUnitCmdAutoheightcal()
{
	m_nSelectedSeqCmdId = eSeqCmd_PressUnit_AutoHeightCal;
	CDialogEx::OnOK();
}


void CDlgCommand_PressUnit::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}
