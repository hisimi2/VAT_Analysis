// DlgCommand_LoadTbl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgCommand_LoadTbl.h"
#include "afxdialogex.h"


// CDlgCommand_LoadTbl 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgCommand_LoadTbl, CDialogEx)

CDlgCommand_LoadTbl::CDlgCommand_LoadTbl(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCommand_LoadTbl::IDD, pParent)
{
	m_nSelectedSeqCmdId = 0;
	m_nManuOper = 0;
}

CDlgCommand_LoadTbl::~CDlgCommand_LoadTbl()
{
	m_cBmpInitial.DeleteObject();
	m_cBmpMovePos.DeleteObject();
	m_cBmpClose.DeleteObject();
}

void CDlgCommand_LoadTbl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	// initial
	DDX_Control(pDX, IDC_BUTTON_LOAD_TBL_CMD_INIT, m_btnInitial);
	// move pos
	DDX_Control(pDX, IDC_BUTTON_LOAD_TBL_CMD_MOVE_POS, m_btnMovePos);
	// close
	DDX_Control(pDX, IDCANCEL, m_btnClose);
}


BEGIN_MESSAGE_MAP(CDlgCommand_LoadTbl, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_LOAD_TBL_CMD_INIT, &CDlgCommand_LoadTbl::OnBnClickedButtonLoadTblCmdInit)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_TBL_CMD_MOVE_POS, &CDlgCommand_LoadTbl::OnBnClickedButtonLoadTblCmdMovePos)
END_MESSAGE_MAP()


// CDlgCommand_LoadTbl 메시지 처리기입니다.


BOOL CDlgCommand_LoadTbl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CDlgCommand_LoadTbl::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// initial
	m_cBmpInitial.LoadBitmap(IDB_CMD_INITIAL);
	m_btnInitial.SetBitmap(m_cBmpInitial);
	// move pos
	m_cBmpMovePos.LoadBitmap(IDB_CMD_MOVEPOS);
	m_btnMovePos.SetBitmap(m_cBmpMovePos);
	// close
	m_cBmpClose.LoadBitmap(IDB_CMD_CLOSE);
	m_btnClose.SetBitmap(m_cBmpClose);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgCommand_LoadTbl::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CDlgCommand_LoadTbl::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}


void CDlgCommand_LoadTbl::OnBnClickedButtonLoadTblCmdInit()
{
	m_nSelectedSeqCmdId = eSeqCmd_LoadTbl_Initial;
	CDialogEx::OnOK();
}


void CDlgCommand_LoadTbl::OnBnClickedButtonLoadTblCmdMovePos()
{
	m_nSelectedSeqCmdId = eSeqCmd_LoadTbl_MovePos;
	CDialogEx::OnOK();
}
