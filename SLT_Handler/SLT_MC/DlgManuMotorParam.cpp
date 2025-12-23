// DlgManuMotorParam.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgManuMotorParam.h"
#include "afxdialogex.h"
#include "DlgCalculatePcd.h"

// CDlgManuMotorParam 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgManuMotorParam, CDialogEx)

CDlgManuMotorParam::CDlgManuMotorParam(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgManuMotorParam::IDD, pParent)
{

}

CDlgManuMotorParam::~CDlgManuMotorParam()
{
}

void CDlgManuMotorParam::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSFLEXGRID_MOTOR_PARAM, m_srdMotorParam);
}


BEGIN_MESSAGE_MAP(CDlgManuMotorParam, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CDlgManuMotorParam::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgManuMotorParam 메시지 처리기입니다.

BOOL CDlgManuMotorParam::PreTranslateMessage(MSG* pMsg)
{
	if((pMsg->message==WM_SYSKEYDOWN) && (pMsg->wParam==VK_F4) )
	{
		return TRUE;
	}
	if((pMsg->message==WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))
	{
		return TRUE;
	}
	if((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ESCAPE) )
	{
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CDlgManuMotorParam::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	g_DB.SELECT_MOTORPARAM_TBL(m_MotorParamList);
	InitControl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgManuMotorParam::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CDlgManuMotorParam::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CDlgManuMotorParam::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}

void CDlgManuMotorParam::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}


void CDlgManuMotorParam::InitControl()
{
	// Row 항목 표시
	int nMotorCount = m_MotorParamList.m_fnGetCount();
	m_srdMotorParam.SetMaxRows(nMotorCount);

	for(int i=0; i<nMotorCount; i++){
		_tMOTOR_PARAM* pItem = (_tMOTOR_PARAM*)m_MotorParamList.m_fnGetData(i);
		// Axis Name
		m_srdMotorParam.SetTextMatrix(i+1, 0, pItem->strAxisName);
		double dAxisTypeVal = 0.0; // 회전축은 각도 단위로 처리 해야함 나머지는 um -> mm 단위
		if (pItem->bDegree == TRUE)
		{
			dAxisTypeVal = 1.0;
		}
		else
		{
			dAxisTypeVal = 1000.0;
		}
		// Normal Speed
		CString strSpd;
		strSpd.Format("%.2f", pItem->nSpd_Normal/ dAxisTypeVal);
		m_srdMotorParam.SetTextMatrix(i+1, 1, strSpd);

		// Acc
		CString strAcc;
		strAcc.Format("%d", pItem->nAcc_Normal);
		m_srdMotorParam.SetTextMatrix(i+1, 2, strAcc);
		// Dec
		CString strDec;
		strDec.Format("%d", pItem->nDec_Normal);
		m_srdMotorParam.SetTextMatrix(i+1, 3, strDec);

		// Posi Limit
		CString strPosiLmt;
		strPosiLmt.Format("%.2f", pItem->dLimit_Posi/ dAxisTypeVal);
		m_srdMotorParam.SetTextMatrix(i+1, 4, strPosiLmt);

		// Nega Limit
		CString strNegaLmt;
		strNegaLmt.Format("%.2f", pItem->dLimit_Nega/ dAxisTypeVal);
		m_srdMotorParam.SetTextMatrix(i+1, 5, strNegaLmt);

		// ZR Spd
		CString strSpdZr;
		strSpdZr.Format("%.2f", pItem->nSpd_ZR/ dAxisTypeVal);
		m_srdMotorParam.SetTextMatrix(i+1, 6, strSpdZr);

		// ZR Acc
		CString strAccZr;
		strAccZr.Format("%d", pItem->nAcc_ZR);
		m_srdMotorParam.SetTextMatrix(i+1, 7, strAccZr);
	}
}
BEGIN_EVENTSINK_MAP(CDlgManuMotorParam, CDialogEx)
	ON_EVENT(CDlgManuMotorParam, IDC_COMMANDBUTTON_SAVE, 22, CDlgManuMotorParam::ClickCommandbuttonSave, VTS_NONE)
	ON_EVENT(CDlgManuMotorParam, IDC_COMMANDBUTTON_PCD, 22, CDlgManuMotorParam::ClickCommandbuttonPcd, VTS_NONE)
END_EVENTSINK_MAP()


void CDlgManuMotorParam::ClickCommandbuttonSave()
{
	int nRet = AfxMessageBox("Do you want to save motor parameter?", MB_YESNO);

	if( nRet == IDYES ){
		//Normal Speed 범위 벗어난 축 있는지 확인.
		std::pair<CString, int> pData = CheckNormalSpdOverRange(); //[first : 축 Name], [second : DB Normal Spd] 
		if (!(pData.first == _T("") && pData.second == 0)) {
			double dRange = (pData.second) * 0.1; //10%
			double dMinData = (pData.second - dRange);
			double dMaxData = pData.second + dRange;

			CString message = _T("");
			message.Format("[Axis = %s]\n\n Normal Speed is out of range.\n\n[Range] %.f ~ %.f ", pData.first, dMinData, dMaxData);
			AfxMessageBox(message);
			return;
		}

		// Row 항목 표시
		int nMotorCount = m_MotorParamList.m_fnGetCount();
		for(int i=0; i<nMotorCount; i++){
			_tMOTOR_PARAM* pItem = (_tMOTOR_PARAM*)m_MotorParamList.m_fnGetData(i);
			double dChangeDBType = 0.0;
			// Normal Speed
			if (pItem->bDegree == TRUE) // theta축은 입력한 그대로 DB값 쓰도록 한다.
			{
				dChangeDBType = 1.0;
			}
			else
			{
				dChangeDBType = 1000.0; // mm -> um 단위로 저장 한다.
			}
			pItem->nSpd_Normal = (int)(atof(m_srdMotorParam.GetTextMatrix(i+1, 1)) * dChangeDBType);
			// Acc
			pItem->nAcc_Normal = atoi(m_srdMotorParam.GetTextMatrix(i+1, 2));

			// Dec
			pItem->nDec_Normal = atoi(m_srdMotorParam.GetTextMatrix(i+1, 3));
			// Posi Limit		
			pItem->dLimit_Posi = atof(m_srdMotorParam.GetTextMatrix(i+1, 4)) * dChangeDBType;
			// Nega Limit
			pItem->dLimit_Nega = atof(m_srdMotorParam.GetTextMatrix(i+1, 5)) * dChangeDBType;
			// ZR Spd
			pItem->nSpd_ZR = (int)(atof(m_srdMotorParam.GetTextMatrix(i+1, 6)) * dChangeDBType);
			// ZR Acc
			pItem->nAcc_ZR = atoi(m_srdMotorParam.GetTextMatrix(i+1, 7));
			// Save
			g_DB.UPDATE_MOTORPARAM_TBL(i , *pItem);

			int nAxisNo_Logical = pItem->nAxisNo_Logical;
			g_Motor[nAxisNo_Logical]->SetVelAcc(pItem->nSpd_Normal, pItem->nAcc_Normal, pItem->nDec_Normal, pItem->nSpd_ZR, pItem->nAcc_ZR, pItem->dLimit_Posi, pItem->dLimit_Nega);
		}
	}	
}


void CDlgManuMotorParam::ClickCommandbuttonPcd()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CDlgCalculatePcd dlg;
	dlg.DoModal();
}


//============================================================================
//[5/14/2025 dohyeong.kim]
//COMMENT : Normal Speed 현재 값 에서 10% 범위내에서 수정 가능.
//			DB와 수정 된 값 비교 후, 10% 초과 확인.       
//RETURN :
//	Param1 : Normal Speed 벗어 난 축 Name Return.  (CString)
//  Parma2 : 현재 DB Normal Speed Data			  (int)
//===========================================================================
std::pair<CString, int> CDlgManuMotorParam::CheckNormalSpdOverRange()
{
	std::pair<CString, int> pData;
	pData = std::make_pair(_T(""), 0); //초기화

	int nMotorCount = m_MotorParamList.m_fnGetCount();
	for (int i = 0; i < nMotorCount; i++) 
	{
		_tMOTOR_PARAM* pItem = (_tMOTOR_PARAM*)m_MotorParamList.m_fnGetData(i);

		//DB에 저장 되어 있는 Normal Speed.
		int nSpdNormal_DB = pItem->nSpd_Normal;

		//작업자 변경 한, Current Normal Speed.
		double dChangeDBType = 0.0;
		if (pItem->bDegree == TRUE){ // theta축은 입력한 그대로 DB값 쓰도록 한다.
			dChangeDBType = 1.0;
		}
		else{
			dChangeDBType = 1000.0; // mm -> um 단위로 저장 한다.
		}
		int nSpdNormal_Cur = (int)(atof(m_srdMotorParam.GetTextMatrix(i + 1, 1)) * dChangeDBType);

		//동일할 경우, 변경 사항 없음.
		if (nSpdNormal_DB == nSpdNormal_Cur) {
			continue;
		}

		//다를 경우 10% 범위 값인지 확인.(+10%, -10%)
		int nGap = abs(nSpdNormal_DB - nSpdNormal_Cur);
		int nStandardRange = nSpdNormal_DB * 0.1;
		if (nGap > nStandardRange) {
			pData.first = pItem->strAxisName;
			pData.second = nSpdNormal_DB / dChangeDBType; //um -> mm
			return pData; //OverRange Axis return
		}
	}

	return pData; 
}

