#include "StdAfx.h"
#include "DatabaseIF.h"
#include "DatabaseIF_Imp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDatabaseIF::CDatabaseIF(void)
{
	m_pDriver = new CDatabaseIF_Imp;
}


CDatabaseIF::~CDatabaseIF(void)
{
	if (m_pDriver != NULL)
	{
		delete m_pDriver;
		m_pDriver = NULL;
	}
}

int	CDatabaseIF::Create(const char* szDBFileName, int nFileNameLen, int nType)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->Create(szDBFileName, nFileNameLen, nType);
}

void CDatabaseIF::Destroy()
{
	((CDatabaseIF_Imp*)m_pDriver)->Destroy();
}

// 비동기로 실행 할 DB 쿼리
int  CDatabaseIF::EXEC_ASYNC_DB_CMD(CADBC* pCmd)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->EXEC_ASYNC_DB_CMD(pCmd);
}

int CDatabaseIF::SELECT_DIO_TBL(int nDir, CMyList& rDataList)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->SELECT_DIO_TBL(nDir, rDataList);
}

int CDatabaseIF::SELECT_DIO_TBL(int nDir, int nFrmIdx, int nIdxCount, CMyList& rDataList)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->SELECT_DIO_TBL( nDir, nFrmIdx, nIdxCount, rDataList );
}

// GPID_LOG Table
int CDatabaseIF::INSERT_GPIB_LOG_TBL(CString strMsg, int nDate)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->INSERT_GPIB_LOG_TBL(strMsg, nDate);
}

// TEMP_PID Table
int	CDatabaseIF::INSERT_TEMP_PID_TBL(int nCtrlNo, _tTEMP_PID& rTempPid)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->INSERT_TEMP_PID_TBL(nCtrlNo, rTempPid);
}

int	CDatabaseIF::DELETE_TEMP_PID_TBL(int nCtrlNo)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->DELETE_TEMP_PID_TBL(nCtrlNo);
}

int	CDatabaseIF::SELECT_TEMP_PID_TBL(CMyList& rDataList)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->SELECT_TEMP_PID_TBL(rDataList);
}

int	CDatabaseIF::SELECT_TEMP_PID_TBL(int nCtrlNo, _tTEMP_PID& rTempPid)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->SELECT_TEMP_PID_TBL(nCtrlNo, rTempPid);
}

int	CDatabaseIF::UPDATE_TEMP_PID_TBL(CMyList& rDataList)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->UPDATE_TEMP_PID_TBL(rDataList);
}

int	CDatabaseIF::UPDATE_TEMP_PID_TBL(int nCtrlNo, _tTEMP_PID& rTempPid)
{
	_tTEMP_PID temp; // 결과만 받아오기 위해 임시 변수를 넣어줌.
	int nResult = ((CDatabaseIF_Imp*)m_pDriver)->SELECT_TEMP_PID_TBL(nCtrlNo, temp);

	if( 0 == nResult )
	{
		return ((CDatabaseIF_Imp*)m_pDriver)->UPDATE_TEMP_PID_TBL(nCtrlNo, rTempPid);
	}
	else
	{
		return ((CDatabaseIF_Imp*)m_pDriver)->INSERT_TEMP_PID_TBL(nCtrlNo, rTempPid);
	}	
}

// Motor Param Table
int CDatabaseIF::SELECT_MOTORPARAM_TBL(CMyList& rMotorParamList)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->SELECT_MOTORPARAM_TBL( rMotorParamList );
}

int CDatabaseIF::SELECT_MOTORPARAM_TBL(int nAxisNo, _tMOTOR_PARAM& rMotorParam)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->SELECT_MOTORPARAM_TBL( nAxisNo, rMotorParam );
}

int CDatabaseIF::UPDATE_MOTORPARAM_TBL(CMyList& rMotorParamList)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->UPDATE_MOTORPARAM_TBL( rMotorParamList );
}

int	CDatabaseIF::UPDATE_MOTORPARAM_TBL(int nPhyNo, double dScaleFactor)

{
	return ((CDatabaseIF_Imp*)m_pDriver)->UPDATE_MOTORPARAM_TBL(nPhyNo, dScaleFactor);
}

int CDatabaseIF::UPDATE_MOTORPARAM_TBL(int nAxisNo, _tMOTOR_PARAM& rMotorParam )
{
	return ((CDatabaseIF_Imp*)m_pDriver)->UPDATE_MOTORPARAM_TBL( nAxisNo, rMotorParam );
}

// TBL_ERROR_LOG
int CDatabaseIF::DELETE_ERROR_LOG_TBL(CString strDate_S, CString strDate_E)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->DELETE_ERROR_LOG_TBL(strDate_S, strDate_E);
}

int CDatabaseIF::SELECT_ERROR_LOG_TBL(CString strDate_S, CString strDate_E, CMyList& rLogList)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->SELECT_ERROR_LOG_TBL(strDate_S, strDate_E, rLogList);
}

int CDatabaseIF::SELECT_ERROR_LOG_TBL( CMyList& rLogList )
{
	return ((CDatabaseIF_Imp*)m_pDriver)->SELECT_ERROR_LOG_TBL( rLogList );
}

int CDatabaseIF::SELECT_ERROR_LOG_TBL(CString strDate_S, CString strDate_E, std::vector<ST_ERROR_MESSAGE>& vLogVector)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->SELECT_ERROR_LOG_TBL(strDate_S, strDate_E, vLogVector);
}

int CDatabaseIF::SELECT_ERROR_LOG_TBL_ERRORCODE(CString strDate_S, CString strDate_E, CString strErrorCode, CMyList& rLogList)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->SELECT_ERROR_LOG_TBL_ERRORCODE(strDate_S, strDate_E, strErrorCode, rLogList);
}

int CDatabaseIF::SELECT_ERROR_LOG_TBL_ERRORCODE(CString strErrorCode, CMyList& rLogList)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->SELECT_ERROR_LOG_TBL_ERRORCODE(strErrorCode, rLogList);
}

int CDatabaseIF::SELECT_ERROR_LOG_TBL_PART     (CString strDate_S, CString strDate_E, CString strPart, CMyList& rLogList)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->SELECT_ERROR_LOG_TBL_PART(strDate_S, strDate_E, strPart, rLogList);
}

int CDatabaseIF::SELECT_ERROR_LOG_TBL_PART     (CString strPart, CMyList& rLogList)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->SELECT_ERROR_LOG_TBL_PART(strPart, rLogList);
}

int CDatabaseIF::SELECT_ERROR_LOG_TBL_LEVEL    (CString strDate_S, CString strDate_E, int strLevel, CMyList& rLogList)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->SELECT_ERROR_LOG_TBL_LEVEL(strDate_S, strDate_E, strLevel, rLogList);
}

int CDatabaseIF::SELECT_ERROR_LOG_TBL_LEVEL    (int strLevel, CMyList& rLogList)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->SELECT_ERROR_LOG_TBL_LEVEL(strLevel, rLogList);
}

int CDatabaseIF::SELECT_ERROR_LOG_TBL_LEVEL(CString strDate_S, CString strDate_E, int strLevel, std::vector<ST_ERROR_MESSAGE>& vLogVector, BOOL nDaliyReport, BOOL bChk_OnLine_OffLine)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->SELECT_ERROR_LOG_TBL_LEVEL(strDate_S, strDate_E, strLevel, vLogVector, nDaliyReport, bChk_OnLine_OffLine);
}

int CDatabaseIF::INSERT_ERROR_LOG_TBL(_tERROR_LOG& tErrLog)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->INSERT_ERROR_LOG_TBL(tErrLog);
}

int CDatabaseIF::INSERT_LOTHISTORY_TBL(_tLOTHISTORY_LOG& tLotHistory)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->INSERT_LOTHISTORY_TBL(tLotHistory);
}

int CDatabaseIF::SELECT_LOTHISTORY_TBL(CString strDate_S, CString strDate_E, std::vector<ST_LOTHISTORY>& vLogVector)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->SELECT_LOTHISTORY_TBL(strDate_S, strDate_E, vLogVector);
}



int CDatabaseIF::SELECT_ERROR_MSG_TBL( std::map<int, std::shared_ptr<_tERROR_MSG>>& rErrMsgList )
{
	return ( ( CDatabaseIF_Imp* )m_pDriver )->SELECT_ERROR_MSG_TBL( rErrMsgList );
}

// TBL_ERROR_MSG
int CDatabaseIF::SELECT_ERROR_MSG_TBL(CMyList& rErrMsgList)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->SELECT_ERROR_MSG_TBL(rErrMsgList);
}

int CDatabaseIF::SELECT_ERROR_MSG_TBL(CString strEqpALID, _tERROR_MSG& rErrMsg)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->SELECT_ERROR_MSG_TBL(strEqpALID, rErrMsg);
}

int CDatabaseIF::SELECT_ERROR_MSG_TBL(int nALID, _tERROR_MSG& rErrMsg)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->SELECT_ERROR_MSG_TBL(nALID, rErrMsg);
}


int CDatabaseIF::INSERT_ERROR_MSG_TBL(CString strEqpALID, _tERROR_MSG& rErrMsg)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->INSERT_ERROR_MSG_TBL(strEqpALID, rErrMsg);
}

int CDatabaseIF::UPDATE_ERROR_MSG_TBL(CString strEqpALID, _tERROR_MSG& rErrMsg)
{
	_tERROR_MSG temp; // 결과만 받아오기 위해 임시 변수를 넣어줌.
	int nRet = ((CDatabaseIF_Imp*)m_pDriver)->SELECT_ERROR_MSG_TBL(strEqpALID, temp);

	if( nRet == 0 )
	{
		return ((CDatabaseIF_Imp*)m_pDriver)->UPDATE_ERROR_MSG_TBL(strEqpALID, rErrMsg);
	}
	else
	{
		return ((CDatabaseIF_Imp*)m_pDriver)->INSERT_ERROR_MSG_TBL(strEqpALID, rErrMsg);
	}	
}

int CDatabaseIF::UPDATE_ERROR_MSG_TBL(int nALID, _tERROR_MSG& rErrMsg)
{
	_tERROR_MSG temp; // 결과만 받아오기 위해 임시 변수를 넣어줌.
	int nRet = ((CDatabaseIF_Imp*)m_pDriver)->SELECT_ERROR_MSG_TBL(nALID, temp);

	if( nRet == 0 )
	{
		return ((CDatabaseIF_Imp*)m_pDriver)->UPDATE_ERROR_MSG_TBL(nALID, rErrMsg);
	}
	else
	{
		return ((CDatabaseIF_Imp*)m_pDriver)->INSERT_ERROR_MSG_TBL(nALID, rErrMsg);
	}	
}

int CDatabaseIF::UPDATE_ADD_COLUMN_TBL(CString strAddColumnName, CString strAddColumnType, CString strSelTable)
{	
	return ((CDatabaseIF_Imp*)m_pDriver)->INSERT_ADD_COLUMN(strAddColumnName, strAddColumnType, strSelTable);
}

// TBL_TEACH_LOG
int	CDatabaseIF::INSERT_TEACH_LOG_TBL(_tTEACH_LOG& tTeachLog)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->INSERT_TEACH_LOG_TBL(tTeachLog);
}

int CDatabaseIF::SELECT_TEACH_LOG_TBL(CString strDate_S, CString strDate_E, std::vector<ST_TEACH_LOG>& vLogVector)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->SELECT_TEACH_LOG_TBL(strDate_S, strDate_E, vLogVector);
}

// TBL_MOTOR_TEACHING
int CDatabaseIF::SELECT_MOTOR_TEACHING_TBL( char* szCokName, CMyList& rTeachingList )
{
	return ((CDatabaseIF_Imp*)m_pDriver)->SELECT_MOTOR_TEACHING_TBL( szCokName, rTeachingList );
}

int CDatabaseIF::SELECT_MOTOR_TEACHING_TBL( char* szCokName, int nAxisNo, CMyList& rTeachingList )
{
	return ((CDatabaseIF_Imp*)m_pDriver)->SELECT_MOTOR_TEACHING_TBL( szCokName, nAxisNo, rTeachingList );
}

int CDatabaseIF::INSERT_MOTOR_TEACHING_TBL( char* szCokName, CMyList& rTeaching )
{
	return ((CDatabaseIF_Imp*)m_pDriver)->INSERT_MOTOR_TEACHING_TBL( szCokName, rTeaching );
}

int CDatabaseIF::UPDATE_MOTOR_TEACHING_TBL( char* szCokName, int nAxisNo, int nPos, _tMOTOR_TEACHING& rValue )
{
 	_tMOTOR_TEACHING* pData = new _tMOTOR_TEACHING;
 	::CopyMemory( pData, &rValue, sizeof( rValue ) );
 	CMyList list;
 	((CDatabaseIF_Imp*)m_pDriver)->SELECT_MOTOR_TEACHING_TBL( szCokName, nAxisNo, list );

  	if( list.m_fnGetCount() > 0 )
  	{
 		return ((CDatabaseIF_Imp*)m_pDriver)->UPDATE_MOTOR_TEACHING_TBL( szCokName, nAxisNo, nPos, rValue );
  	}
  	else
  	{
 		list.m_fnClear();
 		list.m_fnAddData( pData );
 		return ((CDatabaseIF_Imp*)m_pDriver)->INSERT_MOTOR_TEACHING_TBL( szCokName, list );
 	}	
}

int CDatabaseIF::DELETE_MOTOR_TEACHING_TBL( char* szCokName )
{
	return ((CDatabaseIF_Imp*)m_pDriver)->DELETE_MOTOR_TEACHING_TBL( szCokName );
}


int	CDatabaseIF::SELECT_MOTION_GROUP_SETTING_TBL( int nGroupNo, CMyList& rSetting )
{
	return ((CDatabaseIF_Imp*)m_pDriver)->SELECT_MOTION_GROUP_SETTING_TBL( nGroupNo, rSetting );
}

//////////////////////////////////////////////////////////// TAG_TBL
int CDatabaseIF::SELECT_TAG_TBL(int nTagId, _tTAG& rValue)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->SELECT_TAG_TBL(nTagId, rValue);
}

int CDatabaseIF::SELECT_TAG_TBL(int nGroupId, CMyList& rTagList)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->SELECT_TAG_TBL(nGroupId, rTagList);
}
int CDatabaseIF::SELECT_TAG_TBL(CMyList& rTagList)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->SELECT_TAG_TBL(rTagList);
}
int CDatabaseIF::INSERT_TAG_TBL(int nTagId, _tTAG& rValue)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->INSERT_TAG_TBL(nTagId, rValue);
}
int CDatabaseIF::UPDATE_TAG_TBL(int nTagId, _tTAG& rValue)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->UPDATE_TAG_TBL(nTagId, rValue);
}



//////////////////////////////////////////////////////////// TAG_GROUP_TBL
int CDatabaseIF::SELECT_TAG_GROUP(int nGroupId, _tTAG_GROUP& rValue)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->SELECT_TAG_GROUP(nGroupId, rValue);
}

int CDatabaseIF::SELECT_TAG_GROUP(CMyList& rTagGroupList)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->SELECT_TAG_GROUP(rTagGroupList);
}

int CDatabaseIF::INSERT_TAG_GROUP(int nGroupId, _tTAG_GROUP& rValue)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->INSERT_TAG_GROUP(nGroupId, rValue);
}

int CDatabaseIF::UPDATE_TAG_GROUP(int nGroupId, _tTAG_GROUP& rValue)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->UPDATE_TAG_GROUP(nGroupId, rValue);
}

int CDatabaseIF::SELECT_AIO_TBL(int nDir, CMyList& rDataList)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->SELECT_AIO_TBL(nDir, rDataList);
}

int CDatabaseIF::SELECT_AIO_TBL(int nDir, int nIndex, _tAIO_LS& rValue)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->SELECT_AIO_TBL(nDir, nIndex, rValue);
}

int CDatabaseIF::SELECT_AIO_TBL(int nDir, int nFrmIdx, int nIdxCount, CMyList& rDataList)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->SELECT_AIO_TBL(nDir, nFrmIdx, nIdxCount, rDataList);
}

int CDatabaseIF::UPDATE_AIO_TBL(int nDir, int nIndex, _tAIO_LS& rValue)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->UPDATE_AIO_TBL(nDir, nIndex, rValue);
}

int CDatabaseIF::INSERT_AIO_TBL(int nDir, int nIndex, _tAIO_LS& rValue)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->INSERT_AIO_TBL(nDir, nIndex, rValue);
}


CString CDatabaseIF::decodeUTF8(CString &encoded)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->decodeUTF8(encoded);
}

CString CDatabaseIF::encodeUTF8(CString &dcoded)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->encodeUTF8(dcoded);
}

//////////////////////////////////////////////////////////// YIELD_RETEST_MANAGEMENT

int CDatabaseIF::INSERT_YIELD_RETEST_MANAGEMENT(_tYIELD_RETEST_MANAGEMENT& rValue)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->INSERT_YIELD_RETEST_MANAGEMENT(rValue);
}
int CDatabaseIF::UPDATE_YIELD_RETEST_MANAGEMENT(int nNumber, _tYIELD_RETEST_MANAGEMENT& rValue)
{
	_tYIELD_RETEST_MANAGEMENT temp; // 결과만 받아오기 위해 임시 변수를 넣어줌.
	int nResult = ((CDatabaseIF_Imp*)m_pDriver)->SELECT_YIELD_RETEST_MANAGEMENT(nNumber, temp);

	if( 0 == nResult )
	{
		return ((CDatabaseIF_Imp*)m_pDriver)->UPDATE_YIELD_RETEST_MANAGEMENT(nNumber, rValue);
	}
	else
	{
		return ((CDatabaseIF_Imp*)m_pDriver)->INSERT_YIELD_RETEST_MANAGEMENT(rValue);
	}	
}
int	CDatabaseIF::DELETE_YIELD_RETEST_MANAGEMENT(int nNumber)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->DELETE_YIELD_RETEST_MANAGEMENT(nNumber);
}
int	CDatabaseIF::CREATE_YIELD_RETEST_MANAGEMENT()
{
	return ((CDatabaseIF_Imp*)m_pDriver)->CREATE_YIELD_RETEST_MANAGEMENT();
}

int CDatabaseIF::SELECT_BARCODE_2D_ALL(std::vector<ST_2D_BARCODE_INFO> &vec)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->SELECT_BARCODE_2D_ALL(vec);
}


int CDatabaseIF::INSERT_BARCODE_2D(ST_2D_BARCODE_INFO& data)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->INSERT_BARCODE_2D(data);
}

int CDatabaseIF::DELETE_BARCODE_2D(const char* sz2DID, const char* szReadTime)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->DELETE_BARCODE_2D(sz2DID,szReadTime);
}

void CDatabaseIF::DELETE_BARCODE_2D_ALL()
{
	((CDatabaseIF_Imp*)m_pDriver)->DELETE_BARCODE_2D_ALL();
}

// Auto GRR Table
int CDatabaseIF::INSERT_GRRDATA_TBL(const _tGRR_DATA& rGrrData)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->INSERT_GRRDATA_TBL(rGrrData);
}

int CDatabaseIF::ASYNC_INSERT_GRRDATA_TBL(const _tGRR_DATA& rGrrData)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->ASYNC_INSERT_GRRDATA_TBL(rGrrData);
}

int CDatabaseIF::DELETE_ALL_GRRDATA_TBL()
{
	return ((CDatabaseIF_Imp*)m_pDriver)->DELETE_ALL_GRRDATA_TBL();
}

int CDatabaseIF::CREATE_GRRDATA_TBL()
{
	return ((CDatabaseIF_Imp*)m_pDriver)->CREATE_GRRDATA_TBL();
}

int CDatabaseIF::SELECT_GRRDATA_TBL(CMyList& rGrrDataList)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->SELECT_GRRDATA_TBL(rGrrDataList);
}

int CDatabaseIF::SELECT_COUNT_GRRDATA_TBL()
{
	return ((CDatabaseIF_Imp*)m_pDriver)->SELECT_COUNT_GRRDATA_TBL();
}


int CDatabaseIF::INSERT_DEVICE_DATA_TBL(int nTableNum, char *szBarcode, char *szSiteNo, char *szErrorStr/*, char *szErrorCode*/, int nTestingCnt, char *szResultBin, CString strRemark)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->INSERT_DEVICE_DATA_TBL(nTableNum, szBarcode, szSiteNo, szErrorStr/*, szErrorCode*/, nTestingCnt, szResultBin,strRemark);
}

int CDatabaseIF::SELECT_DEVICE_DATA_TBL(int nTableNum, std::deque<ST_AAB_CHECK_DATA> &qDvcData)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->SELECT_DEVICE_DATA_TBL(nTableNum, qDvcData);
}

int CDatabaseIF::DELETE_DEVICE_DATA_TBL(int nTableNum, char *szBarcode)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->DELETE_DEVICE_DATA_TBL(nTableNum, szBarcode);
}

int CDatabaseIF::DELETE_DEVICE_DATA_TBL_ALL()
{
	return ((CDatabaseIF_Imp*)m_pDriver)->DELETE_DEVICE_DATA_TBL_ALL();
}

int CDatabaseIF::DELETE_LOG_TBL_ALL()
{
	return ((CDatabaseIF_Imp*)m_pDriver)->DELETE_LOG_TBL_ALL();
}

int CDatabaseIF::SELECT_DAILY_DATA_TBL(CString strStartDate, CString strEndDate, ST_DAILY_REPORT* stDailyData)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->SELECT_DAILY_DATA_TBL(strStartDate, strEndDate, stDailyData);
}
int CDatabaseIF::UPDATE_DAILY_DATA_TBL(CString strDate, ST_DAILY_REPORT stAddData)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->UPDATE_DAILY_DATA_TBL(strDate, stAddData);
}
int CDatabaseIF::CREATE_DAILY_DATA_TBL()
{
	return ((CDatabaseIF_Imp*)m_pDriver)->CREATE_DAILY_DATA_TBL();
}

int CDatabaseIF::CREATE_CYLINDER_TIME_TBL()
{
	return ((CDatabaseIF_Imp*)m_pDriver)->CREATE_CYLINDER_TIME_TBL();
}

int CDatabaseIF::CHECK_CYLINDER_TIME_TBL()
{
	return ((CDatabaseIF_Imp*)m_pDriver)->CHECK_CYLINDER_TIME_TBL();
}

int CDatabaseIF::INSERT_CYLINDER_TIME_TBL(_tCYLINDER_TIME& rCylinderData)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->INSERT_CYLINDER_TIME_TBL(rCylinderData);
}

int CDatabaseIF::UPDATE_CYLINDER_TIME_TBL(int nIdx, double dCylinderValue)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->UPDATE_CYLINDER_TIME_TBL(nIdx, dCylinderValue);
}

int CDatabaseIF::SELECT_CYLINDER_TIME_TBL(CMyList& rDataList)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->SELECT_CYLINDER_TIME_TBL(rDataList);
}

int CDatabaseIF::SELECT_VAT_BASE_INFO(int nLocIdx, double& nPosX, double& nPosY)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->SELECT_VAT_BASE_INFO(nLocIdx, nPosX, nPosY);
}

int CDatabaseIF::CHECK_VAT_TBL()
{
	return ((CDatabaseIF_Imp*)m_pDriver)->CHECK_VAT_TBL();
}

int CDatabaseIF::SELECT_VAT_Z_AUTO_FOCUS(double (*rVATGuiDataList)[3], int nType)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->SELECT_VAT_Z_AUTO_FOCUS(rVATGuiDataList, nType);
}

int CDatabaseIF::SELECT_VAT_CAM_CENTER_CAL(double(*rVATGuiDataList)[3])
{
	return ((CDatabaseIF_Imp*)m_pDriver)->SELECT_VAT_CAM_CENTER_CAL(rVATGuiDataList);
}

int CDatabaseIF::SELECT_VAT_VISION_AUTO_TEACHING(double(*rVATGuiDataList)[3], int nHand)
{
	return ((CDatabaseIF_Imp*)m_pDriver)->SELECT_VAT_VISION_AUTO_TEACHING(rVATGuiDataList, nHand);
}
