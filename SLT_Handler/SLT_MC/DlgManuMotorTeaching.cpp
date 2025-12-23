// DlgManuMotorTeaching.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgManuMotorTeaching.h"
#include "afxdialogex.h"

#include "DlgManuMotor.h"
#include "DlgManuMotorJog.h"
#include "DlgManuPressUnit.h"
#include "DlgManuTestPP.h"
#include "DlgManuSetplate.h"
#include "DlgManuTrayPP.h"
#include "DlgManuMotorParam.h"
#include "DlgManuTransfer.h"
#include "DlgZPickerActionMove.h"

#include "DlgManuZMotorTeaching.h"

#include <algorithm>
// CDlgManuMotorTeaching 대화 상자입니다.

CRamTeaching  m_TeachingData;   // decrease by PAGESIZE Error 전역을 써야함.
static bool g_bManualMoveCancel = false;

IMPLEMENT_DYNAMIC(CDlgManuMotorTeaching, CDialogEx)

CDlgManuMotorTeaching::CDlgManuMotorTeaching(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgManuMotorTeaching::IDD, pParent)
{
	m_nCurRobotId  = 0;
	m_nLastSel_Row = 0;
	m_nLastSel_Col = 0;
	g_bManualMoveCancel = false;

	char szLogFileName2[512] = { 0, };
	sprintf_s(szLogFileName2, sizeof(szLogFileName2), "%sOriginEncoder.log", SZ_LOG_FILE_PATH);
	m_EncoderLog.SetFileName(szLogFileName2);

	m_nLastLocCount = 0;
	m_nLastPosCount = 0;

	m_pDlgVAT = NULL;

	m_nTestPpType = eTestPPLoadType;

	//Y축 Group Position 초기화.
	for (int i = 0; i < 4; i++) {
		m_vYAxisGroupPoint[i].clear();
	}

	m_vGroupPoint.clear();

	m_nTeachType = _eTeachType_Single;
}

CDlgManuMotorTeaching::~CDlgManuMotorTeaching()
{
	// servo motor
	m_cBmpServoMotor.DeleteObject();
	// motor teaching
	m_cBmpMotorTeaching.DeleteObject();
	// encoder read
	m_cBmpEncoderRead.DeleteObject();
	// motor moving
	m_cBmpMotorMoving.DeleteObject();

	// base point
	m_cBmpBasePoint.DeleteObject();
	// parameter setup
	m_cBmpParameterSetup.DeleteObject();
	// reload
	m_cBmpReload.DeleteObject();
	// save
	m_cBmpSave.DeleteObject();
	// exit
	m_cBmpExit.DeleteObject();

	// press unit
	m_cBmpPressUnit.DeleteObject();
	// tray pp
	m_cBmpTrayPP.DeleteObject();
	// test pp
	m_cBmpTestPP.DeleteObject();
	// transfer
	m_cBmpTransfer.DeleteObject();
	// setplate & stacker
	m_cBmpSetPlateStacker.DeleteObject();
	//pcd backlash
	m_cBmpPcdBacklash.DeleteObject();
	//Vision Auto Teaching
	m_cBmpVisionAutoTeaching.DeleteObject();
	//Test PP Offset
	m_cBmpTestPpOffset.DeleteObject();
	// Picker Align
	m_cBmpPickerAlign.DeleteObject();

	m_cBmpPickerHeightCal.DeleteObject();

	m_cBmpVAT.DeleteObject();
}

void CDlgManuMotorTeaching::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPREAD_LOC_TABLE, m_sprLocTable);
	DDX_Control(pDX, IDC_CB_ROBOT_CHOOSE, m_cbRobotName);

	// servo motor
	DDX_Control(pDX, IDC_COMMANDBUTTON_MANU_MOTOR, m_btnServoMotor);
	// motor teaching
	DDX_Control(pDX, IDC_COMMANDBUTTON_TEACHING_JOG, m_btnMotorTeaching);
	// encoder read
	DDX_Control(pDX, IDC_COMMANDBUTTON_READ_ENCODER, m_btnEncoderRead);
	// motor moving
	DDX_Control(pDX, IDC_COMMANDBUTTON_MOVE, m_btnMotorMoving);
	// base point
	DDX_Control(pDX, IDC_COMMANDBUTTON_BASE, m_btnBasePoint);
	// parameter setup
	DDX_Control(pDX, IDC_COMMANDBUTTON_PARAMETER, m_btnParameterSetup);
	// reload
	DDX_Control(pDX, IDC_COMMANDBUTTON_RELOAD, m_btnReload);
	// Save
	DDX_Control(pDX, IDC_COMMANDBUTTON_SAVE, m_btnSave);
	// exit
	DDX_Control(pDX, IDC_OK, m_btnExit);
	// press unit
	DDX_Control(pDX, IDC_COMMANDBUTTON_PRESS, m_btnPressUnit);
	// tray pp
	DDX_Control(pDX, IDC_COMMANDBUTTON_TRAYPP, m_btnTrayPP);
	// test pp
	DDX_Control(pDX, IDC_COMMANDBUTTON_TESTPP, m_btnTestPP);
	// setplate & stacker
	DDX_Control(pDX, IDC_COMMANDBUTTON_SETPLATE_STACKER, m_btnSetPlateStacker);
	DDX_Control(pDX, IDC_COMMANDBUTTON_PCDBACKLASH, m_btnPcdNBacklash);
	// Test PP Offset
	DDX_Control(pDX, IDC_COMMANDBUTTON_TESTPP_OFFSET, m_btnTestPpOffset);

/*	DDX_Control(pDX, IDC_COMMANDBUTTON_NEW_PCDBACKLASH, m_btnNewBacklash);*/

	DDX_Control(pDX, IDC_COMMANDBUTTON_TRANSFER, m_btnTransfer);
	
	DDX_Control( pDX, IDC_COMMANDBUTTON_PICKERALIGN, m_btnPickerAlign );

	DDX_Control(pDX, IDC_BTN_VISION_AUTO_TEACHING, m_btnVisionAutoTeaching);

	DDX_Control(pDX, IDC_COMMANDBUTTON_NEW_PICKER_MOTOR, m_btnPickerHeightCal);
	
	DDX_Control(pDX, IDC_VISION_AUTO_TEACHING, m_btnVAT);

}


BEGIN_MESSAGE_MAP(CDlgManuMotorTeaching, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_COMMANDBUTTON_MANU_MOTOR, &CDlgManuMotorTeaching::OnBnClickedCommandbuttonManuMotor)
	ON_BN_CLICKED(IDC_COMMANDBUTTON_TEACHING_JOG, &CDlgManuMotorTeaching::OnBnClickedCommandbuttonTeachingJog)
	ON_BN_CLICKED(IDC_COMMANDBUTTON_MOVE, &CDlgManuMotorTeaching::OnBnClickedCommandbuttonMove)
	ON_BN_CLICKED(IDC_COMMANDBUTTON_BASE, &CDlgManuMotorTeaching::OnBnClickedCommandbuttonBase)
	ON_BN_CLICKED(IDC_COMMANDBUTTON_PARAMETER, &CDlgManuMotorTeaching::OnBnClickedCommandbuttonParameter)
	ON_BN_CLICKED(IDC_COMMANDBUTTON_SAVE, &CDlgManuMotorTeaching::OnBnClickedCommandbuttonSave)
	ON_BN_CLICKED(IDC_OK, &CDlgManuMotorTeaching::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_CB_ROBOT_CHOOSE, &CDlgManuMotorTeaching::OnCbnSelchangeCbRobotChoose)
	ON_BN_CLICKED(IDC_COMMANDBUTTON_RELOAD, &CDlgManuMotorTeaching::OnBnClickedCommandbuttonReload)
	ON_BN_CLICKED(IDC_COMMANDBUTTON_TESTPP, &CDlgManuMotorTeaching::OnBnClickedCommandbuttonTestpp)
	ON_BN_CLICKED(IDC_COMMANDBUTTON_TRAYPP, &CDlgManuMotorTeaching::OnBnClickedCommandbuttonTraypp)
	ON_BN_CLICKED(IDC_COMMANDBUTTON_PRESS, &CDlgManuMotorTeaching::OnBnClickedCommandbuttonPress)
	ON_BN_CLICKED(IDC_COMMANDBUTTON_SETPLATE_STACKER, &CDlgManuMotorTeaching::OnBnClickedCommandbuttonSetplateStacker)
	ON_BN_CLICKED(IDC_COMMANDBUTTON_TRANS_EVSTACKER, &CDlgManuMotorTeaching::OnBnClickedCommandbuttonTransEvstacker)
	ON_BN_CLICKED(IDC_COMMANDBUTTON_READ_ENCODER, &CDlgManuMotorTeaching::OnBnClickedCommandbuttonReadEncoder)
	ON_BN_CLICKED(IDC_COMMANDBUTTON_PCDBACKLASH, &CDlgManuMotorTeaching::OnBnClickedCommandbuttonPcdbacklash)
	ON_BN_CLICKED(IDC_COMMANDBUTTON_TESTPP_OFFSET, &CDlgManuMotorTeaching::OnBnClickedCommandbuttonTestppOffset)
	ON_BN_CLICKED(IDC_COMMANDBUTTON_TRANSFER, &CDlgManuMotorTeaching::OnBnClickedCommandbuttonTransfer)
	//ON_BN_CLICKED( IDC_COMMANDBUTTON_PICKERALIGN, &CDlgManuMotorTeaching::OnBnClickedBtnPickeralign )
	//ON_BN_CLICKED(IDC_COMMANDBUTTON_NEW_PCDBACKLASH, &CDlgManuMotorTeaching::OnBnClickedCommandbuttonNewPcdbacklash)
	ON_BN_CLICKED(IDC_BTN_VISION_AUTO_TEACHING, &CDlgManuMotorTeaching::OnBnClickedBtnVisionAutoTeaching)
	ON_BN_CLICKED(IDC_COMMANDBUTTON_TRAYPP_OFFSET, &CDlgManuMotorTeaching::OnBnClickedCommandbuttonTrayppOffset)
	ON_BN_CLICKED(IDC_COMMANDBUTTON_NEW_PICKER_MOTOR, &CDlgManuMotorTeaching::OnBnClickedCommandbuttonNewPickerMotor)	
	ON_BN_CLICKED(IDC_VISION_AUTO_TEACHING, &CDlgManuMotorTeaching::OnBnClickedVisionAutoTeaching)

END_MESSAGE_MAP()


// CDlgManuMotorTeaching 메시지 처리기입니다.



BOOL CDlgManuMotorTeaching::PreTranslateMessage(MSG* pMsg)
{
	if((pMsg->message==WM_SYSKEYDOWN) && (pMsg->wParam==VK_F4) )
	{
		return TRUE;
	}
	if (pMsg->message == WM_KEYDOWN)
	{
		return TRUE;
	}
	//if((pMsg->message==WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))
	//{
	//	return TRUE;
	//}
	//if((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ESCAPE) )
	//{
	//	return TRUE;
	//}

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CDlgManuMotorTeaching::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	g_DMCont.m_dmEQP.SB(BDM0_OPEN_SLT_UI_DLG_HISTORY, TRUE);
	// servo motor
	m_cBmpServoMotor.LoadBitmap(IDB_SERVO_MOTOR);
	m_btnServoMotor.SetBitmap(m_cBmpServoMotor);
	// motor teaching
	m_cBmpMotorTeaching.LoadBitmap(IDB_MOTOR_TEACHING);
	m_btnMotorTeaching.SetBitmap(m_cBmpMotorTeaching);
	// encoder read
	m_cBmpEncoderRead.LoadBitmap(IDB_ENCODER_READ);
	m_btnEncoderRead.SetBitmap(m_cBmpEncoderRead);
	// motor moving
	m_cBmpMotorMoving.LoadBitmap(IDB_MOTOR_MOVING);
	m_btnMotorMoving.SetBitmap(m_cBmpMotorMoving);

	// base point
	//m_cBmpBasePoint.LoadBitmap(IDB_BASE_POINT);
	//m_btnBasePoint.SetBitmap(m_cBmpBasePoint);

	// parameter setup
	m_cBmpParameterSetup.LoadBitmap(IDB_PARAMETER_SETUP);
	m_btnParameterSetup.SetBitmap(m_cBmpParameterSetup);

	// reload
	m_cBmpReload.LoadBitmap(IDB_RELOAD_SUB);
	m_btnReload.SetBitmap(m_cBmpReload);

	//// save
	m_cBmpSave.LoadBitmap(IDB_SAVE_SUB);
	m_btnSave.SetBitmap(m_cBmpSave);

	// exit
	m_cBmpExit.LoadBitmap(IDB_EXIT_SUB);
	m_btnExit.SetBitmap(m_cBmpExit);

	// press unit
	m_cBmpPressUnit.LoadBitmap(IDB_PRESS_UNIT_SUB);
	m_btnPressUnit.SetBitmap(m_cBmpPressUnit);

	// tray pp
	m_cBmpTrayPP.LoadBitmap(IDB_TRAYPP_SUB);
	m_btnTrayPP.SetBitmap(m_cBmpTrayPP);

	// test pp
	m_cBmpTestPP.LoadBitmap(IDB_TESTPP_SUB);
	m_btnTestPP.SetBitmap(m_cBmpTestPP);

	// setplate & stacker
	m_cBmpSetPlateStacker.LoadBitmap(IDB_SETPLATE_STACKER_SUB);
	m_btnSetPlateStacker.SetBitmap(m_cBmpSetPlateStacker);

	//pcd & backlash
	m_cBmpPcdBacklash.LoadBitmap(IDB_PCDBACKLASH);
	m_btnPcdNBacklash.SetBitmap(m_cBmpPcdBacklash);

	//Vision Auto Teaching
	m_cBmpVisionAutoTeaching.LoadBitmap(IDB_VAT);
	m_btnVisionAutoTeaching.SetBitmap(m_cBmpVisionAutoTeaching);

	//Test PP Offset
	m_cBmpTestPpOffset.LoadBitmap(IDB_TESTPP_OFFSET_SUB);
	m_btnTestPpOffset.SetBitmap(m_cBmpTestPpOffset);

	m_cBmpTransfer.LoadBitmap(IDB_TRANSFER);
	m_btnTransfer.SetBitmap(m_cBmpTransfer);
	
	//Picker Height Cal
	m_cBmpPickerHeightCal.LoadBitmap(IDB_PICKER_HEIGHT_CAL_GRAY);
	m_btnPickerHeightCal.SetBitmap(m_cBmpPickerHeightCal);


	//Z Auto Teaching
	m_cBmpVAT.LoadBitmap(IDB_VISION_AUTO_TEACHING);
	m_btnVAT.SetBitmap(m_cBmpVAT);


//  	m_cBmpNewBacklash.LoadBitmap(IDB_PICKER_HEIGHT_CAL);
//  	m_btnNewBacklash.SetBitmap(m_cBmpNewBacklash);

	char szDrvFile[512]={0,};
	char szCokName[512]={0,};
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));

	char szCurDir[512]={0,};
	GetCurrentDirectory(sizeof(szCurDir), szCurDir);
	sprintf_s(szDrvFile, sizeof(szDrvFile), "%s\\sys_data\\%s", szCurDir, SZ_DRIVER_FILE_NAME);

	g_DMCont.m_Teach.Load();
	m_TeachingData.Initialize(szDrvFile, szCokName);
	m_TeachingData = g_DMCont.m_Teach;

	InitControl();

	g_DMCont.m_dmHandlerOpt.SB(BDM11_GUI_OPEN_HISTORY, TRUE);

	if(g_DMCont.m_dmVAT.GB(BDM14_VAT_MD_USE) == FALSE)
		GetDlgItem(IDC_VISION_AUTO_TEACHING)->ShowWindow(SW_HIDE);
	else
		GetDlgItem(IDC_VISION_AUTO_TEACHING)->ShowWindow(SW_SHOW);

	
	(CDSSPanel *)GetDlgItem(IDC_SSPANEL_TESTPP_TYPE)->ShowWindow(SW_HIDE);

	(CDSSPanel *)GetDlgItem(IDC_SSPANEL_SINGLE_TEACHING)->EnableWindow(FALSE);
	(CDSSPanel *)GetDlgItem(IDC_SSPANEL_GROUP_TEACHING)->EnableWindow(FALSE);
	

	m_vYAxisGroupPoint[0].push_back(std::make_pair(eCOL2_Y, eRow5_Loading_Table_1_LD)); //LOAD PP
	m_vYAxisGroupPoint[0].push_back(std::make_pair(eCOL2_Y, eRow1_Station1));			//UNLOAD PP
	m_vYAxisGroupPoint[0].push_back(std::make_pair(eCOL2_Y, eRow9_Loading_Table_1_UD));	//UNLOAD PP

	m_vYAxisGroupPoint[1].push_back(std::make_pair(eCOL2_Y, eRow6_Loading_Table_2_LD));	//LOAD PP
	m_vYAxisGroupPoint[1].push_back(std::make_pair(eCOL2_Y, eRow2_Station2));			//UNLOAD PP
	m_vYAxisGroupPoint[1].push_back(std::make_pair(eCOL2_Y, eRow10_Loading_Table_2_UD));//UNLOAD PP

	m_vYAxisGroupPoint[2].push_back(std::make_pair(eCOL2_Y, eRow7_Loading_Table_3_LD));	//LOAD PP
	m_vYAxisGroupPoint[2].push_back(std::make_pair(eCOL2_Y, eRow3_Station3));			//UNLOAD PP
	m_vYAxisGroupPoint[2].push_back(std::make_pair(eCOL2_Y, eRow11_Loading_Table_3_UD));//UNLOAD PP

	m_vYAxisGroupPoint[3].push_back(std::make_pair(eCOL2_Y, eRow8_Loading_Table_4_LD));	//LOAD PP
	m_vYAxisGroupPoint[3].push_back(std::make_pair(eCOL2_Y, eRow4_Station4));			//UNLOAD PP
	m_vYAxisGroupPoint[3].push_back(std::make_pair(eCOL2_Y, eRow12_Loading_Table_4_UD));//UNLOAD PP


	MakeLog("Motor Set Up & Teaching Dialog Show");

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgManuMotorTeaching::OnDestroy()
{
	CDialogEx::OnDestroy();

	m_TeachingData.Finalize();

	m_cBmpServoMotor.DeleteObject();
	// motor teaching
	m_cBmpMotorTeaching.DeleteObject();
	// encoder read
	m_cBmpEncoderRead.DeleteObject();
	// motor moving
	m_cBmpMotorMoving.DeleteObject();

	// base point
	m_cBmpBasePoint.DeleteObject();
	// parameter setup
	m_cBmpParameterSetup.DeleteObject();
	// reload
	m_cBmpReload.DeleteObject();
	// save
	m_cBmpSave.DeleteObject();
	// exit
	m_cBmpExit.DeleteObject();

	// press unit
	m_cBmpPressUnit.DeleteObject();
	// tray pp
	m_cBmpTrayPP.DeleteObject();
	// test pp
	m_cBmpTestPP.DeleteObject();
	// transfer
	m_cBmpTransfer.DeleteObject();
	// setplate & stacker
	m_cBmpSetPlateStacker.DeleteObject();
	//pcd backlash
	m_cBmpPcdBacklash.DeleteObject();
	//Vision Auto Teaching
	m_cBmpVisionAutoTeaching.DeleteObject();
	//Test PP Offset
	m_cBmpTestPpOffset.DeleteObject();
	// Picker Align
	m_cBmpPickerAlign.DeleteObject();

	m_cBmpNewBacklash.DeleteObject();

	m_cBmpPickerHeightCal.DeleteObject();

	m_cBmpVAT.DeleteObject();

	g_DMCont.m_dmHandlerOpt.SB(BDM11_GUI_OPEN_HISTORY, FALSE);
	g_DMCont.m_dmEQP.SB(BDM0_OPEN_SLT_UI_DLG_HISTORY, FALSE);

	if (m_pDlgVAT != NULL)
	{
		m_pDlgVAT->DestroyWindow();
		delete m_pDlgVAT;
		m_pDlgVAT = NULL;
	}
}


void CDlgManuMotorTeaching::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CDlgManuMotorTeaching::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}


void CDlgManuMotorTeaching::OnBnClickedCommandbuttonManuMotor()
{
	CDlgManuMotor dlg;
	dlg.DoModal();
}


void CDlgManuMotorTeaching::OnBnClickedCommandbuttonTeachingJog()
{
	if( m_nCurRobotId == tRobotId_TRAYPP_1 && m_nCurRobotId == tRobotId_TRAYPP_2 && ( m_nLastSel_Col == 6 || m_nLastSel_Col == 7 ) )
	{
		AfxMessageBox( _T( "Pitch Not USE" ) );
		return;
	}

	CDlgManuMotorJog dlg;
	// Set Robot Select
	dlg.m_nSelRobot = m_cbRobotName.GetCurSel();

	// Set Title
	CString strRobotName = _T("");
	m_cbRobotName.GetLBText(m_cbRobotName.GetCurSel(),strRobotName);
	dlg.m_strRobotName = strRobotName;

	// Set Axis Number
	ST_LOCATION_HEAD tLocHead;	
	m_TeachingData.GetTeachPosHead(m_nCurRobotId, tLocHead);
	CStringList LocNameList;
	m_TeachingData.ReadLocNameList(m_nCurRobotId, LocNameList);
	if( m_nLastSel_Row <= 0 || m_nLastSel_Row > LocNameList.GetCount() ){
		return;
	}
	if( m_nLastSel_Col <= 0 || m_nLastSel_Col > tLocHead.nPosCount ){
		return;
	}
	
	std::vector<int> vAxisNo;
	vAxisNo.clear();
	if(m_cbRobotName.GetCurSel() == 25){ // transfer 의 경우 head가 2개 이기 때문에, 선택 되어지는 Head의 Z축만 동작하도록 한다.
		vAxisNo.push_back(35);
		if(m_nLastSel_Col == 2 || m_nLastSel_Col == 3){
			vAxisNo.push_back(-1);
			vAxisNo.push_back(22);
		}
		else if(m_nLastSel_Col == 4 || m_nLastSel_Col == 5){
			vAxisNo.push_back(-1);
			vAxisNo.push_back(23);
		}
	}else{
		for(int jj=0; jj<tLocHead.nPosCount; jj++){
			vAxisNo.push_back(tLocHead.anPosAxisNo[jj]);
		}
	}

	int nAxisCnt = (int)vAxisNo.size();
	if ((m_nCurRobotId == tRobotId_TESTPP_1 || m_nCurRobotId == tRobotId_TESTPP_2) && m_nTestPpType == eTestPPUnLoadType/*(m_nLastSel_Col >= 8  )*/)
	{
		vAxisNo.erase(vAxisNo.begin(), vAxisNo.begin() + (nAxisCnt / 2));
		dlg.m_nTestPPHandType = eTestPPUnLoadType;
		dlg.m_sTestHandName = " - [Test UnLoad Hand]";
	}
	else if ((m_nCurRobotId == tRobotId_TESTPP_1 || m_nCurRobotId == tRobotId_TESTPP_2) && m_nTestPpType == eTestPPLoadType/*(m_nLastSel_Col <  8)*/)
	{
		vAxisNo.erase(vAxisNo.begin() + (nAxisCnt / 2), vAxisNo.end());
		dlg.m_nTestPPHandType = eTestPPLoadType;
		dlg.m_sTestHandName = " - [Test Load Hand]";
	}
	else {
	}

	
	std::vector<int>::iterator pos = std::unique(vAxisNo.begin(), vAxisNo.end());
	vAxisNo.erase(pos, vAxisNo.end());
	dlg.m_vAxisNo = vAxisNo;

	dlg.DoModal();	
}

void CDlgManuMotorTeaching::callback_progressbar_cancel_event( int _axis_no ) 
{
	g_bManualMoveCancel = true;

	while( true ) {
		MSG message;
		if( ::PeekMessage( &message, NULL, 0, 0, PM_REMOVE ) ) {
			::TranslateMessage( &message );
			::DispatchMessage( &message );
		}
		Sleep( 10 );

		g_Motor[ _axis_no ]->SetSlowStop();

		//Teachingmove 중 Ampfault 발생, Cancel event에서 while 탈출 불가로 수정 [5/13/2025 seungtaek.oh]
		if(g_Motor[_axis_no]->MotionDone() == ERR_NO_ERROR || g_Motor[_axis_no]->MotionDone() != ERR_MOTOR_CMN_IN_MOVE_STATE) {
			break;
		}
	}
}

void CDlgManuMotorTeaching::OnBnClickedCommandbuttonMove()
{
	ST_LOCATION_HEAD tLocHead;	
	m_TeachingData.GetTeachPosHead(m_nCurRobotId, tLocHead);
	CStringList LocNameList;
	m_TeachingData.ReadLocNameList(m_nCurRobotId, LocNameList);
	
	if( m_nLastSel_Row <= 0 || m_nLastSel_Row > LocNameList.GetCount() ){
		return;
	}
	if( m_nLastSel_Col <= 0 || m_nLastSel_Col > tLocHead.nPosCount ){
		return;
	}

// 	int nErr = ComInterlock();
// 
// 	if( nErr != ERR_NO_ERROR ){
// 		CParamList AlarmData;
// 		g_Error.AlarmReport(nErr, AlarmData, NULL);
// 		return;
// 	}

	int nLocIdx, nPosIdx;
	//기존 방식
// 	nPosIdx = m_nLastSel_Col-1;
// 	nLocIdx = m_nLastSel_Row-1;

	if (m_nCurRobotId == tRobotId_TESTPP_1 || m_nCurRobotId == tRobotId_TESTPP_2)
	{
		if (m_nTestPpType == eTestPPLoadType) {
			if (1 <= m_nLastSel_Col && m_nLastSel_Col <= 7) { //Load Index.
				nPosIdx = m_nLastSel_Col - 1;
			}
			else {
				return;
			}
		}
		else {
			if (1 <= m_nLastSel_Col && m_nLastSel_Col <= 7) { //Unload Index.
				nPosIdx = (m_nLastSel_Col + 7) - 1;
			}
			else {
				return;
			}
		}
	}
	else
	{
		nPosIdx = m_nLastSel_Col - 1;		
	}

	nLocIdx = m_nLastSel_Row - 1;

	/*User Spd, Acc, Dcc & Seq Spd, Acc, Dcc 를 가지고 계산*/
	int nIndexSpd[eMaxTestPPCount][CTaskTestPP::eAxisMax] = { { NDM4_SPD_TestPP1_X1, NDM4_SPD_TestPP1_Y, NDM4_SPD_TestPP1_Z1},
	{ NDM4_SPD_TestPP2_X1, NDM4_SPD_TestPP2_Y, NDM4_SPD_TestPP2_Z1 } };

	int nIndexAcc[eMaxTestPPCount][CTaskTestPP::eAxisMax] = { { NDM4_SPD_TestPP1_X1, NDM4_ACC_TestPP1_Y, NDM4_ACC_TestPP1_Z1 },
	{ NDM4_SPD_TestPP2_X1, NDM4_ACC_TestPP2_Y, NDM4_ACC_TestPP2_Z1 } };

	int nIndexDcc[eMaxTestPPCount][CTaskTestPP::eAxisMax] = { { NDM4_SPD_TestPP1_X1, NDM4_DCC_TestPP1_Y, NDM4_DCC_TestPP1_Z1 },
	{ NDM4_SPD_TestPP2_X1, NDM4_DCC_TestPP2_Y, NDM4_DCC_TestPP2_Z1 } };

	double dRateSpd = (double)g_DMCont.m_dmTestPP.GN(nIndexSpd[0][1]);
	int    nRateAcc = g_DMCont.m_dmTestPP.GN(nIndexAcc[0][1]);
	int    nRateDcc = g_DMCont.m_dmTestPP.GN(nIndexDcc[0][1]);

	double dRetSpd = 0;
	int    nRetAcc = 0, nRetDcc = 0;
	double dCurPos = 0.0;

	/*if (nRetAcc < 200)*/ nRetAcc = 200;
	/*if (nRetDcc < 200)*/ nRetDcc = 200;

	int nRet = AfxMessageBox("Do you want to move motor?", MB_YESNO);
	if( nRet == IDYES )
	{
		double dTPos = m_TeachingData.GetTeachPos(m_nCurRobotId, nLocIdx, nPosIdx);
		int nAxisNo = tLocHead.anPosAxisNo[nPosIdx];
		if( nAxisNo == -1 ){
			return;
		}

		double dMtSpd = g_Motor[nAxisNo]->GetSpeed();
		double dMtAcc = g_Motor[nAxisNo]->GetAcc();
		double dMtDcc = g_Motor[nAxisNo]->GetDcc();

		dRetSpd = (dMtSpd * (1)) * (1);
		nRetAcc = (int)(dMtAcc - (dMtAcc*0.75)*(nRateAcc / 100.0)); // 설정되어 있는 값에서 spd rate가 높을 수록 acc/dcc를 감소 시켜준다. 가장 빠를 때가 설정 속도의 95%뺀 acc/dcc로 함.
		nRetDcc = (int)(dMtDcc - (dMtDcc*0.75)*(nRateDcc / 100.0));

		int nErr = 0;
		g_bManualMoveCancel = false;

		if (ChkInterlockBeforeMove(nAxisNo, dTPos) != ERR_NO_ERROR) {
			return;
		}

		if (nAxisNo >= eAXIS_TRAY_PP_1_Z && nAxisNo <= eAXIS_TEST_PP_2_Z2) {

			SetFlatnessOffset(nAxisNo, nLocIdx);

			std::vector<CPoint> vPicker;
			vPicker.clear();
			CDlgZPickerActionMove dlg;
			dlg.m_nAxisZNo = nAxisNo;
			int nBoardType = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);
			//BOOL bVatOffsetUse = g_DMCont.m_dmVAT.GB(BDM14_VAT_MD_USE);
			if (dlg.DoModal() == IDOK)
			{
				vPicker = dlg.m_vPicker;
				nErr = g_Motor[nAxisNo]->Move(vPicker, dTPos, 200, 100, 100, nBoardType, FALSE /*,bVatOffsetUse*/);
			}
			else{
				g_bManualMoveCancel = true;
			}
			//nErr = g_Motor[nAxisNo]->Move(eJogDir_Positive, MAX_PICKER_CNT_EACH_SLAVE, &nPicker[4], m_dJogSpeed, 20);
		}
		else if (nAxisNo >= eAXIS_LOAD_TABLE_1_ROTATE && nAxisNo <= eAXIS_LOAD_TABLE_4_ROTATE)
		{
			nErr = g_Motor[nAxisNo]->/*Move(dTPos, dRetSpd, nRetAcc, nRetDcc);*/Move(dTPos, dRetSpd, nRetAcc, nRetDcc);
		}
		else
		{
			nErr = g_Motor[nAxisNo]->/*Move(dTPos, dRetSpd, nRetAcc, nRetDcc);*/Move(dTPos, TEACH_MOVE_SPD_LOW, TEACH_MOVE_ACC_LOW, TEACH_MOVE_DEC_LOW);
		}

		if( nErr != ERR_NO_ERROR ){
			CParamList AlarmData;
			g_Error.AlarmReport(nErr, AlarmData, NULL);
			return;
		}

		GetDlgItem(IDC_COMMANDBUTTON_MANU_MOTOR)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMMANDBUTTON_TEACHING_JOG)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMMANDBUTTON_MOVE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMMANDBUTTON_TESTPP)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMMANDBUTTON_TRAYPP)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMMANDBUTTON_PRESS)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMMANDBUTTON_SETPLATE_STACKER)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMMANDBUTTON_PARAMETER)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMMANDBUTTON_RELOAD)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMMANDBUTTON_SAVE)->EnableWindow(FALSE);
		GetDlgItem(IDC_OK)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPREAD_LOC_TABLE)->EnableWindow(FALSE);

		g_LibCommCtrl.ProgressBarCreate(true, callback_progressbar_cancel_event, nAxisNo);
		g_LibCommCtrl.ProgressBarSetStatus("Motor Axis Move!!!!");
		int nCnt =0;
		double dTimeStart = (double)GetTickCount64();

		
		while(1)
		{
			if( g_bManualMoveCancel == true ) {
				break;
			}

			MSG message;
			if(::PeekMessage(&message, NULL, 0, 0, PM_REMOVE)){
				::TranslateMessage(&message);
				::DispatchMessage(&message);
			}
			Sleep(200);

			g_LibCommCtrl.ProgressBarSetPos(nCnt);

			g_Motor[nAxisNo]->GetCurrentPos(dCurPos);
			nCnt = abs(dCurPos) / abs(dTPos) * 100;
		

			if (nCnt < 0) nCnt = 0;
			if (nCnt > 100) nCnt = 100;

			//nCnt++;

			double dSpanTime = (double)GetTickCount64();
			if((dSpanTime - dTimeStart) >= MOTOR_TIME_OUT_MIDDLE*2000.0){
				g_Motor[nAxisNo]->SetStop();
				g_LibCommCtrl.ProgressBarSetPos(100);
				break;
			}

			if( g_bManualMoveCancel == false ) {
				if(g_Motor[nAxisNo]->AxisDone() == ERR_NO_ERROR || g_Motor[ nAxisNo ]->AxisDone() != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CString strInformation = _T("");					
					g_Motor[nAxisNo]->GetCurrentPos(dCurPos);
					strInformation.Format(_T("Axis = %s ,Moving End Encoder :  %f"), g_Motor[nAxisNo]->GetAxisName(), dCurPos);
					m_EncoderLog.LOG(strInformation);

 					g_LibCommCtrl.ProgressBarSetPos( 100 );
 					break;
				}
			}
		}
		g_bManualMoveCancel = false;
		g_LibCommCtrl.ProgressBarDestroy();

		GetDlgItem(IDC_COMMANDBUTTON_MOVE)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMMANDBUTTON_MANU_MOTOR)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMMANDBUTTON_TEACHING_JOG)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMMANDBUTTON_MOVE)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMMANDBUTTON_TESTPP)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMMANDBUTTON_TRAYPP)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMMANDBUTTON_PRESS)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMMANDBUTTON_SETPLATE_STACKER)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMMANDBUTTON_PARAMETER)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMMANDBUTTON_RELOAD)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMMANDBUTTON_SAVE)->EnableWindow(TRUE);
		GetDlgItem(IDC_OK)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPREAD_LOC_TABLE)->EnableWindow(TRUE);
	}
}


void CDlgManuMotorTeaching::OnBnClickedCommandbuttonBase()
{
}

void CDlgManuMotorTeaching::OnBnClickedCommandbuttonParameter()
{
	CDlgManuMotorParam dlg;
	dlg.DoModal();	
}


void CDlgManuMotorTeaching::OnBnClickedCommandbuttonSave()
{
	int nRet = AfxMessageBox("do you want to save?", MB_YESNO);
	if( nRet == IDYES ){
		SaveTeachingData();
	}
}


void CDlgManuMotorTeaching::OnBnClickedOk()
{
// 	BOOL bFlag = g_DMCont.m_dmHandlerOpt.GB(BDM11_GUI_ALIGNMENT_OPEN_HISTORY);
// 	if (bFlag == TRUE) {
// 		g_DMCont.m_dmHandlerOpt.SB(BDM11_GUI_ALIGNMENT_OPEN_HISTORY, FALSE);
// 		g_DMCont.m_dmHandlerOpt.SB(BDM11_GUI_OPEN_HISTORY, FALSE);
// 	}
	
	CDialogEx::OnOK();
}


void CDlgManuMotorTeaching::InitControl()
{
	CString strRobotName = _T("");
	char szCokName[128]={0,};
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));
	
	SetDlgItemText(IDC_EDIT_DEVICE, szCokName);

	// Draw Robot List
	m_TeachingData.ReadRobotNameList(m_RobotNameList);

	int nCount = m_RobotNameList.m_fnGetCount();
	for(int i=0; i<nCount; i++)
	{
		TRobotName* pItem = (TRobotName*)m_RobotNameList.m_fnGetData(i);
		if( pItem )
		{
			switch(pItem->nRobotId)
			{
			case tRobotId_PRESS_DN1_SITE1: { strRobotName.Format("Station#1"); } break;
			case tRobotId_PRESS_DN2_SITE2: { strRobotName.Format("Station#2"); } break;
			case tRobotId_PRESS_DN3_SITE3: { strRobotName.Format("Station#3"); } break;
			case tRobotId_PRESS_DN4_SITE4: { strRobotName.Format("Station#4"); } break;
			case tRobotId_PRESS_DN5_SITE5: { strRobotName.Format("Station#5"); } break;
			case tRobotId_PRESS_DN6_SITE6: { strRobotName.Format("Station#6"); } break;
			case tRobotId_PRESS_DN7_SITE7: { strRobotName.Format("Station#7"); } break;
			case tRobotId_PRESS_DN8_SITE8: { strRobotName.Format("Station#8"); } break;
			case tRobotId_TESTPP_1 : { strRobotName.Format("Test PP 1"); } break;
			case tRobotId_TESTPP_2 : { strRobotName.Format("Test PP 2"); } break;
			case tRobotId_SHUTTLE_1 : { strRobotName.Format("Load Table 1_ULD"); } break;
			case tRobotId_SHUTTLE_2 : { strRobotName.Format("Load Table 2_LD"); } break;
			case tRobotId_SHUTTLE_3: { strRobotName.Format("Load Table 3_LD"); } break;
			case tRobotId_SHUTTLE_4: { strRobotName.Format("Load Table 4_ULD"); } break;
			case tRobotId_TRAYPP_1 : { strRobotName.Format("Tray Load PP"); } break;
			case tRobotId_TRAYPP_2: { strRobotName.Format("Tray Unload PP"); } break;
			case tRobotId_FEEDER: {strRobotName.Format("Feeder"); } break;
			case tRobotId_TRANSFER : { strRobotName.Format("Transfer"); } break;
			}
			//m_cbRobotName.AddString(pItem->szName);
			m_cbRobotName.AddString(strRobotName);
		}
	}
	m_cbRobotName.SetCurSel(0);
	TRobotName* pItem = (TRobotName*)m_RobotNameList.m_fnGetData(0);
	int nRobotId = pItem->nRobotId;
	UpdateTeachingData(nRobotId, FALSE);
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-10-09]
  NAME     : 
  PARAMS   : nRobotId         : Robot고유id,  #define RID_
             bSaveAndValidate : TRUE = Control(화면)에 있는 정보를 메모리에 적용한다.
			                    FALSE = 메모리에 있는 정보를 Control(화면)에 반영한다.
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CDlgManuMotorTeaching::UpdateTeachingData(int nRobotId, BOOL bSaveAndValidate/*=TRUE*/)
{
	//Draw Location Color Init
	for (int row = 1; row <= m_nLastLocCount; row++) {
		for (int col = 1; col <= m_nLastPosCount; col++) {
			m_sprLocTable.SetRow(row);
			m_sprLocTable.SetCol(col);

			m_sprLocTable.SetBackColor(WWHITE);
			m_sprLocTable.SetForeColor(BBLACK);
		}
	}
	
	// 요청 사항[11/12/2024 dohyeong.kim]
	// Test PP Teaching Position 한 페이지에서 볼수 있도록 Col 폭조절 
	//m_sprLocTable.SetColWidth(0, 20.5);
	m_sprLocTable.SetColWidth(-1, 10.5);

	ST_LOCATION_HEAD tPositionNames;
	m_TeachingData.ReadPosNameList(nRobotId, tPositionNames);
	int nPosCount = tPositionNames.nPosCount;
	CStringList LocNameList;
	m_TeachingData.ReadLocNameList(nRobotId, LocNameList);
	int nLocCount = LocNameList.GetCount();
	
	m_nLastPosCount = nPosCount;
	m_nLastLocCount = nLocCount;

	int nRow, nCol;
	
	if( bSaveAndValidate == FALSE )
	{

		int test = m_sprLocTable.GetMaxCols();
		// clear data
		for(nCol=0; nCol<m_sprLocTable.GetMaxCols()+1; nCol++){
			for(nRow=0; nRow<m_sprLocTable.GetMaxRows()+1; nRow++){
				m_sprLocTable.SetTextMatrix(nRow, nCol, "");
			}
		}

		m_sprLocTable.SetCol(m_nLastSel_Col);
		m_sprLocTable.SetRow(m_nLastSel_Row);
		m_sprLocTable.SetBackColor(COLOR_WWHITE);

		m_sprLocTable.ClearRange(0,0,-1,-1,1);
		m_sprLocTable.SetFontName("Times New Roman");
		m_sprLocTable.SetFontSize(14);


		m_sprLocTable.SetRowHeight(-1 , 20);
		m_sprLocTable.SetScrollBarExtMode(TRUE);

		// Draw Position Name
		for(nCol=1; nCol<=nPosCount; nCol++)
		{
			CString strVal = tPositionNames.aszPosName[nCol-1];
			//m_sprLocTable.SetTextMatrix(0, nCol, strVal);

			if (nRobotId == tRobotId_TESTPP_1 || nRobotId == tRobotId_TESTPP_2)
			{
				if (m_nTestPpType == eTestPPLoadType) {
					if (1 <= nCol && nCol <= 7) { //Load만 표기.
						m_sprLocTable.SetTextMatrix(0, nCol, strVal);
					}
					else {
						continue;
					}
				}
				else {
					if (nCol > 7) {//Unload만 표기.
						m_sprLocTable.SetTextMatrix(0, nCol - 7, strVal);
					}
					else {
						continue;
					}
				}
			}
			else
			{
				m_sprLocTable.SetTextMatrix(0, nCol, strVal);
			}
		}

		// Draw Location Name
		POSITION pos = LocNameList.GetHeadPosition();
		for(nRow=1; nRow<=nLocCount; nRow++)
		{
// 			CString strVal = LocNameList.GetNext(pos);
// 			m_sprLocTable.SetTextMatrix(nRow, 0, strVal);

			CString strVal = LocNameList.GetNext(pos);
		
			if (nRobotId == tRobotId_TESTPP_1 || nRobotId == tRobotId_TESTPP_2)
			{
				if (m_nTestPpType == eTestPPLoadType) {
					m_sprLocTable.SetRow(nRow);
					m_sprLocTable.SetRowHidden(FALSE);
				}
				else {
					//불필요한 Teaching Point HIDE
					if (nRow == (LOC_IDX_TEST_PP_TBL_1_LD + 1) ||
						nRow == (LOC_IDX_TEST_PP_TBL_2_LD + 1) ||
						nRow == (LOC_IDX_TEST_PP_TBL_4_LD + 1) ||
						nRow == (LOC_IDX_CLEAN_TBL + 1))
					{
						m_sprLocTable.SetRow(nRow);
						m_sprLocTable.SetRowHidden(TRUE);
					}

					//Buffer Teaching Point로 이름 변경.
					if (nRow == (LOC_IDX_TEST_PP_TBL_3_LD + 1))
					{
						strVal = _T("Buffer Table");
					}
				}
			}
			else {
				m_sprLocTable.SetRow(nRow);
				m_sprLocTable.SetRowHidden(FALSE);
			}

			m_sprLocTable.SetTextMatrix(nRow, 0, strVal);

		}

		// Draw Location Data
		for(nRow=1; nRow<=nLocCount; nRow++)
		{
			
			for(nCol=1; nCol<=nPosCount; nCol++)
			{
				BOOL bChkCellBlock = ChkCellBlock(nRow,nCol, nRobotId);
				CString strVal;
				m_sprLocTable.SetRow(nRow);
				m_sprLocTable.SetCol(nCol);

				m_sprLocTable.SetCellType(5);
				m_sprLocTable.SetTypeHAlign(2); //center
				m_sprLocTable.SetTypeVAlign(2); //center
				if (bChkCellBlock == TRUE)
				{
					m_sprLocTable.SetBackColor(DDARKGRAY);
					m_sprLocTable.SetForeColor(WWHITE);

					strVal.Format("NONE");
				}
				else
				{
					m_sprLocTable.SetBackColor(WWHITE);
					m_sprLocTable.SetForeColor(BBLACK);

					double dTPos = m_TeachingData.GetTeachPos(nRobotId, nRow - 1, nCol - 1);
					strVal.Format("%.3f", dTPos / 1000.0); // 표시는 모두 [mm] 단위로 한다.
					
				}

				//기존
				//m_sprLocTable.SetTextMatrix(nRow, nCol, strVal);

				//Load/Unload Hand Data 따로 표기.
				if (nRobotId == tRobotId_TESTPP_1 || nRobotId == tRobotId_TESTPP_2)
				{
					//Test PP Position 표기.
					if (m_nTestPpType == eTestPPLoadType) {
						if (1 <= nCol && nCol <= 7) { //Load만 표기.
							m_sprLocTable.SetTextMatrix(nRow, nCol, strVal);
						}
						else {
							continue;
						}
					}
					else {
						if (nCol > 7) {//Unload만 표기.
							m_sprLocTable.SetTextMatrix(nRow, nCol - 7, strVal);
						}
						else {
							continue;
						}
					}
				}
				else {
					m_sprLocTable.SetTextMatrix(nRow, nCol, strVal);
				}



			}
		}
	}
	else
	{
		for(nRow=1; nRow<=nLocCount; nRow++)
		{
			for(nCol=1; nCol<=nPosCount; nCol++)
			{
		

				if (nRobotId == tRobotId_TESTPP_1 || nRobotId == tRobotId_TESTPP_2)
				{
					if (m_nTestPpType == eTestPPLoadType) {
						if (1 <= nCol && nCol <= 7) { //Load만 저장.
							CString strVal = m_sprLocTable.GetTextMatrix(nRow, nCol);
							double dTPos = atof(strVal)*1000.0;
							m_TeachingData.SetTeachPos(nRobotId, nRow - 1, nCol - 1, dTPos);
						}
						else {
							continue;
						}
					}
					else {
						if (nCol > 7) { //Unload만 저장.
							CString strVal = m_sprLocTable.GetTextMatrix(nRow, nCol - 7);
							double dTPos = atof(strVal)*1000.0;
							m_TeachingData.SetTeachPos(nRobotId, nRow - 1, nCol - 1, dTPos);
						}
						else {
							continue;
						}
					}
				}
				else
				{	
// 					CString strVal = m_sprLocTable.GetTextMatrix(nRow, nCol - 7);
// 					double dTPos = atof(strVal)*1000.0;
// 					m_TeachingData.SetTeachPos(nRobotId, nRow - 1, nCol - 1, dTPos);
					CString strVal = m_sprLocTable.GetTextMatrix(nRow, nCol);
					double dTPos = atof(strVal)*1000.0;
					m_TeachingData.SetTeachPos(nRobotId, nRow - 1, nCol - 1, dTPos);
				}

				//기존 방식
				// 				CString strVal = m_sprLocTable.GetTextMatrix(nRow, nCol);
				// 				double dTPos = atof(strVal)*1000.0;
				// 				m_TeachingData.SetTeachPos(nRobotId, nRow - 1, nCol - 1, dTPos);
			}
		}
	}

	m_nCurRobotId = nRobotId;

// 	m_sprLocTable.ClearSelection();
// 	m_sprLocTable.


}

void CDlgManuMotorTeaching::SaveTeachingData()
{
	WriteChangeTeachLog();
	g_DMCont.m_Teach = m_TeachingData;
	g_DMCont.m_Teach.Save();

	//m_TeachingData = g_DMCont.m_Teach;
}

void CDlgManuMotorTeaching::SelchangeCbRobotChoose()
{
	int nSel = m_cbRobotName.GetCurSel();
	TRobotName* pItem = (TRobotName*)m_RobotNameList.m_fnGetData(nSel);
	int nRobotId = pItem->nRobotId;

	CDSSPanel *pSSTeachType_Single = (CDSSPanel *)GetDlgItem(IDC_SSPANEL_SINGLE_TEACHING);
	CDSSPanel *pSSTeachType_Group = (CDSSPanel *)GetDlgItem(IDC_SSPANEL_GROUP_TEACHING);
	CDSSPanel *pSSTestPpType = (CDSSPanel *)GetDlgItem(IDC_SSPANEL_TESTPP_TYPE);
	if (nRobotId == tRobotId_TESTPP_1 || nRobotId == tRobotId_TESTPP_2){
		pSSTestPpType->ShowWindow(SW_SHOW);
		DisplayTestPpSSPanel();

		pSSTeachType_Single->EnableWindow(TRUE);
		pSSTeachType_Group->EnableWindow(TRUE);
		DisplayGroupTeachSSPanel();
	}
	else {
		pSSTestPpType->ShowWindow(SW_HIDE);

		pSSTeachType_Single->EnableWindow(FALSE);
		pSSTeachType_Group->EnableWindow(FALSE);

		pSSTeachType_Single->SetBackColor(DISABLE_BACK_COLOR);
		pSSTeachType_Single->SetForeColor(DISABLE_FORE_COLOR);
		pSSTeachType_Group->SetBackColor(DISABLE_BACK_COLOR);
		pSSTeachType_Group->SetForeColor(DISABLE_FORE_COLOR);
	}


	UpdateTeachingData(nRobotId, FALSE);
}

void CDlgManuMotorTeaching::OnCbnSelchangeCbRobotChoose()
{
	SelchangeCbRobotChoose();
}

BEGIN_EVENTSINK_MAP(CDlgManuMotorTeaching, CDialogEx)
	ON_EVENT(CDlgManuMotorTeaching, IDC_SPREAD_LOC_TABLE, 5, CDlgManuMotorTeaching::Click_RobotTeachingTable, VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuMotorTeaching, IDC_SPREAD_LOC_TABLE, 11, CDlgManuMotorTeaching::DblClick_RobotTeachingTable, VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuMotorTeaching, IDC_SSPANEL_TESTPP_TYPE, DISPID_CLICK, CDlgManuMotorTeaching::ClickSspanelTestppType, VTS_NONE)
	ON_EVENT(CDlgManuMotorTeaching, IDC_SSPANEL_SINGLE_TEACHING, DISPID_CLICK, CDlgManuMotorTeaching::ClickSspanelSingleTeaching, VTS_NONE)
	ON_EVENT(CDlgManuMotorTeaching, IDC_SSPANEL_GROUP_TEACHING, DISPID_CLICK, CDlgManuMotorTeaching::ClickSspanelGroupTeaching, VTS_NONE)

END_EVENTSINK_MAP()


void CDlgManuMotorTeaching::Click_RobotTeachingTable(long Col, long Row)
{
	//NONE 처리
	BOOL bChkCellBlock = ChkCellBlock(Row, Col, m_nCurRobotId);
	if (bChkCellBlock == TRUE) {
		return;
	}
	
	//Click 했을 때, GroupPoint있을 경우, 색상 전체 초기화.
	for (int i = 0; i < m_vGroupPoint.size(); i++) {
		m_sprLocTable.SetCol(m_vGroupPoint[i].first);
		m_sprLocTable.SetRow(m_vGroupPoint[i].second);
		m_sprLocTable.SetBackColor(COLOR_WWHITE);
	}

	if (m_nTeachType == _eTeachType_Group)
	{
		//Last Sell Group Position 확인.
		BOOL bIsGroupPos_LastSel = IsGroupTeachPosition(m_nLastSel_Col, m_nLastSel_Row);
		if (bIsGroupPos_LastSel == TRUE) {
			ChangeGroupTeachingColor(m_nLastSel_Col, m_nLastSel_Row, COLOR_WWHITE);
		}
		else {
			ChangeSingleTeachingColor(m_nLastSel_Col, m_nLastSel_Row, COLOR_WWHITE);
		}
		m_vGroupPoint.clear();

		//Current Sell Group Position 확인.
		BOOL bIsGroupPos_CurSel = IsGroupTeachPosition(Col, Row);
		if (bIsGroupPos_CurSel == TRUE) {
			ChangeGroupTeachingColor(Col, Row, COLOR_RED);
		}
		else {
			ChangeSingleTeachingColor(Col, Row, COLOR_RED);
		}

		//Y Group은 Load & Unload PP 묶여 있어서, Hand Type Switch될 경우, 표기가 필요함.
// 		if (m_vGroupPoint.size() > 0 && Col != eCOL2_Y)
// 		{
// 			m_vGroupPoint.clear();
// 		}

	}
	else {
		m_sprLocTable.SetRow(m_nLastSel_Row);
		m_sprLocTable.SetCol(m_nLastSel_Col);
		m_sprLocTable.SetBackColor(COLOR_WWHITE);

		m_sprLocTable.SetRow(Row);
		m_sprLocTable.SetCol(Col);
		m_sprLocTable.SetBackColor(COLOR_RED);
	}



	m_nLastSel_Row = Row;
	m_nLastSel_Col = Col;

	UpdateData(FALSE);
}


void CDlgManuMotorTeaching::DblClick_RobotTeachingTable(long Col, long Row)
{
	if(Col == 0 || Row == 0){
		return;
	}

	//NONE 처리
	BOOL bChkCellBlock = ChkCellBlock(Row, Col, m_nCurRobotId);
	if (bChkCellBlock == TRUE) {
		return;
	}


 	char szValue_New[128]={0,};
 	char szValue_Old[128]={0,};
 	char szTitle[128]={0,};
 	char szMin[128]={0,};
 	char szMax[128]={0,};

	int nLocIdx, nPosIdx;

	nPosIdx = m_nLastSel_Col - 1;

	if(m_nCurRobotId == tRobotId_TESTPP_1 || m_nCurRobotId == tRobotId_TESTPP_2)
	{
		if (m_nTestPpType == eTestPPLoadType) {
			nPosIdx = m_nLastSel_Col - 1;
		}
		else {
			nPosIdx = (m_nLastSel_Col + 7) - 1;
		}
	}




	nLocIdx = m_nLastSel_Row-1;

	ST_LOCATION_HEAD tLocHead;	
	m_TeachingData.GetTeachPosHead(m_nCurRobotId, tLocHead);
	int nAxisNo = tLocHead.anPosAxisNo[nPosIdx];
	_tMOTOR_PARAM tMotorParam;
	g_DB.SELECT_MOTORPARAM_TBL(nAxisNo, tMotorParam);
 
	
	BOOL bIsShowFlatnessOffset = ChkShowFlatnessOffset(nAxisNo, nLocIdx, nPosIdx);


	if (eAXIS_TRAY_PP_1_Z <= nAxisNo && nAxisNo <= eAXIS_TEST_PP_2_Z2 && bIsShowFlatnessOffset == TRUE)
	{
		CString strCurValue = m_sprLocTable.GetTextMatrix(Row, Col);

		int nPartNo = ConvertTeachLocStagetoVatPartIdx(nAxisNo, nLocIdx);

 		CDlgManuZMotorTeaching dlg(&strCurValue, m_nCurRobotId,  nLocIdx, nPosIdx, nAxisNo, nPartNo); //nAxisNo, nPartNo는 Picker Gap을 위함.
  		dlg.DoModal();

		m_TeachingData = g_DMCont.m_Teach;
		m_sprLocTable.SetTextMatrix(Row, Col, strCurValue);
	}
	else
	{
		CString strValue_Old = m_sprLocTable.GetTextMatrix(Row, Col);

		sprintf_s(szValue_Old, sizeof(szValue_Old), "%s", strValue_Old.GetString());

		if (nAxisNo >= eAXIS_LOAD_TABLE_1_ROTATE && nAxisNo <= eAXIS_LOAD_TABLE_4_ROTATE)
		{
			sprintf_s(szMax, sizeof(szMax), "%.3f", tMotorParam.dLimit_Posi);
			sprintf_s(szMin, sizeof(szMin), "%.3f", tMotorParam.dLimit_Nega);
		}
		else
		{
			sprintf_s(szMax, sizeof(szMax), "%.3f", tMotorParam.dLimit_Posi / 1000.0);
			sprintf_s(szMin, sizeof(szMin), "%.3f", tMotorParam.dLimit_Nega / 1000.0);
		}
	
		int nRet = g_LibCommCtrl.GetNumberBox(this, szValue_New, sizeof(szValue_New), szValue_Old, "Input"/*szTitle*/, szMax, szMin);
		if (nRet == RETURN_OK) {
			if (m_nTeachType == _eTeachType_Group)
			{
				BOOL bIsGroupPos = IsGroupTeachPosition(Col, Row);
				if (bIsGroupPos == TRUE) {
					ChangeGroupTeachingData(Col, Row, szValue_New);
				}
				else {
					m_sprLocTable.SetTextMatrix(Row, Col, szValue_New);
					UpdateTeachingData(m_nCurRobotId);
				}
			}
			else
			{
				m_sprLocTable.SetTextMatrix(Row, Col, szValue_New);


				//AutoCalculateValue(m_nCurRobotId, Col, Row);

				UpdateTeachingData(m_nCurRobotId);
			}
		}
	}


// 이전 사용 방식 [7/3/2025 dohyeong.kim]
// 	CString strValue_Old = m_sprLocTable.GetTextMatrix(Row, Col);
// 
//  	sprintf_s(szValue_Old, sizeof(szValue_Old), "%s", strValue_Old.GetString() );
// 
// 	if (nAxisNo >= eAXIS_LOAD_TABLE_1_ROTATE && nAxisNo <= eAXIS_LOAD_TABLE_4_ROTATE)
// 	{
// 		sprintf_s(szMax, sizeof(szMax), "%.3f", tMotorParam.dLimit_Posi);
// 		sprintf_s(szMin, sizeof(szMin), "%.3f", tMotorParam.dLimit_Nega);
// 	}
// 	else
// 	{
// 		sprintf_s(szMax, sizeof(szMax), "%.3f", tMotorParam.dLimit_Posi / 1000.0);
// 		sprintf_s(szMin, sizeof(szMin), "%.3f", tMotorParam.dLimit_Nega / 1000.0);
// 	}
//  
//  
//  	int nRet = g_LibCommCtrl.GetNumberBox(this, szValue_New, sizeof(szValue_New), szValue_Old, "Input"/*szTitle*/, szMax, szMin);
// 	if( nRet == RETURN_OK ){
// 		if (m_nTeachType == _eTeachType_Group)
// 		{
// 			BOOL bIsGroupPos = IsGroupTeachPosition(Col, Row);
// 			if (bIsGroupPos == TRUE) {
// 				ChangeGroupTeachingData(Col, Row, szValue_New);
// 			}
// 			else {
// 				m_sprLocTable.SetTextMatrix(Row, Col, szValue_New);
// 				UpdateTeachingData(m_nCurRobotId);
// 			}
// 		}
// 		else
// 		{
// 			m_sprLocTable.SetTextMatrix(Row, Col, szValue_New);
// 			UpdateTeachingData(m_nCurRobotId);
// 		}
// 	}
}

BOOL CDlgManuMotorTeaching::IsGroupTeachPosition(long Col, long Row)
{
	if (m_nTeachType == _eTeachType_Single) {
		return FALSE;
	}

	if (!(m_nCurRobotId == tRobotId_TESTPP_1 || m_nCurRobotId == tRobotId_TESTPP_2)) {
		return FALSE;
	}


	switch (Col)
	{
		case eCOL5_Z_Safety:
		//case eCOL12_Z2_Safety:
		{
			if (eRow1_Station1 <= Row && Row <= eRow14_Home) {
				return TRUE; //Group Position
			}
			
		}break;
		case eCOL6_X_Pitch:
		case eCOL7_Y_Pitch:
		//case eCOL13_X2_Pitch:
		//case eCOL14_Y2_Pitch:
		{
			if (Row == eRow13_Socket_Clean || Row == eRow14_Home) {
				return FALSE;
			}
	
			if (eRow1_Station1 <= Row && Row <= eRow12_Loading_Table_4_UD)
			{
				return TRUE; //Group Position
			}
		}break;
		case eCOL2_Y: 
		//case eCOL9_Y2: 
		{
			if (m_nTestPpType == eTestPPLoadType) {
// 				if (eRow5_Loading_Table_1_LD <= Row && Row <= eRow8_Loading_Table_4_LD) {
// 					return TRUE; //Group Position
// 				}

				if (Row != eRow13_Socket_Clean && Row != eRow14_Home) {
					return TRUE; //Group Position
				}
			}
			else {
				if ((eRow1_Station1 <= Row && Row <= eRow4_Station4) || (eRow9_Loading_Table_1_UD <= Row && Row <= eRow12_Loading_Table_4_UD)) {
					return TRUE; //Group Position
				}
			}

//			for (int i = 0; i < 4; i++) { //vYAxisPoint Size, 총 4point
//				for (int j = 0; j < 3; j++) { //point당 연관된 위치 갯수, 총 3개
//					if (Col == (long)m_vYAxisGroupPoint[i][j].first && Row == (long)m_vYAxisGroupPoint[i][j].second) {
//						return TRUE; //Group Position
//  						if (m_nTestPpType == eTestPPLoadType) {
//  							if (eRow5_Loading_Table_1_LD <= Row && Row <= eRow8_Loading_Table_4_LD) {
//  								return TRUE; //Group Position
//  							}
//  						}
//  						else {
//  							if ((eRow1_Station1 <= Row && Row <= eRow4_Station4) || (eRow9_Loading_Table_1_UD <= Row && Row <= eRow12_Loading_Table_4_UD)) {
//  								return TRUE; //Group Position
//  							}
//  						}

						
//					}
//				}
//			}
	
//			return FALSE;
		}break;
	}

	return FALSE;
}


void CDlgManuMotorTeaching::SetGroupTeachPosition(long Col, long Row)
{
	if (m_nTeachType == _eTeachType_Single) {
		return;
	}

	if (!(m_nCurRobotId == tRobotId_TESTPP_1 || m_nCurRobotId == tRobotId_TESTPP_2)) {
		return;
	}
	
	m_vGroupPoint.clear();

	switch (Col)
	{
		case eCOL5_Z_Safety:
		//case eCOL12_Z2_Safety:
		{
			for (int nRow = eRow1_Station1; nRow <= eRow14_Home; nRow++) {
				m_vGroupPoint.push_back(std::make_pair(Col, nRow));
			}
		}break;
		case eCOL6_X_Pitch:
		case eCOL7_Y_Pitch:
		//case eCOL13_X2_Pitch:
		//case eCOL14_Y2_Pitch:
		{
			if (Row == eRow13_Socket_Clean || Row == eRow14_Home) {
				return;
			}

			int nROW_START_IDX = 0;
			int nROW_END_IDX = 0;
			//Station
			if (eRow1_Station1 <= Row && Row <= eRow4_Station4)
			{
				nROW_START_IDX = eRow1_Station1;
				nROW_END_IDX = eRow4_Station4;
			}
			//Loading Table
			else
			{
				nROW_START_IDX = eRow5_Loading_Table_1_LD;
				nROW_END_IDX = eRow12_Loading_Table_4_UD;
			}


			for (int nRow = nROW_START_IDX; nRow <= nROW_END_IDX; nRow++) {
				m_vGroupPoint.push_back(std::make_pair(Col, nRow));
			}

		}break;
		case eCOL2_Y: // Station Unload PP Y축 Teaching과 연관
		{
			//============================================================Load Type Group Point
			if (m_nTestPpType == eTestPPLoadType)
			{
				if (Row == eRow13_Socket_Clean || Row == eRow14_Home) {
					return;
				}

				if ((eRow1_Station1 <= Row && Row <= eRow4_Station4))
				{
					m_vGroupPoint.push_back(std::make_pair(Col, Row)); //click point
					m_vGroupPoint.push_back(std::make_pair(Col, (Row-1) + eRow9_Loading_Table_1_UD));
					return;
				}
				else if (eRow9_Loading_Table_1_UD <= Row && Row <= eRow12_Loading_Table_4_UD)
				{
					m_vGroupPoint.push_back(std::make_pair(Col, Row));
					m_vGroupPoint.push_back(std::make_pair(Col, eRow1_Station1 + (Row - eRow9_Loading_Table_1_UD)));
					return;
				}
			}
			


			//============================================================Load PP & Unload PP Group Point
			BOOL bIsGroup = FALSE;
			int nMainGroupIdx = -1;
 			for (int i = 0; i < 4; i++) { //vYAxisPoint Size, 총 4point
 				for (int j = 0; j < 3; j++) { //point당 연관된 위치 갯수, 총 3개
 					if (Row == (long)m_vYAxisGroupPoint[i][j].second) {
						if (m_nTestPpType == eTestPPLoadType) {
							if (eRow5_Loading_Table_1_LD <= Row && Row <= eRow8_Loading_Table_4_LD) {
								nMainGroupIdx = i;
							}
						}
						else {
							if ((eRow1_Station1 <= Row && Row <= eRow4_Station4) || (eRow9_Loading_Table_1_UD <= Row && Row <= eRow12_Loading_Table_4_UD)) {
								nMainGroupIdx = i;
							}
						}
 					}
 				}
 			}

			//Load, Unload PP Point총 3ea
			if (nMainGroupIdx != -1) {
				for (int nSubGroupIdx = 0; nSubGroupIdx < 3; nSubGroupIdx++) {
					m_vGroupPoint.push_back(m_vYAxisGroupPoint[nMainGroupIdx][nSubGroupIdx]);
				}
			}


		}break;
		default:
			break;
	}

}

void CDlgManuMotorTeaching::ChangeSingleTeachingColor(long Col, long Row, OLE_COLOR color)
{
	m_sprLocTable.SetCol(Col);
	m_sprLocTable.SetRow(Row);
	m_sprLocTable.SetBackColor(color);
}


void CDlgManuMotorTeaching::ChangeGroupTeachingColor(long Col, long Row, OLE_COLOR color)
{
	if (m_nTeachType == _eTeachType_Single) {
		return;
	}

	if (!(m_nCurRobotId == tRobotId_TESTPP_1 || m_nCurRobotId == tRobotId_TESTPP_2)) {
		return;
	}

	SetGroupTeachPosition(Col, Row);

	for (int i = 0; i < m_vGroupPoint.size(); i++) {
		if (Col == eCOL2_Y && m_vGroupPoint.size() == 3) {
			//Y축은 Load/Unload 구분하여 표기.
			//Y축 Group은 총 3가지 Point로 첫번째만 Load Hand Position.
			if (m_nTestPpType == eTestPPLoadType) {
				if (i != 0) continue;
			}
			else {
				if (i == 0) continue;
			}
		}
		m_sprLocTable.SetCol(m_vGroupPoint[i].first);
		m_sprLocTable.SetRow(m_vGroupPoint[i].second);
		m_sprLocTable.SetBackColor(color);
	}
}


void CDlgManuMotorTeaching::ChangeGroupTeachingData(long Col, long Row, LPCTSTR strValue)
{
	//m_sprLocTable.SetTextMatrix(Row, Col, szValue);
	if (!(m_nCurRobotId == tRobotId_TESTPP_1 || m_nCurRobotId == tRobotId_TESTPP_2)) {
		return;
	}

	SetGroupTeachPosition(Col, Row);

	double dTPos = atof(strValue) * 1000.0;;

	for (int i = 0; i < m_vGroupPoint.size(); i++) {
		int nCol = m_vGroupPoint[i].first;
		int nRow = m_vGroupPoint[i].second;

		//Load Position & Unload Position 둘다 저장해야 함으로, 따로 Position 저장이 필요.
		//Load Position 1ea & Unload Position 2ea(총 3개)	
		if (Col == eCOL2_Y && m_vGroupPoint.size() == 3)
		{
			if (i == 0) //Load Position
			{
				if (m_nTestPpType == eTestPPLoadType) { //현재 Page가 Load Type일 경우, 표기
					m_sprLocTable.SetTextMatrix(nRow, nCol, strValue);
				}

				m_TeachingData.SetTeachPos(m_nCurRobotId, nRow - 1, nCol - 1, dTPos);
			}
			else //Unload Position
			{
				if (m_nTestPpType == eTestPPUnLoadType) { //현재 Page가 Unload Type일 경우, 표기
					m_sprLocTable.SetTextMatrix(nRow, nCol, strValue);
				}

				//Teach Position 저장 시에는 Unload로 index 변환.
				m_TeachingData.SetTeachPos(m_nCurRobotId, nRow - 1, (nCol + 7) - 1, dTPos);
			}		
		}
		else
		{
			m_sprLocTable.SetTextMatrix(nRow, nCol, strValue);

			if (m_nTestPpType == eTestPPLoadType) {
				m_TeachingData.SetTeachPos(m_nCurRobotId, nRow - 1, nCol - 1, dTPos);
			}
			else {
				m_TeachingData.SetTeachPos(m_nCurRobotId, nRow - 1, (nCol + 7) - 1, dTPos);
			}
			
			//UpdateTeachingData(m_nCurRobotId);
		}
	}
}


void CDlgManuMotorTeaching::OnBnClickedCommandbuttonReload()
{
	m_TeachingData = g_DMCont.m_Teach;
	SelchangeCbRobotChoose();
}


void CDlgManuMotorTeaching::OnBnClickedCommandbuttonTestpp()
{
	CDlgManuTestPP dlg;
	int nRobot = m_cbRobotName.GetCurSel();

	if(eMANU_OPER_TEST_PP1 <= nRobot && nRobot <= eMANU_OPER_TEST_PP2)
	{
		g_DMCont.m_dmEQP.SB(BDM0_OPEN_EJECTOR_DLG_HISTORY, TRUE);//jira 403
		dlg.m_nHandIdx =nRobot - eMANU_OPER_TEST_PP1;
		dlg.DoModal();
		g_DMCont.m_dmEQP.SB(BDM0_OPEN_EJECTOR_DLG_HISTORY, FALSE);//jira 403
	}
	else
	{
		AfxMessageBox("is not select Test PP Axis...Please check it !!");
	}
}


void CDlgManuMotorTeaching::OnBnClickedCommandbuttonTraypp()
{
	g_DMCont.m_dmEQP.SB(BDM0_OPEN_EJECTOR_DLG_HISTORY, TRUE);//jira 403
	CDlgManuTrayPP dlg;

	int nRobot = m_cbRobotName.GetCurSel();

	if (eMANU_OPER_TRAY_PP1_LD <= nRobot && nRobot <= eMANU_OPER_TRAY_PP2_UD)
	{
		g_DMCont.m_dmEQP.SB(BDM0_OPEN_EJECTOR_DLG_HISTORY, TRUE);//jira 403
		dlg.m_nHandIdx = nRobot - eMANU_OPER_TRAY_PP1_LD;
		dlg.DoModal();
		g_DMCont.m_dmEQP.SB(BDM0_OPEN_EJECTOR_DLG_HISTORY, FALSE);//jira 403
	}
	else
	{
		AfxMessageBox("is not select Tray PP Axis...Please check it !!");
	}

	g_DMCont.m_dmEQP.SB(BDM0_OPEN_EJECTOR_DLG_HISTORY, FALSE);//jira 403
}


void CDlgManuMotorTeaching::OnBnClickedCommandbuttonPress()
{
	CDlgManuPressUnit dlg;
	dlg.DoModal();
}


void CDlgManuMotorTeaching::OnBnClickedCommandbuttonSetplateStacker()
{
	CDlgManuSetplate dlg;
	dlg.DoModal();
}


void CDlgManuMotorTeaching::OnBnClickedCommandbuttonTransEvstacker()
{
	CDlgManuTransfer dlg;
	dlg.DoModal();
}

void CDlgManuMotorTeaching::OnBnClickedCommandbuttonReadEncoder()
{
	ST_LOCATION_HEAD tLocHead;	
	m_TeachingData.GetTeachPosHead(m_nCurRobotId, tLocHead);
	CStringList LocNameList;
	m_TeachingData.ReadLocNameList(m_nCurRobotId, LocNameList);

	if( m_nLastSel_Row <= 0 || m_nLastSel_Row > LocNameList.GetCount() ){
		return;
	}
	if( m_nLastSel_Col <= 0 || m_nLastSel_Col > tLocHead.nPosCount ){
		return;
	}

	int nLocIdx, nPosIdx;
// 	nPosIdx = m_nLastSel_Col-1;
// 	nLocIdx = m_nLastSel_Row-1;

	if (m_nCurRobotId == tRobotId_TESTPP_1 || m_nCurRobotId == tRobotId_TESTPP_2)
	{
		if (m_nTestPpType == eTestPPLoadType) {
			if (1 <= m_nLastSel_Col && m_nLastSel_Col <= 7) { //Load Index.
				nPosIdx = m_nLastSel_Col - 1;
			}
			else {
				return;
			}
		}
		else {
			if (1 <= m_nLastSel_Col && m_nLastSel_Col <= 7) { //Unload Index.
				nPosIdx = (m_nLastSel_Col + 7) - 1;
			}
			else {
				return;
			}
		}
	}
	else
	{
		nPosIdx = m_nLastSel_Col - 1;
	}

	nLocIdx = m_nLastSel_Row - 1;

	double dCurPos = 0.0;
	int nRet = AfxMessageBox("Do you want to read encoder?", MB_YESNO);

	if( nRet == IDYES )
	{
		int nAxisNo = tLocHead.anPosAxisNo[nPosIdx];
		if( nAxisNo == -1 ){
			return;
		}

		if (eAXIS_TRAY_PP_1_Z <= nAxisNo && nAxisNo <= eAXIS_TEST_PP_2_Z2) {
			//기존 평균값으로 GetEncoder
			//g_Motor[nAxisNo]->GetCurrenPosAvg(dCurPos);

			//변경 후, 기준 Picker값 Get Encoder하도록 수정.
			int nBBxIdx = 0;
//			switch (nAxisNo)
//			{
			if(eAXIS_TRAY_PP_1_Z == nAxisNo)		nBBxIdx = eBMAxis_LOAD_TRAY_Picker01;
			if(eAXIS_TEST_PP_1_Z == nAxisNo)		nBBxIdx = eBMAxis_LOAD_TESTPP_1_Picker08;
			if(eAXIS_TEST_PP_1_Z2 == nAxisNo)	nBBxIdx = eBMAxis_UNLOAD_TESTPP_1_Picker08;	
			if(eAXIS_TRAY_PP_2_Z == nAxisNo)		nBBxIdx = eBMAxis_UNLOAD_TRAY_Picker01;
			if(eAXIS_TEST_PP_2_Z == nAxisNo)		nBBxIdx = eBMAxis_LOAD_TESTPP_2_Picker08;
			if(eAXIS_TEST_PP_2_Z2 == nAxisNo)	nBBxIdx = eBMAxis_UNLOAD_TESTPP_2_Picker08;	
//			default:
//				break;
//			}

			g_Motor[nAxisNo]->GetCurrentPos(dCurPos, _eBbxmnpAxis(nBBxIdx));
		}
		else {
			g_Motor[nAxisNo]->GetCurrentPos(dCurPos);
		}
		//g_Motor[nAxisNo]->GetCurrentPos(dCurPos);
		dCurPos -= g_Motor[nAxisNo]->GetLastBacklashOffset();

		char szValue_New[128]={0,};
		double dMotorChangeType = 0.0;

		
		if (g_Motor[nAxisNo]->GetDegreeAxis() == TRUE)
		{
			dMotorChangeType = 1.0;
		}
		else
		{
			dMotorChangeType = 1000.0;
		}
		sprintf_s(szValue_New, sizeof(szValue_New), "%.3f", dCurPos/ dMotorChangeType);
		m_sprLocTable.SetTextMatrix(m_nLastSel_Row, m_nLastSel_Col, szValue_New);
		UpdateTeachingData(m_nCurRobotId);
	}
}


void CDlgManuMotorTeaching::MakeLog(LPCTSTR fmt, ...)
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

int CDlgManuMotorTeaching::ComInterlock()
{
	int nErr = ERR_NO_ERROR;
	nErr = g_TaskSystemCtrl.ChkComInterlock();

	return nErr;
}

void CDlgManuMotorTeaching::WriteChangeTeachLog()
{
	CString strLog = _T("");

	//m_TeachingData.ReadRobotNameList(m_RobotNameList);
	int nCount = m_RobotNameList.m_fnGetCount();

	for(int i=0; i<nCount; i++){
		TRobotName* pItem = (TRobotName*)m_RobotNameList.m_fnGetData(i);
		int nRobotId = pItem->nRobotId;

		ST_LOCATION_HEAD tPositionNames;
		m_TeachingData.ReadPosNameList(nRobotId, tPositionNames);
		int nPosCount = tPositionNames.nPosCount;
		CStringList LocNameList;
		m_TeachingData.ReadLocNameList(nRobotId, LocNameList);
		int nLocCount = LocNameList.GetCount();

		// Draw Location Data
		for(int nRow=1; nRow<=nLocCount; nRow++)
		{
			for(int nCol=1; nCol<=nPosCount; nCol++)
			{
				double dTPosAfter = m_TeachingData.GetTeachPos(nRobotId, nRow-1, nCol-1);
				double dTPosBefore = g_DMCont.m_Teach.GetTeachPos(nRobotId, nRow-1, nCol-1);
				if(dTPosBefore != dTPosAfter ){
					CString strValAxis = tPositionNames.aszPosName[nCol-1];
					CString strPosName = _T("");


					// Draw Location Name
					POSITION pos = LocNameList.GetHeadPosition();
					for(int i=1; i<=nLocCount; i++)
					{
						CString strTemp = LocNameList.GetNext(pos);
						if( i == nRow){
							strPosName = strTemp;
						}
					}

					
 					strcpy_s(tTeachLog.szRobot_Name, pItem->szName);
					strcpy_s(tTeachLog.szPos_Name, strPosName);
					strcpy_s(tTeachLog.szAxis_Name, strValAxis);
					tTeachLog.dPosition_Before = dTPosBefore / 1000.0;
					tTeachLog.dPosition_After = dTPosAfter / 1000.0;
					SYSTEMTIME t;
					::GetLocalTime(&t);
					tTeachLog.strOccurred_Time.Format("%04d-%02d-%02d %02d:%02d:%02d.%03d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);

					g_DBTeach.INSERT_TEACH_LOG_TBL(tTeachLog);

					strLog.Format(_T("Robot=%s, Pos=%s, Axis=%s ] Before=%.2f mm -> After=%.2f mm"), tTeachLog.szRobot_Name, (LPCTSTR)strPosName, (LPCTSTR)strValAxis, dTPosBefore/1000.0, dTPosAfter/1000.0);
					//TWLOG.MakeLog(static_cast<int>(eLogID_SYSTEM), Debug, "", __LINE__, NULL, strLog);
					TWLOG.MakeLog(static_cast<int>(eLogID_Teaching), Debug, "", __LINE__, NULL, strLog);
				}
			}
		}
	}	
}


void CDlgManuMotorTeaching::OnBnClickedCommandbuttonPcdbacklash()
{
	CDlgPcdBacklash dlg;
	dlg.DoModal();
}


void CDlgManuMotorTeaching::OnBnClickedCommandbuttonTestppOffset()
{
	g_GuiComm.SendTestPpOffSetShow();
	MakeLog("[TestPp Off set Dialog Click]");
}


void CDlgManuMotorTeaching::OnBnClickedCommandbuttonTransfer()
{
	CDlgManuTransfer dlg;
	dlg.DoModal();
}

// void CDlgManuMotorTeaching::OnBnClickedBtnPickeralign()
// {
// // 	CDlgVATMain dlgPickerAlign;
// // 	dlgPickerAlign.DoModal();
// }

void CDlgManuMotorTeaching::OnBnClickedCommandbuttonNewPcdbacklash()
{

}

void CDlgManuMotorTeaching::OnBnClickedBtnVisionAutoTeaching()
{
// 	CVAT_MainDlg DO;
// 	DO.DoModal();
// 
// 	g_DMCont.m_Teach.Load();
// 	m_TeachingData = g_DMCont.m_Teach;
// 
// 	int nSel = m_cbRobotName.GetCurSel();
// 	TRobotName* pItem = (TRobotName*)m_RobotNameList.m_fnGetData(nSel);
// 	int nRobotId = pItem->nRobotId;
// 	UpdateTeachingData(nRobotId, FALSE);
}


void CDlgManuMotorTeaching::OnBnClickedCommandbuttonTrayppOffset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CDlgManuMotorTeaching::OnBnClickedCommandbuttonNewPickerMotor()
{
}


BOOL CDlgManuMotorTeaching::ChkCellBlock(int nRow,int nCol, int nRobotid)
{
	BOOL nRet = FALSE;

	switch (nRobotid)
	{
		case tRobotId_TRAYPP_1:
		{
			if (nRow == 3 || nRow == 6) //Loading Table1, 4은 NONE
				nRet = TRUE;
		}break;
		case tRobotId_TRAYPP_2:
		{
			if (nRow == 6 || nRow == 7) // Loading Table 2, 4는 NONE
				nRet = TRUE;
		}break;	
		case tRobotId_TESTPP_1:
		case tRobotId_TESTPP_2:
		{
			//LOAD/UNLOAD Teaching Data 표기방법 이원화 후, 사용 안함.[11/6/2024 dohyeong.kim]

// 			if (nRow == 13 && (nCol >= 8 && nCol <= 14)) // Clean Table Test PP Unload Hand NONE
// 				nRet = TRUE;
// 			
// 			//Load Table Buffer 사용 구역 제외 다 NONE 처리
// 			if ((5 <= nRow && nRow <= 8) && (nCol >= 8 && nCol <= 14)) {
// 				if (nRow == 7)//Buffer 제외
// 					nRow = FALSE;
// 				else
// 					nRet = TRUE;
// 			}

		}
		default:
		{
	
		}break;
	}

	return nRet;
}

void CDlgManuMotorTeaching::SetFlatnessOffset(int nAxisNo, int nLocStage)
{
	int nHand = -1;

	if (eAXIS_TRAY_PP_1_Z == nAxisNo)		nHand = VAT_HAND_LOAD_TRAYPP;
	else if(eAXIS_TRAY_PP_2_Z == nAxisNo)	nHand = VAT_HAND_UNLOAD_TRAYPP;
	else if (eAXIS_TEST_PP_1_Z == nAxisNo)	nHand = VAT_HAND_LOAD_TESTPP1;
	else if (eAXIS_TEST_PP_1_Z2 == nAxisNo)	nHand = VAT_HAND_UNLOAD_TESTPP1;
	else if (eAXIS_TEST_PP_2_Z == nAxisNo)	nHand = VAT_HAND_LOAD_TESTPP2;
	else if (eAXIS_TEST_PP_2_Z2 == nAxisNo)	nHand = VAT_HAND_UNLOAD_TESTPP2;


	int nPart = ConvertTeachLocStagetoVatPartIdx(nAxisNo, nLocStage);

	if (nPart == -1) {
		return;
	}

	double dPickerGap[VAT_PICKER_MAX] = { 0, };
	for (int i = 0; i < VAT_PICKER_MAX; i++) {
		dPickerGap[i] = g_VATCommonData.m_stVATZTeachInfo.stPickerGap.getPickerGap(nHand, nPart, i);
	}

	g_pBbxmnpClient->SetFlatnessOffset(nHand, dPickerGap);

// 	int nIdx = ConvertTeachLocStagetoVatPartIdx(nAxisNo, nLocStage);
// 	
// 	if (nIdx == -1)
// 		return;
// 
// 	//memcpy
// //	switch (nAxisNo)
// //	{
// 		if (eAXIS_TRAY_PP_1_Z	==nAxisNo)	g_pBbxmnpClient->SetZAutoTeachingData(g_VATCommonData.m_stVATZTeachInfo_temp.dLoadTrayPp_Loc_PickerGap[nIdx]);						//break;
// 		if (eAXIS_TEST_PP_1_Z	==nAxisNo)	g_pBbxmnpClient->SetZAutoTeachingData(g_VATCommonData.m_stVATZTeachInfo_temp.dTestPp1_Loc_PickerGap[VAT_TESTPP_LOAD][nIdx]);		//break;
// 		if (eAXIS_TEST_PP_1_Z2	==nAxisNo)g_pBbxmnpClient->SetZAutoTeachingData(g_VATCommonData.m_stVATZTeachInfo_temp.dTestPp1_Loc_PickerGap[VAT_TESTPP_UNLOAD][nIdx]);	//	break;
// 		if (eAXIS_TRAY_PP_2_Z	==nAxisNo)	g_pBbxmnpClient->SetZAutoTeachingData(g_VATCommonData.m_stVATZTeachInfo_temp.dUnloadTrayPp_Loc_PickerGap[nIdx]);				//	break;
// 		if (eAXIS_TEST_PP_2_Z	==nAxisNo)	g_pBbxmnpClient->SetZAutoTeachingData(g_VATCommonData.m_stVATZTeachInfo_temp.dTestPp2_Loc_PickerGap[VAT_TESTPP_LOAD][nIdx]);	//	break;
// 		if (eAXIS_TEST_PP_2_Z2	==nAxisNo)g_pBbxmnpClient->SetZAutoTeachingData(g_VATCommonData.m_stVATZTeachInfo_temp.dTestPp2_Loc_PickerGap[VAT_TESTPP_UNLOAD][nIdx]);	//	break;
// //		default:
// //			break;
// //	}
}

int CDlgManuMotorTeaching::ConvertTeachLocStagetoVatPartIdx(int nAxisNo, int nLocStage)
{
	//Teaching Table Idx -> VAT
	int nRetIdx = -1;

	if (nAxisNo == eAXIS_TRAY_PP_1_Z)
	{
		switch (nLocStage)
		{
		case  eLocIdxTrayLoadPP::LOC_IDX_TRAY_LOAD_PP_FEEDER: {nRetIdx = VAT_IDX_TRAY_PP_FEEDER; }break;
		case eLocIdxTrayLoadPP::LOC_IDX_TRAY_LOAD_PP_CONV_3: {nRetIdx = VAT_IDX_TRAY_PP_CONV_3; }break;
			//		case eLocIdxTrayLoadPP::LOC_IDX_TRAY_LOAD_PP_TBL_1: {nLocConvertor = LOC_IDX_TRAY_PP_TBL_1; }break;
		case eLocIdxTrayLoadPP::LOC_IDX_TRAY_LOAD_PP_TBL_2: {nRetIdx = VAT_IDX_TRAY_PP_TBL_2; }break;
		case  eLocIdxTrayLoadPP::LOC_IDX_TRAY_LOAD_PP_TBL_3: {nRetIdx = VAT_IDX_TRAY_PP_TBL_3; }break;
			//		case eLocIdxTrayLoadPP::LOC_IDX_TRAY_LOAD_PP_TBL_4: {nLocConvertor = LOC_IDX_TRAY_PP_TBL_4; }break;
			//		case eLocIdxTrayLoadPPManual::LOC_IDX_TRAY_LOAD_PP_MANUAL_SAFETY: { nRetIdx = LOC_IDX_TRAY_PP_SAFETY; }break;
		default:
			break;
		}
	}
	else if(nAxisNo == eAXIS_TRAY_PP_2_Z)
	{
		switch (nLocStage)
		{
		case eLocIdxTrayUnLoadPP::LOC_IDX_TRAY_UNLOAD_PP_CONV_4: {nRetIdx = VAT_IDX_TRAY_PP_CONV_4; }break;
		case eLocIdxTrayUnLoadPP::LOC_IDX_TRAY_UNLOAD_PP_CONV_5: {nRetIdx = VAT_IDX_TRAY_PP_CONV_5; }break;
		case eLocIdxTrayUnLoadPP::LOC_IDX_TRAY_UNLOAD_PP_CONV_6: {nRetIdx = VAT_IDX_TRAY_PP_CONV_6; }break;
		case eLocIdxTrayUnLoadPP::LOC_IDX_TRAY_UNLOAD_PP_CONV_7: {nRetIdx = VAT_IDX_TRAY_PP_CONV_7; }break;
		case eLocIdxTrayUnLoadPP::LOC_IDX_TRAY_UNLOAD_PP_TBL_1: {nRetIdx = VAT_IDX_TRAY_PP_TBL_1; }break;
			// 		case eLocIdxTrayUnLoadPP::LOC_IDX_TRAY_UNLOAD_PP_TBL_2: {nLocConvertor = LOC_IDX_TRAY_PP_TBL_2; }break;
			// 		case eLocIdxTrayUnLoadPP::LOC_IDX_TRAY_UNLOAD_PP_TBL_3: {nLocConvertor = LOC_IDX_TRAY_PP_TBL_3; }break;
		case eLocIdxTrayUnLoadPP::LOC_IDX_TRAY_UNLOAD_PP_TBL_4: {nRetIdx = VAT_IDX_TRAY_PP_TBL_4; }break;
			//		case eLocIdxTrayUnLoadPPManual::LOC_IDX_TRAY_UNLOAD_PP_MANUAL_SAFETY: { nRetIdx = LOC_IDX_TRAY_PP_SAFETY; }break;
		default:
			break;
		}
	}
	else
	{
		if(nLocStage != LOC_IDX_TEST_PP_HOME)
			nRetIdx = nLocStage;
	}

	return nRetIdx;
}


void CDlgManuMotorTeaching::DisplayTestPpSSPanel()
{
	CDSSPanel *pSSTestPpType = (CDSSPanel *)GetDlgItem(IDC_SSPANEL_TESTPP_TYPE);
	if (m_nTestPpType == eTestPPLoadType)
	{
		
		pSSTestPpType->SetWindowText("LOAD");
		pSSTestPpType->SetBackColor(LLIGHTGREEN);
	}
	else
	{
		pSSTestPpType->SetWindowText("UNLOAD");
		pSSTestPpType->SetBackColor(ORANGE);
	}


// 	(m_nTestPpType == eTestPPLoadType) ? pSSTestPpType->SetBackColor(LLIGHTCYAN) : pSSTestPpType->SetBackColor(ORANGE);
// 	(m_nTestPpType == eTestPPLoadType) ? pSSTestPpType->SetWindowText("LOAD") : pSSTestPpType->SetWindowText("UNLOAD");
}

void CDlgManuMotorTeaching::ClickSspanelTestppType()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	

	if (m_nTestPpType == eTestPPLoadType) {
		m_nTestPpType = eTestPPUnLoadType;
	}
	else {
		m_nTestPpType = eTestPPLoadType;
	}

	DisplayTestPpSSPanel();

	UpdateTeachingData(m_nCurRobotId, FALSE);


	if (m_nTeachType == _eTeachType_Group)
	{
		//Group Position 확인.		
		//Load && Unload Group Position 공유 되는 것만 변경 (총 3개 Position이 m_vGroupPoint에 담김)
		if (m_vGroupPoint.size() == 3) 
		{
			//Y Position만 Load & Unload Hand 그룹으로 되어 있으므로, Hand Type 변경 시, 표기해 줘야 함.
			BOOL bIsCOL2_Y = FALSE;
			for (std::vector<std::pair<int, int>>::iterator iter = m_vGroupPoint.begin(); iter != m_vGroupPoint.end(); iter++)
			{
				if (iter->first == eCOL2_Y) {
					bIsCOL2_Y = TRUE;
					break;
				}
			}

			//나머지 Group Position은 Hand Type변경 시, 표기 필요 없음.
			if (bIsCOL2_Y != TRUE)
				return;


			for (int i = 0; i < m_vGroupPoint.size(); i++) {
				if (m_nTestPpType == eTestPPLoadType) {
					if (i != 0) continue;
				}
				else {
					if (i == 0) continue;
				}

				m_sprLocTable.SetCol(m_vGroupPoint[i].first);
				m_sprLocTable.SetRow(m_vGroupPoint[i].second);
				m_sprLocTable.SetBackColor(COLOR_RED);			
			}
		}
	}
}


void CDlgManuMotorTeaching::ClickSspanelSingleTeaching()
{
	if (!(m_nCurRobotId == tRobotId_TESTPP_1 || m_nCurRobotId == tRobotId_TESTPP_2)) {
		return;
	}

	m_nTeachType = _eTeachType_Single;

	DisplayGroupTeachSSPanel();
}


void CDlgManuMotorTeaching::ClickSspanelGroupTeaching()
{
	if (!(m_nCurRobotId == tRobotId_TESTPP_1 || m_nCurRobotId == tRobotId_TESTPP_2)) {
		return;
	}
	
	m_nTeachType = _eTeachType_Group;

	DisplayGroupTeachSSPanel();
}


void CDlgManuMotorTeaching::DisplayGroupTeachSSPanel()
{
	CDSSPanel *pSSTeachType_Single = (CDSSPanel *)GetDlgItem(IDC_SSPANEL_SINGLE_TEACHING);
	CDSSPanel *pSSTeachType_Group = (CDSSPanel *)GetDlgItem(IDC_SSPANEL_GROUP_TEACHING);

	if (m_nTeachType == _eTeachType_Single)
	{
		pSSTeachType_Single->SetBackColor(YELLOW/*RGB(128, 128, 0)*/);
		pSSTeachType_Single->SetForeColor(BBLACK);		

		pSSTeachType_Group->SetBackColor(DISABLE_BACK_COLOR); 
		pSSTeachType_Group->SetForeColor(DISABLE_FORE_COLOR); 
	}
	else //_eTeachType_Group
	{
		pSSTeachType_Single->SetBackColor(DISABLE_BACK_COLOR);		
		pSSTeachType_Single->SetForeColor(DISABLE_FORE_COLOR);		

		pSSTeachType_Group->SetBackColor(YELLOW/*RGB(128, 128, 0)*/);
		pSSTeachType_Group->SetForeColor(BBLACK);
	}
}


void CDlgManuMotorTeaching::OnBnClickedVisionAutoTeaching()
{
 	BOOL bVATUse = g_DMCont.m_dmVAT.GB(BDM14_VAT_MD_USE);
 	BOOL bGuiHistory = g_DMCont.m_dmVAT.GB(BDM14_VAT_UI_OPEN_HSITORY); // GUI 켜져 있는지 확인.
 
 	if (/*bVATUse == TRUE &&*/ bGuiHistory == FALSE)
 	{
 		//create로 변경 가능.
 		m_pDlgVAT = new CVAT_MainDlg(this);
 		m_pDlgVAT->Create(IDD_VAT_MAINDLG, this);
 		m_pDlgVAT->ShowWindow(SW_SHOW);
 	}
	else {
		if (m_pDlgVAT != NULL) {
			m_pDlgVAT->ShowWindow(SW_SHOW);
		}
	}
}

BOOL CDlgManuMotorTeaching::ChkShowFlatnessOffset(int nAxisNo, int nLoc, int nPos)
{
	

	//[LOC 'Home' Check]
	BOOL bLocIsHome = FALSE;
	if ((nAxisNo == eAXIS_TRAY_PP_1_Z && nLoc == LOC_IDX_TRAY_LOAD_PP_SAFETY) ||
		(nAxisNo == eAXIS_TRAY_PP_2_Z && nLoc == LOC_IDX_TRAY_UNLOAD_PP_SAFETY) ||
		((nAxisNo == eAXIS_TEST_PP_1_Z || nAxisNo == eAXIS_TEST_PP_1_Z2 || nAxisNo == eAXIS_TEST_PP_2_Z || nAxisNo == eAXIS_TEST_PP_2_Z2) && nLoc == LOC_IDX_TEST_PP_HOME)) {
		bLocIsHome = TRUE;
	}

	//[POS 'Safety' Check]
	BOOL bPosIsSafety = FALSE;
	if ((nAxisNo == eAXIS_TRAY_PP_1_Z && nPos == 3) ||
		(nAxisNo == eAXIS_TRAY_PP_2_Z && nPos == 4) ||
		(nAxisNo == eAXIS_TEST_PP_1_Z || nAxisNo == eAXIS_TEST_PP_1_Z2 || nAxisNo == eAXIS_TEST_PP_2_Z || nAxisNo == eAXIS_TEST_PP_2_Z2) && (nPos == 4 || nPos == 11)){
		bPosIsSafety = TRUE;
	}
	
	if (bLocIsHome || bPosIsSafety) {
		return FALSE; //Flatness 필요 없음.
	}
	else {
		return TRUE;
	}

}


//Move 전 Interlock 확인 사항들 작성.
int CDlgManuMotorTeaching::ChkInterlockBeforeMove(int nAxisNo, double dTPos)
{
	int nRet = ERR_NO_ERROR;

	//UD Table Y축 Tray Site Move일 경우, Rotate 위치 확인.	
	if(nAxisNo == eAXIS_LOAD_TABLE_1_Y || nAxisNo == eAXIS_LOAD_TABLE_4_Y)
	{
		//Load Side Teaching Position
		double dTeachPos = g_TaskLoadTbl[eAXIS_LOAD_TABLE_1_Y].GetTeachPos(LOC_IDX_TBL_LD_SIDE, CTaskLoadTable::eAxisY);

		//Target Position이랑 Teaching Position 비교
		if (dTeachPos - 1000.0 <= dTPos && dTPos <= dTeachPos + 1000.0)
		{
			int idx = nAxisNo - eAXIS_LOAD_TABLE_1_Y;
			int IsRotateLoadSide = g_TaskLoadTbl[idx].chk_rotate_direction(CTaskLoadTable::eDir_LoadSite);
			if (IsRotateLoadSide != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Table%d", idx + 1);
				g_Error.AlarmReport(ERR_LOAD_TBL_ROTATE_STATE_TRAY_SITE_FAIL, AlarmData, NULL);
				nRet = -1;
			}
		}
	}

	return nRet;
}


void CDlgManuMotorTeaching::AutoCalculateValue(int nRobotId, long Col, long Row)
{
	if (nRobotId == tRobotId_TESTPP_1 || nRobotId == tRobotId_TESTPP_2)
	{
		//LD Test PP의 LD Table Teaching 값 변경 시, Safety 위치 자동 계산.
		if (Col == eCOL1_X && (eRow5_Loading_Table_1_LD <= Row && Row <= eRow8_Loading_Table_4_LD))
		{
			int nBoardType = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);
			if (nBoardType == eBoardDir_Forward)
				return;

			std::vector<double> vLDTeachPos;
			vLDTeachPos.clear();

			double dTbl_YPitch = (double)g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Pitch_Y) / 1000.0;
			for (int nRow = eRow5_Loading_Table_1_LD; nRow <= eRow8_Loading_Table_4_LD; nRow++)
			{
				CString strVal = m_sprLocTable.GetTextMatrix(nRow, eCOL1_X);

				double dVal = 0.0;
				if (nBoardType == eBoardDir_Backward)
				{
					if (nRobotId == tRobotId_TESTPP_1)	dVal = atof(strVal) - dTbl_YPitch;
					else                                dVal = atof(strVal) + dTbl_YPitch;				
				}
				else
				{
					if (nRobotId == tRobotId_TESTPP_1)	dVal = atof(strVal) + dTbl_YPitch;
					else                                dVal = atof(strVal) - dTbl_YPitch;
				}

				vLDTeachPos.push_back(dVal);			
			}


			//Calculate
			//1mm Margin으로 Safety Position 설정.
			double dSafetyPos = 0.0;
			if (nRobotId == tRobotId_TESTPP_1) {
				dSafetyPos = *max_element(vLDTeachPos.begin(), vLDTeachPos.end()) + 1;
			}
			else {
				dSafetyPos = *min_element(vLDTeachPos.begin(), vLDTeachPos.end()) - 1;
			}

			CString strSafetyPos = _T("");
			strSafetyPos.Format("%.3f", dSafetyPos);
			m_sprLocTable.SetTextMatrix(eRow14_Home, eCOL1_X, (LPCTSTR)strSafetyPos);
		}
	}

// 	CString strVal = m_sprLocTable.GetTextMatrix(Row, Col);
// 	double dTPos = atof(strVal)*1000.0;
// 	m_TeachingData.SetTeachPos(nRobotId, nRow - 1, nCol - 1, dTPos);

}