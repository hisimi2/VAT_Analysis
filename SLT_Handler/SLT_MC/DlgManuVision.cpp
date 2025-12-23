// DlgManuVision.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgManuVision.h"
#include "afxdialogex.h"
#include "DlgManuVisionSetup.h"


// CDlgManuVision 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgManuVision, CDialogEx)

CDlgManuVision::CDlgManuVision(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgManuVision::IDD, pParent)
{
	m_vVisionCommObj.clear();
	//m_vVisionCommObj.push_back(g_pBarcode);
	m_vVisionCommObj.push_back(g_pVisionTestHand_1);
	m_vVisionCommObj.push_back(g_pVisionTestHand_2);
}

CDlgManuVision::~CDlgManuVision()
{
}

void CDlgManuVision::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TARGET1, m_cbTargetSel1);
	DDX_Control(pDX, IDC_COMBO_TARGET2, m_cbTargetSel2);
	DDX_Control(pDX, IDC_EDIT_VISION_MANU_LOG, m_editManuOperEvent);
}


BEGIN_MESSAGE_MAP(CDlgManuVision, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CONN1, &CDlgManuVision::OnBnClickedButtonConn1)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_CONN2, &CDlgManuVision::OnBnClickedButtonConn2)
	ON_BN_CLICKED(IDC_BUTTON_CONN3, &CDlgManuVision::OnBnClickedButtonConn3)
	ON_CBN_SELCHANGE(IDC_COMBO_TARGET1, &CDlgManuVision::OnCbnSelchangeComboTarget1)
	ON_CBN_SELCHANGE(IDC_COMBO_TARGET2, &CDlgManuVision::OnCbnSelchangeComboTarget2)
	ON_BN_CLICKED(IDC_BUTTON_CMD_SET_COK, &CDlgManuVision::OnBnClickedButtonCmdSetCok)
	ON_BN_CLICKED(IDC_BUTTON_CMD_CLEAN_DISK, &CDlgManuVision::OnBnClickedButtonCmdCleanDisk)
	ON_BN_CLICKED(IDC_BUTTON_CMD_REQ_RESULT, &CDlgManuVision::OnBnClickedButtonCmdReqResult)
	ON_BN_CLICKED(IDC_BUTTON_CMD_INSP_READY, &CDlgManuVision::OnBnClickedButtonCmdInspReady)
	ON_BN_CLICKED(IDC_BUTTON_CMD_VISION_SHUT_DOWN, &CDlgManuVision::OnBnClickedButtonCmdVisionShutDown)
	ON_BN_CLICKED(IDC_BUTTON_CMD_REQ_MEASURE, &CDlgManuVision::OnBnClickedButtonCmdReqMeasure)
	ON_MESSAGE(UM_VISION_IF_SOCK_EVENT, OnVisionSockEvent)
	ON_MESSAGE(UM_VISION_IF_LOG_MSG, OnVisionLogMsg)
	ON_BN_CLICKED(IDC_BUTTON_CMD_VISION_READY_START, &CDlgManuVision::OnBnClickedButtonCmdVisionReadyStart)
	ON_BN_CLICKED(IDC_BUTTON_CMD_VISION_READY_END, &CDlgManuVision::OnBnClickedButtonCmdVisionReadyEnd)
	ON_BN_CLICKED(IDOK, &CDlgManuVision::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgManuVision 메시지 처리기입니다.




BOOL CDlgManuVision::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CDlgManuVision::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//m_cbTargetSel1.AddString("2D Barcode");
	m_cbTargetSel1.AddString("Test Hand 1");
	m_cbTargetSel1.AddString("Test Hand 2");
	m_cbTargetSel1.AddString("All");
	m_cbTargetSel1.SetCurSel(0);

	//for(int i=0; i<4; i++){
	//	CString strTarget;
	//	strTarget.Format("Loading Table %d", i+1);
	//	m_cbTargetSel2.AddString(strTarget);
	//}

	for(int i=0; i<(eMaxPressUnitCount/2); i++){
		CString strTarget;
		strTarget.Format("Station %d", i+1);
		m_cbTargetSel2.AddString(strTarget);
	}

	m_cbTargetSel2.SetCurSel(0);

	g_pBarcode->AttachObserver(this);
	g_pVisionTestHand_1->AttachObserver(this);
	g_pVisionTestHand_2->AttachObserver(this);

	SetTimer(0, 500, NULL);

	MakeLog("Vision Intergace Dialog Show");
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgManuVision::OnDestroy()
{
	CDialogEx::OnDestroy();

	KillTimer(0);

	g_pBarcode->DetachObserver(this);
	g_pVisionTestHand_1->DetachObserver(this);
	g_pVisionTestHand_2->DetachObserver(this);
}


void CDlgManuVision::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch( nIDEvent )
	{
	case 0:
		{
			KillTimer(0);
			UpdateVisionState();
			SetTimer(0, 500, NULL);
		}break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CDlgManuVision::UpdateVisionState()
{
	std::vector<int> vCtrlIdConnSts;
	//vCtrlIdConnSts.push_back(IDC_CHECK_VISION_CONN_STATE1);
	vCtrlIdConnSts.push_back(IDC_CHECK_VISION_CONN_STATE2);
	vCtrlIdConnSts.push_back(IDC_CHECK_VISION_CONN_STATE3);

	for(std::vector<int>::size_type i=0; i<vCtrlIdConnSts.size(); ++i)
	{
		CVisionComm* pVisionComm = dynamic_cast<CVisionComm*>(m_vVisionCommObj[i]);
		if( pVisionComm != NULL )
		{
			BOOL bConnected = pVisionComm->IsConnected();
			CButton* pBtn = (CButton*)GetDlgItem(vCtrlIdConnSts[i]);
			if( pBtn )
			{
				pBtn->SetCheck(bConnected);
			}
		}
	}
}

void CDlgManuVision::OnConnection(int nVisionPc)
{
	CDlgManuVisionSetup dlg;
	dlg.m_nVisionPc = nVisionPc;
	dlg.DoModal();
}

void CDlgManuVision::OnBnClickedButtonConn1()
{
	MakeLog("Barcode Reader Connect Button Click");
	OnConnection( eVision_BarCode );
}

void CDlgManuVision::OnBnClickedButtonConn2()
{
	MakeLog("Test Hand1 Connect Button Click");
	OnConnection( eVision_TestSiteUp );
}


void CDlgManuVision::OnBnClickedButtonConn3()
{
	MakeLog("Test Hand2 Connect Button Click");
	OnConnection( eVision_TestSiteDown );
}

void CDlgManuVision::OnCbnSelchangeComboTarget1()
{
	//if( m_cbTargetSel1.GetCurSel() == 0 ){
	//	// bar code
	//	m_cbTargetSel2.ResetContent();
	//	for(int i=0; i<eMaxLoadTblCount; i++){
	//		CString strTarget;
	//		strTarget.Format("Loading Table %d", i+1);
	//		m_cbTargetSel2.AddString(strTarget);
	//	}
	//	m_cbTargetSel2.SetCurSel(0);
	//}
	//else if( m_cbTargetSel1.GetCurSel() == 1 ||  m_cbTargetSel1.GetCurSel() == 2 ){
		m_cbTargetSel2.ResetContent();
		for(int i=0; i<(eMaxPressUnitCount/2); i++){
			CString strTarget;
			strTarget.Format("Station %d", i+1);
			m_cbTargetSel2.AddString(strTarget);
		}
		m_cbTargetSel2.SetCurSel(0);
	//}
}


void CDlgManuVision::OnCbnSelchangeComboTarget2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CDlgManuVision::OnBnClickedButtonCmdSetCok()
{
	int nVision = m_cbTargetSel1.GetCurSel();
	int nErr = 0;
	if(nVision < /*3*/2 )
	{
		nErr = m_vVisionCommObj[nVision]->Cmd_SetCok();
	}
	else
	{
		for(std::vector<CVisionComm*>::size_type i=0; i<m_vVisionCommObj.size(); i++)
		{
			nErr = m_vVisionCommObj[i]->Cmd_SetCok();
		}
	}
	if( nErr != 0 ){
		AfxMessageBox("Error");
	}
}


void CDlgManuVision::OnBnClickedButtonCmdCleanDisk()
{
	int nVision = m_cbTargetSel1.GetCurSel();
	int nErr = 0;
	if(nVision < /*3*/2 )
	{
		nErr = m_vVisionCommObj[nVision]->Cmd_ReqCleanDisk();
	}
	else
	{
		for(std::vector<CVisionComm*>::size_type i=0; i<m_vVisionCommObj.size(); i++)
		{
			nErr = m_vVisionCommObj[i]->Cmd_ReqCleanDisk();
		}
	}
	if( nErr != 0 ){
		AfxMessageBox("Error");
	}
}


void CDlgManuVision::OnBnClickedButtonCmdReqResult()
{
	int nVision = m_cbTargetSel1.GetCurSel();

	switch( nVision )
	{
	//case 0:
	//	{
	//		int nLoadTbl = m_cbTargetSel2.GetCurSel();
	//		int nErr = g_pBarcode->Cmd_ReqMeasureResult(VISION_CAM_ID_2DBRACODE_1 + nLoadTbl);
	//		if( nErr != 0 ){
	//			CString err_msg;
	//			err_msg.Format("Message Send Error.[CODE = %d]", nErr);
	//			AfxMessageBox(err_msg);
	//		}
	//	}break;
	case 0:
		{
			int nSite = m_cbTargetSel2.GetCurSel();
			int nErr = g_pVisionTestHand_1->Cmd_ReqMeasureResult(VISION_CAM_ID_TESTSITE_UP_1 + nSite);
			if( nErr != 0 ){
				CString err_msg;
				err_msg.Format("Message Send Error.[CODE = %d]", nErr);
				AfxMessageBox(err_msg);
			}
		}break;
	case 1:
		{
			int nSite = m_cbTargetSel2.GetCurSel();
			int nErr = g_pVisionTestHand_2->Cmd_ReqMeasureResult(VISION_CAM_ID_TESTSITE_DOWN_1 + nSite);
			if( nErr != 0 ){
				CString err_msg;
				err_msg.Format("Message Send Error.[CODE = %d]", nErr);
				AfxMessageBox(err_msg);
			}
		}break;
	default:
		{
			AfxMessageBox("Invalid target selection!!!");
		}break;
	}
}


void CDlgManuVision::OnBnClickedButtonCmdInspReady()
{
	int nVision = m_cbTargetSel1.GetCurSel();
	int nErr = 0;
	if(nVision < /*3*/2 )
	{
		int nCamNo = VISION_CAM_ID_2DBRACODE_1 + m_cbTargetSel2.GetCurSel();
		nErr = m_vVisionCommObj[nVision]->Cmd_ReqInspReady(nCamNo);
	}
	else
	{
		//for(int i=0; i<eMaxLoadTblCount; i++)
		//{
		//	g_pBarcode->Cmd_ReqInspReady(VISION_CAM_ID_2DBRACODE_1+i);
		//}

		//for(int i=0; i<(eMaxPressUnitCount/2); i++)
		//{
		//	g_pVisionTestHand_1->Cmd_ReqInspReady(VISION_CAM_ID_TESTSITE_UP_(0));
		//	g_pVisionTestHand_2->Cmd_ReqInspReady(VISION_CAM_ID_TESTSITE_DOWN_(0));
		//}
	}
	if( nErr != 0 ){
		AfxMessageBox("Error");
	}
}


void CDlgManuVision::OnBnClickedButtonCmdVisionShutDown()
{
	int nVision = m_cbTargetSel1.GetCurSel();
	int nErr = 0;
	if(nVision < /*3*/2 )
	{
		nErr = m_vVisionCommObj[nVision]->Cmd_ReqVisionShutDown();
	}
	else
	{
		for(std::vector<CVisionComm*>::size_type i=0; i<m_vVisionCommObj.size(); i++)
		{
			nErr = m_vVisionCommObj[i]->Cmd_ReqVisionShutDown();
		}
	}
	if( nErr != 0 ){
		AfxMessageBox("Error");
	}
}


void CDlgManuVision::OnBnClickedButtonCmdReqMeasure()
{
	int nVision = m_cbTargetSel1.GetCurSel();

	switch( nVision )
	{
	//case 0:
	//	{
	//		int nLoadTbl = m_cbTargetSel2.GetCurSel();
	//		int nErr = g_pBarcode->Cmd_ReqMeasure(VISION_CAM_ID_2DBRACODE_1 + nLoadTbl);
	//		if( nErr != 0 ){
	//			CString err_msg;
	//			err_msg.Format("Message Send Error.[CODE = %d]", nErr);
	//			AfxMessageBox(err_msg);
	//		}
	//	}break;
	case 0:
		{
			int nSite = m_cbTargetSel2.GetCurSel();

			_tVisionMsg tVisionMsgMeasure;
			std::vector<CPoint> vPicker;
			vPicker.push_back(CPoint(0, 0));
			int nErr = g_pVisionTestHand_1->Cmd_ReqMeasure_StationCam(nSite, &tVisionMsgMeasure, vPicker,CPoint(0,0), nSite);
			if( nErr != 0 ){
				CString err_msg;
				err_msg.Format("Message Send Error.[CODE = %d]", nErr);
				AfxMessageBox(err_msg);
			}
		}break;
	case 1:
		{
			int nSite = m_cbTargetSel2.GetCurSel();
			_tVisionMsg tVisionMsgMeasure;
			std::vector<CPoint> vPicker;
			vPicker.push_back(CPoint(0, 0));
			int nErr = g_pVisionTestHand_2->Cmd_ReqMeasure_StationCam(nSite + (eMaxPressUnitCount/2), &tVisionMsgMeasure, vPicker, CPoint(0, 0), nSite);
	
			if( nErr != 0 ){
				CString err_msg;
				err_msg.Format("Message Send Error.[CODE = %d]", nErr);
				AfxMessageBox(err_msg);
			}
		}break;
	default:
		{
			AfxMessageBox("Invalid target selection!!!");
		}break;
	}
}


void CDlgManuVision::GetTimeString(CString& strTimeString)
{
	SYSTEMTIME t;
	::GetLocalTime(&t);
	strTimeString.Format("[%02d:%02d.%03d]", t.wMinute, t.wSecond, t.wMilliseconds);
}

void CDlgManuVision::UpdateEventMessage(const char* fmt, ...)
{
	char szEventMessage[1024]={0,};
	va_list va;
	va_start(va, fmt);
	vsprintf_s(szEventMessage, sizeof(szEventMessage), fmt, va);
	va_end(va);

	if( m_editManuOperEvent.GetLineCount() > 10 ){
		m_editManuOperEvent.SetWindowText("");
	}
	CString strTimeString;
	GetTimeString( strTimeString );

	CString strTemp;
	m_editManuOperEvent.GetWindowText(strTemp);
	if( strTemp != "" ){
		strTemp += _T("\r\n");
	}
	strTemp += strTimeString + _T(" ");
	strTemp += szEventMessage;

	m_editManuOperEvent.SetWindowText( strTemp );
}


LRESULT CDlgManuVision::OnVisionSockEvent(WPARAM wParma, LPARAM lParam)
{
	CString strVisionPc;
	if( wParma == eVision_BarCode ){
		strVisionPc = "Barcode Reader";
	}else if( wParma == eVision_TestSiteUp ){
		strVisionPc = "Test hand 1 Inspector";
	}else{
		strVisionPc = "Test hand 2 Inspector";
	}

	int nEventID = (int)lParam;
	CString strEventMsg;
	switch ( nEventID )
	{
	case IPC_EVT_CONNECTED:
		{
			strEventMsg = "connected";
		}break;
	case IPC_EVT_DISCONNECTED:
		{
			strEventMsg = "disconnected";
		}break;
	case IPC_EVT_RCV_EMPTY:
		{
			strEventMsg = "receive empty message";
		}break;
	case IPC_EVT_RCV_ERROR:
		{
			strEventMsg = "receive error";
		}break;
	case IPC_EVT_TM_OVER_MES:
		{
			strEventMsg = "timeout occured";
		}break;
	}


	UpdateEventMessage("[sock event] vision unit = %s, event = %s", strVisionPc, strEventMsg); 
	return 0;
}

LRESULT CDlgManuVision::OnVisionLogMsg(WPARAM wParma, LPARAM lParam)
{
	CString strVisionPc;
	if( wParma == eVision_BarCode ){
		strVisionPc = "Barcode Reader";
	}else if( wParma == eVision_TestSiteUp ){
		strVisionPc = "Test hand 1 Inspector";
	}else{
		strVisionPc = "Test hand 2 Inspector";
	}

	UpdateEventMessage("%s - %s", strVisionPc, (char*)lParam);

	return 0;
}

void CDlgManuVision::ButtonClickLog(int nBtnNum)
{
	int nVision = m_cbTargetSel1.GetCurSel();
	int nCamera = m_cbTargetSel2.GetCurSel();
	
	//eVision_BarCode=0,
	//	eVision_TestSiteUp,
	//	eVision_TestSiteDown,
	//  All    3
}
void CDlgManuVision::MakeLog(LPCTSTR fmt, ...)
{
	TCHAR tmpMsg[1024] = {0,};
	if(fmt)
	{
		va_list argList;
		va_start(argList, fmt);
		_vstprintf_s(tmpMsg, fmt, argList);
		va_end(argList);
	}
	TWLOG.MakeLog(static_cast<int>(eLogID_SYSTEM), Debug, "", __LINE__, NULL, tmpMsg);
}

void CDlgManuVision::OnBnClickedButtonCmdVisionReadyStart()
{
	int nVision = m_cbTargetSel1.GetCurSel();
	int nStationNum = m_cbTargetSel2.GetCurSel();
	nStationNum += 1;
	int nErr = 0;
	if (nVision == /*3*/0)
	{		
		nErr = m_vVisionCommObj[nVision]->Cmd_VisionReady_StationCam(eCamStateRun_LightON, nStationNum);
	}
	else if (nVision == /*3*/1)
	{
		nErr = m_vVisionCommObj[nVision]->Cmd_VisionReady_StationCam(eCamStateRun_LightON, nStationNum + (eMaxPressUnitCount / 2));
	}
	else
	{
		for (std::vector<CVisionComm*>::size_type i = 0; i<m_vVisionCommObj.size(); i++)
		{
			if(i == 0)
				nErr = m_vVisionCommObj[i]->Cmd_VisionReady_StationCam(eCamStateRun_LightON, nStationNum);
			else
				nErr = m_vVisionCommObj[i]->Cmd_VisionReady_StationCam(eCamStateRun_LightON, nStationNum + (eMaxPressUnitCount / 2));
		}
	}
	if (nErr != 0) {
		AfxMessageBox("Error");
	}
}


void CDlgManuVision::OnBnClickedButtonCmdVisionReadyEnd()
{
	int nVision = m_cbTargetSel1.GetCurSel();
	int nStationNum = m_cbTargetSel2.GetCurSel();
	int nErr = 0;
	nStationNum += 1;
	if (nVision == /*3*/0)
	{
		nErr = m_vVisionCommObj[nVision]->Cmd_VisionReady_StationCam(eCamStateRun_LightOFF, nStationNum);
	}
	else if (nVision == /*3*/1) {
		nErr = m_vVisionCommObj[nVision]->Cmd_VisionReady_StationCam(eCamStateRun_LightOFF, nStationNum + (eMaxPressUnitCount / 2));
	}
	else
	{
		for (std::vector<CVisionComm*>::size_type i = 0; i<m_vVisionCommObj.size(); i++)
		{
			if (i == 0)
				nErr = m_vVisionCommObj[i]->Cmd_VisionReady_StationCam(eCamStateRun_LightOFF, nStationNum);
			else
				nErr = m_vVisionCommObj[i]->Cmd_VisionReady_StationCam(eCamStateRun_LightOFF, nStationNum + (eMaxPressUnitCount / 2));
		}
	}
	if (nErr != 0) {
		AfxMessageBox("Error");
	}
}


void CDlgManuVision::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}
