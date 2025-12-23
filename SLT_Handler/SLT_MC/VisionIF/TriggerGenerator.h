#pragma once
//#include "Trigger/xxEnc852Serial.h"
/*#include "Trigger/xxEnc852Serial.h"*/
//#include "../Trigger/xxEnc852Serial.h"
#include "Trigger/xxEnc852Serial.h"
#define DEF_COMPORT								_T("COM%d")
#define DEF_TRIGGER_MAX							4

#define DEF_TID_2D_DATA_RESULT_MODULE1					(0)
#define DEF_TID_2D_DATA_RESULT_MODULE2					(1)

class CTriggerGenerator
{
public:
	CTriggerGenerator();
	~CTriggerGenerator();
	
	int Connect();
	int DisConnect();

//	int SetTriggerPos(LPCTSTR _recipe_name);
	int SetTriggerPos(int nTblIdx);
	int SetTriggerManulPos(int nTblIdx);
	void SetStartTrigger(int nTblIdx);
	void SetEndTrigger(bool _is_loader);
	void GetTriggerInform(int nTblIdx, double& trigger_pos0, double& trigger_pos1, double& trigger_interval, int& dir);
	int GetTriggerCount(int nTriggerId);
	int GetTriggerCurPos(int nTriggerId);

	void ChkTriggerParameter();
	void ClearTriggerCount(int nTriggerId);
	void SetTblResolution(double dTblResolution);
	double GetTblResolution() { return m_dTblResolution; };
	double m_dTblResolution;

	BOOL IsOpen();

private:
	xxEnc852Serial serial_;
	afx_msg void OnTimer(UINT nIDEvent);

private:
	BOOL m_bConfig_2D_BarCode_Use;
	int  m_nConfig_IO_Driver_Type;
};
