// DataAssignDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "DataAssignDlg.h"
#include "afxdialogex.h"

#define DATA_POS_LOADER		0

// CDataAssignDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDataAssignDlg, CDialog)

CDataAssignDlg::CDataAssignDlg(BOOL bModaless /*= FALSE*/,CWnd* pParent /*=NULL*/)
	: CDialog(CDataAssignDlg::IDD, pParent)
{
	memset(m_nDeviceData, 0, sizeof(m_nDeviceData));
	memset(m_nDeviceDataBefore, 0, sizeof(m_nDeviceDataBefore));
	
	m_nPosNo = LOC_IDX_TRAY_PP_FEEDER;

	m_bSkiptoAssign = FALSE;
	m_bModeModaless = bModaless;

	m_nCntY = 0;
	m_nCntX = 0;

	m_strTitle = _T("");


}

CDataAssignDlg::~CDataAssignDlg()
{
}

void CDataAssignDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SITESET_SAVE, m_BtnSave);
	DDX_Control(pDX, IDCANCEL, m_BtnCancel);
	DDX_Control(pDX, IDC_SPREAD_DVC_DATA, m_CS_SET_DATA);
}


BEGIN_MESSAGE_MAP(CDataAssignDlg, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_SITESET_SAVE, &CDataAssignDlg::OnBnClickedSitesetSave)
	ON_BN_CLICKED(IDC_BTN_SETPLATE1, &CDataAssignDlg::OnBnClickedBtnSetplate1)
	ON_BN_CLICKED(IDC_BTN_SETPLATE2, &CDataAssignDlg::OnBnClickedBtnSetplate2)
	ON_BN_CLICKED(IDC_BTN_TBL1, &CDataAssignDlg::OnBnClickedBtnTbl1)
	ON_BN_CLICKED(IDC_BTN_TBL2, &CDataAssignDlg::OnBnClickedBtnTbl2)
	ON_BN_CLICKED(IDC_BTN_TBL3, &CDataAssignDlg::OnBnClickedBtnTbl3)
	ON_BN_CLICKED(IDC_BTN_TBL4, &CDataAssignDlg::OnBnClickedBtnTbl4)
	ON_BN_CLICKED(IDC_BUTTON_DVC_EXIST, &CDataAssignDlg::OnBnClickedButtonDvcExist)
	ON_BN_CLICKED(IDC_BUTTON_DVC_NOEXIST, &CDataAssignDlg::OnBnClickedButtonDvcNoexist)
	ON_BN_CLICKED(IDCANCEL, &CDataAssignDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_DVC_EXIST_ALL, &CDataAssignDlg::OnBnClickedButtonDvcExistAll)
	ON_BN_CLICKED(IDC_BUTTON_DVC_NOEXIST_ALL, &CDataAssignDlg::OnBnClickedButtonDvcNoexistAll)

	ON_BN_CLICKED(IDC_BUTTON_TRAYPP_SAFETY_MOVE, &CDataAssignDlg::OnBnClickedButtonTrayppSafetyMove)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDataAssignDlg 메시지 처리기입니다.


BOOL CDataAssignDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	MakeLog("[Data AssignDlg Show]");

	g_DMCont.m_dmHandlerOpt.SB(BDM11_GUI_OPEN_HISTORY, TRUE);

	m_BtnCancel.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnCancel.SetIcon(IDI_BTN_EXIT);
	m_BtnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnSave.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnSave.SetIcon(IDI_BTN_SAVE);
	m_BtnSave.SetAlign(CButtonST::ST_ALIGN_VERT);

	int nTrayCntX = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_X);
	int nTrayCntY = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_Y);
	m_CS_SET_DATA.SetMaxCols(nTrayCntX);
	m_CS_SET_DATA.SetMaxRows(nTrayCntY);


	SetDlgItemText(IDC_STATIC_POS_TITLE, _T("Loader"));

	NumResetting();
	UpdateDeviceData();

	if( m_bSkiptoAssign == TRUE ) {
		GetDlgItem( IDC_BTN_SETPLATE1 )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_BTN_SETPLATE2 )->ShowWindow( SW_HIDE );
	}
	else {
		char szCompany[16] = { 0, };
		g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));
	}

	if (m_bModeModaless)
	{
		CRect r;
		GetWindowRect(&r);
		::SetWindowPos(GetSafeHwnd(), HWND_TOPMOST, 150, 250, r.Width(), r.Height(), SWP_NOMOVE | SWP_NOSIZE);
	}

	char szCompany[16] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDataAssignDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


BOOL CDataAssignDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialog::PreTranslateMessage(pMsg);
}


void CDataAssignDlg::OnBnClickedSitesetSave()
{
	int nCntX=0, nCntY=0, nStageNo=0;


	if (!AfxMessageBox(_T("Data Save ??? "), MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		AfxMessageBox(_T("No"));
		return;
	}


	switch(m_nPosNo)
	{
	case LOC_IDX_TRAY_PP_FEEDER:
		{
			nStageNo = eDeviceStage_TRAY_FEEDER;
			nCntX = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_X);
			nCntY = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_Y);
		}break;
	//case LOC_IDX_TRAY_PP_SHT_1  :
	//	{
	//		nStageNo = eDeviceStage_LD_TBL_1;
	//		nCntX = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X)/2;
	//		nCntY = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);
	//	}break;
	//case LOC_IDX_TRAY_PP_SHT_2  :
	//	{
	//		nStageNo = eDeviceStage_LD_TBL_2;
	//		nCntX = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X)/2;
	//		nCntY = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);
	//	}break;
	}
	for(int nY=0; nY < nCntY; nY++){
		for(int nX=0; nX < nCntX; nX++){
			ST_DD_DEVICE stDevice;
			ST_DD_DEVICE stGetDevice;
			if( m_nDeviceData[nY][nX] == DEF_EXIST ){
				stDevice.sExist = DEF_EXIST;
				//sprintf_s(stDevice.szBinResult, sizeof(stDevice.szBinResult), "99");

				g_DvcDataMem.GetDeviceData(nStageNo, nX, nY, &stGetDevice);
				if (strcmp(stGetDevice.szBinResult, "20") == 0) 
				{
					sprintf_s(stDevice.szBinResult, sizeof(stDevice.szBinResult), "20");
					stDevice.sExist = DEF_NOT_EXIST;
				}
				else
				{
					sprintf_s(stDevice.szBinResult, sizeof(stDevice.szBinResult), "99");
				}
			}
		
			g_DvcDataMem.SetDeviceData(nStageNo, nX, nY, &stDevice);


// 			g_DvcDataMem.GetDeviceData(nStageNo, nX, nY, &stGetDevice);
// 			if(strcmp(stGetDevice.szBinResult,"20") != 0)
// 				g_DvcDataMem.SetDeviceData(nStageNo, nX, nY, &stDevice);
		}
	}

	DataAssignChangeLog();

	memcpy(m_nDeviceDataBefore,m_nDeviceData,sizeof(m_nDeviceDataBefore));
}


void CDataAssignDlg::OnBnClickedBtnSetplate1()
{
	SetDlgItemText(IDC_STATIC_POS_TITLE, _T("Loader"));
	m_nPosNo = LOC_IDX_TRAY_PP_FEEDER;
	NumResetting();
	UpdateDeviceData();
}


void CDataAssignDlg::OnBnClickedBtnSetplate2()
{
	// 미사용
	//SetDlgItemText(IDC_STATIC_POS_TITLE, _T("Setplate#2"));
	//m_nPosNo = LOC_IDX_TRAY_PP_CONV_2;
	//NumResetting();
	//UpdateDeviceData();
}


void CDataAssignDlg::OnBnClickedBtnTbl1()
{
	// 미사용
	//SetDlgItemText(IDC_STATIC_POS_TITLE, _T("LoadTable#1"));
	//m_nPosNo = LOC_IDX_TRAY_PP_SHT_1;
	//NumResetting();
	//UpdateDeviceData();
}


void CDataAssignDlg::OnBnClickedBtnTbl2()
{
	// 미사용
	//SetDlgItemText(IDC_STATIC_POS_TITLE, _T("LoadTable#2"));
	//m_nPosNo = LOC_IDX_TRAY_PP_SHT_2;
	//NumResetting();
	//UpdateDeviceData();
}


void CDataAssignDlg::OnBnClickedBtnTbl3()
{
	// 미사용
	//SetDlgItemText(IDC_STATIC_POS_TITLE, _T("LoadTable#3"));
	//m_nPosNo = DATA_POS_LOADTBL3;
	//NumResetting();
	//UpdateDeviceData();
}


void CDataAssignDlg::OnBnClickedBtnTbl4()
{
	// 미사용
	//SetDlgItemText(IDC_STATIC_POS_TITLE, _T("LoadTable#4"));
	//m_nPosNo = DATA_POS_LOADTBL4;
	//NumResetting();
	//UpdateDeviceData();
}


void CDataAssignDlg::OnBnClickedButtonDvcExist()
{
	SetDataSpread(1);
}


void CDataAssignDlg::OnBnClickedButtonDvcNoexist()
{
	SetDataSpread(0);
}

void CDataAssignDlg::SetDataSpread(int nData)
{
	for(int nY=m_CS_SET_DATA.GetSelBlockRow(); nY<=m_CS_SET_DATA.GetSelBlockRow2(); nY++)
	{
		if(nY < 1) { continue; }

		m_CS_SET_DATA.SetRow(nY);
		for(int nX=m_CS_SET_DATA.GetSelBlockCol(); nX<=m_CS_SET_DATA.GetSelBlockCol2(); nX++)
		{
			if(nX < 1) { continue; }
			int nTempority = nY;
			m_CS_SET_DATA.SetCol(nX);
			CString strChange = _T("");

			if(nData > 0)
			{
				m_CS_SET_DATA.SetBackColor(COLOR_YELLOW);
				m_CS_SET_DATA.SetForeColor(COL_BLUE_DEVICE);

				strChange = "O";

				m_CS_SET_DATA.SetRow(0);
				m_CS_SET_DATA.SetCol(nX);
				m_CS_SET_DATA.SetGridColor(COLOR_YELLOW);
				m_CS_SET_DATA.SetRow(nTempority);

				m_nDeviceData[nY-1][nX-1] = nData;
			}
			else
			{
				m_nDeviceData[nY-1][nX-1] = nData;
				m_CS_SET_DATA.SetBackColor(COL_GRAY_BACKGROUND);
				strChange = " ";
			}
			//m_CS_SET_DATA.SetTextMatrix(nY,nX,strChange);
			m_CS_SET_DATA.SetText(strChange);
		}
	}

	m_CS_SET_DATA.Refresh();
}

void CDataAssignDlg::NumResetting()
{
	CString strNumCal =_T("");
	int nCol=0, nRow=0, nCntX=0, nCntY=0;

	switch(m_nPosNo)
	{
	case LOC_IDX_TRAY_PP_FEEDER:
		{
			nCntX = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_X);
			nCntY = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_Y);
		}break;
	case LOC_IDX_TRAY_PP_TBL_1:
	case LOC_IDX_TRAY_PP_TBL_2:
	case LOC_IDX_TRAY_PP_TBL_3:
	case LOC_IDX_TRAY_PP_TBL_4:
		{
			nCntX = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X)/2;
			nCntY = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);
		}break;
	}

	for(nCol=1; nCol<=nCntX; nCol++)
	{
		m_CS_SET_DATA.SetRow(-1000);
		m_CS_SET_DATA.SetCol(nCol);
		m_CS_SET_DATA.SetBackColor(COL_SPREAD_HEADER_COLOR);

		strNumCal.Format("%d", nCol);
		//m_CS_SET_DATA.SetText(strNumCal);
		m_CS_SET_DATA.SetTextMatrix(-1000,nCol,strNumCal);
	}

	for(nRow=1;nRow<=nCntY; nRow++)
	{
		m_CS_SET_DATA.SetCol(-1000);
		m_CS_SET_DATA.SetRow(nRow);
		m_CS_SET_DATA.SetBackColor(COL_SPREAD_HEADER_COLOR);

		strNumCal.Format("%d", nRow);
		//m_CS_SET_DATA.SetText(strNumCal);
		m_CS_SET_DATA.SetTextMatrix(nRow,-1000,strNumCal);
	}
	m_CS_SET_DATA.Refresh();
}

void CDataAssignDlg::UpdateDeviceData()
{
	memset(m_nDeviceData, 0, sizeof(m_nDeviceData));
	int nCntX=0, nCntY=0, nStageNo=0;
	switch(m_nPosNo)
	{
	case LOC_IDX_TRAY_PP_FEEDER:
		{
			nStageNo = eDeviceStage_TRAY_FEEDER;
			nCntX = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_X);
			nCntY = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_Y);
			m_strTitle = _T("Feeder Loader");
		}break;
		//TABLE 현재 사용안함.
	case LOC_IDX_TRAY_PP_TBL_1:
		{
			nStageNo = eDeviceStage_LD_TBL_1; //[작업 필요]
			nCntX = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X)/2;
			nCntY = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);
			m_strTitle = _T("Load Table#1");
		}break;
	case LOC_IDX_TRAY_PP_TBL_3:
		{
			nStageNo = eDeviceStage_LD_TBL_3;
			nCntX = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X)/2;
			nCntY = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);
			m_strTitle = _T("Load Table#2");
		}break;
	}

	SetDlgItemText(IDC_STATIC_POS_TITLE, m_strTitle);

	m_nCntX = nCntX;
	m_nCntY = nCntY;

	m_CS_SET_DATA.SetMaxCols(nCntX);
	m_CS_SET_DATA.SetMaxRows(nCntY);

	for(int i=0; i < nCntY; i++)
	{
		m_CS_SET_DATA.SetRow(i+1);

		for(int ii=0; ii < nCntX; ii++)
		{
			m_CS_SET_DATA.SetCol(ii+1);

			int nDeviceData = 0;

			ST_DD_DEVICE stDeviceData;
			g_DvcDataMem.GetDeviceData(nStageNo, ii, i, &stDeviceData);

			CString StrDeviceData;
			ST_DD_DEVICE stGetDevice;
			if(stDeviceData.sExist == DEF_EXIST)
			{
				m_nDeviceData[i][ii] = DEF_EXIST;
				//m_CS_SET_DATA.SetBackColor(COLOR_YELLOW);
				if (strcmp(stDeviceData.szBinResult, "20") == 0) {
					StrDeviceData = "20";
					m_CS_SET_DATA.SetBackColor(COLOR_GREEN);
				}
				else {
					m_CS_SET_DATA.SetForeColor(COL_BLUE_DEVICE);

					StrDeviceData = "O";
					m_CS_SET_DATA.SetBackColor(COLOR_YELLOW);
				}
			
							
			}
			else
			{
				if (strcmp(stDeviceData.szBinResult, "20") == 0) {
					StrDeviceData = "20";
					m_nDeviceData[i][ii] = DEF_EXIST;
					m_CS_SET_DATA.SetBackColor(COLOR_GREEN);
				}
				else
				{
					m_CS_SET_DATA.SetBackColor(COL_GRAY_BACKGROUND);
					StrDeviceData = " ";
				}
				
			}

			//m_CS_SET_DATA.SetText(StrDeviceData);
			m_CS_SET_DATA.SetTextMatrix(i+1,ii+1,StrDeviceData);
		}
	}
	
	memcpy(m_nDeviceDataBefore,m_nDeviceData,sizeof(m_nDeviceDataBefore));

	m_CS_SET_DATA.Refresh();
}

void CDataAssignDlg::ChkLotState()
{
	BOOL bRetOneCycle = g_DMCont.m_dmEQP.GB( BDM0_ONECYCLE_HISTORY );
	
	GetDlgItem( IDC_BTN_SETPLATE1 )->EnableWindow( bRetOneCycle );
	GetDlgItem( IDC_BUTTON_DVC_EXIST )->EnableWindow( bRetOneCycle );
	GetDlgItem( IDC_BUTTON_DVC_NOEXIST )->EnableWindow( bRetOneCycle );
	GetDlgItem( IDC_SPREAD_DVC_DATA )->EnableWindow( bRetOneCycle );
	GetDlgItem(IDC_BUTTON_DVC_EXIST_ALL)->EnableWindow(bRetOneCycle);
	GetDlgItem(IDC_BUTTON_DVC_NOEXIST_ALL)->EnableWindow(bRetOneCycle);
}

void CDataAssignDlg::DataAssignChangeLog()
{
	std::vector<std::pair<CString,CString>> vData;

	CString strData = _T(""), strAddData =_T(""), strDelData =_T("");
	int nLogCount = 1, nDivisionDataCount = 0;

	for(int i=0; i < m_nCntY; i++)
	{
		for(int ii=0; ii < m_nCntX; ii++)
		{
			std::pair<CString,CString> p;

			if(m_nDeviceDataBefore[i][ii] !=m_nDeviceData[i][ii])
			{
				if(m_nDeviceDataBefore[i][ii] == DEF_EXIST)
				{
					strData.Format(_T("%d-%d"),i+1,ii+1);
					p.first = strData;
				}
				else
				{
					strData.Format(_T("%d-%d"),i+1,ii+1);
					p.second = strData;
				}				

				vData.push_back(p);	
			}
		}
	}
	int nCount =1;
	for(int i=1; i<100; i++){
		nCount= i*100;
		if((int)vData.size()<nCount){
			nDivisionDataCount = i;
			break;
		}
	}

	if(!(vData.empty()))
	{
		for(int i=0; i< (int)vData.size(); i++)
		{
			std::pair<CString,CString> p;
			p = vData[i];
			strDelData += p.first;
			strAddData += p.second;

			if(( ((int)vData.size()/nDivisionDataCount)*nLogCount) == i || (int)vData.size()-1 == i){

				if(strAddData.Compare("") != 0)
				{
					MakeLog("[MENU = Data Assign, %s ADD Device Data= %s]",m_strTitle, strAddData);
				}
				if(strDelData.Compare("") != 0)
				{
					MakeLog("[MENU = Data Assign, %s Delete Device Data= %s]",m_strTitle, strDelData);
				}
				strAddData =_T("");
				strDelData =_T("");
				if(nLogCount<nDivisionDataCount-1){
					nLogCount ++;
				}
			}else if(vData.size()-1 != i){

				if(p.first != "")
				{
					strDelData +=", ";
				}
				if(p.second != "")
				{
					strAddData +=", ";
				}
			}
		}
		vData.clear();
	}
}

void CDataAssignDlg::MakeLog(LPCTSTR fmt, ...)
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


void CDataAssignDlg::OnBnClickedCancel()
{
	g_DMCont.m_dmHandlerOpt.SB(BDM11_GUI_OPEN_HISTORY, FALSE);

	if (m_bModeModaless == TRUE)
		delete this;
	else
		CDialog::OnCancel();	
}


void CDataAssignDlg::OnBnClickedButtonDvcExistAll()
{
	SetDataSpreadAll(1);
}


void CDataAssignDlg::OnBnClickedButtonDvcNoexistAll()
{
	SetDataSpreadAll(0);
}

void CDataAssignDlg::SetDataSpreadAll(int nData)
{
	for (int nY = 1; nY <= m_CS_SET_DATA.GetMaxRows(); nY++)
	{
		if (nY < 1) { continue; }

		m_CS_SET_DATA.SetRow(nY);
		for (int nX = 1; nX <= m_CS_SET_DATA.GetMaxCols(); nX++)
		{
			if (nX < 1) { continue; }
			int nTempority = nY;
			m_CS_SET_DATA.SetCol(nX);
			CString strChange = _T("");

			if (nData > 0)
			{
				m_CS_SET_DATA.SetBackColor(COLOR_YELLOW);
				m_CS_SET_DATA.SetForeColor(COL_BLUE_DEVICE);

				strChange = "O";

				m_CS_SET_DATA.SetRow(0);
				m_CS_SET_DATA.SetCol(nX);
				m_CS_SET_DATA.SetGridColor(COLOR_YELLOW);
				m_CS_SET_DATA.SetRow(nTempority);

				m_nDeviceData[nY - 1][nX - 1] = nData;
			}
			else
			{
				m_nDeviceData[nY - 1][nX - 1] = nData;
				m_CS_SET_DATA.SetBackColor(COL_GRAY_BACKGROUND);
				strChange = " ";
			}
			m_CS_SET_DATA.SetText(strChange);
		}
	}

	m_CS_SET_DATA.Refresh();
}



void CDataAssignDlg::OnBnClickedButtonTrayppSafetyMove()
{
	g_McComm.CmdTrayPPSafetyMovePos();
}


void CDataAssignDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == 1000)
	{
		if (g_DMCont.m_dmEQP.GB(BDM0_EQP_RUN_STATE))
		{
			GetDlgItem(IDC_BUTTON_TRAYPP_SAFETY_MOVE)->EnableWindow(FALSE);
		}
	}

	CDialog::OnTimer(nIDEvent);
}
