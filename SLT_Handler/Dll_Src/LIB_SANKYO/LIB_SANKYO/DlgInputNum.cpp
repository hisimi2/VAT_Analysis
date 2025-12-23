// DlgInputNum.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgInputNum.h"
#include "afxdialogex.h"


// CDlgInputNum 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgInputNum, CDialogEx)

CDlgInputNum::CDlgInputNum(int nNum, CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgInputNum::IDD, pParent)
{
	ZeroMemory(m_szNum, sizeof(m_szNum));
	sprintf_s(m_szNum, sizeof(m_szNum), "%d", nNum);
}

CDlgInputNum::~CDlgInputNum()
{
}

void CDlgInputNum::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgInputNum, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_INPUT, &CDlgInputNum::OnEnChangeEditInput)
END_MESSAGE_MAP()


// CDlgInputNum 메시지 처리기입니다.


BOOL CDlgInputNum::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetDlgItemText(IDC_EDIT_INPUT, m_szNum);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgInputNum::GetNum(char* szBuff, int nSize)
{
	strncpy_s(szBuff, nSize, m_szNum, strlen(m_szNum));
}

void CDlgInputNum::SetNum(char* szBuff, int nSize)
{
	strncpy_s(m_szNum, sizeof(m_szNum), szBuff, nSize);
}


void CDlgInputNum::OnEnChangeEditInput()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItemText(IDC_EDIT_INPUT, m_szNum, sizeof(m_szNum));
}
