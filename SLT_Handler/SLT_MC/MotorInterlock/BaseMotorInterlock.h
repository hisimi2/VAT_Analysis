#pragma once
class CBaseMotorInterlock
{
public:
	CBaseMotorInterlock(void);
	~CBaseMotorInterlock(void);

	int CommonInterlock();
	int CommonMotorInterlock(OneAxis *pMotor, BOOL bOrgChk = TRUE);
};

