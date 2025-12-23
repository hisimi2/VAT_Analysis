// DlgCalculatePcd.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgCalculatePcd.h"
#include "afxdialogex.h"


// CDlgCalculatePcd 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgCalculatePcd, CDialogEx)

CDlgCalculatePcd::CDlgCalculatePcd(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_PCD_CAL, pParent)
{
	m_dDesignDis = 0.0;
	m_dMeasureDis = 0.0;
	m_dDesignScaleFactor = 0.0;
	m_dMeasureScaleFactor = 0.0;
	m_nCurAxis = 0;
	m_bChkCal = FALSE;
}

CDlgCalculatePcd::~CDlgCalculatePcd()
{
	m_listMotorParam.m_fnClear();
}

void CDlgCalculatePcd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PCD_CAL_AXIS_SELECT, m_cbAxisSelect);
}


BEGIN_MESSAGE_MAP(CDlgCalculatePcd, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_PCD_CAL_AXIS_SELECT, &CDlgCalculatePcd::OnCbnSelchangeComboPcdCalAxisSelect)
	ON_BN_CLICKED(IDC_BUTTON_PCD_CALCULATE, &CDlgCalculatePcd::OnBnClickedButtonPcdCalculate)
	ON_BN_CLICKED(IDC_BUTTON_PCD_CAL_SAVE, &CDlgCalculatePcd::OnBnClickedButtonPcdCalSave)
	ON_EN_SETFOCUS(IDC_EDIT_PCD_DESIGN_DISTANCE, &CDlgCalculatePcd::OnEnSetfocusEditPcdDesignDistance)
	ON_EN_SETFOCUS(IDC_EDIT_PCD_MEASURE_DISTANCE, &CDlgCalculatePcd::OnEnSetfocusEditPcdMeasureDistance)
END_MESSAGE_MAP()


// CDlgCalculatePcd 메시지 처리기입니다.


BOOL CDlgCalculatePcd::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.


	g_DB.SELECT_MOTORPARAM_TBL(m_listMotorParam);
	int nAxisCount = m_listMotorParam.m_fnGetCount();
	for (int i = 0; i < nAxisCount; i++) {
		_tMOTOR_PARAM* pItem = (_tMOTOR_PARAM*)m_listMotorParam.m_fnGetData(i);
		CString strAxisName = pItem->strAxisName;
		m_cbAxisSelect.AddString(strAxisName);
	}

	m_cbAxisSelect.SetCurSel(0);

	CString strData;
	_tMOTOR_PARAM* pItem = (_tMOTOR_PARAM*)m_listMotorParam.m_fnGetData(0);
	m_dDesignScaleFactor = pItem->dScaleFactor;
	strData.Format(_T("%.3f"), m_dDesignScaleFactor);
	GetDlgItem(IDC_EDIT_PCD_DESIGN_SCALEFACTOR)->SetWindowText(strData);

	strData.Format(_T("%.3f"), 0.0);
	GetDlgItem(IDC_EDIT_PCD_MEASURE_SCALEFACTOR)->SetWindowText(strData);

	m_dDesignDis = g_DMCont.m_dmEQP.GD(DDM0_PCD_CAL_DESIGN_DISTANCE);
	strData.Format(_T("%.3f"), m_dDesignDis);
	GetDlgItem(IDC_EDIT_PCD_DESIGN_DISTANCE)->SetWindowText(strData);

	m_dMeasureDis = g_DMCont.m_dmEQP.GD(DDM0_PCD_CAL_MEASURE_DISTANCE);
	strData.Format(_T("%.3f"), m_dMeasureDis);
	GetDlgItem(IDC_EDIT_PCD_MEASURE_DISTANCE)->SetWindowText(strData);



	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgCalculatePcd::OnCbnSelchangeComboPcdCalAxisSelect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_cbAxisSelect.GetCurSel() >= 0) {
		m_nCurAxis = m_cbAxisSelect.GetCurSel();
	}

	CString strData;
	_tMOTOR_PARAM* pItem = (_tMOTOR_PARAM*)m_listMotorParam.m_fnGetData(m_nCurAxis);
	m_dDesignScaleFactor = pItem->dScaleFactor;
	strData.Format(_T("%.3f"), m_dDesignScaleFactor);
	GetDlgItem(IDC_EDIT_PCD_DESIGN_SCALEFACTOR)->SetWindowText(strData);
}


void CDlgCalculatePcd::OnBnClickedButtonPcdCalculate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//						100.1/scalefactor
	//								100/pulse
	double dMeasurePulse = m_dMeasureDis / m_dDesignScaleFactor;
	double dDesignScaleFactor = m_dDesignDis / dMeasurePulse;
	 
	double dCal = m_dDesignScaleFactor - dDesignScaleFactor;
	m_dDesignScaleFactor += dCal;
	m_dMeasureScaleFactor = m_dDesignScaleFactor;
	CString strData;
	strData.Format(_T("%.3f"), m_dMeasureScaleFactor);
	GetDlgItem(IDC_EDIT_PCD_MEASURE_SCALEFACTOR)->SetWindowText(strData);

	m_bChkCal = TRUE;

}


void CDlgCalculatePcd::OnBnClickedButtonPcdCalSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_bChkCal) {
		AfxMessageBox("Please Press Calculate Button!");
		return;
	}

	g_DMCont.m_dmEQP.SD(DDM0_PCD_CAL_DESIGN_DISTANCE, m_dDesignDis);
	g_DMCont.m_dmEQP.SD(DDM0_PCD_CAL_MEASURE_DISTANCE, m_dMeasureDis);

	g_DB.UPDATE_MOTORPARAM_TBL(m_nCurAxis, m_dMeasureScaleFactor);
	AfxMessageBox("Scale Factor Save Success !");

	g_Motor[m_nCurAxis]->SetScaleFactor(m_dMeasureScaleFactor);
}


void CDlgCalculatePcd::OnEnSetfocusEditPcdDesignDistance()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szCur[VAT_STRING_SIZE] = { 0, };
	char szChange[VAT_STRING_SIZE] = { 0, };
	CString strCur = _T("");

	CEdit *editDesignDis = (CEdit *)GetDlgItem(IDC_EDIT_PCD_DESIGN_DISTANCE);

	editDesignDis->GetWindowText(strCur);

	sprintf_s(szCur, sizeof(szCur), "%s", strCur.GetString());

	BOOL bRet = g_LibCommCtrl.GetNumberBox(this, szChange, 15, szCur, "Pcd Design Distance", "500.0", "0");
	if (bRet != TRUE)
		return;

	m_dDesignDis = _ttof(szChange);

	editDesignDis->SetWindowText(szChange);
}


void CDlgCalculatePcd::OnEnSetfocusEditPcdMeasureDistance()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szCur[VAT_STRING_SIZE] = { 0, };
	char szChange[VAT_STRING_SIZE] = { 0, };
	CString strCur = _T("");

	CEdit *editMeasureDis = (CEdit *)GetDlgItem(IDC_EDIT_PCD_MEASURE_DISTANCE);

	editMeasureDis->GetWindowText(strCur);

	sprintf_s(szCur, sizeof(szCur), "%s", strCur.GetString());

	BOOL bRet = g_LibCommCtrl.GetNumberBox(this, szChange, 15, szCur, "Pcd Measure Distance", "500.0", "0");
	if (bRet != TRUE)
		return;

	m_dMeasureDis = _ttof(szChange);

	editMeasureDis->SetWindowText(szChange);
}
