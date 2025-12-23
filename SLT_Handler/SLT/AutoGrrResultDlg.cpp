// AutoGrrResultDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "AutoGrrResultDlg.h"
#include "afxdialogex.h"

// CAutoGrrResultDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAutoGrrResultDlg, CDialog)

CAutoGrrResultDlg::CAutoGrrResultDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAutoGrrResultDlg::IDD, pParent)
{
	m_nAutoGrrTestCount = 0;
	m_nAutoGrrUnitCount = 0;
	m_nBeforeDataCnt    = 0;
	m_nDataCnt          = 0;
	m_nTestSiteNum		= 0;
}

CAutoGrrResultDlg::~CAutoGrrResultDlg()
{
	m_List.m_fnClear();		
}

void CAutoGrrResultDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPREAD_GRR_RESULT, m_SprdResult);
	DDX_Control(pDX, IDC_SPREAD_GRR_RESULT_STATION, m_SprdStationPF);
	//DDX_Control(pDX, IDC_SPREAD_GRR_RESULT2, m_SprdStationPF);
	DDX_Control(pDX, IDC_COMBO_STATION_NO, m_CtrlCombo);
}


BEGIN_MESSAGE_MAP(CAutoGrrResultDlg, CDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDCANCEL, &CAutoGrrResultDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, &CAutoGrrResultDlg::OnBnClickedButtonRefresh)
	ON_CBN_SELCHANGE(IDC_COMBO_STATION_NO, &CAutoGrrResultDlg::OnBnClickedCombo)
END_MESSAGE_MAP()


BOOL CAutoGrrResultDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	Initialize();
	
	m_CtrlCombo.SetCurSel(0);
	OnBnClickedCombo();

	return TRUE;
}

void CAutoGrrResultDlg::Initialize()
{
	m_nAutoGrrTestCount = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoGrrTestCount);
	m_nAutoGrrUnitCount = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoGrrUnitCount);
	m_nTestSiteNum = 0;
	//int nGrrSiteSocketOnOff[eMaxPressUnitCount][STATION_MAX_Y_SITE][STATION_MAX_X_SITE];
	int nSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	int nSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	
	for (int nStation = 0; nStation < eMaxPressUnitCount; nStation++) {

		if (g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrDeviceSwapUse))
		{
			for (int nY = 0; nY < nSite_Div_Y; nY++) {
				for (int nX = 0; nX < nSite_Div_X; nX++) {
					int nOnOff = g_DMCont.m_dmHandlerOpt.GN(NDM11_GRR_SocketOnOff_Site1_1 + (nStation * nSite_Div_Y * nSite_Div_X) + (nY * nSite_Div_X) + nX);
					if (nOnOff == DEF_SITE_ON) {
						m_nTestSiteNum++;
					}
				}
			}
		}
		else {
			int nStationOnOff = 0;
			for (int nY = 0; nY < nSite_Div_Y; nY++) {
				for (int nX = 0; nX < nSite_Div_X; nX++) {
					int nOnOff = g_DMCont.m_dmHandlerOpt.GN(NDM11_GRR_SocketOnOff_Site1_1 + (nStation * nSite_Div_Y * nSite_Div_X) + (nY * nSite_Div_X) + nX);
					if (nOnOff == DEF_SITE_ON) {
						nStationOnOff++;
					}
				}
			}
			if(nStationOnOff > 0)
				m_nTestSiteNum++;
		}
	}

	//spread 초기화 
	InitSpread();
	InitSpreadStation();

	m_CtrlCombo.SetCurSel(0);
//	InitSpreadResult2(m_CtrlCombo.GetCurSel());

	//SetTimer(1,5000, NULL);
	GetDataToDB();
}

void CAutoGrrResultDlg::Finalize()
{
	KillTimer(1);
}

/*=============================================================================
  AUTHOR   : 손정일 [2016-09-25]
  NAME     : 
  PARAMETER:  
  RETURN   : 
  COMMENTS : Auto GRR 결과 데이터를 DB에서 들고 온다
==============================================================================*/
void CAutoGrrResultDlg::GetDataToDB()
{
	int nResult = 0;
	BOOL bError = FALSE;
	m_cs.Lock();
	//m_nDataCnt = g_DB.SELECT_COUNT_GRRDATA_TBL();

	//if(m_nDataCnt != m_nBeforeDataCnt){

		m_List.m_fnClear();

		nResult = -1;
		nResult = g_DB.SELECT_GRRDATA_TBL(m_List);
		m_SprdResult.ClearRange(1,3,( 3 + m_nAutoGrrTestCount),(m_nAutoGrrUnitCount*m_nTestSiteNum) + 3,FALSE);
		m_SprdStationPF.ClearRange(1, 3, (3 + m_nAutoGrrTestCount), (m_nAutoGrrUnitCount*m_nTestSiteNum) + 3, FALSE);
		DropSpread();
		if(nResult == 0){
			ViewDatafromSpread();
			ViewStationDatafromSpread();
		}else {
			bError = TRUE;
		}
	//}

	m_cs.Unlock();
	if(bError == TRUE)
		AfxMessageBox("CAutoGrrResultDlg::GetDataToDB() Error");

	m_nBeforeDataCnt = m_nDataCnt;
}

/*=============================================================================
  AUTHOR   : 손정일 [2016-09-25]
  NAME     : 
  PARAMETER:  
  RETURN   : 
  COMMENTS : Auto GRR 결과 데이터를 Spread에 보여 준다
==============================================================================*/
void CAutoGrrResultDlg::ViewDatafromSpread()
{
	_tGRR_DATA* pRow = new _tGRR_DATA;

	char szID[10] ={0,} , szBeforeID[10] = {0,}, szBeforeStation[10] ={0,}, szStation[10] ={0,}, szBeforeTestSite[10] ={0,}, szTestSite[10] ={0,}, szPF[2] ={0,};

	int nRow = 3, nCol = 0;

	int nCount = m_List.m_fnGetCount();
	int i = 0;
	for(; i<nCount; i++) {
		pRow = (_tGRR_DATA*)m_List.m_fnGetData(i);
		sprintf_s(szID,   sizeof(szID),   pRow->szUnique);
		sprintf_s( szStation, sizeof( szStation ), pRow->szSiteNum );
		sprintf_s( szTestSite, sizeof( szTestSite ), pRow->szSubSiteNo );

		// ID Check 
		if(strcmp(szID, szBeforeID) !=0) {
			m_SprdResult.SetTextMatrix( nRow, 1, szID);
		}	

// 		//기존 방식
// 		// Station & Site Check
// 		if( strcmp(szID,szBeforeID) == 0 && strcmp(szStation,szBeforeStation) == 0 ) {
// 			m_SprdResult.SetTextMatrix( nRow, 1, "");
// 			//m_SprdResult.SetTextMatrix( nRow, 2, "" );
// 		}else {
// 			CString strSite = _T("");
// 			strSite.Format("%s_%s", szStation, szTestSite);
// 			m_SprdResult.SetTextMatrix(nRow, 2, strSite);// szStation );
// 		}
		
		//변경된 방식(Station Num & Site Num 따로 표기)
		//Station Num
		if (strcmp(szID, szBeforeID) == 0 && strcmp(szStation, szBeforeStation) == 0) {
			m_SprdResult.SetTextMatrix(nRow, 1, "");
		}
		else {
			CString strSite = _T("");
			strSite.Format("%s", szStation);
			m_SprdResult.SetTextMatrix(nRow, 2, strSite);// szStation );
		}


//		int index_num = 3;
// 		if( g_DMCont.m_dmHandlerOpt.GB( BDM11_MD_AutoGrrDeviceSwapUse ) )
// 		{
// 			// Site Check
// 			if( strcmp( szID, szBeforeID ) == 0 && strcmp( szStation, szBeforeStation ) == 0 && strcmp( szTestSite, szBeforeTestSite ) == 0 ) {
// 				nCol++;
// 			}else {
// 				m_SprdResult.SetTextMatrix( nRow, 3, szTestSite );
// 				nRow++;
// 				nCol = 0;
// 			}
// 			index_num = 4;
// 			
// 		}
// 		else {
// 			if( strcmp( szID, szBeforeID ) == 0 && strcmp( szStation, szBeforeStation ) == 0 ) {
// 				nCol++;
// 			}
// 			else {
// 				nRow++;
// 				nCol = 0;
// 			}
// 		}


		// Site Check
		int index_num = 4;

		if (strcmp(szID, szBeforeID) == 0 && strcmp(szStation, szBeforeStation) == 0 && strcmp(szTestSite, szBeforeTestSite) == 0) {
			nCol++;
		}
		else {
			m_SprdResult.SetTextMatrix(nRow, 3, szTestSite);
			nRow++;
			nCol = 0;
		}


		m_SprdResult.SetCol( ( index_num + nCol ) );
		m_SprdResult.SetRow( nRow - 1 );

		if(strcmp(pRow->szTestResult,"PASS") == 0){
			sprintf_s(szPF,sizeof(szPF),"P");
			m_SprdResult.SetBackColor(COLOR_GREEN);
		}else{
			sprintf_s(szPF,sizeof(szPF),"F");
			m_SprdResult.SetBackColor(COLOR_RED);
		}
		
		m_SprdResult.SetTextMatrix(nRow-1,( index_num +nCol),szPF);
		m_SprdResult.SetTypeEditMultiLine(TRUE);

		sprintf_s(szBeforeID,   sizeof(szBeforeID),   szID);
		sprintf_s(szBeforeStation, sizeof(szBeforeStation), szStation);
		sprintf_s(szBeforeTestSite, sizeof(szBeforeTestSite), szTestSite);
	}
}

void CAutoGrrResultDlg::ViewStationDatafromSpread()
{
	int nStationNo = m_CtrlCombo.GetCurSel() + 1;

	_tGRR_DATA* pRow = new _tGRR_DATA;

	char szID[10] = { 0, }, szBeforeID[10] = { 0, }, szBeforeStation[10] = { 0, }, szStation[10] = { 0, }, szBeforeTestSite[10] = { 0, }, szTestSite[10] = { 0, }, szPF[2] = { 0, };

	int nRow = 3, nCol = 0;

	int nCount = m_List.m_fnGetCount();
	int i = 0;
	for (; i < nCount; i++) {
		pRow = (_tGRR_DATA*)m_List.m_fnGetData(i);
		sprintf_s(szID, sizeof(szID), pRow->szUnique);
		sprintf_s(szStation, sizeof(szStation), pRow->szSiteNum);
		sprintf_s(szTestSite, sizeof(szTestSite), pRow->szSubSiteNo);

		CString strCurStationNo = _T("");
		strCurStationNo.Format("%d", nStationNo);

		if (strcmp(strCurStationNo, szStation) != 0) {
			continue;
		}

		// ID Check 
		if (strcmp(szID, szBeforeID) != 0) {
			m_SprdStationPF.SetTextMatrix(nRow, 1, szID);
		}

		//변경된 방식(Station Num & Site Num 따로 표기)
		//Station Num
		if (strcmp(szID, szBeforeID) == 0 && strcmp(szStation, szBeforeStation) == 0) {
			m_SprdStationPF.SetTextMatrix(nRow, 1, "");
		}
		else {
			CString strSite = _T("");
			strSite.Format("%s", szStation);
			m_SprdStationPF.SetTextMatrix(nRow, 2, strSite);// szStation );
		}

		// Site Check
		int index_num = 4;

		if (strcmp(szID, szBeforeID) == 0 && strcmp(szStation, szBeforeStation) == 0 && strcmp(szTestSite, szBeforeTestSite) == 0) {
			nCol++;
		}
		else {
			m_SprdStationPF.SetTextMatrix(nRow, 3, szTestSite);
			nRow++;
			nCol = 0;
		}


		m_SprdStationPF.SetCol((index_num + nCol));
		m_SprdStationPF.SetRow(nRow - 1);

		if (strcmp(pRow->szTestResult, "PASS") == 0) {
			sprintf_s(szPF, sizeof(szPF), "P");
			m_SprdStationPF.SetBackColor(COLOR_GREEN);
		}
		else {
			sprintf_s(szPF, sizeof(szPF), "F");
			m_SprdStationPF.SetBackColor(COLOR_RED);
		}

		m_SprdStationPF.SetTextMatrix(nRow - 1, (index_num + nCol), szPF);
		m_SprdStationPF.SetTypeEditMultiLine(TRUE);

		sprintf_s(szBeforeID, sizeof(szBeforeID), szID);
		sprintf_s(szBeforeStation, sizeof(szBeforeStation), szStation);
		sprintf_s(szBeforeTestSite, sizeof(szBeforeTestSite), szTestSite);
	}
}

void CAutoGrrResultDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch( nIDEvent )
	{
	case 1:
		{
			KillTimer(1);
			//GetDataToDB();
			SetTimer(1,5000,NULL);
		}break;
	}

	CDialog::OnTimer(nIDEvent);
}


void CAutoGrrResultDlg::OnBnClickedCancel()
{
	ShowWindow(SW_HIDE);
	//Finalize();
	//CDialog::OnCancel();
}

void CAutoGrrResultDlg::InitSpread()
{
	char strTestCount[10] ={0,};
	int nTestCount =0;
// 	int Max_Col = 2;
// 	if( g_DMCont.m_dmHandlerOpt.GB( BDM11_MD_AutoGrrDeviceSwapUse ) )
// 		Max_Col = 3;

	int Max_Col = 3;

	m_SprdResult.SetMaxCols( Max_Col + m_nAutoGrrTestCount );
	m_SprdResult.SetMaxRows( (m_nTestSiteNum* m_nAutoGrrUnitCount) + 2 );
	m_SprdResult.SetRowsFrozen(1);
	m_SprdResult.SetRowsFrozen(2);

	for(int i = 1; i < m_nTestSiteNum + 2; i++) {
		m_SprdResult.SetRowHeight(i,20);
	}
	 
	m_SprdResult.SetTextMatrix(1,1,"ID");
	m_SprdResult.AddCellSpan(1,1,1,2);
	m_SprdResult.SetTextMatrix(1,2,"Station No");
	m_SprdResult.AddCellSpan(2,1,1,2);

// 	int index_num = 3;
// 	if( g_DMCont.m_dmHandlerOpt.GB( BDM11_MD_AutoGrrDeviceSwapUse ) ) {
// 		m_SprdResult.SetTextMatrix( 1, 3, "Site No" );
// 		m_SprdResult.AddCellSpan( 3, 1, 1, 2 );
// 		index_num = 4;
// 	}

	int index_num = 4;
	m_SprdResult.SetTextMatrix(1, 3, "Site No");
	m_SprdResult.AddCellSpan(3, 1, 1, 2);

	m_SprdResult.SetTextMatrix( 1, index_num, "P/F" );
	m_SprdResult.AddCellSpan( index_num, 1, m_nAutoGrrTestCount, 1 );
	for(int i = index_num; i<m_nAutoGrrTestCount + index_num; i++) {
		nTestCount = i - index_num + 1;
		sprintf_s(strTestCount,sizeof(strTestCount), "%d", nTestCount);

		m_SprdResult.SetTextMatrix(2,i,strTestCount);
		m_SprdResult.SetColWidth(i, index_num );
	}
}

void CAutoGrrResultDlg::InitSpreadStation()
{
	char strTestCount[10] = { 0, };
	int nTestCount = 0;
	int Max_Col = 3;

	m_SprdStationPF.SetMaxCols(Max_Col + m_nAutoGrrTestCount);
	m_SprdStationPF.SetMaxRows((m_nTestSiteNum* m_nAutoGrrUnitCount) + 2);
	m_SprdStationPF.SetRowsFrozen(1);
	m_SprdStationPF.SetRowsFrozen(2);

	for (int i = 1; i < m_nTestSiteNum + 2; i++) {
		m_SprdStationPF.SetRowHeight(i, 20);
	}

	m_SprdStationPF.SetTextMatrix(1, 1, "ID");
	m_SprdStationPF.AddCellSpan(1, 1, 1, 2);
	m_SprdStationPF.SetTextMatrix(1, 2, "Station No");
	m_SprdStationPF.AddCellSpan(2, 1, 1, 2);

	int index_num = 4;
	m_SprdStationPF.SetTextMatrix(1, 3, "Site No");
	m_SprdStationPF.AddCellSpan(3, 1, 1, 2);

	m_SprdStationPF.SetTextMatrix(1, index_num, "P/F");
	m_SprdStationPF.AddCellSpan(index_num, 1, m_nAutoGrrTestCount, 1);
	for (int i = index_num; i < m_nAutoGrrTestCount + index_num; i++) {
		nTestCount = i - index_num + 1;
		sprintf_s(strTestCount, sizeof(strTestCount), "%d", nTestCount);

		m_SprdStationPF.SetTextMatrix(2, i, strTestCount);
		m_SprdStationPF.SetColWidth(i, index_num);
	}
}

void CAutoGrrResultDlg::DropSpread()
{
	for(int i = 3; i<(m_nTestSiteNum * m_nAutoGrrUnitCount + 3); i++){
		for(int j = 0; j<3+(m_nAutoGrrTestCount*2); j++){
			m_SprdResult.SetTextMatrix(i,j,"");
			m_SprdResult.SetCol(j);
			m_SprdResult.SetRow(i);
			m_SprdResult.SetBackColor(COLOR_WWHITE);
		}
	}
}

BEGIN_EVENTSINK_MAP(CAutoGrrResultDlg, CDialog)
	ON_EVENT(CAutoGrrResultDlg, IDC_SPREAD_GRR_RESULT, 5, CAutoGrrResultDlg::ClickSpreadGrrResult, VTS_I4 VTS_I4)
END_EVENTSINK_MAP()


void CAutoGrrResultDlg::ClickSpreadGrrResult(long Col, long Row)
{
// 	int Max_Col = 3;
// 	if (g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrDeviceSwapUse))
// 		Max_Col = 4;
// 	if (Col > Max_Col + m_nAutoGrrTestCount)
// 		return;
// 	if (Row > (m_nTestSiteNum* m_nAutoGrrUnitCount) + 2)
// 		return;

	if (Col < 4)
		return;

	CString strTestResult =  m_SprdResult.GetTextMatrix( Row, Col);

	if( strTestResult != _T("P") && strTestResult != _T("F")) {
		GetDlgItem(IDC_EDIT_RESULT_DESCRIPTION)->SetWindowText(_T("Date Error"));
		GetDlgItem( IDC_EDIT_2DID_DISPLAY )->SetWindowText( _T( "Date Error" ) );
		return;
	}

	//Data Index 밀리는 현상.
	//DB에는 Data 결과 나오는 순으로 저장이 됨.
	//Test 완료 되지 않아 결과가 없는 것 Count에서 제외
	_tGRR_DATA* Grr_Data = nullptr;
	int nMaxCnt = 0;
	int nCurRowCnt = 0;
	int nCurColCnt = 0;

	int No_Data_Cnt = 0;
	int Exist_Data_Cnt = 0;

	//Click한 부분 Data 몇개 존재 하는지 갯수 Check
	m_nDataCnt = g_DB.SELECT_COUNT_GRRDATA_TBL();
	for (int i = 0; i < m_nDataCnt; i++)
	{
		Grr_Data = (_tGRR_DATA*)m_List.m_fnGetData(i);
		//첫 번째, Row에서 Test Count 가장 큰놈 Check.
		if (Grr_Data->nTestCntNo > nMaxCnt)
		{
			nMaxCnt = Grr_Data->nTestCntNo;
		}

		//Test Count 1이면 다음 Row에 해당
		if (Grr_Data->nTestCntNo == 1)
		{
			nCurRowCnt++;
			nCurColCnt++;
			nMaxCnt = 1;	
		}

		//Click한 Row 보다 작은 애들은 Data Counting
		if (nCurRowCnt <= (Row - 3)/* && nCurColCnt == (Col - 4)*/)
		{
			Exist_Data_Cnt++;
		}
		else
		{
			//Click한 Row와 같을 때, 해당 Col이전인 애들 Counting
			if (nMaxCnt < (Col - 4) + 1)
			{
				Exist_Data_Cnt++;
			}
			else
			{
				//Click한 Cell 이전 까지 Data 몇개 있는지 확인.
				break;
			}
		}
	}


	int GRR_Data_Cnt =  (( Row- 3) * m_nAutoGrrTestCount ) + (Col - 4);

	//No_Data_Cnt : 실제 Click한 위치의 Full Data(모든 Test가 완료 된 후, 실제 Data 위치) - Click한 위치 이전 까지 실제 Data
	No_Data_Cnt = GRR_Data_Cnt - Exist_Data_Cnt;
	GRR_Data_Cnt -= No_Data_Cnt;
	
// 	int nSpaceCount = 0;
// 	for (int nY = 3; nY <= Row; nY++)
// 	{
// 		for (int nX = 3; nX <= Max_Col + m_nAutoGrrTestCount; nX++)
// 		{
// 			if (nY == Row && nX > Col)
// 			{
// 				continue;
// 			}
// 
// 			CString strTestResult = m_SprdResult.GetTextMatrix(nY, nX);
// 			if (strTestResult != _T("P") && strTestResult != _T("F"))
// 				nSpaceCount++;
// 		}
// 	}
// 	
// 	
// 	if( GRR_Data_Cnt - nSpaceCount > m_List.m_fnGetCount() )
// 		return;
	
	_tGRR_DATA* pGrr_Data = nullptr;
	pGrr_Data = (_tGRR_DATA*)m_List.m_fnGetData(GRR_Data_Cnt);
//	pGrr_Data = (_tGRR_DATA*)m_List.m_fnGetData(GRR_Data_Cnt - nSpaceCount);
	
	char szDescription[100] = {0,};
	char sz2DID[ 100 ] = { 0, };
	sprintf_s( szDescription, sizeof( szDescription ), pGrr_Data->szResultDescription );
	sprintf_s( sz2DID, sizeof( sz2DID ), pGrr_Data->sz2DID );
	
	GetDlgItem( IDC_EDIT_RESULT_DESCRIPTION )->SetWindowText( szDescription );
	GetDlgItem( IDC_EDIT_2DID_DISPLAY )->SetWindowText( sz2DID );
	
	pGrr_Data = nullptr;
}

void CAutoGrrResultDlg::OnBnClickedButtonRefresh()
{
	GetDataToDB();
	OnBnClickedCombo();
}

void CAutoGrrResultDlg::OnBnClickedCombo()
{
	//int nStationNo = m_CtrlCombo.GetCurSel();
	//InitSpreadResult2(nStationNo);
	ViewStationDatafromSpread();
}

void CAutoGrrResultDlg::InitSpreadResult2(int nStationNo)
{
	int nSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	int nSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);

	m_SprdStationPF.SetMaxCols(m_nAutoGrrTestCount * 4);
	m_SprdStationPF.SetMaxRows(8);

	for (int nCol = 0; nCol < m_nAutoGrrTestCount * 4; nCol++)
	{
		for (int nRow = 0; nRow < 8; nRow++)
		{
			m_SprdStationPF.SetTextMatrix(nRow + 1, nCol + 1, "");
			m_SprdStationPF.SetBackColor(COLOR_WWHITE);
		}
	}

	for (int nY = 0; nY < nSite_Div_Y; nY++)
	{
		for (int nX = 0; nX < nSite_Div_X; nX++)
		{
			char szData[MAX_PATH] = { 0, };
			g_DMCont.m_dmContac_Dn.GS(SDM1_SiteGrrPF_History1_1 + nStationNo*STATION_MAX_PARA + nX + nY*STATION_MAX_X_SITE, szData, sizeof(szData));

			CString strData(szData);
			int nLength = strData.GetLength();
			for (int nCol = 0; nCol < nLength; nCol++)
			{
				m_SprdStationPF.SetTextMatrix(nX + nY*nSite_Div_X + 1, nCol + 1, strData.Mid(nCol, 1));// (LPSTR)szData[nCol]);

				m_SprdResult.SetCol(nCol + 1);
				m_SprdResult.SetRow(nX + nY*nSite_Div_X + 1);

				if (strData.Mid(nCol, 1) == "P")
					m_SprdStationPF.SetBackColor(COLOR_GREEN);
				else
					m_SprdStationPF.SetBackColor(COLOR_RED);
			}
		}
	}
}