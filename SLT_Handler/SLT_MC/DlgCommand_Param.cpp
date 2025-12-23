// DlgCommand_Param.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgCommand_Param.h"
#include "afxdialogex.h"


// CDlgCommand_Param 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgCommand_Param, CDialogEx)

CDlgCommand_Param::CDlgCommand_Param(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCommand_Param::IDD, pParent)
{
	m_vPrmBtnCtrlId.push_back(IDC_BUTTON_PARAM1);
	m_vPrmBtnCtrlId.push_back(IDC_BUTTON_PARAM2);
	m_vPrmBtnCtrlId.push_back(IDC_BUTTON_PARAM3);
	m_vPrmBtnCtrlId.push_back(IDC_BUTTON_PARAM4);
	m_vPrmBtnCtrlId.push_back(IDC_BUTTON_PARAM5);
	m_vPrmBtnCtrlId.push_back(IDC_BUTTON_PARAM6);
	m_vPrmBtnCtrlId.push_back(IDC_BUTTON_PARAM7);
	m_vPrmBtnCtrlId.push_back(IDC_BUTTON_PARAM8);
	m_strSelectedItem = "";
}

CDlgCommand_Param::~CDlgCommand_Param()
{
}

void CDlgCommand_Param::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgCommand_Param, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_PARAM1, &CDlgCommand_Param::OnBnClickedButtonParam1)
	ON_BN_CLICKED(IDC_BUTTON_PARAM2, &CDlgCommand_Param::OnBnClickedButtonParam2)
	ON_BN_CLICKED(IDC_BUTTON_PARAM3, &CDlgCommand_Param::OnBnClickedButtonParam3)
	ON_BN_CLICKED(IDC_BUTTON_PARAM4, &CDlgCommand_Param::OnBnClickedButtonParam4)
	ON_BN_CLICKED(IDC_BUTTON_PARAM5, &CDlgCommand_Param::OnBnClickedButtonParam5)
	ON_BN_CLICKED(IDC_BUTTON_PARAM6, &CDlgCommand_Param::OnBnClickedButtonParam6)
	ON_BN_CLICKED(IDCANCEL, &CDlgCommand_Param::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_PARAM7, &CDlgCommand_Param::OnBnClickedButtonParam7)
	ON_BN_CLICKED(IDC_BUTTON_PARAM8, &CDlgCommand_Param::OnBnClickedButtonParam8)
END_MESSAGE_MAP()


// CDlgCommand_Param 메시지 처리기입니다.


BOOL CDlgCommand_Param::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CDlgCommand_Param::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	for(std::vector<int>::size_type i=0; i <m_vPrmBtnCtrlId.size(); i++){
		GetDlgItem(m_vPrmBtnCtrlId[i])->ShowWindow(SW_HIDE);
	}

	for(int i=0; i < (int)m_strParamNameList.size(); i++){
		GetDlgItem(m_vPrmBtnCtrlId[i])->ShowWindow(SW_SHOW);
		SetDlgItemText(m_vPrmBtnCtrlId[i], m_strParamNameList[i].c_str());
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgCommand_Param::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CDlgCommand_Param::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}


void CDlgCommand_Param::OnBnClickedButtonParam1()
{
	m_strSelectedItem = m_strParamNameList[0].c_str();
	CDialogEx::OnOK();
}


void CDlgCommand_Param::OnBnClickedButtonParam2()
{
	m_strSelectedItem = m_strParamNameList[1].c_str();
	CDialogEx::OnOK();
}

void CDlgCommand_Param::OnBnClickedButtonParam3()
{
	m_strSelectedItem = m_strParamNameList[2].c_str();
	CDialogEx::OnOK();
}


void CDlgCommand_Param::OnBnClickedButtonParam4()
{
	m_strSelectedItem = m_strParamNameList[3].c_str();
	CDialogEx::OnOK();
}


void CDlgCommand_Param::OnBnClickedButtonParam5()
{
	m_strSelectedItem = m_strParamNameList[4].c_str();
	CDialogEx::OnOK();
}


void CDlgCommand_Param::OnBnClickedButtonParam6()
{
	m_strSelectedItem = m_strParamNameList[5].c_str();
	CDialogEx::OnOK();
}

void CDlgCommand_Param::OnBnClickedButtonParam7()
{
	m_strSelectedItem = m_strParamNameList[6].c_str();
	CDialogEx::OnOK();
}

void CDlgCommand_Param::OnBnClickedButtonParam8()
{
	m_strSelectedItem = m_strParamNameList[7].c_str();
	CDialogEx::OnOK();
}

void CDlgCommand_Param::SetParamName(std::vector<std::string> strParamNameList)
{
	m_strParamNameList = strParamNameList;
}

void CDlgCommand_Param::GetSelectedItem(CString& strSelectedItem)
{
	strSelectedItem = m_strSelectedItem;
}

void CDlgCommand_Param::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}
