#pragma once


//==============================================================================
//
//==============================================================================
#include "DEF_DB_TBL.h"
#include "ThreadBase.h"
#include "Logger.h"
#include "MyList.h"
#include "AsyncDBCommand.h" 
#include <afxmt.h>
#include "CppSQLite3.h"
#include <vector>
#include <deque>
#include <map>
#include <memory>

//==============================================================================
//
//==============================================================================
class CDatabaseIF_Imp : public CThreadBase, public CObject
{
public:
	CDatabaseIF_Imp(void);
	~CDatabaseIF_Imp(void);
	// Driver 시작과 종료
	int  Create(const char* szIniFileName, int nFileNameLen, int nType);
	void Destroy();

	// DB Connection
	int  ConnectDatabase(const char* szDBPath);
	void CloseDatabase();

private:
	// other...
	int	CHECK_TABLE( CString strTableName );

public:
	// DIO_LS Table
	int SELECT_DIO_TBL(int nDir, CMyList& rDataList);
	int SELECT_DIO_TBL(int nDir, int nFrmIdx, int nIdxCount, CMyList& rDataList);

	// TBL_AIO Table
	int SELECT_AIO_TBL(int nDir, int nIndex, _tAIO_LS& rValue);
	int SELECT_AIO_TBL(int nDir, CMyList& rDataList);
	int SELECT_AIO_TBL(int nDir, int nFrmIdx, int nIdxCount, CMyList& rDataList);
	int UPDATE_AIO_TBL(int nDir, int nIndex, _tAIO_LS& rValue);
	int INSERT_AIO_TBL(int nDir, int nIndex, _tAIO_LS& rValue);

	// GPIB_LOG Table
	int INSERT_GPIB_LOG_TBL(CString strMsg, int nDate);
	
	// TEMP_PID Table
	int	INSERT_TEMP_PID_TBL(int nCtrlNo, _tTEMP_PID& rTempPid);
	int	DELETE_TEMP_PID_TBL(int nCtrlNo);
	int	SELECT_TEMP_PID_TBL(CMyList& rDataList);
	int	SELECT_TEMP_PID_TBL(int nCtrlNo, _tTEMP_PID& rTempPid);
	int	UPDATE_TEMP_PID_TBL(CMyList& rDataList);
	int	UPDATE_TEMP_PID_TBL(int nCtrlNo, _tTEMP_PID& rTempPid);

	// ROBOT Table
	int Select_RobotTbl(int nAxisNo, _tRobot& rData);
	int Update_RobotTbl(int nAxisNo, _tRobot& rData);

	// Motor Param Table
	int SELECT_MOTORPARAM_TBL(CMyList& rMotorParamList);
	int SELECT_MOTORPARAM_TBL(int nAxisNo, _tMOTOR_PARAM& rMotorParam);
	int UPDATE_MOTORPARAM_TBL(CMyList& rMotorParamList);
	int UPDATE_MOTORPARAM_TBL(int nAxisNo, _tMOTOR_PARAM& rMotorParam);
	//Scale Factor Update
	int UPDATE_MOTORPARAM_TBL(int nPhyNo, double dScaleFactor);


	// TBL_ERROR_LOG
	int	DELETE_ERROR_LOG_TBL(CString strDate_S, CString strDate_E); // YYYY-MM-DD HH:MM:SS.SSS
	int	SELECT_ERROR_LOG_TBL(CString strDate_S, CString strDate_E, CMyList& rLogList);
	int SELECT_ERROR_LOG_TBL(CMyList& rLogList);
	int SELECT_ERROR_LOG_TBL(CString strDate_S, CString strDate_E, std::vector<ST_ERROR_MESSAGE>& vLogVector);
	int	SELECT_ERROR_LOG_TBL_ERRORCODE(CString strDate_S, CString strDate_E, CString strErrorCode, CMyList& rLogList);
	int	SELECT_ERROR_LOG_TBL_ERRORCODE(CString strErrorCode, CMyList& rLogList);
	int	SELECT_ERROR_LOG_TBL_PART     (CString strDate_S, CString strDate_E, CString strPart, CMyList& rLogList);
	int	SELECT_ERROR_LOG_TBL_PART     (CString strPart, CMyList& rLogList);
	int	SELECT_ERROR_LOG_TBL_LEVEL    (CString strDate_S, CString strDate_E, int nLevel, CMyList& rLogList,BOOL bDaliyReport = FALSE, BOOL bChk_OnLine_OffLine = FALSE);
	int	SELECT_ERROR_LOG_TBL_LEVEL    (int nLevel, CMyList& rLogList);
	int	SELECT_ERROR_LOG_TBL_LEVEL(CString strDate_S, CString strDate_E, int nLevel, std::vector<ST_ERROR_MESSAGE>& vLogVector, BOOL bDaliyReport =FALSE, BOOL bChk_OnLine_OffLine = FALSE);
	int	INSERT_ERROR_LOG_TBL(_tERROR_LOG& tErrLog);
	int INSERT_ERROR_LOG_TBL(CString strErrCode, CString a_strMessage, SYSTEMTIME a_tOccurredTime, CString a_strUser);
	int DELETE_LOG_TBL_ALL();

// 	//TBL_LOTHISTORY
	int INSERT_LOTHISTORY_TBL(_tLOTHISTORY_LOG& tLotHistory);
	int SELECT_LOTHISTORY_TBL(CString strDate_S, CString strDate_E, std::vector<ST_LOTHISTORY>& vLogVector);
	
	// TBL_TEACH_LOG
	int CREATE_TEACH_LOG_TBL(CString strDateYYYYMM);
	int INSERT_TEACH_LOG_TBL(_tTEACH_LOG& tTeachLog);
	CString MAKE_SELECT_TEACH_LOG_QUERY(CString strDate_S, CString strDate_E, CString strConditionType ="", CString strCondition ="");
	int EXEC_SELECT_TEACH_LOG_QUERY(CString strDate_S, CString strDate_E, std::vector<ST_TEACH_LOG>& vLogTeachVector, CString strQuery);
	int SELECT_TEACH_LOG_TBL(CString strDate_S, CString strDate_E, std::vector<ST_TEACH_LOG>& vLogTeachVector);
	// Auto GRR Table
	int INSERT_GRRDATA_TBL(const _tGRR_DATA& rGrrData);
	int ASYNC_INSERT_GRRDATA_TBL(const _tGRR_DATA& rGrrData);
	int DELETE_ALL_GRRDATA_TBL();
	int SELECT_GRRDATA_TBL(CMyList& rGrrDataList);
	int SELECT_COUNT_GRRDATA_TBL();
	int	CREATE_GRRDATA_TBL();

	//TBL_VAT_BASE_INFO
	int SELECT_VAT_BASE_INFO(int nLocIdx, double& nPosX, double& nPosY);

private:
	int	CREATE_ERROR_LOG_TBL( CString strDateYYYYMM );
	int CREATE_LOTHISTORY_TBL();
	int EXEC_SELECT_ERROR_LOG_QUERY( CString strDate_S, CString strDate_E, CMyList& rLogList, CString strQuery,BOOL bDaliyReport = FALSE, BOOL bChk_OnLine_OffLine = FALSE);
	int EXEC_SELECT_ERROR_LOG_QUERY(CString strDate_S, CString strDate_E, std::vector<ST_ERROR_MESSAGE>& vLogVector, CString strQuery, BOOL bDaliyReport = FALSE, BOOL bChk_OnLine_OffLine = FALSE);
	CString MAKE_SELECT_ERROR_LOG_QUERY( CString strDate_S, CString strDate_E, CString strConditionType = "", CString strCondition = "" );
 	CString MAKE_SELECT_LOTHISTORY_QUERY(CString strDate_S, CString strDate_E);
 	int EXEC_SELECT_LOTHISTORY_QUERY(CString strDate_S, CString strDate_E, std::vector<ST_LOTHISTORY>& vLogVector, CString strQuery);

public:
	// TBL_ERROR_MSG
	int SELECT_ERROR_MSG_TBL( std::map< int, std::shared_ptr< _tERROR_MSG > >& rErrMsgList );
	int SELECT_ERROR_MSG_TBL(CMyList& rErrMsgList);
	int SELECT_ERROR_MSG_TBL(CString strEqpALID, _tERROR_MSG& rErrMsg);
	int SELECT_ERROR_MSG_TBL(int nALID, _tERROR_MSG& rErrMsg);
	int INSERT_ERROR_MSG_TBL(CString strEqpALID, _tERROR_MSG& rErrMsg);
	int INSERT_ERROR_MSG_TBL(int nALID, _tERROR_MSG& rErrMsg);
	int UPDATE_ERROR_MSG_TBL(CString strEqpALID, _tERROR_MSG& rErrMsg);
	int UPDATE_ERROR_MSG_TBL(int nALID, _tERROR_MSG& rErrMsg);

	// TBL_MOTOR_TEACHING
	int SELECT_MOTOR_TEACHING_TBL( char* szCokName, CMyList& rTeachingList );
	int SELECT_MOTOR_TEACHING_TBL( char* szCokName, int nAxisNo, CMyList& rTeachingList );
	int INSERT_MOTOR_TEACHING_TBL( char* szCokName, CMyList& rTeaching );
	int UPDATE_MOTOR_TEACHING_TBL( char* szCokName, int nAxisNo, int nPos, _tMOTOR_TEACHING& rValue );
	int DELETE_MOTOR_TEACHING_TBL( char* szCokName );

	// TBL_MOTION_GROUP_SETTING
	int	SELECT_MOTION_GROUP_SETTING_TBL( int nGroupNo, CMyList& rSetting );

	// TBL_TAG
	int     SELECT_TAG_TBL(int nTagId, _tTAG& rValue);
	int     SELECT_TAG_TBL(int nGroupId, CMyList& rTagList);
	int     SELECT_TAG_TBL(CMyList& rTagList);
	int     INSERT_TAG_TBL(int nTagId, _tTAG& rValue);
	int     UPDATE_TAG_TBL(int nTagId, _tTAG& rValue);

	// TBL_TAG_GROUP
	int     SELECT_TAG_GROUP(int nGroupId, _tTAG_GROUP& rValue);
	int     SELECT_TAG_GROUP(CMyList& rTagGroupList);
	int     INSERT_TAG_GROUP(int nGroupId, _tTAG_GROUP& rValue);
	int     UPDATE_TAG_GROUP(int nGroupId, _tTAG_GROUP& rValue);
	
	// TBL_YIELD_RETEST_MANAGEMENT
	int     SELECT_YIELD_RETEST_MANAGEMENT(int nNumber, _tYIELD_RETEST_MANAGEMENT& rValue);
	int     INSERT_YIELD_RETEST_MANAGEMENT(_tYIELD_RETEST_MANAGEMENT& rValue);
	int     UPDATE_YIELD_RETEST_MANAGEMENT(int nNumber, _tYIELD_RETEST_MANAGEMENT& rValue);
	int		CREATE_YIELD_RETEST_MANAGEMENT();
	int		DELETE_YIELD_RETEST_MANAGEMENT(int nNumber);

	int		SELECT_BARCODE_2D_ALL(std::vector<ST_2D_BARCODE_INFO> &vec);
	int     INSERT_BARCODE_2D(ST_2D_BARCODE_INFO& data);
	int		DELETE_BARCODE_2D(const char* sz2DID, const char* szReadTime);
	void	DELETE_BARCODE_2D_ALL();

	//TBL_DEVICE_DATA   // add by SJI [160919] Add AAB Retest Rule Check
	int    CREATE_DEVICE_DATA_TBL(CString strTableName);
	int    INSERT_DEVICE_DATA_TBL(int nTableNum, char *szBarcode, char *szSiteNo, char *szErrorStr/*, char *szErrorCode*/, int nTestingCnt, char *szResultBin, CString strRemark ="");
	int    SELECT_DEVICE_DATA_TBL(int nTableNum/*enum eAAB_DB_TYPE */, std::deque<ST_AAB_CHECK_DATA> &qDvcData);
	int    DELETE_DEVICE_DATA_TBL(int nTableNum, char *szBarcode);
	int    DELETE_DEVICE_DATA_TBL_ALL();

	// Daily Report
	int SELECT_DAILY_DATA_TBL(CString strStartDate, CString strEndDate, ST_DAILY_REPORT* stDailyData);
	int UPDATE_DAILY_DATA_TBL(CString strDate, ST_DAILY_REPORT stAddData);
	int CREATE_DAILY_DATA_TBL();
	

	// Cylinder Time
	int CREATE_CYLINDER_TIME_TBL();
	int INSERT_CYLINDER_TIME_TBL(_tCYLINDER_TIME& rCylinderData);
	int CHECK_CYLINDER_TIME_TBL();
	int UPDATE_CYLINDER_TIME_TBL(int nInx, double dCylinderValue);
	int SELECT_CYLINDER_TIME_TBL(CMyList& rDataList);

	// 비동기로 실행 할 DB 쿼리
	int  EXEC_ASYNC_DB_CMD(CADBC* pCmd);
	void CLEAR_DB_QUEUE();
	int  GetCountAsyncDbCmd();

	void ThreadProc1();

	CString decodeUTF8(CString &encoded);
	CString encodeUTF8(CString &dcoded);

	int INSERT_ADD_COLUMN(CString strAddColumn, CString strAddColumnType, CString strSelTable = "ALL");

	//VAT POS DATA
	int CHECK_VAT_TBL();
	int SELECT_VAT_Z_AUTO_FOCUS(double(*rVATGuiDataList)[3], int nType);
	int SELECT_VAT_CAM_CENTER_CAL(double(*rVATGuiDataList)[3]);
	int SELECT_VAT_VISION_AUTO_TEACHING(double(*rVATGuiDataList)[3], int nHand);

private:
	char					m_szIniFileName[512];
	char					m_szDatabaseFileName[512];
	BOOL					m_bOpened;
	CList<CADBC*, CADBC*>	m_listAsyncDBCmd;
	CCriticalSection		m_csCmdSync;
	CppSQLite3DB			m_db;
	CLogger					m_Logger;
	char					m_szMachineSetupDate[ 64 ];

	CCriticalSection		m_csQuary;
};

extern CDatabaseIF_Imp* g_DbIf;