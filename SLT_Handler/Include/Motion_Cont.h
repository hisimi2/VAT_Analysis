#pragma once

#include "DEF_MOTION.h"
#include "OneAxis.h"
#include "stdafx.h"

class CMyList;
class OneAxis;
class CBbxmnpClient;
class AFX_EXT_CLASS CMotion_Cont
{
public:
	CMotion_Cont(void);
	~CMotion_Cont(void);

	int GetAxisCount();
	int DB_Select_RobotTbl(CMyList& AxisInfoList);

	int     Create(char* szIniFileName, int nFileLen);
	void    Destroy();

	void ShowDialogSimul();
	int GetDriverType();
	CBbxmnpClient* GetbbxmnpClient();

	OneAxis* operator [](int nIndex);
};

/* ==== sample code ====
CMotion_Cont g_Motor;
g_Motor.Create();

g_Motor[axis_no]->Move(0);

CMyList AxisInfo;
g_Motor.DB_Select_AxisTbl(&AxisInfo

g_Motor.Destroy();
*/
