// SltPgmSetDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "SltPgmSetDlg.h"
#include "afxdialogex.h"
#include "direct.h"

// CSltPgmSetDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSltPgmSetDlg, CDialog)

CSltPgmSetDlg::CSltPgmSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSltPgmSetDlg::IDD, pParent)
{
	m_strGetSelectListbox = _T("");
	m_nListIndex = 0;
}

CSltPgmSetDlg::~CSltPgmSetDlg()
{
}

void CSltPgmSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PKG_NAME, m_strPgmName);
	DDX_Control(pDX, IDC_LIST_PKG      , m_ListPGM);
	DDX_LBIndex(pDX, IDC_LIST_PKG      , m_nListIndex);
//	DDX_Control(pDX, IDC_BUTTON_SAVE   , m_BtnSave);
	DDX_Control(pDX, IDCANCEL          , m_BtnCancel);
	DDX_Control(pDX, IDC_BUTTON_MODIFY , m_BtnModify);
	DDX_Control(pDX, IDC_BUTTON_DEL    , m_BtnDelete);
	DDX_Control(pDX, IDC_BUTTON_NEW    , m_BtnNew);
}


BEGIN_MESSAGE_MAP(CSltPgmSetDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CSltPgmSetDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CSltPgmSetDlg::OnBnClickedButtonDel)
	ON_LBN_SELCHANGE(IDC_LIST_PKG, &CSltPgmSetDlg::OnLbnSelchangeListPgm)
	ON_BN_CLICKED(IDC_BUTTON_NEW, &CSltPgmSetDlg::OnBnClickedButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, &CSltPgmSetDlg::OnBnClickedButtonModify)
END_MESSAGE_MAP()


// CSltPgmSetDlg 메시지 처리기입니다.


BOOL CSltPgmSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	char szData[MAX_PATH] = {0,};
	g_DMCont.m_dmHandlerOpt.GS(SDM11_SLT_ProgramName, szData, sizeof(szData));
	GetDlgItem(IDC_EDIT_PKG_NAME)->SetWindowText((LPCTSTR)szData);
	m_strGetSelectListbox = m_strPgmName = (LPCTSTR)szData;

	OnBtnGUI();
	OnRadioBtnDisplayUpdate();
	InitListData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSltPgmSetDlg::OnBtnGUI()
{
	m_BtnCancel.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnCancel.SetIcon(IDI_BTN_EXIT);
	m_BtnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);

// 	m_BtnSave.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
// 	m_BtnSave.SetIcon(IDI_BTN_SAVE);
// 	m_BtnSave.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnModify.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnModify.SetIcon(IDI_BTN_MODIFY);
	m_BtnModify.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnDelete.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnDelete.SetIcon(IDI_BTN_CLEAR);
	m_BtnDelete.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnNew.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnNew.SetIcon(IDI_BTN_NEW);
	m_BtnNew.SetAlign(CButtonST::ST_ALIGN_VERT);
}

void CSltPgmSetDlg::OnBnClickedButtonSave()
{

}

void CSltPgmSetDlg::OnRadioBtnDisplayUpdate()
{

}


void CSltPgmSetDlg::OnBnClickedButtonDel()
{
	UpdateData(TRUE);
	int nRetMsg = 0;

	if(m_strGetSelectListbox == m_strPgmName){
		AfxMessageBox("can't delete this file!! Because this is setup file...!!!");
		return;
	}else{
		nRetMsg = AfxMessageBox(" Do you want to delete?", MB_TOPMOST | MB_YESNO);

		if(nRetMsg != IDYES)
			return;
	}

	char szTemp[512]={0,};
	::GetCurrentDirectory(sizeof(szTemp), szTemp);

	CString strDir = _T("");
	strDir.Format("%s\\sys_data\\HandlerRecipe\\%s\\%s\\Tester.dat",szTemp,SZ_SLT_PGM_FOLDER,m_strGetSelectListbox);
	::DeleteFile( strDir );
	strDir.Format( "%s\\sys_data\\HandlerRecipe\\%s\\%s\\DefaultBin.csv", szTemp, SZ_SLT_PGM_FOLDER, m_strGetSelectListbox );
	::DeleteFile( strDir );
	strDir.Format( "%s\\sys_data\\HandlerRecipe\\%s\\%s", szTemp, SZ_SLT_PGM_FOLDER, m_strGetSelectListbox );

	int nRtn = ::RemoveDirectory( strDir );
	nRtn = ::GetLastError();

	CString strSetPKGData = _T("");
	GetDlgItem(IDC_LIST_PKG)->GetWindowText(strSetPKGData);
	char szTempCok[256] = {0,};
	sprintf_s(szTempCok, sizeof(szTempCok), strSetPKGData);
 	InitListData();
// 	InitTrayData(szTempCok);

	MakeLog("[MENU = SLT Program Setup, Program Name = %s ][ Delete ]", m_strGetSelectListbox);
}

void CSltPgmSetDlg::InitListData()
{
	m_ListPGM.ResetContent();

	_chdir("SYS_DATA");
	_chdir("HandlerRecipe");
	_chdir(SZ_SLT_PGM_FOLDER);
	CFileFind finder;
	BOOL endf = finder.FindFile();

	while(endf) 
	{
		endf = finder.FindNextFile();
		CString strExt = finder.GetFileName();

		if(!finder.IsDots() && finder.IsDirectory()){		
			//strExt.Replace(".dat", "");
			m_ListPGM.AddString(strExt);
		}
		NEXT;
	}
	_chdir("..");
	_chdir("..");
	_chdir("..");

	CString strSetPKGData = _T("");
	CString strData = _T("");
	GetDlgItem(IDC_EDIT_PKG_NAME)->GetWindowText(strSetPKGData);
	for(int i=0; i< m_ListPGM.GetCount(); i++)
	{
		strData.Empty();
		m_ListPGM.GetText(i, strData);
		if(strData == strSetPKGData){
			m_ListPGM.SetCurSel(i);
			m_nListIndex = i;
		}
	}
}

void CSltPgmSetDlg::OnLbnSelchangeListPgm()
{
	m_ListPGM.GetText(m_ListPGM.GetCurSel(),m_strGetSelectListbox);
	m_nListIndex = m_ListPGM.GetCurSel();
}

void CSltPgmSetDlg::OnBnClickedButtonNew()
{
	UpdateData(TRUE);
	
    CDlgTesterPgmSet DO;
    DO.m_nNewModifyType = eSltPgm_ItemNew;
    DO.DoModal();

	InitListData();
}


void CSltPgmSetDlg::OnBnClickedButtonModify()
{
	UpdateData(TRUE);

	char szPath[MAX_PATH]  = {0,};
	char szTemp[MAX_PATH]  = {0,};
	char szSltPgmType[256] = {0,};
	::GetCurrentDirectory(sizeof(szTemp), szTemp);
	sprintf_s(szPath, sizeof(szPath), "%s\\sys_data\\HandlerRecipe\\%s\\%s\\Tester.dat",szTemp,SZ_SLT_PGM_FOLDER, (LPCTSTR)m_strGetSelectListbox);


    CDlgTesterPgmSet DO;
    DO.m_strPgmName = m_strGetSelectListbox;
    DO.m_nNewModifyType = eSltPgm_ItemModify;
    DO.DoModal();
}


void CSltPgmSetDlg::MakeLog(LPCTSTR fmt, ...)
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