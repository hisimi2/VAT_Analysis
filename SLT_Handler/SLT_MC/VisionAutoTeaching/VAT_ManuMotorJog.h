#pragma once


// CVAT_ManuMotorJog 대화 상자입니다.

class CVAT_ManuMotorJog : public CDialogEx
{
	DECLARE_DYNAMIC(CVAT_ManuMotorJog)

public:
	CVAT_ManuMotorJog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CVAT_ManuMotorJog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VAT_MANU_MOTOR_JOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();

	

public:
	//Hand 변경 시, 외부 호출 함수.
	void InitHandChange(int nHand);

	//Timer 제거.
	void Kill_Timer();
	

	int m_nCurHand;
	

	//Motor
	OneAxis* m_pMotorX;
	OneAxis* m_pMotorY;
	OneAxis* m_pMotorZ;	
	void InitMotorAxis(int nHand);
	
	//Speed
	std::vector<int> m_vXYSpeed;
	std::vector<int> m_vZSpeed;
	int m_nJogSpeedIdx;
	void InitMotorSpeed();
	void InitComboBox();

	//Display
	void DisplayCurPos();
	void DisplayHandSenStatus();

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	enum eTimer 
	{
		//Manual Cmd 상태 check 및 Jog 동작 block.
		eTimer_CheckExecuteStatus = 0,

		//Display Cur Position
		eTimer_DisplayCurPos,
		
		//Hand Sensor Status
		eTimer_HandSenStatus,

		//Interlock
		eTimer_x_move_left,
		eTimer_x_move_right,

		eTimer_y_move_forward,
		eTimer_y_move_backward,

		eTimer_z_move_up,
		eTimer_z_move_down,

		eTimer_Max,
	};

public:
	enum eVATMotorAxis
	{
		eVATMotor_X = 0,
		eVATMotor_Y,
		eVATMotor_Z,
	};

	//Interlock
	int JogComInterlock(int nAxis);
	BOOL IsJogMoveSpeedNormal(int nAxis);

	DECLARE_EVENTSINK_MAP()
	void MouseUpColorbtctrlXAxisLeft(short Button, short Shift, long X, long Y);
	void MouseDownColorbtctrlXAxisLeft(short Button, short Shift, long X, long Y);
	void MouseUpColorbtctrlXAxisRight(short Button, short Shift, long X, long Y);
	void MouseDownColorbtctrlXAxisRight(short Button, short Shift, long X, long Y);
	void MouseUpColorbtctrlYAxisForward(short Button, short Shift, long X, long Y);
	void MouseDownColorbtctrlYAxisForward(short Button, short Shift, long X, long Y);
	void MouseUpColorbtctrlYAxisBackward(short Button, short Shift, long X, long Y);
	void MouseDownColorbtctrlYAxisBackward(short Button, short Shift, long X, long Y);
	void MouseUpColorbtctrlZAxisUp(short Button, short Shift, long X, long Y);
	void MouseDownColorbtctrlZAxisUp(short Button, short Shift, long X, long Y);
	void MouseUpColorbtctrlZAxisDown(short Button, short Shift, long X, long Y);
	void MouseDownColorbtctrlZAxisDown(short Button, short Shift, long X, long Y);

	//Vacuum/blow
	void ClickVatBtnAllVacuum();
	void ClickVatBtnAllBlow();

	//Jog 동작 block.
	BOOL m_bLastExecuteStatus;
	void EnableJogButton(BOOL bStatus);
};
