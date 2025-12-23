// DlgManuVisionSetup.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgManuVisionSetup.h"
#include "afxdialogex.h"


// CDlgManuVisionSetup 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgManuVisionSetup, CDialogEx)

CDlgManuVisionSetup::CDlgManuVisionSetup(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgManuVisionSetup::IDD, pParent)
{
	m_vVisionCommObj.clear();
	m_vVisionCommObj.push_back(g_pBarcode);
	m_vVisionCommObj.push_back(g_pVisionTestHand_1);
	m_vVisionCommObj.push_back(g_pVisionTestHand_2);

	m_nVisionPc = 0;
}

CDlgManuVisionSetup::~CDlgManuVisionSetup()
{
}

void CDlgManuVisionSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgManuVisionSetup, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgManuVisionSetup::OnBnClickedOk)
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgManuVisionSetup 메시지 처리기입니다.


BOOL CDlgManuVisionSetup::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	CString strIp = _T("");
	int nPort = 0;
	char szTemp[128]={0,};

	m_vVisionCommObj[m_nVisionPc]->ReadConnectConfig(strIp, nPort);
	sprintf_s(szTemp, sizeof(szTemp), "%d", nPort);

	SetDlgItemText(IDC_EDIT_VISION_CONNET_IP, strIp);	
	SetDlgItemText(IDC_EDIT_VISION_CONNECT_PORT, szTemp);

	CString strTitle;
	strTitle.Format(_T("VISION IP SETUP - %s"), (LPCTSTR)(m_vVisionCommObj[m_nVisionPc]->GetVisionPcName()) );
	SetDlgItemText(IDC_SSPANEL_VISION_IP_SETUP_TITLE, strTitle);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL CDlgManuVisionSetup::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDlgManuVisionSetup::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CDlgManuVisionSetup::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}

void CDlgManuVisionSetup::OnBnClickedOk()
{
	char szIp[128]={0,};
	char szPort[128]={0,};

	GetDlgItemText(IDC_EDIT_VISION_CONNET_IP, szIp, sizeof(szIp));
	GetDlgItemText(IDC_EDIT_VISION_CONNECT_PORT, szPort, sizeof(szPort));

	char szDriverFileName[MAX_PATH]={0,};
	char szTemp[512]={0,};
	::GetCurrentDirectory(sizeof(szTemp), szTemp);
	sprintf_s(szDriverFileName, sizeof(szDriverFileName), "%s\\sys_data\\%s", szTemp, SZ_DRIVER_FILE_NAME);
	CConfigData ConfigData(szDriverFileName);
	int nCompanyType = ConfigData.GetInt("MC TO VISION COMM CONFIG", "2D COMPANY TYPE", "1");

	if( e2dVision_TYPE_TW == nCompanyType || m_nVisionPc != eVision_BarCode ){
		int nErr = m_vVisionCommObj[m_nVisionPc]->SocketConnect(eSocketTypeClient, szIp, atoi(szPort));
		m_vVisionCommObj[m_nVisionPc]->WriteConnectConfig(szIp, atoi(szPort));
	}else{
		g_pBarcode->Finalize();
		Sleep(1000);
		g_pBarcode = new CVisionComm_Barcode(eVision_BarCode, "BARCODE", nCompanyType);
		int nErr = g_pBarcode->Initialize(this, szDriverFileName);
		int aa = 0;
	}

	CDialogEx::OnOK();
}

