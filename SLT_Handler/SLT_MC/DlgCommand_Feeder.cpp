// DlgCommand_Feeder.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgCommand_Feeder.h"
#include "afxdialogex.h"


// CDlgCommand_Feeder 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgCommand_Feeder, CDialogEx)

CDlgCommand_Feeder::CDlgCommand_Feeder(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CMD_FEEDER, pParent)
{
	m_nSelectedSeqCmdId = 0;
}

CDlgCommand_Feeder::~CDlgCommand_Feeder()
{
	m_cBmpInitial.DeleteObject();
	m_cBmpMovePos.DeleteObject();
	m_cBmpClose.DeleteObject();
}

void CDlgCommand_Feeder::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	// initial
	DDX_Control(pDX, IDC_BUTTON_FEEDER_CMD_INIT, m_btnInitial);
	// move pos
	DDX_Control(pDX, IDC_BUTTON_FEEDER_CMD_MOVE_POS, m_btnMovePos);
	// pick place move
	DDX_Control(pDX, IDC_BUTTON_FEEDER_CMD_MOVE_PICK_PLACE, m_btnPickPlaceMove);
	// close
	DDX_Control(pDX, IDCANCEL, m_btnClose);
}


BEGIN_MESSAGE_MAP(CDlgCommand_Feeder, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_FEEDER_CMD_INIT, &CDlgCommand_Feeder::OnBnClickedButtonFeederCmdInit)
	ON_BN_CLICKED(IDC_BUTTON_FEEDER_CMD_MOVE_POS, &CDlgCommand_Feeder::OnBnClickedButtonFeederCmdMovePos)
	ON_BN_CLICKED(IDC_BUTTON_FEEDER_CMD_MOVE_PICK_PLACE, &CDlgCommand_Feeder::OnBnClickedButtonFeederCmdMovePickPlace)
END_MESSAGE_MAP()


// CDlgCommand_Feeder 메시지 처리기입니다.

BOOL CDlgCommand_Feeder::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::PreTranslateMessage(pMsg);
}

// BOOL CDlgCommand_Feeder::OnInitDialog()
// {
// 	CDialogEx::OnInitDialog();
// 
// 	// initial
// 	m_cBmpInitial.LoadBitmap(IDB_CMD_INITIAL);
// 	m_btnInitial.SetBitmap(m_cBmpInitial);
// 	// move pos
// 	m_cBmpMovePos.LoadBitmap(IDB_CMD_MOVEPOS);
// 	m_btnMovePos.SetBitmap(m_cBmpMovePos);
// 	// close
// 	m_cBmpClose.LoadBitmap(IDB_CMD_CLOSE);
// 	m_btnClose.SetBitmap(m_cBmpClose);
// 
// 	return TRUE;  // return TRUE unless you set the focus to a control
// 				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
// }

void CDlgCommand_Feeder::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CDlgCommand_Feeder::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}

void CDlgCommand_Feeder::OnBnClickedButtonFeederCmdInit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nSelectedSeqCmdId = eSeqCmd_Feeder_Initial;
	CDialogEx::OnOK();
}


void CDlgCommand_Feeder::OnBnClickedButtonFeederCmdMovePos()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nSelectedSeqCmdId = eSeqCmd_Feeder_MovePos;
	CDialogEx::OnOK();
}


void CDlgCommand_Feeder::OnBnClickedButtonFeederCmdMovePickPlace()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nSelectedSeqCmdId = eSeqCmd_Feeder_MovePickPlace;
	CDialogEx::OnOK();
}


BOOL CDlgCommand_Feeder::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	// initial
	m_cBmpInitial.LoadBitmap(IDB_CMD_INITIAL);
	m_btnInitial.SetBitmap(m_cBmpInitial);
	// move pos
	m_cBmpMovePos.LoadBitmap(IDB_CMD_MOVEPOS);
	m_btnMovePos.SetBitmap(m_cBmpMovePos);
	// pick place move
	m_cBmpPickPlaceMove.LoadBitmap(IDB_PICKPLACE_MOVE);
	m_btnPickPlaceMove.SetBitmap(m_cBmpPickPlaceMove);
	// close
	m_cBmpClose.LoadBitmap(IDB_CMD_CLOSE);
	m_btnClose.SetBitmap(m_cBmpClose);



	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
