// DlgCommand_TestPP.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgCommand_TestPP.h"
#include "afxdialogex.h"
#include "DlgCommand_Param.h"

// CDlgCommand_TestPP 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgCommand_TestPP, CDialogEx)

CDlgCommand_TestPP::CDlgCommand_TestPP(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCommand_TestPP::IDD, pParent)
{
	m_nSelectedSeqCmdId = eSeqCmd_None;
	m_vSelectCmdPicker.clear();
	m_nManuOper = 0;
	ZeroMemory(m_nSelectedSeqCmdParam, sizeof(m_nSelectedSeqCmdParam));
}

CDlgCommand_TestPP::~CDlgCommand_TestPP()
{
	m_cBmpInitial.DeleteObject();
	m_cBmpMovePos.DeleteObject();
	m_cBmpPickPlaceMove.DeleteObject();
	m_cBmpExchangeMove.DeleteObject();
	m_cBmpEmptyCheck.DeleteObject();
	m_cBmpClose.DeleteObject();
}

void CDlgCommand_TestPP::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	// initial
	DDX_Control(pDX, IDC_BUTTON_STACKER_CMD_INIT, m_btnInitial);
	// move pos
	DDX_Control(pDX, IDC_BUTTON_STACKER_CMD_MOVE_POS, m_btnMovePos);
	// pick place move
	DDX_Control(pDX, IDC_BUTTON_STACKER_CMD_MOVE_PICK_PLACE, m_btnPickPlaceMove);
	// exchange move
	DDX_Control(pDX, IDC_BUTTON_STACKER_CMD_MOVE_EXCHANGE, m_btnExchangeMove);
	// empty check
	DDX_Control(pDX, IDC_BUTTON_STACKER_CMD_EMPTY_CHECK, m_btnEmptyCheck);
	// close
	DDX_Control(pDX, IDCANCEL, m_btnClose);
}


BEGIN_MESSAGE_MAP(CDlgCommand_TestPP, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_STACKER_CMD_INIT, &CDlgCommand_TestPP::OnBnClickedButtonStackerCmdInit)
	ON_BN_CLICKED(IDC_BUTTON_STACKER_CMD_MOVE_POS, &CDlgCommand_TestPP::OnBnClickedButtonStackerCmdMovePos)
	ON_BN_CLICKED(IDC_BUTTON_STACKER_CMD_MOVE_PICK_PLACE, &CDlgCommand_TestPP::OnBnClickedButtonStackerCmdMovePickPlace)
	ON_BN_CLICKED(IDC_BUTTON_STACKER_CMD_EMPTY_CHECK, &CDlgCommand_TestPP::OnBnClickedButtonStackerCmdEmptyCheck)
	ON_BN_CLICKED(IDC_BUTTON_STACKER_CMD_MOVE_EXCHANGE, &CDlgCommand_TestPP::OnBnClickedButtonStackerCmdMoveExchange)
	ON_BN_CLICKED(IDC_BUTTON_STACKER_CMD_4x2SAMEPICKPLACE, &CDlgCommand_TestPP::OnBnClickedButtonStackerCmd4x2samepickplace)
END_MESSAGE_MAP()


// CDlgCommand_TestPP 메시지 처리기입니다.


BOOL CDlgCommand_TestPP::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CDlgCommand_TestPP::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	// initial
	m_cBmpInitial.LoadBitmap(IDB_CMD_INITIAL);
	m_btnInitial.SetBitmap(m_cBmpInitial);
	// move pos
	m_cBmpMovePos.LoadBitmap(IDB_CMD_MOVEPOS);
	m_btnMovePos.SetBitmap(m_cBmpMovePos);
	// pick place move
	m_cBmpPickPlaceMove.LoadBitmap(IDB_PICKPLACE_MOVE);
	m_btnPickPlaceMove.SetBitmap(m_cBmpPickPlaceMove);
	// exchange move
	m_cBmpExchangeMove.LoadBitmap(IDB_EXCHANGE_MOVE);
	m_btnExchangeMove.SetBitmap(m_cBmpExchangeMove);
	// empty check
	m_cBmpEmptyCheck.LoadBitmap(IDB_EMPTY_CHECK);
	m_btnEmptyCheck.SetBitmap(m_cBmpEmptyCheck);
	// close
	m_cBmpClose.LoadBitmap(IDB_CMD_CLOSE);
	m_btnClose.SetBitmap(m_cBmpClose);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgCommand_TestPP::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CDlgCommand_TestPP::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}


void CDlgCommand_TestPP::OnBnClickedButtonStackerCmdInit()
{
	m_nSelectedSeqCmdId = eSeqCmd_TestPP_Initial;
	CDialogEx::OnOK();
}


void CDlgCommand_TestPP::OnBnClickedButtonStackerCmdMovePos()
{
	m_nSelectedSeqCmdId = eSeqCmd_TestPP_MovePos;
	CDialogEx::OnOK();
}

void CDlgCommand_TestPP::OnBnClickedButtonStackerCmdMovePickPlace()
{
	m_nSelectedSeqCmdId = eSeqCmd_TestPP_MovePickPlace;
	CDialogEx::OnOK();
}

void CDlgCommand_TestPP::OnBnClickedButtonStackerCmdEmptyCheck()
{
	//m_nSelectedSeqCmdId = eSeqCmd_TestPP_EmptyCheck;
	CDialogEx::OnOK();
}


void CDlgCommand_TestPP::OnBnClickedButtonStackerCmdMoveExchange()
{
	m_nSelectedSeqCmdId = eSeqCmd_TestPP_MoveExchange;
	CDialogEx::OnOK();
}

void CDlgCommand_TestPP::MakeLog(LPCTSTR fmt, ...)
{
	TCHAR tmpMsg[1024] = {0,};
	if(fmt)
	{
		va_list argList;
		va_start(argList, fmt);
		_vstprintf_s(tmpMsg, fmt, argList);
		va_end(argList);
	}
	TWLOG.MakeLog(static_cast<int>(eLogID_SYSTEM), Debug, "", __LINE__, NULL, tmpMsg);
}

void CDlgCommand_TestPP::OnBnClickedButtonStackerCmd4x2samepickplace()
{
	// 미사용
	//	m_nSelectedSeqCmdId = eSeqCmd_TestPP_MoveSameTimePickPlace;
	CDialogEx::OnOK();
}
