#pragma once

#include <vector>
#include "TesterIF\TesterIF.h"
//==============================================================================
//
//==============================================================================
class CBaseTaskManager;


//==============================================================================
//
//==============================================================================
class CTaskPressUnit : public CBaseTaskManager
{
public:
	CTaskPressUnit(void);
	~CTaskPressUnit(void);

	int Initialize(int nUnitIdx, char* szDriverFileName);
	void Finalize();

	void ThreadProc1();

	int OnEvent(const CBaseTaskManager* pSender,int nEventId, CParamList* pDataList=NULL);
	void OnOperatorCommand(int nOperatorCommand, WPARAM wParam, LPARAM lParam);

	// Motor
	OneAxis* GetAxis(int nAxisIdx);
	int do_MovePos(int nLocStage, int nSpdRate=100, double dPosOffset=0.0);
	int Interlock_MovePos(int nLoc);

	int GetTeachPos(int nLocStage, double* pdPosZ);
	int ChkMotorPos(int nLoc, int nAxisIdx=eAxisAll);
	int ChkMotorPos(int nLoc, double dOffset, int nAxisIdx=eAxisAll);
	int WhereIsMotorPos(std::vector<int>& vLocList, int nAxisIdx=eAxisAll);
	int GetPressUnitIndex(int nPressIndex);

	// Status info
	int  GetSiteDvcTestingInfo(int nX, int nY);
	int  SetSiteDvcTestingInfo(int nX, int nY, int nStatus);
	int  GetStationInfo(BOOL bGrr=FALSE, BOOL bTesting =FALSE);
	int  GetAuditStationInfo();

	// Device data
	int  GetSiteDeviceData(ST_DD_DEVICE* lpstBuff, int nX, int nY );
	int  SetSiteDeviceData(ST_DD_DEVICE* lpstBuff, int nX, int nY );
	int  ChkSiteInDvc();
	int  GetSiteInDvc(BOOL bRetestBin=FALSE);
	int  GetSiteInDvcTestPossible();
	// Etc
	void SetFailHistoryAndPFcnt(int nX, int nY, int nPassFail, int nTestCnt);
	int  GetCalcTestingCnt(int nCurTestCnt, ST_DD_DEVICE & stDvcData );
	void PushYieldData(int nX, int nY, int nPassFail,int nTestCnt = -1);

	int  ConvertStageIdx(int nAutoAreaIdx, int* pnStageIdx);	

	void SetNotTestHistory(int nX, int nY);
	void SetHwBinHistory(int nX, int nY, int nHwBinNo);

	void SetTestStatusHistory(int eStatus);

	int  GetSocketOnCnt(BOOL bGrrMode=FALSE);
	int  GetSocketOnOff(int nX, int nY);
	int  GetSocketOnOffGrr(int nX, int nY);
	int  GetSocketOnOffAudit(int nX, int nY);
	int GetSocketOnCntGrr();
	BOOL GetStationOn(BOOL bGrrMode=FALSE);
	int GetSocketAuditOnCnt();
	int GetSocketAuditOnOffCnt(BOOL bReserve = FALSE);
	int CalYieldMonitoring();

	//sensor
	BOOL GetDI(int nIndex);
	int  ChkDI(int nIndex, BOOL bValue=TRUE);
	int  ChkSocketArrivedSen_All( CString &strErrMsg, std::vector<CPoint> &vErrPocket, std::vector<CString> &vErrHistor, int &nDeviceType, BOOL isSafety = FALSE);
	int  ChkSocketArrivedSen(CString &strErrMsg, std::vector<CPoint> &vErrPocket, std::vector<CString> &vErrHistor, int &nDeviceType, BOOL isSafety = FALSE);

	// blow
	int  Interlock_VacBlow(int nVacBlow/*=enum eVacBlowState{*/);
	int  do_VacBlow(int nVacBlow/*=enum eVacBlowState{*/);
	int  do_SideVibratorOnOff(int nOnOff);
	BOOL IsInitNeed();

	//void SetTest2DIDErrorBin(int nCmdStage, SYSTEMTIME sysTmTestStart);
	//Press Unit Idx를 이용하여 AutoTestSite Idx 
	int CovertAutoTestSiteIdx(int nStationID);
	//Press Unit Idx를 이용하여 PressArea Idx 
	int CovertPressAreaIdx(int nStationID);

	int GetAllStationFailCnt();
	void AddAllStationFailCnt();
	void ClearAllStationFailCnt();

	//Handler Error Bin으로 설정
	void SetTestTimeOutHandlerErrorBin(int nCmdStage, SYSTEMTIME sysTmTestStart);
	void SetTestHandlerErrorBin(ST_DD_DEVICE& pDvcData, ResultData& rData, int& nTargetPos, char* szHWbin, int& nBinColor,CString& strResult);
	void SetTest2DIDHandlerErrorBin(int nCmdStage, SYSTEMTIME sysTmTestStart, int* n2DIDSiteErrorNumber);

	void SetFail2DIDcnt(int nX, int nY);

	void TesterSiteOnOffCheck(int* pDirSiteOnOff);
	void GetSocketStsForEachSocket(BOOL bGrrMode, BOOL *pSocketSts);
	void GetSocketAuditOnOffForEachSocket(BOOL *pSocketSts);
	int  GetDirDualSocketOnCnt(BOOL bGrrMode, int nDirTester);
	BOOL PressInCleanDeviceCheck(BOOL bGrr);
	int  GetSite1stFailInDvcCnt();

	void SetPressReqTable(BOOL bState, int nTblType);
	BOOL GetPressReqTable(int nTblType);

	void SetTextBinSolution(CString& strBin, int nStationNum);
	int  GetBinColor(ST_DD_DEVICE pDvcData);

	void SaveFileHeightCalData(double dMeasureData);

	CString GetAABString(int nTestCount);

	enum eTeachPosZ {
		eZPOS_OUT_OF_TEACH = -1,
		eZPOS_CONTACT=0,
		eZPOS_UP,
		eZPOS_MAX,

		eMotPos_Between_NegaToSafety = 0,
		eMotPos_Safety,
		eMotPos_Between_SafetyToContact,
		eMotPos_Contact,
		eMotPos_Between_ContactToPosi,	
	};

	enum eDI{
		xZAXIS_SAFETY_SEN = 0,
		xZAXIS_ARRIVED_SAFETY_SEN_1,
		xZAXIS_ARRIVED_SAFETY_SEN_2,
		xZAXIS_ARRIVED_SAFETY_SEN_3,
		xZAXIS_ARRIVED_SAFETY_SEN_4,
		xZAXIS_ARRIVED_SAFETY_SEN_5,
		xZAXIS_ARRIVED_SAFETY_SEN_6,
		xZAXIS_ARRIVED_SAFETY_SEN_7,
		xZAXIS_ARRIVED_SAFETY_SEN_8,
		xZAXIS_ARRIVED_SAFETY_FIBER_SEN_R1,
		xZAXIS_ARRIVED_SAFETY_FIBER_SEN_F1,
		xMAX_COUNT,
	};

	enum eDO{
		yPRESS_BLOW_ON =0,
		yVIBRATOR_ON_OFF =1,
		yVIBRATOR_FOR,
		yVIBRATOR_BACK,
		yMAX_COUNT,
	};

	enum eMotor{
		eAxisAll = -1,
		eAxisZ   =  0,
		eMaxAxisCount
	};
	
	enum eArriveSensorOption
	{
		eAllSite =0,
		eOnly_On_Site,
	};

	enum eArriveSensorType
	{
		eEach = 0,
		eFiberSensor,

		eArriveSensor_ForceAction = 99, //Vibrator 강제 동작을 위한 Type
	};


	enum eTesterHistoryStatus 
	{
		eSetPrepare = 0,
		eSetStatus,
	};

	// Physical Information
	int                                m_nUnitIdx;	   // 0=LoadTbl1, 1=LoadTbl2
	std::vector<int>                   m_vX;           // DI id list
	std::vector<int>                   m_vY;           // DO id list
	std::vector<OneAxis*>              m_vMotor;       // Motor Pointer list x, y
	double                             m_dMotorErrRange[eMaxAxisCount];

	CMap<int,int,CString,CString>      m_mapDI;
	CMap<int,int,CString,CString>      m_mapDO;

	CMap<int,int,CString,CString>      m_mapMotor;

	std::vector<int>              m_vSocketArrsenIdx;

	BOOL m_bInitialized;

    void MakeLog(LPCTSTR fmt, ...);
private:
	int m_AllStationFailCnt;
	BOOL m_bPressReqState[eMaxTblCount/2];
};

