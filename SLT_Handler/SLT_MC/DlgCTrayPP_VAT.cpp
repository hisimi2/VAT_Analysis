// DlgCTrayVAT.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgCTrayPP_VAT.h"

#include "TpgCTrayPP_VAT_Pkr.h"
#include "TpgCTrayPP_VAT_CTray.h"
#include "TpgCTrayPP_VAT_LdTbl.h"
#include "TpgCTrayPP_VAT_Buffer.h"

#include "DlgManuMotor.h"
#include "DlgSelectItem.h"

//////////////////////////////////////////////////////////////////////////
const char* INVALID_FIELD_CHAR = "*";
LPCSTR disp_fmt = "%.3f";

//////////////////////////////////////////////////////////////////////////

// CDlgCTrayVAT 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgCTrayVAT, CDialogEx)

CDlgCTrayVAT::CDlgCTrayVAT(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCTrayVAT::IDD, pParent)
{
	m_tpgOld = NULL;
	ZeroMemory( m_arrTpgAligns, sizeof(m_arrTpgAligns) );
	m_nLastSel_Row = m_nLastSel_Col = 0;
}

CDlgCTrayVAT::~CDlgCTrayVAT()
{
	for( int i=0; i<MAX_TPG_COUNT; i++ )
		SAFE_DELETE( m_arrTpgAligns[ i ] );
}

void CDlgCTrayVAT::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_3POINT, m_tabTeachingObj);
	DDX_Control(pDX, IDC_SPREAD_VAT_CAM_POS, m_gridvatCamPos);
	DDX_Control(pDX, IDC_SPRD_CUR_POS, m_gridCurEnc);
}


BEGIN_MESSAGE_MAP(CDlgCTrayVAT, CDialogEx)
	ON_WM_TIMER()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_3POINT, &CDlgCTrayVAT::OnTcnSelchangeTab3point)
	ON_BN_CLICKED(IDCANCEL, &CDlgCTrayVAT::OnBnClickedBtnClose)
	ON_BN_CLICKED(IDC_MANU_BTN_ENCODER, &CDlgCTrayVAT::OnBnClickedManuBtnEncoder)
	ON_BN_CLICKED(IDC_MANU_BTN_SERVO, &CDlgCTrayVAT::OnBnClickedManuBtnServo)
	ON_BN_CLICKED(IDC_BTN_AUTO_FOCUS, &CDlgCTrayVAT::OnBnClickedBtnAutoFocus)
	ON_BN_CLICKED(IDC_BTN_REF_PKR_FINE_ADJ, &CDlgCTrayVAT::OnBnClickedBtnRefPkrFineAdj)
	ON_BN_CLICKED(IDC_BTN_APPLY, &CDlgCTrayVAT::OnBnClickedBtnApply)
END_MESSAGE_MAP()



BOOL CDlgCTrayVAT::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	g_VatHW.traypp.Load();
	m_stEditWideCam = g_VatHW.traypp.m_WideCamPos;
	m_stEditNarrowCam = g_VatHW.traypp.m_NarwCamPos;

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


void CDlgCTrayVAT::OnTimer( UINT_PTR nIDEvent )
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

void CDlgCTrayVAT::init_tab_pages()
{
	/*inti tab control*/
	m_tabTeachingObj.DeleteAllItems();
	m_tabTeachingObj.InsertItem( TPG_PICKER, "Picker"		);
	m_tabTeachingObj.InsertItem( TPG_CTRAY1, "Tray1"		);
	m_tabTeachingObj.InsertItem( TPG_CTRAY2, "Tray2"		);
	m_tabTeachingObj.InsertItem( TPG_CTRAY3, "Tray3"		);
	m_tabTeachingObj.InsertItem( TPG_CTRAY4, "Tray4"		);
	m_tabTeachingObj.InsertItem( TPG_CTRAY5, "Tray5"		);
	m_tabTeachingObj.InsertItem( TPG_CTRAY6, "Tray6"		);
	m_tabTeachingObj.InsertItem( TPG_LDTBL1, "LD.Table1"	);
	m_tabTeachingObj.InsertItem( TPG_LDTBL2, "LD.Table2"	);
	m_tabTeachingObj.InsertItem( TPG_LDTBL3, "LD.Table3"	);
	m_tabTeachingObj.InsertItem( TPG_LDTBL4, "LD.Table4"	);
	m_tabTeachingObj.InsertItem( TPG_BUFFER, "Buffer"		);

	/*picker page - default show */
	m_arrTpgAligns[ TPG_PICKER ] = 
		new CTpgTrayPpVat_Pkr( &m_tabTeachingObj );

	/*c-tray */
	for( int tab_index=TPG_CTRAY1; tab_index<=TPG_CTRAY6; tab_index++ ){
		m_arrTpgAligns[ tab_index ] = 
			new CTpgTrayPpVat_CTray( (tab_index-TPG_CTRAY1), &m_tabTeachingObj);
	}

	/*load table*/
	for( int tab_index=TPG_LDTBL1; tab_index<=TPG_LDTBL4; tab_index++){
		m_arrTpgAligns[ tab_index ] = 
			new CTpgTraytPpVat_LdTbl( (tab_index-TPG_LDTBL1), &m_tabTeachingObj);
	}

	/*buffer*/
	m_arrTpgAligns[ TPG_BUFFER ] = 
		new CTpgTrayPpVat_Buffer( &m_tabTeachingObj );

	m_tabTeachingObj.SetCurSel( 0 );
	OnTcnSelchangeTab3point(NULL, NULL);
}

void CDlgCTrayVAT::display_cam_position()
{
	CPointF *p1,*p2,*p3;
	CString v;

	p1 = &this->m_stEditWideCam.CamPos;
	p2 = &this->m_stEditWideCam.PitchEnc;
	p3 = &this->m_stEditWideCam.Pitch;
	v.Format( disp_fmt, p1->x ); m_gridvatCamPos.SetTextMatrix( 1,1, v );
	v.Format( disp_fmt, p1->y ); m_gridvatCamPos.SetTextMatrix( 1,2, v );
	v.Format( disp_fmt, p1->z ); m_gridvatCamPos.SetTextMatrix( 1,3, v );
	v.Format( disp_fmt, p2->x ); m_gridvatCamPos.SetTextMatrix( 1,4, v );
	v.Format( disp_fmt, p2->y ); m_gridvatCamPos.SetTextMatrix( 1,5, v );
	v.Format( disp_fmt, p3->x ); m_gridvatCamPos.SetTextMatrix( 1,6, v );
	v.Format( disp_fmt, p3->y ); m_gridvatCamPos.SetTextMatrix( 1,7, v );
	p1 = &this->m_stEditWideCam.CamAdjOfs;
	v.Format( disp_fmt, p1->x ); m_gridvatCamPos.SetTextMatrix( 2,1, v );
	v.Format( disp_fmt, p1->y ); m_gridvatCamPos.SetTextMatrix( 2,2, v );
	v.Format( disp_fmt, p1->z ); m_gridvatCamPos.SetTextMatrix( 2,3, v );
	p1 = &this->m_stEditNarrowCam.CamPos;
	p2 = &this->m_stEditNarrowCam.PitchEnc;
	p3 = &this->m_stEditNarrowCam.Pitch;
	v.Format( disp_fmt, p1->x ); m_gridvatCamPos.SetTextMatrix( 3,1, v );
	v.Format( disp_fmt, p1->y ); m_gridvatCamPos.SetTextMatrix( 3,2, v );
	v.Format( disp_fmt, p1->z ); m_gridvatCamPos.SetTextMatrix( 3,3, v );
	v.Format( disp_fmt, p2->x ); m_gridvatCamPos.SetTextMatrix( 3,4, v );
	v.Format( disp_fmt, p2->y ); m_gridvatCamPos.SetTextMatrix( 3,5, v );
	v.Format( disp_fmt, p3->x ); m_gridvatCamPos.SetTextMatrix( 3,6, v );
	v.Format( disp_fmt, p3->y ); m_gridvatCamPos.SetTextMatrix( 3,7, v );
	p1 = &this->m_stEditNarrowCam.CamAdjOfs;
	v.Format( disp_fmt, p1->x ); m_gridvatCamPos.SetTextMatrix( 4,1, v );
	v.Format( disp_fmt, p1->y ); m_gridvatCamPos.SetTextMatrix( 4,2, v );
	v.Format( disp_fmt, p1->z ); m_gridvatCamPos.SetTextMatrix( 4,3, v );
}

void CDlgCTrayVAT::display_hand_encoder()
{
	CString v;
	double dCurEnc;
	
	OneAxis* pMotorX  = g_TaskTrayPP.GetAxis(CTaskTrayPP::eAxisX);
	OneAxis* pMotorY  = g_TaskTrayPP.GetAxis(CTaskTrayPP::eAxisY);
	OneAxis* pMotorZ  = g_TaskTrayPP.GetAxis(CTaskTrayPP::eAxisZ);
	OneAxis* pMotorPX = g_TaskTrayPP.GetAxis(CTaskTrayPP::eAxisPitchX);
	OneAxis* pMotorPY = g_TaskTrayPP.GetAxis(CTaskTrayPP::eAxisPitchY);

	dCurEnc = pMotorX->GetCurrentPos() / 1000.0;
	v.Format( disp_fmt, dCurEnc ); m_gridCurEnc.SetTextMatrix( 1,1, v );

	dCurEnc = pMotorY->GetCurrentPos() / 1000.0;
	v.Format( disp_fmt, dCurEnc ); m_gridCurEnc.SetTextMatrix( 2,1, v );
	
	dCurEnc = pMotorZ->GetCurrentPos() / 1000.0;
	v.Format( disp_fmt, dCurEnc ); m_gridCurEnc.SetTextMatrix( 3,1, v );
	
	dCurEnc = pMotorPX->GetCurrentPos() / 1000.0;
	v.Format( disp_fmt, dCurEnc ); m_gridCurEnc.SetTextMatrix( 4,1, v );
	
	dCurEnc = pMotorPY->GetCurrentPos() / 1000.0;
	v.Format( disp_fmt, dCurEnc ); m_gridCurEnc.SetTextMatrix( 5,1, v );
}


// CDlgCTrayVAT 메시지 처리기입니다.
void CDlgCTrayVAT::OnTcnSelchangeTab3point(NMHDR *pNMHDR, LRESULT *pResult)
{
	int tab_index = m_tabTeachingObj.GetCurSel();

	ASSERT(0<=tab_index && tab_index<MAX_TPG_COUNT);

	/*old page hide*/
	if(m_tpgOld)
		m_tpgOld->ShowWindow(SW_HIDE);

	/*page change*/
	m_tpgOld = m_arrTpgAligns[ tab_index ];

	/*new page show*/
	if(m_tpgOld)
		m_tpgOld->ShowWindow(SW_SHOW);

	if(pResult)
		*pResult = 0;
}

void CDlgCTrayVAT::OnBnClickedBtnClose()
{
	EndDialog(0);
}


BEGIN_EVENTSINK_MAP(CDlgCTrayVAT, CDialogEx)
	ON_EVENT(CDlgCTrayVAT, IDC_SPREAD_VAT_CAM_POS, 5, CDlgCTrayVAT::ClickSpreadVatCamPos, VTS_I4 VTS_I4)
	ON_EVENT(CDlgCTrayVAT, IDC_SPREAD_VAT_CAM_POS, 11, CDlgCTrayVAT::DblClickSpreadVatCamPos, VTS_I4 VTS_I4)
END_EVENTSINK_MAP()


/*spread click*/
void CDlgCTrayVAT::ClickSpreadVatCamPos(long Col, long Row)
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

void CDlgCTrayVAT::DblClickSpreadVatCamPos(long Col, long Row)
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
		OneAxis* pMotor  = g_TaskTrayPP.GetAxis( m_nLastSel_Col-1 );
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
		MakeLog("CDlgCTrayVAT::Web Cam Techaing (row=%d,col=%d)\t %s->%s",m_nLastSel_Row,m_nLastSel_Col,strValue_Old,strValue_New);
		m_gridvatCamPos.SetTextMatrix(m_nLastSel_Row, m_nLastSel_Col, strValue_New);
		edit_cam_pos( m_nLastSel_Row, m_nLastSel_Col, strValue_New );
		display_cam_position();
	}
}

void CDlgCTrayVAT::edit_cam_pos( int r, int c, const char* v )
{
	CVatHwDesnDim::TrayPP::CamPitchPos* pitch_ptr = NULL;
	
	double dv = _ttof( v );

	switch( r ){
	case 1:
		pitch_ptr = &this->m_stEditWideCam;
		switch( c ){
		case 1: case 2: case 3: 
			*((&pitch_ptr->CamPos.x)+c-1) = dv; break; /*x y z*/
		case 4: case 5: 
			*((&pitch_ptr->PitchEnc.x)+c-4) = dv; break; /*pxe pye*/
		case 6: case 7: 
			*((&pitch_ptr->Pitch.x)+c-6) = dv; break; /*px py*/
		}
		break;;
	case 2:
		ASSERT( 1<=c && c<=3 );
		pitch_ptr = &this->m_stEditWideCam;
		*((&pitch_ptr->CamAdjOfs.x)+c-1) = dv; /*x y z*/
		break;
	case 3:
		pitch_ptr = &this->m_stEditNarrowCam;
		switch( c ){
		case 1: case 2: case 3: 
			*((&pitch_ptr->CamPos.x)+c-1) = dv; break; /*x y z*/
		case 4: case 5: 
			*((&pitch_ptr->PitchEnc.x)+c-4) = dv; break; /*pxe pye*/
		case 6: case 7: 
			*((&pitch_ptr->Pitch.x)+c-6) = dv; break; /*px py*/
		}
		break;
	case 4:
		ASSERT( 1<=c && c<=3 );
		pitch_ptr = &this->m_stEditNarrowCam;
		*((&pitch_ptr->CamAdjOfs.x)+c-1) = dv; /*x y z*/
		break;
	}
}

void CDlgCTrayVAT::start_cmd_exec_check( timer_id cmd_tmr )
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

void CDlgCTrayVAT::cmd_compt_check( timer_id cmd_tmr )
{
	int traypp_cmd_status = g_TaskTrayPP.GetSeqCmdStatus( eEqpCtrlMode_Manual );
	if( traypp_cmd_status == CBaseSeqCmd::eSeqCmdStatus_Ready )
	{
		switch( cmd_tmr )
		{
		case af_check_tmr_500ms: /* af result */
		case wcam_adj_tmr_500ms: /* wcam auto teaching result */
			/* common operation */
			m_stEditWideCam = g_VatHW.traypp.m_WideCamPos;
			m_stEditNarrowCam = g_VatHW.traypp.m_NarwCamPos;
			display_cam_position();
			break;
		default: ASSERT("invalid command timer"==0); break;
		}

		this->EnableWindow( TRUE );
		KillTimer( cmd_tmr );
	}
}


BOOL CDlgCTrayVAT::is_modify()
{
	int cmp = 0;
	cmp += memcmp(&this->m_stEditWideCam, &g_VatHW.traypp, sizeof(this->m_stEditWideCam));
	cmp += memcmp(&this->m_stEditNarrowCam, &g_VatHW.traypp, sizeof(this->m_stEditNarrowCam));
	return (cmp != 0) ? TRUE : FALSE;
}

void CDlgCTrayVAT::OnBnClickedManuBtnEncoder()
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

		OneAxis* pMotor  = g_TaskTrayPP.GetAxis( m_nLastSel_Col-1 );

		dCurPos = pMotor->GetCurrentPos() / 1000.0;
		MakeLog("CDlgCTrayVAT::Web Cam Techaing (row=%d,col=%d)\t %s->%f",m_nLastSel_Row,m_nLastSel_Col,m_gridvatCamPos.GetTextMatrix(m_nLastSel_Row,m_nLastSel_Col),dCurPos);
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


void CDlgCTrayVAT::OnBnClickedManuBtnServo()
{
	CDlgManuMotor dlg;
	dlg.DoModal();
}


void CDlgCTrayVAT::OnBnClickedBtnAutoFocus()
{
	double cmd_af_range;
	CTrayPPCmd_VatAF::VatPitchMode cmd_widnarw;
	CString strWidNarw;

	{/* user edit apply */
		if( is_modify() ){
			OnBnClickedBtnApply();
			if( is_modify() ) return;
		}	
	}

	int nRet = AfxMessageBox("Do you want to Web Cam Auto Focus?", MB_YESNO);
	if( nRet != IDYES ) return;

	{/* wide or narrow selection */
		CDlgSelectItem::ItemArray ar;
		ar.push_back( CString("Wide")	);
		ar.push_back( CString("Narrow")	);

		CDlgSelectItem dlg(this, ar);
		if( dlg.DoModal() != IDOK ) return;

		nRet = AfxMessageBox( dlg.GetValue() + " Auto focus ?", MB_YESNO|MB_ICONWARNING  );
		if( nRet != IDYES ) return;

		strWidNarw = dlg.GetValue();
		switch( dlg.GetSelIndex() ){
		case 0: cmd_widnarw = CTrayPPCmd_VatAF::WidePitch; break;
		case 1: cmd_widnarw = CTrayPPCmd_VatAF::NarrowPitch; break;
		default:
			ASSERT("dlg.GetSelIndex() invalid"==0);
			return;
		}
	}
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
		CTrayPPCmd_VatAF* pSeqCmd = new CTrayPPCmd_VatAF;
		pSeqCmd->m_eCmd_PitchMode = cmd_widnarw;
		pSeqCmd->m_dCmd_ScanRangeUm = cmd_af_range;
		//pSeqCmd->m_CmdParam_PassRangeUm = ??;
		//pSeqCmd->m_CmdParam_ScanZStepUm = ??;

		int nErr = g_TaskSystemCtrl.SendCommand( g_TaskTrayPP, pSeqCmd, eEqpCtrlMode_Manual );
		if( nErr != ERR_NO_ERROR ){
			CParamList AlarmData;
			g_Error.AlarmReport( nErr, AlarmData, NULL );
		}else{
			MakeLog( "CDlgCTrayVAT::Web Cam Auto Focus(%s)", strWidNarw );

			/* command execute status check @OnTimer():af_check_tmr_500ms */
			start_cmd_exec_check( af_check_tmr_500ms );
		}
	}
}


void CDlgCTrayVAT::OnBnClickedBtnRefPkrFineAdj()
{
	double cmd_err_range;
	CTrayPPCmd_VatWCamTch::VatPitchMode cmd_widnarw;
	CString strWidNarw;

	{/* user edit apply */
		if( is_modify() ){
			OnBnClickedBtnApply();
			if( is_modify() ) return;
		}	
	}

	int nRet = AfxMessageBox("Do you want to Web Cam Auto position adjust?", MB_YESNO);
	if( nRet != IDYES ) return;

	{/* wide or narrow selection */
		CDlgSelectItem::ItemArray ar;
		ar.push_back( CString("Wide")	);
		ar.push_back( CString("Narrow")	);

		CDlgSelectItem dlg(this, ar);
		if( dlg.DoModal() != IDOK ) return;

		nRet = AfxMessageBox( dlg.GetValue() + " teashing ?", MB_YESNO|MB_ICONWARNING  );
		if( nRet != IDYES ) return;

		strWidNarw = dlg.GetValue();
		switch( dlg.GetSelIndex() ){
		case 0: cmd_widnarw = CTrayPPCmd_VatWCamTch::WidePitch; break;
		case 1: cmd_widnarw = CTrayPPCmd_VatWCamTch::NarrowPitch; break;
		default:
			ASSERT("dlg.GetSelIndex() invalid"==0);
			return;
		}
	}

	{/* input scan range */
		CString v;
		nRet = g_LibCommCtrl.GetNumberBox( this, v.GetBuffer(256), 255, 
			"0.01", "Input Adjust Pass ±Range (mm)", "1.0", "0.001" );
		v.ReleaseBuffer();

		if( nRet != RETURN_OK ) return;
		cmd_err_range = _ttof( v )*1000;
	}

	{/* create command */
		CTrayPPCmd_VatWCamTch* pSeqCmd = new CTrayPPCmd_VatWCamTch;
		pSeqCmd->m_eCmd_PitchMode = cmd_widnarw;
		pSeqCmd->m_dCmd_PassRangeUm = cmd_err_range;

		int nErr = g_TaskSystemCtrl.SendCommand( g_TaskTrayPP, pSeqCmd, eEqpCtrlMode_Manual );
		if( nErr != ERR_NO_ERROR ){
			CParamList AlarmData;
			g_Error.AlarmReport( nErr, AlarmData, NULL );
		}else{
			MakeLog( "CDlgCTrayVAT::Web Cam Auto position adjust(%s)", strWidNarw );

			/* command execute status check @OnTimer():wcam_adj_tmr_500ms */
			start_cmd_exec_check( wcam_adj_tmr_500ms );
		}
	}
}


void CDlgCTrayVAT::OnBnClickedBtnApply()
{
	int nRet = AfxMessageBox("Do you want to APPLY?", MB_YESNO);

	if( nRet == IDYES )
	{
		g_VatHW.traypp.m_WideCamPos = this->m_stEditWideCam;
		g_VatHW.traypp.m_NarwCamPos = this->m_stEditNarrowCam;
		g_VatHW.traypp.Save();

		MakeLog( "CDlgCTrayVAT::Web Cam position applyed");
	}
}


void CDlgCTrayVAT::MakeLog(LPCTSTR fmt, ...)
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
