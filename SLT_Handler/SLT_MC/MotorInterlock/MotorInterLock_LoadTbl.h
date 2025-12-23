#pragma once
#include "basemotorinterlock.h"
class CMotorInterLock_LoadTbl :	public CBaseMotorInterlock
{
public:
	CMotorInterLock_LoadTbl(int nLoadTblIdx, int nTestPpIdx);
	~CMotorInterLock_LoadTbl(void);

	int m_nLoadTblIdx;
	int m_nTestPpIdx;

// 	int Interlock_x_org();
// 	int Interlock_x_mov(double dTPos, double dSpd, int nAcc, int nDcc);

	int Interlock_y_org();
	int Interlock_y_mov( double dTPos, double dSpd, int nAcc, int nDcc );
	int Interlock_y_jog( double dSpd, int nAcc, int nDcc );

	int Interlock_Rotate_org();
	int Interlock_Rotate_mov(double dTPos, double dSpd, int nAcc, int nDcc);
	int Interlock_Rotate_jog(double dSpd, int nAcc, int nDcc);

private:
	int Interlock_common();
	int Interlock_rotate_common();
	int Interlock_trayPP_z_position_check();
};

