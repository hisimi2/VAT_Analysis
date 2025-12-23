#pragma once

#include "ConfigData.h"
#include "RegulatorIF.h"

#define DEF_REGULATOR_MAX_PRESSURE_MPA         (0.5)
//#define DEF_PRESS_UNIT_CYL_DIAMETER_DEFAULT    (45.0)
#define DEF_PRESS_UNIT_CYL_DIAMETER_DEFAULT    (40.0)


class CBaseTaskManager;

class CTaskRegulator : public CBaseTaskManager
{
public:
	CTaskRegulator(void);
	~CTaskRegulator(void);

	int Initialize(CWnd * cWnd, int nIdx, char* szDriverFileName);
	void Finalize();

	void ThreadProc1();

	int OnCommand(CBaseSeqCmd* pSeqCmd, int nEqpCtrlCmdType=eEqpCtrlMode_Auto);
	int OnEvent(const CBaseTaskManager* pSender,int nEventId, CParamList* pDataList=NULL);
	int OnCommandRsp(CBaseSeqCmd* pRespCmd, int nEventId, WPARAM wParam, LPARAM lParam);

	void OnOperatorCommand(int nOperatorCommand, WPARAM wParam, LPARAM lParam);


	// Regulator
	int do_SetRegulator(double dPressure);
	int Interlock_SetRegulator(double dPressure);
	int do_SetForcePerPin(double dGram);
	double GetCurrentForcePerPin();
	double ConvertMPa_To_ForcePerPin(double a_dMPa, int nPinCount);
	double ConvertForcePerPin_To_MPa(double a_gf, int nPinCount);
	BOOL   ChkPermitNotOver( double dRegulatorPermit=0.0 );

	// V/V
	int do_RegulatorValve(BOOL bOpen);
	int Interlock_RegulatorValve(BOOL bOpen);

	double GetCurrentPressure();
	double GetCommandPressure();
	double GetTrimForceValue_gf(double a_dForce_gf, int nCylinderSize);
	double GetTrimForceValue_gf(double a_dForce_gf);
	double GetCylinderRadious();
	void SetCylinderRadious(int nCylinderRadious);

	void Read_Cylinder_Config();

	double                             m_dSetPressure;  // MPa
	int                                m_nRegulatorIdx; // enum _ePressUnit

	CConfigData m_ConfigData;
	int m_nCylinder_Size;
	double m_dContactForceSlop_gf[eMaxContactCylinderSizeCount];
	double m_dForce_Offset_gf[eMaxContactCylinderSizeCount];
	double m_dForce_TrimValue_gf;

	std::vector<int>                   m_vX;           // DI id list
	std::vector<int>                   m_vY;           // DO id list
	std::vector<int>                   m_vAI;          // AI id list
	std::vector<int>                   m_vAO;          // AO id list

	CMap<int,int,CString,CString>      m_mapDI;
	CMap<int,int,CString,CString>      m_mapDO;
	CMap<int,int,CString,CString>      m_mapAI;
	CMap<int,int,CString,CString>      m_mapAO;

	double                             m_dCylinderRadius_mm;
	double                             m_dCylinderRadius_cm;
	
	enum eDI{
		xMAX_COUNT,
	};
	enum eDO{
		yREGUL_VV_ON_OFF   = 0,
		//yREGUL_VV_OFF,
		yMAX_COUNT
	};
	enum eAI{
		aiREGUL_IN = 0,
		aiMAX_COUNT,
	};
	enum eAO{
		aoREGUL_OUT = 0,
		aoMAX_COUNT,
	};

//	CRegulatorIF  m_RegulatorIF;
//	ST_REGUL_DATA m_stRegulData;
};
