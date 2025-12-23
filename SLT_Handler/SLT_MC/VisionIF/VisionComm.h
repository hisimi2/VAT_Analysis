#pragma once

#include "../Include/DEF_LIB_IPC_SOCK.h"
#include "../Include/IPCSock.h"
#include "../Include/MyList.h"
#include "Logger.h"
#include <vector>


// ==============================================================================
// SLT Vision DATAID 
// ==============================================================================
#define 	VISION_DATAID_RESULT_DATA_CAM1_BAR_CODE_X1Y1   (0)
#define 	VISION_DATAID_RESULT_DATA_CAM1_BAR_CODE_X2Y1   (1)
#define 	VISION_DATAID_RESULT_DATA_CAM1_BAR_CODE_X1Y2   (2)
#define 	VISION_DATAID_RESULT_DATA_CAM1_BAR_CODE_X2Y2   (3)
														   
#define 	VISION_DATAID_RESULT_DATA_CAM2_BAR_CODE_X1Y1   (100)
#define 	VISION_DATAID_RESULT_DATA_CAM2_BAR_CODE_X2Y1   (101)
#define 	VISION_DATAID_RESULT_DATA_CAM2_BAR_CODE_X1Y2   (102)
#define 	VISION_DATAID_RESULT_DATA_CAM2_BAR_CODE_X2Y2   (103)
														   
#define 	VISION_DATAID_RESULT_DATA_CAM3_BAR_CODE_X1Y1   (200)
#define 	VISION_DATAID_RESULT_DATA_CAM3_BAR_CODE_X2Y1   (201)
#define 	VISION_DATAID_RESULT_DATA_CAM3_BAR_CODE_X1Y2   (202)
#define 	VISION_DATAID_RESULT_DATA_CAM3_BAR_CODE_X2Y2   (203)
														   
#define 	VISION_DATAID_RESULT_DATA_CAM4_BAR_CODE_X1Y1   (300)
#define 	VISION_DATAID_RESULT_DATA_CAM4_BAR_CODE_X2Y1   (301)
#define 	VISION_DATAID_RESULT_DATA_CAM4_BAR_CODE_X1Y2   (302)
#define 	VISION_DATAID_RESULT_DATA_CAM4_BAR_CODE_X2Y2   (303)

#define     VISION_DATAID_RESULT_DATA_POCKET_UP_(x)		   (VISION_DATAID_RESULT_DATA_POCKET_UP1+(x*1000))
#define 	VISION_DATAID_RESULT_DATA_POCKET_UP1		   (1000)
#define 	VISION_DATAID_RESULT_DATA_POCKET_UP2		   (2000)
#define 	VISION_DATAID_RESULT_DATA_POCKET_UP3		   (3000)
#define 	VISION_DATAID_RESULT_DATA_POCKET_UP4		   (4000)
#define 	VISION_DATAID_RESULT_DATA_POCKET_UP5		   (5000)
#define 	VISION_DATAID_RESULT_DATA_POCKET_UP6		   (6000)
#define 	VISION_DATAID_RESULT_DATA_POCKET_UP7		   (7000)
#define 	VISION_DATAID_RESULT_DATA_POCKET_UP8		   (8000)

#define     VISION_DATAID_RESULT_DATA_POCKET_DOWN_(x)	   (VISION_DATAID_RESULT_DATA_POCKET_DOWN1+(x*1000))
#define 	VISION_DATAID_RESULT_DATA_POCKET_DOWN1	       (9000)
#define 	VISION_DATAID_RESULT_DATA_POCKET_DOWN2	       (10000)
#define 	VISION_DATAID_RESULT_DATA_POCKET_DOWN3	       (11000)
#define 	VISION_DATAID_RESULT_DATA_POCKET_DOWN4	       (12000)
#define 	VISION_DATAID_RESULT_DATA_POCKET_DOWN5	       (13000)
#define 	VISION_DATAID_RESULT_DATA_POCKET_DOWN6	       (14000)
#define 	VISION_DATAID_RESULT_DATA_POCKET_DOWN7	       (15000)
#define 	VISION_DATAID_RESULT_DATA_POCKET_DOWN8	       (16000)
#define     VISION_DATAID_CAM_BUSY_LOAD_TBL1               (20000)
#define     VISION_DATAID_CAM_BUSY_LOAD_TBL2               (20001)
#define     VISION_DATAID_CAM_BUSY_LOAD_TBL3               (20002)
#define     VISION_DATAID_CAM_BUSY_LOAD_TBL4               (20003)
#define     VISION_DATAID_CAM_BUSY_TEST_SITE_UP_1          (20100)
#define     VISION_DATAID_CAM_BUSY_TEST_SITE_UP_2          (20101)
#define     VISION_DATAID_CAM_BUSY_TEST_SITE_UP_3          (20102)
#define     VISION_DATAID_CAM_BUSY_TEST_SITE_UP_4          (20103)
#define     VISION_DATAID_CAM_BUSY_TEST_SITE_UP_5          (20104)
#define     VISION_DATAID_CAM_BUSY_TEST_SITE_UP_6          (20105)
#define     VISION_DATAID_CAM_BUSY_TEST_SITE_UP_7          (20106)
#define     VISION_DATAID_CAM_BUSY_TEST_SITE_UP_8          (20107)
#define     VISION_DATAID_CAM_BUSY_TEST_SITE_DN_1          (20200)
#define     VISION_DATAID_CAM_BUSY_TEST_SITE_DN_2          (20201)
#define     VISION_DATAID_CAM_BUSY_TEST_SITE_DN_3          (20202)
#define     VISION_DATAID_CAM_BUSY_TEST_SITE_DN_4          (20203)
#define     VISION_DATAID_CAM_BUSY_TEST_SITE_DN_5          (20204)
#define     VISION_DATAID_CAM_BUSY_TEST_SITE_DN_6          (20205)
#define     VISION_DATAID_CAM_BUSY_TEST_SITE_DN_7          (20206)
#define     VISION_DATAID_CAM_BUSY_TEST_SITE_DN_8          (20207)

#define     VISION_DATAID_RESULT_DATA_TEST_HAND(x)	   (VISION_DATAID_RESULT_DATA_TEST_HAND1+(x*1000))
#define		VISION_DATAID_RESULT_DATA_TEST_HAND1		   (25000)
#define		VISION_DATAID_RESULT_DATA_TEST_HAND2		   (26000)

#define     VISION_DATAID_CAM_BUSY_TEST_HAND_(x)	   (VISION_DATAID_CAM_BUSY_TEST_HAND_1+(x))
#define		VISION_DATAID_CAM_BUSY_TEST_HAND_1			   (0)
#define		VISION_DATAID_CAM_BUSY_TEST_HAND_2			   (1)


// ==============================================================================
// SLT Vision Remote Command
// ==============================================================================
#define     VISION_RCMD_REQ_SET_COK		                   (1000)
#define     VISION_RCMD_REQ_INSP_READY	                   (1001)
#define     VISION_RCMD_REQ_CLEAN_DISK	                   (1002)
#define     VISION_RCMD_REQ_SHUTTLE_MOVE                   (1003)
#define     VISION_RCMD_REQ_MEASURE		                   (1004)
#define     VISION_RCMD_REQ_SHUTDOWN	                   (1005)
#define     VISION_RCMD_RPT_SHUTTLE_MOVE_COMP              (1006)
#define     VISION_RCMD_REQ_VISION_USE_NOUSE               (1007)
#define     VISION_RCMD_REQ_MEASURE_EXIST                  (1008)
#define     VISION_RCMD_REQ_LASER_ON                       (1009)
#define     VISION_RCMD_REQ_LASER_OFF                      (1010)
#define		VISION_READY_REQ_VISION_READY				   (1011)
#define		VISION_RCMD_REQ_BARCORDE_LIGHT_ON_OFF		   (1012)
#define     VISION_RCMD_REQ_MEASURE_HAND_CAM               (1014)
#define     VISION_RCMD_REQ_MEASURE_EXIST_HAND_CAM         (1018)

// ==============================================================================
// SLT Vision Data ID
// ==============================================================================
// H -> V
// TEST HAND1
// #define VISION_REQ_MEASURE_TEST_HAND_1_EMPTY				30000
// #define VISION_REQ_MEASURE_TEST_HAND_1_EXIST				30100
// #define VISION_REQ_MEASURE_TEST_HAND_1_CLEAN_DEVICE			30200
// #define VISION_REQ_RESULT_TEST_HAND_1_EMPTY					30001
// #define VISION_REQ_RESULT_TEST_HAND_1_EXIST					30101
// #define VISION_REQ_RESULT_TEST_HAND_1_CLEAN_DEVICE			30201
// TEST HAND2
// #define VISION_REQ_MEASURE_TEST_HAND_2_EMPTY				31000
// #define VISION_REQ_MEASURE_TEST_HAND_2_EXIST				31100
// #define VISION_REQ_MEASURE_TEST_HAND_2_CLEAN_DEVICE			31200
// #define VISION_REQ_RESULT_TEST_HAND_2_EMPTY					31001
// #define VISION_REQ_RESULT_TEST_HAND_2_EXIST					31101
// #define VISION_REQ_RESULT_TEST_HAND_2_CLEAN_DEVICE			31201

//STATION 
//PC1 
#define VISION_REQ_MEASURE_STATION_1_EMPTY					30000
#define VISION_REQ_MEASURE_STATION_2_EMPTY					30001
#define VISION_REQ_MEASURE_STATION_3_EMPTY					30002
#define VISION_REQ_MEASURE_STATION_4_EMPTY					30003
#define VISION_MEASURE_VAT_LD_UPPER							50004
#define VISION_MEASURE_VAT_ULD_UPPER						50005
#define VISION_MEASURE_VAT_TEST_1_LD_UPPER 					50006
#define VISION_MEASURE_VAT_TEST_1_ULD_UPPER 				50007
#define VISION_MEASURE_VAT_LD_LOWER 						50008
#define VISION_MEASURE_VAT_ULD_LOWER 						50009
#define VISION_MEASURE_VAT_TEST_1_LOWER 					50010

//PC2
#define VISION_REQ_MEASURE_STATION_5_EMPTY					31000
#define VISION_REQ_MEASURE_STATION_6_EMPTY					31001
#define VISION_REQ_MEASURE_STATION_7_EMPTY					31002
#define VISION_REQ_MEASURE_STATION_8_EMPTY					31003


#define VISION_MEASURE_VAT_TEST_2_LD_UPPER					51004
#define VISION_MEASURE_VAT_TEST_2_ULD_UPPER 				51005
#define VISION_MEASURE_VAT_TEST_2_LOWER						51006


//STATION 
//PC 1
#define VISION_RSP_MEASURE_STATION_1_EMPTY					40000
#define VISION_RSP_MEASURE_STATION_2_EMPTY					40001
#define VISION_RSP_MEASURE_STATION_3_EMPTY					40002
#define VISION_RSP_MEASURE_STATION_4_EMPTY					40003
//VAT
#define VISION_RESULT_VAT_LD_UPPER							60004
#define VISION_RESULT_VAT_ULD_UPPER							60005
#define VISION_RESULT_VAT_TEST_1_LD_UPPER					60006
#define VISION_RESULT_VAT_TEST_1_ULD_UPPER					60007
#define VISION_RESULT_VAT_LD_LOWER							60008
#define VISION_RESULT_VAT_ULD_LOWER							60009
#define VISION_RESULT_VAT_TEST_1_LOWER						60010
//PC 2
#define VISION_RSP_MEASURE_STATION_5_EMPTY					41000
#define VISION_RSP_MEASURE_STATION_6_EMPTY					41001
#define VISION_RSP_MEASURE_STATION_7_EMPTY					41002
#define VISION_RSP_MEASURE_STATION_8_EMPTY					41003
//VAT
#define VISION_RESULT_VAT_TEST_2_LD_UPPER					61004
#define VISION_RESULT_VAT_TEST_2_ULD_UPPER					61005
#define VISION_RESULT_VAT_TEST_2_LOWER						61006





// ==============================================================================
// SLT Vision Cam Const
// ==============================================================================
#define	    VISION_CAM_ID_2DBRACODE_1	                    (0)
#define	    VISION_CAM_ID_2DBRACODE_2	                    (1)
#define	    VISION_CAM_ID_2DBRACODE_3	                    (2)
#define	    VISION_CAM_ID_2DBRACODE_4	                    (3)
#define     VISION_CAM_ID_2DBRACODE_(x)                     (VISION_CAM_ID_2DBRACODE_1+x)

#define	    VISION_CAM_ID_TESTSITE_UP_1	                    (4)
#define	    VISION_CAM_ID_TESTSITE_UP_2	                    (5)
#define	    VISION_CAM_ID_TESTSITE_UP_3	                    (6)
#define	    VISION_CAM_ID_TESTSITE_UP_4	                    (7)
#define	    VISION_CAM_ID_TESTSITE_UP_5	                    (8)
#define	    VISION_CAM_ID_TESTSITE_UP_6	                    (9)
#define	    VISION_CAM_ID_TESTSITE_UP_7	                    (10)
#define	    VISION_CAM_ID_TESTSITE_UP_8	                    (11)
#define	    VISION_CAM_ID_TESTSITE_UP_(x)	                (VISION_CAM_ID_TESTSITE_UP_1+x)

#define	    VISION_CAM_ID_TESTSITE_DOWN_1	                (12)
#define	    VISION_CAM_ID_TESTSITE_DOWN_2	                (13)
#define	    VISION_CAM_ID_TESTSITE_DOWN_3	                (14)
#define	    VISION_CAM_ID_TESTSITE_DOWN_4	                (15)
#define	    VISION_CAM_ID_TESTSITE_DOWN_5	                (16)
#define	    VISION_CAM_ID_TESTSITE_DOWN_6	                (17)
#define	    VISION_CAM_ID_TESTSITE_DOWN_7	                (18)
#define	    VISION_CAM_ID_TESTSITE_DOWN_8	                (19)
#define	    VISION_CAM_ID_TESTSITE_DOWN_(x)	                (VISION_CAM_ID_TESTSITE_DOWN_1+x)	

#define     VISION_CAM_ID_TEST_HAND_1						(0)
#define     VISION_CAM_ID_TEST_HAND_2						(1)
#define	    VISION_CAM_ID_TEST_HAND_(x)						(VISION_CAM_ID_TEST_HAND_1+x)	

//==============================================================================
// SLT Vision User Messages
//==============================================================================
#define     UM_VISION_IF_SOCK_EVENT                         (WM_USER + 3000)
#define     UM_VISION_IF_LOG_MSG                            (WM_USER + 3001)

enum _eCamMeasureState{
	eCam_Not_Busy=0, eCam_Busy = 1,
};

enum _eCamMeasureReadyState {
	eCamStateRun_LightON =0, eCamStateRun_LightOFF = 1,
};

enum _eBarcodeLightState {
	eBarcodeLight_ON = 0, eBarcodeLight_OFF,
};

enum _eCamResultState{
	eCamResult_OK=0, eCamResult_NG = 1, eCamResult_Busy=2,
};

enum _eVisionMsgWaitState{
	eVisionMsgWaitState_Waiting = 0, eVisionMsgWaitState_Received = 1, eVisionMsgWaitState_Canceled,
};

enum _eVisionLotState {
	eVisionLotStateError =0, eVisionLotStateStart, eVisionLotStateEnd,
};



//====================================================================================================

enum eInspVAT_CamIndex{		
	//PC 1
	VAT_CAM_INDEX_STATION_1 = 1,
	VAT_CAM_INDEX_STATION_2,
	VAT_CAM_INDEX_STATION_3,
	VAT_CAM_INDEX_STATION_4,
	VAT_CAM_INDEX_TRAY_LOADER_UPPER,
	VAT_CAM_INDEX_TRAY_UNLOADER_UPPER,
	VAT_CAM_INDEX_TEST_1_LD_UPPER,
	VAT_CAM_INDEX_TEST_1_ULD_UPPER,
	VAT_CAM_INDEX_TRAY_LOADER_LOWER,
	VAT_CAM_INDEX_TRAY_UNLOADER_LOWER,
	VAT_CAM_INDEX_TEST_1_LOWER,

	//PC 2
	VAT_CAM_INDEX_STATION_5	= 1,
	VAT_CAM_INDEX_STATION_6,
	VAT_CAM_INDEX_STATION_7,
	VAT_CAM_INDEX_STATION_8,
	VAT_CAM_INDEX_TEST_2_LD_UPPER,
	VAT_CAM_INDEX_TEST_2_ULD_UPPER,
	VAT_CAM_INDEX_TEST_2_LOWER,

// 	INSP_AT_TWSL_TRAY_LOAD_PP  = 3400,
// 	INSP_AT_TWSL_TRAY_UNLOAD_PP = 3500,
// 	INSP_AT_TWSL_TEST_PP1_LOAD = 3600,
// 	INSP_AT_TWSL_TEST_PP1_UNLOAD = 3700,
// 	INSP_AT_TWSL_TEST_PP2_LOAD = 3800,
// 	INSP_AT_TWSL_TEST_PP2_UNLOAD = 3900,

};		

//TWSL301N
// enum _eVAT_AREA 
// { 
// 	VAT_AREA_TRAY_LOADPP = 0, 
// 	VAT_AREA_TRAY_UNLOADPP,
// 	VAT_AREA_TESTPP1_LOAD,
// 	VAT_AREA_TESTPP1_UNLOAD,
// 	VAT_AREA_TESTPP2_LOAD,
// 	VAT_AREA_TESTPP2_UNLOAD,
// 	 
// 	VAT_HAND_MAX,
// 
// 	REQUEST_SET_PACKAGE_DATA = 7000,
// 	VAT_AREA_NONE = -1
// };




enum eCamStatus{
	CAM_STATUS_SUCCESS = 0,
	CAM_STATUS_DISCONNECT	,
	CAM_STATUS_TIMEOUT		,
	CAM_STATUS_FAILED		,
	CAM_STATUS_UNKNOWN		,
	CAM_STATUS_LIGHT_FAILED	,

	CAM_STATUS_IDLE = 99,
};

enum eDataType {
	DataType_Offset,
	DataType_Speed,

	DataSpd_Slow = 0,
	DataSpd_Fast,

	DataFocus_In = 0,
	DataUpDn_Up,
	DataUpDn_Dn 
};


#define MEASUER_VAT_LED_IO_POWER	     (100)
#define MEASUER_VAT_LED_IO_STATUS	     (101)

#define BARCODE_LIGHT_VISION_CHANNEL	(8)

// PCD/BACKLASH
typedef struct _ST_PCD_BACKLASH_RCV_DATA{
	int nHand;
	int nState;
	double dX;
	double dY;
	double dSettingSize;
	double dMesureSize;
	_ST_PCD_BACKLASH_RCV_DATA() {
		memset(this, 0, sizeof(_ST_PCD_BACKLASH_RCV_DATA));
		nState = CAM_STATUS_IDLE;
	}
}ST_PCD_BACKLASH_RCV_DATA;




//==============================================================================
//
//==============================================================================
class _tVisionReqData
{
public:
	_tVisionReqData()
	{
		nDataId = 0;
		ZeroMemory(szDataValue, sizeof(char)*128);
	};
	_tVisionReqData(const _tVisionReqData& src)
	{
		nDataId = src.nDataId;
		ZeroMemory(szDataValue, sizeof(char)*128);
		strcpy_s(szDataValue, sizeof(char)*128, src.szDataValue);
	};
	~_tVisionReqData()
	{

	};
	_tVisionReqData& operator = (const _tVisionReqData& src)
	{
		if( this == &src)
		{
			return *this;
		}
		nDataId = src.nDataId;
		ZeroMemory(szDataValue, sizeof(char)*128);
		strcpy_s(szDataValue, sizeof(char)*128, src.szDataValue);
		return *this;
	}
	int nDataId;
	char szDataValue[128];
};

class _tVisionMsg : public CObject
{
public:
	_tVisionMsg()
	{
		clear();
	};

	_tVisionMsg(const _tVisionMsg& src)
	{
		nMsgId = src.nMsgId;
		nS = src.nS;
		nF = src.nF;
		nRcmd = src.nRcmd;
		nReceived = src.nReceived;
		nRcmdAck = src.nRcmdAck;
		vReqData = src.vReqData;
	}

	~_tVisionMsg()
	{
	}

	_tVisionMsg& operator = (const _tVisionMsg& src)
	{
		if( this == &src )
		{
			return *this;
		}
		nMsgId = src.nMsgId;
		nS = src.nS;
		nF = src.nF;
		nRcmd = src.nRcmd;
		nReceived = src.nReceived;
		nRcmdAck = src.nRcmdAck;
		vReqData = src.vReqData;

		return *this;
	};

	void clear()
	{
		nMsgId = 0;
		nS = 0;
		nF = 0;
		nRcmd = 0;
		nReceived = 0;
		nRcmdAck = 0;
		vReqData.clear();
	};

	// messgae를 보낼 때 셋팅 된다.
	int nMsgId; 
	int nS, nF;
	int nRcmd;

	// message를 받은 후 셋팅 된다.
	int nReceived;   // 0:not received, 1:received, 2:canceled  enum _eVisionMsgWaitState
	int nRcmdAck;
	std::vector<_tVisionReqData> vReqData;
};

typedef struct _ST_VISON_MSG{
	int nMsgId;
	int nReceived;
	int nRcmdAck;
	int nStatus;
	int nDataCount;
	char szData[10][128];
	
	_ST_VISON_MSG(){
		clear();
	}

	void clear(){
		nMsgId = 0;
		nReceived = 0;
		nRcmdAck = 0;
		nStatus = -1;
		nDataCount = 0;
		ZeroMemory(szData, sizeof(szData));
	}
}ST_VISON_MSG;

//==============================================================================
// CLASS : 
//==============================================================================
class CVisionComm :	public CIPCSock
{
public:
	enum eVisionEqpType{
		eEqpType_Momory=0, eEqpType_Logic, eEqpType_Strip, eEqpType_SLT, eEqpType_Module
	};

public:
	CVisionComm(int nVisionPcIdx, CString strVisionName, int nCamCompanyType=e2dVision_TYPE_TW);
	CVisionComm();
	~CVisionComm(void);

	virtual int Cmd_SetCok(_tVisionMsg* pVisionMsg=NULL);
	virtual int Cmd_ReqCleanDisk(_tVisionMsg* pVisionMsg=NULL);
	virtual int Cmd_ReqVisionShutDown(_tVisionMsg* pVisionMsg=NULL);
	virtual int Cmd_ReqInspReady(int nCamNo,_tVisionMsg* pVisionMsg=NULL);
	virtual int Cmd_ReqMeasure(int nCamNo,_tVisionMsg* pVisionMsg=NULL);
	virtual int Cmd_ReqMeasureResult(int nCamNo,_tVisionMsg* pVisionMsg=NULL);
	virtual int Cmd_ReqMeasureExist(int nCamNo,_tVisionMsg* pVisionMsg=NULL);
	virtual int Cmd_VisionUseNoUse( int nUseNoUse, _tVisionMsg* pVisionMsg=NULL );
	//virtual int GetCmdDone(_tVisionMsg* pVisionMsg);
	virtual int GetCmdDone(int nCam);
	virtual int Cmd_LaserOnOff(int nCamNo, BOOL bOnOff ,_tVisionMsg* pVisionMsg=NULL, BOOL bRevChk=TRUE);

	//Test Hand Cam
	virtual int Cmd_ReqMeasure_HandCam(int nCamNo,_tVisionMsg* pVisionMsg, std::vector<CPoint> vPicker, CPoint cPocket, int nPressIdx);
	virtual int Cmd_ReqMeasureExist_HandCam(int nCamNo,_tVisionMsg* pVisionMsg, std::vector<CPoint> vPicker, CPoint cPocket, int nPressIdx, int nDvcType);
	virtual int Cmd_VisionReady_HandCam(BOOL bCamState);

	//Station Cam
	virtual int Cmd_ReqMeasure_StationCam(int nStationNo, _tVisionMsg* pVisionMsg, std::vector<CPoint> vPicker, CPoint cPocket, int nPressIdx/*=StationNo*/);
	virtual int Cmd_VisionReady_StationCam(BOOL bCamState, int nStationNo);

	//2D Barcode LED
	virtual int Cmd_ReqMeasure_BarcodeLED(int nLEDOnOff, int nLEDIdx);

	void ReadConnectConfig(CString& a_strIp, int& a_nPort, int nCamNo=0);
	void WriteConnectConfig(CString a_strIp, int a_nPort, int nCamNo=0);
	CString GetVisionPcName(){ return m_strVisionPcName; };

	int GetCmdResultData(int nStationIdx, int* nRevData=NULL ,CString& strRevPosResult= CString());
public:
	int AttachObserver(CWnd* pObserver);
	int DetachObserver(CWnd* pObserver);

protected:
	void ClearObserver();
	int GetObserverCount();
	CWnd* GetObserverByIndex(int nIndex);
	BOOL CheckDuplicate(CWnd* pItem);

protected:
	CCriticalSection	m_csObserver;
	CCriticalSection	m_csCmdDone;
	CList<CWnd*, CWnd*>	m_listObserver;
	CLogger             m_Logger;
	void SendLogMessage(const char* fmt, ...);


protected:
	int m_nVisionPcIdx;
	CString m_strVisionPcName;
	int m_nCamCompanyType;
	std::vector<_tVisionMsg*> m_VisionCmdResult;
	char m_szDriverFileName[MAX_PATH];

protected:
	
private:
	int Def_CamNoChangeRealNo(int nCamNo);
	int Def_StationNoChangeMessageNo(int nStationNo);
public:
	std::vector<ST_VISON_MSG> m_VisionMsg;
	ST_VISON_MSG m_BarcodeLightMsg;

	//Data ID -> VAT CAM
	int ConvertDataIdToVatCam(int nDataId);
};

