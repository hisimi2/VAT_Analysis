#pragma once
#include "basemotorinterlock.h"

#define INPOSITION(curpos, startpos, endpos)	(startpos <= curpos && curpos <= endpos) ? TRUE : FALSE

class CMotorInterlock_PressUnit : public CBaseMotorInterlock
{
public:
	CMotorInterlock_PressUnit(int nPressIdx=0,int nTestPpIdx =0);
	~CMotorInterlock_PressUnit(void);
	int m_nPressIdx;
	int m_nTestPpIdx;

	int Interlock_z_org();
	int Interlock_z_mov( double dTPos, double dSpd, int nAcc, int nDcc );
	int Interlock_z_jog( double dSpd, int nAcc, int nDcc );

private:
	int ChangePressIdxToAreaIdx();
	int Interlock_Press_common();
	int ChangeTestPPIdxToAreaIdx(std::vector<int>& vTestPPAreaStartEnd);
	BOOL Interlock_Chk_Testpp_Y_In_Press_Area(int nPressIdx, int nTestPPIdx, int nTestPPType);
};
