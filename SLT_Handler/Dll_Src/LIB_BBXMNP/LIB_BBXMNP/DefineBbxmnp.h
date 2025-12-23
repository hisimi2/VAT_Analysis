#ifndef _DEFINE_BBXMNP_
#define _DEFINE_BBXMNP_

#define		PICKER_Z_SAFETY_POS				(0)
#define		eBMAxis_Picker_MAX				(8)
#define		eMULTI_OPER_MAX					(4)
#define		MAX_PICKER_CNT_EACH_SLAVE		(4)
#define		MAX_SLAVE_BOARD_CNT				(12)
#define		LOAD_TRAY_PICKER_SLAVE_NUM1				(1)
#define		LOAD_TRAY_PICKER_SLAVE_NUM2				(2)
#define		TEST_PP1_LOAD_HAND_PICKER_SLAVE_NUM1	(65)
#define		TEST_PP1_LOAD_HAND_PICKER_SLAVE_NUM2	(66)
#define		TEST_PP1_UNLOAD_HAND_PICKER_SLAVE_NUM1	(67)
#define		TEST_PP1_UNLOAD_HAND_PICKER_SLAVE_NUM2	(68)
#define		UNLOAD_TRAY_PICKER_SLAVE_NUM1			(1)
#define		UNLOAD_TRAY_PICKER_SLAVE_NUM2			(2)
#define		TEST_PP2_LOAD_HAND_PICKER_SLAVE_NUM1	(65)
#define		TEST_PP2_LOAD_HAND_PICKER_SLAVE_NUM2	(66)
#define		TEST_PP2_UNLOAD_HAND_PICKER_SLAVE_NUM1	(67)
#define		TEST_PP2_UNLOAD_HAND_PICKER_SLAVE_NUM2	(68)
#define		HAND_PICKER_MASTER_BRD_NUM1		(0)
#define		HAND_PICKER_MASTER_BRD_NUM2		(1)


enum _eBbxmnpSlaveNum {
	eBMAxis_LOAD_TRAY_PICKER_SLAVE_NUM1,
	eBMAxis_LOAD_TRAY_PICKER_SLAVE_NUM2,
	eBMAxis_TEST_PP1_LOAD_HAND_PICKER_SLAVE_NUM1,
	eBMAxis_TEST_PP1_LOAD_HAND_PICKER_SLAVE_NUM2,
	eBMAxis_TEST_PP1_UNLOAD_HAND_PICKER_SLAVE_NUM1,
	eBMAxis_TEST_PP1_UNLOAD_HAND_PICKER_SLAVE_NUM2,
	eBMAxis_UNLOAD_TRAY_PICKER_SLAVE_NUM1 = 0,
	eBMAxis_UNLOAD_TRAY_PICKER_SLAVE_NUM2,
	eBMAxis_TEST_PP2_LOAD_HAND_PICKER_SLAVE_NUM1,
	eBMAxis_TEST_PP2_LOAD_HAND_PICKER_SLAVE_NUM2,
	eBMAxis_TEST_PP2_UNLOAD_HAND_PICKER_SLAVE_NUM1,
	eBMAxis_TEST_PP2_UNLOAD_HAND_PICKER_SLAVE_NUM2,

};
enum _ePickerDirIdx {
	eDirPicker_Picker01 = 0,
	eDirPicker_Picker02,
	eDirPicker_Picker03,
	eDirPicker_Picker04,
	eDirPicker_Picker05,
	eDirPicker_Picker06,
	eDirPicker_Picker07,
	eDirPicker_Picker08,
};

enum _eBbxmnpAxis {
	// LoadTray
	eBMAxis_LOAD_TRAY_Picker01,
	eBMAxis_LOAD_TRAY_Picker02,
	eBMAxis_LOAD_TRAY_Picker03,
	eBMAxis_LOAD_TRAY_Picker04,
	eBMAxis_LOAD_TRAY_Picker05,
	eBMAxis_LOAD_TRAY_Picker06,
	eBMAxis_LOAD_TRAY_Picker07,
	eBMAxis_LOAD_TRAY_Picker08,

	// load Test PP1
	eBMAxis_LOAD_TESTPP_1_Picker01,
	eBMAxis_LOAD_TESTPP_1_Picker02,
	eBMAxis_LOAD_TESTPP_1_Picker03,
	eBMAxis_LOAD_TESTPP_1_Picker04,
	eBMAxis_LOAD_TESTPP_1_Picker05,
	eBMAxis_LOAD_TESTPP_1_Picker06,
	eBMAxis_LOAD_TESTPP_1_Picker07,
	eBMAxis_LOAD_TESTPP_1_Picker08,

	// Unload Test PP1
	eBMAxis_UNLOAD_TESTPP_1_Picker01,
	eBMAxis_UNLOAD_TESTPP_1_Picker02,
	eBMAxis_UNLOAD_TESTPP_1_Picker03,
	eBMAxis_UNLOAD_TESTPP_1_Picker04,
	eBMAxis_UNLOAD_TESTPP_1_Picker05,
	eBMAxis_UNLOAD_TESTPP_1_Picker06,
	eBMAxis_UNLOAD_TESTPP_1_Picker07,
	eBMAxis_UNLOAD_TESTPP_1_Picker08,
	// Unload Tray
	eBMAxis_UNLOAD_TRAY_Picker01,
	eBMAxis_UNLOAD_TRAY_Picker02,
	eBMAxis_UNLOAD_TRAY_Picker03,
	eBMAxis_UNLOAD_TRAY_Picker04,
	eBMAxis_UNLOAD_TRAY_Picker05,
	eBMAxis_UNLOAD_TRAY_Picker06,
	eBMAxis_UNLOAD_TRAY_Picker07,
	eBMAxis_UNLOAD_TRAY_Picker08,

	// load Test PP 2
	eBMAxis_LOAD_TESTPP_2_Picker01,
	eBMAxis_LOAD_TESTPP_2_Picker02,
	eBMAxis_LOAD_TESTPP_2_Picker03,
	eBMAxis_LOAD_TESTPP_2_Picker04,
	eBMAxis_LOAD_TESTPP_2_Picker05,
	eBMAxis_LOAD_TESTPP_2_Picker06,
	eBMAxis_LOAD_TESTPP_2_Picker07,
	eBMAxis_LOAD_TESTPP_2_Picker08,

	// Unload Test PP2
	eBMAxis_UNLOAD_TESTPP_2_Picker01,
	eBMAxis_UNLOAD_TESTPP_2_Picker02,
	eBMAxis_UNLOAD_TESTPP_2_Picker03,
	eBMAxis_UNLOAD_TESTPP_2_Picker04,
	eBMAxis_UNLOAD_TESTPP_2_Picker05,
	eBMAxis_UNLOAD_TESTPP_2_Picker06,
	eBMAxis_UNLOAD_TESTPP_2_Picker07,
	eBMAxis_UNLOAD_TESTPP_2_Picker08,

};

enum _eMoveType {
	eMoveType_INC,	// Relative Move
	eMoveType_ABS	// Absolute Move
};

enum _eJogDir {
	eJogDir_Positive,
	eJogDir_Negative
};

typedef union {
	unsigned int mem;

	struct {
		USHORT EMG:1;
		USHORT ALM:1;
		USHORT P_LMT:1;
		USHORT N_LMT:1;
		USHORT ORG:1;
		USHORT DIR:1;
		USHORT H_OK:1;
		USHORT PCS:1;
		USHORT CRC:1;
		USHORT EZ:1;
		USHORT CLR:1;
		USHORT LAT:1;
		USHORT SD:1;
		USHORT INP:1;
		USHORT SON:1;
		USHORT RST:1;
		USHORT STA:1;
	}mem_bit;
}udtCardStatus;

typedef union
{
	unsigned int mem;
	struct
	{
		USHORT DRV_STOP : 1;		// 모션 구동 중
		USHORT WAIT_DR : 1;			// DR 입력 신호 기다림
		USHORT WAIT_STA : 1;		// STA 입력 신호 기다림
		USHORT WAIT_INSYNC : 1;		// 내부 동기 신호 기다림
		USHORT WAIT_OTHER : 1;		// 타축 정지 신호 기다림
		USHORT WAIT_ERC : 1;		// ERC 출력 완료를 기다림
		USHORT WAIT_DIR : 1;		// 방향 변화를 기다림
		USHORT BACKLASH : 1;		// BACKLASH 상태
		USHORT WAIT_PE : 1;			// PE 입력 신호 기다림
		USHORT IN_FA : 1;			// FA 정속 동작 중 (홈 스페셜 속도)
		USHORT IN_FL : 1;			// FL 정속 동작 중
		USHORT IN_FUR : 1;			// 가속 중
		USHORT IN_FH : 1;			// FH 정속 동작 중
		USHORT IN_FDR : 1;			// 감속 중
		USHORT WAIT_INP : 1;		// INP 입력 신호 기다림
		USHORT IN_CMP : 1;			// CMP 동작 중
		USHORT WAIT_SYNC : 1;		// SYNC 동작 중
		USHORT WAIT_GANT : 1;		// GANT 동작 중
	}mem_bit;
}uMotionStatus;

typedef void (*FN_PARENT_CALLBACK) (CWnd* pWnd, LPARAM lParam, WPARAM wParam);
#endif