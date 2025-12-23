#pragma once

#include "basemotorinterlock.h"

class CMotorInterlock_Transfer : public CBaseMotorInterlock
{
public:
	CMotorInterlock_Transfer(int nTrayPPInx);
	~CMotorInterlock_Transfer(void);

	int m_nTrayPPInx;
	int Interlock_x_org();
	int Interlock_x_mov( double dTPos, double dSpd, int nAcc, int nDcc );
	int Interlock_x_jog( double dSpd, int nAcc, int nDcc );

	int Interlock_z_org(int nHead);
	int Interlock_z_mov(int nHead, double dTPos, double dSpd, int nAcc, int nDcc);

private:
	int Interlock_Transfer_Common();
	int Interlock_x_common();
	int Interlock_z_common(int nHead);
};

