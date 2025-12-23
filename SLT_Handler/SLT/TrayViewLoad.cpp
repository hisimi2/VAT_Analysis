// TrayViewLoad.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "TrayViewLoad.h"
#include "afxdialogex.h"


// CTrayViewLoad 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTrayViewLoad, CDialog)

CTrayViewLoad::CTrayViewLoad(CWnd* pParent /*=NULL*/)
	: CDialog(CTrayViewLoad::IDD, pParent)
{

}

CTrayViewLoad::~CTrayViewLoad()
{
}

void CTrayViewLoad::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPREAD_LD_SETPLATE1, m_SprdSetplateLD1);
	DDX_Control(pDX, IDC_SPREAD_LD_FEEDER, m_SprdFeeder);
	DDX_Control(pDX, IDC_SPREAD_LD_TBL_1, m_SprdLoadTbl1);
	DDX_Control(pDX, IDC_SPREAD_LD_TBL_2, m_SprdLoadTbl2);
	DDX_Control(pDX, IDC_SPREAD_LD_TBL_3, m_SprdLoadTbl3);
	DDX_Control(pDX, IDC_SPREAD_LD_TBL_4, m_SprdLoadTbl4);
	DDX_Control(pDX, IDC_SPREAD_TRAY_LOAD_PP, m_SprdLoadTrayPP);
	DDX_Control(pDX, IDC_SPREAD_TRAY_UNLOAD_PP, m_SprdUnloadTrayPP);
	
	DDX_Control(pDX, IDC_SPREAD_TEST_PP1_LOAD, m_SprdTestPP1_LD);
	DDX_Control(pDX, IDC_SPREAD_TEST_PP1_UNLOAD, m_SprdTestPP1_UD);
	DDX_Control(pDX, IDC_SPREAD_TEST_PP2_LOAD, m_SprdTestPP2_LD);
	DDX_Control(pDX, IDC_SPREAD_TEST_PP2_UNLOAD, m_SprdTestPP2_UD);
	DDX_Control(pDX, IDC_SPREAD_TEST_PP_CLEAN1, m_SprdTestPP1_Clean);
	DDX_Control(pDX, IDC_SPREAD_TEST_PP_CLEAN2, m_SprdTestPP2_Clean);
	DDX_Control(pDX, IDC_COMBO_KIND, m_CtrlComboKindView);
}


BEGIN_MESSAGE_MAP(CTrayViewLoad, CDialog)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CTrayViewLoad 메시지 처리기입니다.


BOOL CTrayViewLoad::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitSpread();
	UpdateSpreadLoadSetplate();
	UpdateSpreadFeeder();
	UpdateSpreadLoadTbl();
	UpdateSpreadTrayPP();

	m_CtrlComboKindView.SetCurSel(0);

	SetTimer(0, 500, NULL);

	InitColorView();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CTrayViewLoad::SetDisplay(CSpreadSheet* pSpread, int nPos, int nMaxCols, int nMaxRows, int nTestPPType/*== -1*/)
{
	ST_DD_DEVICE stDeviceData;
	BOOL bAutoRetestUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);
	int nAutoRetestKind     = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoRetest);

	for(int nCols=0; nCols<nMaxCols; nCols++)
	{
		for(int nRows=0; nRows<nMaxRows;nRows++)
		{
			//if(pSpread == )
			int nRow = 0;
			if ((nPos == eDeviceStage_TEST_PP_1 || nPos == eDeviceStage_TEST_PP_2) && nTestPPType == eTestPPUnLoadType)
			{
				nRow = nRows + 2;
			}
			else
			{
				nRow = nRows;
			}

			g_DvcDataMem.GetDeviceData(nPos, nCols, nRow, &stDeviceData);

			if(stDeviceData.sExist == eDeviceData_None) {
				if (strcmp(stDeviceData.szBinResult, DEF_TRAY_PICK_ERR_DEVICE) == 0)
					pSpread->SetTextMatrix(nRows + 1, nCols + 1, "20");
				else
					pSpread->SetTextMatrix(nRows +1, nCols+1,"");
			}else {
				CString str = _T("");

				if(nPos == eDeviceStage_TRAY_PP_1 && nCols < 4)
                {
					if( m_CtrlComboKindView.GetCurSel() == 1 ){ // 2d barcode
						str.Format("%s",stDeviceData.sz2DBarcode);
					}else if( m_CtrlComboKindView.GetCurSel() == 2 ){ // err string
						str.Format("%s",stDeviceData.szErrString);
// 					}else if( m_CtrlComboKindView.GetCurSel() == 3 ){ // err Code
// 						str.Format("%s",stDeviceData.szErrorCode);
					}else{
						str.Format("%s",stDeviceData.szBinResult);
					}					
				}else {
						if( m_CtrlComboKindView.GetCurSel() == 1 ){ // 2d barcode
							str.Format("%s",stDeviceData.sz2DBarcode);
						}else if( m_CtrlComboKindView.GetCurSel() == 2 ){ // err string
							str.Format("%s",stDeviceData.szErrString);
// 						}else if( m_CtrlComboKindView.GetCurSel() == 3 ){ // err Code
// 							str.Format("%s",stDeviceData.szErrorCode);
						}else{
							
							char szTestRetest[32] = {0,};
							if( bAutoRetestUseMode == eOPTION_USE) {
								switch(stDeviceData.nTestingCnt)
								{
								case eARB_1stTestEnd: { sprintf_s(szTestRetest, sizeof(szTestRetest), "A"); }break;
								case eARB_2stTestEnd: 
									{ 
										if(nAutoRetestKind == eAutoRetest_Aonly){
											sprintf_s(szTestRetest, sizeof(szTestRetest), "Aonly"); 
										}else if(nAutoRetestKind == eAutoRetest_AB){
											sprintf_s(szTestRetest, sizeof(szTestRetest), "AB"); 
										}/*else if(nAutoRetestKind == eAutoRetest_AA){
											sprintf_s(szTestRetest, sizeof(szTestRetest), "AA"); 
										}*/else{
											sprintf_s(szTestRetest, sizeof(szTestRetest), "AA"); 
										}
									}break;
								case eARB_3stTestEnd: 
									{ 
										if(nAutoRetestKind == eAutoRetest_Aonly){
											sprintf_s(szTestRetest, sizeof(szTestRetest), "Aonly"); 
										}else if(nAutoRetestKind == eAutoRetest_AB){
											sprintf_s(szTestRetest, sizeof(szTestRetest), "AB"); 
										}else if(nAutoRetestKind == eAutoRetest_AA){
											sprintf_s(szTestRetest, sizeof(szTestRetest), "AA"); 
										}else{
											sprintf_s(szTestRetest, sizeof(szTestRetest), "AAB"); 
										}                    
									}break;
								default: { sprintf_s(szTestRetest, sizeof(szTestRetest), " "); }break;
								}
							}else{
								sprintf_s(szTestRetest, sizeof(szTestRetest), " ");
							}
						
							str.Format("%s_%s",stDeviceData.szBinResult,szTestRetest);
							if (stDeviceData.szBinResult == _T("")) {
								int test = 0;
							}
						}
				}
				pSpread->SetTextMatrix(nRows+1, nCols+1,str);
			}

			if(stDeviceData.nTestingCnt == eARB_1stTestEnd){
				pSpread->SetForeColor(COLOR_GREEN);
			}else if(stDeviceData.nTestingCnt == eARB_2stTestEnd){
				pSpread->SetForeColor(COLOR_DARKYELLOW);
			}else if(stDeviceData.nTestingCnt == eARB_3stTestEnd){
				pSpread->SetForeColor(COLOR_RED);
			}else{
				pSpread->SetForeColor(COLOR_BBLACK);
			}			
		}
	}
}

void CTrayViewLoad::SetDisplayGrr(CSpreadSheet* pSpread, int nPos, int nMaxCols, int nMaxRows, int nTestPPType/* = -1*/)
{
	ST_DD_DEVICE stDeviceData;
	CString str = _T("");

	int nKind = m_CtrlComboKindView.GetCurSel();
	for(int nCols=0; nCols<nMaxCols; nCols++)
	{
		for(int nRows=0; nRows<nMaxRows;nRows++)
		{
			int nRow = 0;
			if ((nPos == eDeviceStage_TEST_PP_1 || nPos == eDeviceStage_TEST_PP_2) && nTestPPType == eTestPPUnLoadType)
			{
				nRow = nRows + 2;
			}
			else
			{
				nRow = nRows;
			}


			g_DvcDataMem.GetDeviceData(nPos, nCols, nRow, &stDeviceData);

			if(stDeviceData.sExist == eDeviceData_None){
				pSpread->SetTextMatrix(nRows+1, nCols+1,"");
			}else{

				if (nKind == 1) { // 2d barcode
					str.Format("%s", stDeviceData.sz2DBarcode);
				}
				else if (nKind == 2) { // err string
					str.Format("%s", stDeviceData.szErrString);
				}
				else {
					//str.Format("%s", stDeviceData.szBinResult);
					str.Format("%s", stDeviceData.szUniqueID);
				}

				//
				pSpread->SetTextMatrix(nRows+1, nCols+1,str);
			}
		}
	}
}

void CTrayViewLoad::ClassifyDisplay(CSpreadSheet* pSpread, int nPos, int nMaxCols, int nMaxRows)
{
	BOOL bAutoGrrUse = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrUse);

	if (bAutoGrrUse)
	{
		SetDisplayGrr(pSpread, nPos, nMaxCols, nMaxRows);
	}
	else
	{
		SetDisplay(pSpread, nPos, nMaxCols, nMaxRows);
	}
}

void CTrayViewLoad::InitSpread()
{
	int nTrayCntX = g_DMCont.m_dmTrayPP.GN( NDM5_TrayDiv_X );
	int nTrayCntY = g_DMCont.m_dmTrayPP.GN( NDM5_TrayDiv_Y );    
	m_SprdSetplateLD1.SetMaxCols(nTrayCntX);
	m_SprdSetplateLD1.SetMaxRows(nTrayCntY);

	m_SprdFeeder.SetMaxCols(nTrayCntX);
	m_SprdFeeder.SetMaxRows(nTrayCntY);

    int nLoadTblCntX = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);
    int nLoadTblCntY = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);
	m_SprdLoadTbl1.SetMaxCols(nLoadTblCntX);
	m_SprdLoadTbl1.SetMaxRows(nLoadTblCntY);
	m_SprdLoadTbl2.SetMaxCols(nLoadTblCntX);
	m_SprdLoadTbl2.SetMaxRows(nLoadTblCntY);
	m_SprdLoadTbl3.SetMaxCols(nLoadTblCntX);
	m_SprdLoadTbl3.SetMaxRows(nLoadTblCntY);
	m_SprdLoadTbl4.SetMaxCols(nLoadTblCntX);
	m_SprdLoadTbl4.SetMaxRows(nLoadTblCntY);

	
	m_SprdLoadTrayPP.SetMaxCols(MAX_TRAY_PP_CNT_X);
	m_SprdLoadTrayPP.SetMaxRows(MAX_TRAY_PP_CNT_Y);

	m_SprdUnloadTrayPP.SetMaxCols(MAX_TRAY_PP_CNT_X);
	m_SprdUnloadTrayPP.SetMaxRows(MAX_TRAY_PP_CNT_Y);

	m_SprdTestPP1_LD.SetMaxCols(MAX_TEST_PP_PKR_CNT_X);
	m_SprdTestPP1_LD.SetMaxRows(MAX_TEST_PP_PKR_CNT_Y/**2*/);
	m_SprdTestPP1_UD.SetMaxCols(MAX_TEST_PP_PKR_CNT_X);
	m_SprdTestPP1_UD.SetMaxRows(MAX_TEST_PP_PKR_CNT_Y);

	m_SprdTestPP2_LD.SetMaxCols(MAX_TEST_PP_PKR_CNT_X);
	m_SprdTestPP2_LD.SetMaxRows(MAX_TEST_PP_PKR_CNT_Y/**2*/);
	m_SprdTestPP2_UD.SetMaxCols(MAX_TEST_PP_PKR_CNT_X);
	m_SprdTestPP2_UD.SetMaxRows(MAX_TEST_PP_PKR_CNT_Y);

	int nCleanBuffX = g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Div_X);
	int nCleanBuffY = g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Div_Y);
	m_SprdTestPP1_Clean.SetMaxCols(nCleanBuffX);
	m_SprdTestPP1_Clean.SetMaxRows(nCleanBuffY);
	m_SprdTestPP2_Clean.SetMaxCols(nCleanBuffX);
	m_SprdTestPP2_Clean.SetMaxRows(nCleanBuffY);

	m_SprdLoadTbl1.SetAutoSize(TRUE);
	m_SprdLoadTbl2.SetAutoSize(TRUE);
	m_SprdLoadTbl3.SetAutoSize(TRUE);
	m_SprdLoadTbl4.SetAutoSize(TRUE);
	m_SprdLoadTrayPP.SetAutoSize(TRUE);
	m_SprdUnloadTrayPP.SetAutoSize(TRUE);
	m_SprdTestPP1_LD.SetAutoSize(TRUE);
	m_SprdTestPP2_LD.SetAutoSize(TRUE);
	m_SprdTestPP1_UD.SetAutoSize(TRUE);
	m_SprdTestPP2_UD.SetAutoSize(TRUE);

	m_SprdTestPP1_Clean.SetAutoSize(TRUE);
	m_SprdTestPP2_Clean.SetAutoSize(TRUE);
}


BOOL CTrayViewLoad::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}


void CTrayViewLoad::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == 0)
	{
		KillTimer(nIDEvent);

		UpdateSpreadLoadSetplate();
		UpdateSpreadFeeder();
		UpdateSpreadLoadTbl();
		UpdateSpreadTrayPP();
		UpdateSpreadTestPP();
		UpdateSpreadCleanTbl();

		SetTimer(0, 1000, NULL);
	}
	

	CDialog::OnTimer(nIDEvent);
}

void CTrayViewLoad::OnDestroy()
{
	CDialog::OnDestroy();
}

void CTrayViewLoad::UpdateSpreadLoadSetplate()
{
	int nTrayCntX = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_X);
	int nTrayCntY = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_Y);

	CSpreadSheet* pSpread = NULL;

	pSpread = (CSpreadSheet*)GetDlgItem(IDC_SPREAD_LD_SETPLATE1);

	BOOL bAutoGrrUse = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrUse);

	if (bAutoGrrUse){
		SetDisplayGrr(pSpread, eDeviceStage_STACKER_CONV_3, nTrayCntX, nTrayCntY);
	}else{
		SetDisplay(pSpread, eDeviceStage_STACKER_CONV_3, nTrayCntX, nTrayCntY);
	}

	pSpread = NULL;
}

void CTrayViewLoad::UpdateSpreadFeeder()
{
	int nTrayCntX = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_X);
	int nTrayCntY = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_Y);

	CSpreadSheet* pSpread = NULL;

	pSpread = (CSpreadSheet*)GetDlgItem(IDC_SPREAD_LD_FEEDER);

	BOOL bAutoGrrUse = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrUse);

	if (bAutoGrrUse) {
		SetDisplayGrr(pSpread, eDeviceStage_TRAY_FEEDER, nTrayCntX, nTrayCntY);
	}
	else {
		SetDisplay(pSpread, eDeviceStage_TRAY_FEEDER, nTrayCntX, nTrayCntY);
	}

	pSpread = NULL;
}

void CTrayViewLoad::UpdateSpreadLoadTbl()
{
	CSpreadSheet* pSpread = NULL;

	int nSprdID[MAX_LD_TBL_CNT] = {IDC_SPREAD_LD_TBL_1,IDC_SPREAD_LD_TBL_2,IDC_SPREAD_LD_TBL_3,IDC_SPREAD_LD_TBL_4};
	int nPos[MAX_LD_TBL_CNT]    = {eDeviceStage_LD_TBL_1, eDeviceStage_LD_TBL_2,eDeviceStage_LD_TBL_3, eDeviceStage_LD_TBL_4 };
	BOOL bAutoGrrUse = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrUse);

	int nLoadTblX = g_DMCont.m_dmShuttle.GN( NDM3_LdTbl_Div_X);
	int nLoadTblY = g_DMCont.m_dmShuttle.GN( NDM3_LdTbl_Div_Y );

	/*MAX_LD_TBL_CNT*/
	for(int i=0; i< MAX_LD_TBL_CNT; i++)
	{
		pSpread = (CSpreadSheet*)GetDlgItem(nSprdID[i]);
		if (bAutoGrrUse){
			SetDisplayGrr(pSpread, nPos[i], nLoadTblX, nLoadTblY );
		}else{
			SetDisplay(pSpread, nPos[i], nLoadTblX, nLoadTblY );
		}
	}

	pSpread = NULL;
}

void CTrayViewLoad::UpdateSpreadTrayPP()
{
	int nResourceID[MAX_TRAY_PP_CNT] = { IDC_SPREAD_TRAY_LOAD_PP,IDC_SPREAD_TRAY_UNLOAD_PP };
	CSpreadSheet* pSpread;
	
	for (int i = 0; i < MAX_TRAY_PP_CNT; i++)
	{
		pSpread = (CSpreadSheet*)GetDlgItem(nResourceID[i]);

		BOOL bAutoGrrUse = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrUse);
		if (bAutoGrrUse) {
			SetDisplayGrr(pSpread, eDeviceStage_TRAY_PP_1 +i, MAX_TRAY_PP_CNT_X, MAX_TRAY_PP_CNT_Y);
		}
		else {
			SetDisplay(pSpread, eDeviceStage_TRAY_PP_1+i, MAX_TRAY_PP_CNT_X, MAX_TRAY_PP_CNT_Y);
		}
		pSpread = NULL;
	}
}

void CTrayViewLoad::UpdateSpreadTestPP()
{
	CSpreadSheet* pSpread = NULL;

	int nSprdID_TestPP1[MAX_TEST_PP_CNT] = {IDC_SPREAD_TEST_PP1_LOAD,IDC_SPREAD_TEST_PP1_UNLOAD};
	int nSprdID_TestPP2[MAX_TEST_PP_CNT] = { IDC_SPREAD_TEST_PP2_LOAD,IDC_SPREAD_TEST_PP2_UNLOAD };
	int nPos[MAX_TEST_PP_CNT]    = {eDeviceStage_TEST_PP_1, eDeviceStage_TEST_PP_2};

	BOOL bAutoGrrUse = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrUse);

	for(int i=0; i< MAX_TEST_PP_CNT; i++)
	{
		//		pSpread = (CSpreadSheet*)GetDlgItem(nSprdID[i]);
		//Hand당 Load, Unload 2번 Check
		for (int j = 0; j < MAX_TEST_PP_CNT; j++) {
			int nHandType = (j == 0) ? eTestPPLoadType : eTestPPUnLoadType;
			pSpread = (CSpreadSheet*)GetDlgItem((i == 0) ? nSprdID_TestPP1[j] : nSprdID_TestPP2[j]);
			if (bAutoGrrUse) {
				SetDisplayGrr(pSpread, nPos[i], MAX_TEST_PP_PKR_CNT_X, MAX_TEST_PP_PKR_CNT_Y/**2*/, nHandType);
			}
			else {
				SetDisplay(pSpread, nPos[i], MAX_TEST_PP_PKR_CNT_X, MAX_TEST_PP_PKR_CNT_Y/**2*/, nHandType);
			}
		}
	}

	pSpread = NULL;

}

void CTrayViewLoad::UpdateSpreadBuffTbl()
{
	CSpreadSheet* pSpread = NULL;

	int nBuffCntX = g_DMCont.m_dmTestPP.GN(NDM4_TestBuffTbl_Div_X);
	int nBuffCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestBuffTbl_Div_Y);

	int nSprdID[MAX_TEST_PP_CNT] = {IDC_SPREAD_TEST_PP_BUFF1,IDC_SPREAD_TEST_PP_BUFF2};
	int nPos[MAX_TEST_PP_CNT]    = {eDeviceStage_TEST_PP_1_ART_BUFF, eDeviceStage_TEST_PP_2_ART_BUFF};

	BOOL bAutoGrrUse = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrUse);

	for(int i=0; i< MAX_TEST_PP_CNT; i++)
	{
		pSpread = (CSpreadSheet*)GetDlgItem(nSprdID[i]);
		if (bAutoGrrUse){
			SetDisplayGrr(pSpread, nPos[i], nBuffCntX, nBuffCntY);
		}else{
			SetDisplay(pSpread, nPos[i], nBuffCntX, nBuffCntY);
		}
	}

	pSpread = NULL;
}

void CTrayViewLoad::UpdateSpreadCleanTbl()
{
	CSpreadSheet* pSpread = NULL;

	int nCleanCntX = g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Div_X);
	int nCleanCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Div_Y);

	int nSprdID[MAX_TEST_PP_CNT] = {IDC_SPREAD_TEST_PP_CLEAN1,IDC_SPREAD_TEST_PP_CLEAN2};
	int nPos[MAX_TEST_PP_CNT]    = {eDeviceStage_TEST_PP_1_CLEAN_BUFF, eDeviceStage_TEST_PP_2_CLEAN_BUFF};

	BOOL bAutoGrrUse = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrUse);

	for(int i=0; i< MAX_TEST_PP_CNT; i++)
	{
		pSpread = (CSpreadSheet*)GetDlgItem(nSprdID[i]);
		if (bAutoGrrUse){
			SetDisplayGrr(pSpread, nPos[i], nCleanCntX, nCleanCntY);
		}else{
			SetDisplay(pSpread, nPos[i], nCleanCntX, nCleanCntY);
		}
	}

	pSpread = NULL;
}

void CTrayViewLoad::OnClose()
{
	KillTimer(0);

	CDialog::OnClose();
}

void CTrayViewLoad::InitColorView()
{
	CSpreadSheet* pSpread = NULL;
	pSpread = (CSpreadSheet*)GetDlgItem(IDC_SPREAD_TEXT_COLOR);

	pSpread->SetTextMatrix(1, 1,"First Test");
	pSpread->SetForeColor(COLOR_GREEN);

	pSpread->SetTextMatrix(1, 2,"Second Test");
	pSpread->SetForeColor(COLOR_DARKYELLOW);

	pSpread->SetTextMatrix(1, 3,"Final Test");
	pSpread->SetForeColor(COLOR_RED);

	pSpread = NULL;
}