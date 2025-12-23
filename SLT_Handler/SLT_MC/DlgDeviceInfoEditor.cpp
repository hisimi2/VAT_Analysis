// DlgDeviceInfoEditor.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgDeviceInfoEditor.h"
#include "afxdialogex.h"


// CDlgDeviceInfoEditor 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgDeviceInfoEditor, CDialogEx)

CDlgDeviceInfoEditor::CDlgDeviceInfoEditor(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_DEVICE_INFO_EDITOR, pParent)
{
	::ZeroMemory( &m_device_data, sizeof( m_device_data ) );
	m_device_stage = 0;
	m_x = 0;
	m_y = 0;
}

CDlgDeviceInfoEditor::CDlgDeviceInfoEditor( int _device_stage, int _x, int _y )
	: CDialogEx( IDD_DLG_DEVICE_INFO_EDITOR, NULL )
{
	m_device_stage = _device_stage;
	m_x = _x;
	m_y = _y;
	g_DMCont.m_dmDevice.GetDeviceData( _device_stage, _x, _y, &m_device_data );
}

CDlgDeviceInfoEditor::~CDlgDeviceInfoEditor()
{
}

void CDlgDeviceInfoEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_COMBO_DEVICE_INFO_STATION_SELECT, m_combo_station_select );
	DDX_Control( pDX, IDC_FPSPREAD_DEVICE_INFO_TEST_CNT, m_spread_test_count );
	DDX_Control( pDX, IDC_CHECK_DEVICE_INFO_EXIST, m_checkbox_exist );
	DDX_Control( pDX, IDC_EDIT_DEVICE_INFO_BIN_CODE, m_edit_bin_code );
	DDX_Control( pDX, IDC_EDIT_DEVICE_INFO_MAX_TEST_CNT, m_edit_max_test_cnt );
	DDX_Control( pDX, IDC_EDIT_DEVICE_INFO_TOTAL_STATION_CNT, m_edit_total_station_cnt );
	DDX_Control( pDX, IDC_EDIT_DEVICE_INFO_TEST_CNT, m_edit_test_cnt );
}


BEGIN_MESSAGE_MAP(CDlgDeviceInfoEditor, CDialogEx)
	ON_BN_CLICKED( IDOK, &CDlgDeviceInfoEditor::OnBnClickedOk )
	ON_CBN_SELCHANGE( IDC_COMBO_DEVICE_INFO_STATION_SELECT, &CDlgDeviceInfoEditor::OnCbnSelchangeComboDeviceInfoStationSelect )
	ON_BN_CLICKED( IDC_BUTTON_DEVICE_INFO_SET_ALL_STATION_TEST_CNT, &CDlgDeviceInfoEditor::OnBnClickedButtonDeviceInfoSetAllStationTestCnt )
	ON_BN_CLICKED( IDC_BUTTON_DEVICE_INFO_SET_LEFT_MODULE_TEST_CNT, &CDlgDeviceInfoEditor::OnBnClickedButtonDeviceInfoSetLeftModuleTestCnt )
	ON_BN_CLICKED( IDC_BUTTON_DEVICE_INFO_SET_RIGHT_MODULE_TEST_CNT, &CDlgDeviceInfoEditor::OnBnClickedButtonDeviceInfoSetRightModuleTestCnt )
END_MESSAGE_MAP()


// CDlgDeviceInfoEditor 메시지 처리기입니다.


BOOL CDlgDeviceInfoEditor::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_spread_test_count.SetMaxCols( STATION_MAX_X_SITE );
	m_spread_test_count.SetMaxRows( STATION_MAX_Y_SITE );

	CString station_name;
	for( int idx = 0; idx < eMaxPressUnitCount; idx++ ) {
		station_name.Format( _T( "STATION%d" ), idx+1 );
		m_combo_station_select.AddString( station_name );
	}
	m_combo_station_select.SetCurSel( 0 );
	ShowTestCount( 0 );

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	int exist = m_device_data.sExist;
	m_checkbox_exist.SetCheck( exist );

	m_edit_bin_code.SetWindowText( m_device_data.szBinResult );

	CString str;
	str.Format( _T( "%d" ), m_device_data.stGrrTestInfo[ 0 ].nMaxTestCnt );
	m_edit_max_test_cnt.SetWindowText( str );

	str.Format( _T( "%d" ), m_device_data.nTotalStationCnt );
	m_edit_total_station_cnt.SetWindowText( str );



	UpdateData();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgDeviceInfoEditor::ShowTestCount( int _station_num ) 
{
	CString str;
	for( int row = 0; row < STATION_MAX_Y_SITE; row++ ) {
		for( int col = 0; col < STATION_MAX_X_SITE; col++ ) {
			int test_cnt = m_device_data.stGrrTestInfo[ _station_num ].nTestCnt[ col ][ row ];
			str.Format( _T( "%d" ), test_cnt );
			m_spread_test_count.SetTextMatrix( row + 1, col + 1, str );
		}
	}
}


void CDlgDeviceInfoEditor::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.


	int exist = m_checkbox_exist.GetCheck();
	m_device_data.sExist = exist;

	CString str;
	m_edit_bin_code.GetWindowText( str );
	StringCchCopy( m_device_data.szBinResult, _countof( m_device_data.szBinResult ), str );


	m_edit_total_station_cnt.GetWindowText( str );
	m_device_data.nTotalStationCnt = StrToInt( str );


	for( int i = 0; i < eMaxPressUnitCount; i++ ) {
		m_edit_max_test_cnt.GetWindowText( str );
		m_device_data.stGrrTestInfo[ i ].nMaxTestCnt = StrToInt( str );

		m_device_data.stGrrTestInfo[ i ].nSiteCntX = STATION_MAX_X_SITE;
		m_device_data.stGrrTestInfo[ i ].nSiteCntY = STATION_MAX_Y_SITE;
	}


	g_DMCont.m_dmDevice.SetDeviceData( m_device_stage, m_x, m_y, &m_device_data );
	CDialogEx::OnOK();
}
BEGIN_EVENTSINK_MAP( CDlgDeviceInfoEditor, CDialogEx )
	ON_EVENT( CDlgDeviceInfoEditor, IDC_FPSPREAD_DEVICE_INFO_TEST_CNT, 4, CDlgDeviceInfoEditor::ChangeFpspreadDeviceInfoTestCnt, VTS_I4 VTS_I4 )
	ON_EVENT( CDlgDeviceInfoEditor, IDC_FPSPREAD_DEVICE_INFO_TEST_CNT, DISPID_KEYDOWN, CDlgDeviceInfoEditor::KeyDownFpspreadDeviceInfoTestCnt, VTS_PI2 VTS_I2 )
END_EVENTSINK_MAP()


void CDlgDeviceInfoEditor::ChangeFpspreadDeviceInfoTestCnt( long Col, long Row )
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CDlgDeviceInfoEditor::OnCbnSelchangeComboDeviceInfoStationSelect()
{
	int station_num = m_combo_station_select.GetCurSel();
	ShowTestCount( station_num );
}

void CDlgDeviceInfoEditor::KeyDownFpspreadDeviceInfoTestCnt( short* KeyCode, short Shift )
{
	int col_start = m_spread_test_count.GetSelBlockCol();
	int col_end = m_spread_test_count.GetSelBlockCol2();
	int row_start = m_spread_test_count.GetSelBlockRow();
	int row_end = m_spread_test_count.GetSelBlockRow2();


	int station_num = m_combo_station_select.GetCurSel();
	for( int row = row_start; row <= row_end; row++ ) {
		for( int col = col_start; col <= col_end; col++ ) {
			CString val = CString( char(*KeyCode) );
			m_spread_test_count.SetTextMatrix( row, col, val );
			m_device_data.stGrrTestInfo[ station_num ].nTestCnt[ col-1 ][ row-1 ] = ( BYTE )StrToInt( val );
		}
	}
}

void CDlgDeviceInfoEditor::OnBnClickedButtonDeviceInfoSetAllStationTestCnt()
{
	for( int station_num = 0; station_num < eMaxPressUnitCount; station_num++ ) {
		for( int row = 0; row < STATION_MAX_Y_SITE; row++ ) {
			for( int col = 0; col < STATION_MAX_X_SITE; col++ ) {
				CString str;
				m_edit_test_cnt.GetWindowText( str );
				m_device_data.stGrrTestInfo[ station_num ].nTestCnt[ col ][ row ] = ( BYTE )StrToInt( str );
			}
		}
	}
}


void CDlgDeviceInfoEditor::OnBnClickedButtonDeviceInfoSetLeftModuleTestCnt()
{
	for each( auto station_num in g_TaskAutoTrayLoadPP.m_test_modules_press_idx[ 0 ] ) {
		for( int row = 0; row < STATION_MAX_Y_SITE; row++ ) {
			for( int col = 0; col < STATION_MAX_X_SITE; col++ ) {
				CString str;
				m_edit_test_cnt.GetWindowText( str );
				m_device_data.stGrrTestInfo[ station_num ].nTestCnt[ col ][ row ] = ( BYTE )StrToInt( str );
			}
		}
	}
}


void CDlgDeviceInfoEditor::OnBnClickedButtonDeviceInfoSetRightModuleTestCnt()
{
	for each( auto station_num in g_TaskAutoTrayLoadPP.m_test_modules_press_idx[ 1 ] ) {
		for( int row = 0; row < STATION_MAX_Y_SITE; row++ ) {
			for( int col = 0; col < STATION_MAX_X_SITE; col++ ) {
				CString str;
				m_edit_test_cnt.GetWindowText( str );
				m_device_data.stGrrTestInfo[ station_num ].nTestCnt[ col ][ row ] = ( BYTE )StrToInt( str );
			}
		}
	}
}
