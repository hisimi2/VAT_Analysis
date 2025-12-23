// VAT_ConfigDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgVATConfig.h"
#include "afxdialogex.h"


// CVAT_ConfigDlg대화 상자입니다.
#define WWHITE			RGB( 255, 255, 255	)
#define NOMAL			RGB( 221, 221, 221  )

IMPLEMENT_DYNAMIC(CDlgVATConfig, CDialogEx)

CDlgVATConfig::CDlgVATConfig(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgVATConfig::IDD, pParent)
	, m_nHandSpeedMode(0)
	, m_nVisionOKRangeMode(0)
	, m_nUseHandXMax(0)
	, m_nUseHandYMax(0)
	, m_dVisionWaitTime(0)
	, m_nApplyPickerAlignOffsetMode(0)
{
		for(int j = 0; j < MAX_TRAY_PP_CNT_Y; j++)
			for(int k = 0; k < MAX_TRAY_PP_CNT_X; k++)
				m_nSelectedPicker[j][k] = 0;
}

CDlgVATConfig::~CDlgVATConfig()
{
}

void CDlgVATConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_HAND_SPEED_50, m_nHandSpeedMode);
	DDX_Radio(pDX, IDC_RADIO_VISION_OK_SCALE, m_nVisionOKRangeMode);
	DDX_Radio( pDX, IDC_RADIO_PITCH_Y_OFFSET_OFF, m_nApplyPickerAlignOffsetMode );
	DDX_Control(pDX, IDC_BTN_SAVE, m_btnSave);
}


BEGIN_MESSAGE_MAP(CDlgVATConfig, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CDlgVATConfig::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_RADIO_VISION_OK_SCALE, &CDlgVATConfig::OnBnClickedRadioVisionOkScale)
	ON_BN_CLICKED(IDC_RADIO_VISION_OK_USER_VALUE, &CDlgVATConfig::OnBnClickedRadioVisionOkUserValue)
	ON_EN_SETFOCUS(IDC_EDIT_VISION_OK_RANGE, &CDlgVATConfig::OnEnSetfocusEditVisionOkRange)
	ON_EN_SETFOCUS(IDC_EDIT_VISION_WAIT_TIME, &CDlgVATConfig::OnEnSetfocusEditVisionWaitTime)
	ON_EN_SETFOCUS(IDC_EDIT_BASE_POS_X, &CDlgVATConfig::OnEnSetfocusEditBasePosX)
	ON_EN_SETFOCUS(IDC_EDIT_BASE_POS_Y, &CDlgVATConfig::OnEnSetfocusEditBasePosY)
END_MESSAGE_MAP()


// CVAT_ConfigDlg 메시지 처리기입니다.


BOOL CDlgVATConfig::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Load Hand Max.
	m_nUseHandXMax = MAX_TRAY_PP_CNT_X;
	m_nUseHandYMax = MAX_TRAY_PP_CNT_Y;

	// Initialize Title Panel.
	CDSSPanel *pTeachingOptionPanel = (CDSSPanel *)GetDlgItem(IDC_SSPANEL_TEACHING);
	pTeachingOptionPanel->SetBackColor(LLIGHTCYAN);

	// Initialize Radio Button.
	m_nHandSpeedMode = g_VatPkrAlign.m_stVATOption.nTeachingHandSpeed;
	m_nVisionOKRangeMode = g_VatPkrAlign.m_stVATOption.nVisionOKRangeType;
	m_nApplyPickerAlignOffsetMode = g_VatPkrAlign.m_stVATOption.nApplyPickerAlignOffset;

	UpdateData(FALSE); // variable -> control.

	CEdit *pEditVisionOKUserValue = (CEdit *)GetDlgItem(IDC_EDIT_VISION_OK_RANGE);

	CString strUserValue;
	strUserValue.Format("%.3f", g_VatPkrAlign.m_stVATOption.dVisionUserOKRange / 1000.0);

	pEditVisionOKUserValue->SetWindowText(strUserValue);

	if( m_nVisionOKRangeMode != VAT_OPTION_VISION_OK_USER_VALUE )
		pEditVisionOKUserValue->EnableWindow(FALSE);
	else
		pEditVisionOKUserValue->EnableWindow(TRUE);

	m_dVisionWaitTime = g_VatPkrAlign.m_stVATOption.dVisionWaitTime;

	ShowEditVisionWaitTime(m_dVisionWaitTime);

	int nHandX  = g_VatPkrAlign.m_stVATOption.nStdPicker[VAT_X];
	int nHandY  = g_VatPkrAlign.m_stVATOption.nStdPicker[VAT_Y];

	// By jhRyu [180827] 기준 피커 위치를 옵션화 해놨지만 현재 모든 설비의 기준피커 위치가 동일하므로
	// 기준피커 변경 못하도록 옵션 설정부분 Disable 시킴. (UI 상에 어떤 피커가 기준피커인지 표시는 됨.)
	CButton *pCheckPitchScale = (CButton *)GetDlgItem(IDC_CHECK_PITCH_SCALE_CAL_USE);
	pCheckPitchScale->SetCheck(g_VatPkrAlign.m_stVATOption.nPitchScaleFactorCalOption);

	// Base X,Y Position
	CEdit *pEditBasePosXValue = (CEdit *)GetDlgItem(IDC_EDIT_BASE_POS_X);
	CString strBasePosValue;
	strBasePosValue.Format("%.3f", g_VatPkrAlign.m_stVATInfo.dPitchJigBase[VAT_X] / 1000.0);
	pEditBasePosXValue->SetWindowText(strBasePosValue);

	CEdit *pEditBasePosYValue = (CEdit *)GetDlgItem(IDC_EDIT_BASE_POS_Y);
	strBasePosValue.Format("%.3f", g_VatPkrAlign.m_stVATInfo.dPitchJigBase[VAT_Y] / 1000.0);
	pEditBasePosYValue->SetWindowText(strBasePosValue);

	return TRUE;
}

void CDlgVATConfig::OnBnClickedBtnSave()
{
	// 0920 : "Data를 저장하시겠습니까?"
	int nRet = AfxMessageBox( "Are you Save??", MB_YESNO );
	if( nRet != IDYES )
		return;

	UpdateData(TRUE); // control -> variable.

	g_VatPkrAlign.m_stVATOption.nTeachingHandSpeed = m_nHandSpeedMode;
	g_VatPkrAlign.m_stVATOption.nVisionOKRangeType = m_nVisionOKRangeMode;


	g_VatPkrAlign.m_stVATOption.dVisionWaitTime = m_dVisionWaitTime;
	g_VatPkrAlign.m_stVATOption.nApplyPickerAlignOffset = m_nApplyPickerAlignOffsetMode;

	CString strOkUserValue;
	CEdit *pEditVisionOKUserValue = (CEdit *)GetDlgItem(IDC_EDIT_VISION_OK_RANGE);
	pEditVisionOKUserValue->GetWindowText(strOkUserValue);

	g_VatPkrAlign.m_stVATOption.dVisionUserOKRange = _ttof(strOkUserValue) * 1000.0;

	for( int i = 0; i < m_nUseHandYMax; i++ ){
		for(int j = 0; j < m_nUseHandXMax; j++ ){
			if( m_nSelectedPicker[ i ][ j ] == 1 ) {
				g_VatPkrAlign.m_stVATOption.nStdPicker[ VAT_X ] = j;
				g_VatPkrAlign.m_stVATOption.nStdPicker[ VAT_Y ] = i;
			}
		}
	}



	CButton *pCheckPitchScale = (CButton *)GetDlgItem(IDC_CHECK_PITCH_SCALE_CAL_USE);
	g_VatPkrAlign.m_stVATOption.nPitchScaleFactorCalOption = pCheckPitchScale->GetCheck();

	CString strBasePosValue;
	CEdit *pEditBasePosXValue = (CEdit *)GetDlgItem(IDC_EDIT_BASE_POS_X);
	pEditBasePosXValue->GetWindowText(strBasePosValue);
	g_VatPkrAlign.m_stVATInfo.dPitchJigBase[VAT_X] = _ttof(strBasePosValue) * 1000.0;

	CEdit *pEditBasePosYValue = (CEdit *)GetDlgItem(IDC_EDIT_BASE_POS_Y);
	pEditBasePosYValue->GetWindowText(strBasePosValue);
	g_VatPkrAlign.m_stVATInfo.dPitchJigBase[VAT_Y] = _ttof(strBasePosValue) * 1000.0;

	g_VatPkrAlign.SaveVATOption();
	g_TaskTrayPP.ApplyVATOffset();
}

void CDlgVATConfig::OnBnClickedRadioVisionOkScale()
{
	UpdateData(TRUE); // control -> variable.

	CEdit *pEditVisionOKUserValue = (CEdit *)GetDlgItem(IDC_EDIT_VISION_OK_RANGE);

	if( m_nVisionOKRangeMode != VAT_OPTION_VISION_OK_USER_VALUE )
		pEditVisionOKUserValue->EnableWindow(FALSE);
	else
		pEditVisionOKUserValue->EnableWindow(TRUE);
}

void CDlgVATConfig::OnBnClickedRadioVisionOkUserValue()
{
	UpdateData(TRUE); // control -> variable.

	CEdit *pEditVisionOKUserValue = (CEdit *)GetDlgItem(IDC_EDIT_VISION_OK_RANGE);

	if( m_nVisionOKRangeMode != VAT_OPTION_VISION_OK_USER_VALUE )
		pEditVisionOKUserValue->EnableWindow(FALSE);
	else
		pEditVisionOKUserValue->EnableWindow(TRUE);
}

void CDlgVATConfig::OnEnSetfocusEditVisionOkRange()
{
	CString strInputValue(_T(""));
	CString strCurrentValue(_T(""));
	CString strTitle(_T("Vision Ok Range"));

	CEdit *pEditVisionOKUserValue = (CEdit *)GetDlgItem(IDC_EDIT_VISION_OK_RANGE);
	pEditVisionOKUserValue->GetWindowText(strCurrentValue);

	BOOL bRet = g_LibCommCtrl.GetNumberBox(this,(LPSTR)(LPCSTR)strInputValue, 15, (LPSTR)(LPCSTR)strCurrentValue, (LPSTR)(LPCSTR)strTitle, "0.100", "0.010");
	if(bRet != TRUE)
		return;

	double dCurrentOkRange = _ttof(strInputValue);
	strInputValue.Format(_T("%.3f"), dCurrentOkRange);

	pEditVisionOKUserValue->SetWindowText(strInputValue);
}

BOOL CDlgVATConfig::PreTranslateMessage(MSG* pMsg)
{
	switch( pMsg->message )
	{
	case WM_KEYDOWN :
		{
			switch( pMsg->wParam )
			{
			case VK_ESCAPE : { pMsg->wParam = NULL; } break;
			case VK_RETURN : { pMsg->wParam = NULL; } break;
			}
		} break;
	case WM_SYSKEYDOWN :
		{
			switch( pMsg->wParam )
			{
			case VK_F4 : { pMsg->wParam = NULL; } break;
			}
		} break;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CDlgVATConfig::InitSpreadSheetPicker(CSpreadSheet &ssHand)
{
	ssHand.SetFontName(_T("Times New Roman"));
	ssHand.SetFontSize(11);
	ssHand.SetTypeHAlign(2); // Center Align
	ssHand.SetTypeVAlign(2);

	ssHand.SetColHeadersShow(FALSE);
	ssHand.SetRowHeadersShow(FALSE);

	CString strPicker[MAX_TRAY_PP_CNT_Y][ MAX_TRAY_PP_CNT_X ] = {
		{ _T("Aa"), _T("Ab"), _T("Ac"), _T("Ad") },
		{ _T("Ba"), _T("Bb"), _T("Bc"), _T("Bd") }
	};

	for(int i = 0; i < MAX_TRAY_PP_CNT_Y; i++)
	{
		for(int j = 0; j < MAX_TRAY_PP_CNT_X; j++)
		{
			int Col = 1+j;
			int Row = 1+i;

			ssHand.SetCol(Col);
			ssHand.SetRow(Row);

			if( i < m_nUseHandYMax && j < m_nUseHandXMax )
			{
				ssHand.SetCellType(7); // Button Type
				ssHand.SetTypeButtonColor(WWHITE);
				ssHand.SetTypeButtonText(strPicker[i][j]);
			}
			else
			{
				ssHand.SetCellType(7);
				ssHand.SetTypeButtonColor(NOMAL);
				ssHand.SetTypeButtonText(" ");
				//ssHand.SetBackColor(NOMAL);
				//ssHand.SetTextMatrix(Row, Col, " ");
			}

			ssHand.SetTypeHAlign(2); // Center Align
			ssHand.SetTypeVAlign(2);
		}
	}

	for(int i = 1; i <= ssHand.GetMaxCols(); i++)
	{
		ssHand.SetColWidth(i, 4.2);
	}

	for(int i = 1; i <= ssHand.GetMaxRows(); i++)
	{
		ssHand.SetRowHeight(i, 35);
	}

	ssHand.SetAutoSize(TRUE);
}

BEGIN_EVENTSINK_MAP(CDlgVATConfig, CDialogEx)
END_EVENTSINK_MAP()

void CDlgVATConfig::ShowEditVisionWaitTime(double dValue_ms)
{
	CString strSec("");
	strSec.Format(_T("%.3f"), (dValue_ms / 1000));

	CEdit *pEditVisionWaitTime = (CEdit *)GetDlgItem(IDC_EDIT_VISION_WAIT_TIME);
	pEditVisionWaitTime->SetWindowText(strSec);
}

void CDlgVATConfig::OnEnSetfocusEditVisionWaitTime()
{
	CString strInputValue(_T(""));
	CString strCurrentValue(_T(""));
	CString strTitle(_T("Vision Wait Time"));

	CEdit *pEditVisionWaitTime = (CEdit *)GetDlgItem(IDC_EDIT_VISION_WAIT_TIME);
	pEditVisionWaitTime->GetWindowText(strCurrentValue);

	BOOL bRet = g_LibCommCtrl.GetNumberBox(this, (LPSTR)(LPCSTR)strInputValue, 15, (LPSTR)(LPCSTR)strCurrentValue, (LPSTR)(LPCSTR)strTitle, "5.000", "0.500");
	if(bRet != TRUE)
		return;

	m_dVisionWaitTime = _ttof(strInputValue) * 1000.0; // msec.

	strInputValue.Format(_T("%.3f"), m_dVisionWaitTime / 1000.0);
	pEditVisionWaitTime->SetWindowText(strInputValue);
}


void CDlgVATConfig::OnEnSetfocusEditBasePosX()
{
	CString strInputValue(_T(""));
	CString strCurrentValue(_T(""));
	CString strTitle(_T("Base Position"));

	CEdit *pEditBasePosXValue = (CEdit *)GetDlgItem(IDC_EDIT_BASE_POS_X);
	pEditBasePosXValue->GetWindowText(strCurrentValue);

	BOOL bRet = g_LibCommCtrl.GetNumberBox(this, (LPSTR)(LPCSTR)strInputValue, 15, (LPSTR)(LPCSTR)strCurrentValue, (LPSTR)(LPCSTR)strTitle, "10.0", "0");
	if (bRet != TRUE)
		return;

	double dCurrentValue = _ttof(strInputValue);
	strInputValue.Format(_T("%.3f"), dCurrentValue);

	pEditBasePosXValue->SetWindowText(strInputValue);
}


void CDlgVATConfig::OnEnSetfocusEditBasePosY()
{
	CString strInputValue(_T(""));
	CString strCurrentValue(_T(""));
	CString strTitle(_T("Base Position"));

	CEdit *pEditBasePosYValue = (CEdit *)GetDlgItem(IDC_EDIT_BASE_POS_Y);
	pEditBasePosYValue->GetWindowText(strCurrentValue);

	BOOL bRet = g_LibCommCtrl.GetNumberBox(this, (LPSTR)(LPCSTR)strInputValue, 15, (LPSTR)(LPCSTR)strCurrentValue, (LPSTR)(LPCSTR)strTitle, "10.0", "0");
	if (bRet != TRUE)
		return;

	double dCurrentValue = _ttof(strInputValue);
	strInputValue.Format(_T("%.3f"), dCurrentValue);

	pEditBasePosYValue->SetWindowText(strInputValue);
}
