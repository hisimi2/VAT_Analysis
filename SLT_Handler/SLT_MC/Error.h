#pragma once

#include "ParamList.h"
#include "MyList.h"

class CError
{
public:
	CError(void);
	~CError(void);

	int Initialize(CWnd* pMainFrameWnd);
	void Finalize();

	void AlarmReport(int nAlid, CParamList &AlarmData, CBaseTaskManager* pSender,std::vector<CPoint>& vFailPicker = std::vector<CPoint>());//=NULL);
	int RequestUserSelectionModal(int nAlid, int nBtnType);
	void GetErrMsgInfo(int nAlid, _tERROR_MSG& tErrMsg);
	_tERROR_MSG* GetErrMsgPtr(int nAlid);
	void GetDisplayMsg(int nAlid, CStringList &rMsgList);
	BOOL IsSetAlarm(int nAlid);
	BOOL Get_Jam_Alarm_Set_State();
    BOOL IsImpossibleStartAlarm();

	int  GetCurrentLotJamCount();
	void ResetCurrentLotJamCount();
	void MakeLog(LPCTSTR fmt, ...);

	/*===============================================*/
	//COMMENT : Vision Error Discription Code
	/*===============================================*/

	enum eVisionError {
		ERRORCODE_INSPECTION_SQ_WAIT = 10,
		ERRORCODE_MEASURE_TYPE,
		ERRORCODE_INPECTION_SQ_START,
		ERRORCODE_CAMERA_RESET_START,
		ERRORCODE_CAMERA_RESET_WAIT,
		ERRORCODE_PRE_GRAB_SET,
		ERRORCODE_GRAB_FAIL,
		ERRORCODE_GRAB_WAIT,
		ERRORCODE_RECEIVEDATA,
		ERRORCODE_INSPECTION = 100,
		ERRORCODE_INSPECTION_TIMEOUT,
		ERRORCODE_RESULT_SEND,
		ERRORCODE_AF_PRE_START = 200,
		ERRORCODE_AF_WAIT,
		ERRORCODE_AF_RESULT,
		ERRORCODE_AF_FORCED_STOP,


	};
	int CheckVisionError(int nErrorDiscription);
private:
	CWnd* m_pMainFrameWnd;
	//CMyList m_AlarmMsgList;
	std::map< int, std::shared_ptr< _tERROR_MSG > > m_AlarmMsgList;

	// Jam occurd time save [ Áßº¹ jam count¿ë ]
	CMyList m_DuplicateJam;
	void InitDuplicateJamTime();

    CCriticalSection m_csMsg;

	int nCurrentJamCount;

//VAT
public:
	BOOL IsVATAlarmActivate();

};

