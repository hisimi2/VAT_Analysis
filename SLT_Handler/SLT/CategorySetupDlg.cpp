// CategorySetupDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "CategorySetupDlg.h"
#include "afxdialogex.h"
#include "direct.h"

// CCategorySetupDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCategorySetupDlg, CDialog)

CCategorySetupDlg::CCategorySetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCategorySetupDlg::IDD, pParent)
{
	m_nProjectName = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
}

CCategorySetupDlg::~CCategorySetupDlg()
{
}

void CCategorySetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL, m_BtnCancel);
	DDX_Control(pDX, IDC_BUTTON_MODIFY, m_BtnModify);
	DDX_Control(pDX, IDC_BUTTON_CONNECT, m_BtnConnect);
	DDX_Control(pDX, IDC_EDIT_CATE, m_EditCategoryText);
	DDX_Control(pDX, IDC_LIST_CATE, m_ListCategory);
	DDX_Control(pDX, IDC_LIST_SW_BINSET, m_ListDataDisplay);
	DDX_Control(pDX, IDC_BUTTON_NEW, m_BtnNew);
	DDX_Control(pDX, IDC_BUTTON_DEL, m_BtnDel);
}


BEGIN_MESSAGE_MAP(CCategorySetupDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, &CCategorySetupDlg::OnBnClickedButtonModify)
	ON_LBN_SELCHANGE(IDC_LIST_CATE, &CCategorySetupDlg::OnLbnSelchangeListCate)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CCategorySetupDlg::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_NEW, &CCategorySetupDlg::OnBnClickedButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CCategorySetupDlg::OnBnClickedButtonDel)
END_MESSAGE_MAP()


// CCategorySetupDlg 메시지 처리기입니다.


void CCategorySetupDlg::OnBnClickedButtonModify()
{
	if( CheckDataSave() == FALSE )
		return;

	CCategorySetupBinDlg DO;
	DO.m_stCateData  = m_stCateData;
	DO.m_strCateName = m_strListSelName;
	DO.DoModal();

	char szData[MAX_PATH] = {0,};
	g_DMCont.m_dmHandlerOpt.GS(SDM11_CategoryName, szData, sizeof(szData));
	m_strListSelName = (LPCTSTR)szData;
	GetDlgItem(IDC_EDIT_CATE)->SetWindowText(m_strListSelName);

	m_ListDataDisplay.DeleteAllItems();
	ST_CATE_INFO lpstData;
	g_COK_FileMgr.LoadCategoryInfo(szData, lpstData);
	m_stCateData = lpstData;
	UpdateDataDisplay(lpstData);

	UpdateListData();

	BOOL bDotMatrix = g_DMCont.m_dmEQP.GB(BDM0_MD_DOTMATRIX);
	if (bDotMatrix == eOPTION_USE)
		SetDotMatrixUpdate();
}

BOOL CCategorySetupDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	OnBtnGUI();
	

	m_ListDataDisplay.InsertColumn(1, "SW Bin", 0, 80);
	m_ListDataDisplay.InsertColumn(2, "HW Bin", 0, 80);
	//m_ListDataDisplay.InsertColumn(3, "Code", 0, 100);
	m_ListDataDisplay.InsertColumn(3, "Description", 0, 300);
	m_ListDataDisplay.InsertColumn(4, "Pass/Fail", 0, 85);
	m_ListDataDisplay.InsertColumn(5, DEF_PROJECT_WATT == m_nProjectName ? "HW Bin(Tester)" : "Error Code", 0, 140);


	char szData[MAX_PATH] = {0,};
	g_DMCont.m_dmHandlerOpt.GS(SDM11_CategoryName, szData, sizeof(szData));
	m_strListSelName = (LPCTSTR)szData;
	GetDlgItem(IDC_EDIT_CATE)->SetWindowText(m_strListSelName);

	ST_CATE_INFO lpstData;
	g_COK_FileMgr.LoadCategoryInfo(szData, lpstData);
	m_stCateData = lpstData;
	UpdateDataDisplay(lpstData);

	UpdateListData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CCategorySetupDlg::OnBtnGUI()
{
	m_BtnCancel.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnCancel.SetIcon(IDI_BTN_EXIT);
	m_BtnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnModify.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnModify.SetIcon(IDI_BTN_MODIFY);
	m_BtnModify.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnConnect.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnConnect.SetIcon(IDI_BTN_CONNECT);
	m_BtnConnect.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnNew.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnNew.SetIcon(IDI_BTN_NEW);
	m_BtnNew.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnDel.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnDel.SetIcon(IDI_BTN_CLEAR);
	m_BtnDel.SetAlign(CButtonST::ST_ALIGN_VERT);
}



void CCategorySetupDlg::UpdateListData()
{
	m_ListCategory.ResetContent();

	_chdir("SYS_DATA");
	_chdir("HandlerRecipe");
	_chdir(SZ_CATEGORY_FOLDER);
	CFileFind finder;
	BOOL endf = finder.FindFile();

	int nTmpIndex = 0, nFindIndex = 0;
	char szData[512] = {0,};
	g_DMCont.m_dmHandlerOpt.GS(SDM11_CategoryName, szData, sizeof(szData));

	while(endf) 
	{
		endf = finder.FindNextFile();
		CString strExt = finder.GetFileName();

		if(!finder.IsDots() && !finder.IsDirectory())
		{
			strExt.Replace(".csv", "");
			m_ListCategory.AddString(strExt);

			if(strExt.Compare(szData) ==0){
				nFindIndex = nTmpIndex;
			}
			nTmpIndex++;
		}
		NEXT;
	}
	_chdir("..");
	_chdir("..");
	_chdir("..");

	m_ListCategory.SetCurSel(nFindIndex);
}

void CCategorySetupDlg::OnLbnSelchangeListCate()
{
	m_ListDataDisplay.DeleteAllItems();
	
	int nIndex = m_ListCategory.GetCurSel();
	m_ListCategory.GetText(nIndex, m_strListSelName);

	ST_CATE_INFO lpstData;
	g_COK_FileMgr.LoadCategoryInfo((LPSTR)(LPCSTR)m_strListSelName, lpstData);

	m_stCateData = lpstData;
	UpdateDataDisplay(lpstData);
}

void CCategorySetupDlg::UpdateDataDisplay(ST_CATE_INFO lpstData)
{
	ST_CATE_INFO stCateData = lpstData;
	int nCount = stCateData.nIndexCount;

	CString str[5];
	for(int i=0; i<nCount; i++)
	{
		str[0].Format( "%s", stCateData.aCateInfo[i].szSWBin );
		str[1].Format( "%s", stCateData.aCateInfo[i].szHWBin );
		//str[2].Format( "%s", stCateData.aCateInfo[i].szCode );
		str[2].Format( "%s", stCateData.aCateInfo[i].szDesc );
		str[3].Format( "%s", stCateData.aCateInfo[i].szPassFail );
		str[4].Format("%s", DEF_PROJECT_WATT == m_nProjectName ? stCateData.aCateInfo[i].szHWBin_Tester :stCateData.aCateInfo[i].szCode);
		m_ListDataDisplay.InsertItem(i, str[0]);

		for( int j = 1; j < _countof(str); j++ ){
			m_ListDataDisplay.SetItem( i, j, LVIF_TEXT, str[ j ], 0, 0, 0, NULL);
		}
	}
}

BOOL CCategorySetupDlg::CheckDataSave()
{
	BOOL bRet = TRUE;
	BOOL bLotState = g_DMCont.m_dmEQP.GB( BDM0_SYS_LOT_STATUS );
	if( bLotState == eHandlerLotMode_OnGoing ) {
		AfxMessageBox( "Handler is Running!! Please save after lot end" );
		bRet = FALSE;
	}
	return bRet;
}

#include <afxsock.h>
void CCategorySetupDlg::OnBnClickedButtonConnect()
{
	//CSocket clientSocket;
	CAsyncSocket clientSocket;
	CString strAdd = _T("127.0.0.1");

	if(clientSocket.Connect(strAdd, strlen(strAdd))){
		AfxMessageBox("Connected to server.");
	}else{
 		AfxMessageBox("Cannot find server.");
	}
}


void CCategorySetupDlg::OnBnClickedButtonNew()
{
	if( CheckDataSave() == FALSE )
		return;

	MakeLog("[MENU = Category] [New Button Click]");
	CRegisterCategory dlg;
	dlg.DoModal();
	UpdateListData();
}


void CCategorySetupDlg::OnBnClickedButtonDel()
{
	if( CheckDataSave() == FALSE )
		return;

	int nRetMsg = 0;

	int nIndex = m_ListCategory.GetCurSel();
	m_ListCategory.GetText(nIndex, m_strListSelName);

	char szData[MAX_PATH] ={0,};
	g_DMCont.m_dmHandlerOpt.GS(SDM11_CategoryName, szData, sizeof(szData));
	
	if(m_strListSelName.Compare(szData) == 0){
		AfxMessageBox("can't delete this file!! Because this is setup file...!!!");
		MakeLog("[MENU = Category] [can't delete this file!! Because this is setup file...!!!]");
		return;
	}else{
		nRetMsg = AfxMessageBox("Do you want to delete?", MB_TOPMOST | MB_YESNO);

		if(nRetMsg != IDYES)
			return;
	}

	char szTemp[512]={0,};
	::GetCurrentDirectory(sizeof(szTemp), szTemp);

	CString strDir = _T("");
	strDir.Format("%s\\sys_data\\handlerRecipe\\%s\\%s.csv",szTemp,SZ_CATEGORY_FOLDER,(LPCTSTR)m_strListSelName);


	CString cstrFindFile = strDir;
	CFileFind Finder;
	BOOL bExist = Finder.FindFile( cstrFindFile );
	while( bExist )
	{
		NEXT;
		bExist = Finder.FindNextFile();	
		if( Finder.IsDots()){ continue; }
		else
		{
			CString cstrFileName = Finder.GetFilePath();
			CFileStatus status;
			CFile::GetStatus( cstrFileName, status );
			::DeleteFile(cstrFileName);
		}
	}
	Finder.Close();

	UpdateListData();
	OnLbnSelchangeListCate();

	MakeLog("[MENU = Category] [%s Category File Delete]",strDir);
}

void CCategorySetupDlg::MakeLog(LPCTSTR fmt, ...)
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