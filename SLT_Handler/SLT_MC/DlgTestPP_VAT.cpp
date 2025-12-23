// DlgTestPP_VAT.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgTestPP_VAT.h"

#include "TpgTestPP_VAT_PkrAlign.h"
#include "TpgTestPP_VAT_SiteAlign.h"
#include "TpgTestPP_VAT_LdTbl.h"
#include "TpgTestPP_VAT_Buffer.h"

#include "DlgManuMotor.h"

//////////////////////////////////////////////////////////////////////////
extern LPCSTR INVALID_FIELD_CHAR;
extern LPCSTR disp_fmt;

//////////////////////////////////////////////////////////////////////////

// CDlgTestPP_VAT 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTestPP_VAT, CDialogEx)

CDlgTestPP_VAT::CDlgTestPP_VAT(int hand_index, CWnd* pParent /* = NULL */)
	: CDialogEx(CDlgTestPP_VAT::IDD, pParent)
{
	m_nHandIndex = hand_index;
	m_tpgOld = NULL;
	ZeroMemory( m_arrTpgs, sizeof(m_arrTpgs) );
	m_nLastSel_Row = m_nLastSel_Col = 0;
	m_dEditSafetyUpZ = 0;
	m_dEditSafetyDnZ = 0;
}

CDlgTestPP_VAT::~CDlgTestPP_VAT()
{
	for( int i=0; i<MAX_TPG_COUNT; i++ )
		SAFE_DELETE( m_arrTpgs[ i ] );
}

void CDlgTestPP_VAT::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_3POINT, m_ctrlTab);
	DDX_Control(pDX, IDC_SPREAD_VAT_CAM_POS, m_gridvatCamPos);
	DDX_Control(pDX, IDC_SPRD_CUR_POS, m_gridCurEnc);
}


BEGIN_MESSAGE_MAP(CDlgTestPP_VAT, CDialogEx)
	ON_WM_TIMER()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_3POINT, &CDlgTestPP_VAT::OnTcnSelchangeTab3point)
	ON_BN_CLICKED(IDCANCEL, &CDlgTestPP_VAT::OnBnClickedBtnClose)
	ON_BN_CLICKED(IDC_MANU_BTN_ENCODER, &CDlgTestPP_VAT::OnBnClickedManuBtnEncoder)
	ON_BN_CLICKED(IDC_MANU_BTN_SERVO, &CDlgTestPP_VAT::OnBnClickedManuBtnServo)
	ON_BN_CLICKED(IDC_BTN_AUTO_FOCUS, &CDlgTestPP_VAT::OnBnClickedBtnAutoFocus)
	ON_BN_CLICKED(IDC_BTN_REF_PKR_FINE_ADJ, &CDlgTestPP_VAT::OnBnClickedBtnRefPkrFineAdj)
	ON_BN_CLICKED(IDC_BTN_APPLY, &CDlgTestPP_VAT::OnBnClickedBtnApply)
	ON_BN_CLICKED(IDC_BTN_Z_SAFETY_UP, &CDlgTestPP_VAT::OnBnClickedBtnZSafetyUp)
	ON_BN_CLICKED(IDC_BTN_Z_SAFETY_DN, &CDlgTestPP_VAT::OnBnClickedBtnZSafetyDn)
	ON_BN_CLICKED(IDC_BTN_RELOAD, &CDlgTestPP_VAT::OnBnClickedBtnReload)
END_MESSAGE_MAP()



BOOL CDlgTestPP_VAT::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	g_VatHW.testpp[m_nHandIndex].Load();
	this->m_stEditCamPos = g_VatHW.testpp[m_nHandIndex].CamPos;
	this->m_stEditCamAdjOfs = g_VatHW.testpp[m_nHandIndex].CamAdjOfs;
	this->m_dEditSafetyUpZ = g_VatHW.testpp[m_nHandIndex].up_safety_z;
	this->m_dEditSafetyDnZ = g_VatHW.testpp[m_nHandIndex].dn_safety_z;


/* unused fields */
	m_gridvatCamPos.SetTextMatrix( 2, 4, INVALID_FIELD_CHAR );
	m_gridvatCamPos.SetTextMatrix( 2, 5, INVALID_FIELD_CHAR );
	m_gridvatCamPos.SetTextMatrix( 2, 6, INVALID_FIELD_CHAR );
	m_gridvatCamPos.SetTextMatrix( 2, 7, INVALID_FIELD_CHAR );
	m_gridvatCamPos.SetTextMatrix( 4, 4, INVALID_FIELD_CHAR );
	m_gridvatCamPos.SetTextMatrix( 4, 5, INVALID_FIELD_CHAR );
	m_gridvatCamPos.SetTextMatrix( 4, 6, INVALID_FIELD_CHAR );
	m_gridvatCamPos.SetTextMatrix( 4, 7, INVALID_FIELD_CHAR );

	init_tab_pages();
	display_cam_position();

	SetTimer( hand_enc_tmr_500ms, 500, NULL );
	return TRUE;
}


void CDlgTestPP_VAT::OnTimer( UINT_PTR nIDEvent )
{
	switch( (timer_id)nIDEvent )
	{
	case general_tmr: 
		/*do nothing*/ 
		break;
	case hand_enc_tmr_500ms: 
		display_hand_encoder(); 
		break;
	case af_check_tmr_500ms: /* af result */
	case wcam_adj_tmr_500ms:
		cmd_compt_check( (timer_id)nIDEvent );
		break;
	default: 
		/*do nothing*/ 
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

void CDlgTestPP_VAT::init_tab_pages()
{
	/*inti tab control*/
	m_ctrlTab.DeleteAllItems();
	m_ctrlTab.InsertItem( TPG_PICKER		, "Picker"		);
	m_ctrlTab.InsertItem( TPG_TEST_SITE		, "Test Site"	);
	m_ctrlTab.InsertItem( TPG_LDTBL			, "LD.Table"	);
	m_ctrlTab.InsertItem( TPG_BUFFER		, "Buffer"		);

	/*picker page - default show */
	m_arrTpgs[ TPG_PICKER ] = 
		new CTpgTestPpVat_PkrAlign( m_nHandIndex, &m_ctrlTab );

	/*test site*/
	m_arrTpgs[ TPG_TEST_SITE ] = 
		new CTpgTestPpVat_SiteAlign(m_nHandIndex, &m_ctrlTab);
	
	m_arrTpgs[ TPG_LDTBL ] = 
		new CTpgTestPpVat_LdTbl( m_nHandIndex, &m_ctrlTab );

	m_arrTpgs[ TPG_BUFFER ] = 
		new CTpgTestPpVat_Buffer( m_nHandIndex, &m_ctrlTab );

	m_ctrlTab.SetCurSel( 0 );
	OnTcnSelchangeTab3point(NULL, NULL);
}

void CDlgTestPP_VAT::display_cam_position()
{
	CPointF *p;
	CString v;

	p = &this->m_stEditCamPos;
	v.Format( disp_fmt, p->x ); m_gridvatCamPos.SetTextMatrix( 1,1, v );
	v.Format( disp_fmt, p->y ); m_gridvatCamPos.SetTextMatrix( 1,2, v );
	v.Format( disp_fmt, p->z ); m_gridvatCamPos.SetTextMatrix( 1,3, v );
	p = &this->m_stEditCamAdjOfs;
	v.Format( disp_fmt, p->x ); m_gridvatCamPos.SetTextMatrix( 2,1, v );
	v.Format( disp_fmt, p->y ); m_gridvatCamPos.SetTextMatrix( 2,2, v );
	v.Format( disp_fmt, p->z ); m_gridvatCamPos.SetTextMatrix( 2,3, v );

	v.Format( disp_fmt, m_dEditSafetyUpZ ); SetDlgItemText( IDC_BTN_Z_SAFETY_UP, v );
	v.Format( disp_fmt, m_dEditSafetyDnZ ); SetDlgItemText( IDC_BTN_Z_SAFETY_DN, v );
}

void CDlgTestPP_VAT::display_hand_encoder()
{
	CString v;
	double dCurEnc;

	OneAxis* pMotorX  = g_TaskTestPP[ m_nHandIndex ].GetAxis(CTaskTestPP::eAxisX);
	OneAxis* pMotorY  = g_TaskTestPP[ m_nHandIndex ].GetAxis(CTaskTestPP::eAxisY);
	OneAxis* pMotorZ  = g_TaskTestPP[ m_nHandIndex ].GetAxis(CTaskTestPP::eAxisZ);

	dCurEnc = pMotorX->GetCurrentPos() / 1000.0;
	v.Format( disp_fmt, dCurEnc ); m_gridCurEnc.SetTextMatrix( 1,1, v );

	dCurEnc = pMotorY->GetCurrentPos() / 1000.0;
	v.Format( disp_fmt, dCurEnc ); m_gridCurEnc.SetTextMatrix( 2,1, v );

	dCurEnc = pMotorZ->GetCurrentPos() / 1000.0;
	v.Format( disp_fmt, dCurEnc ); m_gridCurEnc.SetTextMatrix( 3,1, v );
}


// CDlgCTrayVAT 메시지 처리기입니다.
void CDlgTestPP_VAT::OnTcnSelchangeTab3point(NMHDR *pNMHDR, LRESULT *pResult)
{
	int tab_index = m_ctrlTab.GetCurSel();

	ASSERT(0<=tab_index && tab_index<MAX_TPG_COUNT);

	/*old page hide*/
	if(m_tpgOld)
		m_tpgOld->ShowWindow(SW_HIDE);

	/*page change*/
	m_tpgOld = m_arrTpgs[ tab_index ];

	/*new page show*/
	if(m_tpgOld)
		m_tpgOld->ShowWindow(SW_SHOW);

	if(pResult)
		*pResult = 0;
}

void CDlgTestPP_VAT::OnBnClickedBtnClose()
{
	EndDialog(0);
}


BEGIN_EVENTSINK_MAP(CDlgTestPP_VAT, CDialogEx)
	ON_EVENT(CDlgTestPP_VAT, IDC_SPREAD_VAT_CAM_POS, 5, CDlgTestPP_VAT::ClickSpreadVatCamPos, VTS_I4 VTS_I4)
	ON_EVENT(CDlgTestPP_VAT, IDC_SPREAD_VAT_CAM_POS, 11, CDlgTestPP_VAT::DblClickSpreadVatCamPos, VTS_I4 VTS_I4)
END_EVENTSINK_MAP()


/*spread click*/
void CDlgTestPP_VAT::ClickSpreadVatCamPos(long Col, long Row)
{
	m_gridvatCamPos.SetRow(m_nLastSel_Row);
	m_gridvatCamPos.SetCol(m_nLastSel_Col);
	m_gridvatCamPos.SetBackColor(COLOR_WWHITE);

	m_gridvatCamPos.SetRow(Row);
	m_gridvatCamPos.SetCol(Col);
	m_gridvatCamPos.SetBackColor(COLOR_RED);

	m_nLastSel_Row=Row;
	m_nLastSel_Col=Col;

	UpdateData(FALSE);
}

void CDlgTestPP_VAT::DblClickSpreadVatCamPos(long Col, long Row)
{
	if( Col == 0 || Row == 0 ){
		return;
	}
	m_nLastSel_Row=Row;
	m_nLastSel_Col=Col;

	CString strValue_Old = m_gridvatCamPos.GetTextMatrix( m_nLastSel_Row, m_nLastSel_Col );
	if( strValue_Old == INVALID_FIELD_CHAR)
		return;

	CString strValue_New;


	CString strMin; 
	CString strMax; 
	if( m_nLastSel_Col==6 || m_nLastSel_Col==7){
		strMin = "0";
		strMax = "30";
	}else{
		OneAxis* pMotor  = g_TaskTestPP[m_nHandIndex].GetAxis( m_nLastSel_Col-1 );
		strMin.Format( disp_fmt, pMotor->GetSwLimitNega()/1000.0 );
		strMax.Format( disp_fmt, pMotor->GetSwLimitPosi()/1000.0 );
	}

	int nRet = g_LibCommCtrl.GetNumberBox(
		this, 
		strValue_New.GetBuffer(256), 255, 
		(char*)strValue_Old.GetString(), 
		"Input"/*szTitle*/, 
		(char*)strMax.GetString(), (char*)strMin.GetString());
	strValue_New.ReleaseBuffer();

	if( nRet == RETURN_OK ){
		MakeLog("CDlgTestPP_VAT::Web Cam Techaing (row=%d,col=%d)\t %s->%s",m_nLastSel_Row,m_nLastSel_Col,strValue_Old,strValue_New);
		m_gridvatCamPos.SetTextMatrix(m_nLastSel_Row, m_nLastSel_Col, strValue_New);
		edit_cam_pos( m_nLastSel_Row, m_nLastSel_Col, strValue_New );
		display_cam_position();
	}
}

void CDlgTestPP_VAT::edit_cam_pos( int r, int c, const char* v )
{
	CPointF* ptr = NULL;

	double dv = _ttof( v );

	switch( r ){
	case 1:
		ptr = &this->m_stEditCamPos;
		ASSERT( 1<=c && c<=3 );
		*((&ptr->x)+c-1) = dv; break; /*x y z*/
		break;;
	case 2:
		ASSERT( 1<=c && c<=3 );
		ptr = &this->m_stEditCamAdjOfs;
		*((&ptr->x)+c-1) = dv; /*x y z*/
		break;
	}
}


BOOL CDlgTestPP_VAT::is_modify()
{
	int cmp = 0;
	cmp += memcmp(&this->m_stEditCamPos, &g_VatHW.testpp[m_nHandIndex].CamPos, sizeof(CPointF));
	cmp += memcmp(&this->m_stEditCamAdjOfs, &g_VatHW.testpp[m_nHandIndex].CamAdjOfs, sizeof(CPointF));
	cmp += memcmp(&this->m_dEditSafetyUpZ, &g_VatHW.testpp[m_nHandIndex].up_safety_z, sizeof(double));
	cmp += memcmp(&this->m_dEditSafetyDnZ, &g_VatHW.testpp[m_nHandIndex].dn_safety_z, sizeof(double));
	return (cmp != 0) ? TRUE : FALSE;
}


void CDlgTestPP_VAT::start_cmd_exec_check( timer_id cmd_tmr )
{
	/* final data prossing @OnTimer():af_check_tmr_500ms */
	this->EnableWindow( FALSE );

	switch( cmd_tmr )
	{
	case af_check_tmr_500ms: SetTimer( cmd_tmr, 500, NULL ); break;
	case wcam_adj_tmr_500ms: SetTimer( cmd_tmr, 500, NULL ); break;
	default: ASSERT("invalid command timer"==0); break;
	}
}

void CDlgTestPP_VAT::cmd_compt_check( timer_id cmd_tmr )
{
	int traypp_cmd_status = g_TaskTestPP[m_nHandIndex].GetSeqCmdStatus( eEqpCtrlMode_Manual );
	if( traypp_cmd_status == CBaseSeqCmd::eSeqCmdStatus_Ready )
	{
		switch( cmd_tmr )
		{
		case af_check_tmr_500ms: /* af result */
		case wcam_adj_tmr_500ms: /* wcam auto teaching result */
			/* common operation */
			this->m_stEditCamPos = g_VatHW.testpp[m_nHandIndex].CamPos;
			this->m_stEditCamAdjOfs = g_VatHW.testpp[m_nHandIndex].CamAdjOfs;
			this->m_dEditSafetyUpZ = g_VatHW.testpp[m_nHandIndex].up_safety_z;
			this->m_dEditSafetyDnZ = g_VatHW.testpp[m_nHandIndex].dn_safety_z;
			display_cam_position();
			break;
		default: ASSERT("invalid command timer"==0); break;
		}

		this->EnableWindow( TRUE );
		KillTimer( cmd_tmr );
	}
}


void CDlgTestPP_VAT::OnBnClickedManuBtnEncoder()
{
	if( m_nLastSel_Col == 0 || m_nLastSel_Row == 0 ){
		return;
	}

	CString strValue_Old = m_gridvatCamPos.GetTextMatrix( m_nLastSel_Row, m_nLastSel_Col );
	if( strValue_Old == INVALID_FIELD_CHAR)
		return;

	double dCurPos = 0.0;
	double dAbsPos = 0.0; 
	int nRet = AfxMessageBox("Do you want to read encoder?", MB_YESNO);

	if( nRet == IDYES )
	{
		CString v;

		OneAxis* pMotor  = g_TaskTestPP[ m_nHandIndex ].GetAxis( m_nLastSel_Col-1 );

		dCurPos = pMotor->GetCurrentPos() / 1000.0;
		MakeLog("CDlgTestPP_VAT::Web Cam Techaing (row=%d,col=%d)\t %s->%f",m_nLastSel_Row,m_nLastSel_Col,m_gridvatCamPos.GetTextMatrix(m_nLastSel_Row,m_nLastSel_Col),dCurPos);
		switch( m_nLastSel_Row ){
			/*absolute coordinates*/
		case 1: case 3:
			v.Format( disp_fmt, dCurPos );
			m_gridvatCamPos.SetTextMatrix(m_nLastSel_Row, m_nLastSel_Col, v);
			edit_cam_pos( m_nLastSel_Row, m_nLastSel_Col, v );
			break;
			/*relative offset*/
		case 2: case 4: 
			dAbsPos = _ttof( m_gridvatCamPos.GetTextMatrix( m_nLastSel_Row-1, m_nLastSel_Col ) );
			v.Format( disp_fmt, (dCurPos - dAbsPos) );
			m_gridvatCamPos.SetTextMatrix(m_nLastSel_Row, m_nLastSel_Col, v);
			edit_cam_pos( m_nLastSel_Row, m_nLastSel_Col, v );
			break;
		}
	}
}


void CDlgTestPP_VAT::OnBnClickedManuBtnServo()
{
	CDlgManuMotor dlg;
	dlg.DoModal();
}


void CDlgTestPP_VAT::OnBnClickedBtnAutoFocus()
{
	double cmd_af_range;

	{/* user edit apply */
	if( is_modify() ){
		OnBnClickedBtnApply();
			if( is_modify() ) return;
		}	
	}

	int nRet = AfxMessageBox("Do you want to Web Cam Auto Focus?", MB_YESNO);
	if( nRet != IDYES ) return;

	{/* input scan range */
		CString v;
		nRet = g_LibCommCtrl.GetNumberBox(
			this, 
			v.GetBuffer(256), 255, 
			"2.5", "Input AF ±Range (mm)", "5.0", "0.1");
		v.ReleaseBuffer();

		if( nRet != RETURN_OK ) return;
		cmd_af_range = _ttof( v )*1000;
	}

	{/* create command */
		CTestPPCmd_VatAF* pSeqCmd = new CTestPPCmd_VatAF(m_nHandIndex);
		pSeqCmd->m_dCmd_ScanRangeUm = cmd_af_range;
		//pSeqCmd->m_CmdParam_PassRangeUm = ??;
		//pSeqCmd->m_CmdParam_ScanZStepUm = ??;

		int nErr = g_TaskSystemCtrl.SendCommand( g_TaskTestPP[m_nHandIndex], pSeqCmd, eEqpCtrlMode_Manual );
		if( nErr != ERR_NO_ERROR ){
			CParamList AlarmData;
			g_Error.AlarmReport( nErr, AlarmData, NULL );
		}else{
			MakeLog( "CDlgTestPP_VAT::Web Cam Auto Focus");

			/* command execute status check @OnTimer():af_check_tmr_500ms */
			start_cmd_exec_check( af_check_tmr_500ms );
		}
	}
}


void CDlgTestPP_VAT::OnBnClickedBtnRefPkrFineAdj()
{
	double cmd_err_range;

	{/* user edit apply */
		if( is_modify() ){
			OnBnClickedBtnApply();
			if( is_modify() ) return;
		}	
	}

	int nRet = AfxMessageBox("Do you want to Web Cam Auto position adjust?", MB_YESNO);
	if( nRet != IDYES ) return;

	{/* input scan range */
		CString v;
		nRet = g_LibCommCtrl.GetNumberBox( this, v.GetBuffer(256), 255, 
			"0.01", "Input Adjust Pass ±Range (mm)", "1.0", "0.001" );
		v.ReleaseBuffer();

		if( nRet != RETURN_OK ) return;
		cmd_err_range = _ttof( v )*1000;
	}

	{/* create command */
		CTestPPCmd_VatWCamTch* pSeqCmd = new CTestPPCmd_VatWCamTch(m_nHandIndex);
		pSeqCmd->m_dCmd_PassRangeUm = cmd_err_range;

		int nErr = g_TaskSystemCtrl.SendCommand( g_TaskTestPP[m_nHandIndex], pSeqCmd, eEqpCtrlMode_Manual );
		if( nErr != ERR_NO_ERROR ){
			CParamList AlarmData;
			g_Error.AlarmReport( nErr, AlarmData, NULL );
		}else{
			MakeLog( "CDlgTestPP_VAT::Web Cam Auto position adjust");

			/* command execute status check @OnTimer():wcam_adj_tmr_500ms */
			start_cmd_exec_check( wcam_adj_tmr_500ms );
		}
	}
}


void CDlgTestPP_VAT::OnBnClickedBtnReload()
{
	int nRet = AfxMessageBox("Do you want to RELOAD?", MB_YESNO);

	if( nRet == IDYES )
	{
		this->m_stEditCamPos = g_VatHW.testpp[m_nHandIndex].CamPos;
		this->m_stEditCamAdjOfs = g_VatHW.testpp[m_nHandIndex].CamAdjOfs;
		this->m_dEditSafetyUpZ = g_VatHW.testpp[m_nHandIndex].up_safety_z;
		this->m_dEditSafetyDnZ = g_VatHW.testpp[m_nHandIndex].dn_safety_z;

		display_cam_position();
		//m_tpgOld->reload
		MakeLog( "CDlgTestPP_VAT::Web Cam position reloaded");
	}
}

void CDlgTestPP_VAT::OnBnClickedBtnApply()
{
	int nRet = AfxMessageBox("Do you want to APPLY?", MB_YESNO);

	if( nRet == IDYES )
	{
		g_VatHW.testpp[m_nHandIndex].CamPos		 = this->m_stEditCamPos   ;
		g_VatHW.testpp[m_nHandIndex].CamAdjOfs	 = this->m_stEditCamAdjOfs;
		g_VatHW.testpp[m_nHandIndex].up_safety_z = this->m_dEditSafetyUpZ ;
		g_VatHW.testpp[m_nHandIndex].dn_safety_z = this->m_dEditSafetyDnZ ;
		g_VatHW.testpp[m_nHandIndex].Save();

		MakeLog( "CDlgTestPP_VAT::Web Cam position applyed");
	}
}


void CDlgTestPP_VAT::MakeLog(LPCTSTR fmt, ...)
{
	CString lg;
	char* tmpMsg = lg.GetBuffer( 1024 );
	if(fmt)
	{
		va_list argList;
		va_start(argList, fmt);
		_vstprintf(tmpMsg, fmt, argList);
		va_end(argList);
	}
	lg.ReleaseBuffer();
	TWLOG.MakeLog(static_cast<int>(eLogID_SYSTEM), Debug, "", __LINE__, NULL, lg.GetString());
}


void CDlgTestPP_VAT::OnBnClickedBtnZSafetyUp()
{
	CString o,n;
	CString min, max;
	OneAxis *pMotor = g_TaskTestPP[m_nHandIndex].GetAxis( CTaskTestPP::eAxisZ );
	
	min.Format(disp_fmt, pMotor->GetSwLimitNega());
	max.Format(disp_fmt, pMotor->GetSwLimitPosi());
	
	GetDlgItemText( IDC_BTN_Z_SAFETY_UP, o.GetBuffer(255), 255 ); o.ReleaseBuffer();

	int nRet = g_LibCommCtrl.GetNumberBox( this, 
		n.GetBuffer(256), 255, (LPSTR)o.GetString(), 
		"Input Upper side Z safety position (mm)", 
		(LPSTR)max.GetString(), (LPSTR)min.GetString() );
	n.ReleaseBuffer();

	if( nRet != RETURN_OK ) return;

	m_dEditSafetyUpZ = _ttof( n );
	SetDlgItemText( IDC_BTN_Z_SAFETY_UP, n );

	MakeLog("CDlgTestPP_VAT(%d)::Z Safety (upper side) Techaing\t %s->%s", m_nHandIndex, o,n );
}


void CDlgTestPP_VAT::OnBnClickedBtnZSafetyDn()
{
	CString o,n;
	CString min, max;
	OneAxis *pMotor = g_TaskTestPP[m_nHandIndex].GetAxis( CTaskTestPP::eAxisZ );

	min.Format(disp_fmt, pMotor->GetSwLimitNega());
	max.Format(disp_fmt, pMotor->GetSwLimitPosi());

	GetDlgItemText( IDC_BTN_Z_SAFETY_DN, o.GetBuffer(255), 255 ); o.ReleaseBuffer();

	int nRet = g_LibCommCtrl.GetNumberBox( this, 
		n.GetBuffer(256), 255, (LPSTR)o.GetString(), 
		"Input Lower side Z safety position (mm)", 
		(LPSTR)max.GetString(), (LPSTR)min.GetString() );
	n.ReleaseBuffer();

	if( nRet != RETURN_OK ) return;

	m_dEditSafetyDnZ = _ttof( n );
	SetDlgItemText( IDC_BTN_Z_SAFETY_DN, n );

	MakeLog("CDlgTestPP_VAT(%d)::Z Safety (lower side) Techaing\t %s->%s", m_nHandIndex, o,n );
}

