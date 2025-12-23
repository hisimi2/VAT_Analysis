// DlgSetupTesterIF.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgSetupTesterIF.h"
#include "afxdialogex.h"


// CDlgSetupTesterIF 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgSetupTesterIF, CDialogEx)

CDlgSetupTesterIF::CDlgSetupTesterIF(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSetupTesterIF::IDD, pParent)
{

}

CDlgSetupTesterIF::~CDlgSetupTesterIF()
{
}

void CDlgSetupTesterIF::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgSetupTesterIF, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgSetupTesterIF 메시지 처리기입니다.

BOOL CDlgSetupTesterIF::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CDlgSetupTesterIF::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgSetupTesterIF::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CDlgSetupTesterIF::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CDlgSetupTesterIF::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}