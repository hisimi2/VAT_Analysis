// DeviceSetup.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "DeviceSetup.h"
#include "afxdialogex.h"
#include "direct.h"

// CDeviceSetup 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDeviceSetup, CDialog)

CDeviceSetup::CDeviceSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CDeviceSetup::IDD, pParent)
{
	m_strTrayFileBak = _T("");
	m_strSltPgmBak = _T( "" );
	m_strCategoryBack = _T( "" );

	m_strTrayFileName = _T("");

	memset(m_szCustomer, 0x00, sizeof(m_szCustomer));
}

CDeviceSetup::~CDeviceSetup()
{
}

void CDeviceSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL, m_BtnCancel);
	DDX_Control(pDX, IDC_BUTTON_EXPO, m_BtnExport);
	DDX_Control(pDX, IDC_BUTTON_IMPO, m_BtnImport);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_BtnSave);
	DDX_Control(pDX, IDC_LIST_CUSTOM, m_ListCustomer);
	DDX_Control(pDX, IDC_LIST_DVC_NAME, m_ListDvcName);
	DDX_Control(pDX, IDC_LIST_PGM_NAME, m_ListPgmName);
	DDX_Control(pDX, IDC_LIST_TRAYFILE, m_ListTrayFile);
	DDX_Control(pDX, IDC_LIST_SLT_PGM, m_ListSLTPgm);
	DDX_Control(pDX, IDC_LIST_CATE, m_ListCategory);
}


BEGIN_MESSAGE_MAP(CDeviceSetup, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CDeviceSetup::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_IMPO, &CDeviceSetup::OnBnClickedButtonImpo)
	ON_BN_CLICKED(IDC_BUTTON_EXPO, &CDeviceSetup::OnBnClickedButtonExpo)
	ON_BN_CLICKED(IDCANCEL, &CDeviceSetup::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_REG_CUSTOM, &CDeviceSetup::OnBnClickedButtonRegCustom)
	ON_BN_CLICKED(IDC_BUTTON_REG_DVC, &CDeviceSetup::OnBnClickedButtonRegDvc)
	ON_BN_CLICKED(IDC_BUTTON_REG_PGM, &CDeviceSetup::OnBnClickedButtonRegPgm)
//	ON_BN_CLICKED(IDC_BUTTON_CUSTOM_MODIFY, &CDeviceSetup::OnBnClickedButtonCustomModify)
END_MESSAGE_MAP()


// CDeviceSetup 메시지 처리기입니다.


BOOL CDeviceSetup::OnInitDialog()
{
	CDialog::OnInitDialog();
	ZeroMemory(m_szCustomer,sizeof(m_szCustomer));

	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company,m_szCustomer,sizeof(m_szCustomer));
	char szData[MAX_PATH] = {0,};

	OnBtnGUI();
	
	//if(strcmp(m_szCustomer,"AMKOR")==0){
		InitalizeDialog_Amkor();
		
		//MoveWindow(0,0,100,100);
// 	}else{
// 		LoadCustomer();
// 		LoadDeviceName();
// 		LoadProgramName();
// 		g_DMCont.m_dmHandlerOpt.GS(SDM11_CustomerName, szData, sizeof(szData));
// 		GetDlgItem(IDC_EDIT_CUSTOM)->SetWindowText((LPCTSTR)szData);
// 
// 		g_DMCont.m_dmHandlerOpt.GS(SDM11_DeviceName, szData, sizeof(szData));
// 		GetDlgItem(IDC_EDIT_DVC_NAME)->SetWindowText((LPCTSTR)szData);
// 
// 		g_DMCont.m_dmHandlerOpt.GS(SDM11_ProgramName, szData, sizeof(szData));
// 		GetDlgItem(IDC_EDIT_PGM_NAME)->SetWindowText((LPCTSTR)szData);
// 	}

	LoadTrayFile();
	LoadSltPgm();
	LoadCategory();

	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szData, sizeof(szData));
	GetDlgItem(IDC_EDIT_TRAYFILE)->SetWindowText((LPCTSTR)szData);
	m_strTrayFileBak.Format("%s", szData);

	g_DMCont.m_dmHandlerOpt.GS(SDM11_SLT_ProgramName, szData, sizeof(szData));
	GetDlgItem(IDC_EDIT_SLT_PGM)->SetWindowText((LPCTSTR)szData);
	m_strSltPgmBak.Format( "%s", szData );

	g_DMCont.m_dmHandlerOpt.GS(SDM11_CategoryName, szData, sizeof(szData));
	GetDlgItem(IDC_EDIT_CATE)->SetWindowText((LPCTSTR)szData);
	m_strCategoryBack.Format( "%s", szData );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDeviceSetup::LoadSltPgm()
{
	m_ListSLTPgm.ResetContent();

	_chdir("SYS_DATA");
	_chdir("HandlerRecipe");
	_chdir(SZ_SLT_PGM_FOLDER);
	CFileFind finder;
	BOOL endf = finder.FindFile();

	int nTmpIndex = 0, nFindIndex = 0;
	char szData[512] = {0,};
	g_DMCont.m_dmHandlerOpt.GS(SDM11_SLT_ProgramName, szData, sizeof(szData));

	while(endf) 
	{
		endf = finder.FindNextFile();
		CString strExt = finder.GetFileName();

		if(!finder.IsDots() && finder.IsDirectory())
		{
			//strExt.Replace(".dat", "");
			m_ListSLTPgm.AddString(strExt);

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

	m_ListSLTPgm.SetCurSel(nFindIndex);
}

void CDeviceSetup::LoadCategory()
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

void CDeviceSetup::LoadTrayFile()
{
	m_ListTrayFile.ResetContent();

	_chdir("SYS_DATA");
	_chdir("HandlerRecipe");
	_chdir(SZ_TRAY_FOLDER);
	CFileFind finder;
	BOOL endf = finder.FindFile();

	int nTmpIndex = 0, nFindIndex = 0;
	char szData[512] = {0,};
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szData, sizeof(szData));

	m_strTrayFileName = szData;

	while(endf) 
	{
		endf = finder.FindNextFile();

		CString strExt = finder.GetFileName();

		// TrayFile의 경우 Package단위로 Folder내부로 관리하기 때문에 예외처리
		if(!finder.IsDots() && finder.IsDirectory())	
		{
			m_ListTrayFile.AddString(strExt);

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

	m_ListTrayFile.SetCurSel(nFindIndex);
}

void CDeviceSetup::LoadCustomer()
{
	char szCurrentDir[512] = {0,};
	char szFileName[512]   = {0,};
	char szData[512]       = {0,};
	::GetCurrentDirectory(sizeof(szCurrentDir), szCurrentDir);
	sprintf_s(szFileName, sizeof(szFileName), "%s\\sys_data\\%s", szCurrentDir, SZ_CUST_DVC_PGM_FILE_NAME);

	CConfigData	 CCustomerName(szFileName);
	int nCount = CCustomerName.GetInt("CUSTOMER" , "COUNT", "0");

	m_ListCustomer.ResetContent();

	int nIndex = 0;
	char szGetRegData[512] = {0,};
	g_DMCont.m_dmHandlerOpt.GS(SDM11_CustomerName, szGetRegData, sizeof(szGetRegData));

	char szSubTitle[256] = {0,};
	for(int i=0; i<nCount; i++)
	{
		ZeroMemory(szSubTitle, sizeof(szSubTitle));
		sprintf_s(szSubTitle, sizeof(szSubTitle), "NAME%03d", i+1);
		CCustomerName.GetString("CUSTOMER", szSubTitle, "", szData);

		m_ListCustomer.AddString((LPCTSTR)szData);

		if(strcmp(szGetRegData, szData) == 0){
			nIndex = i;
		}
	}

	m_ListCustomer.SetCurSel(nIndex);
}

void CDeviceSetup::LoadDeviceName()
{
	char szCurrentDir[512] = {0,};
	char szFileName[512]   = {0,};
	char szData[512]       = {0,};
	::GetCurrentDirectory(sizeof(szCurrentDir), szCurrentDir);
	sprintf_s(szFileName, sizeof(szFileName), "%s\\sys_data\\%s", szCurrentDir, SZ_CUST_DVC_PGM_FILE_NAME);

	CConfigData	 CDeviceName(szFileName);
	int nCount = CDeviceName.GetInt("DEVICE" , "COUNT", "0");

	m_ListDvcName.ResetContent();

	int nIndex = 0;
	char szGetRegData[512] = {0,};
	g_DMCont.m_dmHandlerOpt.GS(SDM11_DeviceName, szGetRegData, sizeof(szGetRegData));

	char szSubTitle[256] = {0,};
	for(int i=0; i<nCount; i++)
	{
		ZeroMemory(szSubTitle, sizeof(szSubTitle));
		sprintf_s(szSubTitle, sizeof(szSubTitle), "NAME%03d", i+1);
		CDeviceName.GetString("DEVICE", szSubTitle, "", szData);

		m_ListDvcName.AddString((LPCTSTR)szData);

		if(strcmp(szGetRegData, szData) == 0){
			nIndex = i;
		}
	}

	m_ListDvcName.SetCurSel(nIndex);
}

void CDeviceSetup::LoadProgramName()
{
	char szCurrentDir[512] = {0,};
	char szFileName[512]   = {0,};
	char szData[512]       = {0,};
	::GetCurrentDirectory(sizeof(szCurrentDir), szCurrentDir);
	sprintf_s(szFileName, sizeof(szFileName), "%s\\sys_data\\%s", szCurrentDir, SZ_CUST_DVC_PGM_FILE_NAME);

	CConfigData	 CPgmName(szFileName);
	int nCount = CPgmName.GetInt("PROGRAM" , "COUNT", "0");

	m_ListPgmName.ResetContent();

	int nIndex = 0;
	char szGetRegData[512] = {0,};
	g_DMCont.m_dmHandlerOpt.GS(SDM11_ProgramName, szGetRegData, sizeof(szGetRegData));

	char szSubTitle[256] = {0,};
	for(int i=0; i<nCount; i++)
	{
		ZeroMemory(szSubTitle, sizeof(szSubTitle));
		sprintf_s(szSubTitle, sizeof(szSubTitle), "NAME%03d", i+1);
		CPgmName.GetString("PROGRAM", szSubTitle, "", szData);

		m_ListPgmName.AddString((LPCTSTR)szData);

		if(strcmp(szGetRegData, szData) == 0){
			nIndex = i;
		}
	}

	m_ListPgmName.SetCurSel(nIndex);
}

void CDeviceSetup::OnBtnGUI()
{
	m_BtnCancel.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnCancel.SetIcon(IDI_BTN_EXIT);
	m_BtnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnSave.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnSave.SetIcon(IDI_BTN_SAVE);
	m_BtnSave.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnExport.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnExport.SetIcon(IDI_BTN_EXPORT);
	m_BtnExport.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnImport.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnImport.SetIcon(IDI_BTN_IMPORT);
	m_BtnImport.SetAlign(CButtonST::ST_ALIGN_VERT);
}

void CDeviceSetup::OnBnClickedButtonSave()
{
	BOOL bLotState = g_DMCont.m_dmEQP.GB(BDM0_SYS_LOT_STATUS);
	if(bLotState == eHandlerLotMode_OnGoing){
		AfxMessageBox("Handler is Running!! Please save after lot end");
		return;
	}

	char szOldTrayFileName[MAX_PATH] = {0,};
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szOldTrayFileName, sizeof(szOldTrayFileName));

	UpdateData(TRUE);

	CString strCustomer = _T(""); 
	CString strDvcName  = _T(""); 
	CString strPgmName  = _T(""); 
	CString strTrayFile = _T(""); 
	CString strSLT_PGM  = _T(""); 
	CString strCategory = _T(""); 

// 	if(strcmp(m_szCustomer,"AMKOR") !=0){
// 		int nIndexCustomer = m_ListCustomer.GetCurSel();
// 		if(nIndexCustomer != LB_ERR){ 
// 			m_ListCustomer.GetText(nIndexCustomer, strCustomer); 
// 		}else{
// 			AfxMessageBox("Customer is not selected!!!!!");
// 		}
// 
// 		int nIndexDvcName = m_ListDvcName.GetCurSel();
// 		if(nIndexDvcName != LB_ERR){ 
// 			m_ListDvcName.GetText(nIndexDvcName, strDvcName); 
// 		}else{
// 			AfxMessageBox("Device is not selected!!!!!");
// 		}
// 
// 		int nIndexPgmName = m_ListPgmName.GetCurSel();
// 		if(nIndexPgmName != LB_ERR){ 
// 			m_ListPgmName.GetText(nIndexPgmName, strPgmName); 
// 		}else{
// 			AfxMessageBox("Program is not selected!!!!!");
// 		}
// 	}

	int nIndexTrayFile = m_ListTrayFile.GetCurSel();
	if(nIndexTrayFile != LB_ERR){ 
		m_ListTrayFile.GetText(nIndexTrayFile, strTrayFile); 
	}else{
		AfxMessageBox("Tray File is not selected!!!!!");
		return;
	}

	if (strcmp(szOldTrayFileName, strTrayFile) != 0 && g_DMCont.m_dmEQP.GN(NDM0_CurLotLoadingCnt) != 0) {
		AfxMessageBox("Please make summary  and change Tray File");
		return;
	}



	int nIndexSLT_PGM = m_ListSLTPgm.GetCurSel();
	if(nIndexSLT_PGM != LB_ERR){ 
		m_ListSLTPgm.GetText(nIndexSLT_PGM, strSLT_PGM); 
	}else{
		AfxMessageBox("SLT Program is not selected!!!!!");
		return;
	}

	int nIndexCategory = m_ListCategory.GetCurSel();
	if(nIndexCategory != LB_ERR){ 
		m_ListCategory.GetText(nIndexCategory, strCategory); 
	}else{
		AfxMessageBox("Category File is not selected!!!!!");
		return;
	}

 	char szRetBuff[MAX_PATH] = {0,};
// 	if(strcmp(m_szCustomer,"AMKOR") != 0){
// 		strcpy_s(szRetBuff, MAX_PATH, (LPSTR)(LPCSTR)strCustomer);
// 		g_DMCont.m_dmHandlerOpt.SS(SDM11_CustomerName , szRetBuff, MAX_PATH);
// 
// 		strcpy_s(szRetBuff, MAX_PATH, (LPSTR)(LPCSTR)strDvcName);
// 		g_DMCont.m_dmHandlerOpt.SS(SDM11_DeviceName , szRetBuff, MAX_PATH);
// 
// 		strcpy_s(szRetBuff, MAX_PATH, (LPSTR)(LPCSTR)strPgmName);
// 		g_DMCont.m_dmHandlerOpt.SS(SDM11_ProgramName , szRetBuff, MAX_PATH);
// 
// 	}

	strcpy_s(szRetBuff, MAX_PATH, (LPSTR)(LPCSTR)strTrayFile);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_TrayFileName , szRetBuff, MAX_PATH);

	strcpy_s(szRetBuff, MAX_PATH, (LPSTR)(LPCSTR)strSLT_PGM);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_SLT_ProgramName , szRetBuff, MAX_PATH);

	strcpy_s(szRetBuff, MAX_PATH, (LPSTR)(LPCSTR)strCategory);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_CategoryName , szRetBuff, MAX_PATH);

	char szCokName[512] = {0,};
	ST_BASIC_INFO   stBasicData, stBeforeBasicData;
	ST_SLT_SYSTEM	stSltSystem;
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));
	g_COK_FileMgr.LoadBasicInfo(szCokName, stBasicData);
	
	stBeforeBasicData = stBasicData;
	
// 	if(strcmp(m_szCustomer,"AMKOR") != 0){
// 		strcpy_s(stBasicData.szCustomerName, sizeof(stBasicData.szCustomerName), strCustomer);
// 		strcpy_s(stBasicData.szDeviceName  , sizeof(stBasicData.szDeviceName)  , strDvcName);
// 		strcpy_s(stBasicData.szPgmName     , sizeof(stBasicData.szPgmName)     , strPgmName);
// 	}

	strcpy_s(stBasicData.szSltPgmName  , sizeof(stBasicData.szSltPgmName)  , strSLT_PGM);
	strcpy_s(stBasicData.szCategoryName, sizeof(stBasicData.szCategoryName), strCategory);
	strcpy_s(stSltSystem.szLastPackageName, sizeof(stSltSystem.szLastPackageName), strTrayFile);

	g_COK_FileMgr.SaveBasicInfo(szCokName, &stBasicData);
	g_COK_FileMgr.SaveSltSysFile(&stSltSystem);

	{	// SJ TODO :: InterLock [Lot 진행 중일 때는 해당 Pkg 변경 불가 해야한다]
		
		if( m_strTrayFileBak != strTrayFile || m_strCategoryBack != strCategory || m_strSltPgmBak != strSLT_PGM )
		{
			CSystemData cSysData;
			ST_SLT_SYSTEM lpstData;
			g_COK_FileMgr.LoadSltSysFile(lpstData);
			cSysData.Initialize(lpstData.szLastPackageName);
			cSysData.UpdateFileDataToSM();
		}
	}
	//g_McComm.CmdOnlyOneCmdNoParam(GUI_RCMD_REQ_PACKAGE_CONVERSION);	
   if(m_strTrayFileBak != strTrayFile){
		g_McComm.CmdOnlyOneCmdNoParam(GUI_RCMD_REQ_PACKAGE_CONVERSION);
		m_strTrayFileBak = strTrayFile;
	}		//GUI Data Change Log

   if( m_strCategoryBack != strCategory ) {
	   g_McComm.CmdBinCategoryRefresh();
	   m_strCategoryBack = strCategory;
   }

   if( m_strSltPgmBak != strSLT_PGM ) {
	   g_McComm.CmdDefaultBinCategoryRefresh();
	   m_strSltPgmBak = strSLT_PGM;
   }

	BOOL bDotMatrix = g_DMCont.m_dmEQP.GB(BDM0_MD_DOTMATRIX);
	if (bDotMatrix == eOPTION_USE)
		SetDotMatrixUpdate();

	std::vector<std::pair<CString,std::pair<CString,CString>>> vData;  // vData.first = stCokData, vData.second.first = BeforeData vData.second.second = AfterData
	
	stBeforeBasicData.IsSameDeviceSetup(stBasicData, vData);
	
	if(m_strTrayFileName != strTrayFile)
	{
		MakeLog("[MENU = Device Setup, DATA NAME = Tray File][BEFORE=%s, AFTER=%s]",m_strTrayFileName, strTrayFile);
		LotInfoDataClear();
	}
	if(!(vData.empty()))
	{
		DeviceSetupChangeLog(vData);
	}
	
	m_strTrayFileName = strTrayFile;
//	CString e = .iscmp(ddcf);
}


void CDeviceSetup::OnBnClickedButtonImpo()
{
	MakeLog("Import Button Click");
	ITEMIDLIST *pidBrowse;
	char		szToPath[MAX_PATH] ={0,};
	char        szFromPath[MAX_PATH] ={0,};
	BROWSEINFO	BrInfo;

	int nIndexTrayFile = m_ListTrayFile.GetCurSel();

	char szTemp[512]={0,};
	::GetCurrentDirectory(sizeof(szTemp), szTemp);
	sprintf_s(szToPath, sizeof(szToPath), "%s\\sys_data\\HandlerRecipe\\%s", szTemp, SZ_TRAY_FOLDER);

	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.pidlRoot = NULL;

	memset(&BrInfo,0,sizeof(BrInfo));
	BrInfo.pszDisplayName = szFromPath;
	BrInfo.lpszTitle = _T("Select Directory");
	BrInfo.ulFlags = BIF_RETURNONLYFSDIRS|BIF_NEWDIALOGSTYLE;

	pidBrowse = ::SHBrowseForFolderA(&BrInfo);

	//폴더 선택 완료 후 진행
	if(pidBrowse != NULL)
	{
		SHGetPathFromIDList(pidBrowse,szFromPath);
		int nFolderErr = ChkCopyFolder(szFromPath);
		if(nFolderErr == 0){
			CString strFolderName =_T("");
			if(ChkCopyFolderName(szFromPath,strFolderName) != 0){
				int nYesNo = AfxMessageBox("Duplicate name exists.\nDo you want to overwrite it?",MB_YESNO);
				if(nYesNo !=  IDYES){
					return;
				}else{
					MakeLog("Tray File overwrite 'Yes' Click [Tray File Name : %s]",strFolderName);
				}
			} 
			int nErr = CopyFolder(szFromPath,szToPath);
			if(nErr != 0){// 0 -> ERR_NO_ERROR
				AfxMessageBox("Tray File Import is Fail");
				MakeLog("Tray File Import Fail [Tray File Name : %s]",strFolderName);
			}else{
				LoadTrayFile();
				char szData[MAX_PATH] ={0,};
				g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szData, sizeof(szData));
				if(strFolderName.Compare(szData) ==0){
					g_McComm.CmdOnlyOneCmdNoParam(GUI_RCMD_REQ_PACKAGE_CONVERSION);	
				}
				MakeLog("Tray File Import [Tray File Name : %s]",strFolderName);
			}
		}
	}
}

void CDeviceSetup::OnBnClickedButtonExpo()
{
	MakeLog("Export Button Click");
	ITEMIDLIST *pidBrowse;
	char		szToPath[MAX_PATH] ={0,};
	char        szFromPath[MAX_PATH] ={0,};
	BROWSEINFO	BrInfo;
	
	int nIndexTrayFile = m_ListTrayFile.GetCurSel();
	CString strTrayFile = _T(""); 

	if(nIndexTrayFile != LB_ERR){ 
		m_ListTrayFile.GetText(nIndexTrayFile, strTrayFile); 
	}

	char szTemp[512]={0,};
	::GetCurrentDirectory(sizeof(szTemp), szTemp);
	sprintf_s(szFromPath, sizeof(szFromPath), "%s\\sys_data\\HandlerRecipe\\%s\\%s", szTemp, (LPCTSTR)SZ_TRAY_FOLDER, (LPCTSTR)strTrayFile);

	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.pidlRoot = NULL;

	memset(&BrInfo,0,sizeof(BrInfo));
	BrInfo.pszDisplayName = szToPath;
	BrInfo.lpszTitle = _T("Select Directory");
	BrInfo.ulFlags = BIF_RETURNONLYFSDIRS|BIF_NEWDIALOGSTYLE;

	pidBrowse = ::SHBrowseForFolderA(&BrInfo);

	//폴더 선택 완료 후 진행
	if(pidBrowse != NULL)
	{
		SHGetPathFromIDList(pidBrowse,szToPath);
		int nErr = CopyFolder(szFromPath,szToPath);
		if(nErr != 0){// 0 -> ERR_NO_ERROR
			AfxMessageBox("Tray File Export is Fail");
			MakeLog("Tray File Export Fail [Tray File Name : %s]",strTrayFile);
		}else{
			MakeLog("Tray File Export [Tray File Name : %s]",strTrayFile);
		}
	}
}

void CDeviceSetup::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialog::OnCancel();
}


void CDeviceSetup::OnBnClickedButtonRegCustom()
{
	CRegisterCustomer DO;
	DO.DoModal();

	LoadCustomer();
}


void CDeviceSetup::OnBnClickedButtonRegDvc()
{
	CRegisterDeviceName DO;
	DO.DoModal();

	LoadDeviceName();
}


void CDeviceSetup::OnBnClickedButtonRegPgm()
{
	CRegisterPgmName DO;
	DO.DoModal();

	LoadProgramName();
}


// void CDeviceSetup::OnBnClickedButtonCustomModify()
// {
// 	
// }

/*=============================================================================
  AUTHOR   : 손정일  [2017-01-02]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : Amkor Device Setup Dialog 로 변경 되기 위해
==============================================================================*/

void CDeviceSetup::InitalizeDialog_Amkor()
{
	GetDlgItem(IDC_STATIC_LOTINFO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_CUSTOMER)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_REG_CUSTOM)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_LIST_CUSTOM)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_CUSTOM)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_DVC_NAME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_REG_DVC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_DVC_NAME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_LIST_DVC_NAME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_PGM_NAME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_REG_PGM)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_PGM_NAME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_LIST_PGM_NAME)->ShowWindow(SW_HIDE);	

	std::vector<int> vControl;
	vControl.push_back(IDC_STATIC_TRAYFILE);
	vControl.push_back(IDC_STATIC_TRAYFILE2);
	vControl.push_back(IDC_STATIC_CATE);
	vControl.push_back(IDC_EDIT_TRAYFILE);
	vControl.push_back(IDC_EDIT_SLT_PGM);
	vControl.push_back(IDC_EDIT_CATE);
	vControl.push_back(IDC_LIST_TRAYFILE);
	vControl.push_back(IDC_LIST_SLT_PGM);
	vControl.push_back(IDC_LIST_CATE);
	vControl.push_back(IDC_BUTTON_SAVE);
	vControl.push_back(IDC_BUTTON_IMPO);
	vControl.push_back(IDC_BUTTON_EXPO);
	vControl.push_back(IDCANCEL);
	vControl.push_back(IDC_STATIC_DVC_SETUP);
	ChangeDialogSize(vControl);
}

/*=============================================================================
  AUTHOR   : 손정일  [2017-01-02]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : Amkor Device Setup Dialog 로 변경 되기 위해
==============================================================================*/
void CDeviceSetup::ChangeDialogSize(std::vector<int> vControl)
{
	for(int i=0; i<(int)vControl.size(); i++){
		HWND hwndBox = ::GetDlgItem(this->m_hWnd, vControl[i]);
		RECT rc1;
		::GetWindowRect(hwndBox, &rc1);

		GetDlgItem(vControl[i])->SetWindowPos(NULL,rc1.left,(rc1.top-530),(rc1.right - rc1.left),(rc1.bottom - rc1.top),0);
	}
	HWND hwndBox = ::GetDlgItem(this->m_hWnd, IDD_DIALOG_DEVICE_SETUP);
	RECT rc1;
	::GetWindowRect(this->m_hWnd, &rc1);

	MoveWindow(0,0,rc1.right,(rc1.bottom -500));
}

void CDeviceSetup::LotInfoDataClear()
{
	g_DMCont.m_dmHandlerOpt.SS(SDM11_LotNo,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_CustomerName,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_DeviceName,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_ScheduleNo,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_RetestCode,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_ProgramName,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_Operation,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_Temp,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_QTY,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_ART,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_TP_REV,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_Soak_Time,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_FUSE,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_HANDLER,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_OperatorID,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_LOAD_BD_NO,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_SOCKER_ID,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_C_KIT_ID,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_FOUNDRY_LOTID,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_Assembly_startdate,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_Assembly_enddate,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_ASSY_vendor_lotid,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_TEST_vendor_lotid,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_BOT_SUB_Vendor,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_Nickname,"",MAX_PATH);
}

void CDeviceSetup::MakeLog(LPCTSTR fmt, ...)
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


void CDeviceSetup::DeviceSetupChangeLog(std::vector<std::pair<CString,std::pair<CString,CString>>> vData)
{
	for(int i = 0 ; i< (int)vData.size(); i++)
	{
		std::pair<CString,std::pair<CString,CString>> p;

		p = vData[i];

		MakeLog("[MENU = Device Setup, DATA NAME = %s ][BEFORE=%s, AFTER=%s]", p.first, p.second.first, p.second.second);

	}
	
}

int CDeviceSetup::CopyFolder(CString strFrom, CString strTo)
{
	SHFILEOPSTRUCT shfo;
	int nRet;
	char* pszFrom = new char[strFrom.GetLength()+2];
	char* pszTo = new char[strTo.GetLength()+2];
	memset( pszFrom, NULL, strFrom.GetLength() + 2 );
	memset( pszTo, NULL, strTo.GetLength() + 2 );

	//EmptyDirectory(szTo);
	SetFileAttributes(strTo, FILE_ATTRIBUTE_ARCHIVE);
	RemoveDirectory(strTo);

	strcpy(pszFrom, strFrom);
	strcpy(pszTo, strTo);

	shfo.hwnd = NULL;
	shfo.wFunc = FO_COPY;
	shfo.pFrom = pszFrom;
	shfo.pTo = pszTo;
	shfo.fFlags =FOF_NOERRORUI | FOF_SILENT | FOF_NOCONFIRMMKDIR |FOF_NOCONFIRMATION ;
	shfo.lpszProgressTitle = NULL;

	nRet = SHFileOperation(&shfo);

	delete[] pszFrom;
	delete[] pszTo;

	return nRet;
}
// Import 할 Folder 안에 File이 있는 지 확인
int CDeviceSetup::ChkCopyFolder(CString strFromPath)
{
	CString strFilePath = _T("");
	CString strErrMsg = _T("");
	strFilePath.Format("%s\\BasicAction.dat", (LPCTSTR)strFromPath);
	if(PathFileExists(strFilePath) != TRUE){
		strErrMsg.Format("Unable to Import because there is not [%s] Folder in BasicAction.dat File", (LPCTSTR)strFromPath);
		AfxMessageBox(strErrMsg);
		return 1;
	}
	strFilePath.Format("%s\\CokInfo.dat", (LPCTSTR)strFromPath);
	if(PathFileExists(strFilePath) != TRUE){
		strErrMsg.Format("Unable to Import because there is not [%s] Folder in CokInfo.dat File", (LPCTSTR)strFromPath);
		AfxMessageBox(strErrMsg);
		return 1;
	}
	strFilePath.Format("%s\\Teaching.dat", (LPCTSTR)strFromPath);
	if(PathFileExists(strFilePath) != TRUE){
		strErrMsg.Format("Unable to Import because there is not [%s] Folder in Teaching.dat File", (LPCTSTR)strFromPath);
		AfxMessageBox(strErrMsg);
		return 1;
	}
	strFilePath.Format("%s\\UserOffset.dat", (LPCTSTR)strFromPath);
	if(PathFileExists(strFilePath) != TRUE){
		strErrMsg.Format("Unable to Import because there is not [%s] Folder in UserOffset.dat File", (LPCTSTR)strFromPath);
		AfxMessageBox(strErrMsg);
		return 1;
	}
	return 0;
}

int CDeviceSetup::ChkCopyFolderName(CString strFromPath,CString &strFolderName)
{
	CString strTok = _T("");
	CString *strTemp = new CString[strFromPath.GetLength()+1];
	int nCnt =0;
	while(AfxExtractSubString(strTok, strFromPath, nCnt, '\\')) // 문자를 잘라준다. (AfxExtractSubString = strTok)
		strTemp[nCnt++] = strTok;  

	CString strFromFile = strTemp[nCnt-1];
	CString strTrayFile = _T("");
	strFolderName = strFromFile;
	for(int i=0; i<m_ListTrayFile.GetCount(); i++){
		m_ListTrayFile.GetText(i, strTrayFile);
		if(strFromFile.Compare(strTrayFile) == 0){
			delete[] strTemp;
			return 1;
		}
	}
	delete[] strTemp;

	return 0;
}
