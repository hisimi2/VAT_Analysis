// DlgManuSetplate.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgManuSetplate.h"
#include "afxdialogex.h"

#include "../Include/OcxFiles/dsspanel.h"
#include "DEF_COMMON.h"
// CDlgManuSetplate 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgManuSetplate, CDialogEx)

CDlgManuSetplate::CDlgManuSetplate(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgManuSetplate::IDD, pParent)
{
	m_nCylManualOpActState = eCylManualOper_None;
	m_nCylManualActionStep = 0;
	m_nCmdManualCyl = eCylManualCmd_None;
	m_nCmdOperation = eCylManualOper_None;

	m_nCylManualOpActStateBackUp = eCylManualOper_None;

	m_nCmdParam_Time = 0;
	m_nCmdCycleCount = 0;
	m_dCylActionTimeRecord = 0;
	m_dTimeCheck = 0.0;
	m_nCylActionTimeRetry = 0;
	m_nCmdTaskerId = 0;
	m_nCmdParam_Loc = 0;

	m_nCylinderNum = 0;

	m_bSPEED_CONTROL_1 = FALSE;
	m_bSPEED_CONTROL_2 = FALSE;
	m_bSPEED_CONTROL_3 = FALSE;
	m_bSPEED_CONTROL_4 = FALSE;
	m_bSPEED_CONTROL_5 = FALSE;
	m_bSPEED_CONTROL_6 = FALSE;
	m_bSPEED_CONTROL_7 = FALSE;

	memset(&m_dCylinderSpec, 0x00, sizeof(m_dCylinderSpec));

	for (int i= 0; i < eMaxStackerCount; i++)
	{
		m_dCylinderSpec[eSTACKER_01 + i][eStackerMain][eStackerUp].dMin = 0.60;
		m_dCylinderSpec[eSTACKER_01 + i][eStackerMain][eStackerUp].dMax = 0.80;

		if (eSTACKER_01 == i || eSTACKER_03 == i)
		{
			m_dCylinderSpec[eSTACKER_01 + i][eStackerMain][eStackerDown].dMax = 0.80;
			m_dCylinderSpec[eSTACKER_01 + i][eStackerMain][eStackerDown].dMin = 0.60;
		}
		else
		{
			m_dCylinderSpec[eSTACKER_01 + i][eStackerMain][eStackerDown].dMax = 0.90;
			m_dCylinderSpec[eSTACKER_01 + i][eStackerMain][eStackerDown].dMin = 0.70;
		}
		

		m_dCylinderSpec[eSTACKER_01 + i][eStackerSub][eStackerUp].dMax = 0.30;
		m_dCylinderSpec[eSTACKER_01 + i][eStackerSub][eStackerUp].dMin = 0.20;
		m_dCylinderSpec[eSTACKER_01 + i][eStackerSub][eStackerDown].dMax = 0.30;
		m_dCylinderSpec[eSTACKER_01 + i][eStackerSub][eStackerDown].dMin = 0.20;

		m_dCylinderSpec[eSTACKER_01 + i][eStackerAlignCylinder][eStackerUp].dMax = 0.20;
		m_dCylinderSpec[eSTACKER_01 + i][eStackerAlignCylinder][eStackerUp].dMin = 0.10;
		m_dCylinderSpec[eSTACKER_01 + i][eStackerAlignCylinder][eStackerDown].dMax = 0.15;
		m_dCylinderSpec[eSTACKER_01 + i][eStackerAlignCylinder][eStackerDown].dMin = 0.05;
	}
}

CDlgManuSetplate::~CDlgManuSetplate()
{
	m_cBmpExit.DeleteObject();
}

void CDlgManuSetplate::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO_OPERATION, m_cbOperationSel);
	DDX_Control(pDX, IDC_COMBO_CONTINUE_TIME, m_cbTimeSel);

	DDX_Check(pDX, IDC_CHECK_CONVEYOR_SPEED1, m_bSPEED_CONTROL_1);
	DDX_Check(pDX, IDC_CHECK_CONVEYOR_SPEED2, m_bSPEED_CONTROL_2);
	DDX_Check(pDX, IDC_CHECK_CONVEYOR_SPEED3, m_bSPEED_CONTROL_3);
	DDX_Check(pDX, IDC_CHECK_CONVEYOR_SPEED4, m_bSPEED_CONTROL_4);
	DDX_Check(pDX, IDC_CHECK_CONVEYOR_SPEED5, m_bSPEED_CONTROL_5);
	DDX_Check(pDX, IDC_CHECK_CONVEYOR_SPEED6, m_bSPEED_CONTROL_6);
	DDX_Check(pDX, IDC_CHECK_CONVEYOR_SPEED7, m_bSPEED_CONTROL_7);

	DDX_Control(pDX, IDC_BTN_STK_MAIN_UP1,	 m_BtnSetplate1_Main_Up  );
	DDX_Control(pDX, IDC_BTN_STK_MAIN_DOWN1, m_BtnSetplate1_Main_Down);
	DDX_Control(pDX, IDC_BTN_STK_SUB_UP1,	 m_BtnSetplate1_Sub_Up	 );
	DDX_Control(pDX, IDC_BTN_STK_SUB_DOWN1,  m_BtnSetplate1_Sub_Down );
	DDX_Control(pDX, IDC_BTN_STK_CLAMP1,	 m_BtnSetplate1_Clamp	 );
	DDX_Control(pDX, IDC_BTN_STK_UNCLAMP1,	 m_BtnSetplate1_Unclamp	 );

	DDX_Control(pDX, IDC_BTN_STK_MAIN_UP2,	 m_BtnSetplate2_Main_Up  );
	DDX_Control(pDX, IDC_BTN_STK_MAIN_DOWN2, m_BtnSetplate2_Main_Down);
	DDX_Control(pDX, IDC_BTN_STK_SUB_UP2,	 m_BtnSetplate2_Sub_Up	 );
	DDX_Control(pDX, IDC_BTN_STK_SUB_DOWN2,  m_BtnSetplate2_Sub_Down );
	DDX_Control(pDX, IDC_BTN_STK_CLAMP2,	 m_BtnSetplate2_Clamp	 );
	DDX_Control(pDX, IDC_BTN_STK_UNCLAMP2,	 m_BtnSetplate2_Unclamp	 );

	DDX_Control(pDX, IDC_BTN_STK_MAIN_UP3,	 m_BtnSetplate3_Main_Up  );
	DDX_Control(pDX, IDC_BTN_STK_MAIN_DOWN3, m_BtnSetplate3_Main_Down);
	DDX_Control(pDX, IDC_BTN_STK_SUB_UP3,	 m_BtnSetplate3_Sub_Up	 );
	DDX_Control(pDX, IDC_BTN_STK_SUB_DOWN3,  m_BtnSetplate3_Sub_Down );
	DDX_Control(pDX, IDC_BTN_STK_CLAMP3,	 m_BtnSetplate3_Clamp	 );
	DDX_Control(pDX, IDC_BTN_STK_UNCLAMP3,	 m_BtnSetplate3_Unclamp	 );

	DDX_Control(pDX, IDC_BTN_STK_MAIN_UP4,	 m_BtnSetplate4_Main_Up  );
	DDX_Control(pDX, IDC_BTN_STK_MAIN_DOWN4, m_BtnSetplate4_Main_Down);
	DDX_Control(pDX, IDC_BTN_STK_SUB_UP4,	 m_BtnSetplate4_Sub_Up	 );
	DDX_Control(pDX, IDC_BTN_STK_SUB_DOWN4,  m_BtnSetplate4_Sub_Down );
	DDX_Control(pDX, IDC_BTN_STK_CLAMP4,	 m_BtnSetplate4_Clamp	 );
	DDX_Control(pDX, IDC_BTN_STK_UNCLAMP4,	 m_BtnSetplate4_Unclamp	 );

	DDX_Control(pDX, IDC_BTN_STK_MAIN_UP5,	 m_BtnSetplate5_Main_Up  );
	DDX_Control(pDX, IDC_BTN_STK_MAIN_DOWN5, m_BtnSetplate5_Main_Down);
	DDX_Control(pDX, IDC_BTN_STK_SUB_UP5,	 m_BtnSetplate5_Sub_Up	 );
	DDX_Control(pDX, IDC_BTN_STK_SUB_DOWN5,  m_BtnSetplate5_Sub_Down );
	DDX_Control(pDX, IDC_BTN_STK_CLAMP5,	 m_BtnSetplate5_Clamp	 );
	DDX_Control(pDX, IDC_BTN_STK_UNCLAMP5,	 m_BtnSetplate5_Unclamp	 );

	DDX_Control(pDX, IDC_BTN_STK_MAIN_UP6,	 m_BtnSetplate6_Main_Up  );
	DDX_Control(pDX, IDC_BTN_STK_MAIN_DOWN6, m_BtnSetplate6_Main_Down);
	DDX_Control(pDX, IDC_BTN_STK_SUB_UP6,	 m_BtnSetplate6_Sub_Up	 );
	DDX_Control(pDX, IDC_BTN_STK_SUB_DOWN6,  m_BtnSetplate6_Sub_Down );
	DDX_Control(pDX, IDC_BTN_STK_CLAMP6,	 m_BtnSetplate6_Clamp	 );
	DDX_Control(pDX, IDC_BTN_STK_UNCLAMP6,	 m_BtnSetplate6_Unclamp	 );

	DDX_Control(pDX, IDC_BTN_STK_MAIN_UP7,	 m_BtnSetplate7_Main_Up  );
	DDX_Control(pDX, IDC_BTN_STK_MAIN_DOWN7, m_BtnSetplate7_Main_Down);
	DDX_Control(pDX, IDC_BTN_STK_SUB_UP7,	 m_BtnSetplate7_Sub_Up	 );
	DDX_Control(pDX, IDC_BTN_STK_SUB_DOWN7,  m_BtnSetplate7_Sub_Down );
	DDX_Control(pDX, IDC_BTN_STK_CLAMP7,	 m_BtnSetplate7_Clamp	 );
	DDX_Control(pDX, IDC_BTN_STK_UNCLAMP7,	 m_BtnSetplate7_Unclamp	 );


	DDX_Control(pDX, IDC_BTN_STK_ALL_LOCK,		 m_BtnConveyor_All_Lock	 );
	DDX_Control(pDX, IDC_BTN_STK_ALL_UNLOCK,	 m_BtnConveyor_All_Unlock );
	DDX_Control(pDX,   IDC_BTN_STK_LOCK1,		 m_BtnConveyor1_Lock	 );
	DDX_Control(pDX, IDC_BTN_STK_UNLOCK1,		 m_BtnConveyor1_Unlock	 );
	DDX_Control(pDX,   IDC_BTN_STK_LOCK2,		 m_BtnConveyor2_Lock	 );
	DDX_Control(pDX, IDC_BTN_STK_UNLOCK2,		 m_BtnConveyor2_Unlock	 );
	DDX_Control(pDX,   IDC_BTN_STK_LOCK3,		 m_BtnConveyor3_Lock	 );
	DDX_Control(pDX, IDC_BTN_STK_UNLOCK3,		 m_BtnConveyor3_Unlock	 );
	DDX_Control(pDX,   IDC_BTN_STK_LOCK4,		 m_BtnConveyor4_Lock	 );
	DDX_Control(pDX, IDC_BTN_STK_UNLOCK4,		 m_BtnConveyor4_Unlock	 );
	DDX_Control(pDX,   IDC_BTN_STK_LOCK5,		 m_BtnConveyor5_Lock	 );
	DDX_Control(pDX, IDC_BTN_STK_UNLOCK5,		 m_BtnConveyor5_Unlock	 );
	DDX_Control(pDX,   IDC_BTN_STK_LOCK6,		 m_BtnConveyor6_Lock	 );
	DDX_Control(pDX, IDC_BTN_STK_UNLOCK6,		 m_BtnConveyor6_Unlock	 );
	DDX_Control(pDX,   IDC_BTN_STK_LOCK7,		 m_BtnConveyor7_Lock	 );
	DDX_Control(pDX, IDC_BTN_STK_UNLOCK7,		 m_BtnConveyor7_Unlock	 );

	DDX_Control( pDX, IDC_BTN_STK1_VIBRATION_FORWARD, m_BtnStacker1_Forward );
	DDX_Control( pDX, IDC_BTN_STK1_VIBRATION_BACK, m_BtnStacker1_Back );
	DDX_Control( pDX, IDC_BTN_STK2_VIBRATION_FORWARD, m_BtnStacker2_Forward );
	DDX_Control( pDX, IDC_BTN_STK2_VIBRATION_BACK, m_BtnStacker2_Back );
	DDX_Control( pDX, IDC_BTN_STK3_VIBRATION_FORWARD, m_BtnStacker3_Forward );
	DDX_Control( pDX, IDC_BTN_STK3_VIBRATION_BACK, m_BtnStacker3_Back );
	DDX_Control( pDX, IDC_BTN_STK4_VIBRATION_FORWARD, m_BtnStacker4_Forward );
	DDX_Control( pDX, IDC_BTN_STK4_VIBRATION_BACK, m_BtnStacker4_Back );
	
	DDX_Control(pDX, IDC_BUTTON_SETPLATE_STACKER_CYL_ACT_STOP, m_BtnStop);

	// exit
	DDX_Control(pDX, IDOK, m_btnExit);
}

BOOL CDlgManuSetplate::IsSubUp( int _index )
{
	BOOL bRet = FALSE;
	if( g_TaskStacker[ _index ].get_cyl_SubPusher(DEF_UP) )
		bRet = TRUE;

	return bRet;
}

BOOL CDlgManuSetplate::IsLock( int _index )
{
	BOOL bRet = FALSE;

	if( g_TaskStacker[ _index ].get_cyl_LockUnlock( DEF_LOCK ) )
		bRet = TRUE;

	return bRet;
}


BEGIN_MESSAGE_MAP(CDlgManuSetplate, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_SETPLATE_STACKER_CYL_ACT_STOP, &CDlgManuSetplate::OnBnClickedButtonSetplateStackerCylActStop)

	ON_BN_CLICKED(IDC_BTN_STK_MAIN_UP1, &CDlgManuSetplate::OnBnClickedBtnStkMainUp1)
	ON_BN_CLICKED(IDC_BTN_STK_MAIN_DOWN1, &CDlgManuSetplate::OnBnClickedBtnStkMainDown1)
	ON_BN_CLICKED(IDC_BTN_STK_SUB_UP1, &CDlgManuSetplate::OnBnClickedBtnStkSubUp1)
	ON_BN_CLICKED(IDC_BTN_STK_SUB_DOWN1, &CDlgManuSetplate::OnBnClickedBtnStkSubDown1)
	ON_BN_CLICKED(IDC_BTN_STK_CLAMP1, &CDlgManuSetplate::OnBnClickedBtnStkClamp1)
	ON_BN_CLICKED(IDC_BTN_STK_UNCLAMP1, &CDlgManuSetplate::OnBnClickedBtnStkUnclamp1)
	ON_BN_CLICKED(IDC_BTN_STK_MAIN_UP2, &CDlgManuSetplate::OnBnClickedBtnStkMainUp2)
	ON_BN_CLICKED(IDC_BTN_STK_MAIN_DOWN2, &CDlgManuSetplate::OnBnClickedBtnStkMainDown2)
	ON_BN_CLICKED(IDC_BTN_STK_SUB_UP2, &CDlgManuSetplate::OnBnClickedBtnStkSubUp2)
	ON_BN_CLICKED(IDC_BTN_STK_SUB_DOWN2, &CDlgManuSetplate::OnBnClickedBtnStkSubDown2)
	ON_BN_CLICKED(IDC_BTN_STK_CLAMP2, &CDlgManuSetplate::OnBnClickedBtnStkClamp2)
	ON_BN_CLICKED(IDC_BTN_STK_UNCLAMP2, &CDlgManuSetplate::OnBnClickedBtnStkUnclamp2)
	ON_BN_CLICKED(IDC_BTN_STK_MAIN_UP3, &CDlgManuSetplate::OnBnClickedBtnStkMainUp3)
	ON_BN_CLICKED(IDC_BTN_STK_MAIN_UP4, &CDlgManuSetplate::OnBnClickedBtnStkMainUp4)
	ON_BN_CLICKED(IDC_BTN_STK_MAIN_UP5, &CDlgManuSetplate::OnBnClickedBtnStkMainUp5)
	ON_BN_CLICKED(IDC_BTN_STK_MAIN_UP6, &CDlgManuSetplate::OnBnClickedBtnStkMainUp6)
	ON_BN_CLICKED(IDC_BTN_STK_MAIN_UP7, &CDlgManuSetplate::OnBnClickedBtnStkMainUp7)
	ON_BN_CLICKED(IDC_BTN_STK_MAIN_DOWN3, &CDlgManuSetplate::OnBnClickedBtnStkMainDown3)
	ON_BN_CLICKED(IDC_BTN_STK_MAIN_DOWN4, &CDlgManuSetplate::OnBnClickedBtnStkMainDown4)
	ON_BN_CLICKED(IDC_BTN_STK_MAIN_DOWN5, &CDlgManuSetplate::OnBnClickedBtnStkMainDown5)
	ON_BN_CLICKED(IDC_BTN_STK_MAIN_DOWN6, &CDlgManuSetplate::OnBnClickedBtnStkMainDown6)
	ON_BN_CLICKED(IDC_BTN_STK_MAIN_DOWN7, &CDlgManuSetplate::OnBnClickedBtnStkMainDown7)
	ON_BN_CLICKED(IDC_BTN_STK_SUB_UP3, &CDlgManuSetplate::OnBnClickedBtnStkSubUp3)
	ON_BN_CLICKED(IDC_BTN_STK_SUB_UP4, &CDlgManuSetplate::OnBnClickedBtnStkSubUp4)
	ON_BN_CLICKED(IDC_BTN_STK_SUB_UP5, &CDlgManuSetplate::OnBnClickedBtnStkSubUp5)
	ON_BN_CLICKED(IDC_BTN_STK_SUB_UP6, &CDlgManuSetplate::OnBnClickedBtnStkSubUp6)
	ON_BN_CLICKED(IDC_BTN_STK_SUB_UP7, &CDlgManuSetplate::OnBnClickedBtnStkSubUp7)
	ON_BN_CLICKED(IDC_BTN_STK_SUB_DOWN3, &CDlgManuSetplate::OnBnClickedBtnStkSubDown3)
	ON_BN_CLICKED(IDC_BTN_STK_SUB_DOWN4, &CDlgManuSetplate::OnBnClickedBtnStkSubDown4)
	ON_BN_CLICKED(IDC_BTN_STK_SUB_DOWN5, &CDlgManuSetplate::OnBnClickedBtnStkSubDown5)
	ON_BN_CLICKED(IDC_BTN_STK_SUB_DOWN6, &CDlgManuSetplate::OnBnClickedBtnStkSubDown6)
	ON_BN_CLICKED(IDC_BTN_STK_SUB_DOWN7, &CDlgManuSetplate::OnBnClickedBtnStkSubDown7)
	ON_BN_CLICKED(IDC_BTN_STK_CLAMP3, &CDlgManuSetplate::OnBnClickedBtnStkClamp3)
	ON_BN_CLICKED(IDC_BTN_STK_CLAMP4, &CDlgManuSetplate::OnBnClickedBtnStkClamp4)
	ON_BN_CLICKED(IDC_BTN_STK_CLAMP5, &CDlgManuSetplate::OnBnClickedBtnStkClamp5)
	ON_BN_CLICKED(IDC_BTN_STK_CLAMP6, &CDlgManuSetplate::OnBnClickedBtnStkClamp6)
	ON_BN_CLICKED(IDC_BTN_STK_CLAMP7, &CDlgManuSetplate::OnBnClickedBtnStkClamp7)
	ON_BN_CLICKED(IDC_BTN_STK_UNCLAMP3, &CDlgManuSetplate::OnBnClickedBtnStkUnclamp3)
	ON_BN_CLICKED(IDC_BTN_STK_UNCLAMP4, &CDlgManuSetplate::OnBnClickedBtnStkUnclamp4)
	ON_BN_CLICKED(IDC_BTN_STK_UNCLAMP5, &CDlgManuSetplate::OnBnClickedBtnStkUnclamp5)
	ON_BN_CLICKED(IDC_BTN_STK_UNCLAMP6, &CDlgManuSetplate::OnBnClickedBtnStkUnclamp6)
	ON_BN_CLICKED(IDC_BTN_STK_UNCLAMP7, &CDlgManuSetplate::OnBnClickedBtnStkUnclamp7)
	ON_BN_CLICKED(IDC_BTN_STK_ALL_LOCK, &CDlgManuSetplate::OnBnClickedBtnStkAllForward)
	ON_BN_CLICKED(IDC_BTN_STK_LOCK1, &CDlgManuSetplate::OnBnClickedBtnStkForward1)
	ON_BN_CLICKED(IDC_BTN_STK_LOCK2, &CDlgManuSetplate::OnBnClickedBtnStkForward2)
	ON_BN_CLICKED(IDC_BTN_STK_LOCK3, &CDlgManuSetplate::OnBnClickedBtnStkForward3)
	ON_BN_CLICKED(IDC_BTN_STK_LOCK4, &CDlgManuSetplate::OnBnClickedBtnStkForward4)
	ON_BN_CLICKED(IDC_BTN_STK_LOCK5, &CDlgManuSetplate::OnBnClickedBtnStkForward5)
	ON_BN_CLICKED(IDC_BTN_STK_LOCK6, &CDlgManuSetplate::OnBnClickedBtnStkForward6)
	ON_BN_CLICKED(IDC_BTN_STK_LOCK7, &CDlgManuSetplate::OnBnClickedBtnStkForward7)
	ON_BN_CLICKED(IDC_BTN_STK_ALL_UNLOCK, &CDlgManuSetplate::OnBnClickedBtnStkAllUnlock)
	ON_BN_CLICKED(IDC_BTN_STK_UNLOCK1, &CDlgManuSetplate::OnBnClickedBtnStkBack1)
	ON_BN_CLICKED(IDC_BTN_STK_UNLOCK2, &CDlgManuSetplate::OnBnClickedBtnStkBack2)
	ON_BN_CLICKED(IDC_BTN_STK_UNLOCK3, &CDlgManuSetplate::OnBnClickedBtnStkBack3)
	ON_BN_CLICKED(IDC_BTN_STK_UNLOCK4, &CDlgManuSetplate::OnBnClickedBtnStkBack4)
	ON_BN_CLICKED(IDC_BTN_STK_UNLOCK5, &CDlgManuSetplate::OnBnClickedBtnStkBack5)
	ON_BN_CLICKED(IDC_BTN_STK_UNLOCK6, &CDlgManuSetplate::OnBnClickedBtnStkBack6)
	ON_BN_CLICKED(IDC_BTN_STK_UNLOCK7, &CDlgManuSetplate::OnBnClickedBtnStkBack7)
	ON_BN_CLICKED( IDC_BTN_STK1_VIBRATION_FORWARD, &CDlgManuSetplate::OnBnClickedBtnStk1VibrationForward )
	ON_BN_CLICKED( IDC_BTN_STK1_VIBRATION_BACK, &CDlgManuSetplate::OnBnClickedBtnStk1VibrationBack )
	ON_BN_CLICKED( IDC_BTN_STK2_VIBRATION_FORWARD, &CDlgManuSetplate::OnBnClickedBtnStk2VibrationForward )
	ON_BN_CLICKED( IDC_BTN_STK2_VIBRATION_BACK, &CDlgManuSetplate::OnBnClickedBtnStk2VibrationBack )
	ON_BN_CLICKED( IDC_BTN_STK3_VIBRATION_FORWARD, &CDlgManuSetplate::OnBnClickedBtnStk3VibrationForward )
	ON_BN_CLICKED( IDC_BTN_STK3_VIBRATION_BACK, &CDlgManuSetplate::OnBnClickedBtnStk3VibrationBack )
	ON_BN_CLICKED( IDC_BTN_STK4_VIBRATION_FORWARD, &CDlgManuSetplate::OnBnClickedBtnStk4VibrationForward )
	ON_BN_CLICKED( IDC_BTN_STK4_VIBRATION_BACK, &CDlgManuSetplate::OnBnClickedBtnStk4VibrationBack )
	ON_BN_CLICKED(IDOK, &CDlgManuSetplate::OnBnClickedOk)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CDlgManuSetplate, CDialogEx)
	ON_EVENT(CDlgManuSetplate, IDC_COLORBTCTRL_CONVEYOR_1_BACK, DISPID_MOUSEUP, CDlgManuSetplate::MouseUpColorbtctrlConveyor1Back, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuSetplate, IDC_COLORBTCTRL_CONVEYOR_1_BACK, DISPID_MOUSEDOWN, CDlgManuSetplate::MouseDownColorbtctrlConveyor1Back, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuSetplate, IDC_COLORBTCTRL_CONVEYOR_1_BACK2, DISPID_MOUSEUP, CDlgManuSetplate::MouseUpColorbtctrlConveyor1Back2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuSetplate, IDC_COLORBTCTRL_CONVEYOR_1_BACK2, DISPID_MOUSEDOWN, CDlgManuSetplate::MouseDownColorbtctrlConveyor1Back2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuSetplate, IDC_COLORBTCTRL_CONVEYOR_1_BACK3, DISPID_MOUSEUP, CDlgManuSetplate::MouseUpColorbtctrlConveyor1Back3, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuSetplate, IDC_COLORBTCTRL_CONVEYOR_1_BACK3, DISPID_MOUSEDOWN, CDlgManuSetplate::MouseDownColorbtctrlConveyor1Back3, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuSetplate, IDC_COLORBTCTRL_CONVEYOR_1_BACK4, DISPID_MOUSEUP, CDlgManuSetplate::MouseUpColorbtctrlConveyor1Back4, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuSetplate, IDC_COLORBTCTRL_CONVEYOR_1_BACK4, DISPID_MOUSEDOWN, CDlgManuSetplate::MouseDownColorbtctrlConveyor1Back4, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuSetplate, IDC_COLORBTCTRL_CONVEYOR_1_BACK5, DISPID_MOUSEUP, CDlgManuSetplate::MouseUpColorbtctrlConveyor1Back5, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuSetplate, IDC_COLORBTCTRL_CONVEYOR_1_BACK5, DISPID_MOUSEDOWN, CDlgManuSetplate::MouseDownColorbtctrlConveyor1Back5, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuSetplate, IDC_COLORBTCTRL_CONVEYOR_1_BACK6, DISPID_MOUSEUP, CDlgManuSetplate::MouseUpColorbtctrlConveyor1Back6, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuSetplate, IDC_COLORBTCTRL_CONVEYOR_1_BACK6, DISPID_MOUSEDOWN, CDlgManuSetplate::MouseDownColorbtctrlConveyor1Back6, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuSetplate, IDC_COLORBTCTRL_CONVEYOR_1_BACK7, DISPID_MOUSEUP, CDlgManuSetplate::MouseUpColorbtctrlConveyor1Back7, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuSetplate, IDC_COLORBTCTRL_CONVEYOR_1_BACK7, DISPID_MOUSEDOWN, CDlgManuSetplate::MouseDownColorbtctrlConveyor1Back7, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuSetplate, IDC_COLORBTCTRL_CONVEYOR_1_FOR, DISPID_MOUSEUP, CDlgManuSetplate::MouseUpColorbtctrlConveyor1For, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuSetplate, IDC_COLORBTCTRL_CONVEYOR_1_FOR, DISPID_MOUSEDOWN, CDlgManuSetplate::MouseDownColorbtctrlConveyor1For, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuSetplate, IDC_COLORBTCTRL_CONVEYOR_1_FOR2, DISPID_MOUSEUP, CDlgManuSetplate::MouseUpColorbtctrlConveyor1For2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuSetplate, IDC_COLORBTCTRL_CONVEYOR_1_FOR2, DISPID_MOUSEDOWN, CDlgManuSetplate::MouseDownColorbtctrlConveyor1For2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuSetplate, IDC_COLORBTCTRL_CONVEYOR_1_FOR3, DISPID_MOUSEUP, CDlgManuSetplate::MouseUpColorbtctrlConveyor1For3, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuSetplate, IDC_COLORBTCTRL_CONVEYOR_1_FOR3, DISPID_MOUSEDOWN, CDlgManuSetplate::MouseDownColorbtctrlConveyor1For3, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuSetplate, IDC_COLORBTCTRL_CONVEYOR_1_FOR4, DISPID_MOUSEUP, CDlgManuSetplate::MouseUpColorbtctrlConveyor1For4, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuSetplate, IDC_COLORBTCTRL_CONVEYOR_1_FOR4, DISPID_MOUSEDOWN, CDlgManuSetplate::MouseDownColorbtctrlConveyor1For4, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuSetplate, IDC_COLORBTCTRL_CONVEYOR_1_FOR5, DISPID_MOUSEUP, CDlgManuSetplate::MouseUpColorbtctrlConveyor1For5, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuSetplate, IDC_COLORBTCTRL_CONVEYOR_1_FOR5, DISPID_MOUSEDOWN, CDlgManuSetplate::MouseDownColorbtctrlConveyor1For5, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuSetplate, IDC_COLORBTCTRL_CONVEYOR_1_FOR6, DISPID_MOUSEUP, CDlgManuSetplate::MouseUpColorbtctrlConveyor1For6, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuSetplate, IDC_COLORBTCTRL_CONVEYOR_1_FOR6, DISPID_MOUSEDOWN, CDlgManuSetplate::MouseDownColorbtctrlConveyor1For6, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuSetplate, IDC_COLORBTCTRL_CONVEYOR_1_FOR7, DISPID_MOUSEUP, CDlgManuSetplate::MouseUpColorbtctrlConveyor1For7, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuSetplate, IDC_COLORBTCTRL_CONVEYOR_1_FOR7, DISPID_MOUSEDOWN, CDlgManuSetplate::MouseDownColorbtctrlConveyor1For7, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

// CDlgManuSetplate 메시지 처리기입니다.

BOOL CDlgManuSetplate::PreTranslateMessage(MSG* pMsg)
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

void CDlgManuSetplate::SetBtnDisplay()
{
	m_BtnStop.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnStop.SetIcon(IDI_STOP);
	m_BtnStop.SetAlign(CButtonST::ST_ALIGN_VERT);
}

BOOL CDlgManuSetplate::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// exit
	m_cBmpExit.LoadBitmap(IDB_EXIT_CYLINDER_STACKER_SETPLATE);
	m_btnExit.SetBitmap(m_cBmpExit);

// 	m_pStackerSenView = new CDlgManuStackerClampSen(TASK_STACKER_LD, eSTACKER_1, this);
// 	m_pStackerSenView->Create(IDD_MANU_STACKER_CLAMP_SEN, this);
// 	CRect rectFrame, rectView;
// 	GetDlgItem(IDC_STATIC_STACKER_SEN_VIEW_FRAME)->GetWindowRect(&rectFrame);
// 	ScreenToClient(&rectFrame);
// 	m_pStackerSenView->GetClientRect(&rectView);
// 	ScreenToClient(&rectView);
// 	int x,y,w,h;
// 	x = rectFrame.left + ((rectFrame.Width()-rectView.Width())/2);
// 	y = rectFrame.top + 5;
// 	w = rectView.Width();
// 	h = rectFrame.Height();
// 	m_pStackerSenView->MoveWindow(x,y,w,h);
// 	m_pStackerSenView->ShowWindow(SW_SHOW);

	m_cbOperationSel.Clear();
	m_cbOperationSel.AddString("1: Only One Action & No Check Sensor");
	m_cbOperationSel.AddString("2: Only One Action & Check Sensor");
	m_cbOperationSel.AddString("3: Continue Action & No Check Sensor");
	m_cbOperationSel.AddString("4: Continue Action & Check Sensor");
	m_cbOperationSel.AddString("5: Time Check Action (Once & Sensor Check)");
	m_cbOperationSel.SetCurSel(0);

	m_cbTimeSel.SetCurSel(0);

	SetBtnDisplay();

	SetTimer(0, DEF_SENSOR_DISP_TM, NULL);

	CreateThread(0);
	
	MakeLog("Set plate & Stacker Dialog Show");

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgManuSetplate::OnDestroy()
{
	CDialogEx::OnDestroy();

	KillTimer(0);
	CloseThread();
	//if( m_pStackerSenView ){
	//	m_pStackerSenView->DestroyWindow();
	//	delete m_pStackerSenView;
	//	m_pStackerSenView = NULL;
	//}
}


void CDlgManuSetplate::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CDlgManuSetplate::OnTimer(UINT_PTR nIDEvent)
{
	CDSSPanel*    pSensor = NULL;
	CMFCButton*   pButton = NULL;

	if(nIDEvent == 0)
	{
		KillTimer(nIDEvent);

		// Stacker1 ~ 7, clamp/unclamp
		int nStackerMainBtnId[eMaxStackerCount][2] = 
		{
			{IDC_BTN_STK_MAIN_UP1,IDC_BTN_STK_MAIN_DOWN1},{IDC_BTN_STK_MAIN_UP2,IDC_BTN_STK_MAIN_DOWN2},{IDC_BTN_STK_MAIN_UP3,IDC_BTN_STK_MAIN_DOWN3},
			{IDC_BTN_STK_MAIN_UP4,IDC_BTN_STK_MAIN_DOWN4},{IDC_BTN_STK_MAIN_UP5,IDC_BTN_STK_MAIN_DOWN5},{IDC_BTN_STK_MAIN_UP6,IDC_BTN_STK_MAIN_DOWN6},
			{IDC_BTN_STK_MAIN_UP7,IDC_BTN_STK_MAIN_DOWN7}
		};
		int nStackerSubBtnId[eMaxStackerCount][2] = 
		{
			{IDC_BTN_STK_SUB_UP1,IDC_BTN_STK_SUB_DOWN1},{IDC_BTN_STK_SUB_UP2,IDC_BTN_STK_SUB_DOWN2},{IDC_BTN_STK_SUB_UP3,IDC_BTN_STK_SUB_DOWN3},
			{IDC_BTN_STK_SUB_UP4,IDC_BTN_STK_SUB_DOWN4},{IDC_BTN_STK_SUB_UP5,IDC_BTN_STK_SUB_DOWN5},{IDC_BTN_STK_SUB_UP6,IDC_BTN_STK_SUB_DOWN6},
			{IDC_BTN_STK_SUB_UP7,IDC_BTN_STK_SUB_DOWN7}
		};
		int nStackerClampBtnId[eMaxStackerCount][2] = 
		{
			{IDC_BTN_STK_CLAMP1,IDC_BTN_STK_UNCLAMP1},{IDC_BTN_STK_CLAMP2,IDC_BTN_STK_UNCLAMP2},{IDC_BTN_STK_CLAMP3,IDC_BTN_STK_UNCLAMP3},
			{IDC_BTN_STK_CLAMP4,IDC_BTN_STK_UNCLAMP4},{IDC_BTN_STK_CLAMP5,IDC_BTN_STK_UNCLAMP5},{IDC_BTN_STK_CLAMP6,IDC_BTN_STK_UNCLAMP6},
			{IDC_BTN_STK_CLAMP7,IDC_BTN_STK_UNCLAMP7}
		};

		int nStackerClampSetID[eMaxStackerCount][4] =
		{
			{ IDC_STACKER_LOCK_RL1,IDC_STACKER_LOCK_RR1,IDC_STACKER_LOCK_FL1,IDC_STACKER_LOCK_FR1 },
			{ IDC_STACKER_LOCK_RL2,IDC_STACKER_LOCK_RR2,IDC_STACKER_LOCK_FL2,IDC_STACKER_LOCK_FR2 },
			{ IDC_STACKER_LOCK_RL3,IDC_STACKER_LOCK_RR3,IDC_STACKER_LOCK_FL3,IDC_STACKER_LOCK_FR3 },
			{ IDC_STACKER_LOCK_RL4,IDC_STACKER_LOCK_RR4,IDC_STACKER_LOCK_FL4,IDC_STACKER_LOCK_FR4 },
			{ IDC_STACKER_LOCK_RL5,IDC_STACKER_LOCK_RR5,IDC_STACKER_LOCK_FL5,IDC_STACKER_LOCK_FR5 },
			{ IDC_STACKER_LOCK_RL6,IDC_STACKER_LOCK_RR6,IDC_STACKER_LOCK_FL6,IDC_STACKER_LOCK_FR6 },
			{ IDC_STACKER_LOCK_RL7,IDC_STACKER_LOCK_RR7,IDC_STACKER_LOCK_FL7,IDC_STACKER_LOCK_FR7 }
		};

		int nConveyorLockBtnId[eMaxStackerCount][2] =
		{
			{IDC_BTN_STK_LOCK1,IDC_BTN_STK_UNLOCK1},{IDC_BTN_STK_LOCK2,IDC_BTN_STK_UNLOCK2},{IDC_BTN_STK_LOCK3,IDC_BTN_STK_UNLOCK3},
			{IDC_BTN_STK_LOCK4,IDC_BTN_STK_UNLOCK4},{IDC_BTN_STK_LOCK5,IDC_BTN_STK_UNLOCK5},{IDC_BTN_STK_LOCK6,IDC_BTN_STK_UNLOCK6},
			{IDC_BTN_STK_LOCK7,IDC_BTN_STK_UNLOCK7}
		};

		int nStackerTopSen[eMaxStackerCount] = 
		{
			IDC_SETPLATE_TRAY_TOP_SEN1,IDC_SETPLATE_TRAY_TOP_SEN2,IDC_SETPLATE_TRAY_TOP_SEN3,IDC_SETPLATE_TRAY_TOP_SEN4,
			IDC_SETPLATE_TRAY_TOP_SEN5,IDC_SETPLATE_TRAY_TOP_SEN6,IDC_SETPLATE_TRAY_TOP_SEN7
		};

		int nStackerBottomSen[eMaxStackerCount] = 
		{
			IDC_SETPLATE_TRAY_BOTTOM_SEN1,IDC_SETPLATE_TRAY_BOTTOM_SEN2,IDC_SETPLATE_TRAY_BOTTOM_SEN3,IDC_SETPLATE_TRAY_BOTTOM_SEN4,
			IDC_SETPLATE_TRAY_BOTTOM_SEN5,IDC_SETPLATE_TRAY_BOTTOM_SEN6,IDC_SETPLATE_TRAY_BOTTOM_SEN7
		};

		int nConveyorArriveSen[eMaxStackerCount] = 
		{
			IDC_SETPLATE_CONVEYOR_ARRIVE_SEN1,IDC_SETPLATE_CONVEYOR_ARRIVE_SEN2,IDC_SETPLATE_CONVEYOR_ARRIVE_SEN3,
			IDC_SETPLATE_CONVEYOR_ARRIVE_SEN4,IDC_SETPLATE_CONVEYOR_ARRIVE_SEN5,IDC_SETPLATE_CONVEYOR_ARRIVE_SEN6,
			IDC_SETPLATE_CONVEYOR_ARRIVE_SEN7
		};

		int nConveyorArriveSlowSen[eMaxStackerCount] =
		{
			IDC_SETPLATE_CONVEYOR_ARRIVE_SLOW_SEN1,IDC_SETPLATE_CONVEYOR_ARRIVE_SLOW_SEN2,IDC_SETPLATE_CONVEYOR_ARRIVE_SLOW_SEN3,
			IDC_SETPLATE_CONVEYOR_ARRIVE_SLOW_SEN4,IDC_SETPLATE_CONVEYOR_ARRIVE_SLOW_SEN5,IDC_SETPLATE_CONVEYOR_ARRIVE_SLOW_SEN6,
			IDC_SETPLATE_CONVEYOR_ARRIVE_SLOW_SEN7
		};

		int nConveyorCTraySlowSen[eMaxStackerCount] = 
		{
			IDC_SETPLATE_CONVEYOR_START_SLOW_SEN1,IDC_SETPLATE_CONVEYOR_START_SLOW_SEN2,IDC_SETPLATE_CONVEYOR_START_SLOW_SEN3,
			IDC_SETPLATE_CONVEYOR_START_SLOW_SEN4,IDC_SETPLATE_CONVEYOR_START_SLOW_SEN5,IDC_SETPLATE_CONVEYOR_START_SLOW_SEN6,
			IDC_SETPLATE_CONVEYOR_START_SLOW_SEN7
		};

		int nConveyorCTraySen[eMaxStackerCount] = 
		{
			IDC_SETPLATE_CONVEYOR_START_SEN1,IDC_SETPLATE_CONVEYOR_START_SEN2,IDC_SETPLATE_CONVEYOR_START_SEN3,
			IDC_SETPLATE_CONVEYOR_START_SEN4,IDC_SETPLATE_CONVEYOR_START_SEN5,IDC_SETPLATE_CONVEYOR_START_SEN6,
			IDC_SETPLATE_CONVEYOR_START_SEN7
		};


		for(int k=0; k<eMaxStackerCount; k++)
		{
			// main up , down button
			BOOL Ret_Main_UP = FALSE, Ret_Main_DOWN = FALSE;
			Ret_Main_UP = g_TaskStacker[ k ].GetDI( CTaskStacker::xC_TRAY_MAIN_PUSHER_UP_SEN );
			Ret_Main_DOWN = g_TaskStacker[ k ].GetDI( CTaskStacker::xC_TRAY_MAIN_PUSHER_DOWN_SEN );
			UpdateCylinderSts( nStackerMainBtnId[k][0] , Ret_Main_UP );
			UpdateCylinderSts( nStackerMainBtnId[k][1] , Ret_Main_DOWN );
	

			// sub up , down button
			BOOL Ret_Sub_UP = FALSE, Ret_Sub_DOWN = FALSE;
			Ret_Sub_UP = g_TaskStacker[ k ].GetDI( CTaskStacker::xC_TRAY_SUB_PUSHER_UP_SEN );
			Ret_Sub_DOWN = g_TaskStacker[ k ].GetDI( CTaskStacker::xC_TRAY_SUB_PUSHER_DOWN_SEN );
			UpdateCylinderSts( nStackerSubBtnId[k][0] , Ret_Sub_UP );
			UpdateCylinderSts( nStackerSubBtnId[k][1] , Ret_Sub_DOWN );

			// lock,unlock button
			BOOL Ret_Lock = FALSE; int nLockUnlockCnt = 0;
			nLockUnlockCnt += (int)g_TaskStacker[ k ].GetDI( CTaskStacker::xC_TRAY_REAR_LEFT_LOCK_SEN );
			nLockUnlockCnt += (int)g_TaskStacker[ k ].GetDI( CTaskStacker::xC_TRAY_REAR_RIGHT_LOCK_SEN );
			nLockUnlockCnt += (int)g_TaskStacker[ k ].GetDI( CTaskStacker::xC_TRAY_FRONT_LEFT_LOCK_SEN );
			nLockUnlockCnt += (int)g_TaskStacker[ k ].GetDI( CTaskStacker::xC_TRAY_FRONT_RIGHT_LOCK_SEN );
			UpdateCylinderSts(nStackerClampBtnId[k][0], (nLockUnlockCnt == 4 ? TRUE : FALSE) );//Ret_Lock );
			UpdateCylinderSts(nStackerClampBtnId[k][1], (nLockUnlockCnt == 0 ? TRUE : FALSE));//!( Ret_Lock ) );

			// Conveyor Lock Unlock
			BOOL Ret_Align_Forward = FALSE, Ret_Align_Back = FALSE;
			Ret_Align_Forward = g_TaskStacker[ k ].GetDI( CTaskStacker::xC_TRAY_ALIGN_FOR_SEN );
			Ret_Align_Back = g_TaskStacker[ k ].GetDI( CTaskStacker::xC_TRAY_ALIGN_BACK_SEN );
			UpdateCylinderSts( nConveyorLockBtnId[ k ][ 0 ], Ret_Align_Forward );
			UpdateCylinderSts( nConveyorLockBtnId[ k ][ 1 ], Ret_Align_Back );

			// Sensor
			CDSSPanel*   chkbt = NULL;
			chkbt = ( CDSSPanel* )GetDlgItem( nStackerTopSen[k] );
			if( g_TaskStacker[ k ].GetDI( CTaskStacker::xC_TRAY_STACKER_FULL_SEN ) ) {
				chkbt->On();
			}else {
				chkbt->Off();
			}

			chkbt = ( CDSSPanel* )GetDlgItem( nStackerBottomSen[ k ] );
			if( g_TaskStacker[ k ].GetDI( CTaskStacker::xC_TRAY_STACKER_EXIST_SEN ) ) {
				chkbt->On();
			}else {
				chkbt->Off();
			}

			chkbt = ( CDSSPanel* )GetDlgItem( nConveyorArriveSen[ k ] );
			if( g_TaskStacker[ k ].GetDI( CTaskStacker::xC_TRAY_CV_REAR_EXIST_SEN ) ) {
				chkbt->On();
			}
			else {
				chkbt->Off();
			}

			chkbt = ( CDSSPanel* )GetDlgItem( nConveyorArriveSlowSen[ k ] );
			if( g_TaskStacker[ k ].GetDI( CTaskStacker::xC_TRAY_CV_REAR_SLOW_SEN ) ) {
				chkbt->On();
			}
			else {
				chkbt->Off();
			}

			chkbt = ( CDSSPanel* )GetDlgItem( nConveyorCTraySlowSen[ k ] );
			if( g_TaskStacker[ k ].GetDI( CTaskStacker::xC_TRAY_CV_FRONT_SLOW_SEN ) ) {
				chkbt->On();
			}
			else {
				chkbt->Off();
			}

			chkbt = ( CDSSPanel* )GetDlgItem( nConveyorCTraySen[ k ] );
			if( g_TaskStacker[ k ].GetDI( CTaskStacker::xC_TRAY_CV_FRONT_EXIST_SEN ) ) {
				chkbt->On();
			}
			else {
				chkbt->Off();
			}

			// lock,unlock each sen
			chkbt = (CDSSPanel*)GetDlgItem(nStackerClampSetID[k][0]);
			if (g_TaskStacker[k].GetDI(CTaskStacker::xC_TRAY_FRONT_LEFT_LOCK_SEN)) {
				chkbt->On();
			}
			else {
				chkbt->Off();
			}
			chkbt = (CDSSPanel*)GetDlgItem(nStackerClampSetID[k][1]);
			if (g_TaskStacker[k].GetDI(CTaskStacker::xC_TRAY_FRONT_RIGHT_LOCK_SEN)) {
				chkbt->On();
			}
			else {
				chkbt->Off();
			}
			chkbt = (CDSSPanel*)GetDlgItem(nStackerClampSetID[k][2]);
			if (g_TaskStacker[k].GetDI(CTaskStacker::xC_TRAY_REAR_LEFT_LOCK_SEN)) {
				chkbt->On();
			}
			else {
				chkbt->Off();
			}
			chkbt = (CDSSPanel*)GetDlgItem(nStackerClampSetID[k][3]);
			if (g_TaskStacker[k].GetDI(CTaskStacker::xC_TRAY_REAR_RIGHT_LOCK_SEN)) {
				chkbt->On();
			}
			else {
				chkbt->Off();
			}
		}

		//-- sequence busy display
		CDSSPanel*   chkbt    = NULL;
		chkbt = (CDSSPanel*)GetDlgItem(IDC_COLORCHECKBTCTRL_BUSY);
		if( m_nCylManualOpActState != eCylManualOper_None ){
			chkbt->On();
		}else{
			chkbt->Off();
		}


		SetTimer(0, DEF_SENSOR_DISP_TM, NULL);
	}

	CDialogEx::OnTimer(nIDEvent);
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-10-06]
  NAME     : 
  PARAMS   : nTaskerId : enum eTaskMgrId
             nCmd      : enum eCylManualCmd
			 nLocId    : Stacker 명령의 경우 Location 지정
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CDlgManuSetplate::SetCommand(int nTaskerId, int nCmd, int nLocId/*=0*/)
{
	if( m_nCylManualOpActState != eCylManualOper_None ){
		return;
	}
	if( m_cbOperationSel.GetCurSel() < 0 ){
		return;
	}
	if( m_cbTimeSel.GetCurSel() < 0 ){
		return;
	}
	m_nCmdTaskerId = nTaskerId;
	m_nCmdManualCyl = nCmd;
	m_nCmdParam_Loc = nLocId;
	m_nCmdOperation = m_cbOperationSel.GetCurSel();  // enum eCylManualOper
	m_nCmdParam_Time = m_cbTimeSel.GetCurSel()+1;
	m_nCylManualActionStep = 0;
	m_nCylManualOpActState = m_nCmdOperation;
	CylinderSpecUpdate();
}

void CDlgManuSetplate::ClearCommand()
{
	//m_nCmdTaskerId = TASK_UNK;
	m_nCmdManualCyl = eCylManualCmd_None;
	m_nCmdOperation = eCylManualOper_None;
	m_nCmdParam_Time = -1;
}
int CDlgManuSetplate::do_CylCommand(BOOL bUndo/*=FALSE*/)
{
	int nErrCode=0;
	switch ( m_nCmdManualCyl )
	{
	case eCylManualCmd_EachClamp:
		{			
			int nCmd = /*DEF_CLAMP;//*/(bUndo==FALSE)?DEF_CLAMP:DEF_UNCLAMP;
			if(m_nCmdParam_Loc == CTaskStacker::yC_TRAY_LOCK){
				nErrCode = g_TaskStacker[m_nCmdTaskerId].do_cyl_LockUnlock(nCmd);
			}else{
				ASSERT(FALSE);
			}
		}break;
	case eCylManualCmd_EachUnclamp:
		{
			int nCmd = /*DEF_UNCLAMP;//*/(bUndo==FALSE)?DEF_UNCLAMP:DEF_CLAMP;	
			if(m_nCmdParam_Loc == CTaskStacker::yC_TRAY_UNLOCK){
				nErrCode = g_TaskStacker[m_nCmdTaskerId].do_cyl_LockUnlock(nCmd);
			}else{
				ASSERT(FALSE);
			}
		}break;
	case eCylManualCmd_AllUp:
		{
			int nCmd = /*DEF_FORW;//*/(bUndo==FALSE)? DEF_FORW : DEF_BACK;
			for( int i = 0; i < eMaxStackerCount; i++){
				nErrCode &= g_TaskStacker[i].do_cyl_TrayAlign(nCmd);
			}
		}break;
	case eCylManualCmd_AllDown:
		{
			int nCmd = /*DEF_BACK;//*/(bUndo==FALSE)? DEF_BACK : DEF_FORW;
			for( int i = 0; i < eMaxStackerCount; i++){
				nErrCode &= g_TaskStacker[i].do_cyl_TrayAlign(nCmd);
			}		
		}break;
	case eCylManualCmd_EachUp  :
		{
			int nCmd = (bUndo==FALSE)?DEF_UP:DEF_DOWN;
			if(m_nCmdParam_Loc == CTaskStacker::yC_TRAY_MAIN_PUSHER_UP){
				nErrCode = g_TaskStacker[m_nCmdTaskerId].do_cyl_MainPusher(nCmd);				
			}else if(m_nCmdParam_Loc == CTaskStacker::yC_TRAY_SUB_PUSHER_UP){
				nErrCode = g_TaskStacker[m_nCmdTaskerId].do_cyl_SubPusher(nCmd);
			}else{
				ASSERT(FALSE);
			}
		}break;
	case eCylManualCmd_EachDown:
		{
			int nCmd = (bUndo==FALSE)?DEF_DOWN:DEF_UP;
			if(m_nCmdParam_Loc == CTaskStacker::yC_TRAY_MAIN_PUSHER_DOWN){
				nErrCode = g_TaskStacker[m_nCmdTaskerId].do_cyl_MainPusher(nCmd);		
			}else if(m_nCmdParam_Loc == CTaskStacker::yC_TRAY_SUB_PUSHER_DOWN){
				nErrCode = g_TaskStacker[m_nCmdTaskerId].do_cyl_SubPusher(nCmd);
			}else{
				ASSERT(FALSE);
			}
		}break;
	case eCylManualCmd_EachFwd:
		{
			int nCmd = /*DEF_FORW;//*/( bUndo == FALSE ) ? DEF_FORW : DEF_BACK;
			if( m_nCmdParam_Loc == CTaskStacker::yCV_C_TRAY_ALIGN_FOR ) {
				nErrCode = g_TaskStacker[ m_nCmdTaskerId ].do_cyl_TrayAlign( nCmd );
			}else {
				ASSERT( FALSE );
			}

		}break;
	case eCylManualCmd_EachBak:
		{
			int nCmd = /*DEF_BACK;//*/( bUndo == FALSE ) ? DEF_BACK : DEF_FORW;
			if( m_nCmdParam_Loc == CTaskStacker::yCV_C_TRAY_ALIGN_FOR ) {
				nErrCode = g_TaskStacker[ m_nCmdTaskerId ].do_cyl_TrayAlign( nCmd );
			}else {
				ASSERT( FALSE );
			}
		}break;
	}
	return nErrCode;
}

int CDlgManuSetplate::Check_CylSensor(BOOL bUndo/*=FALSE*/)
{
	int nErrCode=0;
	switch ( m_nCmdManualCyl )
	{
	case eCylManualCmd_EachClamp:
		{			
			int nCmd = /*DEF_CLAMP;//*/(bUndo==FALSE)?DEF_CLAMP:DEF_UNCLAMP;	
			if(m_nCmdParam_Loc == CTaskStacker::yC_TRAY_LOCK){
				nErrCode = g_TaskStacker[m_nCmdTaskerId].chk_cyl_LockUnlock(nCmd);
			}else{
				ASSERT(FALSE);
			}
		}break;
	case eCylManualCmd_EachUnclamp:
		{
			int nCmd = /*DEF_UNCLAMP;//*/(bUndo==FALSE)?DEF_UNCLAMP:DEF_CLAMP;
			if(m_nCmdParam_Loc == CTaskStacker::yC_TRAY_UNLOCK){
				nErrCode = g_TaskStacker[m_nCmdTaskerId].chk_cyl_LockUnlock(nCmd);
			}else{
				ASSERT(FALSE);
			}
		}break;
	case eCylManualCmd_AllUp:
		{
			int nCmd = (bUndo==FALSE)?DEF_FORW:DEF_BACK;
			for( int ni = 0; ni < eMaxStackerCount; ni++)
			{
				nErrCode &= g_TaskStacker[m_nCmdTaskerId].chk_cyl_TrayAlign(nCmd);
			}
		}break;
	case eCylManualCmd_AllDown:
		{
			int nCmd = (bUndo==FALSE)? DEF_BACK : DEF_FORW;
			for( int ni = 0; ni < eMaxStackerCount; ni++)
			{
				nErrCode &= g_TaskStacker[m_nCmdTaskerId].chk_cyl_TrayAlign(nCmd);
			}
		}break;
	case eCylManualCmd_EachUp  :
		{
			int nCmd = (bUndo==FALSE)?DEF_UP:DEF_DOWN;
			if(m_nCmdParam_Loc == CTaskStacker::yC_TRAY_MAIN_PUSHER_UP){
				nErrCode = g_TaskStacker[m_nCmdTaskerId].chk_cyl_MainPusher(nCmd);				
			}else if(m_nCmdParam_Loc == CTaskStacker::yC_TRAY_SUB_PUSHER_UP){
				nErrCode = g_TaskStacker[m_nCmdTaskerId].chk_cyl_SubPusher(nCmd);
			}else{
				ASSERT(FALSE);
			}
		}break;
	case eCylManualCmd_EachDown:
		{
			int nCmd = (bUndo==FALSE)?DEF_DOWN:DEF_UP;
			if(m_nCmdParam_Loc == CTaskStacker::yC_TRAY_MAIN_PUSHER_DOWN){
				nErrCode = g_TaskStacker[m_nCmdTaskerId].chk_cyl_MainPusher(nCmd);		
			}else if(m_nCmdParam_Loc == CTaskStacker::yC_TRAY_SUB_PUSHER_DOWN){
				nErrCode = g_TaskStacker[m_nCmdTaskerId].chk_cyl_SubPusher(nCmd);
			}else{
				ASSERT(FALSE);
			}
		}break;
	case eCylManualCmd_EachFwd:
		{
			int nCmd = /*DEF_FORW;//*/( bUndo == FALSE ) ? DEF_FORW : DEF_BACK;
			if( m_nCmdParam_Loc == CTaskStacker::yCV_C_TRAY_ALIGN_FOR ) {
				nErrCode = g_TaskStacker[ m_nCmdTaskerId ].chk_cyl_TrayAlign( nCmd );
			}else {
				ASSERT( FALSE );
			}

		}break;
	case eCylManualCmd_EachBak:
		{
			int nCmd = /*DEF_BACK;//*/( bUndo == FALSE ) ? DEF_BACK : DEF_FORW;
			if( m_nCmdParam_Loc == CTaskStacker::yCV_C_TRAY_ALIGN_FOR ) {
				nErrCode = g_TaskStacker[ m_nCmdTaskerId ].chk_cyl_TrayAlign( nCmd );
			}else {
				ASSERT( FALSE );
			}
		}break;
	}
	return nErrCode;
}

void CDlgManuSetplate::DisplayCylinderActionTime(BOOL bClear/*=FALSE*/)
{
 	CString strTime;
 	strTime.Format("%.3f", m_dCylActionTimeRecord);
 	if( bClear ) strTime = "";
 	SetDlgItemText(UNITSETDLL_IDC_STATIC_TIME3, strTime);
	
}

void CDlgManuSetplate::CylinderManualAction()
{
	switch( m_nCylManualOpActState )
	{
	case eCylManualOper_None:
		{
			CylManualOper_None();
		}break;
	case eCylManualOper_Only_One_Action_and_No_Check_Sensor :
		{
			CylManualOper_Only_One_Action_and_No_Check_Sensor();
		}break;
	case eCylManualOper_Only_One_Action_and_Check_Sensor:
		{
			CylManualOper_Only_One_Action_and_Check_Sensor();
		}break;
	case eCylManualOper_Continue_Action_and_No_Check_Sensor:
		{
			CylManualOper_Continue_Action_and_No_Check_Sensor();
		}break;
	case eCylManualOper_Continue_Action_and_Check_Sensor:
		{
			CylManualOper_Continue_Action_and_Check_Sensor();
		}break;
	case eCylManualOper_Time_Check_Action_Once_and_Sensor_Check:
		{
			CylManualOper_Time_Check_Action_Once_and_Sensor_Check();
		}break;
	}
}
//==============================================================================
//
//==============================================================================
void CDlgManuSetplate::CylManualOper_None()
{
	switch( m_nCylManualActionStep )
	{
	case 0: // 아무것도 하지 않고 대기 한다.
		{
			ClearCommand();
			m_nCylManualActionStep++;
		}break;
	case 1:
		{
		}break;
	}
}
//==============================================================================
// cylinder 한번만 동작하고 센서체크 하지 않음.
//==============================================================================
void CDlgManuSetplate::CylManualOper_Only_One_Action_and_No_Check_Sensor()
{
	switch (m_nCylManualActionStep)
	{
		case 0:
		{
// 			if (g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK) != eNoDeviceNoTray && m_nCmdManualCyl == eCylManualCmd_EachUp)
// 			{
// 				g_TaskStacker[m_nCmdTaskerId].ConveyorMoving(eConveyorMove_BKWD, false);
// 				Sleep(200);
// 
// 
// 				int nErr = g_TaskStacker[m_nCmdTaskerId].ChkDI(CTaskStacker::xC_TRAY_CV_FRONT_EXIST_SEN, true);
// 				if (nErr != ERR_NO_ERROR) {
// 					break;
// 				}
// 
// 				g_TaskStacker[m_nCmdTaskerId].ConveyorStop();
// 			}

			m_nCylManualActionStep++;
		}break;
		case 1:
		{
			int nErr = do_CylCommand();
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, NULL);
			}
			m_nCylManualActionStep++;
		}break;
		case 2:
		{

			m_nCylManualOpActState = eCylManualOper_None;
		}break;
	}
}
//==============================================================================
// cylinder 한번만 동작하고 센서체크 함.
//==============================================================================
void CDlgManuSetplate::CylManualOper_Only_One_Action_and_Check_Sensor()
{
	switch( m_nCylManualActionStep )
	{
	case 0:
		{
			int nErr = do_CylCommand();
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, NULL);
				m_nCylManualActionStep = 9000;
				break;
			}
			m_dTimeCheck = (double)GetTickCount64();
			m_nCylManualActionStep++;
		}break;
	case 1:
		{
			double dSpanTime = (double)GetTickCount64() - m_dTimeCheck;
			if( dSpanTime > ((double)m_nCmdParam_Time * 1000.0) )
			{
				m_nCylManualActionStep++;
			}
		}break;
	case 2:
		{
			int nErr = Check_CylSensor();
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, NULL);
				m_nCylManualActionStep++;
				break;
			}
			m_nCylManualActionStep++;
		}break;
	case 3:
		{
			m_nCylManualOpActState = eCylManualOper_None;
		}break;
	case 9000:
		{
			m_nCylManualOpActState = eCylManualOper_None;
		}break;
	}
}

//==============================================================================
// 작업자가  stop하기 전까지 cylinder 동작하고 센서체크 하지 않음.
//==============================================================================
void CDlgManuSetplate::CylManualOper_Continue_Action_and_No_Check_Sensor()
{
	switch( m_nCylManualActionStep )
	{
	case 0:
		{
			m_nCmdCycleCount=0;
			m_nCylManualActionStep = 1000;
		}break;
	case 1000:
		{
			int nErr = do_CylCommand();
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, NULL);
				m_nCylManualActionStep = 9000;
				break;
			}
			m_dTimeCheck = (double)GetTickCount64();
			m_nCylManualActionStep++;
		}break;
	case 1001:
		{
			double dSpanTime = (double)GetTickCount64() - m_dTimeCheck;
			if( dSpanTime > ((double)m_nCmdParam_Time*1000.0) )
			{
				m_nCmdCycleCount++;
				m_nCylManualActionStep = 2000;
			}
		}break;
	case 2000:
		{
			int nErr = do_CylCommand(TRUE);
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, NULL);
				m_nCylManualActionStep = 9000;
				break;
			}
			m_dTimeCheck = (double)GetTickCount64();
			m_nCylManualActionStep++;
		}break;
	case 2001:
		{
			double dSpanTime = (double)GetTickCount64() - m_dTimeCheck;
			if( dSpanTime > ((double)m_nCmdParam_Time*1000.0) )
			{
				m_nCylManualActionStep = 1000;
			}
		}break;
	case 9000:
		{
			m_nCylManualOpActState = eCylManualOper_None;
		}break;
	}
}

//==============================================================================
// 작업자가 stop하기 전까지 cylinder 동작하고 센서체크 함.
//==============================================================================
void CDlgManuSetplate::CylManualOper_Continue_Action_and_Check_Sensor()
{
	switch( m_nCylManualActionStep )
	{
	case 0:
		{
			m_nCmdCycleCount = 0;
			m_nCylManualActionStep = 1000;
		}break;
	case 1000:
		{
			int nErr = do_CylCommand();
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, NULL);
				m_nCylManualActionStep = 9000;
				break;
			}
			m_dTimeCheck = (double)GetTickCount64();
			m_nCylManualActionStep++;
		}break;
	case 1001:
		{
			double dSpanTime = (double)GetTickCount64() - m_dTimeCheck;
			if( dSpanTime > ((double)m_nCmdParam_Time*1000.0) )
			{
				m_nCylManualActionStep++;
			}
		}break;
	case 1002:
		{
			int nErr = Check_CylSensor();
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, NULL);
				m_nCylManualActionStep=9000;
				break;
			}
			m_nCmdCycleCount++;
			m_nCylManualActionStep = 2000;
		}break;

	case 2000:
		{
			int nErr = do_CylCommand(TRUE);
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, NULL);
				m_nCylManualActionStep = 9000;
				break;
			}
			m_dTimeCheck = (double)GetTickCount64();
			m_nCylManualActionStep++;
		}break;
	case 2001:
		{
			double dSpanTime = (double)GetTickCount64() - m_dTimeCheck;
			if( dSpanTime > ((double)m_nCmdParam_Time*1000.0) )
			{
				m_nCylManualActionStep++;
			}
		}break;
	case 2002:
		{
			int nErr = Check_CylSensor(TRUE);
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, NULL);
				m_nCylManualActionStep = 9000;
				break;
			}
			m_nCylManualActionStep = 1000;
		}break;

	case 9000:
		{
			m_nCylManualOpActState = eCylManualOper_None;
		}break;
	}
}


//==============================================================================
// 실린더 한번 동작 시키고 센서 들어오는 시간 계산 함.
//==============================================================================
void CDlgManuSetplate::CylManualOper_Time_Check_Action_Once_and_Sensor_Check()
{
	switch( m_nCylManualActionStep )
	{
	case 0:
		{
			m_nCylActionTimeRetry = 0;
			DisplayCylinderActionTime(TRUE);
			m_nCylManualActionStep = 1000;
			m_dCylActionTimeRecord = 0.0;
		}break;
		//실린더 동작
	case 1000: 
		{
			int nErr = do_CylCommand();
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, NULL);
				m_nCylManualActionStep = 9000;
				break;
			}
			m_dTimeCheck = (double)GetTickCount64();
			m_nCylManualActionStep++;
		}break;
		// 센서 체크
	case 1001:
		{
			double dSpanTime = (double)GetTickCount64() - m_dTimeCheck;
			if( dSpanTime > 5000.0 )
			{
// 				if( m_nCylActionTimeRetry < 1)
// 				{
// 					m_nCylManualActionStep = 1000;
// 					m_nCylActionTimeRetry++;
// 					break;
// 				}
// 				else
// 				{
					CParamList AlarmData;
					switch(m_nCmdTaskerId)
					{
						case eSTACKER_01: { AlarmData.m_fnAddItemFormat("Stacker1 Manual Action Time Over");	}break;
						case eSTACKER_02: { AlarmData.m_fnAddItemFormat("Stacker2 Manual Action Time Over");	}break;
						case eSTACKER_03: { AlarmData.m_fnAddItemFormat("Stacker3 Manual Action Time Over");	}break;
						case eSTACKER_04: { AlarmData.m_fnAddItemFormat("Stacker4 Manual Action Time Over");	}break;
						case eSTACKER_05: { AlarmData.m_fnAddItemFormat("Stacker5 Manual Action Time Over");	}break;
						case eSTACKER_06: { AlarmData.m_fnAddItemFormat("Stacker6 Manual Action Time Over");	}break;
						case eSTACKER_07 :{ AlarmData.m_fnAddItemFormat("Stacker7 Manual Action Time Over");	}break;
					}
					
					g_Error.AlarmReport(ERR_SYS_MANUAL_ACTION_TIMEOVER, AlarmData, NULL);
					m_nCylManualActionStep = 9000;
					break;
//				}
			}
			int nChk = Check_CylSensor();
			if( nChk == ERR_NO_ERROR )
			{
				m_dCylActionTimeRecord = dSpanTime / 1000.0;
				m_nCylManualActionStep++;
				break;
			}
		}break;
	case 1002:
		{
			DisplayCylinderActionTime();
			g_DB.UPDATE_CYLINDER_TIME_TBL(m_nCylinderNum, m_dCylActionTimeRecord);
			m_nCylinderNum = 0;
			m_nCylManualOpActState = eCylManualOper_None;
		}break;
	case 9000:
		{
			m_nCylManualOpActState = eCylManualOper_None;
		}break;
	}
}

void CDlgManuSetplate::ThreadProc1()
{
	SetThreadInterval(0, 50);
	while( GetThreadAliveFlag(0) ){
		if( GetPauseFlag(0) ) {
			NEXT;
			continue;
		}
		CylinderManualAction();
		NEXT;
	}
}

void CDlgManuSetplate::OnBnClickedButtonSetplateStackerCylActStop()
{
	MakeLog("Stop Button Click");
	m_nCylManualOpActState = eCylManualOper_None;
	ClearCommand();
}

// nState : 0:off, 1:on, -1:error
void CDlgManuSetplate::UpdateCylinderSts(int nCtrlId, int nStatus)
{
	CMFCButton*   pButton = (CMFCButton*)GetDlgItem(nCtrlId);
	if( nStatus == 0 ){
		pButton->SetFaceColor(COLOR_NOMAL, TRUE); 
	}else if( nStatus == 1 ){
		pButton->SetFaceColor(COLOR_GREEN, TRUE);
	}else{
		pButton->SetFaceColor(COLOR_DARKRED, TRUE); 
	}
}

void CDlgManuSetplate::do_Vibrator_Cyl( int _StackerIdx, int _OnOff /*=DEF_FORW*/ )
{
	int nErr = g_TaskStacker[ _StackerIdx ].do_cyl_Vibrator( _OnOff );
	if( nErr != ERR_NO_ERROR ) {
		CParamList AlarmData;
		g_Error.AlarmReport( nErr, AlarmData, NULL );
	}
}

void CDlgManuSetplate::ButtonClickLog( int nBtnNum, int nIdx)
{
	char szOperations[ 64 ] = { 0, }, szBtnName[ 32 ] = { 0, };

	switch( nBtnNum )
	{
	case eClickBtn_Main_Up:			sprintf_s( szBtnName, sizeof( szBtnName ), "Main Up %d", nIdx+1); break;
	case eClickBtn_Main_Down:		sprintf_s( szBtnName, sizeof( szBtnName ), "Main Down %d", nIdx+1); break;
	case eClickBtn_Sub_Up:			sprintf_s( szBtnName, sizeof( szBtnName ), "Sub Down %d", nIdx+1); break;
	case eClickBtn_Sub_Down:		sprintf_s( szBtnName, sizeof( szBtnName ), "Sub Down %d", nIdx+1); break;
	case eClickBtn_Lock:			sprintf_s( szBtnName, sizeof( szBtnName ), "Tray Lock %d", nIdx+1); break;
	case eClickBtn_Unock:			sprintf_s( szBtnName, sizeof( szBtnName ), "Tray Unlock %d", nIdx+1); break;
	case eClickBtn_Align_Fwd:		sprintf_s( szBtnName, sizeof( szBtnName ), "Tray Align_Fwd %d", nIdx+1); break;
	case eClickBtn_Align_Back:		sprintf_s( szBtnName, sizeof( szBtnName ), "Tray Align_Back %d", nIdx+1); break;
	case eClickBtn_All_Align_Fwd:	sprintf_s( szBtnName, sizeof( szBtnName ), "Tray All Align_Fwd"); break;
	case eClickBtn_All_Align_Back:	sprintf_s( szBtnName, sizeof( szBtnName ), "Tray All Align_Back"); break;
	case eClickBtn_Stop:			MakeLog( "Stop Button Click" ); break;
	}

	if( nBtnNum != eClickBtn_Stop )
	{
		int nTime = m_cbTimeSel.GetCurSel() + 1;

		switch( m_cbOperationSel.GetCurSel() )
		{
		case 0: sprintf_s( szOperations, sizeof( szOperations ), "Only One Action & No Check Sensor" ); break;
		case 1: sprintf_s( szOperations, sizeof( szOperations ), "Only One Action & Check Sensor" ); break;
		case 2: sprintf_s( szOperations, sizeof( szOperations ), "Continue Action & No Check Sensor" ); break;
		case 3: sprintf_s( szOperations, sizeof( szOperations ), "Continue Action & Check Sensor" ); break;
		case 4: sprintf_s( szOperations, sizeof( szOperations ), "Time Check Action (Once & Sensor Check)" ); break;
		}
		MakeLog( "%s Button Click Operations: %s, Time: %d sec", szBtnName, szOperations, nTime );
	}
}

void CDlgManuSetplate::MakeLog(LPCTSTR fmt, ...)
{
	TCHAR tmpMsg[1024] = {0,};
	if(fmt)
	{
		va_list argList;
		va_start(argList, fmt);
		_vstprintf_s(tmpMsg, fmt, argList);
		va_end(argList);
	}
	TWLOG.MakeLog(static_cast<int>(eLogID_SYSTEM), Debug, "", __LINE__, NULL, tmpMsg);
}


void CDlgManuSetplate::OnBnClickedBtnStkMainUp1()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog( eClickBtn_Main_Up, eSTACKER_01);
	SetCommand(eSTACKER_01, eCylManualCmd_EachUp, CTaskStacker::yC_TRAY_MAIN_PUSHER_UP);
	CylinderNum(eEmpty_Stacker1_Main_UP);
}

void CDlgManuSetplate::OnBnClickedBtnStkMainUp2()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog( eClickBtn_Main_Up, eSTACKER_02);
	SetCommand(eSTACKER_02, eCylManualCmd_EachUp, CTaskStacker::yC_TRAY_MAIN_PUSHER_UP);
	CylinderNum(eLoader_Stacker_Main_Up);
}

void CDlgManuSetplate::OnBnClickedBtnStkMainUp3()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog( eClickBtn_Main_Up, eSTACKER_03);
	SetCommand(eSTACKER_03, eCylManualCmd_EachUp, CTaskStacker::yC_TRAY_MAIN_PUSHER_UP);
	CylinderNum(eEmpty_Stacker2_Main_UP);
	
}

void CDlgManuSetplate::OnBnClickedBtnStkMainUp4()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog( eClickBtn_Main_Up, eSTACKER_04);
	SetCommand(eSTACKER_04, eCylManualCmd_EachUp, CTaskStacker::yC_TRAY_MAIN_PUSHER_UP);
	CylinderNum(eUnloader_Stacker1_Main_Up);
}

void CDlgManuSetplate::OnBnClickedBtnStkMainUp5()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog( eClickBtn_Main_Up, eSTACKER_05);
	SetCommand(eSTACKER_05, eCylManualCmd_EachUp, CTaskStacker::yC_TRAY_MAIN_PUSHER_UP);
	CylinderNum(eUnloader_Stacker2_Main_Up);
}

void CDlgManuSetplate::OnBnClickedBtnStkMainUp6()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog( eClickBtn_Main_Up, eSTACKER_06);
	SetCommand(eSTACKER_06, eCylManualCmd_EachUp, CTaskStacker::yC_TRAY_MAIN_PUSHER_UP);
	CylinderNum(eUnloader_Stacker3_Main_Up);
}

void CDlgManuSetplate::OnBnClickedBtnStkMainUp7()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog( eClickBtn_Main_Up, eSTACKER_07);
	SetCommand(eSTACKER_07, eCylManualCmd_EachUp, CTaskStacker::yC_TRAY_MAIN_PUSHER_UP);
	CylinderNum(eUnloader_Stacker4_Main_Up);
}

void CDlgManuSetplate::OnBnClickedBtnStkMainDown1()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Main_Down, eSTACKER_01);
	SetCommand(eSTACKER_01, eCylManualCmd_EachDown, CTaskStacker::yC_TRAY_MAIN_PUSHER_DOWN);
	CylinderNum(eEmpty_Stacker1_Main_Down);
}
void CDlgManuSetplate::OnBnClickedBtnStkMainDown2()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Main_Down, eSTACKER_02);
	SetCommand(eSTACKER_02, eCylManualCmd_EachDown, CTaskStacker::yC_TRAY_MAIN_PUSHER_DOWN);
	CylinderNum(eLoader_Stacker_Main_Down);
}

void CDlgManuSetplate::OnBnClickedBtnStkMainDown3()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Main_Down, eSTACKER_03);
	SetCommand(eSTACKER_03, eCylManualCmd_EachDown, CTaskStacker::yC_TRAY_MAIN_PUSHER_DOWN);
	CylinderNum(eEmpty_Stacker2_Main_Down);
}

void CDlgManuSetplate::OnBnClickedBtnStkMainDown4()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Main_Down, eSTACKER_04);
	SetCommand(eSTACKER_04, eCylManualCmd_EachDown, CTaskStacker::yC_TRAY_MAIN_PUSHER_DOWN);
	CylinderNum(eUnloader_Stacker1_Main_Down);
}

void CDlgManuSetplate::OnBnClickedBtnStkMainDown5()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Main_Down, eSTACKER_05);
	SetCommand(eSTACKER_05, eCylManualCmd_EachDown, CTaskStacker::yC_TRAY_MAIN_PUSHER_DOWN);
	CylinderNum(eUnloader_Stacker2_Main_Down);
}

void CDlgManuSetplate::OnBnClickedBtnStkMainDown6()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Main_Down, eSTACKER_06);
	SetCommand(eSTACKER_06, eCylManualCmd_EachDown, CTaskStacker::yC_TRAY_MAIN_PUSHER_DOWN);
	CylinderNum(eUnloader_Stacker3_Main_Down);
}

void CDlgManuSetplate::OnBnClickedBtnStkMainDown7()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Main_Down, eSTACKER_07);
	SetCommand(eSTACKER_07, eCylManualCmd_EachDown, CTaskStacker::yC_TRAY_MAIN_PUSHER_DOWN);
	CylinderNum(eUnloader_Stacker4_Main_Down);
}
void CDlgManuSetplate::OnBnClickedBtnStkSubUp1()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Sub_Up, eSTACKER_01);
	SetCommand(eSTACKER_01, eCylManualCmd_EachUp, CTaskStacker::yC_TRAY_SUB_PUSHER_UP);
	CylinderNum(eEmpty_Stacker1_Sub_UP);
}
void CDlgManuSetplate::OnBnClickedBtnStkSubUp2()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Sub_Up, eSTACKER_02);
	SetCommand(eSTACKER_02, eCylManualCmd_EachUp, CTaskStacker::yC_TRAY_SUB_PUSHER_UP);
	CylinderNum(eLoader_Stacker_Sub_Up);
}

void CDlgManuSetplate::OnBnClickedBtnStkSubUp3()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Sub_Up, eSTACKER_03);
	SetCommand(eSTACKER_03, eCylManualCmd_EachUp, CTaskStacker::yC_TRAY_SUB_PUSHER_UP);
	CylinderNum(eEmpty_Stacker2_Sub_UP);
}

void CDlgManuSetplate::OnBnClickedBtnStkSubUp4()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Sub_Up, eSTACKER_04);
	SetCommand(eSTACKER_04, eCylManualCmd_EachUp, CTaskStacker::yC_TRAY_SUB_PUSHER_UP);
	CylinderNum(eUnloader_Stacker1_Sub_Up);
}

void CDlgManuSetplate::OnBnClickedBtnStkSubUp5()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Sub_Up, eSTACKER_05);
	SetCommand(eSTACKER_05, eCylManualCmd_EachUp, CTaskStacker::yC_TRAY_SUB_PUSHER_UP);
	CylinderNum(eUnloader_Stacker2_Sub_Up);
}

void CDlgManuSetplate::OnBnClickedBtnStkSubUp6()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Sub_Up, eSTACKER_06);
	SetCommand(eSTACKER_06, eCylManualCmd_EachUp, CTaskStacker::yC_TRAY_SUB_PUSHER_UP);
	CylinderNum(eUnloader_Stacker3_Sub_Up);
}

void CDlgManuSetplate::OnBnClickedBtnStkSubUp7()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Sub_Up, eSTACKER_07);
	SetCommand(eSTACKER_07, eCylManualCmd_EachUp, CTaskStacker::yC_TRAY_SUB_PUSHER_UP);
	CylinderNum(eUnloader_Stacker4_Sub_Up);
}
void CDlgManuSetplate::OnBnClickedBtnStkSubDown1()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Sub_Down, eSTACKER_01);
	SetCommand(eSTACKER_01, eCylManualCmd_EachDown, CTaskStacker::yC_TRAY_SUB_PUSHER_DOWN);
	CylinderNum(eEmpty_Stacker1_Sub_Down);
}
void CDlgManuSetplate::OnBnClickedBtnStkSubDown2()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Sub_Down, eSTACKER_02);
	SetCommand(eSTACKER_02, eCylManualCmd_EachDown, CTaskStacker::yC_TRAY_SUB_PUSHER_DOWN);
	CylinderNum(eLoader_Stacker_Sub_Down);
}

void CDlgManuSetplate::OnBnClickedBtnStkSubDown3()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Sub_Down, eSTACKER_03);
	SetCommand(eSTACKER_03, eCylManualCmd_EachDown, CTaskStacker::yC_TRAY_SUB_PUSHER_DOWN);
	CylinderNum(eEmpty_Stacker2_Sub_Down);
}

void CDlgManuSetplate::OnBnClickedBtnStkSubDown4()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Sub_Down, eSTACKER_04);
	SetCommand(eSTACKER_04, eCylManualCmd_EachDown, CTaskStacker::yC_TRAY_SUB_PUSHER_DOWN);
	CylinderNum(eUnloader_Stacker1_Sub_Down);
}

void CDlgManuSetplate::OnBnClickedBtnStkSubDown5()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Sub_Down, eSTACKER_05);
	SetCommand(eSTACKER_05, eCylManualCmd_EachDown, CTaskStacker::yC_TRAY_SUB_PUSHER_DOWN);
	CylinderNum(eUnloader_Stacker2_Sub_Down);
}

void CDlgManuSetplate::OnBnClickedBtnStkSubDown6()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Sub_Down, eSTACKER_06);
	SetCommand(eSTACKER_06, eCylManualCmd_EachDown, CTaskStacker::yC_TRAY_SUB_PUSHER_DOWN);
	CylinderNum(eUnloader_Stacker3_Sub_Down);
}

void CDlgManuSetplate::OnBnClickedBtnStkSubDown7()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Sub_Down, eSTACKER_07);
	SetCommand(eSTACKER_07, eCylManualCmd_EachDown, CTaskStacker::yC_TRAY_SUB_PUSHER_DOWN);
	CylinderNum(eUnloader_Stacker4_Sub_Down);
}
void CDlgManuSetplate::OnBnClickedBtnStkClamp1()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Lock, eSTACKER_01);
	SetCommand(eSTACKER_01, eCylManualCmd_EachClamp, CTaskStacker::yC_TRAY_LOCK);
}

void CDlgManuSetplate::OnBnClickedBtnStkClamp2()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Lock, eSTACKER_02);
	SetCommand(eSTACKER_02, eCylManualCmd_EachClamp, CTaskStacker::yC_TRAY_LOCK);
}

void CDlgManuSetplate::OnBnClickedBtnStkClamp3()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Lock, eSTACKER_03);
	SetCommand(eSTACKER_03, eCylManualCmd_EachClamp, CTaskStacker::yC_TRAY_LOCK);
}

void CDlgManuSetplate::OnBnClickedBtnStkClamp4()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Lock, eSTACKER_04);
	SetCommand(eSTACKER_04, eCylManualCmd_EachClamp, CTaskStacker::yC_TRAY_LOCK);
}

void CDlgManuSetplate::OnBnClickedBtnStkClamp5()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Lock, eSTACKER_05);
	SetCommand(eSTACKER_05, eCylManualCmd_EachClamp, CTaskStacker::yC_TRAY_LOCK);
}

void CDlgManuSetplate::OnBnClickedBtnStkClamp6()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Lock, eSTACKER_06);
	SetCommand(eSTACKER_06, eCylManualCmd_EachClamp, CTaskStacker::yC_TRAY_LOCK);
}

void CDlgManuSetplate::OnBnClickedBtnStkClamp7()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Lock, eSTACKER_07);
	SetCommand(eSTACKER_07, eCylManualCmd_EachClamp, CTaskStacker::yC_TRAY_LOCK);
}
void CDlgManuSetplate::OnBnClickedBtnStkUnclamp1()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Unock, eSTACKER_01);
	SetCommand(eSTACKER_01, eCylManualCmd_EachUnclamp, CTaskStacker::yC_TRAY_UNLOCK);
}

void CDlgManuSetplate::OnBnClickedBtnStkUnclamp2()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Unock, eSTACKER_02);
	SetCommand(eSTACKER_02, eCylManualCmd_EachUnclamp, CTaskStacker::yC_TRAY_UNLOCK);
}

void CDlgManuSetplate::OnBnClickedBtnStkUnclamp3()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Unock, eSTACKER_03);
	SetCommand(eSTACKER_03, eCylManualCmd_EachUnclamp, CTaskStacker::yC_TRAY_UNLOCK);
}

void CDlgManuSetplate::OnBnClickedBtnStkUnclamp4()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Unock, eSTACKER_04);
	SetCommand(eSTACKER_04, eCylManualCmd_EachUnclamp, CTaskStacker::yC_TRAY_UNLOCK);
}

void CDlgManuSetplate::OnBnClickedBtnStkUnclamp5()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Unock, eSTACKER_05);
	SetCommand(eSTACKER_05, eCylManualCmd_EachUnclamp, CTaskStacker::yC_TRAY_UNLOCK);
}

void CDlgManuSetplate::OnBnClickedBtnStkUnclamp6()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Unock, eSTACKER_06);
	SetCommand(eSTACKER_06, eCylManualCmd_EachUnclamp, CTaskStacker::yC_TRAY_UNLOCK);
}

void CDlgManuSetplate::OnBnClickedBtnStkUnclamp7()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Unock, eSTACKER_07);
	SetCommand(eSTACKER_07, eCylManualCmd_EachUnclamp, CTaskStacker::yC_TRAY_UNLOCK);
}


void CDlgManuSetplate::OnBnClickedBtnStkAllForward()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_All_Align_Fwd, eSTACKER_01);
	SetCommand(eSTACKER_01, eCylManualCmd_AllUp );
}


void CDlgManuSetplate::OnBnClickedBtnStkForward1()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Align_Fwd, eSTACKER_01);
	SetCommand(eSTACKER_01, eCylManualCmd_EachFwd, CTaskStacker::yCV_C_TRAY_ALIGN_FOR );
	CylinderNum(eCentering_Clamp1_Lock);
}


void CDlgManuSetplate::OnBnClickedBtnStkForward2()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Align_Fwd, eSTACKER_02);
	SetCommand(eSTACKER_02, eCylManualCmd_EachFwd, CTaskStacker::yCV_C_TRAY_ALIGN_FOR );
	CylinderNum(eCentering_Clamp2_Lock);
}


void CDlgManuSetplate::OnBnClickedBtnStkForward3()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Align_Fwd, eSTACKER_03);
	SetCommand(eSTACKER_03, eCylManualCmd_EachFwd, CTaskStacker::yCV_C_TRAY_ALIGN_FOR );
	CylinderNum(eCentering_Clamp3_Lock);
}


void CDlgManuSetplate::OnBnClickedBtnStkForward4()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Align_Fwd, eSTACKER_04);
	SetCommand(eSTACKER_04, eCylManualCmd_EachFwd, CTaskStacker::yCV_C_TRAY_ALIGN_FOR );
	CylinderNum(eCentering_Clamp4_Lock);
}


void CDlgManuSetplate::OnBnClickedBtnStkForward5()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Align_Fwd, eSTACKER_05);
	SetCommand(eSTACKER_05, eCylManualCmd_EachFwd, CTaskStacker::yCV_C_TRAY_ALIGN_FOR );
	CylinderNum(eCentering_Clamp5_Lock);
}


void CDlgManuSetplate::OnBnClickedBtnStkForward6()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Align_Fwd, eSTACKER_06);
	SetCommand(eSTACKER_06, eCylManualCmd_EachFwd, CTaskStacker::yCV_C_TRAY_ALIGN_FOR );
	CylinderNum(eCentering_Clamp6_Lock);
}


void CDlgManuSetplate::OnBnClickedBtnStkForward7()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Align_Fwd, eSTACKER_07);
	SetCommand(eSTACKER_07, eCylManualCmd_EachFwd, CTaskStacker::yCV_C_TRAY_ALIGN_FOR );
	CylinderNum(eCentering_Clamp7_Lock);
}


void CDlgManuSetplate::OnBnClickedBtnStkAllUnlock()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_All_Align_Back, eSTACKER_01);
	SetCommand(eSTACKER_01, eCylManualCmd_AllDown );
	CylinderNum(eCentering_Clamp7_Lock);
}


void CDlgManuSetplate::OnBnClickedBtnStkBack1()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Align_Back, eSTACKER_01);
	SetCommand(eSTACKER_01, eCylManualCmd_EachBak, CTaskStacker::yCV_C_TRAY_ALIGN_FOR );
	CylinderNum(eCentering_Clamp1_Unlock);
}


void CDlgManuSetplate::OnBnClickedBtnStkBack2()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Align_Back, eSTACKER_02);
	SetCommand(eSTACKER_02, eCylManualCmd_EachBak, CTaskStacker::yCV_C_TRAY_ALIGN_FOR );
	CylinderNum(eCentering_Clamp2_Unlock);
}


void CDlgManuSetplate::OnBnClickedBtnStkBack3()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Align_Back, eSTACKER_03);
	SetCommand(eSTACKER_03, eCylManualCmd_EachBak, CTaskStacker::yCV_C_TRAY_ALIGN_FOR );
	CylinderNum(eCentering_Clamp3_Unlock);
}


void CDlgManuSetplate::OnBnClickedBtnStkBack4()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Align_Back, eSTACKER_04);
	SetCommand(eSTACKER_04, eCylManualCmd_EachBak, CTaskStacker::yCV_C_TRAY_ALIGN_FOR );
	CylinderNum(eCentering_Clamp4_Unlock);
}


void CDlgManuSetplate::OnBnClickedBtnStkBack5()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Align_Back, eSTACKER_05);
	SetCommand(eSTACKER_05, eCylManualCmd_EachBak, CTaskStacker::yCV_C_TRAY_ALIGN_FOR );
	CylinderNum(eCentering_Clamp5_Unlock);
}


void CDlgManuSetplate::OnBnClickedBtnStkBack6()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Align_Back, eSTACKER_06);
	SetCommand(eSTACKER_06, eCylManualCmd_EachBak, CTaskStacker::yCV_C_TRAY_ALIGN_FOR );
	CylinderNum(eCentering_Clamp6_Unlock);
}


void CDlgManuSetplate::OnBnClickedBtnStkBack7()
{
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Align_Back, eSTACKER_07);
	SetCommand(eSTACKER_07, eCylManualCmd_EachBak, CTaskStacker::yCV_C_TRAY_ALIGN_FOR );
	CylinderNum(eCentering_Clamp7_Unlock);
}


void CDlgManuSetplate::MouseUpColorbtctrlConveyor1Back(short Button, short Shift, long X, long Y)
{
	g_TaskStacker[eSTACKER_01].ConveyorStop();
}


void CDlgManuSetplate::MouseDownColorbtctrlConveyor1Back(short Button, short Shift, long X, long Y)
{
	UpdateData( TRUE );
	int nErr = g_TaskStacker[eSTACKER_01].ConveyorMoving(eConveyorMove_BKWD,m_bSPEED_CONTROL_1 == TRUE ? true : false);
	if (nErr != ERR_NO_ERROR)
	{
		CParamList AlarmData;
		g_Error.AlarmReport(ERR_STACKER_1_INTERLOCK_CONVEYOR_DRIVER_NOT_WORK, AlarmData, NULL);
	}
}


void CDlgManuSetplate::MouseUpColorbtctrlConveyor1Back2(short Button, short Shift, long X, long Y)
{
	g_TaskStacker[eSTACKER_02].ConveyorStop();
}


void CDlgManuSetplate::MouseDownColorbtctrlConveyor1Back2(short Button, short Shift, long X, long Y)
{
	UpdateData( TRUE );
	int nErr = g_TaskStacker[eSTACKER_02].ConveyorMoving(eConveyorMove_BKWD,m_bSPEED_CONTROL_2 == TRUE ? true : false);
	if (nErr != ERR_NO_ERROR)
	{
		CParamList AlarmData;
		g_Error.AlarmReport(ERR_STACKER_2_INTERLOCK_CONVEYOR_DRIVER_NOT_WORK, AlarmData, NULL);
	}
}


void CDlgManuSetplate::MouseUpColorbtctrlConveyor1Back3(short Button, short Shift, long X, long Y)
{
	g_TaskStacker[eSTACKER_03].ConveyorStop();
}


void CDlgManuSetplate::MouseDownColorbtctrlConveyor1Back3(short Button, short Shift, long X, long Y)
{
	UpdateData( TRUE );
	int nErr = g_TaskStacker[eSTACKER_03].ConveyorMoving(eConveyorMove_BKWD,m_bSPEED_CONTROL_3 == TRUE ? true : false);
	if (nErr != ERR_NO_ERROR)
	{
		CParamList AlarmData;
		g_Error.AlarmReport(ERR_STACKER_3_INTERLOCK_CONVEYOR_DRIVER_NOT_WORK, AlarmData, NULL);
	}
}


void CDlgManuSetplate::MouseUpColorbtctrlConveyor1Back4(short Button, short Shift, long X, long Y)
{
	g_TaskStacker[eSTACKER_04].ConveyorStop();
}


void CDlgManuSetplate::MouseDownColorbtctrlConveyor1Back4(short Button, short Shift, long X, long Y)
{
	UpdateData( TRUE );
	int nErr = g_TaskStacker[eSTACKER_04].ConveyorMoving(eConveyorMove_BKWD,m_bSPEED_CONTROL_4 == TRUE ? true : false);
	if (nErr != ERR_NO_ERROR)
	{
		CParamList AlarmData;
		g_Error.AlarmReport(ERR_STACKER_4_INTERLOCK_CONVEYOR_DRIVER_NOT_WORK, AlarmData, NULL);
	}
}


void CDlgManuSetplate::MouseUpColorbtctrlConveyor1Back5(short Button, short Shift, long X, long Y)
{
	g_TaskStacker[eSTACKER_05].ConveyorStop();
}


void CDlgManuSetplate::MouseDownColorbtctrlConveyor1Back5(short Button, short Shift, long X, long Y)
{
	UpdateData( TRUE );
	int nErr = g_TaskStacker[eSTACKER_05].ConveyorMoving(eConveyorMove_BKWD,m_bSPEED_CONTROL_5 == TRUE ? true : false);
	if (nErr != ERR_NO_ERROR)
	{
		CParamList AlarmData;
		g_Error.AlarmReport(ERR_STACKER_5_INTERLOCK_CONVEYOR_DRIVER_NOT_WORK, AlarmData, NULL);
	}
}


void CDlgManuSetplate::MouseUpColorbtctrlConveyor1Back6(short Button, short Shift, long X, long Y)
{
	g_TaskStacker[eSTACKER_06].ConveyorStop();
}


void CDlgManuSetplate::MouseDownColorbtctrlConveyor1Back6(short Button, short Shift, long X, long Y)
{
	UpdateData( TRUE );
	int nErr = g_TaskStacker[eSTACKER_06].ConveyorMoving(eConveyorMove_BKWD,m_bSPEED_CONTROL_6 == TRUE ? true : false);
	if (nErr != ERR_NO_ERROR)
	{
		CParamList AlarmData;
		g_Error.AlarmReport(ERR_STACKER_6_INTERLOCK_CONVEYOR_DRIVER_NOT_WORK, AlarmData, NULL);
	}
}


void CDlgManuSetplate::MouseUpColorbtctrlConveyor1Back7(short Button, short Shift, long X, long Y)
{
	g_TaskStacker[eSTACKER_07].ConveyorStop();
}


void CDlgManuSetplate::MouseDownColorbtctrlConveyor1Back7(short Button, short Shift, long X, long Y)
{
	UpdateData( TRUE );
	int nErr = g_TaskStacker[eSTACKER_07].ConveyorMoving(eConveyorMove_BKWD,m_bSPEED_CONTROL_7 == TRUE ? true : false);
	if (nErr != ERR_NO_ERROR)
	{
		CParamList AlarmData;
		g_Error.AlarmReport(ERR_STACKER_7_INTERLOCK_CONVEYOR_DRIVER_NOT_WORK, AlarmData, NULL);
	}
}


void CDlgManuSetplate::MouseUpColorbtctrlConveyor1For(short Button, short Shift, long X, long Y)
{
	g_TaskStacker[eSTACKER_01].ConveyorStop();
}


void CDlgManuSetplate::MouseDownColorbtctrlConveyor1For(short Button, short Shift, long X, long Y)
{
	UpdateData( TRUE );
	int nErr = g_TaskStacker[eSTACKER_01].ConveyorMoving(eConveyorMove_FWD,m_bSPEED_CONTROL_1 == TRUE ? true : false);
	if (nErr != ERR_NO_ERROR)
	{
		CParamList AlarmData;
		g_Error.AlarmReport(ERR_STACKER_1_INTERLOCK_CONVEYOR_DRIVER_NOT_WORK, AlarmData, NULL);
	}
}


void CDlgManuSetplate::MouseUpColorbtctrlConveyor1For2(short Button, short Shift, long X, long Y)
{
	g_TaskStacker[eSTACKER_02].ConveyorStop();
}


void CDlgManuSetplate::MouseDownColorbtctrlConveyor1For2(short Button, short Shift, long X, long Y)
{
	UpdateData( TRUE );
	int nErr = g_TaskStacker[eSTACKER_02].ConveyorMoving(eConveyorMove_FWD,m_bSPEED_CONTROL_2 == TRUE ? true : false);
	if (nErr != ERR_NO_ERROR)
	{
		CParamList AlarmData;
		g_Error.AlarmReport(ERR_STACKER_2_INTERLOCK_CONVEYOR_DRIVER_NOT_WORK, AlarmData, NULL);
	}
}


void CDlgManuSetplate::MouseUpColorbtctrlConveyor1For3(short Button, short Shift, long X, long Y)
{
	g_TaskStacker[eSTACKER_03].ConveyorStop();
}


void CDlgManuSetplate::MouseDownColorbtctrlConveyor1For3(short Button, short Shift, long X, long Y)
{
	UpdateData( TRUE );
	int nErr = g_TaskStacker[eSTACKER_03].ConveyorMoving(eConveyorMove_FWD,m_bSPEED_CONTROL_3 == TRUE ? true : false);
	if (nErr != ERR_NO_ERROR)
	{
		CParamList AlarmData;
		g_Error.AlarmReport(ERR_STACKER_3_INTERLOCK_CONVEYOR_DRIVER_NOT_WORK, AlarmData, NULL);
	}
}


void CDlgManuSetplate::MouseUpColorbtctrlConveyor1For4(short Button, short Shift, long X, long Y)
{
	g_TaskStacker[eSTACKER_04].ConveyorStop();
}


void CDlgManuSetplate::MouseDownColorbtctrlConveyor1For4(short Button, short Shift, long X, long Y)
{
	UpdateData( TRUE );
	int nErr = g_TaskStacker[eSTACKER_04].ConveyorMoving(eConveyorMove_FWD,m_bSPEED_CONTROL_4 == TRUE ? true : false);
	if (nErr != ERR_NO_ERROR)
	{
		CParamList AlarmData;
		g_Error.AlarmReport(ERR_STACKER_4_INTERLOCK_CONVEYOR_DRIVER_NOT_WORK, AlarmData, NULL);
	}
}


void CDlgManuSetplate::MouseUpColorbtctrlConveyor1For5(short Button, short Shift, long X, long Y)
{
	g_TaskStacker[eSTACKER_05].ConveyorStop();
}


void CDlgManuSetplate::MouseDownColorbtctrlConveyor1For5(short Button, short Shift, long X, long Y)
{
	UpdateData( TRUE );
	int nErr = g_TaskStacker[eSTACKER_05].ConveyorMoving(eConveyorMove_FWD,m_bSPEED_CONTROL_5 == TRUE ? true : false);
	if (nErr != ERR_NO_ERROR)
	{
		CParamList AlarmData;
		g_Error.AlarmReport(ERR_STACKER_5_INTERLOCK_CONVEYOR_DRIVER_NOT_WORK, AlarmData, NULL);
	}
}


void CDlgManuSetplate::MouseUpColorbtctrlConveyor1For6(short Button, short Shift, long X, long Y)
{
	g_TaskStacker[eSTACKER_06].ConveyorStop();
}


void CDlgManuSetplate::MouseDownColorbtctrlConveyor1For6(short Button, short Shift, long X, long Y)
{
	UpdateData( TRUE );
	int nErr = g_TaskStacker[eSTACKER_06].ConveyorMoving(eConveyorMove_FWD,m_bSPEED_CONTROL_6 == TRUE ? true : false);
	if (nErr != ERR_NO_ERROR)
	{
		CParamList AlarmData;
		g_Error.AlarmReport(ERR_STACKER_6_INTERLOCK_CONVEYOR_DRIVER_NOT_WORK, AlarmData, NULL);
	}
}


void CDlgManuSetplate::MouseUpColorbtctrlConveyor1For7(short Button, short Shift, long X, long Y)
{
	g_TaskStacker[eSTACKER_07].ConveyorStop();
}


void CDlgManuSetplate::MouseDownColorbtctrlConveyor1For7(short Button, short Shift, long X, long Y)
{
	UpdateData( TRUE );
	int nErr = g_TaskStacker[eSTACKER_07].ConveyorMoving(eConveyorMove_FWD,m_bSPEED_CONTROL_7 == TRUE ? true : false);
	if (nErr != ERR_NO_ERROR)
	{
		CParamList AlarmData;
		g_Error.AlarmReport(ERR_STACKER_7_INTERLOCK_CONVEYOR_DRIVER_NOT_WORK, AlarmData, NULL);
	}
}


void CDlgManuSetplate::OnBnClickedBtnStk1VibrationForward()
{
	do_Vibrator_Cyl( eSTACKER_04 );
}

void CDlgManuSetplate::OnBnClickedBtnStk1VibrationBack()
{
	do_Vibrator_Cyl( eSTACKER_04, DEF_BACK );
}

void CDlgManuSetplate::OnBnClickedBtnStk2VibrationForward()
{
	do_Vibrator_Cyl( eSTACKER_05 );
}

void CDlgManuSetplate::OnBnClickedBtnStk2VibrationBack()
{
	do_Vibrator_Cyl( eSTACKER_05, DEF_BACK );
}

void CDlgManuSetplate::OnBnClickedBtnStk3VibrationForward()
{
	do_Vibrator_Cyl( eSTACKER_06 );
}

void CDlgManuSetplate::OnBnClickedBtnStk3VibrationBack()
{
	do_Vibrator_Cyl( eSTACKER_06, DEF_BACK );
}

void CDlgManuSetplate::OnBnClickedBtnStk4VibrationForward()
{
	do_Vibrator_Cyl( eSTACKER_07 );
}

void CDlgManuSetplate::OnBnClickedBtnStk4VibrationBack()
{
	do_Vibrator_Cyl( eSTACKER_07, DEF_BACK );
}


void CDlgManuSetplate::OnBnClickedOk()
{
	CDialogEx::OnOK();
}


HBRUSH CDlgManuSetplate::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);
	// TODO:  여기서 DC의 특성을 변경합니다.
	switch (nCtlColor)
	{
		case CTLCOLOR_STATIC:
		{
			if (pWnd->GetDlgCtrlID() == UNITSETDLL_IDC_STATIC_TIME3)
			{
				CylinderSpecInOut(pDC);		
				//pDC->SetBkColor(RGB(0, 255, 0));
			}
		}
	}
	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

void CDlgManuSetplate::CylinderSpecIndex(int& nCmdManualCyl, int& nCmdParam_Loc)
{
	switch (m_nCmdManualCyl)
	{
	case eCylManualCmd_EachUp:
	{
		nCmdManualCyl = eStackerUp;
		if (m_nCmdParam_Loc == CTaskStacker::yC_TRAY_MAIN_PUSHER_UP)
			nCmdParam_Loc = eStackerMain;
		else
			nCmdParam_Loc = eStackerSub;
	}
	break;
	case eCylManualCmd_EachDown:
	{
		nCmdManualCyl = eStackerDown;
		if (m_nCmdParam_Loc == CTaskStacker::yC_TRAY_MAIN_PUSHER_DOWN)
			nCmdParam_Loc = eStackerMain;
		else
			nCmdParam_Loc = eStackerSub;
	}
	break;
	
	case eCylManualCmd_EachFwd:
	{
		nCmdManualCyl = eStackerUp;
		nCmdParam_Loc = eStackerAlignCylinder;
	}
	break;
	case eCylManualCmd_EachBak:
	{
		nCmdManualCyl = eStackerDown;
		nCmdParam_Loc = eStackerAlignCylinder;
	}
	break;

// 	case eCylManualCmd_EachClamp:
// 	{
// 		nCmdManualCyl = eStackerUp;
// 		nCmdParam_Loc = eStackerClamp;
// 	}
// 	case eCylManualCmd_EachUnclamp:
// 	{
// 		nCmdManualCyl = eStackerDown;
// 		nCmdParam_Loc = eStackerClamp;
// 	}break;

	default:
		break;
	}
}

void CDlgManuSetplate::CylinderSpecInOut(CDC* pDC)
{	
	int nCmdManualCyl=0, nCmdParam_Loc =0;
	
	CylinderSpecIndex(nCmdManualCyl, nCmdParam_Loc);

	if(m_dCylinderSpec[m_nCmdTaskerId][nCmdParam_Loc][nCmdManualCyl].dMin <= m_dCylActionTimeRecord && 
		m_dCylinderSpec[m_nCmdTaskerId][nCmdParam_Loc][nCmdManualCyl].dMax >= m_dCylActionTimeRecord)
		pDC->SetTextColor(RGB(0, 0, 255));
	else
		pDC->SetTextColor(RGB(255, 0, 0));	

}

void CDlgManuSetplate::CylinderSpecUpdate()
{
	int nCmdManualCyl = 0, nCmdParam_Loc = 0;

	CylinderSpecIndex(nCmdManualCyl, nCmdParam_Loc);

	CString str;
	str.Format("%.2f ~ %.2f", m_dCylinderSpec[m_nCmdTaskerId][nCmdParam_Loc][nCmdManualCyl].dMin, m_dCylinderSpec[m_nCmdTaskerId][nCmdParam_Loc][nCmdManualCyl].dMax);
	GetDlgItem(UNITSETDLL_IDC_STATIC_SPEC)->SetWindowText(str);
}

void CDlgManuSetplate::CylinderNum(int nNum)
{
	m_nCylinderNum = nNum;
}