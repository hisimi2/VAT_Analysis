#pragma once

#include "basemotorinterlock.h"

class CMotorInterlock_Stacker :	public CBaseMotorInterlock
{
public:
	CMotorInterlock_Stacker(int nStackerIdx=0);
	~CMotorInterlock_Stacker(void);

	int m_nStackerIdx;

private:
};

