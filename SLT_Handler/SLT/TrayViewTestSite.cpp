// TrayViewTestSite.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "TrayViewTestSite.h"
#include "afxdialogex.h"


// CTrayViewTestSite 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTrayViewTestSite, CDialogEx)

CTrayViewTestSite::CTrayViewTestSite(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTrayViewTestSite::IDD, pParent)
{

}

CTrayViewTestSite::~CTrayViewTestSite()
{
}

void CTrayViewTestSite::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_KIND, m_CtrlComboKindView);

	int nSpreadCtrlId[]={IDC_SPREAD_TEST_SITE_STATION1,IDC_SPREAD_TEST_SITE_STATION2,IDC_SPREAD_TEST_SITE_STATION3,IDC_SPREAD_TEST_SITE_STATION4,IDC_SPREAD_TEST_SITE_STATION5,IDC_SPREAD_TEST_SITE_STATION6,
		                 IDC_SPREAD_TEST_SITE_STATION7,IDC_SPREAD_TEST_SITE_STATION8,IDC_SPREAD_TEST_SITE_STATION9,IDC_SPREAD_TEST_SITE_STATION10,IDC_SPREAD_TEST_SITE_STATION11,IDC_SPREAD_TEST_SITE_STATION12,
		                 IDC_SPREAD_TEST_SITE_STATION13,IDC_SPREAD_TEST_SITE_STATION14,IDC_SPREAD_TEST_SITE_STATION15,IDC_SPREAD_TEST_SITE_STATION16};

	for(int i=0; i<eMaxPressUnitCount; i++)
	{
		DDX_Control(pDX, nSpreadCtrlId[i], m_SprdTestSite[i]);
	}
}


BEGIN_MESSAGE_MAP(CTrayViewTestSite, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CTrayViewTestSite 메시지 처리기입니다.


BOOL CTrayViewTestSite::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CTrayViewTestSite::OnDestroy()
{
	CDialogEx::OnDestroy();
}

void CTrayViewTestSite::OnClose()
{
	KillTimer(0);

	CDialogEx::OnClose();
}



BOOL CTrayViewTestSite::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	InitSpread();

	SetTimer(0, 500, NULL);

	m_CtrlComboKindView.SetCurSel(0);
	
	InitColorView();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CTrayViewTestSite::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
}


void CTrayViewTestSite::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == 0)
	{
		KillTimer(nIDEvent);

		for(int i=0; i<eMaxPressUnitCount; i++){
			UpdateSpreadTestSite(i);
		}

		SetTimer(0, 1000, NULL);
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CTrayViewTestSite::InitSpread()
{
	int nTestCntX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nTestCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	for(int i=0; i<eMaxPressUnitCount; i++)
	{
		m_SprdTestSite[i].SetMaxCols(nTestCntX);
		m_SprdTestSite[i].SetMaxRows(nTestCntY);
		m_SprdTestSite[i].SetAutoSize(TRUE);	
	}
}

void CTrayViewTestSite::SetDisplay(CSpreadSheet* pSpread, int nPos, int nMaxCols, int nMaxRows)
{
	ST_DD_DEVICE stDeviceData;
	BOOL bAutoRetestUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);
	int nAutoRetestKind     = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoRetest);

	for(int nCols=0; nCols<nMaxCols; nCols++)
	{
		for(int nRows=0; nRows<nMaxRows;nRows++)
		{
			g_DvcDataMem.GetDeviceData(nPos, nCols, nRows, &stDeviceData);

			if(stDeviceData.sExist == eDeviceData_None){
				pSpread->SetTextMatrix(nRows+1, nCols+1,"");
			}else{
				CString str = _T("");

				if( m_CtrlComboKindView.GetCurSel() == 1 ){ // 2d barcode
					str.Format("%s",stDeviceData.sz2DBarcode);
				}else if( m_CtrlComboKindView.GetCurSel() == 2 ){ // err string
					str.Format("%s",stDeviceData.szErrString);
// 				}else if( m_CtrlComboKindView.GetCurSel() == 3 ){ // err Code
// 					str.Format("%s",stDeviceData.szErrorCode);
				}else{ // exist
					char szTestRetest[32] = {0,};
					if( bAutoRetestUseMode == eOPTION_USE)
					{
						switch(stDeviceData.nTestingCnt)
						{
						case eARB_1stTestEnd: 
							{ 
								sprintf_s(szTestRetest, sizeof(szTestRetest), "A"); 
							}break;
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
					}
					else
					{
						sprintf_s(szTestRetest, sizeof(szTestRetest), " ");
					}
					str.Format("%s_%s",stDeviceData.szBinResult,szTestRetest);
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

void CTrayViewTestSite::SetDisplayGrr(CSpreadSheet* pSpread, int nPos, int nMaxCols, int nMaxRows)
{
	ST_DD_DEVICE stDeviceData;
	CString str = _T("");

	int nKind = m_CtrlComboKindView.GetCurSel();
	for(int nCols=0; nCols<nMaxCols; nCols++)
	{
		for(int nRows=0; nRows<nMaxRows;nRows++)
		{
			g_DvcDataMem.GetDeviceData(nPos, nCols, nRows, &stDeviceData);

			if(stDeviceData.sExist == eDeviceData_None){
				pSpread->SetTextMatrix(nRows+1, nCols+1,"");
			}else{

				if (nKind == 1) // 2d barcode
					str.Format("%s", stDeviceData.sz2DBarcode);
				else if (nKind == 2) // err string
					str.Format("%s", stDeviceData.szErrString);
				else
					str.Format("%s", stDeviceData.szUniqueID);
				
				pSpread->SetTextMatrix(nRows+1, nCols+1,str);
			}
		}
	}
}

void CTrayViewTestSite::ClassifyDisplay(CSpreadSheet* pSpread, int nPos, int nMaxCols, int nMaxRows)
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

#define imsi_NDM11_DUT_X  10
#define imsi_NDM11_DUT_Y  11


// nStationNo : 0 ~ 15(max station)
void CTrayViewTestSite::UpdateSpreadTestSite(int nStationNo)
{
	ASSERT(nStationNo>=0 && nStationNo < eMaxPressUnitCount);
	if( nStationNo < 0 ) return;
	if(nStationNo >= eMaxPressUnitCount) return;

	int nSpreadCtrlId[] = {IDC_SPREAD_TEST_SITE_STATION1,IDC_SPREAD_TEST_SITE_STATION2,IDC_SPREAD_TEST_SITE_STATION3,IDC_SPREAD_TEST_SITE_STATION4,IDC_SPREAD_TEST_SITE_STATION5,IDC_SPREAD_TEST_SITE_STATION6,
	                       IDC_SPREAD_TEST_SITE_STATION7,IDC_SPREAD_TEST_SITE_STATION8,IDC_SPREAD_TEST_SITE_STATION9,IDC_SPREAD_TEST_SITE_STATION10,IDC_SPREAD_TEST_SITE_STATION11,IDC_SPREAD_TEST_SITE_STATION12,
	                       IDC_SPREAD_TEST_SITE_STATION13,IDC_SPREAD_TEST_SITE_STATION14,IDC_SPREAD_TEST_SITE_STATION15,IDC_SPREAD_TEST_SITE_STATION16};

	CSpreadSheet* pSpread = NULL;
	pSpread = (CSpreadSheet*)GetDlgItem(nSpreadCtrlId[nStationNo]);
	ClassifyDisplay(pSpread, eDeviceStage_TEST_SITE_1 + nStationNo, g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X), g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y));

	pSpread = NULL;
}

void CTrayViewTestSite::InitColorView()
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