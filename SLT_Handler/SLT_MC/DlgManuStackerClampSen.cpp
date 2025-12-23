// DlgManuStackerClampSen.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgManuStackerClampSen.h"
#include "afxdialogex.h"
#include "../Include/OcxFiles/dsspanel.h"


// CDlgManuStackerClampSen 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgManuStackerClampSen, CDialogEx)

CDlgManuStackerClampSen::CDlgManuStackerClampSen(int nTaskId/*=TASK_STACKER_LD*/, int nStackerIdx/*=0*/, CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgManuStackerClampSen::IDD, pParent), m_nTaskId(nTaskId), m_nStackerIdx(nStackerIdx)
{
	// 미사용
	//m_nSenCtrlId[CTaskStacker::eRear_Left  ][DEF_CLAMP] = IDC_STACKER_CLAMP_SEN_R_L;
	//m_nSenCtrlId[CTaskStacker::eRear_Right ][DEF_CLAMP] = IDC_STACKER_CLAMP_SEN_R_R;
	//m_nSenCtrlId[CTaskStacker::eFront_Left ][DEF_CLAMP] = IDC_STACKER_CLAMP_SEN_F_L;
	//m_nSenCtrlId[CTaskStacker::eFront_Right][DEF_CLAMP] = IDC_STACKER_CLAMP_SEN_F_R;
	//m_nSenCtrlId[CTaskStacker::eRear_Left  ][DEF_UNCLAMP] = IDC_STACKER_UNCLAMP_SEN_R_L;
	//m_nSenCtrlId[CTaskStacker::eRear_Right ][DEF_UNCLAMP] = IDC_STACKER_UNCLAMP_SEN_R_R;
	//m_nSenCtrlId[CTaskStacker::eFront_Left ][DEF_UNCLAMP] = IDC_STACKER_UNCLAMP_SEN_F_L;
	//m_nSenCtrlId[CTaskStacker::eFront_Right][DEF_UNCLAMP] = IDC_STACKER_UNCLAMP_SEN_F_R;
}

CDlgManuStackerClampSen::~CDlgManuStackerClampSen()
{
}

void CDlgManuStackerClampSen::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgManuStackerClampSen, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgManuStackerClampSen 메시지 처리기입니다.


BOOL CDlgManuStackerClampSen::PreTranslateMessage(MSG* pMsg)
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


BOOL CDlgManuStackerClampSen::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ChangeSensor(m_nTaskId, m_nStackerIdx);
	SetTimer(0, 500, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgManuStackerClampSen::OnDestroy()
{
	CDialogEx::OnDestroy();

	KillTimer(0);
}


void CDlgManuStackerClampSen::OnTimer(UINT_PTR nIDEvent)
{
	switch( nIDEvent )
	{
	case 0:
		{
			KillTimer(0);
			UpdateDisplayClampSen();
			SetTimer(0, 500, NULL);
		}break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CDlgManuStackerClampSen::ChangeSensor(int nTaskId, int nStackerIdx)
{
	// 미사용
	//m_nTaskId = nTaskId;
	//m_nStackerIdx = nStackerIdx;
	//CString strTitle;
	//if( nTaskId == TASK_STACKER_LD ){
	//	strTitle.Format("LOAD STACKER %d", nStackerIdx+1);
	//}else{
	//	strTitle.Format("UNLOAD STACKER %d", nStackerIdx+1);
	//}
	//SetDlgItemText(IDC_STATIC_STACKER_SEN_VIEW_TITLE, strTitle);
}

void CDlgManuStackerClampSen::UpdateDisplayClampSen()
{
	// 미사용
	//CTaskStacker* pTaskStacker = NULL;
	//if( m_nTaskId == TASK_STACKER_LD ){
	//	pTaskStacker = &g_TaskStackerLD;
	//}else{
	//	pTaskStacker = &g_TaskStackerUD;
	//}
	//
	//for(int j=0; j<CTaskStacker::eMaxStackerSenLoc; j++){
	//	CDSSPanel* pSensor = NULL;
	//	pSensor = (CDSSPanel*)GetDlgItem(m_nSenCtrlId[j][DEF_CLAMP]);
	//	BOOL bSen = pTaskStacker->GetClampSen(m_nStackerIdx, j, DEF_CLAMP);
	//	if( bSen ) pSensor->On();
	//	else       pSensor->Off();
	//
	//	pSensor = (CDSSPanel*)GetDlgItem(m_nSenCtrlId[j][DEF_UNCLAMP]);
	//	bSen = pTaskStacker->GetClampSen(m_nStackerIdx, j, DEF_UNCLAMP);
	//	if( bSen ) pSensor->On();
	//	else       pSensor->Off();
	//}
}
