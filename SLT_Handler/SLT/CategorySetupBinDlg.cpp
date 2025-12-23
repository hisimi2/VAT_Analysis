// CategorySetupBinDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "CategorySetupBinDlg.h"
#include "afxdialogex.h"
#include <algorithm>
#include <set>
#include <map>
using namespace std;


// CCategorySetupBinDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCategorySetupBinDlg, CDialog)

CCategorySetupBinDlg::CCategorySetupBinDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCategorySetupBinDlg::IDD, pParent)
{
	m_strCateName = _T("");
	m_nMaxRowNum = 0;

	m_nProjectName = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
}

CCategorySetupBinDlg::~CCategorySetupBinDlg()
{
}

void CCategorySetupBinDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL, m_BtnCancel);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_BtnSave);
	DDX_Control(pDX, IDC_BUTTON_DEL , m_BtnDelete);
	DDX_Control(pDX, IDC_BUTTON_INSERT , m_BtnInsert);
	DDX_Control(pDX, IDC_SPREAD_OUT1, m_SprdBinModfiy);
	DDX_Control( pDX, IDC_BUTTON_ONLINE_DEFAULT, m_BtnDefault );
}


BEGIN_MESSAGE_MAP(CCategorySetupBinDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CCategorySetupBinDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CCategorySetupBinDlg::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_INSERT, &CCategorySetupBinDlg::OnBnClickedButtonInsert)
	ON_BN_CLICKED( IDC_BUTTON_ONLINE_DEFAULT, &CCategorySetupBinDlg::OnBnClickedButtonOnlineDefault )
END_MESSAGE_MAP()


// CCategorySetupBinDlg 메시지 처리기입니다.


BOOL CCategorySetupBinDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strTitle = m_strCateName + _T(" Category Setup");
	SetWindowText(strTitle);

	OnBtnGUI();
	UpdateSpread();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CCategorySetupBinDlg::UpdateSpread()
{
	int nCount = m_nMaxRowNum = m_stCateData.nIndexCount;
	m_SprdBinModfiy.SetMaxCols(/*5*/5);
	m_SprdBinModfiy.SetMaxRows(m_nMaxRowNum);


	if (DEF_PROJECT_WATT ==m_nProjectName)
	{
		m_SprdBinModfiy.SetTextMatrix(0, 5, "HW Bin(Tester)");
	}
	else {
		m_SprdBinModfiy.SetTextMatrix(0, 5, "Error Code");
	}

	for(int i=0; i<nCount; i++)
	{
		m_SprdBinModfiy.SetTextMatrix(i+1,1,m_stCateData.aCateInfo[i].szSWBin);
		m_SprdBinModfiy.SetTextMatrix(i+1,2,m_stCateData.aCateInfo[i].szHWBin);
		m_SprdBinModfiy.SetTextMatrix(i+1,3,m_stCateData.aCateInfo[i].szDesc);
		m_SprdBinModfiy.SetTextMatrix(i+1,4,m_stCateData.aCateInfo[i].szPassFail);
		m_SprdBinModfiy.SetTextMatrix(i+1,5, DEF_PROJECT_WATT == m_nProjectName ? m_stCateData.aCateInfo[i].szHWBin_Tester : m_stCateData.aCateInfo[i].szCode);
	}
}

void CCategorySetupBinDlg::OnBtnGUI()
{
	m_BtnCancel.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnCancel.SetIcon(IDI_BTN_EXIT);
	m_BtnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnInsert.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnInsert.SetIcon(IDI_BTN_NEW);
	m_BtnInsert.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnDelete.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnDelete.SetIcon(IDI_BTN_CLEAR);
	m_BtnDelete.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnSave.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnSave.SetIcon(IDI_BTN_SAVE);
	m_BtnSave.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnDefault.SetShade( CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB( 55, 55, 255 ) );
	m_BtnDefault.SetAlign( CButtonST::ST_ALIGN_VERT );
}

void CCategorySetupBinDlg::OnBnClickedButtonSave()
{
	BOOL bLotState = g_DMCont.m_dmEQP.GB(BDM0_SYS_LOT_STATUS);
	char szData[MAX_PATH] = {0,};
	g_DMCont.m_dmHandlerOpt.GS(SDM11_CategoryName, szData, sizeof(szData));
	if(bLotState == eHandlerLotMode_OnGoing && m_strCateName.Compare(szData) == 0){
		AfxMessageBox("Handler is Running!! Please save after lot end");
		return;
	}

	ST_CATE_INFO stTempCateData;
	int nIndexCount = 0;
	int nEmptyCellNum = 0;
	BOOL bEmptyTextFlag = FALSE;
	BOOL bPassFailTextFlag = FALSE;
	BOOL bPassFailTextNGFlag = FALSE;
	BOOL bHwBinNGFalg = FALSE;
	BOOL bPassFailNumberFlag = FALSE;
	CString strTmpData[5];
	CString str[5];

	for(int i=0; i<m_nMaxRowNum; i++)
	{
		for(int j=0; j<5; j++){
			strTmpData[j].Empty();
			str[j].Empty();
		}

		strTmpData[0] = m_SprdBinModfiy.GetTextMatrix(i+1,1);
		strTmpData[1] = m_SprdBinModfiy.GetTextMatrix(i+1,2);
		//strTmpData[2] = m_SprdBinModfiy.GetTextMatrix(i+1,3);
		strTmpData[2] = m_SprdBinModfiy.GetTextMatrix(i+1,3);
		strTmpData[3] = m_SprdBinModfiy.GetTextMatrix(i+1,4);
		strTmpData[4] = m_SprdBinModfiy.GetTextMatrix(i + 1, 5);

		// Err Str 대문자!
		strTmpData[2].MakeUpper();

		str[0] = strTmpData[0];
		str[1] = strTmpData[1];
		//str[2] = strTmpData[2];
		str[2] = strTmpData[2];
		str[3] = strTmpData[3];
		str[4] = strTmpData[4];

		str[0].Replace(" ","");
		str[1].Replace(" ","");
		//str[2].Replace(" ","");
		str[2].Replace(" ","");
		str[3].Replace(" ","");
		str[4].Replace(" ", "");


		if(  str[0].IsEmpty() == TRUE && str[1].IsEmpty() == TRUE && str[2].IsEmpty() == TRUE 
			&& str[3].IsEmpty() == TRUE && str[4].IsEmpty() == TRUE)
		{
			nEmptyCellNum += 1;
		}
		else if(  str[0].IsEmpty() == TRUE || str[1].IsEmpty() == TRUE || str[2].IsEmpty() == TRUE 
	      || str[3].IsEmpty() == TRUE || str[4].IsEmpty() == TRUE)
		{
			bEmptyTextFlag = TRUE;
			//break;
		}

		int nLen = strTmpData[4].GetLength();
		if (nLen > 4)
		{
			AfxMessageBox("Error Code can only be Length 4!!");
			return;
		}

		sprintf_s(stTempCateData.aCateInfo[i-nEmptyCellNum].szSWBin   , sizeof(stTempCateData.aCateInfo[i-nEmptyCellNum].szSWBin)	,"%s", strTmpData[0]);
		sprintf_s(stTempCateData.aCateInfo[i-nEmptyCellNum].szHWBin   , sizeof(stTempCateData.aCateInfo[i-nEmptyCellNum].szHWBin)	,"%s", strTmpData[1]);
		sprintf_s(stTempCateData.aCateInfo[i-nEmptyCellNum].szDesc    , sizeof(stTempCateData.aCateInfo[i-nEmptyCellNum].szDesc)	,"%s", strTmpData[2]);
		sprintf_s(stTempCateData.aCateInfo[i-nEmptyCellNum].szPassFail, sizeof(stTempCateData.aCateInfo[i-nEmptyCellNum].szPassFail),"%s", strTmpData[3]);	
		sprintf_s(DEF_PROJECT_WATT == m_nProjectName? stTempCateData.aCateInfo[i - nEmptyCellNum].szHWBin_Tester :stTempCateData.aCateInfo[i-nEmptyCellNum].szCode    , sizeof(stTempCateData.aCateInfo[i-nEmptyCellNum].szCode)	,"%s", strTmpData[4]);
		nIndexCount++;
	}

	if(bEmptyTextFlag == TRUE)
	{
		AfxMessageBox("Intermediate cell is not empty...!!");
		return;
	}

	stTempCateData.nIndexCount = nIndexCount;

	int nSortType = g_DMCont.m_dmEQP.GN( NDM0_SortType);// SortingType 을 가져온다
	std::vector<std::pair<CString,std::pair<CString,CString>>>  vData;
	m_stCateData.IsSameCategorySetup(&stTempCateData, vData, nSortType/*, m_stCateData.nIndexCount, nIndexCount*/); // 변경된것이 있는지 확인을 한다

	m_stCateData = stTempCateData;
	
	for (int i = 0; i < m_nMaxRowNum; i++)
	{
		// PASS/FAIL은 0 or 1만 존재
		if (atoi(m_stCateData.aCateInfo[i].szPassFail) != 0 && atoi(m_stCateData.aCateInfo[i].szPassFail) != 1)
			bPassFailTextFlag = TRUE;
		
		if (strcmp(m_stCateData.aCateInfo[i].szDesc, "PASS") == 0 && atoi(m_stCateData.aCateInfo[i].szPassFail) != 1)
			bPassFailTextNGFlag = TRUE;
		else if(strcmp(m_stCateData.aCateInfo[i].szDesc, "PASS") != 0 && atoi(m_stCateData.aCateInfo[i].szPassFail) == 1)
			bPassFailTextNGFlag = TRUE;

		int nHwbin = atoi(m_stCateData.aCateInfo[i].szHWBin);
		if(nHwbin <= 0 || nHwbin > 4)
			bHwBinNGFalg = TRUE;
	}

	if (bPassFailTextFlag == TRUE) {
		AfxMessageBox("Pass/Fail can only be 1 or 0!!");
		return;
	}

	if (bPassFailTextNGFlag == TRUE) {
		AfxMessageBox("Pass can only be 1!!");
		return;
	}

	if (bHwBinNGFalg == TRUE) {
		AfxMessageBox("HW Bin can only be 1 ~ 4!!");
		return;
	}
	
	//int nChkCateErr = checkCategoryInterlock();
	//if (nChkCateErr == TRUE)
	//{
	//	return;
	//}

	g_COK_FileMgr.SaveCategoryInfo((LPSTR)(LPCTSTR)m_strCateName, &m_stCateData);
	g_McComm.CmdOnlyOneCmdNoParam(GUI_RCMD_REQ_BIN_CATEGORY_REFRESH);

	if(!(vData.empty())) // 변경된것이 있으면
	{
		CategorySetupChangeLog(vData);
	}
}

void CCategorySetupBinDlg::OnBnClickedButtonDel()
{
	m_nMaxRowNum -= 1;
	if(m_nMaxRowNum < 1){
		m_nMaxRowNum = 1;
		return;
	}
	int nSelectSel = m_SprdBinModfiy.GetSelBlockRow();
	m_SprdBinModfiy.DeleteRows(nSelectSel,1);

	m_SprdBinModfiy.SetMaxRows(m_nMaxRowNum);
	m_SprdBinModfiy.SetReDraw(TRUE);
}


void CCategorySetupBinDlg::OnBnClickedButtonInsert()
{
	int nSelectSel = m_SprdBinModfiy.GetSelBlockRow();
	m_nMaxRowNum += 1;
	m_SprdBinModfiy.InsertRows(m_nMaxRowNum,1);
	
	m_SprdBinModfiy.SetMaxRows(m_nMaxRowNum);
	m_SprdBinModfiy.SetReDraw(TRUE);
}

void CCategorySetupBinDlg::CategorySetupChangeLog(std::vector<std::pair<CString,std::pair<CString,CString>>> vData)
{
	int nSortType = g_DMCont.m_dmEQP.GN( NDM0_SortType);// SortingType 을 가져온다

	for(int i = 0 ; i< (int)vData.size(); i++)
	{
		std::pair<CString,std::pair<CString,CString>> p;

		p = vData[i];

		if(p.first == "ADD" || p.first == "Delete") {
			MakeLog("[MENU = Category Setup, CATEGORY = %s ][ %s  %s ]",m_strCateName, p.second.second, p.first);
		} else {
			MakeLog("[MENU = Category Setup, CATEGORY = %s][BEFORE=%s, AFTER=%s]",m_strCateName, p.second.first, p.second.second);
		}
	}
}

void CCategorySetupBinDlg::MakeLog(LPCTSTR fmt, ...)
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

int CCategorySetupBinDlg::checkCategoryInterlock()
{
	//ST_CATE_INFO stTempCateData;
	BOOL bHWBinTextFlag = FALSE;
	BOOL bPassFailTextFlag = FALSE;
	BOOL bPASSTextFlag = FALSE;
	BOOL bFAILTextFlag = FALSE;
	BOOL bErrorStringFlag = FALSE;
	BOOL bSWBinTextFlag = FALSE;
	BOOL bHWBinDataFlag = FALSE;
	BOOL bSWBinID = FALSE;

	vector<char*> vErrorStr;
	set<int> sID;
	pair<set<int>::iterator, bool > pr;
	vector<int> vHWBinData1, vHWBinData2;

	for (int i = 0; i < m_nMaxRowNum; i++)
	{
		// HW Bin은 4이상 입력 불가
		if (atoi(m_stCateData.aCateInfo[i].szHWBin) > 4 || atoi(m_stCateData.aCateInfo[i].szHWBin) <= 0) {
			bHWBinTextFlag = TRUE;
		}

		// PASS/FAIL은 0 or 1만 존재
		if (strcmp(m_stCateData.aCateInfo[i].szDesc, "HE") == 0 || strcmp(m_stCateData.aCateInfo[i].szDesc, "2D_READ_ERROR") == 0 || strcmp( m_stCateData.aCateInfo[ i ].szDesc, "TEST_TIME_OUT" ) == 0 ) {
			if (atoi(m_stCateData.aCateInfo[i].szPassFail) != 0)
			{
				bPassFailTextFlag = TRUE;
			}
		}
		else if (strcmp(m_stCateData.aCateInfo[i].szDesc, "FAIL") == 0 && atoi(m_stCateData.aCateInfo[i].szPassFail) != 0) {
			bFAILTextFlag = TRUE;
		}
		else if (strcmp(m_stCateData.aCateInfo[i].szDesc, "PASS") == 0 && atoi(m_stCateData.aCateInfo[i].szPassFail) != 1) {
			bPASSTextFlag = TRUE;
		}
		else {
			if (atoi(m_stCateData.aCateInfo[i].szPassFail) != 0 && atoi(m_stCateData.aCateInfo[i].szPassFail) != 1) {
				bPassFailTextFlag = TRUE;
			}
		}

		// Pass -> 1 / Fail -> 0
		if (strcmp(m_stCateData.aCateInfo[i].szDesc, "PASS") == 0 && atoi(m_stCateData.aCateInfo[i].szSWBin) != 1) {
			bSWBinTextFlag = TRUE;
		}
		else if (strcmp(m_stCateData.aCateInfo[i].szDesc, "FAIL") == 0 && atoi(m_stCateData.aCateInfo[i].szSWBin) != 0) {
			bSWBinTextFlag = TRUE;
		}

		// SW Bin ID 중복되면 안됨
		pr = sID.insert(atoi(m_stCateData.aCateInfo[i].szSWBin));
		if (pr.second == FALSE) { bSWBinID = TRUE; }

		// Pass/Fail의 HW Bin이 동일하면 안됨
		if (atoi(m_stCateData.aCateInfo[i].szPassFail) == 1) {
			vHWBinData1.push_back(atoi(m_stCateData.aCateInfo[i].szHWBin));
		}
		else if (atoi(m_stCateData.aCateInfo[i].szPassFail) == 0) {
			vHWBinData2.push_back(atoi(m_stCateData.aCateInfo[i].szHWBin));
		}

		// 필수 ErrStr 존재여부
		vErrorStr.push_back(m_stCateData.aCateInfo[i].szDesc);
	}

	if (bHWBinTextFlag == TRUE) {
		AfxMessageBox("HW Bin can only be entered from 1 to 4!!");
		return TRUE;
	}

	if (bPassFailTextFlag == TRUE) {
		AfxMessageBox("Pass/Fail can only be 1 or 0!!");
		return TRUE;
	}

	if (bSWBinTextFlag == TRUE) {
		AfxMessageBox("The SW Bin of the PASS must be 1 and the SW Bin of the FAIL must be 0!!");
		return TRUE;
	}

	if (bSWBinID == TRUE) {
		AfxMessageBox("Duplicate SW ID");
		return TRUE;
	}

	if (bPASSTextFlag == TRUE) {
		AfxMessageBox("PASS must be 1!!");
		return TRUE;
	}

	if (bFAILTextFlag == TRUE) {
		AfxMessageBox("FAIL must be 0!!");
		return TRUE;
	}

	for (vector<int>::size_type i = 0; i < vHWBinData2.size(); i++) {
		auto iter3 = find(vHWBinData1.begin(), vHWBinData1.end(), vHWBinData2[i]);
		if (iter3 != vHWBinData1.end()) {
			bHWBinDataFlag = TRUE;
		}
	}

	if (bHWBinDataFlag == TRUE) {
		AfxMessageBox("HW Bin of Pass and Fail is equal!!");
		return TRUE;
	}

	//auto iter = find_if(vErrorStr.begin(), vErrorStr.end(),
	//	[](char* errorString) -> bool
	//{
	//	if (strcmp(errorString, "HE") == 0)
	//	{
	//		return TRUE;
	//	}
	//	else { return FALSE; }
	//});

	auto iter1 = find_if(vErrorStr.begin(), vErrorStr.end(),
		[](char* errorString) -> bool
	{
		if (strcmp(errorString, "2D_READ_ERROR") == 0)
		{
			return TRUE;
		}
		else { return FALSE; }
	});

	auto iter2 = find_if(vErrorStr.begin(), vErrorStr.end(),
		[](char* errorString) -> bool
	{
		if (strcmp(errorString, "PASS") == 0)
		{
			return TRUE;
		}
		else { return FALSE; }
	});

	auto iter3 = find_if(vErrorStr.begin(), vErrorStr.end(),
		[](char* errorString) -> bool
	{
		if (strcmp(errorString, "FAIL") == 0)
		{
			return TRUE;
		}
		else { return FALSE; }
	});

	//auto iter4 = find_if( vErrorStr.begin(), vErrorStr.end(),
	//	[]( char* errorString ) -> bool
	//{
	//	if( strcmp( errorString, "TEST_TIME_OUT" ) == 0 )
	//	{
	//		return TRUE;
	//	}
	//	else { return FALSE; }
	//} );

	if (/*iter == vErrorStr.end() || */iter1 == vErrorStr.end() || iter2 == vErrorStr.end() || iter3 == vErrorStr.end() /*|| iter4 == vErrorStr.end()*/) {
		AfxMessageBox("2D_READ_ERROR/PASS/FAIL are not existed!!");
		return TRUE;
	}

	return FALSE;
}

void CCategorySetupBinDlg::OnBnClickedButtonOnlineDefault()
{
	for( int i = 0; i <= m_nMaxRowNum; i++ ) {
		m_SprdBinModfiy.DeleteRows( i, 1 );
	}

	char szCompany[16] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));
	int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);

	

	if (strcmp(szCompany, DEF_COMPANY_AMKOR) != 0) {
		m_nMaxRowNum = 5;
	}
	else
		m_nMaxRowNum = 4;

	m_SprdBinModfiy.SetMaxCols( 5 );
	m_SprdBinModfiy.SetMaxRows(m_nMaxRowNum);

	m_SprdBinModfiy.SetTextMatrix( 1, 1, _T( "1" ) );
	m_SprdBinModfiy.SetTextMatrix( 1, 2, _T( "1" ) );
	m_SprdBinModfiy.SetTextMatrix( 1, 3, _T( "PASS" ) );
	m_SprdBinModfiy.SetTextMatrix( 1, 4, _T( "1" ) );
	m_SprdBinModfiy.SetTextMatrix( 1, 5, _T("0")); //Error Code or HW Bin (Tester)

	m_SprdBinModfiy.SetTextMatrix( 2, 1, _T( "0" ) );
	m_SprdBinModfiy.SetTextMatrix( 2, 2, _T( "2" ) );
	m_SprdBinModfiy.SetTextMatrix( 2, 3, _T( "FAIL" ) );
	m_SprdBinModfiy.SetTextMatrix( 2, 4, _T( "0" ) );
	m_SprdBinModfiy.SetTextMatrix(2, 5, _T("1")); //Error Code or HW Bin (Tester)

	m_SprdBinModfiy.SetTextMatrix( 3, 1, _T( "2" ) );
	m_SprdBinModfiy.SetTextMatrix( 3, 2, _T( "3" ) );
	m_SprdBinModfiy.SetTextMatrix( 3, 3, _T( "2D_READ_ERROR" ) );
	m_SprdBinModfiy.SetTextMatrix( 3, 4, _T( "0" ) );
	m_SprdBinModfiy.SetTextMatrix(3, 5, _T("2")); //Error Code or HW Bin (Tester)

	m_SprdBinModfiy.SetTextMatrix(4, 1, _T("97"));
	m_SprdBinModfiy.SetTextMatrix(4, 2, _T("4"));
	m_SprdBinModfiy.SetTextMatrix(4, 3, _T("TEST_TIME_OUT"));
	m_SprdBinModfiy.SetTextMatrix(4, 4, _T("0"));
	m_SprdBinModfiy.SetTextMatrix(4, 5, _T("97")); //Error Code or HW Bin (Tester)

	if (strcmp(szCompany, DEF_COMPANY_AMKOR) != 0) {
		m_SprdBinModfiy.SetTextMatrix(5, 1, _T("95"));
		m_SprdBinModfiy.SetTextMatrix(5, 2, _T("4"));
		m_SprdBinModfiy.SetTextMatrix(5, 3, _T("TEST_ALARM"));
		m_SprdBinModfiy.SetTextMatrix(5, 4, _T("0"));
		m_SprdBinModfiy.SetTextMatrix(5, 5, _T("95")); //Error Code or HW Bin (Tester)
	}
}
