#pragma once

#include "./Sequence/Base/TaskTrayBasePP.h"

class CTaskTrayUnloadPP : public CTaskTrayBasePP
{
public:
	enum eWorkSite {
		eWorkSite_NotAvaliable = 0,
		eWorkSite_Conveyor,
	};

	enum eDI  {
		xC_TRAY_HAND_R1_VACUUM_SEN = 0,
		xC_TRAY_HAND_R2_VACUUM_SEN,
		xC_TRAY_HAND_R3_VACUUM_SEN,
		xC_TRAY_HAND_R4_VACUUM_SEN,
		xC_TRAY_HAND_F1_VACUUM_SEN,
		xC_TRAY_HAND_F2_VACUUM_SEN,
		xC_TRAY_HAND_F3_VACUUM_SEN,
		xC_TRAY_HAND_F4_VACUUM_SEN,
		xC_TRAY_HAND_Y_SAFETY_AREA_SEN,

		xMAX_COUNT,
	};

	enum eDO {
		yC_TRAY_HAND_R1_VACUUM_RESET = 0,
		yC_TRAY_HAND_R2_VACUUM_RESET,
		yC_TRAY_HAND_R3_VACUUM_RESET,
		yC_TRAY_HAND_R4_VACUUM_RESET,
		yC_TRAY_HAND_F1_VACUUM_RESET,
		yC_TRAY_HAND_F2_VACUUM_RESET,
		yC_TRAY_HAND_F3_VACUUM_RESET,
		yC_TRAY_HAND_F4_VACUUM_RESET,

		yC_TRAY_HAND_R1_VACUUM_SET,
		yC_TRAY_HAND_R2_VACUUM_SET,
		yC_TRAY_HAND_R3_VACUUM_SET,
		yC_TRAY_HAND_R4_VACUUM_SET,
		yC_TRAY_HAND_F1_VACUUM_SET,
		yC_TRAY_HAND_F2_VACUUM_SET,
		yC_TRAY_HAND_F3_VACUUM_SET,
		yC_TRAY_HAND_F4_VACUUM_SET,

		yC_TRAY_HAND_R1_BLOW,
		yC_TRAY_HAND_R2_BLOW,
		yC_TRAY_HAND_R3_BLOW,
		yC_TRAY_HAND_R4_BLOW,
		yC_TRAY_HAND_F1_BLOW,
		yC_TRAY_HAND_F2_BLOW,
		yC_TRAY_HAND_F3_BLOW,
		yC_TRAY_HAND_F4_BLOW,
		
		yC_TRAY_HAND_VAT_CAM_UP,
		yC_TRAY_HAND_VAT_CAM_DN,

		yMAX_COUNT,
	};

public:
	CTaskTrayUnloadPP();
	~CTaskTrayUnloadPP() = default;

	int         Initialize(int nTrayPPIdx, char* szDriverFileName);
	void        Finalize();
			    
	int         OnEvent(const CBaseTaskManager* pSender, int nEventId, CParamList* pDataList = NULL);
	void	    OnOperatorCommand(int nOperatorCommand, WPARAM wParam, LPARAM lParam);
			    
	int         WhereIsMotorPosXY(eLocIdxTrayPP& pos_xy);

public:
	void		MakeLog(LPCTSTR fmt, ...) final;
	void		MakeVatLog(LPCTSTR fmt, ...) final;
    BOOL        GetPickerAlignStatus(int nX, int nY);
	void        GetPickerOffset(double &dOffsetX, double &dOffsetY, int nX, int nY);
	bool		is_safety_state();

	void        ApplyVATOffset();

	int			chk_TrayUnload_Y_Safety_sen(BOOL bOnOff);
	int			get_TrayUnload_Y_Safety_sen(BOOL bOnOff);

private:
	// common
	void		ThreadProc1();

	// DIO, 외부에서 IO 제어가 필요하다면 인터페이스 함수를 만들어서 제공 해주어라.
	// DIO 제어 관련 아래 함수를 public으로 노출 시키지 말 것.
	virtual int			ChkDI(eDI nIndex, eOnOffState bValue = eOn);
	virtual eOnOffState GetDI(eDI nIndex);
	virtual int			SetDO(eDO nIndex, eOnOffState eOnOff);

public:
	BOOL  m_bSkipToAssign;

	//pcd & Backlash
public:
	void ApplyBacklash();
	void MakeVatPreparePos(double & dX, double & dY, double dPrepareOffset);
//VAT
public:
	HWND m_hLog;

	void SetVatLogHandle(HWND hOwner);
	void WriteDisplayLog(const char * fmt, ...);

};

