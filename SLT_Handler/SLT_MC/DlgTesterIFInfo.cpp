// DlgTesterIFInfo.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgTesterIFInfo.h"
#include "afxdialogex.h"


// CDlgTesterIFInfo 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTesterIFInfo, CDialogEx)

CDlgTesterIFInfo::CDlgTesterIFInfo(int nUnitIdx,CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgTesterIFInfo::IDD, pParent)
{
	m_nTesterUnitIdx = nUnitIdx;
}

CDlgTesterIFInfo::~CDlgTesterIFInfo()
{
}

void CDlgTesterIFInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_VERSION_INFO, m_treeInfo);
}


BEGIN_MESSAGE_MAP(CDlgTesterIFInfo, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgTesterIFInfo::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDlgTesterIFInfo 메시지 처리기입니다.


BOOL CDlgTesterIFInfo::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CDlgTesterIFInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitCtrl();
	SetTimer(1, 500, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgTesterIFInfo::OnDestroy()
{
	CDialogEx::OnDestroy();

	KillTimer(1);
}


void CDlgTesterIFInfo::OnTimer(UINT_PTR nIDEvent)
{
	switch( nIDEvent )
	{
	case 1:
		{
			KillTimer(1);
			UpdateTesterInfo();
			SetTimer(1, 500, NULL);
		}break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CDlgTesterIFInfo::InitCtrl()
{
	CTesterStatus cTesterStatus = g_AppleTestIF.GetTesterStatus(m_nTesterUnitIdx);
	if( cTesterStatus.m_nRegistration != REG_TESTER ){
		return;
	}

	HTREEITEM  hRoot;
	HTREEITEM  hChild;
	std::vector<HTREEITEM> vhChild;
	hRoot = m_treeInfo.InsertItem(_T("Tester Information"), 0/* nImage */, 1/* nSelectedImage */, TVI_ROOT, TVI_LAST ) ;
	// version
	{
		hChild = m_treeInfo.InsertItem(_T("Versions"), 1/* nImage */, 1/* nSelectedImage */, hRoot, TVI_LAST ) ;
		std::vector<std::pair<std::string,std::string>> listVersion;
		cTesterStatus.PasingMsgType(cTesterStatus.m_sInfo[0], listVersion);
		for(std::vector<std::pair<std::string,std::string>>::size_type i=0; i<listVersion.size(); ++i){
			std::pair<std::string,std::string>& p = listVersion[i];
			CString strVersion;
			strVersion.Format("%s:%s", p.first.c_str(), p.second.c_str());
			m_treeInfo.InsertItem(strVersion, 1/* nImage */, 1/* nSelectedImage */, hChild, TVI_LAST ) ;
		}
		vhChild.push_back(hChild);
	}

	m_treeInfo.Expand(hRoot, TVE_EXPAND);
	for(std::vector<HTREEITEM>::size_type i=0; i<vhChild.size(); ++i){
		m_treeInfo.Expand(vhChild[i], TVE_EXPAND);
	}
}

void CDlgTesterIFInfo::UpdateTesterInfo()
{
	CTesterStatus cTesterStatus = g_AppleTestIF.GetTesterStatus(m_nTesterUnitIdx);
	if( cTesterStatus.m_nRegistration != REG_TESTER ){
		return;
	}

	// testerid
	CString strStationID;
	strStationID.Format("StationID : %d", cTesterStatus.m_nStationID);
	SetDlgItemText(IDC_STATIC_TESTER_ID, strStationID );
	// status
	SetDlgItemText(IDC_STATIC_TESTER_STATUS, cTesterStatus.GetStatus());
	// location
	CString strLocation;
	strLocation.Format("Location : %d", cTesterStatus.m_nReserved2);
	SetDlgItemText(IDC_STATIC_TESTER_LOCATION, strLocation);
	// occupied
	CString strOccupied;
	if( cTesterStatus.m_nReserved3 == 1){
		strOccupied = _T("Occupied");
	}else{
		strOccupied = _T("Not occupied");
	}
	SetDlgItemText(IDC_STATIC_TESTER_OCCUPIED, strOccupied);
}

void CDlgTesterIFInfo::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
