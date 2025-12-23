// DlgTableVacuumCheck.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "DlgTableVacuumCheck.h"
#include "afxdialogex.h"


// CDlgTableVacuumCheck 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTableVacuumCheck, CDialog)

CDlgTableVacuumCheck::CDlgTableVacuumCheck(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_TABLE_VACUUM_CHECK, pParent)
{

}

CDlgTableVacuumCheck::~CDlgTableVacuumCheck()
{
}

void CDlgTableVacuumCheck::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPREAD_TABLE1_VACUUM_CHECK, m_ssLoadTable_Part[0]);
	DDX_Control(pDX, IDC_SPREAD_TABLE2_VACUUM_CHECK, m_ssLoadTable_Part[1]);
	DDX_Control(pDX, IDC_SPREAD_TABLE3_VACUUM_CHECK, m_ssLoadTable_Part[2]);
	DDX_Control(pDX, IDC_SPREAD_TABLE4_VACUUM_CHECK, m_ssLoadTable_Part[3]);

	DDX_Control(pDX, IDC_CHECK_TABLE_VACUUM_CHECK_ONOFF, m_ChkTableVacuumCheckOnOff);
	
	
}


BEGIN_MESSAGE_MAP(CDlgTableVacuumCheck, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgTableVacuumCheck::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_TABLE_VACUUM_CHECK_ONOFF, &CDlgTableVacuumCheck::OnBnClickedCheckTableVacuumCheckOnoff)
END_MESSAGE_MAP()


// CDlgTableVacuumCheck 메시지 처리기입니다.


BOOL CDlgTableVacuumCheck::OnInitDialog()
{
	CDialog::OnInitDialog();

	int nTableVacuumChk = g_DMCont.m_dmShuttle.GB(BDM3_UI_TABLE_VACUUM_CHECK_ONOFF);
	m_ChkTableVacuumCheckOnOff.SetCheck(nTableVacuumChk);

	BOOL bLotState = g_DMCont.m_dmEQP.GB(BDM0_SYS_LOT_STATUS);
	if (m_ChkTableVacuumCheckOnOff.GetCheck() == FALSE)
	{
		if (bLotState == eHandlerLotMode_OnGoing || g_DMCont.m_dmEQP.GN(NDM0_CurLotLoadingCnt) > 0) {
			GetDlgItem(IDC_CHECK_TABLE_VACUUM_CHECK_ONOFF)->EnableWindow(FALSE);
		}
	}

	int nPocketCount_X = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);
	int nPocketCount_Y = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);

	for (int i = 0; i < eMaxLoadTblCount; i++)
	{
		m_ssLoadTable_Part[i].SetMaxCols(nPocketCount_X);
		m_ssLoadTable_Part[i].SetMaxRows(nPocketCount_Y);
	}


	int nSprdData[eMaxLoadTblCount] = { IDC_SPREAD_TABLE1_VACUUM_CHECK,IDC_SPREAD_TABLE2_VACUUM_CHECK,
		IDC_SPREAD_TABLE3_VACUUM_CHECK ,IDC_SPREAD_TABLE4_VACUUM_CHECK };

	CSpreadSheet* pSpread = NULL;
	for (int i = 0; i < eMaxLoadTblCount; i++)
	{
		for (int nY = 0; nY < nPocketCount_Y; nY++)
		{
			for (int nX = 0; nX < nPocketCount_X; nX++)
			{			
				int nMemAddress = (i * (nPocketCount_X * nPocketCount_Y)) + (nX + (nY * nPocketCount_X));
				pSpread = (CSpreadSheet*)GetDlgItem(nSprdData[i]);
				BOOL bCheck = g_DMCont.m_dmShuttle.GB(BDM3_TABLE1_POCKET_VACUUM_CHECK_COMPLETE_X1_Y1 + nMemAddress);
				pSpread->SetTextMatrix(nY + 1, nX + 1, (bCheck == TRUE ? "1" : ""));
			}
		}
	
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgTableVacuumCheck::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialog::OnOK();
}


void CDlgTableVacuumCheck::OnBnClickedCheckTableVacuumCheckOnoff()
{
	BOOL bFlag = FALSE;
	if (m_ChkTableVacuumCheckOnOff.GetCheck())
	{
		bFlag = TRUE;
	}
	else {
		bFlag = FALSE;
	}

	g_DMCont.m_dmShuttle.SB(BDM3_UI_TABLE_VACUUM_CHECK_ONOFF, bFlag);
}
