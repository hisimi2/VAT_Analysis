// SPD_TM_Transfer.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "SPD_TM_Transfer.h"
#include "afxdialogex.h"


// CSPD_TM_Transfer 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSPD_TM_Transfer, CDialog)

CSPD_TM_Transfer::CSPD_TM_Transfer(CWnd* pParent /*=NULL*/)
	: CDialog(CSPD_TM_Transfer::IDD, pParent)
{
	m_nSPD_Trans_X      = g_DMCont.m_dmCTrayXfer.GN(NDM9_SPD_Transfer_X	);
	m_nACC_Trans_X      = g_DMCont.m_dmCTrayXfer.GN(NDM9_ACC_Transfer_X	);
	m_nDCC_Trans_X      = g_DMCont.m_dmCTrayXfer.GN(NDM9_DCC_Transfer_X	);

	m_dTransfer_CylDownWait     = (double)g_DMCont.m_dmCTrayXfer.GN(NDM9_Transfer_CylDownWait) /1000.0;

	m_dStacker_MainWait = ( double )g_DMCont.m_dmStacker.GN( NDM7_STK_MAIN_UPDN_WAITTIME ) / 1000.0;
	m_dStacker_SubWait = ( double )g_DMCont.m_dmStacker.GN( NDM7_STK_SUB_UPDN_WAITTIME ) / 1000.0;
	m_dStacker_ClampWait = ( double )g_DMCont.m_dmStacker.GN( NDM7_STK_CLAMP_WAITTIME ) / 1000.0;
	m_dConveyorUnload_StopWait = (double)g_DMCont.m_dmStacker.GN(NDM7_STK_CONVEYOR_UNLOAD_STOP_WAIT) / 1000.0;
	m_dConveyorload_StopWait = (double)g_DMCont.m_dmStacker.GN(NDM7_STK_CONVEYOR_LOAD_STOP_WAIT) / 1000.0;

	m_nSlowUnloadStackerOption = g_DMCont.m_dmStacker.GN(NDM7_SLOW_UNLOAD_STACKER_OPTION);

}

CSPD_TM_Transfer::~CSPD_TM_Transfer()
{
}

void CSPD_TM_Transfer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_BtnSave);
	DDX_Control(pDX, IDCANCEL, m_BtnCancel);
	DDX_Text(pDX, IDC_EDIT_SPD_TRANS_X		, m_nSPD_Trans_X);
	DDX_Text(pDX, IDC_EDIT_ACC_TRANS_X		, m_nACC_Trans_X);
	DDX_Text( pDX, IDC_EDIT_DCC_TRANS_X, m_nDCC_Trans_X );
	DDX_Text( pDX, IDC_EDIT_TRANSFER_CYL_DOWN_WAIT, m_dTransfer_CylDownWait );

	DDX_Text( pDX, IDC_EDIT_STACKER_CYL_MAIN_WAIT, m_dStacker_MainWait );
	DDX_Text( pDX, IDC_EDIT_STACKER_CYL_SUB_WAIT, m_dStacker_SubWait );
	DDX_Text( pDX, IDC_EDIT_STACKER_CYL_CLAMP_WAIT, m_dStacker_ClampWait );

	DDX_Control(pDX, IDC_EDIT_SPD_TRANS_X		, m_editSPD_Trans_X);
	DDX_Control(pDX, IDC_EDIT_ACC_TRANS_X		, m_editACC_Trans_X);
	DDX_Control(pDX, IDC_EDIT_DCC_TRANS_X		, m_editDCC_Trans_X);
	DDX_Control(pDX, IDC_EDIT_TRANSFER_CYL_DOWN_WAIT		, m_editTransfer_CylDownWait);

	DDX_Control( pDX, IDC_EDIT_STACKER_CYL_MAIN_WAIT, m_editStacker_CylMainUpDownWait );
	DDX_Control( pDX, IDC_EDIT_STACKER_CYL_SUB_WAIT, m_editStacker_CylSubUpDownWait );
	DDX_Control( pDX, IDC_EDIT_STACKER_CYL_CLAMP_WAIT, m_editStacker_CylClampUnclampWait );
	
	DDX_Text( pDX, IDC_EDIT_STACKER_VEYOR_WAIT, m_dConveyorUnload_StopWait);
	DDX_Control(pDX, IDC_EDIT_STACKER_VEYOR_WAIT, m_editConveyorUnload_StopWait);

	DDX_Text(pDX, IDC_EDIT_STACKER_VEYOR_LOAD_WAIT, m_dConveyorload_StopWait);
	DDX_Control(pDX, IDC_EDIT_STACKER_VEYOR_LOAD_WAIT, m_editConveyorload_StopWait);

	DDX_Control(pDX, IDC_RADIO_SLOW_UNLOAD_STACKER_ON, m_rbChkSlowUnStkUse);
	DDX_Control(pDX, IDC_RADIO_SLOW_UNLOAD_STACKER_OFF, m_rbChkSlowUnStkNotUse);
}


BEGIN_MESSAGE_MAP(CSPD_TM_Transfer, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CSPD_TM_Transfer::OnBnClickedButtonSave)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_TRANS_X, &CSPD_TM_Transfer::OnEnSetfocusEditSpdTransX)
	ON_EN_SETFOCUS(IDC_EDIT_ACC_TRANS_X, &CSPD_TM_Transfer::OnEnSetfocusEditAccTransX)
	ON_EN_SETFOCUS(IDC_EDIT_DCC_TRANS_X, &CSPD_TM_Transfer::OnEnSetfocusEditDccTransX)
	ON_EN_SETFOCUS(IDC_EDIT_TRANSFER_CYL_DOWN_WAIT, &CSPD_TM_Transfer::OnEnSetfocusEditTransferCylDownWait)
	ON_EN_SETFOCUS( IDC_EDIT_STACKER_CYL_MAIN_WAIT, &CSPD_TM_Transfer::OnEnSetfocusEditStackerCylMainWait )
	ON_EN_SETFOCUS( IDC_EDIT_STACKER_CYL_SUB_WAIT, &CSPD_TM_Transfer::OnEnSetfocusEditStackerCylSubWait )
	ON_EN_SETFOCUS( IDC_EDIT_STACKER_CYL_CLAMP_WAIT, &CSPD_TM_Transfer::OnEnSetfocusEditStackerCylClampWait )
	ON_EN_SETFOCUS(IDC_EDIT_STACKER_VEYOR_WAIT, &CSPD_TM_Transfer::OnEnSetfocusEditStackerVeyorWait)
	ON_EN_SETFOCUS(IDC_EDIT_STACKER_VEYOR_LOAD_WAIT, &CSPD_TM_Transfer::OnEnSetfocusEditStackerVeyorLoadWait)
	ON_BN_CLICKED(IDC_RADIO_SLOW_UNLOAD_STACKER_ON, &CSPD_TM_Transfer::OnBnClickedRadioSlowUnloadStackerOn)
	ON_BN_CLICKED(IDC_RADIO_SLOW_UNLOAD_STACKER_OFF, &CSPD_TM_Transfer::OnBnClickedRadioSlowUnloadStackerOff)
END_MESSAGE_MAP()


// CSPD_TM_Transfer 메시지 처리기입니다.


BOOL CSPD_TM_Transfer::OnInitDialog()
{
	CDialog::OnInitDialog();

	OnBtnGUI();
	OnEditBoxGUI();

	int nSlowUnloadStackerOption = g_DMCont.m_dmStacker.GN(NDM7_SLOW_UNLOAD_STACKER_OPTION);

	m_rbChkSlowUnStkUse.SetCheck(nSlowUnloadStackerOption); //기본 값 할당.
	m_rbChkSlowUnStkNotUse.SetCheck(!nSlowUnloadStackerOption);

	char szCompany[16] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

	GetDlgItem(IDC_GROUP_SLOW_UNLOAD_STACKER_OPTION)->ShowWindow(FALSE);
	GetDlgItem(IDC_RADIO_SLOW_UNLOAD_STACKER_ON)->ShowWindow(FALSE);
	GetDlgItem(IDC_RADIO_SLOW_UNLOAD_STACKER_OFF)->ShowWindow(FALSE);
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSPD_TM_Transfer::OnBtnGUI()
{
	m_BtnCancel.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnCancel.SetIcon(IDI_BTN_EXIT);
	m_BtnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnSave.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnSave.SetIcon(IDI_BTN_SAVE);
	m_BtnSave.SetAlign(CButtonST::ST_ALIGN_VERT);
}

void CSPD_TM_Transfer::OnEditBoxGUI()
{
	m_editSPD_Trans_X		.SetBackColor(COLOR_WWHITE);
	m_editACC_Trans_X		.SetBackColor(COLOR_WWHITE);
	m_editDCC_Trans_X		.SetBackColor(COLOR_WWHITE);
	m_editTransfer_CylDownWait .SetBackColor(COLOR_WWHITE);

	m_editStacker_CylMainUpDownWait.SetBackColor( COLOR_WWHITE );
	m_editStacker_CylSubUpDownWait.SetBackColor( COLOR_WWHITE );
	m_editStacker_CylClampUnclampWait.SetBackColor( COLOR_WWHITE );
	m_editConveyorUnload_StopWait.SetBackColor(COLOR_WWHITE);
	m_editConveyorload_StopWait.SetBackColor(COLOR_WWHITE);


	m_editSPD_Trans_X		.SetTextSize(30);
	m_editACC_Trans_X		.SetTextSize(30);
	m_editDCC_Trans_X		.SetTextSize(30);
	m_editTransfer_CylDownWait .SetTextSize(30);

	m_editStacker_CylMainUpDownWait.SetTextSize( 30 );
	m_editStacker_CylSubUpDownWait.SetTextSize( 30 );
	m_editStacker_CylClampUnclampWait.SetTextSize( 30 );
	m_editConveyorUnload_StopWait.SetTextSize(30);
	m_editConveyorload_StopWait.SetTextSize(30);
}


void CSPD_TM_Transfer::OnBnClickedButtonSave()
{
	UpdateData(TRUE);

	g_DMCont.m_dmCTrayXfer.SN(NDM9_SPD_Transfer_X		,m_nSPD_Trans_X    );
	g_DMCont.m_dmCTrayXfer.SN(NDM9_ACC_Transfer_X		,m_nACC_Trans_X    );
	g_DMCont.m_dmCTrayXfer.SN(NDM9_DCC_Transfer_X		,m_nDCC_Trans_X    );

	g_DMCont.m_dmCTrayXfer.SN(NDM9_Transfer_CylDownWait		,(int)(m_dTransfer_CylDownWait *1000));

	g_DMCont.m_dmStacker.SN( NDM7_STK_MAIN_UPDN_WAITTIME, ( int )( m_dStacker_MainWait * 1000 ) );
	g_DMCont.m_dmStacker.SN( NDM7_STK_SUB_UPDN_WAITTIME, ( int )( m_dStacker_SubWait * 1000 ) );
	g_DMCont.m_dmStacker.SN( NDM7_STK_CLAMP_WAITTIME, ( int )( m_dStacker_ClampWait * 1000 ) );
	g_DMCont.m_dmStacker.SN( NDM7_STK_CONVEYOR_UNLOAD_STOP_WAIT, (int)(m_dConveyorUnload_StopWait * 1000));
	g_DMCont.m_dmStacker.SN(NDM7_STK_CONVEYOR_LOAD_STOP_WAIT, (int)(m_dConveyorload_StopWait * 1000));
	g_DMCont.m_dmStacker.SN(NDM7_SLOW_UNLOAD_STACKER_OPTION, m_nSlowUnloadStackerOption);

	char szCokName[512] = {0,};
	ST_COK_INFO   stCokData, stBeforeCokData;
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));	
	g_COK_FileMgr.LoadCokInfo(szCokName, stCokData);

	stBeforeCokData = stCokData;

	stCokData.aTransfer.aAxisForm[eAxis_X].nSPD = m_nSPD_Trans_X;
	stCokData.aTransfer.aAxisForm[eAxis_X].nACC = m_nACC_Trans_X;
	stCokData.aTransfer.aAxisForm[eAxis_X].nDCC = m_nDCC_Trans_X;

	stCokData.aTranser_Tm.nCylDownWaitTm = (int)(m_dTransfer_CylDownWait*1000);

	stCokData.nStacker_Main_WaitTm = ( int )( m_dStacker_MainWait * 1000 );
	stCokData.nStacker_Sub_WaitTm = ( int )( m_dStacker_SubWait * 1000 );
	stCokData.nStacker_Clamp_WaitTm = ( int )( m_dStacker_ClampWait * 1000 );
	stCokData.nStacker_Conveyor_StopWaitTm = (int)(m_dConveyorUnload_StopWait * 1000);
	stCokData.nSlowUnloadStackerOption = m_nSlowUnloadStackerOption;

	g_COK_FileMgr.SaveCokInfo(szCokName, &stCokData);

	//GUI Data Change Log
	std::vector<std::pair<CString,std::pair<double,double>>> vData;  // vData.first = stCokData, vData.second.first = BeforeData vData.second.second = AfterData
	
	stBeforeCokData.IsSameTransfer(stCokData, vData);

	if(!(vData.empty()))
	{
		TransferSpeedTiemChangeLog(vData);
	}
}

void CSPD_TM_Transfer::OnEnSetfocusEditSpdTransX()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nSPD_Trans_X, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Transfer X Speed", "100", "5");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_SPD_TRANS_X)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_Transfer::OnEnSetfocusEditAccTransX()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nACC_Trans_X, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Transfer X Accel", "100", "5");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_ACC_TRANS_X)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_Transfer::OnEnSetfocusEditDccTransX()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nDCC_Trans_X, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Transfer X Decel", "100", "5");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_DCC_TRANS_X)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_Transfer::OnEnSetfocusEditTransferCylDownWait()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dTransfer_CylDownWait);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Tranfer Cylinder Down Wait", "20", "0");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_TRANSFER_CYL_DOWN_WAIT)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_Transfer::MakeLog(LPCTSTR fmt, ...)
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


void CSPD_TM_Transfer::TransferSpeedTiemChangeLog(std::vector<std::pair<CString,std::pair<double,double>>> vData)
{
	for(int i = 0 ; i< (int)vData.size(); i++)
	{
		std::pair<CString,std::pair<double,double>> p;

		p = vData[i];

		MakeLog("[MENU = Transfer Speed & Time, DATA NAME = %s ][BEFORE=%.2f, AFTER=%.2f]", p.first, p.second.first, p.second.second);
	}
}



void CSPD_TM_Transfer::OnEnSetfocusEditStackerCylMainWait()
{
	char szVal[ 10 ] = { 0, };
	char szRetVal[ 64 ] = { 0, };
	sprintf_s( szVal, sizeof( szVal ), "%0.2f", m_dStacker_MainWait );

	int nRet = g_LibCommCtrl.GetNumberBox( this, szRetVal, 10, szVal, "Stacker Main Cylinder Wait Time", "5", "0" );

	if( nRet == RETURN_OK ) {
		GetDlgItem( IDC_EDIT_STACKER_CYL_MAIN_WAIT )->SetWindowText( szRetVal );
	}
	else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T( "Range Over" ) );
	}
}


void CSPD_TM_Transfer::OnEnSetfocusEditStackerCylSubWait()
{
	char szVal[ 10 ] = { 0, };
	char szRetVal[ 64 ] = { 0, };
	sprintf_s( szVal, sizeof( szVal ), "%0.2f", m_dStacker_SubWait );

	int nRet = g_LibCommCtrl.GetNumberBox( this, szRetVal, 10, szVal, "Stacker Sub Cylinder Wait", "5", "0" );

	if( nRet == RETURN_OK ) {
		GetDlgItem( IDC_EDIT_STACKER_CYL_SUB_WAIT )->SetWindowText( szRetVal );
	}
	else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T( "Range Over" ) );
	}
}


void CSPD_TM_Transfer::OnEnSetfocusEditStackerCylClampWait()
{
	char szVal[ 10 ] = { 0, };
	char szRetVal[ 64 ] = { 0, };
	sprintf_s( szVal, sizeof( szVal ), "%0.2f", m_dStacker_ClampWait );

	int nRet = g_LibCommCtrl.GetNumberBox( this, szRetVal, 10, szVal, "Stacker Clamp Cylinder Down Wait", "5", "0" );

	if( nRet == RETURN_OK ) {
		GetDlgItem( IDC_EDIT_STACKER_CYL_CLAMP_WAIT )->SetWindowText( szRetVal );
	}
	else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T( "Range Over" ) );
	}
}


void CSPD_TM_Transfer::OnEnSetfocusEditStackerVeyorWait()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dConveyorUnload_StopWait);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Conveyor Stop Wait", "2", "0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_STACKER_VEYOR_WAIT)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}

void CSPD_TM_Transfer::OnEnSetfocusEditStackerVeyorLoadWait()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dConveyorload_StopWait);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Conveyor Stop Wait", "2", "0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_STACKER_VEYOR_LOAD_WAIT)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_Transfer::OnBnClickedRadioSlowUnloadStackerOn()
{
	m_nSlowUnloadStackerOption = DEF_ON;
}


void CSPD_TM_Transfer::OnBnClickedRadioSlowUnloadStackerOff()
{
	m_nSlowUnloadStackerOption = DEF_OFF;
}


