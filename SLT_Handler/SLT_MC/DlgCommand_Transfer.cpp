// DlgCommand_Transfer.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgCommand_Transfer.h"
#include "afxdialogex.h"

// CDlgCommand_Transfer 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgCommand_Transfer, CDialogEx)

CDlgCommand_Transfer::CDlgCommand_Transfer(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCommand_Transfer::IDD, pParent)
{
	m_nSelectedSeqCmdId = 0;
}

CDlgCommand_Transfer::~CDlgCommand_Transfer()
{
	m_cBmpInitial.DeleteObject();
	m_cBmpMovePos.DeleteObject();
	m_cBmpMovePickPlace.DeleteObject();
	m_cBmpClose.DeleteObject();
}

void CDlgCommand_Transfer::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_TRANSFER_CMD_INIT, m_btnInitial);
	DDX_Control( pDX, IDC_BUTTON_TRANSFER_CMD_MOVE_POS, m_btnMovePos );
	DDX_Control( pDX, IDC_BUTTON_TRANSFER_CMD_MOVE_PICK_PLACE, m_btnMovePickPlace );
	DDX_Control(pDX, IDCANCEL, m_btnClose);
}


BEGIN_MESSAGE_MAP(CDlgCommand_Transfer, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_TRANSFER_CMD_INIT, &CDlgCommand_Transfer::OnBnClickedButtonTransferCmdInit)
	ON_BN_CLICKED(IDC_BUTTON_TRANSFER_CMD_MOVE_POS, &CDlgCommand_Transfer::OnBnClickedButtonTransferCmdMovePos)
	ON_BN_CLICKED( IDC_BUTTON_TRANSFER_CMD_MOVE_PICK_PLACE, &CDlgCommand_Transfer::OnBnClickedButtonTransferCmdMovePickPlace )
	ON_BN_CLICKED(IDCANCEL, &CDlgCommand_Transfer::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgCommand_Transfer 메시지 처리기입니다.


BOOL CDlgCommand_Transfer::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CDlgCommand_Transfer::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// initial
	m_cBmpInitial.LoadBitmap(IDB_CMD_INITIAL);
	m_btnInitial.SetBitmap(m_cBmpInitial);
	// move pos
	m_cBmpMovePos.LoadBitmap(IDB_CMD_MOVEPOS);
	m_btnMovePos.SetBitmap(m_cBmpMovePos);
	// move pick place
	m_cBmpMovePickPlace.LoadBitmap( IDB_PICKPLACE_MOVE );
	m_btnMovePickPlace.SetBitmap( m_cBmpMovePickPlace );
	// close
	m_cBmpClose.LoadBitmap( IDB_CMD_CLOSE );
	m_btnClose.SetBitmap(m_cBmpClose);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgCommand_Transfer::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CDlgCommand_Transfer::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}


void CDlgCommand_Transfer::OnBnClickedButtonTransferCmdInit()
{
	m_nSelectedSeqCmdId = eSeqCmd_Transfer_Initial;
	CDialogEx::OnOK();
}


void CDlgCommand_Transfer::OnBnClickedButtonTransferCmdMovePos()
{
	m_nSelectedSeqCmdId = eSeqCmd_Transfer_MovePos;
	CDialogEx::OnOK();
}



void CDlgCommand_Transfer::OnBnClickedButtonTransferCmdMovePickPlace()
{
	m_nSelectedSeqCmdId = eSeqCmd_Transfer_MovePickPlace;
	CDialogEx::OnOK();
}


void CDlgCommand_Transfer::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}
