// DlgAxisActionMove.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgAxisActionMove.h"
#include "afxdialogex.h"


// CDlgAxisActionMove 대화 상자입니다.

IMPLEMENT_DYNAMIC( CDlgAxisActionMove, CDialogEx )

CDlgAxisActionMove::CDlgAxisActionMove( CWnd* pParent /*=NULL*/ )
	: CDialogEx( CDlgAxisActionMove::IDD, pParent )
{
	m_nStartPoint = -1;
	m_nSelectAction = 0;
	m_dDelayTime = 0.0;
	m_nSelectHandType = eTestPPLoadType;
	m_nCmdStage_auto = -1;
	m_vPicker_auto.clear();
	m_bChkTime = FALSE;
	m_nSelectDevice = 0;
}

CDlgAxisActionMove::~CDlgAxisActionMove()
{
	m_CmbFont.DeleteObject();
}

void CDlgAxisActionMove::DoDataExchange( CDataExchange* pDX )
{
	CDialogEx::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_COMBO_END_MOVE_POS, m_CmbEndPoint );
	DDX_Control( pDX, IDC_CHECK_HAND_POS1, m_ChkHand1_1 );
	DDX_Control( pDX, IDC_CHECK_HAND_POS2, m_ChkHand1_2 );
	DDX_Control( pDX, IDC_CHECK_HAND_POS3, m_ChkHand1_3 );
	DDX_Control( pDX, IDC_CHECK_HAND_POS4, m_ChkHand1_4 );
	DDX_Control( pDX, IDC_CHECK_HAND_POS5, m_ChkHand1_5 );
	DDX_Control( pDX, IDC_CHECK_HAND_POS6, m_ChkHand1_6 );
	DDX_Control( pDX, IDC_CHECK_HAND_POS7, m_ChkHand1_7 );
	DDX_Control( pDX, IDC_CHECK_HAND_POS8, m_ChkHand1_8 );
	DDX_Control( pDX, IDC_CHECK_HAND_POS9, m_ChkHand2_1 );
	DDX_Control( pDX, IDC_CHECK_HAND_POS10, m_ChkHand2_2 );
	DDX_Control( pDX, IDC_CHECK_HAND_POS11, m_ChkHand2_3 );
	DDX_Control( pDX, IDC_CHECK_HAND_POS12, m_ChkHand2_4 );
	DDX_Control( pDX, IDC_CHECK_HAND_POS13, m_ChkHand2_5 );
	DDX_Control( pDX, IDC_CHECK_HAND_POS14, m_ChkHand2_6 );
	DDX_Control( pDX, IDC_CHECK_HAND_POS15, m_ChkHand2_7 );
	DDX_Control( pDX, IDC_CHECK_HAND_POS16, m_ChkHand2_8 );
	DDX_Control(pDX, IDC_CHECK_HAND_PICK_PLACE_END, m_ChkPickPlaceEnd);
	DDX_Radio( pDX, IDC_RADIO_SEL_ACT1, m_nSelectAction );
	DDX_Text(pDX, IDC_EDIT_DELAY_TIME, m_dDelayTime);
	DDX_Radio(pDX, IDC_RADIO_SEL_LOAD_HAND, m_nSelectHandType);
	DDX_Radio(pDX, IDC_RADIO_SEL_DEVICE_NORMAL, m_nSelectDevice);
}


BEGIN_MESSAGE_MAP( CDlgAxisActionMove, CDialogEx )
	ON_BN_CLICKED( IDC_BUTTON_SET_Y1, &CDlgAxisActionMove::OnBnClickedButtonSetY1 )
	ON_BN_CLICKED( IDC_BUTTON_SET_Y2, &CDlgAxisActionMove::OnBnClickedButtonSetY2 )
	ON_BN_CLICKED( IDC_BUTTON_SET_ALL, &CDlgAxisActionMove::OnBnClickedButtonSetAll )
	ON_BN_CLICKED( IDC_BUTTON_MANU_MOVE, &CDlgAxisActionMove::OnBnClickedButtonManuMove )
	ON_BN_CLICKED( IDC_BUTTON_SET_LOAD, &CDlgAxisActionMove::OnBnClickedButtonSetLoad )
	ON_BN_CLICKED( IDC_BUTTON_SET_UNLOAD, &CDlgAxisActionMove::OnBnClickedButtonSetUnload )
	ON_BN_CLICKED( IDC_BUTTON_SET_CLEAR, &CDlgAxisActionMove::OnBnClickedButtonSetClear )
	ON_EN_SETFOCUS(IDC_EDIT_DELAY_TIME, &CDlgAxisActionMove::OnEnSetfocusEditDelayTime)
	ON_CONTROL_RANGE(BN_CLICKED,IDC_RADIO_SEL_LOAD_HAND,IDC_RADIO_SEL_UNLOAD_HAND,&CDlgAxisActionMove::OnRadioSeletTestHandType)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDCANCEL, &CDlgAxisActionMove::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RADIO_SEL_LOAD_HAND, &CDlgAxisActionMove::OnBnClickedRadioSelLoadHand)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_SEL_DEVICE_NORMAL, IDC_RADIO_SEL_DEVICE_CLEAN, &CDlgAxisActionMove::OnBnClickedRadioSelDeviceNormal)
	ON_CBN_SELCHANGE(IDC_COMBO_END_MOVE_POS, &CDlgAxisActionMove::OnCbnSelchangeComboEndMovePos)
END_MESSAGE_MAP()


// CDlgAxisActionMove 메시지 처리기입니다.


BOOL CDlgAxisActionMove::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString str = _T("");
	SetDlgItemText( IDC_EDIT_POS_X, _T( "1" ) );
	SetDlgItemText( IDC_EDIT_POS_Y, _T( "1" ) );
	SetDlgItemText( IDC_EDIT_POS_Z, _T( "1" ) );

	m_dDelayTime = g_DMCont.m_dmHandlerOpt.GD(DDM11_TESTPP_DELAY_TIME);
	
	str.Format("%.1f", m_dDelayTime);
	GetDlgItem(IDC_EDIT_DELAY_TIME)->SetWindowText(str);

	OnUpdateItemList();
	OnUpdateStartPointText();
	OnUpdateEndPointCombo();

	m_CmbFont.CreateFont(30,0,0,0, FW_BOLD,FALSE,FALSE,0, DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,CLEARTYPE_NATURAL_QUALITY,DEFAULT_PITCH | FF_SWISS,_T("Arial"));

	m_CmbEndPoint.SetFont(&m_CmbFont);

	m_CmbEndPoint.SetCurSel( 0 );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL CDlgAxisActionMove::PreTranslateMessage( MSG* pMsg )
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::PreTranslateMessage( pMsg );
}

void CDlgAxisActionMove::OnUpdateStartPointText()
{
	switch( m_nStartPoint )
	{
	case eMANU_OPER_PRESS_DN_1: SetDlgItemText( IDC_EDIT_STR_MOVE_POS, _T( "Press 1" ) );                    break;
	case eMANU_OPER_PRESS_DN_2: SetDlgItemText( IDC_EDIT_STR_MOVE_POS, _T( "Press 2" ) );                    break;
	case eMANU_OPER_PRESS_DN_3:	SetDlgItemText( IDC_EDIT_STR_MOVE_POS, _T( "Press 3" ) );                    break;
	case eMANU_OPER_PRESS_DN_4:	SetDlgItemText( IDC_EDIT_STR_MOVE_POS, _T( "Press 4" ) );                    break;
	case eMANU_OPER_PRESS_DN_5:	SetDlgItemText( IDC_EDIT_STR_MOVE_POS, _T( "Press 5" ) );                    break;
	case eMANU_OPER_PRESS_DN_6:	SetDlgItemText( IDC_EDIT_STR_MOVE_POS, _T( "Press 6" ) );                    break;
	case eMANU_OPER_PRESS_DN_7:	SetDlgItemText( IDC_EDIT_STR_MOVE_POS, _T( "Press 7" ) );                    break;
	case eMANU_OPER_PRESS_DN_8:	SetDlgItemText( IDC_EDIT_STR_MOVE_POS, _T( "Press 8" ) );                    break;
	case eMANU_OPER_TEST_PP1:	SetDlgItemText( IDC_EDIT_STR_MOVE_POS, _T( "Test PP 1" ) );                  break;
	case eMANU_OPER_TEST_PP2:	SetDlgItemText( IDC_EDIT_STR_MOVE_POS, _T( "Test PP 2" ) );                  break;
	case eMANU_OPER_LD_TBL1:	SetDlgItemText( IDC_EDIT_STR_MOVE_POS, _T( "Loading Table 1" ) );            break;
	case eMANU_OPER_LD_TBL2:	SetDlgItemText( IDC_EDIT_STR_MOVE_POS, _T( "Loading Table 2" ) );            break;
	case eMANU_OPER_LD_TBL3:	SetDlgItemText(	IDC_EDIT_STR_MOVE_POS, _T( "Loading Table 3" ) );			 break;
	case eMANU_OPER_LD_TBL4:	SetDlgItemText(	IDC_EDIT_STR_MOVE_POS, _T( "Loading Table 4" ) );			 break;
	case eMANU_OPER_TRAY_PP1_LD:	SetDlgItemText( IDC_EDIT_STR_MOVE_POS, _T( "Tray PP 1" ) );                  break;
	case eMANU_OPER_TRAY_PP2_UD:	SetDlgItemText(IDC_EDIT_STR_MOVE_POS, _T("Tray PP 2"));						 break;
	case eMANU_OPER_FEEDER:			SetDlgItemText(IDC_EDIT_STR_MOVE_POS, _T("Feeder"));						 break;
	case eMANU_OPER_TRANSFER:	SetDlgItemText( IDC_EDIT_STR_MOVE_POS, _T( "Transfer" ) );                   break;
	default:
		break;
	};
}

void CDlgAxisActionMove::OnUpdateItemList()
{
	GetDlgItem(IDC_CHECK_HAND_PICK_PLACE_END)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATIC_TEXT110)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TEXT111)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_DELAY_TIME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO_SEL_LOAD_HAND)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO_SEL_UNLOAD_HAND)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_SET_LOAD)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_SET_UNLOAD)->ShowWindow(SW_HIDE);

	switch( m_nStartPoint )
	{
	case eMANU_OPER_PRESS_DN_1: case eMANU_OPER_PRESS_DN_2:	case eMANU_OPER_PRESS_DN_3:	case eMANU_OPER_PRESS_DN_4:
	case eMANU_OPER_PRESS_DN_5:	case eMANU_OPER_PRESS_DN_6:	case eMANU_OPER_PRESS_DN_7:	case eMANU_OPER_PRESS_DN_8:
	case eMANU_OPER_LD_TBL1:	case eMANU_OPER_LD_TBL2: case eMANU_OPER_LD_TBL3:	case eMANU_OPER_LD_TBL4:
	case eMANU_OPER_FEEDER:
		{
			GetDlgItem( IDC_STATIC_TEXT42 )->ShowWindow( SW_HIDE );
			GetDlgItem( IDC_EDIT_POS_X )->ShowWindow( SW_HIDE );
			GetDlgItem(IDC_STATIC_TEXT47)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_POS_Y)->ShowWindow(SW_HIDE);		
			GetDlgItem( IDC_STATIC_TEXT44 )->ShowWindow( SW_HIDE );
			GetDlgItem( IDC_BUTTON_SET_LOAD )->ShowWindow( SW_HIDE );
			GetDlgItem( IDC_BUTTON_SET_UNLOAD )->ShowWindow( SW_HIDE );
			GetDlgItem( IDC_BUTTON_SET_Y1 )->ShowWindow( SW_HIDE );
			GetDlgItem( IDC_BUTTON_SET_Y2 )->ShowWindow( SW_HIDE );
			GetDlgItem( IDC_BUTTON_SET_ALL )->ShowWindow( SW_HIDE );
			GetDlgItem( IDC_STATIC_TEXT45 )->ShowWindow( SW_HIDE );
			GetDlgItem( IDC_RADIO_SEL_ACT1 )->ShowWindow( SW_HIDE );
			GetDlgItem( IDC_RADIO_SEL_ACT2 )->ShowWindow( SW_HIDE );
			GetDlgItem( IDC_RADIO_SEL_ACT3 )->ShowWindow( SW_HIDE );
			GetDlgItem( IDC_STATIC_TEXT50 )->ShowWindow( SW_HIDE );
			GetDlgItem( IDC_EDIT_POS_Z )->ShowWindow( SW_HIDE );
			GetDlgItem( IDC_BUTTON_SET_CLEAR )->ShowWindow( SW_HIDE );


			GetDlgItem(IDC_STATIC_TEXT121)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_RADIO_SEL_DEVICE_NORMAL)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_RADIO_SEL_DEVICE_CLEAN)->ShowWindow(SW_HIDE);

			if (m_nStartPoint == eMANU_OPER_FEEDER && m_nSelectAction == eDlgActionOnlyMove) {
				SetDlgItemText(IDC_STATIC_TEXT47, _T("Step : "));
				GetDlgItem(IDC_STATIC_TEXT47)->MoveWindow(96, 120, 100, 100);
				GetDlgItem(IDC_EDIT_POS_Y)->MoveWindow(200, 118, 60, 23);

				GetDlgItem(IDC_STATIC_TEXT47)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_EDIT_POS_Y)->ShowWindow(SW_SHOW);
				SetTimer(1, 200, NULL);
			}

			int ids_CHECK_HAND_POS[] = { IDC_CHECK_HAND_POS1,IDC_CHECK_HAND_POS2,IDC_CHECK_HAND_POS3,IDC_CHECK_HAND_POS4,IDC_CHECK_HAND_POS5,IDC_CHECK_HAND_POS6,IDC_CHECK_HAND_POS7,IDC_CHECK_HAND_POS8,
			IDC_CHECK_HAND_POS9, IDC_CHECK_HAND_POS10, IDC_CHECK_HAND_POS11, IDC_CHECK_HAND_POS12, IDC_CHECK_HAND_POS13, IDC_CHECK_HAND_POS14, IDC_CHECK_HAND_POS15, IDC_CHECK_HAND_POS16 };

			for( int i = 0; i < eMaxPressUnitCount * 2; i++ ) {
				GetDlgItem( ( ids_CHECK_HAND_POS[i] ) )->ShowWindow( SW_HIDE );
			}
		}break;
	case eMANU_OPER_TEST_PP1:	case eMANU_OPER_TEST_PP2:
		{
			int ids_CHECK_HAND_POS[] = { IDC_CHECK_HAND_POS1,IDC_CHECK_HAND_POS2,IDC_CHECK_HAND_POS3,IDC_CHECK_HAND_POS4,IDC_CHECK_HAND_POS5,IDC_CHECK_HAND_POS6,IDC_CHECK_HAND_POS7,IDC_CHECK_HAND_POS8,
			IDC_CHECK_HAND_POS9, IDC_CHECK_HAND_POS10, IDC_CHECK_HAND_POS11, IDC_CHECK_HAND_POS12, IDC_CHECK_HAND_POS13, IDC_CHECK_HAND_POS14, IDC_CHECK_HAND_POS15, IDC_CHECK_HAND_POS16 };

			//4Para 예외처리.
			int nSiteDivY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

			for( int i = 0; i < eMaxPressUnitCount * 2; i++ ) {
				if (nSiteDivY != 2)
				{
					if ((ids_CHECK_HAND_POS[i]) == IDC_CHECK_HAND_POS1
						|| (ids_CHECK_HAND_POS[i]) == IDC_CHECK_HAND_POS2
						|| (ids_CHECK_HAND_POS[i]) == IDC_CHECK_HAND_POS3
						|| (ids_CHECK_HAND_POS[i]) == IDC_CHECK_HAND_POS4) {
						continue;
					}
				}
				else
				{
					if ((ids_CHECK_HAND_POS[i]) == IDC_CHECK_HAND_POS1
						|| (ids_CHECK_HAND_POS[i]) == IDC_CHECK_HAND_POS2
						|| (ids_CHECK_HAND_POS[i]) == IDC_CHECK_HAND_POS3
						|| (ids_CHECK_HAND_POS[i]) == IDC_CHECK_HAND_POS4
						|| (ids_CHECK_HAND_POS[i]) == IDC_CHECK_HAND_POS9
						|| (ids_CHECK_HAND_POS[i]) == IDC_CHECK_HAND_POS10
						|| (ids_CHECK_HAND_POS[i]) == IDC_CHECK_HAND_POS11
						|| (ids_CHECK_HAND_POS[i]) == IDC_CHECK_HAND_POS12) {
						continue;
					}
				}
				GetDlgItem( ( ids_CHECK_HAND_POS[i] ) )->ShowWindow( SW_HIDE );
			}

			if( m_nSelectAction == eDlgActionPick || m_nSelectAction == eDlgActionPlace ) {
				GetDlgItem( IDC_RADIO_SEL_ACT3 )->ShowWindow( SW_HIDE );
				GetDlgItem(IDC_CHECK_HAND_PICK_PLACE_END)->ShowWindow(SW_SHOW);
			}
			else if( m_nSelectAction == eDlgActionExchange ) {
				GetDlgItem( IDC_STATIC_TEXT42 )->ShowWindow( SW_HIDE );
				GetDlgItem( IDC_EDIT_POS_X )->ShowWindow( SW_HIDE );
				GetDlgItem( IDC_RADIO_SEL_ACT1 )->ShowWindow( SW_HIDE );
				GetDlgItem( IDC_RADIO_SEL_ACT2 )->ShowWindow( SW_HIDE );

				GetDlgItem(IDC_STATIC_TEXT121)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RADIO_SEL_DEVICE_NORMAL)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RADIO_SEL_DEVICE_CLEAN)->ShowWindow(SW_HIDE);
			}
			else {
				GetDlgItem( IDC_STATIC_TEXT45 )->ShowWindow( SW_HIDE );
				GetDlgItem( IDC_RADIO_SEL_ACT1 )->ShowWindow( SW_HIDE );
				GetDlgItem( IDC_RADIO_SEL_ACT2 )->ShowWindow( SW_HIDE );
				GetDlgItem( IDC_RADIO_SEL_ACT3 )->ShowWindow( SW_HIDE );

				GetDlgItem(IDC_STATIC_TEXT121)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RADIO_SEL_DEVICE_NORMAL)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RADIO_SEL_DEVICE_CLEAN)->ShowWindow(SW_HIDE);
			}

			GetDlgItem(IDC_BUTTON_SET_LOAD)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BUTTON_SET_UNLOAD)->ShowWindow(SW_HIDE);
			GetDlgItem( IDC_STATIC_TEXT50 )->ShowWindow( SW_HIDE );
			GetDlgItem( IDC_EDIT_POS_Z )->ShowWindow( SW_HIDE );

			GetDlgItem(IDC_RADIO_SEL_LOAD_HAND)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_RADIO_SEL_UNLOAD_HAND)->ShowWindow(SW_SHOW);

			if (nSiteDivY != 2)
			{
				GetDlgItem(IDC_BUTTON_SET_Y1)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_SET_Y2)->EnableWindow(FALSE);
			}
			


		}break;
	case eMANU_OPER_TRAY_PP1_LD:
		{
			GetDlgItem(IDC_STATIC_TEXT50)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_POS_Z)->ShowWindow(SW_HIDE);


			GetDlgItem(IDC_STATIC_TEXT121)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_RADIO_SEL_DEVICE_NORMAL)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_RADIO_SEL_DEVICE_CLEAN)->ShowWindow(SW_HIDE);

			int ids_CHECK_HAND_POS[] = { IDC_CHECK_HAND_POS1,IDC_CHECK_HAND_POS2,IDC_CHECK_HAND_POS3,IDC_CHECK_HAND_POS4,IDC_CHECK_HAND_POS5,IDC_CHECK_HAND_POS6,IDC_CHECK_HAND_POS7,IDC_CHECK_HAND_POS8,
				IDC_CHECK_HAND_POS9, IDC_CHECK_HAND_POS10, IDC_CHECK_HAND_POS11, IDC_CHECK_HAND_POS12, IDC_CHECK_HAND_POS13, IDC_CHECK_HAND_POS14, IDC_CHECK_HAND_POS15, IDC_CHECK_HAND_POS16 };

			for (int i = 0; i < eMaxPressUnitCount * 2; i++) {
				if ((ids_CHECK_HAND_POS[i]) == IDC_CHECK_HAND_POS1
					|| (ids_CHECK_HAND_POS[i]) == IDC_CHECK_HAND_POS2
					|| (ids_CHECK_HAND_POS[i]) == IDC_CHECK_HAND_POS3
					|| (ids_CHECK_HAND_POS[i]) == IDC_CHECK_HAND_POS4
					|| (ids_CHECK_HAND_POS[i]) == IDC_CHECK_HAND_POS9
					|| (ids_CHECK_HAND_POS[i]) == IDC_CHECK_HAND_POS10
					|| (ids_CHECK_HAND_POS[i]) == IDC_CHECK_HAND_POS11
					|| (ids_CHECK_HAND_POS[i]) == IDC_CHECK_HAND_POS12) {
					continue;
				}
				GetDlgItem((ids_CHECK_HAND_POS[i]))->ShowWindow(SW_HIDE);
			}
			if (m_nSelectAction == eDlgActionPick || m_nSelectAction == eDlgActionPlace) {
				GetDlgItem(IDC_RADIO_SEL_ACT3)->ShowWindow(SW_HIDE);
			}
			else {
				GetDlgItem(IDC_STATIC_TEXT45)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RADIO_SEL_ACT1)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RADIO_SEL_ACT2)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RADIO_SEL_ACT3)->ShowWindow(SW_HIDE);
			}
		}break;
	case eMANU_OPER_TRAY_PP2_UD:
		{
			GetDlgItem( IDC_STATIC_TEXT50 )->ShowWindow( SW_HIDE );
			GetDlgItem( IDC_EDIT_POS_Z )->ShowWindow( SW_HIDE );

			GetDlgItem(IDC_STATIC_TEXT121)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_RADIO_SEL_DEVICE_NORMAL)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_RADIO_SEL_DEVICE_CLEAN)->ShowWindow(SW_HIDE);

			int ids_CHECK_HAND_POS[] = { IDC_CHECK_HAND_POS1,IDC_CHECK_HAND_POS2,IDC_CHECK_HAND_POS3,IDC_CHECK_HAND_POS4,IDC_CHECK_HAND_POS5,IDC_CHECK_HAND_POS6,IDC_CHECK_HAND_POS7,IDC_CHECK_HAND_POS8,
			IDC_CHECK_HAND_POS9, IDC_CHECK_HAND_POS10, IDC_CHECK_HAND_POS11, IDC_CHECK_HAND_POS12, IDC_CHECK_HAND_POS13, IDC_CHECK_HAND_POS14, IDC_CHECK_HAND_POS15, IDC_CHECK_HAND_POS16 };
			
			for( int i = 0; i < eMaxPressUnitCount * 2; i++ ) {
				if(    ( ids_CHECK_HAND_POS[i] ) == IDC_CHECK_HAND_POS1
					|| ( ids_CHECK_HAND_POS[i] ) == IDC_CHECK_HAND_POS2
					|| ( ids_CHECK_HAND_POS[i] ) == IDC_CHECK_HAND_POS3
					|| ( ids_CHECK_HAND_POS[i] ) == IDC_CHECK_HAND_POS4
					|| ( ids_CHECK_HAND_POS[i] ) == IDC_CHECK_HAND_POS9
					|| ( ids_CHECK_HAND_POS[i] ) == IDC_CHECK_HAND_POS10
					|| ( ids_CHECK_HAND_POS[i] ) == IDC_CHECK_HAND_POS11
					|| ( ids_CHECK_HAND_POS[i] ) == IDC_CHECK_HAND_POS12 ) {
					continue;
				}
				GetDlgItem( ( ids_CHECK_HAND_POS[i] ) )->ShowWindow( SW_HIDE );
			}
			if( m_nSelectAction == eDlgActionPick || m_nSelectAction == eDlgActionPlace ) {
				GetDlgItem( IDC_RADIO_SEL_ACT3 )->ShowWindow( SW_HIDE );
			}
			else {
				GetDlgItem( IDC_STATIC_TEXT45 )->ShowWindow( SW_HIDE );
				GetDlgItem( IDC_RADIO_SEL_ACT1 )->ShowWindow( SW_HIDE );
				GetDlgItem( IDC_RADIO_SEL_ACT2 )->ShowWindow( SW_HIDE );
				GetDlgItem( IDC_RADIO_SEL_ACT3 )->ShowWindow( SW_HIDE );
			}
		}break;
	case eMANU_OPER_TRANSFER:
		{
			GetDlgItem( IDC_STATIC_TEXT42 )->ShowWindow( SW_HIDE );
			GetDlgItem( IDC_EDIT_POS_X )->ShowWindow( SW_HIDE );
			GetDlgItem( IDC_STATIC_TEXT47 )->ShowWindow( SW_HIDE );
			GetDlgItem( IDC_EDIT_POS_Y )->ShowWindow( SW_HIDE );

			GetDlgItem( IDC_STATIC_TEXT44 )->ShowWindow( SW_HIDE );
			GetDlgItem( IDC_BUTTON_SET_LOAD )->ShowWindow( SW_HIDE );
			GetDlgItem( IDC_BUTTON_SET_UNLOAD )->ShowWindow( SW_HIDE );
			GetDlgItem( IDC_BUTTON_SET_Y1 )->ShowWindow( SW_HIDE );
			GetDlgItem( IDC_BUTTON_SET_Y2 )->ShowWindow( SW_HIDE );
			GetDlgItem( IDC_BUTTON_SET_ALL )->ShowWindow( SW_HIDE );
			GetDlgItem( IDC_BUTTON_SET_CLEAR )->ShowWindow( SW_HIDE );
			GetDlgItem( IDC_STATIC_TEXT44 )->ShowWindow( SW_HIDE );
			GetDlgItem( IDC_STATIC_TEXT50 )->ShowWindow( SW_HIDE );
			GetDlgItem( IDC_EDIT_POS_Z )->ShowWindow( SW_HIDE );

			GetDlgItem(IDC_STATIC_TEXT121)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_RADIO_SEL_DEVICE_NORMAL)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_RADIO_SEL_DEVICE_CLEAN)->ShowWindow(SW_HIDE);
			

// 			int nHandCheckResorce[] = { IDC_CHECK_HAND_POS1 ,IDC_CHECK_HAND_POS2,IDC_CHECK_HAND_POS3,IDC_CHECK_HAND_POS4,IDC_CHECK_HAND_POS5,IDC_CHECK_HAND_POS6,IDC_CHECK_HAND_POS7,IDC_CHECK_HAND_POS8 };
// 
// 			for( int i = 0; i < eMaxPressUnitCount; i++ ) {
// 				GetDlgItem(nHandCheckResorce[i])->ShowWindow( SW_HIDE );
// 			}

			int ids_CHECK_HAND_POS[] = { IDC_CHECK_HAND_POS1,IDC_CHECK_HAND_POS2,IDC_CHECK_HAND_POS3,IDC_CHECK_HAND_POS4,IDC_CHECK_HAND_POS5,IDC_CHECK_HAND_POS6,IDC_CHECK_HAND_POS7,IDC_CHECK_HAND_POS8,
				IDC_CHECK_HAND_POS9, IDC_CHECK_HAND_POS10, IDC_CHECK_HAND_POS11, IDC_CHECK_HAND_POS12, IDC_CHECK_HAND_POS13, IDC_CHECK_HAND_POS14, IDC_CHECK_HAND_POS15, IDC_CHECK_HAND_POS16 };

			for (int i = 0; i < eMaxPressUnitCount * 2; i++) {
				GetDlgItem((ids_CHECK_HAND_POS[i]))->ShowWindow(SW_HIDE);
			}

			if( m_nSelectAction == eDlgActionPick || m_nSelectAction == eDlgActionPlace ) {
				GetDlgItem( IDC_RADIO_SEL_ACT3 )->ShowWindow( SW_HIDE );
			}
			else {
				GetDlgItem( IDC_STATIC_TEXT45 )->ShowWindow( SW_HIDE );
				GetDlgItem( IDC_RADIO_SEL_ACT1 )->ShowWindow( SW_HIDE );
				GetDlgItem( IDC_RADIO_SEL_ACT2 )->ShowWindow( SW_HIDE );
				GetDlgItem( IDC_RADIO_SEL_ACT3 )->ShowWindow( SW_HIDE );
			}
		}break;
	default:
		break;
	};
}

void  CDlgAxisActionMove::OnUpdateEndPointCombo()
{
	m_CmbEndPoint.ResetContent();

	switch( m_nStartPoint )
	{
	case eMANU_OPER_PRESS_DN_1: case eMANU_OPER_PRESS_DN_2: case eMANU_OPER_PRESS_DN_3:	case eMANU_OPER_PRESS_DN_4:
	case eMANU_OPER_PRESS_DN_5:	case eMANU_OPER_PRESS_DN_6:	case eMANU_OPER_PRESS_DN_7:	case eMANU_OPER_PRESS_DN_8:
		{
			m_CmbEndPoint.AddString( "Safety Position" );
			m_CmbEndPoint.AddString( "Contact Position" );
			char szCompany[16];
			g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));
		
			GetDlgItem(IDC_STATIC_TEXT110)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_DELAY_TIME)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_TEXT111)->ShowWindow(SW_HIDE);
			
		}break;
	case eMANU_OPER_TEST_PP1:	case eMANU_OPER_TEST_PP2:
		{
			if( m_nSelectAction == eDlgActionExchange )
			{
// 				m_CmbEndPoint.AddString( "Test Site Down1" );
// 				m_CmbEndPoint.AddString( "Test Site Down2" );
// 				m_CmbEndPoint.AddString( "Test Site Down3" );
// 				m_CmbEndPoint.AddString( "Test Site Down4" );
			}
			else {
				if (m_nStartPoint == eMANU_OPER_TEST_PP1)
				{
					m_CmbEndPoint.AddString("Test Site Down1");
					m_CmbEndPoint.AddString("Test Site Down2");
					m_CmbEndPoint.AddString("Test Site Down3");
					m_CmbEndPoint.AddString("Test Site Down4");
				}
				else
				{
					m_CmbEndPoint.AddString("Test Site Down5");
					m_CmbEndPoint.AddString("Test Site Down6");
					m_CmbEndPoint.AddString("Test Site Down7");
					m_CmbEndPoint.AddString("Test Site Down8");
				}
				
				if (m_nSelectHandType == eTestPPLoadType) {
					m_CmbEndPoint.AddString("Loading Table1_L");
					m_CmbEndPoint.AddString("Loading Table2_L");
					m_CmbEndPoint.AddString("Loading Table3_L");
					m_CmbEndPoint.AddString("Loading Table4_L");
				}
				else {
					m_CmbEndPoint.AddString("Loading Table Buffer_L");
				}
				m_CmbEndPoint.AddString("Loading Table1_U");
				m_CmbEndPoint.AddString("Loading Table2_U");
				m_CmbEndPoint.AddString("Loading Table3_U");
				m_CmbEndPoint.AddString("Loading Table4_U");
				if (m_nSelectHandType == eTestPPLoadType) {
					m_CmbEndPoint.AddString("SocketClean Buff");
				}
				m_CmbEndPoint.AddString("Safety");
				
			}
		}break;
	case eMANU_OPER_LD_TBL1: case eMANU_OPER_LD_TBL4:
	{
		m_CmbEndPoint.AddString("UnLoading Site" );
		m_CmbEndPoint.AddString("Test Site1");
		m_CmbEndPoint.AddString("Test Site2");
		m_CmbEndPoint.AddString("Test Site3");
		m_CmbEndPoint.AddString("Test Site4");
	}break;
	case eMANU_OPER_LD_TBL2: case eMANU_OPER_LD_TBL3:
		{
//			m_CmbEndPoint.AddString("UnLoading Site" );
			m_CmbEndPoint.AddString("Loading Site");
			m_CmbEndPoint.AddString("Test Site1");
			m_CmbEndPoint.AddString("Test Site2");
			m_CmbEndPoint.AddString("Test Site3");
			m_CmbEndPoint.AddString("Test Site4");
			m_CmbEndPoint.AddString("2d Barcode");
		}break;
	case eMANU_OPER_TRAY_PP1_LD:
		{
			m_CmbEndPoint.AddString("Feeder");
			m_CmbEndPoint.AddString("Conveyor 3");
//			m_CmbEndPoint.AddString("Loading Table #1");
			m_CmbEndPoint.AddString("Loading Table #2");
			m_CmbEndPoint.AddString("Loading Table #3");
//			m_CmbEndPoint.AddString("Loading Table #4");
			m_CmbEndPoint.AddString("Safety");
		}break;
	case eMANU_OPER_TRAY_PP2_UD:
	{			
			m_CmbEndPoint.AddString("Unloader #1");
			m_CmbEndPoint.AddString("Unloader #2");
			m_CmbEndPoint.AddString("Unloader #3");
			m_CmbEndPoint.AddString("Unloader #4");	
			m_CmbEndPoint.AddString("Loading Table #1");
// 			m_CmbEndPoint.AddString("Loading Table #2");
// 			m_CmbEndPoint.AddString("Loading Table #3");
			m_CmbEndPoint.AddString("Loading Table #4");
			m_CmbEndPoint.AddString("Safety");		
		}break;
	case eMANU_OPER_TRANSFER:
		{
			m_CmbEndPoint.AddString( "Loader Empty" );
			m_CmbEndPoint.AddString("Unloader Empty");
			m_CmbEndPoint.AddString( "Loader" );	
			m_CmbEndPoint.AddString( "Unloader #1" );
			m_CmbEndPoint.AddString( "Unloader #2" );
			m_CmbEndPoint.AddString( "Unloader #3" );
			m_CmbEndPoint.AddString( "Unloader #4" );

			//TRANSFER MOVEPickPlace 시 Safety의 경우가 없어 행업 수정[5/26/2025 seungtaek.oh]
			if (m_nSelectAction != DEF_PICK && m_nSelectAction != DEF_PLACE) {
				m_CmbEndPoint.AddString("Safety");
			}
		}break;
	case eMANU_OPER_FEEDER:
		{
			if(m_nSelectAction == eDlgActionOnlyMove)
			{
				m_CmbEndPoint.AddString("Pick Start Pos");
				m_CmbEndPoint.AddString("Tray Pick Pos");
				m_CmbEndPoint.AddString("Tray Place Pos");
				m_CmbEndPoint.AddString("Safety");
			}
			else
			{
				m_CmbEndPoint.AddString("Loading Site PICK");
				m_CmbEndPoint.AddString("Unloading Site PLACE");
			}	
		}break;
	default:
		break;
	};
}


void CDlgAxisActionMove::OnBnClickedButtonSetY1()
{
	m_ChkHand1_1.SetCheck( TRUE );
	m_ChkHand1_2.SetCheck( TRUE );
	m_ChkHand1_3.SetCheck( TRUE );
	m_ChkHand1_4.SetCheck( TRUE );
	m_ChkHand1_5.SetCheck( TRUE );
	m_ChkHand1_6.SetCheck( TRUE );
	m_ChkHand1_7.SetCheck( TRUE );
	m_ChkHand1_8.SetCheck( TRUE );
}


void CDlgAxisActionMove::OnBnClickedButtonSetY2()
{
	m_ChkHand2_1.SetCheck( TRUE );
	m_ChkHand2_2.SetCheck( TRUE );
	m_ChkHand2_3.SetCheck( TRUE );
	m_ChkHand2_4.SetCheck( TRUE );
	m_ChkHand2_5.SetCheck( TRUE );
	m_ChkHand2_6.SetCheck( TRUE );
	m_ChkHand2_7.SetCheck( TRUE );
	m_ChkHand2_8.SetCheck( TRUE );
}


void CDlgAxisActionMove::OnBnClickedButtonSetAll()
{
	m_ChkHand1_1.SetCheck( TRUE );
	m_ChkHand1_2.SetCheck( TRUE );
	m_ChkHand1_3.SetCheck( TRUE );
	m_ChkHand1_4.SetCheck( TRUE );
	m_ChkHand1_5.SetCheck( TRUE );
	m_ChkHand1_6.SetCheck( TRUE );
	m_ChkHand1_7.SetCheck( TRUE );
	m_ChkHand1_8.SetCheck( TRUE );
	m_ChkHand2_1.SetCheck( TRUE );
	m_ChkHand2_2.SetCheck( TRUE );
	m_ChkHand2_3.SetCheck( TRUE );
	m_ChkHand2_4.SetCheck( TRUE );
	m_ChkHand2_5.SetCheck( TRUE );
	m_ChkHand2_6.SetCheck( TRUE );
	m_ChkHand2_7.SetCheck( TRUE );
	m_ChkHand2_8.SetCheck( TRUE );

	//4Para 예외처리.
	int nSiteDivY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	if (nSiteDivY != 2  && (m_nStartPoint == eMANU_OPER_TEST_PP1 || m_nStartPoint == eMANU_OPER_TEST_PP2))
	{
		m_ChkHand2_1.SetCheck(FALSE);
		m_ChkHand2_2.SetCheck(FALSE);
		m_ChkHand2_3.SetCheck(FALSE);
		m_ChkHand2_4.SetCheck(FALSE);
	}
}


void CDlgAxisActionMove::OnBnClickedButtonManuMove()
{
	UpdateData();
	
	int nLocStage = -1;
	std::vector<int> vPicker;
	std::vector<CPoint> vPick;
	std::vector<CPoint> vPlace;
	int nUseHand[ eMAX_Y_PICKER ][ eMAX_X_PICKER ] = { {0}, };

	nUseHand[ 0 ][ 0 ] = m_ChkHand1_1.GetCheck();	nUseHand[ 0 ][ 1 ] = m_ChkHand1_2.GetCheck();
	nUseHand[ 0 ][ 2 ] = m_ChkHand1_3.GetCheck();	nUseHand[ 0 ][ 3 ] = m_ChkHand1_4.GetCheck();
	nUseHand[ 1 ][ 0 ] = m_ChkHand2_1.GetCheck();	nUseHand[ 1 ][ 1 ] = m_ChkHand2_2.GetCheck();
	nUseHand[ 1 ][ 2 ] = m_ChkHand2_3.GetCheck();	nUseHand[ 1 ][ 3 ] = m_ChkHand2_4.GetCheck();

	if( CheckUI_InterLock( nUseHand ) == 1 ) {
		return;
	}
	int nCmdStage = -1;
	int nCmdPickPlace = -1; // DEF_PLACE, DEF_PICK
	int nErr = -1;
	CParamList AlarmData;
	CString strStartPoint = _T( "" );

	GetDlgItemText( IDC_EDIT_STR_MOVE_POS, strStartPoint );

	switch( m_nStartPoint )
	{
	case eMANU_OPER_PRESS_DN_1:
	case eMANU_OPER_PRESS_DN_2:
	case eMANU_OPER_PRESS_DN_3:
	case eMANU_OPER_PRESS_DN_4:
	case eMANU_OPER_PRESS_DN_5:
	case eMANU_OPER_PRESS_DN_6:
	case eMANU_OPER_PRESS_DN_7:
	case eMANU_OPER_PRESS_DN_8:
		{
			/*LOC_IDX_PRESS_UNIT_SAFETY =0, LOC_IDX_PRESS_UNIT_CONTACT*/
			nCmdStage = m_CmbEndPoint.GetCurSel();
			CString strMovePoint;
			if (nCmdStage == LOC_IDX_PRESS_UNIT_SAFETY /*|| nCmdStage == LOC_IDX_PRESS_UNIT_CONTACT*/)
			{
				CPressUnitCmd_MovePos* pSeqCmd = new CPressUnitCmd_MovePos;
				pSeqCmd->m_nCmdStage = nCmdStage;
				nErr = g_TaskSystemCtrl.SendCommand(g_TaskPressUnit[m_nStartPoint], pSeqCmd, eEqpCtrlMode_Manual);
				strMovePoint = (nCmdStage == 0 ? _T("Safety Position") : _T("Contact Position"));
			}
			else if (nCmdStage == LOC_IDX_PRESS_UNIT_CONTACT)
			{
				CPressUnitCmd_Contact* pSeqCmd = new CPressUnitCmd_Contact;
				pSeqCmd->m_nCmdStage = nCmdStage;
				nErr = g_TaskSystemCtrl.SendCommand(g_TaskPressUnit[m_nStartPoint], pSeqCmd, eEqpCtrlMode_Manual);
				strMovePoint = (nCmdStage == 0 ? _T("Safety Position") : _T("Contact Position"));
			}

			//AlarmData
		
			MakeLog( "%s Move Point: %s", strStartPoint, strMovePoint );
		}break;
	case eMANU_OPER_TEST_PP1:
	case eMANU_OPER_TEST_PP2:
		{
			if (m_nSelectHandType != eTestPPLoadType && m_nSelectHandType != eTestPPUnLoadType) {
				AfxMessageBox("Hand Type Select !!");
				return;
			}
				
			int nTestPpIdx = m_nStartPoint - eMANU_OPER_TEST_PP1;

			nCmdStage = ConvertTestPPCmdStage(m_CmbEndPoint.GetCurSel());
			//nCmdStage = m_CmbEndPoint.GetCurSel();

			if( m_nSelectAction == eDlgActionExchange )
				nCmdStage += LOC_IDX_STATION_1;

			if( m_nSelectAction == DEF_PICK ) { nCmdPickPlace = DEF_PICK; }
			else if( m_nSelectAction == DEF_PLACE ) { nCmdPickPlace = DEF_PLACE; }

			// check site pos safety
			int nRetCmd = ERR_NO_ERROR;
			switch( nCmdStage ){
			case LOC_IDX_STATION_1: {nRetCmd = g_TaskPressUnit[ ePressUnit_Down_1 + ( eMaxATS_Area  * nTestPpIdx ) ].ChkDI( CTaskPressUnit::xZAXIS_SAFETY_SEN, DEF_ON ); } break;
			case LOC_IDX_STATION_2: {nRetCmd = g_TaskPressUnit[ ePressUnit_Down_2 + ( eMaxATS_Area  * nTestPpIdx ) ].ChkDI( CTaskPressUnit::xZAXIS_SAFETY_SEN, DEF_ON ); } break;
			case LOC_IDX_STATION_3: {nRetCmd = g_TaskPressUnit[ ePressUnit_Down_3 + ( eMaxATS_Area  * nTestPpIdx ) ].ChkDI( CTaskPressUnit::xZAXIS_SAFETY_SEN, DEF_ON ); } break;
			case LOC_IDX_STATION_4: {nRetCmd = g_TaskPressUnit[ ePressUnit_Down_4 + ( eMaxATS_Area  * nTestPpIdx ) ].ChkDI( CTaskPressUnit::xZAXIS_SAFETY_SEN, DEF_ON ); } break;
			}

			if( nRetCmd != ERR_NO_ERROR ) {
				AfxMessageBox( "Press Unit is not safety pos!!!" );
				return;
			}

			CString strTargetX = _T( "" ), strTargetY = _T( "" );
			GetDlgItemText( IDC_EDIT_POS_X, strTargetX );
			GetDlgItemText( IDC_EDIT_POS_Y, strTargetY );

			int nTargetX = atoi( strTargetX ) - 1;
			int nTargetY = atoi( strTargetY ) - 1;

			// Pocket Interlock 
			if( nCmdStage <= LOC_IDX_STATION_1 && LOC_IDX_STATION_4 >= nCmdStage )
			{
				if (m_nSelectAction != eDlgActionExchange)
				{
					CPoint Select_Picker = Chk_Select(nUseHand);
					if (nTargetX != Select_Picker.x) {
						CString strErrorMsg = _T("");
						strErrorMsg.Format(_T("Setting X Pos : %d!!!"), Select_Picker.x + 1);
						AfxMessageBox(strErrorMsg);
						return;
					}
				}
			}
			else{			
			
			}

			if (m_nSelectHandType == eTestPPUnLoadType) // Unload Hand는 Clean Device Offset이 필요 없음
				m_nSelectDevice = eCleanOffsetNotUse;
			
			CString strSelectAction = _T( "" );
			CString strSelectPicker = _T( "" );

			switch( m_nSelectAction ) {
			case DEF_PICK: strSelectAction = _T( "Move Pick" ); break;
			case DEF_PLACE: strSelectAction = _T( "Move Place" ); break;
			case DEF_EXCH: strSelectAction = _T( "Exchage" ); break;
			default: strSelectAction = _T( "Move Pos" ); break;
			}

			if( m_nSelectAction == DEF_PICK || m_nSelectAction == DEF_PLACE ) {
				for( int nY = 0; nY < MAX_TRAY_PP_CNT_Y; nY++ ) {
					for( int nX = 0; nX < MAX_TRAY_PP_CNT_X; nX++ ) {
						if( nUseHand[ nY ][ nX ] == TRUE ) { vPick.push_back( CPoint( nX, nY ) ); }
					}
				}
				strSelectPicker = g_TaskTestPP[ nTestPpIdx ].GetPickerName( vPick );

				CTestPPCmd_MovePickPlace* pSeqCmd = new CTestPPCmd_MovePickPlace;
				pSeqCmd->m_nCmdStage = nCmdStage;
				pSeqCmd->m_cCmdPocket = CPoint( nTargetX, nTargetY );
				pSeqCmd->m_nCmdPick = nCmdPickPlace;
				pSeqCmd->m_vCmdPicker = vPick;
				pSeqCmd->m_nHandType = m_nSelectHandType;
				pSeqCmd->m_bCleanOffsetUse = m_nSelectDevice;


				

				pSeqCmd->m_cCmdManualPickPlace = DEF_UNK;

				if(m_ChkPickPlaceEnd.GetCheck() == FALSE)
					pSeqCmd->m_cCmdManualPickPlace = DEF_UNK;
				else {
					pSeqCmd->m_cCmdManualPickPlace = nCmdPickPlace;
				}
				nErr = g_TaskSystemCtrl.SendCommand( g_TaskTestPP[ nTestPpIdx ], pSeqCmd, eEqpCtrlMode_Manual );
				//CTestPPCmd_MoveSamePlaceStation* pSeqCmd = new CTestPPCmd_MoveSamePlaceStation;
				//pSeqCmd->m_nCmdStage = LOC_IDX_STATION_UP_3;
				//pSeqCmd->m_cCmdPocket = CPoint(0, 0);
				//pSeqCmd->m_vCmdPicker.push_back(CPoint(0, 0));
				//nErr = g_TaskSystemCtrl.SendCommand(g_TaskTestPP[nTestPpIdx], pSeqCmd, eEqpCtrlMode_Manual);
			}else if( m_nSelectAction == eDlgActionExchange ) {
				for( int nX = 0; nX < MAX_TRAY_PP_CNT_X; nX++ ) {
					if( nUseHand[ 0 ][ nX ] == TRUE ) { vPick.push_back( CPoint( nX, 0 ) ); }
					if( nUseHand[ 1 ][ nX ] == TRUE ) { vPlace.push_back( CPoint( nX, 1 ) ); }
				}

				strSelectPicker += g_TaskTestPP[ nTestPpIdx ].GetPickerName( vPlace );
				strSelectPicker += g_TaskTestPP[ nTestPpIdx ].GetPickerName( vPick );

// 				CTestPPCmd_MoveExchange* pSeqCmd = new CTestPPCmd_MoveExchange;
// 				pSeqCmd->m_nCmdStage = nCmdStage;
// 				pSeqCmd->m_cCmdPocketPick = CPoint( vPick[ 0 ].x, nTargetY );
// 				pSeqCmd->m_cCmdPocketPlace = CPoint( vPlace[0].x, nTargetY );
// 				pSeqCmd->m_vCmdPicker_Pick = vPick;
// 				pSeqCmd->m_vCmdPicker_Place = vPlace;
// 				pSeqCmd->m_nHandType = m_nSelectHandType;
// 				nErr = g_TaskSystemCtrl.SendCommand( g_TaskTestPP[ nTestPpIdx ], pSeqCmd, eEqpCtrlMode_Manual );

			}else {
				for( int nY = 0; nY < MAX_TRAY_PP_CNT_Y; nY++ ) {
					for( int nX = 0; nX < MAX_TRAY_PP_CNT_X; nX++ ) {
						if( nUseHand[ nY ][ nX ] == TRUE ) { vPick.push_back( CPoint( nX, nY ) ); }
					}
				}
				strSelectPicker = g_TaskTestPP[ nTestPpIdx ].GetPickerName( vPick );

				CTestPPCmd_MoveAlreadyPos* pSeqCmd = new CTestPPCmd_MoveAlreadyPos;
				pSeqCmd->m_nCmdStage = nCmdStage;
				pSeqCmd->m_cCmdPocket = CPoint( nTargetX, nTargetY );;
				pSeqCmd->m_vCmdPicker = vPick;
				pSeqCmd->m_bXmoveTarget = TRUE;
				pSeqCmd->m_nHandType = m_nSelectHandType;
				nErr = g_TaskSystemCtrl.SendCommand( g_TaskTestPP[ nTestPpIdx ], pSeqCmd, eEqpCtrlMode_Manual );
			}

			if( nErr == ERR_NO_ERROR ) {
				MakeLog( "Test PP %d %s  CmdStage: %s X Pos: %d Y Pos: %d Using picker : %s ", nTestPpIdx + 1, strSelectAction, g_TaskTestPP[ nTestPpIdx ].GetLocName( nCmdStage ), nTargetX + 1, nTargetY + 1, strSelectPicker );
			}
		}break;
	case eMANU_OPER_LD_TBL1:
	case eMANU_OPER_LD_TBL2:
	case eMANU_OPER_LD_TBL3:
	case eMANU_OPER_LD_TBL4:
		{
			int nLoadTblIdx = m_nStartPoint - eMANU_OPER_LD_TBL1;

			nCmdStage = m_CmbEndPoint.GetCurSel();
			CLoadTblCmd_MovePos* pSeqCmd = new CLoadTblCmd_MovePos;
			pSeqCmd->m_nCmdStage = nCmdStage;
			nErr = g_TaskSystemCtrl.SendCommand( g_TaskLoadTbl[ nLoadTblIdx ], pSeqCmd, eEqpCtrlMode_Manual );

			CString strMovePoint = ( nCmdStage == LOC_IDX_TBL_LD_SIDE ? _T( "Load Position" ) : _T( "Test Position" ) );

			if (nCmdStage == LOC_IDX_TBL_LD_SIDE) {
				strMovePoint = _T("Load Position");
			}
			else {
				strMovePoint.Format(_T("Test Position %d"), nLoadTblIdx + 1);
			}

			if( nErr == ERR_NO_ERROR ) {
				MakeLog( "%s Move Point: %s", strStartPoint, strMovePoint );
			}
		}break;
	case eMANU_OPER_TRAY_PP1_LD:
		{
		int nTrayPPIdx = m_nStartPoint - eMANU_OPER_TRAY_PP1_LD;
		nCmdStage = g_TaskTrayLoadPP.TrayHandManuLocIdxConvertor(RIDX_TRAY_PP_1, m_CmbEndPoint.GetCurSel());
		if (m_nSelectAction == DEF_PICK) { nCmdPickPlace = DEF_PICK; }
		else if (m_nSelectAction == DEF_PLACE) { nCmdPickPlace = DEF_PLACE; }

		CString strSelectAction = _T("");
		CString strSelectPicker = _T("");
		char szTempPicker[16] = { 0, };
		switch (m_nSelectAction)
		{
		case DEF_PICK: strSelectAction = _T("Move Pick"); break;
		case DEF_PLACE: strSelectAction = _T("Move Place"); break;
		default: strSelectAction = _T("Move Pos"); break;
		}

		CString strTargetX = _T(""), strTargetY = _T("");
		GetDlgItemText(IDC_EDIT_POS_X, strTargetX);
		GetDlgItemText(IDC_EDIT_POS_Y, strTargetY);
		int nTargetX = atoi(strTargetX) - 1;
		int nTargetY = atoi(strTargetY) - 1;

		for (int nY = 0; nY < MAX_TRAY_PP_CNT_Y; nY++) {
			for (int nX = 0; nX < MAX_TRAY_PP_CNT_X; nX++) {
				if (nUseHand[nY][nX] == TRUE) {
					vPick.push_back(CPoint(nX, nY));
					sprintf_s(szTempPicker, sizeof(szTempPicker), "[%d-%d] ", nY + 1, nX + 1);
					strSelectPicker += szTempPicker;
				}
			}
		}

		double tray_pitch_y_um = g_DMCont.m_dmTrayPP.GN(NDM5_Tray_Pitch_Y);
		int nY_PitchType = 1;
		if (tray_pitch_y_um < DEF_TRAY_PP_MAX_Y_PITCH_NARROW)
		{
			nY_PitchType = 2;
		}

		// 1호기 Feeder 개선으로 주석 처리
		//  [4/23/2025 donghyun.shin]
// 		int nEQPNo = g_DMCont.m_dmEQP.GN(NDM11_Handler_EQP_No);
// 		if (nEQPNo > DEF_EQUIPMENT_UNIT1)
// 		{
			//Y열 첫번째 Picker 있으면, Feeder Step 입력 값이랑 동일.
			BOOL bChk = FALSE;
			for (int nX = 0; nX < MAX_TRAY_PP_CNT_X; nX++) {
				if (nUseHand[0][nX] == TRUE){
					bChk = TRUE;
					break;
				}
			}	
			
			if (bChk == FALSE) 
			{
				nTargetY -= nY_PitchType;
			}
//		}



		if (m_CmbEndPoint.GetCurSel() == 0) {
			CBaseSeqCmd::eSeqCmdStatus feeder_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayFeeder.GetSeqCmdStatus();
			int nFeederPos = g_TaskTrayFeeder.ChkMotorPos(CTaskTrayFeeder::eAxisY, LOC_IDX_FEEDER_LOAD_PICK_START_POS, nTargetY);

			if (feeder_cmd_status == CBaseSeqCmd::eSeqCmdStatus_Ready && nFeederPos != ERR_NO_ERROR) {
				CTrayFeederCmd_MovePos* pFeederSeqCmd = new CTrayFeederCmd_MovePos();
				pFeederSeqCmd->m_nCmdStage = eLocIdxFeeder::LOC_IDX_FEEDER_LOAD_PICK_START_POS;
				pFeederSeqCmd->m_nTrayRowStep = nTargetY;
				nErr = g_TaskSystemCtrl.SendCommand(g_TaskTrayFeeder, pFeederSeqCmd, eEqpCtrlMode_Manual);


				m_bChkTime = FALSE;
				//Feeder와 Tray 연동하여 Move.
				m_nCmdStage_auto = nCmdStage;
				m_vPicker_auto = vPick;
				SetTimer(0, 500, NULL);
				break;
			}
		}


		if (m_nSelectAction == DEF_PICK || m_nSelectAction == DEF_PLACE)
		{
// 			if (m_CmbEndPoint.GetCurSel() == 0) {
// 				CBaseSeqCmd::eSeqCmdStatus feeder_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayFeeder.GetSeqCmdStatus();
// 				int nFeederPos = g_TaskTrayFeeder.ChkMotorPos(CTaskTrayFeeder::eAxisY, LOC_IDX_FEEDER_LOAD_PICK_START_POS, nTargetY);
// 
// 				if (feeder_cmd_status == CBaseSeqCmd::eSeqCmdStatus_Ready && nFeederPos != ERR_NO_ERROR) {
// 					CTrayFeederCmd_MovePos* pFeederSeqCmd = new CTrayFeederCmd_MovePos();
// 					pFeederSeqCmd->m_nCmdStage = eLocIdxFeeder::LOC_IDX_FEEDER_LOAD_PICK_START_POS;
// 					pFeederSeqCmd->m_nTrayRowStep = nTargetY;
// 					nErr = g_TaskSystemCtrl.SendCommand(g_TaskTrayFeeder, pFeederSeqCmd, eEqpCtrlMode_Manual);
// 
// 
// 					m_bChkTime = FALSE;
// 					//Feeder와 Tray 연동하여 Move.
// 					m_nCmdStage_auto = nCmdStage;
// 					m_vPicker_auto = vPick;
// 					SetTimer(0, 500, NULL);
// 					break;
// 				}
// 			}

			CTrayLoadPPCmd_MovePickPlace* pSeqCmd = new CTrayLoadPPCmd_MovePickPlace;
			static_cast<CTrayLoadPPCmd_MovePickPlace*>(pSeqCmd)->eStage = (eLocIdxTrayPP)nCmdStage;
			static_cast<CTrayLoadPPCmd_MovePickPlace*>(pSeqCmd)->vPicker = vPick;
			static_cast<CTrayLoadPPCmd_MovePickPlace*>(pSeqCmd)->Pocket = CPoint(nTargetX, nTargetY);
			static_cast<CTrayLoadPPCmd_MovePickPlace*>(pSeqCmd)->ePickPlace = (ePPCmd)nCmdPickPlace;
			nErr = g_TaskSystemCtrl.SendCommand(g_TaskTrayLoadPP, pSeqCmd, eEqpCtrlMode_Manual);
		}
		else {
			CTrayLoadPPCmd_MovePos* pSeqCmd = new CTrayLoadPPCmd_MovePos;
			static_cast<CTrayLoadPPCmd_MovePos*>(pSeqCmd)->eStage = (eLocIdxTrayPP)nCmdStage;
			static_cast<CTrayLoadPPCmd_MovePos*>(pSeqCmd)->vPicker = vPick;
			static_cast<CTrayLoadPPCmd_MovePos*>(pSeqCmd)->Pocket = CPoint(nTargetX, nTargetY);
			nErr = g_TaskSystemCtrl.SendCommand(g_TaskTrayLoadPP, pSeqCmd, eEqpCtrlMode_Manual);
		}

		if (nErr == ERR_NO_ERROR) {
			MakeLog("Tray PP %s  CmdStage: %s X Pos: %d Y Pos: %d Using picker : %s ", strSelectAction, g_TaskTrayLoadPP.GetLocName((eLocIdxTrayPP)nCmdStage), nTargetX + 1, nTargetY + 1, strSelectPicker);
		}
		}break;
	case eMANU_OPER_TRAY_PP2_UD:
		{
			int nTrayPPIdx = m_nStartPoint - eMANU_OPER_TRAY_PP1_LD;
			nCmdStage = g_TaskTrayUnloadPP.TrayHandManuLocIdxConvertor(RIDX_TRAY_PP_2, m_CmbEndPoint.GetCurSel());
			if( m_nSelectAction == DEF_PICK ) { nCmdPickPlace = DEF_PICK; }
			else if( m_nSelectAction == DEF_PLACE ) { nCmdPickPlace = DEF_PLACE; }

			CString strSelectAction = _T( "" );
			CString strSelectPicker = _T( "" );
			char szTempPicker[ 16 ] = { 0, };
			switch( m_nSelectAction )
			{
			case DEF_PICK: strSelectAction = _T( "Move Pick" ); break;
			case DEF_PLACE: strSelectAction = _T( "Move Place" ); break;
			default: strSelectAction = _T( "Move Pos" ); break;
			}

			CString strTargetX = _T( "" ), strTargetY = _T( "" );
			GetDlgItemText( IDC_EDIT_POS_X, strTargetX );
			GetDlgItemText( IDC_EDIT_POS_Y, strTargetY );
			int nTargetX = atoi( strTargetX ) - 1;
			int nTargetY = atoi( strTargetY ) - 1;

			for( int nY = 0; nY < MAX_TRAY_PP_CNT_Y; nY++ ) {
				for( int nX = 0; nX < MAX_TRAY_PP_CNT_X; nX++ ) {
					if( nUseHand[ nY ][ nX ] == TRUE ) {
						vPick.push_back( CPoint( nX, nY ) );
						sprintf_s( szTempPicker, sizeof( szTempPicker ), "[%d-%d] ", nY + 1, nX + 1 );
						strSelectPicker += szTempPicker;
					}
				}
			}

			if( m_nSelectAction == DEF_PICK || m_nSelectAction == DEF_PLACE )
			{
				CTrayUnloadPPCmd_MovePickPlace* pSeqCmd = new CTrayUnloadPPCmd_MovePickPlace;
				pSeqCmd->m_Cmd.eStage     = (eLocIdxTrayPP)nCmdStage;
				pSeqCmd->m_Cmd.Pocket     = CPoint( nTargetX, nTargetY );
				pSeqCmd->m_Cmd.vPicker    = vPick;
				pSeqCmd->m_Cmd.ePickPlace = (ePPCmd)nCmdPickPlace;
				nErr = g_TaskSystemCtrl.SendCommand( g_TaskTrayUnloadPP, pSeqCmd, eEqpCtrlMode_Manual );
			}
			else {
				CTrayUnloadPPCmd_MovePos* pSeqCmd = new CTrayUnloadPPCmd_MovePos;
				pSeqCmd->m_Cmd.eStage = (eLocIdxTrayPP)nCmdStage;
				pSeqCmd->m_Cmd.Pocket = CPoint( nTargetX, nTargetY );
				pSeqCmd->m_Cmd.vPicker = vPick;
				nErr = g_TaskSystemCtrl.SendCommand( g_TaskTrayUnloadPP, pSeqCmd, eEqpCtrlMode_Manual );
			}

			if( nErr == ERR_NO_ERROR ) {
				MakeLog( "Tray PP %s  CmdStage: %s X Pos: %d Y Pos: %d Using picker : %s ", strSelectAction, g_TaskTrayUnloadPP.GetLocName( (eLocIdxTrayPP)nCmdStage ), nTargetX + 1, nTargetY + 1, strSelectPicker );
			}
		}break;
	case eMANU_OPER_FEEDER:
		{
			nCmdStage = m_CmbEndPoint.GetCurSel();
			CString strMovePoint = _T("");

			if (m_nSelectAction == DEF_PICK || m_nSelectAction == DEF_PLACE)
			{
				nCmdStage += LOC_IDX_FEEDER_TRAY_PICK_POS;
				CTrayFeederCmd_MovePickPlace* pSeqCmd = new CTrayFeederCmd_MovePickPlace;
				pSeqCmd->m_nCmdStage = (eLocIdxFeeder)nCmdStage;
				pSeqCmd->m_nCmdPickPlace = ((eLocIdxFeeder)nCmdStage == eLocIdxFeeder::LOC_IDX_FEEDER_TRAY_PICK_POS) ? ePPCmd::ePick : ePPCmd::ePlace;
				nErr = g_TaskSystemCtrl.SendCommand(g_TaskTrayFeeder, pSeqCmd, eEqpCtrlMode_Manual);
				strMovePoint = ((eLocIdxFeeder)nCmdStage == eLocIdxFeeder::LOC_IDX_FEEDER_TRAY_PICK_POS) ? _T("Loading Site PICK") : _T("Unloading Site PLACE");
			}
			else
			{
				CTrayFeederCmd_MovePos* pSeqCmd = new CTrayFeederCmd_MovePos;
				pSeqCmd->m_nCmdStage = (eLocIdxFeeder)nCmdStage;

				if (nCmdStage == 0) {
					CString strTargetY = _T("");
					GetDlgItemText(IDC_EDIT_POS_Y, strTargetY);
					int nTargetY = atoi(strTargetY) - 1;
					pSeqCmd->m_nTrayRowStep = nTargetY;
				}

				nErr = g_TaskSystemCtrl.SendCommand(g_TaskTrayFeeder, pSeqCmd, eEqpCtrlMode_Manual);
				switch (nCmdStage)
				{

				case eLocIdxFeeder::LOC_IDX_FEEDER_LOAD_PICK_START_POS: {strMovePoint = _T("Pick Start Pos"); }break;
				case eLocIdxFeeder::LOC_IDX_FEEDER_TRAY_PICK_POS: {strMovePoint = _T("Tray Pick Pos"); }break;
				case eLocIdxFeeder::LOC_IDX_FEEDER_TRAY_PLACE_POS: {strMovePoint = _T("Tray Place Pos"); }break;
				case eLocIdxFeeder::LOC_IDX_FEEDER_SAFETY: {strMovePoint = _T("Safety Pos"); }break;
				}
			}

			if (nErr == ERR_NO_ERROR) {
				MakeLog("%s Move Point: %s", strStartPoint, strMovePoint);
			}
		}break;
	case eMANU_OPER_TRANSFER:
		{
			nCmdStage = m_CmbEndPoint.GetCurSel();

			eLocIdxTransfer stacker_number;
			switch( nCmdStage )
			{
			case 0: stacker_number = eLocIdxTransfer::LOC_IDX_TRANSFER_CONV1;	break;
			case 1: stacker_number = eLocIdxTransfer::LOC_IDX_TRANSFER_CONV2;	break;
			case 2: stacker_number = eLocIdxTransfer::LOC_IDX_TRANSFER_CONV3;	break;
			case 3: stacker_number = eLocIdxTransfer::LOC_IDX_TRANSFER_CONV4;	break;
			case 4: stacker_number = eLocIdxTransfer::LOC_IDX_TRANSFER_CONV5;	break;
			case 5: stacker_number = eLocIdxTransfer::LOC_IDX_TRANSFER_CONV6;	break;
			case 6: stacker_number = eLocIdxTransfer::LOC_IDX_TRANSFER_CONV7;	break;
			case 7: stacker_number = eLocIdxTransfer::LOC_IDX_TRANSFER_SAFETY;		break;
			}

			if( m_nSelectAction == DEF_PICK ) { nCmdPickPlace = DEF_PICK; }
			else if( m_nSelectAction == DEF_PLACE ) { nCmdPickPlace = DEF_PLACE; }

			if( m_nSelectAction == DEF_PICK || m_nSelectAction == DEF_PLACE ) {
				CTransferCmd_MovePickPlace* pSeqCmd = new CTransferCmd_MovePickPlace( nCmdStage, nCmdPickPlace );
				nErr = g_TaskSystemCtrl.SendCommand( g_TaskTransfer, pSeqCmd, eEqpCtrlMode_Manual );
			}
			else {
				CTransferCmd_MovePos* pSeqCmd = new CTransferCmd_MovePos( stacker_number );
				nErr = g_TaskSystemCtrl.SendCommand( g_TaskTransfer, pSeqCmd, eEqpCtrlMode_Manual );
			}

			if( nErr == ERR_NO_ERROR ) {
				MakeLog( "Move Transfer Pos, Action = %s", ( m_nSelectAction == DEF_PICK ? "Pick" : ( m_nSelectAction == DEF_PLACE ? "Place" : "Only Move" ) ) );
			}
		}break;
	default:
		ASSERT( FALSE );
		break;
	};

	if( nErr != ERR_NO_ERROR ) {
		g_Error.AlarmReport( nErr, AlarmData, NULL );
	}

//	OnCancel();
}

BOOL CDlgAxisActionMove::CheckUI_InterLock( int nUseHand[ eMAX_Y_PICKER ][ eMAX_X_PICKER ] )
{
	int nLocStage = m_CmbEndPoint.GetCurSel();

	int nLimitPocktX = 0, nLimitPocktY = 0;
	int nLimitPicthX = 0, nLimitPicthY = 0;

	CString strTblCntX = _T( "" ), strTblCntY = _T( "" ), strErrMsg = _T( "" );
	GetDlgItemText( IDC_EDIT_POS_X, strTblCntX );
	GetDlgItemText( IDC_EDIT_POS_Y, strTblCntY );

	BOOL bTransX = FALSE, bTransY = FALSE;
	int nXpos = GetDlgItemInt( IDC_EDIT_POS_X, &bTransX, FALSE );
	int nYpos = GetDlgItemInt( IDC_EDIT_POS_Y, &bTransY, FALSE );

	switch( m_nStartPoint )
	{
	case eMANU_OPER_TEST_PP1:
	case eMANU_OPER_TEST_PP2:
		{
			int nLocStage = ConvertTestPPCmdStage(m_CmbEndPoint.GetCurSel());

			int nTestPP_XPitch = g_DMCont.m_dmTestPP.GN( NDM4_TestPP_Pitch_X ) / 1000;
			int nTestPP_YPitch = g_DMCont.m_dmTestPP.GN( NDM4_TestPP_Pitch_Y ) / 1000;


			if( m_nSelectAction == eDlgActionExchange ) 
			{
				nLimitPocktX = g_DMCont.m_dmTestPP.GN( NDM4_TestSite_Div_X );
				nLimitPocktY = g_DMCont.m_dmTestPP.GN( NDM4_TestSite_Div_Y );
				nLimitPicthX = g_DMCont.m_dmTestPP.GN( NDM4_TestSite_Pitch_X ) / 1000;
				nLimitPicthY = g_DMCont.m_dmTestPP.GN( NDM4_TestSite_Pitch_Y ) / 1000;

				if( nYpos > nLimitPocktY || nYpos <= 0 ) {
					strErrMsg.Format( _T( "Target Pocket is over range!!! [ Y : 1 to %d ]" ), nLimitPocktY );
					AfxMessageBox( strErrMsg );
					return 1;
				}

				// Picker X Pitch Interlock
				if( nLimitPicthX != nTestPP_XPitch )
				{
					int Last_Selected_Picker_X = -1;
					int Last_Selected_Picker_Y = -1;
					for( int nY = 0; nY < MAX_TEST_PP_PKR_CNT_Y; nY++ ) {
						for( int nX = 0; nX < MAX_TEST_PP_PKR_CNT_X; nX++ ) {
							if( nUseHand[ nY ][ nX ] )
							{
								int nPocker_X_Pitch = nLimitPicthX * ( nX - Last_Selected_Picker_X );

								if( Last_Selected_Picker_X != -1 && Last_Selected_Picker_Y == nY && nTestPP_XPitch != nPocker_X_Pitch ) {
									AfxMessageBox( _T( "It doesn't fit the X Pitch." ) );
									return 1;
								}
								Last_Selected_Picker_X = nX;
								Last_Selected_Picker_Y = nY;
							}
						}
					}
				}

				int PickCnt = 0, PlaceCnt = 0;
				for( int nX = 0; nX < MAX_TEST_PP_PKR_CNT_X; nX++ ) {
					if( nUseHand[ 0 ][ nX ] ) { PickCnt++; }
					if( nUseHand[ 1 ][ nX ] ) { PlaceCnt++; }
				}
				if( PickCnt < 1 ) {
					AfxMessageBox( _T( "Selected Pick TestPP hand is Zero!!!!!!" ) );
					return 1;
				}

				if( PlaceCnt < 1 ) {
					AfxMessageBox( _T( "Selected Place TestPP hand is Zero!!!!!!" ) );
					return 1;
				}
			}
			else {
				if(nLocStage >= LOC_IDX_TEST_PP_TBL_1_LD && nLocStage <= LOC_IDX_TEST_PP_TBL_4_UD) {	// move loading table and after action
					nLimitPocktX = g_DMCont.m_dmShuttle.GN( NDM3_LdTbl_Div_X );
					nLimitPocktY = g_DMCont.m_dmShuttle.GN( NDM3_LdTbl_Div_Y );
					nLimitPicthX = g_DMCont.m_dmShuttle.GN( NDM3_LdTbl_Pitch_X ) / 1000;
					nLimitPicthY = g_DMCont.m_dmShuttle.GN( NDM3_LdTbl_Pitch_Y ) / 1000;
				}
// 				else if(nLocStage >= LOC_IDX_ART_BUFFER_1 && nLocStage <= LOC_IDX_ART_BUFFER_4) {
// 					nLimitPocktX = g_DMCont.m_dmTestPP.GN( NDM4_TestBuffTbl_Div_X );
// 					nLimitPocktY = g_DMCont.m_dmTestPP.GN( NDM4_TestBuffTbl_Div_Y );
// 					nLimitPicthX = g_DMCont.m_dmTestPP.GN( NDM4_TestBuffTbl_Pitch_X ) / 1000;
// 					nLimitPicthY = g_DMCont.m_dmTestPP.GN( NDM4_TestBuffTbl_Pitch_Y ) / 1000;
// 				}
				else if(nLocStage >= LOC_IDX_CLEAN_TBL && nLocStage <= LOC_IDX_CLEAN_TBL) {
					nLimitPocktX = g_DMCont.m_dmTestPP.GN( NDM4_TestCleanTbl_Div_X );
					nLimitPocktY = g_DMCont.m_dmTestPP.GN( NDM4_TestCleanTbl_Div_Y );
					nLimitPicthX = g_DMCont.m_dmTestPP.GN( NDM4_TestCleanTbl_Pitch_X ) / 1000;
					nLimitPicthY = g_DMCont.m_dmTestPP.GN( NDM4_TestCleanTbl_Pitch_Y ) / 1000;
				}
				else if(nLocStage >= LOC_IDX_STATION_1 && nLocStage <= LOC_IDX_STATION_4) {
					nLimitPocktX = g_DMCont.m_dmTestPP.GN( NDM4_TestSite_Div_X );
					nLimitPocktY = g_DMCont.m_dmTestPP.GN( NDM4_TestSite_Div_Y );
					nLimitPicthX = g_DMCont.m_dmTestPP.GN( NDM4_TestSite_Pitch_X ) / 1000;
					nLimitPicthY = g_DMCont.m_dmTestPP.GN( NDM4_TestSite_Pitch_Y ) / 1000;
				}
				else
				{
					nLimitPocktX = 1;
					nLimitPocktY = 1;
				}

				if( nXpos > nLimitPocktX || nYpos > nLimitPocktY || nXpos <= 0 || nYpos <= 0 ) {
					strErrMsg.Format( _T( "Target Pocket is over range!!! [ X : 1 to %d ][ Y : 1 to %d ]" ), nLimitPocktX, nLimitPocktY );
					AfxMessageBox( strErrMsg );
					return 1;
				}

				// Picker X Pitch Interlock
// 				if( nLimitPicthX != nTestPP_XPitch ) {
// 					int Last_Selected_Picker_X = -1;
// 					int Last_Selected_Picker_Y = -1;
// 					for( int nY = 0; nY < MAX_TEST_PP_PKR_CNT_Y; nY++ ) {
// 						for( int nX = 0; nX < MAX_TEST_PP_PKR_CNT_X; nX++ ) {
// 							if( nUseHand[ nY ][ nX ] )
// 							{
// 								int nPocker_X_Pitch = nLimitPicthX * ( nX - Last_Selected_Picker_X );
// 
// 								if( Last_Selected_Picker_X != -1 && Last_Selected_Picker_Y == nY && nTestPP_XPitch != nPocker_X_Pitch ) {
// 									AfxMessageBox( _T( "It doesn't fit the X Pitch." ) );
// 									return 1;
// 								}
// 								Last_Selected_Picker_X = nX;
// 								Last_Selected_Picker_Y = nY;
// 							}
// 						}
// 					}
// 				}

// 				// Picker Y Pitch Interlock
// 				if( nLimitPicthY != nTestPP_YPitch ) {
// 					int Last_Selected_Picker_X = -1;
// 					int Last_Selected_Picker_Y = -1;
// 					for( int nY = 0; nY < MAX_TEST_PP_PKR_CNT_Y; nY++ ) {
// 						for( int nX = 0; nX < MAX_TEST_PP_PKR_CNT_X; nX++ ) {
// 							if( nUseHand[ nY ][ nX ] )
// 							{
// 								int nPocker_Y_Pitch = nLimitPicthY * ( nY - Last_Selected_Picker_Y );
// 
// 								if( Last_Selected_Picker_Y != -1 && Last_Selected_Picker_Y != nY && nTestPP_YPitch != nLimitPicthY ) {
// 									AfxMessageBox( _T( "It doesn't fit the Y Pitch." ) );
// 									return 1;
// 								}
// 								Last_Selected_Picker_Y = nY;
// 							}
// 						}
// 					}
// 				}

				int nHandCnt = 0;
				for( int nY = 0; nY < MAX_TEST_PP_PKR_CNT_Y; nY++ ) {
					for( int nX = 0; nX < MAX_TEST_PP_PKR_CNT_X; nX++ ) {
						if( nUseHand[ nY ][ nX ] ) { nHandCnt++; }
					}
				}

				if( nHandCnt < 1 ) {
					AfxMessageBox( _T( "TestPP hand is Zero!!!!!!" ) );
					return 1;
				}
			}
		}break;
	case eMANU_OPER_TRAY_PP1_LD:
		{
			int TrayPP_XPitch = g_DMCont.m_dmTrayPP.GN(NDM5_TrayPP_LdTbl_Pitch_X) / 1000;
			int TrayPP_YPitch = g_DMCont.m_dmTrayPP.GN(NDM5_TrayPP_LdTbl_Pitch_Y) / 1000;

			BOOL bTransX = FALSE, bTransY = FALSE;
			int nXpos = GetDlgItemInt(IDC_EDIT_POS_X, &bTransX, FALSE);
			int nYpos = GetDlgItemInt(IDC_EDIT_POS_Y, &bTransY, FALSE);

			//case : LoadTbl#1 ~ #2
			if (nLocStage == 0) {
				double tray_pitch_y_um = g_DMCont.m_dmTrayPP.GN(NDM5_Tray_Pitch_Y);
				int nY_PitchType = 1;
				if (tray_pitch_y_um < DEF_TRAY_PP_MAX_Y_PITCH_NARROW)
				{
					nY_PitchType = 2;
				}

				nLimitPocktX = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_X);
				nLimitPocktY = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_Y); /*- (nY_PitchType + 1);*/
			}
			else if (nLocStage == 1) {
				nLimitPocktX = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_X);
				nLimitPocktY = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_Y);
				nLimitPicthX = g_DMCont.m_dmTrayPP.GN(NDM5_Tray_Pitch_X) / 1000;
				nLimitPicthY = g_DMCont.m_dmTrayPP.GN(NDM5_Tray_Pitch_Y) / 1000;
			}
			else {
				nLimitPocktX = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);
				nLimitPocktY = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);
				nLimitPicthX = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Pitch_X) / 1000;
				nLimitPicthY = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Pitch_Y) / 1000;
			}

			//32
			if (nXpos > nLimitPocktX || nYpos > nLimitPocktY || nXpos <= 0 || nYpos <= 0) {
				CString str;
				str.Format("Target Pocket is over range!!![X:1 to %d][Y:1 to %d]", nLimitPocktX, nLimitPocktY);
				AfxMessageBox(str);
				//AfxMessageBox(_T("Target Pocket is over range!!! [ X : 1 to %d ][ Y : 1 to %d ]"), nLimitPocktX, nLimitPocktY);
				return 1;
			}

			int nHandCnt = 0;
			for (int nY = 0; nY < MAX_TRAY_PP_CNT_Y; nY++) {
				for (int nX = 0; nX < MAX_TRAY_PP_CNT_X; nX++) {
					if (nUseHand[nY][nX]) { nHandCnt++; }
				}
			}

			if (nHandCnt < 1) {
				AfxMessageBox(_T("TrayPP hand is Zero!!!!!!"));
				return 1;
			}
		}break;
	case eMANU_OPER_TRAY_PP2_UD:
		{
			int TrayPP_XPitch = g_DMCont.m_dmTrayPP.GN( NDM5_TrayPP_LdTbl_Pitch_X ) / 1000;
			int TrayPP_YPitch = g_DMCont.m_dmTrayPP.GN( NDM5_TrayPP_LdTbl_Pitch_Y ) / 1000;

			BOOL bTransX = FALSE, bTransY = FALSE;
			int nXpos = GetDlgItemInt( IDC_EDIT_POS_X, &bTransX, FALSE );
			int nYpos = GetDlgItemInt( IDC_EDIT_POS_Y, &bTransY, FALSE );

			//case : LoadTbl#1 ~ #2
			if( nLocStage > 3 ) { // Table
				nLimitPocktX = g_DMCont.m_dmShuttle.GN( NDM3_LdTbl_Div_X );
				nLimitPocktY = g_DMCont.m_dmShuttle.GN( NDM3_LdTbl_Div_Y );
				nLimitPicthX = g_DMCont.m_dmShuttle.GN( NDM3_LdTbl_Pitch_X ) / 1000;
				nLimitPicthY = g_DMCont.m_dmShuttle.GN( NDM3_LdTbl_Pitch_Y ) / 1000;
			}
			else {
				nLimitPocktX = g_DMCont.m_dmTrayPP.GN( NDM5_TrayDiv_X );
				nLimitPocktY = g_DMCont.m_dmTrayPP.GN( NDM5_TrayDiv_Y );
				nLimitPicthX = g_DMCont.m_dmTrayPP.GN( NDM5_Tray_Pitch_X ) / 1000;
				nLimitPicthY = g_DMCont.m_dmTrayPP.GN( NDM5_Tray_Pitch_Y ) / 1000;
			}

			if( nXpos > nLimitPocktX || nYpos > nLimitPocktY || nXpos <= 0 || nYpos <= 0 ) {
				CString str;
				str.Format("Target Pocket is over range!!![X:1 to %d][Y:1 to %d]", nLimitPocktX, nLimitPocktY);
				AfxMessageBox(str);
				//AfxMessageBox( _T( "Target Pocket is over range!!! [ X : 1 to %d ][ Y : 1 to %d ]" ), nLimitPocktX, nLimitPocktY );
				return 1;
			}

			int nHandCnt = 0;
			for( int nY = 0; nY < MAX_TRAY_PP_CNT_Y; nY++ ) {
				for( int nX = 0; nX < MAX_TRAY_PP_CNT_X; nX++ ) {
					if( nUseHand[ nY ][ nX ] ) { nHandCnt++; }
				}
			}

			if( nHandCnt < 1 ) {
				AfxMessageBox( _T( "TrayPP hand is Zero!!!!!!" ) );
				return 1;
			}

		}break;
	case eMANU_OPER_PRESS_DN_1:
	case eMANU_OPER_PRESS_DN_2:
	case eMANU_OPER_PRESS_DN_3:
	case eMANU_OPER_PRESS_DN_4:
	case eMANU_OPER_PRESS_DN_5:
	case eMANU_OPER_PRESS_DN_6:
	case eMANU_OPER_PRESS_DN_7:
	case eMANU_OPER_PRESS_DN_8:
	case eMANU_OPER_LD_TBL1:
	case eMANU_OPER_LD_TBL2:
	case eMANU_OPER_LD_TBL3:
	case eMANU_OPER_LD_TBL4:
	case eMANU_OPER_TRANSFER:
		{}break;
	default:
		break;
	};

	return ERR_NO_ERROR;
}

CPoint CDlgAxisActionMove::Chk_Select( int _nUseHand[ eMAX_Y_PICKER ][ eMAX_X_PICKER ] )
{
	for( int nY = 0; nY < eMAX_Y_PICKER; nY++ ) {
		for( int nX = 0; nX < eMAX_X_PICKER; nX++ ) {
			if( _nUseHand[ nY ][ nX ] == TRUE )
			{
				return CPoint( nX, nY );
			}
		}
	}
	return CPoint( 0, 0 );
}

void CDlgAxisActionMove::OnBnClickedButtonSetLoad()
{
	switch( m_nStartPoint )
	{
	case eMANU_OPER_TEST_PP1:
	case eMANU_OPER_TEST_PP2:
		{
			m_ChkHand2_1.SetCheck( TRUE );
			m_ChkHand2_2.SetCheck( TRUE );
			m_ChkHand2_3.SetCheck( TRUE );
			m_ChkHand2_4.SetCheck( TRUE );
		}break;
	default:
		{
			m_ChkHand1_1.SetCheck( TRUE );
			m_ChkHand1_3.SetCheck( TRUE );
			m_ChkHand2_1.SetCheck( TRUE );
			m_ChkHand2_3.SetCheck( TRUE );
		}break;
	}
}


void CDlgAxisActionMove::OnBnClickedButtonSetUnload()
{
	switch( m_nStartPoint )
	{
	case eMANU_OPER_TEST_PP1:
	case eMANU_OPER_TEST_PP2:
		{
			m_ChkHand1_1.SetCheck( TRUE );
			m_ChkHand1_2.SetCheck( TRUE );
			m_ChkHand1_3.SetCheck( TRUE );
			m_ChkHand1_4.SetCheck( TRUE );
		}break;
	default:
		{
			m_ChkHand1_2.SetCheck( TRUE );
			m_ChkHand1_4.SetCheck( TRUE );
			m_ChkHand2_2.SetCheck( TRUE );
			m_ChkHand2_4.SetCheck( TRUE );
		}break;
	}
}


void CDlgAxisActionMove::OnBnClickedButtonSetClear()
{
	m_ChkHand1_1.SetCheck( FALSE );
	m_ChkHand1_2.SetCheck( FALSE );
	m_ChkHand1_3.SetCheck( FALSE );
	m_ChkHand1_4.SetCheck( FALSE );
	m_ChkHand1_5.SetCheck( FALSE );
	m_ChkHand1_6.SetCheck( FALSE );
	m_ChkHand1_7.SetCheck( FALSE );
	m_ChkHand1_8.SetCheck( FALSE );
	m_ChkHand2_1.SetCheck( FALSE );
	m_ChkHand2_2.SetCheck( FALSE );
	m_ChkHand2_3.SetCheck( FALSE );
	m_ChkHand2_4.SetCheck( FALSE );
	m_ChkHand2_5.SetCheck( FALSE );
	m_ChkHand2_6.SetCheck( FALSE );
	m_ChkHand2_7.SetCheck( FALSE );
	m_ChkHand2_8.SetCheck( FALSE );
}

void CDlgAxisActionMove::MakeLog( LPCTSTR fmt, ... )
{
	TCHAR tmpMsg[ 1024 ] = { 0, };
	if( fmt )
	{
		va_list argList;
		va_start( argList, fmt );
		_vstprintf_s( tmpMsg, fmt, argList );
		va_end( argList );
	}
	TWLOG.MakeLog( static_cast< int >( eLogID_SYSTEM ), Debug, "", __LINE__, NULL, tmpMsg );
}

void CDlgAxisActionMove::OnEnSetfocusEditDelayTime()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	double dDelayTime = 0.0;
	sprintf_s(szVal, sizeof(szVal), "%.1f", m_dDelayTime);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Delay Time", "9999", "0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_DELAY_TIME)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
	dDelayTime = atof(szRetVal);
	g_DMCont.m_dmHandlerOpt.SD(DDM11_TESTPP_DELAY_TIME, dDelayTime);
	
}

void CDlgAxisActionMove::OnRadioSeletTestHandType(UINT id)
{
	UpdateData(TRUE);
	switch (id)
	{
	case IDC_RADIO_SEL_LOAD_HAND:
	{
		m_nSelectHandType = eTestPPLoadType;
		GetDlgItem(IDC_STATIC_TEXT121)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RADIO_SEL_DEVICE_NORMAL)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RADIO_SEL_DEVICE_CLEAN)->ShowWindow(SW_SHOW);
	}break;
	case IDC_RADIO_SEL_UNLOAD_HAND:
	{
		m_nSelectHandType = eTestPPUnLoadType;
		GetDlgItem(IDC_STATIC_TEXT121)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_SEL_DEVICE_NORMAL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_SEL_DEVICE_CLEAN)->ShowWindow(SW_HIDE);
	}break;
	default:
		break;
	}

	OnUpdateEndPointCombo();
	m_CmbEndPoint.SetCurSel(0);
}


void CDlgAxisActionMove::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent)
	{
	case 0:
	{
		KillTimer(nIDEvent);

		CString strTargetX = _T(""), strTargetY = _T("");
		GetDlgItemText(IDC_EDIT_POS_X, strTargetX);
		GetDlgItemText(IDC_EDIT_POS_Y, strTargetY);
		int nTargetX = atoi(strTargetX) - 1;
		int nTargetY = atoi(strTargetY) - 1;


		CBaseSeqCmd::eSeqCmdStatus feeder_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayFeeder.GetSeqCmdStatus();
		int nFeederPos = g_TaskTrayFeeder.ChkMotorPos(CTaskTrayFeeder::eAxisY, LOC_IDX_FEEDER_LOAD_PICK_START_POS, nTargetY);
		if (feeder_cmd_status == CBaseSeqCmd::eSeqCmdStatus_Ready && nFeederPos == ERR_NO_ERROR) {

			int nErr = ERR_NO_ERROR;
			if (m_nSelectAction == DEF_PICK || m_nSelectAction == DEF_PLACE)
			{
				CTrayLoadPPCmd_MovePickPlace* pSeqCmd = new CTrayLoadPPCmd_MovePickPlace;
				static_cast<CTrayLoadPPCmd_MovePickPlace*>(pSeqCmd)->eStage = (eLocIdxTrayPP)m_nCmdStage_auto;
				static_cast<CTrayLoadPPCmd_MovePickPlace*>(pSeqCmd)->vPicker = m_vPicker_auto;
				static_cast<CTrayLoadPPCmd_MovePickPlace*>(pSeqCmd)->Pocket = CPoint(nTargetX, nTargetY);
				static_cast<CTrayLoadPPCmd_MovePickPlace*>(pSeqCmd)->ePickPlace = (ePPCmd)m_nSelectAction;

				nErr = g_TaskSystemCtrl.SendCommand(g_TaskTrayLoadPP, pSeqCmd, eEqpCtrlMode_Manual);
			}
			else
			{
				CTrayLoadPPCmd_MovePos* pSeqCmd = new CTrayLoadPPCmd_MovePos;
				static_cast<CTrayLoadPPCmd_MovePos*>(pSeqCmd)->eStage = (eLocIdxTrayPP)m_nCmdStage_auto;
				static_cast<CTrayLoadPPCmd_MovePos*>(pSeqCmd)->vPicker = m_vPicker_auto;
				static_cast<CTrayLoadPPCmd_MovePos*>(pSeqCmd)->Pocket = CPoint(nTargetX, nTargetY);
				nErr = g_TaskSystemCtrl.SendCommand(g_TaskTrayLoadPP, pSeqCmd, eEqpCtrlMode_Manual);
			}


			//int nErr = g_TaskSystemCtrl.SendCommand(g_TaskTrayLoadPP, pSeqCmd, eEqpCtrlMode_Manual);

			if (nErr == ERR_NO_ERROR) {
				CString strSelectAction = _T("");
				CString strSelectPicker = _T("");

				switch (m_nSelectAction) {
				case DEF_PICK: strSelectAction = _T("Move Pick"); break;
				case DEF_PLACE: strSelectAction = _T("Move Place"); break;
				case DEF_EXCH: strSelectAction = _T("Exchage"); break;
				default: strSelectAction = _T("Move Pos"); break;
				}
				MakeLog("Tray PP %s  CmdStage: %s X Pos: %d Y Pos: %d", strSelectAction, g_TaskTrayLoadPP.GetLocName((eLocIdxTrayPP)m_nCmdStage_auto), nTargetX + 1, nTargetY + 1);
 			}
		}
		else {
			//check time over
			if (m_bChkTime == FALSE) {
				m_bChkTime = TRUE;
				m_tcTimeout = (double)GetTickCount64();//check time picker up
			}

			double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
			if (dSpanTime > (TIME_OUT_SHORT * 1000.0)) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Tray PP waiting for  %s time over", g_TaskTrayLoadPP.GetLocName((eLocIdxTrayPP)m_nCmdStage_auto));
				g_Error.AlarmReport(ERR_TRAY_PP_1_MOTOR_MOVE_TIMEOUT, AlarmData, NULL);
				break;
			}


			SetTimer(0, 500, NULL);
		}
	}break;
	case 1: //feeder row step 확인.
	{
		KillTimer(nIDEvent);
		int nCmdStage = m_CmbEndPoint.GetCurSel();
		if (nCmdStage == 0) {
			SetDlgItemText(IDC_STATIC_TEXT47, _T("Step : "));
			GetDlgItem(IDC_STATIC_TEXT47)->MoveWindow(96, 120, 100, 100);
			GetDlgItem(IDC_EDIT_POS_Y)->MoveWindow(200, 118, 60, 23);

			GetDlgItem(IDC_STATIC_TEXT47)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_POS_Y)->ShowWindow(SW_SHOW);
		}
		else {
			GetDlgItem(IDC_STATIC_TEXT47)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_POS_Y)->ShowWindow(SW_HIDE);
		}

		SetTimer(1, 200, NULL);
	}break;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CDlgAxisActionMove::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}


void CDlgAxisActionMove::OnBnClickedRadioSelLoadHand()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

int CDlgAxisActionMove::ConvertTestPPCmdStage(int nEndPointIdx)
{
	/*
	(combo 0)LOC_IDX_STATION_1,
	(combo 1)LOC_IDX_STATION_2,
	(combo 2)LOC_IDX_STATION_3,
	(combo 3)LOC_IDX_STATION_4,

	()LOC_IDX_TEST_PP_TBL_1_LD,				==> 사용 안함.
	()LOC_IDX_TEST_PP_TBL_2_LD,				==> 사용 안함.
	(combo 4)LOC_IDX_TEST_PP_TBL_3_LD,		==> Buffer로 사용.
	()LOC_IDX_TEST_PP_TBL_4_LD,				==> 사용 안함.

	(combo 5)LOC_IDX_TEST_PP_TBL_1_UD,
	(combo 6)LOC_IDX_TEST_PP_TBL_2_UD,
	(combo 7)LOC_IDX_TEST_PP_TBL_3_UD,
	(combo 8)LOC_IDX_TEST_PP_TBL_4_UD,

	()LOC_IDX_CLEAN_TBL,					==> 사용 안함.

	(combo 9)LOC_IDX_TEST_PP_HOME,
	
	*/

	int nRetCmdStage = -1;

	//Load Type은 Combo Idx와 CmdStage 동일
	if (m_nSelectHandType == eTestPPLoadType)
	{
		nRetCmdStage = nEndPointIdx;
	}
	else
	{
		switch (nEndPointIdx)
		{
		case 4: nRetCmdStage = LOC_IDX_TEST_PP_TBL_3_LD; break;
		case 5:	nRetCmdStage = LOC_IDX_TEST_PP_TBL_1_UD; break;
		case 6: nRetCmdStage = LOC_IDX_TEST_PP_TBL_2_UD; break;
		case 7:	nRetCmdStage = LOC_IDX_TEST_PP_TBL_3_UD; break;
		case 8: nRetCmdStage = LOC_IDX_TEST_PP_TBL_4_UD; break;
		case 9: nRetCmdStage = LOC_IDX_TEST_PP_HOME;	 break;
		default: nRetCmdStage = nEndPointIdx;			 break;
		}
	}
	return nRetCmdStage;
}

void CDlgAxisActionMove::OnCbnSelchangeComboEndMovePos()
{
	int nCmdEndPoint = m_CmbEndPoint.GetCurSel();
	switch (m_nStartPoint)
	{
		case eMANU_OPER_TEST_PP1:
		case eMANU_OPER_TEST_PP2:
		{
			if (m_nSelectHandType == eTestPPLoadType && m_nSelectAction != eDlgActionOnlyMove && nCmdEndPoint >= LOC_IDX_STATION_1 && nCmdEndPoint <= LOC_IDX_STATION_4)
			{
				GetDlgItem(IDC_STATIC_TEXT121)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_RADIO_SEL_DEVICE_NORMAL)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_RADIO_SEL_DEVICE_CLEAN)->ShowWindow(SW_SHOW);
			}
			else {
				GetDlgItem(IDC_STATIC_TEXT121)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RADIO_SEL_DEVICE_NORMAL)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RADIO_SEL_DEVICE_CLEAN)->ShowWindow(SW_HIDE);
			}
		}break;
	}
}


void CDlgAxisActionMove::OnBnClickedRadioSelDeviceNormal(UINT id)
{
	UpdateData(TRUE);
	switch (id)
	{
	case IDC_RADIO_SEL_DEVICE_NORMAL:
	{
		m_nSelectDevice = eCleanOffsetNotUse;
	}break;
	case IDC_RADIO_SEL_DEVICE_CLEAN:
	{
		m_nSelectDevice = eCleanOffsetUse;
	}break;
	default:
		break;
	}
}
