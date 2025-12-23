// DlgHomeCheckStatus.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgHomeCheckStatus.h"
#include "afxdialogex.h"

#define MAX_AXIS_COUNT   46

// CDlgHomeCheckStatus 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgHomeCheckStatus, CDialogEx)

CDlgHomeCheckStatus::CDlgHomeCheckStatus(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgHomeCheckStatus::IDD, pParent)
{
	m_veAxisNum.clear();
	m_veAxisNum.push_back(eAXIS_PRESS_UNIT_DN1);		
	m_veAxisNum.push_back(eAXIS_PRESS_UNIT_DN2);		
	m_veAxisNum.push_back(eAXIS_PRESS_UNIT_DN3);
	m_veAxisNum.push_back(eAXIS_PRESS_UNIT_DN4);
	m_veAxisNum.push_back(eAXIS_PRESS_UNIT_DN5);
	m_veAxisNum.push_back(eAXIS_PRESS_UNIT_DN6);
	m_veAxisNum.push_back(eAXIS_PRESS_UNIT_DN7);
	m_veAxisNum.push_back(eAXIS_PRESS_UNIT_DN8);

	m_veAxisNum.push_back(eAXIS_TEST_PP_1_X);
	m_veAxisNum.push_back(eAXIS_TEST_PP_1_X2);
	m_veAxisNum.push_back(eAXIS_TEST_PP_1_Y);
	m_veAxisNum.push_back(eAXIS_TEST_PP_1_Z);
	m_veAxisNum.push_back(eAXIS_TEST_PP_1_Z2);
	m_veAxisNum.push_back(eAXIS_TEST_PP_1_X_PITCH);
	m_veAxisNum.push_back(eAXIS_TEST_PP_1_X2_PITCH);
	m_veAxisNum.push_back(eAXIS_TEST_PP_1_Y_PITCH);
	m_veAxisNum.push_back(eAXIS_TEST_PP_1_Y2_PITCH);

	m_veAxisNum.push_back(eAXIS_TEST_PP_2_X);
	m_veAxisNum.push_back(eAXIS_TEST_PP_2_X2);
	m_veAxisNum.push_back(eAXIS_TEST_PP_2_Y);
	m_veAxisNum.push_back(eAXIS_TEST_PP_2_Z);
	m_veAxisNum.push_back(eAXIS_TEST_PP_2_Z2);
	m_veAxisNum.push_back(eAXIS_TEST_PP_2_X_PITCH);
	m_veAxisNum.push_back(eAXIS_TEST_PP_2_X2_PITCH);
	m_veAxisNum.push_back(eAXIS_TEST_PP_2_Y_PITCH);
	m_veAxisNum.push_back(eAXIS_TEST_PP_2_Y2_PITCH);

	m_veAxisNum.push_back(eAXIS_LOAD_TABLE_1_Y);
	m_veAxisNum.push_back(eAXIS_LOAD_TABLE_2_Y);
	m_veAxisNum.push_back(eAXIS_LOAD_TABLE_3_Y);
	m_veAxisNum.push_back(eAXIS_LOAD_TABLE_4_Y);
	m_veAxisNum.push_back(eAXIS_LOAD_TABLE_1_ROTATE);
	m_veAxisNum.push_back(eAXIS_LOAD_TABLE_2_ROTATE);
	m_veAxisNum.push_back(eAXIS_LOAD_TABLE_3_ROTATE);
	m_veAxisNum.push_back(eAXIS_LOAD_TABLE_4_ROTATE);

	m_veAxisNum.push_back(eAXIS_TRAY_PP_1_X);
	m_veAxisNum.push_back(eAXIS_TRAY_FEEDER_Y);
	m_veAxisNum.push_back(eAXIS_TRAY_PP_1_Z);
	m_veAxisNum.push_back(eAXIS_TRAY_PP_1_X_PITCH);
	m_veAxisNum.push_back(eAXIS_TRAY_PP_1_Y_PITCH);

	m_veAxisNum.push_back(eAXIS_TRAY_PP_2_X);
	m_veAxisNum.push_back(eAXIS_TRAY_PP_2_Y);
	m_veAxisNum.push_back(eAXIS_TRAY_PP_2_Z);
	m_veAxisNum.push_back(eAXIS_TRAY_PP_2_X_PITCH);
	m_veAxisNum.push_back(eAXIS_TRAY_PP_2_Y_PITCH);

	m_veAxisNum.push_back(eAXIS_TRANSFER_X);


	m_vdCurPos.push_back(IDC_EDIT_CUR_POS_1);
	m_vdCurPos.push_back(IDC_EDIT_CUR_POS_2);
	m_vdCurPos.push_back(IDC_EDIT_CUR_POS_3);
	m_vdCurPos.push_back(IDC_EDIT_CUR_POS_4);
	m_vdCurPos.push_back(IDC_EDIT_CUR_POS_5);
	m_vdCurPos.push_back(IDC_EDIT_CUR_POS_6);
	m_vdCurPos.push_back(IDC_EDIT_CUR_POS_7);
	m_vdCurPos.push_back(IDC_EDIT_CUR_POS_8);
	m_vdCurPos.push_back(IDC_EDIT_CUR_POS_9);
	m_vdCurPos.push_back(IDC_EDIT_CUR_POS_10);
	m_vdCurPos.push_back(IDC_EDIT_CUR_POS_11);
	m_vdCurPos.push_back(IDC_EDIT_CUR_POS_12);
	m_vdCurPos.push_back(IDC_EDIT_CUR_POS_13);
	m_vdCurPos.push_back(IDC_EDIT_CUR_POS_14);
	m_vdCurPos.push_back(IDC_EDIT_CUR_POS_15);
	m_vdCurPos.push_back(IDC_EDIT_CUR_POS_16);
	m_vdCurPos.push_back(IDC_EDIT_CUR_POS_17);
	m_vdCurPos.push_back(IDC_EDIT_CUR_POS_18);
	m_vdCurPos.push_back(IDC_EDIT_CUR_POS_19);
	m_vdCurPos.push_back(IDC_EDIT_CUR_POS_20);
	m_vdCurPos.push_back(IDC_EDIT_CUR_POS_21);
	m_vdCurPos.push_back(IDC_EDIT_CUR_POS_22);
	m_vdCurPos.push_back(IDC_EDIT_CUR_POS_23);
	m_vdCurPos.push_back(IDC_EDIT_CUR_POS_24);
	m_vdCurPos.push_back(IDC_EDIT_CUR_POS_25);
	m_vdCurPos.push_back(IDC_EDIT_CUR_POS_26);
	m_vdCurPos.push_back(IDC_EDIT_CUR_POS_27);
	m_vdCurPos.push_back(IDC_EDIT_CUR_POS_28);
	m_vdCurPos.push_back(IDC_EDIT_CUR_POS_29);
	m_vdCurPos.push_back(IDC_EDIT_CUR_POS_30);
	m_vdCurPos.push_back(IDC_EDIT_CUR_POS_31);
	m_vdCurPos.push_back(IDC_EDIT_CUR_POS_32);
	m_vdCurPos.push_back(IDC_EDIT_CUR_POS_33);
	m_vdCurPos.push_back(IDC_EDIT_CUR_POS_34);
	m_vdCurPos.push_back(IDC_EDIT_CUR_POS_35);
	m_vdCurPos.push_back(IDC_EDIT_CUR_POS_36);
	m_vdCurPos.push_back(IDC_EDIT_CUR_POS_37);
	m_vdCurPos.push_back(IDC_EDIT_CUR_POS_38);
	m_vdCurPos.push_back(IDC_EDIT_CUR_POS_39);
	m_vdCurPos.push_back(IDC_EDIT_CUR_POS_40);
	m_vdCurPos.push_back(IDC_EDIT_CUR_POS_41);
	m_vdCurPos.push_back(IDC_EDIT_CUR_POS_42);
	m_vdCurPos.push_back(IDC_EDIT_CUR_POS_43);
	m_vdCurPos.push_back(IDC_EDIT_CUR_POS_44);
	m_vdCurPos.push_back(IDC_EDIT_CUR_POS_45);
	m_nEqpOption = 1;

	m_Home_Status = new CDSSPanel[MAX_AXIS_COUNT];
}

CDlgHomeCheckStatus::~CDlgHomeCheckStatus()
{
	delete[] m_Home_Status;
}

void CDlgHomeCheckStatus::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CLRCHK_HOME_STATUS_1, m_Home_Status[eAXIS_PRESS_UNIT_DN1]);
	DDX_Control(pDX, IDC_CLRCHK_HOME_STATUS_2, m_Home_Status[eAXIS_PRESS_UNIT_DN2]);
	DDX_Control(pDX, IDC_CLRCHK_HOME_STATUS_3, m_Home_Status[eAXIS_PRESS_UNIT_DN3]);
	DDX_Control(pDX, IDC_CLRCHK_HOME_STATUS_4, m_Home_Status[eAXIS_PRESS_UNIT_DN4]);
	DDX_Control(pDX, IDC_CLRCHK_HOME_STATUS_5, m_Home_Status[eAXIS_PRESS_UNIT_DN5]);
	DDX_Control(pDX, IDC_CLRCHK_HOME_STATUS_6, m_Home_Status[eAXIS_PRESS_UNIT_DN6]);
	DDX_Control(pDX, IDC_CLRCHK_HOME_STATUS_7, m_Home_Status[eAXIS_PRESS_UNIT_DN7]);
	DDX_Control(pDX, IDC_CLRCHK_HOME_STATUS_8, m_Home_Status[eAXIS_PRESS_UNIT_DN8]);

	DDX_Control(pDX, IDC_CLRCHK_HOME_STATUS_9, m_Home_Status[eAXIS_TEST_PP_1_X]);
	DDX_Control(pDX, IDC_CLRCHK_HOME_STATUS_10, m_Home_Status[eAXIS_TEST_PP_1_X2]);
	DDX_Control(pDX, IDC_CLRCHK_HOME_STATUS_11, m_Home_Status[eAXIS_TEST_PP_1_Y]);
	DDX_Control(pDX, IDC_CLRCHK_HOME_STATUS_12, m_Home_Status[eAXIS_TEST_PP_1_Z]);
	DDX_Control(pDX, IDC_CLRCHK_HOME_STATUS_13, m_Home_Status[eAXIS_TEST_PP_1_Z2]);
	DDX_Control(pDX, IDC_CLRCHK_HOME_STATUS_14, m_Home_Status[eAXIS_TEST_PP_1_X_PITCH]);
	DDX_Control(pDX, IDC_CLRCHK_HOME_STATUS_15, m_Home_Status[eAXIS_TEST_PP_1_X2_PITCH]);
	DDX_Control(pDX, IDC_CLRCHK_HOME_STATUS_16, m_Home_Status[eAXIS_TEST_PP_1_Y_PITCH]);
	DDX_Control(pDX, IDC_CLRCHK_HOME_STATUS_17, m_Home_Status[eAXIS_TEST_PP_1_Y2_PITCH]);

	DDX_Control(pDX, IDC_CLRCHK_HOME_STATUS_18, m_Home_Status[eAXIS_TEST_PP_2_X]);
	DDX_Control(pDX, IDC_CLRCHK_HOME_STATUS_19, m_Home_Status[eAXIS_TEST_PP_2_X2]);
	DDX_Control(pDX, IDC_CLRCHK_HOME_STATUS_20, m_Home_Status[eAXIS_TEST_PP_2_Y]);
	DDX_Control(pDX, IDC_CLRCHK_HOME_STATUS_21, m_Home_Status[eAXIS_TEST_PP_2_Z]);
	DDX_Control(pDX, IDC_CLRCHK_HOME_STATUS_22, m_Home_Status[eAXIS_TEST_PP_2_Z2]);
	DDX_Control(pDX, IDC_CLRCHK_HOME_STATUS_23, m_Home_Status[eAXIS_TEST_PP_2_X_PITCH]);
	DDX_Control(pDX, IDC_CLRCHK_HOME_STATUS_24, m_Home_Status[eAXIS_TEST_PP_2_X2_PITCH]);
	DDX_Control(pDX, IDC_CLRCHK_HOME_STATUS_25, m_Home_Status[eAXIS_TEST_PP_2_Y_PITCH]);
	DDX_Control(pDX, IDC_CLRCHK_HOME_STATUS_26, m_Home_Status[eAXIS_TEST_PP_2_Y2_PITCH]);


	DDX_Control(pDX, IDC_CLRCHK_HOME_STATUS_27, m_Home_Status[eAXIS_LOAD_TABLE_1_Y]);
	DDX_Control(pDX, IDC_CLRCHK_HOME_STATUS_28, m_Home_Status[eAXIS_LOAD_TABLE_2_Y]);
	DDX_Control(pDX, IDC_CLRCHK_HOME_STATUS_29, m_Home_Status[eAXIS_LOAD_TABLE_3_Y]);
	DDX_Control(pDX, IDC_CLRCHK_HOME_STATUS_30, m_Home_Status[eAXIS_LOAD_TABLE_4_Y]);

	DDX_Control(pDX, IDC_CLRCHK_HOME_STATUS_31, m_Home_Status[eAXIS_LOAD_TABLE_1_ROTATE]);
	DDX_Control(pDX, IDC_CLRCHK_HOME_STATUS_32, m_Home_Status[eAXIS_LOAD_TABLE_2_ROTATE]);
	DDX_Control(pDX, IDC_CLRCHK_HOME_STATUS_33, m_Home_Status[eAXIS_LOAD_TABLE_3_ROTATE]);
	DDX_Control(pDX, IDC_CLRCHK_HOME_STATUS_34, m_Home_Status[eAXIS_LOAD_TABLE_4_ROTATE]);

	DDX_Control(pDX, IDC_CLRCHK_HOME_STATUS_35, m_Home_Status[eAXIS_TRAY_PP_1_X]);
	DDX_Control(pDX, IDC_CLRCHK_HOME_STATUS_36, m_Home_Status[eAXIS_TRAY_FEEDER_Y]);
	DDX_Control(pDX, IDC_CLRCHK_HOME_STATUS_37, m_Home_Status[eAXIS_TRAY_PP_1_Z]);
 	DDX_Control(pDX, IDC_CLRCHK_HOME_STATUS_38, m_Home_Status[eAXIS_TRAY_PP_1_X_PITCH]);
 	DDX_Control(pDX, IDC_CLRCHK_HOME_STATUS_39, m_Home_Status[eAXIS_TRAY_PP_1_Y_PITCH]);

	DDX_Control(pDX, IDC_CLRCHK_HOME_STATUS_40, m_Home_Status[eAXIS_TRAY_PP_2_X]);
	DDX_Control(pDX, IDC_CLRCHK_HOME_STATUS_41, m_Home_Status[eAXIS_TRAY_PP_2_Y]);
	DDX_Control(pDX, IDC_CLRCHK_HOME_STATUS_42, m_Home_Status[eAXIS_TRAY_PP_2_Z]);
	DDX_Control(pDX, IDC_CLRCHK_HOME_STATUS_43, m_Home_Status[eAXIS_TRAY_PP_2_X_PITCH]);
	DDX_Control(pDX, IDC_CLRCHK_HOME_STATUS_44, m_Home_Status[eAXIS_TRAY_PP_2_Y_PITCH]);

	DDX_Control(pDX, IDC_CLRCHK_HOME_STATUS_45, m_Home_Status[eAXIS_TRANSFER_X]);

	DDX_Control(pDX, IDC_STATIC_HOME_CHECK, m_StaticHomeStatus);	
}


BOOL CDlgHomeCheckStatus::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect r;
	GetWindowRect(&r);
	::SetWindowPos(GetSafeHwnd(), HWND_TOPMOST, 150, 250, r.Width(), r.Height(), SWP_NOMOVE | SWP_NOSIZE);



	SetTimer(1,DEF_SENSOR_DISP_TM,NULL);

	m_StaticHomeStatus.m_fnSetFont("Fixedsys");
	m_StaticHomeStatus.SetTextSize(25);
	m_StaticHomeStatus.SetBackColor(COLOR_NOMAL);

	

	return TRUE;
}

void CDlgHomeCheckStatus::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	KillTimer(1);
}

void CDlgHomeCheckStatus::OnTimer( UINT_PTR nIDEvent )
{
	if( nIDEvent == 1 ){
		KillTimer(nIDEvent);
		UpdateHomeCheckStatus();
		SetTimer(nIDEvent, DEF_SENSOR_DISP_TM, NULL);
	}

	CDialogEx::OnTimer(nIDEvent);
}

BEGIN_MESSAGE_MAP(CDlgHomeCheckStatus, CDialogEx)
		ON_WM_TIMER()
		ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, &CDlgHomeCheckStatus::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgHomeCheckStatus 메시지 처리기입니다.


void CDlgHomeCheckStatus::UpdateHomeCheckStatus()
{
	CString strCurPos;
	
	int nMotorState = 0;

	for (int i = 0; i < eMAX_AXIS_COUNT; i++) {
		double dCurPos = 0;


		g_Motor[m_veAxisNum[i]]->GetCurrentPos(dCurPos);
		strCurPos.Format("%7.3f", (dCurPos / 1000.0));
		GetDlgItem(m_vdCurPos[i])->SetWindowText(strCurPos);

		if(g_Motor[m_veAxisNum[i]]->IsOrgCompleted(nullptr) == TRUE){
			m_Home_Status[m_veAxisNum[i]].On();
			nMotorState++;
		}else{
			m_Home_Status[m_veAxisNum[i]].Off();
		}
	}

	if (eMAX_AXIS_COUNT == nMotorState)
		m_StaticHomeStatus.SetBackColor(COLOR_GREEN);
	else
		m_StaticHomeStatus.SetBackColor(COLOR_RED);

}



void CDlgHomeCheckStatus::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}
