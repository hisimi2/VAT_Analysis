#pragma once

#include "DEF_MOTION.h"
#include "MyList.h"
#include "OneAxis.h"
#include "DlgMotionSim.h"

class OneAxis;

class CMotion_Cont_Imp
{
public:
	CMotion_Cont_Imp(void);
	~CMotion_Cont_Imp(void);

	int     Initialize(char* szIniFileName, int nFileLen);
	void    Finalize();
		    
	int     GetAxisCount();
	int     GetMotorParam(int nRobotNo, _tMOTOR_PARAM& rData);
	BOOL    GetInitBoardStatus(){return m_bInitBoardStatus;};	// return = Board initial status
	void    ShowDialogSimul();

	OneAxis* operator [](int nIndex);
	OneAxis* GetAxis(int nIndex);
	int GetDriverType(){ return m_nDriverType; };
private:
	OneAxis*			m_aAxis[MAX_AXIS_COUNT_ONE_PC];
	int					m_nAxisCount;
	BOOL				m_bInitBoardStatus;
	CMyList             m_ListAxisInfoDb;
	CDlgMotionSim*      m_pDlgSimul;
	int                 m_nDriverType;
	BOOL                m_bInitAxisStatus;
	int					m_nBoardType;
public:
	enum eSimulRespMode{eZeroTimeResp = 0, eRealTimeResp};
	int                 m_nSimulResponseMode; // 0=zero time, 1=real time
};

/* application
  CMotion_Cont g_Motor;

  g_Motor.Create(szIniFileName, strlen(szIniFileName));

  g_Motor[nAxisNo]->Move(1000);

  g_Motor.Destroy();
*/


/* dll source
  CMotion_Cont_Imp* g_pMotionImp = new CMotion_Cont_Imp;
  g_pMotionImp->Create(szIniFileName, strlen(szIniFileName));

  g_pMotionImp[nAxisNo]->Move(1000);

  CMyList AxisInfoList;
  DE_Select_RobotTbl(&AxisInfoList);
  for(int i=0; i<AxisInfoList.m_fnGetCount(); i++)
  {
    CAxisInfoItem* pItem = (CAxisInfoItem*)AxisInfoList.m_fnGetItem(i);
    m_aAxis[i]->Init(pItem->m_stAxisInfo.nAxisNo_Physical, pItem->m_stAxisInfo.dScaleFactor);
  }


  g_pMotionImp->Destroy();
  delete g_pMotionImp;
  g_pMotionImp = NULL;
*/

