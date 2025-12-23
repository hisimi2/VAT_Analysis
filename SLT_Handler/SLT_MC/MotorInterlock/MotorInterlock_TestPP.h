#pragma once
#include "basemotorinterlock.h"

class CMotorInterlock_TestPP :	public CBaseMotorInterlock
{
public:
	CMotorInterlock_TestPP(int nTestPpIdx=0, int nTestPpType = 0);
	~CMotorInterlock_TestPP(void);

	int m_nTestPpIdx;
	int m_nTestPpType;

	int Interlock_x_org();
	int Interlock_x_mov( double dTPos, double dSpd, int nAcc, int nDcc );
	int Interlock_x_jog( double dSpd, int nAcc, int nDcc );

	int Interlock_y_org();
	int Interlock_y_mov( double dTPos, double dSpd, int nAcc, int nDcc );
	int Interlock_y_jog( double dSpd, int nAcc, int nDcc );

	int Interlock_z_org();
	int Interlock_z_mov( double dTPos, double dSpd, int nAcc, int nDcc );
	int Interlock_z_jog( double dSpd, int nAcc, int nDcc );

	int Interlock_xpitch_org();
	int Interlock_xpitch_mov(double dTPos, double dSpd, int nAcc, int nDcc);
	int Interlock_xpitch_jog(double dSpd, int nAcc, int nDcc);

	int Interlock_ypitch_org();
	int Interlock_ypitch_mov(double dTPos, double dSpd, int nAcc, int nDcc);
	int Interlock_ypitch_jog(double dSpd, int nAcc, int nDcc);


private:
	int Interlock_x_common();
	int Interlock_y_common();
	int Interlock_z_common();
	int Interlock_xpitch_common();
	int Interlock_ypitch_common();
	int Interlock_common();
	int Interlock_Orgcommon();
	int Interlock_PressUnit();

	int Interlock_X_Related_Station_Range(double dTargetX);

	void Interlock_Y_Range_Related_Press_Z_Safety(double dTPos, int nHandType, std::vector<int> &nRelatedStationNo);

//	BOOL CompareXData(int nHandIdx, double dData1, double dData2); //Module1, 2 X축 Station 진입 방향이 다름.
};

