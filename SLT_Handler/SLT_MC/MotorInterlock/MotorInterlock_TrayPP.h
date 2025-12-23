#pragma once

#include "basemotorinterlock.h"

class CMotorInterlock_TrayPP : public CBaseMotorInterlock
{
public:
	CMotorInterlock_TrayPP(int nTrayPpIdx = 0);
	~CMotorInterlock_TrayPP(void);

	int m_nTrayPpIdx;

	int Interlock_x_org();
	int Interlock_x_mov( double dTPos, double dSpd, int nAcc, int nDcc );
	int Interlock_x_jog( double dSpd, int nAcc, int nDcc );

	int Interlock_y_org();
	int Interlock_y_mov( double dTPos, double dSpd, int nAcc, int nDcc );
	int Interlock_y_jog( double dSpd, int nAcc, int nDcc );

	int Interlock_Feeder_org();
	int Interlock_Feeder_mov(double dTPos, double dSpd, int nAcc, int nDcc);
	int Interlock_Feeder_jog(double dSpd, int nAcc, int nDcc);


	int Interlock_z_org();
	int Interlock_z_mov( double dTPos, double dSpd, int nAcc, int nDcc );
	int Interlock_z_jog( double dSpd, int nAcc, int nDcc );

	int Interlock_xpitch_org();
	int Interlock_xpitch_mov( double dTPos, double dSpd, int nAcc, int nDcc );
	int Interlock_xpitch_jog( double dSpd, int nAcc, int nDcc );

	int Interlock_ypitch_org();
	int Interlock_ypitch_mov( double dTPos, double dSpd, int nAcc, int nDcc );
	int Interlock_ypitch_jog( double dSpd, int nAcc, int nDcc );

private:
	int Interlock_TrayPP_Common();
	int Interlock_x_common();
	int Interlock_y_common();
	int Interlock_z_common();
	int Interlock_xpitch_common();
	int Interlock_ypitch_common();
	int Interlock_Tray_Feeder_Common();
	BOOL Interlock_TrayPP_Feeder_Impact_Pos(int nHandType, double dCurPos, std::vector<int> &vImpactPkr);
};

