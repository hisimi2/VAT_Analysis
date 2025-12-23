// DlgInputNum.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgInputNum.h"
#include "afxdialogex.h"


// CDlgInputNum 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgInputNum, CDialogEx)

CDlgInputNum::CDlgInputNum(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgInputNum::IDD, pParent)
{
	m_strValue = "";
}

CDlgInputNum::~CDlgInputNum()
{
}

void CDlgInputNum::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgInputNum, CDialogEx)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, &CDlgInputNum::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgInputNum 메시지 처리기입니다.


BOOL CDlgInputNum::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CDlgInputNum::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgInputNum::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CDlgInputNum::GetValue(CString& strValue)
{
	strValue = m_strValue;
}


void CDlgInputNum::OnBnClickedOk()
{
	GetDlgItemText(IDC_EDIT_LIB_MC_INPUT_NUM, m_strValue);
	CDialogEx::OnOK();
}
