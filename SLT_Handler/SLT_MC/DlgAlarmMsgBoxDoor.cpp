// DlgAlarmMsgBox.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgAlarmMsgBoxDoor.h"
#include "afxdialogex.h"
#include <string>
#include <vector>
using namespace std;

static int m_nPosicionControl[eMaxDoorSensor] = { IDC_EDIT_0 ,IDC_EDIT_1,IDC_EDIT_2,IDC_EDIT_3,IDC_EDIT_4,IDC_EDIT_5,IDC_EDIT_6,IDC_EDIT_7,IDC_EDIT_8,IDC_EDIT_9,IDC_EDIT_10 };

// CDlgAlarmMsgBoxDoor 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgAlarmMsgBoxDoor, CDialogEx)

CDlgAlarmMsgBoxDoor::CDlgAlarmMsgBoxDoor(ST_ALARM_RPT_OBJ* pstAlarmObj, CParamList* pAlarmData, CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgAlarmMsgBoxDoor::IDD, pParent)
{
	m_AlarmData = *pAlarmData;
	m_pSender = (CBaseTaskManager*)pstAlarmObj->pSender;
	m_nAlid = pstAlarmObj->nAlid;
	//m_vFailPicker = pstAlarmObj->vFailPicker;
	// vFailPicker X = Door 위치 번호, vFailPicker Picker Y = 사용 안함
	for (int i = 0; i < (int)pstAlarmObj->vFailPicker.size(); i++)
	{
		m_vErrorPos.push_back(pstAlarmObj->vFailPicker[i].x);
	}
	
	m_bDuplicatealarm = pstAlarmObj->bDuplicateAlarm;

	m_vUserSelectButtonCtrl.clear();
	m_vUserSelectButtonCtrl.push_back(ID_BUTTON_USER_SEL_1);
	m_vUserSelectButtonCtrl.push_back(ID_BUTTON_USER_SEL_2);
	m_vUserSelectButtonCtrl.push_back(ID_BUTTON_USER_SEL_3);
	m_vUserSelectButtonCtrl.push_back(ID_BUTTON_USER_SEL_4);

	for (int i = 0; i < eMaxDoorSensor; i++)
	{
		bIsRed[i] = FALSE;
	}
}

CDlgAlarmMsgBoxDoor::~CDlgAlarmMsgBoxDoor()
{

}

void CDlgAlarmMsgBoxDoor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPRD_PICKER, m_SprdPicker);
	DDX_Control(pDX, IDC_STATIC_PICTURE_DOOR2, m_bitPic1);

	for (int i = 0; i < eMaxDoorSensor; i++) {
		DDX_Control(pDX, m_nPosicionControl[i], m_editPosition[i]);
	}
}


BEGIN_MESSAGE_MAP(CDlgAlarmMsgBoxDoor, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CDlgAlarmMsgBoxDoor::OnBnClickedOk)
	ON_BN_CLICKED(ID_BUTTON_USER_SEL_1, &CDlgAlarmMsgBoxDoor::OnBnClickedButtonUserSel1)
	ON_BN_CLICKED(ID_BUTTON_USER_SEL_2, &CDlgAlarmMsgBoxDoor::OnBnClickedButtonUserSel2)
	ON_BN_CLICKED(ID_BUTTON_USER_SEL_3, &CDlgAlarmMsgBoxDoor::OnBnClickedButtonUserSel3)
	ON_BN_CLICKED(ID_BUTTON_USER_SEL_4, &CDlgAlarmMsgBoxDoor::OnBnClickedButtonUserSel4)
	ON_BN_CLICKED(ID_BUTTON_SET_MBTYPE, &CDlgAlarmMsgBoxDoor::OnBnClickedButtonSetMbtype)
END_MESSAGE_MAP()


// CDlgAlarmMsgBoxDoor 메시지 처리기입니다.


BOOL CDlgAlarmMsgBoxDoor::PreTranslateMessage(MSG* pMsg)
{
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4))
	{
		return TRUE;
	}
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))
	{
		return TRUE;
	}
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ESCAPE))
	{
		return TRUE;
	}
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_SPACE))
	{
		return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CDlgAlarmMsgBoxDoor::DisplayErrMsg()
{
	CString strAlid;
	strAlid.Format("%d", m_nAlid);
	// Error Code
	SetDlgItemText(IDC_TEXTBOX_ERRCODE, strAlid);//m_tErrorMsg.szEQP_ALID);

												 // Part
	SetDlgItemText(IDC_TEXTBOX_SUBCODE, m_tErrorMsg.szPart);

	// Unit Name
	SetDlgItemText(IDC_TEXTBOX_UNITNAME, "-");


	// 영문은 Default로 표시 함.
	SetDlgItemText(IDC_MYMSGEDIT_ENG, m_strMessageEng);
	int nLang = g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_LANGUAGE);
	if (nLang == eSysLang_Eng) {
		GetDlgItem(IDC_MYMSGEDIT_ENG)->ShowWindow(SW_HIDE);
	}
	// system lang 표시
	SetDlgItemText(IDC_MYMSGEDIT_LANG_SETUP, m_strMessageSysLang);
}

void CDlgAlarmMsgBoxDoor::InitControls()
{
	g_DB.SELECT_ERROR_MSG_TBL(m_nAlid, m_tErrorMsg);
	char szUserId[64] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_OperatorID, szUserId, sizeof(szUserId));

	int nLang = g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_LANGUAGE);
	if (nLang == eSysLang_Kor) {
		m_strMessageSysLang = m_tErrorMsg.strKorea;
	}
	else if (nLang == eSysLang_Chi) {
		m_strMessageSysLang = m_tErrorMsg.strChina;
	}
	else if (nLang == eSysLang_Jap) {
		m_strMessageSysLang = m_tErrorMsg.strJapan;
	}
	else {
		m_strMessageSysLang = m_tErrorMsg.strEnglish;
	}
	m_strMessageEng = m_tErrorMsg.strEnglish;

	if (m_AlarmData.m_fnGetCount() > 0)
	{
		CString strAlarmData;
		//m_AlarmData.m_fnGetDelimitedText(strAlarmData);
		for (int i = 0; i < m_AlarmData.m_fnGetCount(); i++)
		{
			strAlarmData = m_AlarmData.m_fnGetItemString(i);
			m_strMessageSysLang += "\r\n";
			m_strMessageSysLang += "[" + strAlarmData + "]";

			m_strMessageEng += "\r\n";
			m_strMessageEng += "[" + strAlarmData + "]";
		}
	}

	InitUserSelectionButton(m_tErrorMsg.nMB_Type, m_nAlid);

	SYSTEMTIME t;
	::GetLocalTime(&t);

	m_tErrorLog.SetErrLogFromErrMsg(m_tErrorMsg, m_strMessageSysLang, t, szUserId);
	DisplayErrMsg();
}


BOOL CDlgAlarmMsgBoxDoor::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitControls();
	InitSpread();
	InitReSize();
	InitPosition();

	GetDlgItem(IDC_TEXTBOX_ERRCODE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_TEXTBOX_SUBCODE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_TEXTBOX_UNITNAME)->ShowWindow(SW_SHOW);

	//#ifdef _DEBUG
	//	int nDesktopWidth = GetSystemMetrics(SM_CXFULLSCREEN);		//by mjna [140930] Message Box Always Center
	//	int nDesktopHeight = GetSystemMetrics(SM_CYFULLSCREEN);
	//	
	//	CRect r;
	//	GetWindowRect(&r);
	//
	//	int x = nDesktopWidth / 2 - r.Width() / 2;
	//	int y = nDesktopHeight / 2 - r.Height() / 2;
	//
	//	
	//	SetWindowPos(&wndTop, x, y,/*150, 250,*/ r.Width(), r.Height(), 0);
	//#endif

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgAlarmMsgBoxDoor::OnDestroy()
{
	CDialogEx::OnDestroy();

	KillTimer(1);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CDlgAlarmMsgBoxDoor::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CDlgAlarmMsgBoxDoor::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case 1:
	{
		KillTimer(nIDEvent);

		for (std::vector<int>::iterator iter = m_vErrorPos.begin(); iter != m_vErrorPos.end(); iter++)
		{	
			bIsRed[*iter] = !bIsRed[*iter];
			m_editPosition[*iter].ShowWindow((bIsRed[*iter] == TRUE ? SW_SHOW : SW_HIDE));
		}
		
// 		if (m_nPosition > 0) {
// 			bIsRed = !bIsRed;
// 			//m_editPosition[m_nPosition - 1].SetBackColor((bIsRed == TRUE ? COLOR_RED : COLOR_LLIGHTGRAY));
// 			m_editPosition[m_nPosition - 1].ShowWindow((bIsRed == TRUE ? SW_SHOW : SW_HIDE));
// 		}
		SetTimer(1, 1000, NULL);
	}break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CDlgAlarmMsgBoxDoor::InitUserSelectionButton(int nMBTYPE, int nAlid)
{
	std::vector<std::string> vBtnCaption;

	vBtnCaption.clear();
	m_vMessageBoxId.clear();

	char szCompany[16] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

	switch (nMBTYPE)
	{
	case TWMB_OK:
	{
		vBtnCaption.push_back("OK");
		m_vMessageBoxId.push_back(TWMB_ID_OK);
	}break;
	case TWMB_OKCANCEL:
	{
		vBtnCaption.push_back("OK");
		vBtnCaption.push_back("CANCEL");
		m_vMessageBoxId.push_back(TWMB_ID_OK);
		m_vMessageBoxId.push_back(TWMB_ID_CANCEL);
	}break;
	case TWMB_RETRY:
	{
		vBtnCaption.push_back("RETRY");
		m_vMessageBoxId.push_back(TWMB_ID_RETRY);
	}break;
	case TWMB_RETRYSKIP:
	{
		vBtnCaption.push_back("RETRY");
		vBtnCaption.push_back("SKIP");
		m_vMessageBoxId.push_back(TWMB_ID_RETRY);
		m_vMessageBoxId.push_back(TWMB_ID_SKIP);
	}break;
	case TWMB_CLEANOUTRETRYSKIP:
	{
		vBtnCaption.push_back("CLEAN OUT");
		vBtnCaption.push_back("RETRY");
		vBtnCaption.push_back("SKIP");
		m_vMessageBoxId.push_back(TWMB_ID_CLEANOUT);
		m_vMessageBoxId.push_back(TWMB_ID_RETRY);
		m_vMessageBoxId.push_back(TWMB_ID_SKIP);
	}break;
	case TWMB_SKIP_PASSWORD:
	{
		vBtnCaption.push_back("P-SKIP");
		m_vMessageBoxId.push_back(TWMB_ID_P_SKIP);
	}break;
	case TWMB_TRAY_END_FEED:
	{
		//vBtnCaption.push_back("TRAYEND");
		vBtnCaption.push_back("TRAYFEED");
		vBtnCaption.push_back("SKIP");
		//m_vMessageBoxId.push_back(TWMB_ID_TRAYEND);
		m_vMessageBoxId.push_back(TWMB_ID_TRAYFEED);
		m_vMessageBoxId.push_back(TWMB_ID_SKIP);
	}break;
	case TWMB_LOAD_PARTIAL_TRAY_END:
	{
		vBtnCaption.push_back("RETRY");
		vBtnCaption.push_back("SKIP");
		vBtnCaption.push_back("TRAY_END");
		m_vMessageBoxId.push_back(TWMB_ID_RETRY);
		m_vMessageBoxId.push_back(TWMB_ID_SKIP);
		m_vMessageBoxId.push_back(TWMB_ID_PARTIAL_TRAYEND);

		if (strcmp(szCompany, DEF_COMPANY_AMKOR) == 0)
		{
			vBtnCaption.push_back("SKIP_DataAssign");
			m_vMessageBoxId.push_back(TWMB_ID_SKIP_TO_ASSIGN);
		}

	}break;
	case TWMB_ID_SKIP_TO_DATA_ASSIGN:
	{
		vBtnCaption.push_back("RETRY");
		vBtnCaption.push_back("SKIP");
		vBtnCaption.push_back("SKIP_DataAssign");
		m_vMessageBoxId.push_back(TWMB_ID_RETRY);
		m_vMessageBoxId.push_back(TWMB_ID_SKIP);
		m_vMessageBoxId.push_back(TWMB_ID_SKIP_TO_ASSIGN);
	}break;
	case TWMB_OK_CLEAR:
	{
		vBtnCaption.push_back("OK");
		vBtnCaption.push_back("Clear");
		m_vMessageBoxId.push_back(TWMB_ID_OK);
		m_vMessageBoxId.push_back(TWMB_ID_SKIP);
	}break;
	case TWMB_SKIP:
	{
		vBtnCaption.push_back("SKIP");
		m_vMessageBoxId.push_back(TWMB_ID_SKIP);
	}break;
	case TWMB_AXIS_MOVE:
	{
		vBtnCaption.push_back("OK");
		vBtnCaption.push_back("AXIS_MOVE");
		//m_vMessageBoxId.push_back(TWMB_ID_OK);
	}break;
	case TWMB_QA_QTY:
	{
		vBtnCaption.push_back("OK");
		vBtnCaption.push_back("QA_QTY");
		//m_vMessageBoxId.push_back(TWMB_ID_OK);
	}break;
	default:
	{
		vBtnCaption.push_back("OK");
		m_vMessageBoxId.push_back(TWMB_ID_OK);
	}break;
	}

	for (std::vector<int>::size_type i = 0; i < m_vUserSelectButtonCtrl.size(); i++)
	{
		GetDlgItem(m_vUserSelectButtonCtrl[i])->ShowWindow(SW_HIDE);
	}

	for (std::vector<std::string>::size_type i = 0; i < vBtnCaption.size(); i++)
	{
		SetDlgItemText(m_vUserSelectButtonCtrl[i], vBtnCaption[i].c_str());
		GetDlgItem(m_vUserSelectButtonCtrl[i])->ShowWindow(SW_SHOW);
	}
}


void CDlgAlarmMsgBoxDoor::UserSelection(int nSelection)
{
	// Recovery 설정
	CString strRecovery;
	int nCmd;

	if (nSelection == eUserSelect_Retry) {
		strRecovery = "RETRY";
		nCmd = TWMB_ID_RETRY;
	}
	else if (nSelection == eUserSelect_Skip) {
		strRecovery = "SKIP";
		nCmd = TWMB_ID_SKIP;
	}
	else if (nSelection == eUserSelect_CleanOut) {
		strRecovery = "CLEANOUT";
		nCmd = TWMB_ID_CLEANOUT;
	}
	else if (nSelection == eUserSelect_Cancel) {
		strRecovery = "CANCEL";
		nCmd = TWMB_ID_CANCEL;
	}
	else if (nSelection == eUserSelect_PSkip) {
		strRecovery = "P-SKIP";
		nCmd = TWMB_ID_P_SKIP;
	}
	else if (nSelection == eUserSelect_TrayEnd) {
		strRecovery = "TRAYEND";
		nCmd = TWMB_ID_TRAYEND;
	}
	else if (nSelection == eUserSelect_TrayFeed) {
		strRecovery = "TRAYFEED";
		nCmd = TWMB_ID_TRAYFEED;
	}
	else if (nSelection == eUserSelect_PartialTrayEnd) {
		strRecovery = "TRAY_END";
		nCmd = TWMB_ID_PARTIAL_TRAYEND;
	}
	else if (nSelection == eUserSelect_SkiptoAssign) {
		strRecovery = "SKIP_TO_ASSIGN";
		nCmd = TWMB_ID_SKIP_TO_ASSIGN;
	}
	else if (nSelection == eUserSelect_Clear) {
		strRecovery = "Clear";
		nCmd = TWMB_ID_CLEAR;
	}
	else if (nSelection == eUserSelect_AxisMove) {
		strRecovery = "AXIS_MOVE";
		nCmd = TWMB_ID_AXIS_MOVE;
	}
	else if (nSelection == eUserSelect_QA_QTY) {
		strRecovery = "QA_QTY";
		nCmd = TWMB_ID_QA_QTY;
	}
	else { //( nSelection == eUserSelect_OK)
		strRecovery = "OK";
		nCmd = TWMB_ID_OK;

		// 		BOOL bIsVatMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_VAT_MODE);
		// 		if (bIsVatMode == TRUE) {
		// 			g_DMCont.m_dmHandlerOpt.SB(BDM11_VAT_ALARM, TRUE);
		// 		}
	}

	if (nSelection == eUserSelect_Skip)
	{
		if (!g_COVER_SW.GetStatus()) {
			CParamList AlarmData;
			g_Error.AlarmReport(ERR_MOT_INTERLOCK_DOOR_IS_NOT_LOCK, AlarmData, NULL);
			return;
		}
	}

	int nRet = UserSelect(m_nAlid, nSelection);

	if (nRet == TRUE)
		return;

	if (m_pSender != NULL) {
		m_pSender->OnOperatorCommand(nCmd, (WPARAM)m_nAlid, (LPARAM)NULL);
	}

	g_GuiComm.Send_AlarmReport(m_nAlid, eALST_CLEAR);

	// Stop Time
	SYSTEMTIME t;
	::GetLocalTime(&t);
	m_tErrorLog.SetStopTime(t);
	sprintf_s(m_tErrorLog.szRecovery, sizeof(m_tErrorLog.szRecovery), "%s", strRecovery.GetString());
	m_tErrorLog.nDuplicate = m_bDuplicatealarm;
	m_tErrorLog.nOnLine_OffLine = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_Run);

	g_DMCont.m_dmHandlerOpt.GS(SDM11_LotNo, m_tErrorLog.szLotNo_Info, sizeof(m_tErrorLog.szLotNo_Info));

	if (m_bDuplicatealarm == TRUE)
	{
		if (g_TaskSystemCtrl.m_strLastDuplicate != strRecovery)
		{
			g_DBErr.INSERT_ERROR_LOG_TBL(m_tErrorLog);
		}

		// 중복 jam은 log에 저장.
		CString strLog;
		strLog.Format("'%s', '%s', '%s', '%s', '%s', %d, '%s', %d , %d"
			, m_tErrorLog.szEQP_ALID, m_tErrorLog.szRecovery, m_tErrorLog.szPart, m_tErrorLog.szMessage, m_tErrorLog.strOccurred_Time, m_tErrorLog.nStop_Time, m_tErrorLog.szUser_Info, m_tErrorLog.nError_Level, m_tErrorLog.nPosition);
		strLog.Replace("\r\n", "");
		g_TaskSystemCtrl.MakeLog(strLog);
	}
	else {
		g_DBErr.INSERT_ERROR_LOG_TBL(m_tErrorLog);
	}

	g_TaskSystemCtrl.m_strLastDuplicate = strRecovery;

	_tERROR_MSG* e = g_Error.GetErrMsgPtr(m_nAlid);
	if (e != NULL)
	{
		e->bAlarmState = FALSE;

		if (e->nError_Level > eErrLevel_Warn) {
			g_TaskSystemCtrl.DeActionTowerAndSound(eSIGNAL_JAM_CALL);
		}
		else /*if(e->nError_Level <= eErrLevel_Warn)*/ {
			g_TaskSystemCtrl.DeActionTowerAndSound(eSIGNAL_REQUEST);
		}
	}

	DestroyWindow();
}

void CDlgAlarmMsgBoxDoor::OnBnClickedOk()
{
	UserSelection(eUserSelect_OK);
	//CDialogEx::OnOK();
}

void CDlgAlarmMsgBoxDoor::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	delete this;

	CDialogEx::PostNcDestroy();
}

int CDlgAlarmMsgBoxDoor::ConvertUserSelection(CString strUserSelection)
{
	int nRetUserSelection = 0;

	if (strUserSelection == "RETRY") {
		nRetUserSelection = eUserSelect_Retry;
	}
	else if (strUserSelection == "SKIP") {
		nRetUserSelection = eUserSelect_Skip;
	}
	else if (strUserSelection == "CLEAN OUT") {
		nRetUserSelection = eUserSelect_CleanOut;
	}
	else if (strUserSelection == "CANCEL") {
		nRetUserSelection = eUserSelect_Cancel;
	}
	else if (strUserSelection == "P-SKIP") {
		nRetUserSelection = eUserSelect_PSkip;
	}
	else if (strUserSelection == "TRAYEND") {
		nRetUserSelection = eUserSelect_TrayEnd;
	}
	else if (strUserSelection == "TRAYFEED") {
		nRetUserSelection = eUserSelect_TrayFeed;
	}
	else if (strUserSelection == "TRAY_END") {
		nRetUserSelection = eUserSelect_PartialTrayEnd;
	}
	else if (strUserSelection == "SKIP_DataAssign") {
		nRetUserSelection = eUserSelect_SkiptoAssign;
	}
	else if (strUserSelection == "Clear") {
		nRetUserSelection = eUserSelect_Clear;
	}
	else if (strUserSelection == "AXIS_MOVE") {
		nRetUserSelection = eUserSelect_AxisMove;
	}
	else if (strUserSelection == "QA_QTY")
	{
		nRetUserSelection = eUserSelect_QA_QTY;
	}
	else { // strUserSelection == "OK"
		nRetUserSelection = eUserSelect_OK;
	}
	return nRetUserSelection;
}

void CDlgAlarmMsgBoxDoor::OnBnClickedButtonUserSel(int nButtonIdx)
{
	CString strUserSelection;
	GetDlgItemText(m_vUserSelectButtonCtrl[nButtonIdx], strUserSelection);
	int nUserSelection = ConvertUserSelection(strUserSelection);
	UserSelection(nUserSelection);
}

void CDlgAlarmMsgBoxDoor::OnBnClickedButtonUserSel1()
{
	OnBnClickedButtonUserSel(0);
}


void CDlgAlarmMsgBoxDoor::OnBnClickedButtonUserSel2()
{
	OnBnClickedButtonUserSel(1);
}


void CDlgAlarmMsgBoxDoor::OnBnClickedButtonUserSel3()
{
	OnBnClickedButtonUserSel(2);
}


void CDlgAlarmMsgBoxDoor::OnBnClickedButtonUserSel4()
{
	OnBnClickedButtonUserSel(3);
}


void CDlgAlarmMsgBoxDoor::OnBnClickedButtonSetMbtype()
{
	char szValue_New[128] = { 0, };
	char szValue_Old[128] = { 0, };
	char szTitle[128] = { 0, };
	char szMin[128] = { 0, };
	char szMax[128] = { 0, };

	CString strValue_Old;

	strValue_Old.Format("%d", m_tErrorMsg.nMB_Type);

	sprintf_s(szValue_Old, sizeof(szValue_Old), "%s", strValue_Old.GetString());
	sprintf_s(szMax, sizeof(szMax), "%d", TWMB_ID_MAX_COUNT - 1);
	sprintf_s(szMin, sizeof(szMin), "%d", 0);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szValue_New, sizeof(szValue_New), szValue_Old, szTitle, szMax, szMin);
	if (nRet == RETURN_OK) {
		m_tErrorMsg.nMB_Type = atoi(szValue_New);
		g_DB.UPDATE_ERROR_MSG_TBL(m_nAlid, m_tErrorMsg);
		InitControls();
	}
}

void CDlgAlarmMsgBoxDoor::InitSpread()
{
	char szCompany[16] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

	int nSize = (int)m_vFailPicker.size();

	if (nSize > 0) {
		if ((m_nAlid >= ERR_PRESS_UNIT_1_ARRIVED_SENSOR && m_nAlid <= ERR_PRESS_UNIT_8_ARRIVED_SENSOR)
			|| (m_nAlid >= ERR_VISION_RESULT_NG && m_nAlid <= ERR_TEST_PP2_VISION_RESULT_NG)
			|| (m_nAlid >= ERR_TEST_PP1_VISION_RESULT_NG_SKIP && m_nAlid <= ERR_TEST_PP2_VISION_RESULT_NG_SKIP)
			|| (m_nAlid >= ERR_TESTER_LOW_YIELD_STATION_1 &&  m_nAlid <= ERR_TESTER_DEFAULT_BIN_FAIL_STATION_16)
			|| (m_nAlid >= ERR_PRESS_UNIT_1_ARRIVED_SENSOR_MOVE_SAFETY && m_nAlid <= ERR_PRESS_UNIT_8_ARRIVED_SENSOR_MOVE_SAFETY
			|| m_nAlid >= ERR_TESTER_LOW_YIELD_STATION_1 &&  m_nAlid <= ERR_TESTER_1ST_FAIL_COUNT_STATION_16
			|| m_nAlid >= ERR_PRESS_UNIT_1_CLEAN_DEVICE_ARRIVED_SENSOR &&  m_nAlid <= ERR_PRESS_UNIT_8_CLEAN_DEVICE_ARRIVED_SENSOR_MOVE_SAFETY))
		{
			int nSiteDivX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
			int nSiteDivY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

			m_SprdPicker.SetMaxRows(nSiteDivY);
			m_SprdPicker.SetMaxCols(nSiteDivX);
		}
		else if (m_nAlid >= ERR_SOCKET_CLEAN_TABLE_1_DEVICE_LIFE_OVER && m_nAlid <= ERR_SOCKET_CLEAN_TABLE_2_DEVICE_LIFE_OVER)
		{
			int nPocketCount_X = g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Div_X);
			int nPocketCount_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Div_Y);
			m_SprdPicker.SetMaxRows(nPocketCount_Y);
			m_SprdPicker.SetMaxCols(nPocketCount_X);
		}
		// 		else if (m_nAlid == ERR_TEST_PP_1_ARRIVED_SEN_FAIL || m_nAlid == ERR_TEST_PP_2_ARRIVED_SEN_FAIL) {

		// 
		// 			m_SprdPicker.SetMaxRows(nPocketCount_Y);
		// 			m_SprdPicker.SetMaxCols(nPocketCount_X);
		// 		}
		else if (m_nAlid >= ERR_LOAD_TBL_1_VISION_BARCODE_IS_SHORT && m_nAlid <= ERR_LOAD_TBL_4_VISION_BARCODE_2D_LENGTH) {
			int nPocketCntX = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);
			int nPocketCntY = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);

			m_SprdPicker.SetMaxRows(nPocketCntX);
			m_SprdPicker.SetMaxCols(nPocketCntY);	
		}

		else if (m_nAlid > 5000 && m_nAlid < 6000) {
			m_SprdPicker.SetMaxRows(2);
			m_SprdPicker.SetMaxCols(4);
		}
		else if (m_nAlid > 7000 && m_nAlid < 8000) {
			m_SprdPicker.SetMaxRows(2);
			m_SprdPicker.SetMaxCols(4);
		}
		else if (m_nAlid > 6000 && m_nAlid < 7000) {
			CPoint cPoint = m_vFailPicker[nSize - 1];
			if (cPoint.x >= 3) {
				m_SprdPicker.SetMaxRows(4);
				m_SprdPicker.SetMaxCols(4);
			}
			else if (cPoint.y >= 3) {
				m_SprdPicker.SetMaxRows(4);
				m_SprdPicker.SetMaxCols(4);
			}
		}

		CSpreadSheet* pSprd = (CSpreadSheet*)GetDlgItem(IDC_SPRD_PICKER);

		for (int i = 0; i < (int)m_vFailPicker.size(); i++) {
			CPoint cPoint = m_vFailPicker[i];
			pSprd->SetRow(cPoint.y + 1);
			pSprd->SetCol(cPoint.x + 1);
			pSprd->SetBackColor(COLOR_RED);
		}

		pSprd = NULL;
	}
	else {
		GetDlgItem(IDC_LABEL_CONTACTOR_FORCE5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPRD_PICKER)->ShowWindow(SW_HIDE);
	}
}

void CDlgAlarmMsgBoxDoor::InitReSize()
{
	CRect r;this->GetWindowRect(&r);
	CRect rect;	m_bitPic1.GetWindowRect(&rect);

	int nDesktopWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	int nDesktopHeight = GetSystemMetrics(SM_CYFULLSCREEN);

	int nX = nDesktopWidth / 2 - r.Width() / 2;
	int nY = nDesktopHeight / 2 - r.Height() / 2;

	int nHeight = r.Height();
	if (m_vErrorPos.size() <= 0)
	{
		nHeight = r.Height() - rect.Height() - 30;
		//this->SetWindowPos(&wndTopMost, 150, 250, r.Width(), rect.top -185, SWP_NOMOVE);
	}

	if (this->GetSafeHwnd() != NULL)
		this->SetWindowPos(&wndTopMost, nX, nY,/*150, 250,*/ r.Width(), nHeight, 0);
}

void CDlgAlarmMsgBoxDoor::InitPosition()
{
	for (int i = 0; i < eMaxDoorSensor; i++)
	{
		GetDlgItem(m_nPosicionControl[i])->ShowWindow(SW_HIDE);
	}
	for (std::vector<int>::iterator iter = m_vErrorPos.begin(); iter != m_vErrorPos.end(); iter++)
	{
		m_editPosition[*iter].SetBackColor(COLOR_RED);
		GetDlgItem(m_nPosicionControl[*iter])->ShowWindow(SW_SHOW);
		bIsRed[*iter] = FALSE;
	}
	
// 	for (int i = 0; i < ePos_MaxPosition; i++) {
// 
// 		if (m_nPosition - 1 != i || m_nPosition <= 0) {
// 			GetDlgItem(m_nPosicionControl[i])->ShowWindow(SW_HIDE);
// 		}
// 		else {
// 			m_editPosition[m_nPosition - 1].SetBackColor(COLOR_RED);
// 			GetDlgItem(m_nPosicionControl[i])->ShowWindow(SW_SHOW);
// 		}
// 	}
	if (m_vErrorPos.size() > 0) {
		
		SetTimer(1, 1000, NULL);
	}


// 	if (m_nPosition > 0) {
// 		bIsRed = FALSE;
// 		SetTimer(1, 1000, NULL);
// 	}
}

//  [11/17/2020 jaehoon.jung]
BOOL CDlgAlarmMsgBoxDoor::IsRightCompany(char * pCompany)
{
	char szCompany[16] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

	BOOL bCompany = strcmp(pCompany, szCompany);
	if (bCompany == 0)
		return TRUE;


	return FALSE;
}

BOOL CDlgAlarmMsgBoxDoor::UserSelect(int nAlid, int nSelection)
{
	// [11/17/2020 jaehoon.jung]
	//회사가 USI 이면 SKIP 동작을 P-SKIP (PassWord 칠 수 있도록) 변경.
	BOOL bAmkor = IsRightCompany(DEF_COMPANY_AMKOR);

	// password skip의 경우 등급이 맞을 때까지 물어보아야 한다.
	if (eUserSelect_PSkip == nSelection || (bAmkor == TRUE) && nSelection == eUserSelect_Skip)
	{
		int nRetLevel = 0;
		BOOL bPW_Skip = FALSE;
		while (bPW_Skip == FALSE)
		{
			NEXT;
			BOOL bRet = g_LibCommCtrl.ShowLogInBox(0, &nRetLevel);
			if (bRet == RETURN_OK && nRetLevel > eUSERLEVEL_TECHNICIAN) {
				bPW_Skip = TRUE;
				return FALSE;
			}
			else
			{
				return TRUE;
			}
		}
	}

	if (nSelection == eUserSelect_OK && nAlid == ERR_ONLINE_USE && (bAmkor == TRUE))
	{
		int nRetLevel = 0;
		BOOL bPW_Skip = FALSE;
		while (bPW_Skip == FALSE)
		{
			NEXT;
			BOOL bRet = g_LibCommCtrl.ShowLogInBox(0, &nRetLevel);
			if (bRet == RETURN_OK && nRetLevel > eUSERLEVEL_TECHNICIAN) {
				bPW_Skip = TRUE;
				return FALSE;
			}
			else
			{
				return TRUE;
			}

		}
	}
	return FALSE;
}