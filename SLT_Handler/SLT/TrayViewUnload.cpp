// TrayViewUnload.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "TrayViewUnload.h"
#include "afxdialogex.h"


// CTrayViewUnload 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTrayViewUnload, CDialog)

CTrayViewUnload::CTrayViewUnload(CWnd* pParent /*=NULL*/)
	: CDialog(CTrayViewUnload::IDD, pParent)
{

}

CTrayViewUnload::~CTrayViewUnload()
{
}

void CTrayViewUnload::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control( pDX, IDC_SPREAD_ULD_SETPLATE1, m_SprdStacker );
	DDX_Control(pDX, IDC_SPREAD_OUT1, m_SprdBarcode);
	DDX_Control(pDX, IDC_COMBO_POS, m_CtrlComboPos);
	DDX_Control(pDX, IDC_COMBO_KIND2, m_CtrlComboKind);
	DDX_Control(pDX, IDC_SPREAD_OUT2, m_SprdBarcode2);
	
}


BEGIN_MESSAGE_MAP(CTrayViewUnload, CDialog)
	ON_WM_TIMER()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CTrayViewUnload 메시지 처리기입니다.


BOOL CTrayViewUnload::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_CtrlComboPos.SetCurSel(0);
	m_CtrlComboKind.SetCurSel(0);

	char szCompany[16] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));
	int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);

	if (strcmp(szCompany, DEF_COMPANY_AMKOR) == 0) {
		GetDlgItem(IDC_SPREAD_ULD_SETPLATE1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPREAD_OUT1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPREAD_OUT2)->ShowWindow(SW_SHOW);

	}
	else {
		GetDlgItem(IDC_SPREAD_ULD_SETPLATE1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SPREAD_OUT1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SPREAD_OUT2)->ShowWindow(SW_HIDE);
	}

	InitSpread();
	UpdateSpreadUnLoadSetplate();

	SetTimer(0, 800, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CTrayViewUnload::UpdateSpreadUnLoadSetplate()
{
	int nTrayCntX = g_DMCont.m_dmTrayPP.GN( NDM5_TrayDiv_X );
	int nTrayCntY = g_DMCont.m_dmTrayPP.GN( NDM5_TrayDiv_Y );

	CSpreadSheet* pSpread = NULL;
	int nPos[ eMaxStackerCount ] = { eDeviceStage_STACKER_CONV_4, eDeviceStage_STACKER_CONV_5, eDeviceStage_STACKER_CONV_6, eDeviceStage_STACKER_CONV_7 };

	ClassifyDisplay( &m_SprdStacker, nPos[ m_CtrlComboPos.GetCurSel() ], nTrayCntX, nTrayCntY );
	SetDisplayBarcode( &m_SprdBarcode, nPos[ m_CtrlComboPos.GetCurSel() ], nTrayCntX, nTrayCntY );
	SetDisplayBarcode2(&m_SprdBarcode2, nPos[m_CtrlComboPos.GetCurSel()], nTrayCntX, nTrayCntY);
	pSpread = NULL;
}

void CTrayViewUnload::ClassifyDisplay( CSpreadSheet* pSpread, int nPos, int nMaxCols, int nMaxRows )
{
	BOOL bAutoGrrUse = g_DMCont.m_dmHandlerOpt.GB( BDM11_MD_AutoGrrUse );

	if( bAutoGrrUse )
		SetDisplayGrr( pSpread, nPos, nMaxCols, nMaxRows );
	else
		SetDisplayBinColor( pSpread, nPos, nMaxCols, nMaxRows );
}

void CTrayViewUnload::SetDisplayGrr(CSpreadSheet* pSpread, int nPos, int nMaxCols, int nMaxRows)
{
	ST_DD_DEVICE stDeviceData;
	CString str = _T("");

	int nKind = m_CtrlComboKind.GetCurSel();
	for(int nCols=0; nCols<nMaxCols; nCols++)
	{
		for(int nRows=0; nRows<nMaxRows;nRows++)
		{
			g_DvcDataMem.GetDeviceData(nPos, nCols, nRows, &stDeviceData);

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
					str.Format("%s", stDeviceData.szUniqueID);
				}

				pSpread->SetTextMatrix(nRows+1, nCols+1,str);
			}
		}
	}
}

void CTrayViewUnload::SetDisplayBinColor( CSpreadSheet * pSpread, int nPos, int nMaxCols, int nMaxRows )
{
	ST_DD_DEVICE stDeviceData;
	CString str = _T( "" );
	for( int nCols = 0; nCols<nMaxCols; nCols++ )
	{
		for( int nRows = 0; nRows<nMaxRows; nRows++ )
		{
			g_DvcDataMem.GetDeviceData( nPos, nCols, nRows, &stDeviceData );

			if( stDeviceData.sExist == eDeviceData_None ) {
				pSpread->SetTextMatrix( nRows + 1, nCols + 1, "" );
				pSpread->SetBackColor( COLOR_LLIGHTGRAY);
			}
			else if(stDeviceData.nBinColor == eGoodBin ) {
				pSpread->SetTextMatrix( nRows + 1, nCols + 1, "G" );
				pSpread->SetBackColor( COLOR_GREEN );
			}
			else {
				pSpread->SetTextMatrix( nRows + 1, nCols + 1, "F" );
				pSpread->SetBackColor( COLOR_DDARKRED );
			}
		}
	}
}

void CTrayViewUnload::SetDisplayBarcode(CSpreadSheet* pSpread, int nPos, int nMaxCols, int nMaxRows)
{
	ST_DD_DEVICE stDeviceData;
	CString str = _T("");
	for(int nCols=0; nCols<nMaxCols; nCols++)
	{
		for(int nRows=0; nRows<nMaxRows;nRows++)
		{
			g_DvcDataMem.GetDeviceData(nPos, nCols, nRows, &stDeviceData);
			
			if( m_CtrlComboKind.GetCurSel() == 1 ){ // 2d barcode
				str.Format("%s",stDeviceData.sz2DBarcode);
			}else if( m_CtrlComboKind.GetCurSel() == 2 ){ // err string
				str.Format("%s",stDeviceData.szErrString);
			}else{
				str.Format("%s",stDeviceData.szBinResult );
			}
			pSpread->SetTextMatrix(nRows+1, nCols+1, (stDeviceData.sExist == eDeviceData_None ? _T("") : str ));
		}
	}
}

void CTrayViewUnload::SetDisplayBarcode2(CSpreadSheet* pSpread, int nPos, int nMaxCols, int nMaxRows)
{
	ST_DD_DEVICE stDeviceData;
	CString str = _T("");
	for (int nCols = 0; nCols < nMaxCols; nCols++)
	{
		for (int nRows = 0; nRows < nMaxRows; nRows++)
		{
			g_DvcDataMem.GetDeviceData(nPos, nCols, nRows, &stDeviceData);

			if (m_CtrlComboKind.GetCurSel() == 1) { // 2d barcode
				str.Format("%s", stDeviceData.sz2DBarcode);
			}
			else if (m_CtrlComboKind.GetCurSel() == 2) { // err string
				str.Format("%s", stDeviceData.szErrString);
			}
			else {
				str.Format("%s", stDeviceData.szBinResult);
			}

			if (stDeviceData.sExist == eDeviceData_None) {
				pSpread->SetTextMatrix(nRows + 1, nCols + 1, "");
				pSpread->SetBackColor(COLOR_LLIGHTGRAY);
			}
			else if (stDeviceData.nBinColor == eGoodBin) {
				pSpread->SetTextMatrix(nRows + 1, nCols + 1, "G");
				pSpread->SetBackColor(COLOR_GREEN);
			}
			else {
				pSpread->SetTextMatrix(nRows + 1, nCols + 1, "F");
				pSpread->SetBackColor(COLOR_DDARKRED);
			}

			pSpread->SetTextMatrix(nRows + 1, nCols + 1, (stDeviceData.sExist == eDeviceData_None ? _T("") : str));
		}
	}
}

void CTrayViewUnload::InitSpread()
{
	int nTrayCntX = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_X	);
	int nTrayCntY = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_Y	);

	m_SprdStacker.SetMaxCols( nTrayCntX );
	m_SprdStacker.SetMaxRows( nTrayCntY );

	m_SprdBarcode.SetMaxCols(nTrayCntX);
	m_SprdBarcode.SetMaxRows(nTrayCntY);

	m_SprdBarcode2.SetMaxCols(nTrayCntX);
	m_SprdBarcode2.SetMaxRows(nTrayCntY);
	
}

BOOL CTrayViewUnload::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}


void CTrayViewUnload::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == 0){
		KillTimer(nIDEvent);

		UpdateSpreadUnLoadSetplate();

		SetTimer(nIDEvent, 800, NULL);
	}

	CDialog::OnTimer(nIDEvent);
}


void CTrayViewUnload::OnClose()
{
	KillTimer(0);

	CDialog::OnClose();
}
