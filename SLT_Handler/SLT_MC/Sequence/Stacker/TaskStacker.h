#pragma once

#include <vector>
#include "DelaySensor.h"

class CBaseTaskManager;

class CTaskStacker : public CBaseTaskManager
{
public:
	enum eThreadNum : int
	{
		eTHREAD_1,
	};

	enum eDI : int
	{
		xC_TRAY_HOLD_FOR_SEN = 0, // new
		xC_TRAY_HOLD_BACK_SEN,	  // new

		xC_TRAY_ALIGN_FOR_SEN,
		xC_TRAY_ALIGN_BACK_SEN,

		xC_TRAY_CV_REAR_EXIST_SEN,
		xC_TRAY_CV_REAR_SLOW_SEN,
		
		xC_TRAY_FRONT_LEFT_LOCK_SEN,
		xC_TRAY_REAR_LEFT_LOCK_SEN,
		xC_TRAY_FRONT_RIGHT_LOCK_SEN,
		xC_TRAY_REAR_RIGHT_LOCK_SEN,
		
		xC_TRAY_SUB_PUSHER_UP_SEN,
		xC_TRAY_SUB_PUSHER_DOWN_SEN,
		
		xC_TRAY_MAIN_PUSHER_UP_SEN,
		xC_TRAY_MAIN_PUSHER_DOWN_SEN,
		
		xC_TRAY_STACKER_FULL_SEN,
		xC_TRAY_STACKER_EXIST_SEN,
		
		xC_TRAY_CV_FRONT_EXIST_SEN,
		xC_TRAY_CV_FRONT_SLOW_SEN,

		xCV_ALARM,

		xMAX_COUNT
	};

	enum eDO : int
	{
		yCV_HIGH_SPEED_CONTROL,
		yCV_FWD_CONTROL,
		yCV_REV_CONTROL,

		yCV_C_TRAY_ALIGN_FOR,  // new
		yCV_C_TRAY_ALIGN_BACK, // new

		yC_TRAY_LOCK,
		yC_TRAY_UNLOCK,

		yC_TRAY_SUB_PUSHER_UP,
		yC_TRAY_SUB_PUSHER_DOWN,

		yC_TRAY_MAIN_PUSHER_UP,
		yC_TRAY_MAIN_PUSHER_DOWN,

		yCV_VIBRATOR_FOR,
		yCV_VIBRATOR_BACK,

		yMAX_COUNT
	};

	CTaskStacker();
	~CTaskStacker();

	int Initialize( TCHAR * szDriverFileName, eStacker_Idx stacker_idx, eStacker_Type stacker_type );
	void Finalize();

	void ThreadProc1();

	int OnEvent( const CBaseTaskManager* pSender, int nEventId, CParamList* pDataList = NULL );

	void OnOperatorCommand( int nOperatorCommand, WPARAM wParam, LPARAM lParam );

	void MakeLog( LPCTSTR fmt, ... );


	// DIO (Cylinder)
	int		ChkDI( int nIndex, BOOL bValue = TRUE );
	BOOL	GetDI( int nIndex );
	int		GetDI( int nIndex, BOOL bValue );

	int do_cyl_LockUnlock( int nLockUnlock );
	int chk_cyl_LockUnlock( int nLockUnlock );
	int get_cyl_LockUnlock( int nLockUnlock );

	int do_cyl_MainPusher(int nUpDn, BOOL bNoInterlock = eNoInterLock_OFF);
	int chk_cyl_MainPusher( int nUpDn );
	int get_cyl_MainPusher( int nUpDn );

	int do_cyl_SubPusher( int nUpDn );
	int chk_cyl_SubPusher( int nUpDn );
	int get_cyl_SubPusher( int nUpDn );

	int do_cyl_TrayAlign( int nFwdBwd );
	int chk_cyl_TrayAlign( int nFwdBwd );
	int get_cyl_TrayAlign( int nFwdBwd );

	int do_cyl_Vibrator( int nFwdBwd );
	
	int ConveyorStop();
	void ConveyorSlowStop();
	int ConveyorMoving( eConveyorMoveDir eMoveDir, bool bHighSpeed );
	bool IsConveyorMoving();
	int ChangeConveyorSpeed( bool bHighSpeed );

	bool ChkConveyorAlarm();
	int ChkConveyorDriveCVAlarm();

	bool GetFullTray();
	bool ChkFullTray(BOOL bOnOff);
	
	bool ChkEmptyTray();
	bool GetEmptyTray();

	int GetConveyorTrayDeviceCnt();

	int ChkExistTray_FrontPos_Sen( int nExist );
	int ChkExistTray_RearPos_Sen( int nExist );
	int GetExistTray_FrontPos_Sen( int nExist );
	int GetExistTray_RearPos_Sen( int nExist );

	int GetExistTray_FrontPos_DvcMode(int nExist);
	int GetExistTray_RearPos_DvcMode(int nExist);

	eStacker_Idx GetIndex();
	eStacker_Type GetStackerType();
	// Physical Information

	std::vector<int>                   m_vX;           // DI id list
	std::vector<int>                   m_vY;           // DO id list

	CMap<int, int, CString, CString>      m_mapDI;
	CMap<int, int, CString, CString>      m_mapDO;
	CMap<int, int, CString, CString>      m_mapMotor;

	int GetLoadTrayStatus();

private:
	eStacker_Idx m_eStackerIdx;	// Zero base
	eStacker_Type m_eStackerType;

	CDelaySensor m_StackerTrayExistDelaySen;
	void Sub_Application();
};



