// DlgCylinderTimeCheck.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgCylinderTimeCheck.h"
#include "afxdialogex.h"



// CDlgCylinderTimeCheck 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgCylinderTimeCheck, CDialog)

CDlgCylinderTimeCheck::CDlgCylinderTimeCheck(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MANU_CYLINDER_TIME_CHECK, pParent)
{
	m_dDelayTime = 0.0;
}

CDlgCylinderTimeCheck::~CDlgCylinderTimeCheck()
{
}

void CDlgCylinderTimeCheck::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_STATION, m_ChkStation);
	DDX_Control(pDX, IDC_CHECK_STACKER, m_ChkStacker);
	DDX_Control(pDX, IDC_CHECK_LOADTABLE, m_ChkLoadTable);
	DDX_Control(pDX, IDC_CHECK_TRANSFER, m_ChkTransfer);
	
	DDX_Control(pDX, IDC_SPREAD_CYLINDER_DATA, m_SprdCylinderData);
	DDX_Control(pDX, IDC_SPREAD_CYLINDER_DATA2, m_SprdCylinderData2);

	DDX_Control(pDX, IDC_EDIT_DELAY_TIME, m_editDelayTime);
	DDX_Text(pDX, IDC_EDIT_DELAY_TIME, m_dDelayTime);

	DDX_Control(pDX, IDC_BUTTON_START, m_btnStart);
	DDX_Control(pDX, IDCANCEL, m_btnExit);

}


BEGIN_MESSAGE_MAP(CDlgCylinderTimeCheck, CDialog)

	ON_BN_CLICKED(IDCANCEL, &CDlgCylinderTimeCheck::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_START, &CDlgCylinderTimeCheck::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_DELAY_TIME_sET, &CDlgCylinderTimeCheck::OnBnClickedButtonDelayTimeset)
	ON_EN_SETFOCUS(IDC_EDIT_DELAY_TIME, &CDlgCylinderTimeCheck::OnEnSetfocusEditDelayTime)
END_MESSAGE_MAP()


// CDlgCylinderTimeCheck 메시지 처리기입니다.

BOOL CDlgCylinderTimeCheck::OnInitDialog()
{
	CDialog::OnInitDialog();

	UIUpdate();
	
	CylinderTimeDataUpdate();

	SpreadSheetInit();
	UpdateSpread();

	m_dDelayTime = g_DMCont.m_dmHandlerOpt.GD(DDM11_CYLINDER_DELAY_TIME);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgCylinderTimeCheck::UIUpdate()
{
	m_font.CreatePointFont(130, "맑은 고딕");

	GetDlgItem(IDC_BUTTON_START)->SetFont(&m_font);
	GetDlgItem(IDCANCEL)->SetFont(&m_font);
	GetDlgItem(IDC_CHECK_STACKER)->SetFont(&m_font);
	GetDlgItem(IDC_CHECK_LOADTABLE)->SetFont(&m_font);
	GetDlgItem(IDC_CHECK_STATION)->SetFont(&m_font);
	GetDlgItem(IDC_CHECK_TRANSFER)->SetFont(&m_font);
	GetDlgItem(IDC_STATIC_ACTION)->SetFont(&m_font);
	GetDlgItem(IDC_STATIC_CYLINDER_DATA)->SetFont(&m_font);

	m_cBmpStart.LoadBitmap(IDB_START);
	m_btnStart.SetBitmap(m_cBmpStart);

	m_cBmpExit.LoadBitmap(IDB_EXIT);
	m_btnExit.SetBitmap(m_cBmpExit);

	m_editDelayTime.SetBackColor(COLOR_WWHITE);
	m_editDelayTime.SetTextSize(30);
}

void CDlgCylinderTimeCheck::CylinderTimeDataUpdate()
{
	CMyList rCylinderList;
	_tCYLINDER_TIME* pItem = NULL;
	g_DB.SELECT_CYLINDER_TIME_TBL(rCylinderList);
	int nCount = rCylinderList.m_fnGetCount();

	for (int i = 0; i < nCount; i++)
	{
		pItem = (_tCYLINDER_TIME*)rCylinderList.m_fnGetData(i);

		g_CylinderTime.m_stCylinderTime[i].strUnitName = pItem->strUnitName;
		g_CylinderTime.m_stCylinderTime[i].dSpecMin = pItem->dSpecMin;
		g_CylinderTime.m_stCylinderTime[i].dSpecMax = pItem->dSpecMax;
		g_CylinderTime.m_stCylinderTime[i].dValue = pItem->dValue;
		g_CylinderTime.m_stCylinderTime[i].nIdx = pItem->nIdx;
	}
}

void CDlgCylinderTimeCheck::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialog::OnCancel();
}


void CDlgCylinderTimeCheck::OnBnClickedButtonStart()
{
	UpdateData(TRUE);

	if (!m_ChkStacker.GetCheck() && !m_ChkLoadTable.GetCheck() && !m_ChkStation.GetCheck() && !m_ChkTransfer.GetCheck())
	{
		AfxMessageBox("Zero Select");
		return;
	}
		
	if (AfxMessageBox("Cylinder Time Check Run??", MB_OKCANCEL) != IDOK)
		return;

// 	CCylindercmd_Check* pSeqCmd = new CCylindercmd_Check;
// 	pSeqCmd->m_bStackerChk = m_ChkStacker.GetCheck();
// 	pSeqCmd->m_bLoadTblChk = m_ChkLoadTable.GetCheck();
// 	pSeqCmd->m_bStationChk = m_ChkStation.GetCheck();
// 	pSeqCmd->m_bTransferChk = m_ChkTransfer.GetCheck();
// 
// 	int nErr = g_TaskSystemCtrl.SendCommand(g_MulitOrg, pSeqCmd, eEqpCtrlMode_Manual);
// 	if (nErr != ERR_NO_ERROR) {
// 		CParamList AlarmData;
// 		g_Error.AlarmReport(nErr, AlarmData, NULL);
// 	}

	CDialog::OnOK();
}

void CDlgCylinderTimeCheck::SpreadSheetInit()
{
	m_SprdCylinderData.SetMaxRows(MAX_CYLINDER_COUNT /2);
	m_SprdCylinderData2.SetMaxRows(MAX_CYLINDER_COUNT /2);

	for (int i=0; i < MAX_CYLINDER_COUNT /2; i++)
	{
		m_SprdCylinderData.SetRowHeight(i+1,13);
		m_SprdCylinderData.SetRowHeight(MAX_CYLINDER_COUNT / 2 + i + 1, 13);
	}
}

void CDlgCylinderTimeCheck::UpdateSpread()
{
	CString str;
	
	for (int nRow = 0; nRow < MAX_CYLINDER_COUNT; nRow++) {
		if (nRow < MAX_CYLINDER_COUNT / 2)
		{
			m_SprdCylinderData.SetTextMatrix(nRow + 1, 1, g_CylinderTime.m_stCylinderTime[nRow].strUnitName);
			str.Format("%.3f ~ %.3f", g_CylinderTime.m_stCylinderTime[nRow].dSpecMin, g_CylinderTime.m_stCylinderTime[nRow].dSpecMax);
			m_SprdCylinderData.SetTextMatrix(nRow + 1, 2, str);
			str.Format("%.3f", g_CylinderTime.m_stCylinderTime[nRow].dValue);
			m_SprdCylinderData.SetTextMatrix(nRow + 1, 3, str);

			m_SprdCylinderData.SetRow(nRow + 1);
			m_SprdCylinderData.SetCol(3);

			if (g_CylinderTime.m_stCylinderTime[nRow].dSpecMin <= g_CylinderTime.m_stCylinderTime[nRow].dValue && g_CylinderTime.m_stCylinderTime[nRow].dSpecMax >= g_CylinderTime.m_stCylinderTime[nRow].dValue)
				m_SprdCylinderData.SetBackColor(COLOR_LLIGHTGREEN);
			else
				m_SprdCylinderData.SetBackColor(COLOR_LLIGHTRED);
		}
		else
		{
			m_SprdCylinderData2.SetTextMatrix(nRow + 1 - MAX_CYLINDER_COUNT / 2, 1, g_CylinderTime.m_stCylinderTime[nRow].strUnitName);
			str.Format("%.3f ~ %.3f", g_CylinderTime.m_stCylinderTime[nRow].dSpecMin, g_CylinderTime.m_stCylinderTime[nRow].dSpecMax);
			m_SprdCylinderData2.SetTextMatrix(nRow + 1 - MAX_CYLINDER_COUNT / 2, 2, str);
			//str.Format("%.2f", m_veCylinderData[nRow].dValue);
			str.Format("%.3f", g_CylinderTime.m_stCylinderTime[nRow].dValue);
			m_SprdCylinderData2.SetTextMatrix(nRow + 1 - MAX_CYLINDER_COUNT / 2, 3, str);
			str.Format("%d", nRow + 1);
			m_SprdCylinderData2.SetTextMatrix(nRow + 1 - MAX_CYLINDER_COUNT / 2, 0, str);
			m_SprdCylinderData2.SetRow(nRow + 1 - MAX_CYLINDER_COUNT / 2);
			m_SprdCylinderData2.SetCol(3);

			if (g_CylinderTime.m_stCylinderTime[nRow].dSpecMin <= g_CylinderTime.m_stCylinderTime[nRow].dValue && g_CylinderTime.m_stCylinderTime[nRow].dSpecMax >= g_CylinderTime.m_stCylinderTime[nRow].dValue)
				m_SprdCylinderData2.SetBackColor(COLOR_LLIGHTGREEN);
			else
				m_SprdCylinderData2.SetBackColor(COLOR_LLIGHTRED);
		}
	
	
	}
}

void CDlgCylinderTimeCheck::OnEnSetfocusEditDelayTime()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_dDelayTime, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "LoopTest Count", "2", "0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_DELAY_TIME)->SetWindowText(szRetVal);
	}
}

void CDlgCylinderTimeCheck::OnBnClickedButtonDelayTimeset()
{
	UpdateData(TRUE);
	g_DMCont.m_dmHandlerOpt.SD(DDM11_CYLINDER_DELAY_TIME, m_dDelayTime);
}


