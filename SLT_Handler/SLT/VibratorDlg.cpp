// VibratorDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "VibratorDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"

// CVibratorDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CVibratorDlg, CDialog)

CVibratorDlg::CVibratorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVibratorDlg::IDD, pParent)
{
	m_nVibUdDvcActCount = g_DMCont.m_dmSetplate.GN( NDM6_CNT_PLACE_DVC_TRAY_FOR_VIBRATION_ACT );
	m_dVibDelayTime = g_DMCont.m_dmSetplate.GD( DDM6_VIBRATOR_ACT_DELAY_SEC );
	m_nVibActTimes = g_DMCont.m_dmSetplate.GN( NDM6_VIBRATOR_ACT_TIMES );
	m_nActionMode = 0;
}

CVibratorDlg::~CVibratorDlg()
{
	::DeleteObject(m_hBitVibrator);
}

void CVibratorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VIBRATOR_SAVE, m_BtnSave);
	DDX_Control(pDX, IDCANCEL, m_BtnCancel);
	DDX_Control(pDX, IDC_EDIT_VIBRATOR_DEALY_TIME, m_editVibDelayTime);
	DDX_Control(pDX, IDC_EDIT_VIBRATOR_ACT_TIMES, m_editVibActTimes);
	DDX_Control(pDX, IDC_EDIT_COUNT_OF_PLACE_DEVICE, m_editVibUdDvcActCount);

	DDX_Text(pDX, IDC_EDIT_VIBRATOR_DEALY_TIME, m_dVibDelayTime);
	DDX_Text(pDX, IDC_EDIT_VIBRATOR_ACT_TIMES, m_nVibActTimes);
	DDX_Text(pDX, IDC_EDIT_COUNT_OF_PLACE_DEVICE,      m_nVibUdDvcActCount);
	DDX_Control(pDX, IDC_PIC1, m_bitPic1);
}


BEGIN_MESSAGE_MAP(CVibratorDlg, CDialog)
	ON_BN_CLICKED(IDC_VIBRATOR_SAVE, &CVibratorDlg::OnBnClickedVibratorSave)
	ON_EN_SETFOCUS( IDC_EDIT_VIBRATOR_DEALY_TIME, &CVibratorDlg::OnEnSetfocusEditUnloadSetplate1)
	ON_EN_SETFOCUS( IDC_EDIT_COUNT_OF_PLACE_DEVICE, &CVibratorDlg::OnEnSetfocusEditActCnt)
	ON_EN_SETFOCUS( IDC_EDIT_VIBRATOR_ACT_TIMES, &CVibratorDlg::OnEnSetfocusEditUnloadCount)
END_MESSAGE_MAP()

BOOL CVibratorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	OnBtnGUI();
	OnEditBoxGUI();

	m_hBitVibrator.LoadBitmap(IDB_BITMAP_VIBRATOR);
	m_bitPic1.SetBitmap(m_hBitVibrator);
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

// CVibratorDlg 메시지 처리기입니다.
void CVibratorDlg::OnBtnGUI()
{
	m_BtnCancel.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnCancel.SetIcon(IDI_BTN_EXIT);
	m_BtnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnSave.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnSave.SetIcon(IDI_BTN_SAVE);
	m_BtnSave.SetAlign(CButtonST::ST_ALIGN_VERT);	
}

void CVibratorDlg::OnBnClickedVibratorSave()
{
	UpdateData(TRUE);

	g_DMCont.m_dmSetplate.SN( NDM6_CNT_PLACE_DVC_TRAY_FOR_VIBRATION_ACT, m_nVibUdDvcActCount );
	g_DMCont.m_dmSetplate.SD( DDM6_VIBRATOR_ACT_DELAY_SEC, m_dVibDelayTime );
	g_DMCont.m_dmSetplate.SN( NDM6_VIBRATOR_ACT_TIMES, m_nVibActTimes );

	char szCokName[512] = {0,};
	ST_BASIC_INFO   stBasicData, stBeforeBasicData;
    g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));
	g_COK_FileMgr.LoadBasicInfo(szCokName, stBasicData);

	stBeforeBasicData = stBasicData;

	stBasicData.nVibUdDvcActCount = m_nVibUdDvcActCount;
	stBasicData.dVibActDelayTime = m_dVibDelayTime;
	stBasicData.nVibActCnt = m_nVibActTimes;

	g_COK_FileMgr.SaveBasicInfo(szCokName, &stBasicData);

	MakeLog("Vibrator UdDvcActCount = %d, ActDealy = %d, ActCount = %d", m_nVibActTimes, (int)(m_dVibDelayTime*1000), m_nVibUdDvcActCount );
	
}

void CVibratorDlg::OnEnSetfocusEditUnloadSetplate1()
{
	int nRet;
	char szVal[ 10 ] = { 0, };
	char szRetVal[ 64 ] = { 0, };
	GetDlgItem( IDC_EDIT_VIBRATOR_DEALY_TIME )->GetWindowText( szVal, _countof( szVal ) );


	nRet = g_LibCommCtrl.GetNumberBox( this, szRetVal, 10, szVal, "Vibrator action Delay Time", "20", "0" );

	if( nRet == RETURN_OK ) {
		GetDlgItem( IDC_EDIT_VIBRATOR_DEALY_TIME )->SetWindowText( szRetVal );
	}
	else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T( "Range Over" ) );
	}
}

void CVibratorDlg::OnEditBoxGUI()
{
	m_editVibUdDvcActCount.SetBackColor(COLOR_WWHITE);
	m_editVibDelayTime.SetBackColor(COLOR_WWHITE);
	m_editVibActTimes.SetBackColor(COLOR_WWHITE);

	m_editVibUdDvcActCount.SetTextSize(30);
	m_editVibDelayTime.SetTextSize(30);
	m_editVibActTimes.SetTextSize(30);
}

void CVibratorDlg::MakeLog(LPCTSTR fmt, ...)
{
	TCHAR tmpMsg[1024] = {0,};
	if(fmt)
	{
		va_list argList;
		va_start(argList, fmt);
		_vstprintf(tmpMsg, fmt, argList);
		va_end(argList);
	}
	TWLOG.MakeLog(static_cast<int>(eLogID_System_GUI), Debug, "", __LINE__, NULL, tmpMsg);
}

// vData.first = stCokData, vData.second.first = BeforeData vData.second.second = AfterData
void CVibratorDlg::SetplateVibratorChangeLog(std::vector<std::pair<CString,std::pair<double,double>>> vData)
{
	for(int i = 0 ; i< (int)vData.size(); i++)
	{
		std::pair<CString,std::pair<double,double>> p;

		p = vData[i];
		
		MakeLog("[MENU = Setplate Vibrator, DATA NAME = %s ][BEFORE=%.2f, AFTER=%.2f]", p.first, p.second.first, p.second.second);
		
	}
}

void CVibratorDlg::OnEnSetfocusEditActCnt()
{
	char szVal[10] = {0,};
	char szRetVal[64] = {0,};
	GetDlgItem( IDC_EDIT_COUNT_OF_PLACE_DEVICE )->GetWindowText( szVal, _countof( szVal ) );

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Count of place device", "100", "0");

	if(nRet == RETURN_OK){
		GetDlgItem( IDC_EDIT_COUNT_OF_PLACE_DEVICE )->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CVibratorDlg::OnEnSetfocusEditUnloadCount()
{
	char szVal[10] = {0,};
	char szRetVal[64] = {0,};
	GetDlgItem( IDC_EDIT_VIBRATOR_ACT_TIMES )->GetWindowText( szVal, _countof( szVal ) );

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Vibrator Act Times", "20", "0");

	if(nRet == RETURN_OK){
		GetDlgItem( IDC_EDIT_VIBRATOR_ACT_TIMES )->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}
