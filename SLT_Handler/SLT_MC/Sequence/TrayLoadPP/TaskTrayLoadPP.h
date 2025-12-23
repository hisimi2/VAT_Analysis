#pragma once

#include "./Sequence/Base/TaskTrayBasePP.h"

class CTaskTrayLoadPP : public CTaskTrayBasePP
{
public:
	enum eDI {
		xLOADER_TRAY_VACUUM_1_SEN = 0,
		xLOADER_TRAY_VACUUM_2_SEN,
		xLOADER_TRAY_VACUUM_3_SEN,
		xLOADER_TRAY_VACUUM_4_SEN,
		xLOADER_TRAY_VACUUM_5_SEN,
		xLOADER_TRAY_VACUUM_6_SEN,
		xLOADER_TRAY_VACUUM_7_SEN,
		xLOADER_TRAY_VACUUM_8_SEN,

		xMAX_COUNT,
	};

	enum eDO {
		yLOADER_TRAY_SET1 = 0,
		yLOADER_TRAY_SET2,
		yLOADER_TRAY_SET3,
		yLOADER_TRAY_SET4,
		yLOADER_TRAY_SET5,
		yLOADER_TRAY_SET6,
		yLOADER_TRAY_SET7,
		yLOADER_TRAY_SET8,
		yLOADER_TRAY_RESET1,
		yLOADER_TRAY_RESET2,
		yLOADER_TRAY_RESET3,
		yLOADER_TRAY_RESET4,
		yLOADER_TRAY_RESET5,
		yLOADER_TRAY_RESET6,
		yLOADER_TRAY_RESET7,
		yLOADER_TRAY_RESET8,

		yLOADER_TRAY_BLOW1,
		yLOADER_TRAY_BLOW2,
		yLOADER_TRAY_BLOW3,
		yLOADER_TRAY_BLOW4,
		yLOADER_TRAY_BLOW5,
		yLOADER_TRAY_BLOW6,
		yLOADER_TRAY_BLOW7,
		yLOADER_TRAY_BLOW8,

		yMAX_COUNT,
	};

	// base
	CTaskTrayLoadPP();
	~CTaskTrayLoadPP() = default;
	int         Initialize(int nTrayPPIdx, char* szDriverFileName);
	void        Finalize();

	void		OnOperatorCommand(int nOperatorCommand, WPARAM wParam, LPARAM lParam);

	// common
	void		MakeLog( LPCTSTR fmt, ... ) final;
	void		MakeVatLog (LPCTSTR fmt, ...) final;



	int			WhereIsMotorPosXY( eLocIdxTrayPP& pos_xy ) final;
	BOOL        GetPickerAlignStatus(int nX, int nY);
	void        GetPickerOffset(double &dOffsetX, double &dOffsetY, int nX, int nY);
	
	void        ApplyVATOffset();
	bool        is_safety_state();

private:
	void		ThreadProc1();

	// DIO
	int			ChkDI( eDI nIndex, eOnOffState bValue = eOn );
	eOnOffState GetDI( eDI nIndex );
	int			SetDO( eDO nIndex, eOnOffState eOnOff );


public:
	BOOL  m_bSkipToAssign;

//pcd & Backlash
public:
	void ApplyBacklash();
//VAT
public:
	HWND m_hLog;
	
	void		SetVatLogHandle(HWND hOwner);
	void		WriteDisplayLog(const char * fmt, ...);
	void        MakeVatPreparePos(double & dX, double & dY, double dPrepareOffset);

	bool		IsQALoadPickEnd(int nQaSampleCnt, int nLotLoadCnt, bool bAutoQAUse, eQAStatus nQAStatus);
	int			GetDeviceFullCount();
};

