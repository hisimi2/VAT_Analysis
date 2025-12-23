#pragma once

#include <string>
#include <vector>
#include <list>
#include "SankyoMotorDrv.h"

class AFX_EXT_CLASS CSankyoDrv : public CWnd
{
	DECLARE_DYNAMIC(CSankyoDrv)

public:
	CSankyoDrv(void);
	~CSankyoDrv(void);

	int Initialize(Sankyo::ST_SANKYO_INIT_INFO stInitInfo);
	void Finalize();
	BOOL IsConnected();
	BOOL IsAutoRunning();
	double GetTorque(int nStation);
	void GetTorqueAll(std::vector<double>& vCont);
	void ShowManualDialog();
	void Op_Action(int nAction);
	void Op_RunMode(int nRunMode);
	BOOL GetAutoRunningHeartBit();


	int Setup_Subscription(int nStation, bool bEnable);
	int Setup_Subscription(std::list<int> vSubscriptionList);	
	bool Setup_GetSubscription(int nStation);
	void Setup_GetSubscriptionAll(std::list<int>& vSubscriptedStation);

	CWnd* m_pDrvObj;

public:
	DECLARE_MESSAGE_MAP()
};

