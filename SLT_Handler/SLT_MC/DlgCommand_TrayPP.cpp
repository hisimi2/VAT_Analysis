// DlgCommand_TrayPP.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgCommand_TrayPP.h"
#include "afxdialogex.h"


// CDlgCommand_TrayPP 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgCommand_TrayPP, CDialogEx)

CDlgCommand_TrayPP::CDlgCommand_TrayPP(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCommand_TrayPP::IDD, pParent)
{
	m_nSelectedSeqCmdId = 0;
	m_nManuOper = 0;
}

CDlgCommand_TrayPP::~CDlgCommand_TrayPP()
{
	m_cBmpInitial.DeleteObject();
	m_cBmpMovePos.DeleteObject();
	m_cBmpPickPlace.DeleteObject();
	m_cBmpPickPlaceMove.DeleteObject();
	m_cBmpEmptyCheck.DeleteObject();
	m_cBmpClose.DeleteObject();
}

void CDlgCommand_TrayPP::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	// initial
	DDX_Control(pDX, IDC_BUTTON_TRAY_PP_CMD_INIT, m_btnInitial);
	// move pos
	DDX_Control(pDX, IDC_BUTTON_TRAY_PP_CMD_MOVE_POS, m_btnMovePos);
	// pick place
	DDX_Control(pDX, IDC_BUTTON_TRAY_PP_CMD_PICK_PLACE, m_btnPickPlace);
	// pick place
	DDX_Control(pDX, IDC_BUTTON_TRAY_PP_CMD_AUOTO_HEIGHT_CAL, m_btnPickerHeightCal);
	// pick place move
	DDX_Control(pDX, IDC_BUTTON_TRAY_PP_CMD_MOVE_PICK_PLACE, m_btnPickPlaceMove);
	// empty check
	DDX_Control(pDX, IDC_BUTTON_TRAY_PP_CMD_EMPTY_CHECK, m_btnEmptyCheck);
	// close
	DDX_Control(pDX, IDCANCEL, m_btnClose);
}


BEGIN_MESSAGE_MAP(CDlgCommand_TrayPP, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_TRAY_PP_CMD_INIT, &CDlgCommand_TrayPP::OnBnClickedButtonTrayPpCmdInit)
	ON_BN_CLICKED(IDC_BUTTON_TRAY_PP_CMD_MOVE_POS, &CDlgCommand_TrayPP::OnBnClickedButtonTrayPpCmdMovePos)
	ON_BN_CLICKED(IDC_BUTTON_TRAY_PP_CMD_PICK_PLACE, &CDlgCommand_TrayPP::OnBnClickedButtonTrayPpCmdPickPlace)
	ON_BN_CLICKED(IDC_BUTTON_TRAY_PP_CMD_AUOTO_HEIGHT_CAL, &CDlgCommand_TrayPP::OnBnClickedButtonTrayPpCmdAuotoHeightCal)
	ON_BN_CLICKED(IDC_BUTTON_TRAY_PP_CMD_EMPTY_CHECK, &CDlgCommand_TrayPP::OnBnClickedButtonTrayPpCmdEmptyCheck)
	ON_BN_CLICKED(IDC_BUTTON_TRAY_PP_CMD_MOVE_PICK_PLACE, &CDlgCommand_TrayPP::OnBnClickedButtonTrayPpCmdMovePickPlace)
END_MESSAGE_MAP()


// CDlgCommand_TrayPP 메시지 처리기입니다.


BOOL CDlgCommand_TrayPP::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CDlgCommand_TrayPP::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// initial
	m_cBmpInitial.LoadBitmap(IDB_CMD_INITIAL);
	m_btnInitial.SetBitmap(m_cBmpInitial);
	// move pos
	m_cBmpMovePos.LoadBitmap(IDB_CMD_MOVEPOS);
	m_btnMovePos.SetBitmap(m_cBmpMovePos);
	// pick place
	m_cBmpPickPlace.LoadBitmap(IDB_PICKPLACE);
	m_btnPickPlace.SetBitmap(m_cBmpPickPlace);
	// pick place move
	m_cBmpPickPlaceMove.LoadBitmap(IDB_PICKPLACE_MOVE);
	m_btnPickPlaceMove.SetBitmap(m_cBmpPickPlaceMove);
	// picker height cal
	m_cBmpPickerHeightCal.LoadBitmap(IDB_PICKER_HEIGHT_CAL_GRAY);
	m_btnPickerHeightCal.SetBitmap(m_cBmpPickerHeightCal);
	// empty check
	m_cBmpEmptyCheck.LoadBitmap(IDB_EMPTY_CHECK);
	m_btnEmptyCheck.SetBitmap(m_cBmpEmptyCheck);
	// close
	m_cBmpClose.LoadBitmap(IDB_CMD_CLOSE);
	m_btnClose.SetBitmap(m_cBmpClose);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgCommand_TrayPP::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CDlgCommand_TrayPP::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}


void CDlgCommand_TrayPP::OnBnClickedButtonTrayPpCmdInit()
{
	m_nSelectedSeqCmdId = eSeqCmd_TrayPP_Initial;
	CDialogEx::OnOK();
}


void CDlgCommand_TrayPP::OnBnClickedButtonTrayPpCmdMovePos()
{
	m_nSelectedSeqCmdId = eSeqCmd_TrayPP_MovePos;
	CDialogEx::OnOK();
}


void CDlgCommand_TrayPP::OnBnClickedButtonTrayPpCmdPickPlace()
{
	//m_nSelectedSeqCmdId = eSeqCmd_TrayPP_PickPlace;
	CDialogEx::OnOK();
}


void CDlgCommand_TrayPP::OnBnClickedButtonTrayPpCmdEmptyCheck()
{
	m_nSelectedSeqCmdId = eSeqCmd_TrayPP_EmptyCheck;
	CDialogEx::OnOK();
}


void CDlgCommand_TrayPP::OnBnClickedButtonTrayPpCmdMovePickPlace()
{
	m_nSelectedSeqCmdId = eSeqCmd_TrayPP_MovePickPlace;
	CDialogEx::OnOK();
}


void CDlgCommand_TrayPP::OnBnClickedButtonTrayPpCmdAuotoHeightCal()
{
	m_nSelectedSeqCmdId = eSeqCmd_TrayPP_PickerHeightCal;
	CDialogEx::OnOK();
}
