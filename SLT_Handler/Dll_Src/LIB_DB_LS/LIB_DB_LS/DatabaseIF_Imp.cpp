#include "StdAfx.h"
#include "DatabaseIF_Imp.h"
#include "../../../Include/ConfigData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDatabaseIF_Imp* g_DbIf = NULL;

CDatabaseIF_Imp::CDatabaseIF_Imp(void)
{
	ZeroMemory( m_szIniFileName,		sizeof( m_szIniFileName ) );
	ZeroMemory( m_szDatabaseFileName,	sizeof( m_szDatabaseFileName ) );
	ZeroMemory( m_szMachineSetupDate,	sizeof( m_szMachineSetupDate ) );
	m_bOpened = FALSE;
}

CDatabaseIF_Imp::~CDatabaseIF_Imp(void)
{
	Destroy();
}


//------------------------------------------------------------------------------------------
int  CDatabaseIF_Imp::EXEC_ASYNC_DB_CMD(CADBC* pCmd)
{
	m_csCmdSync.Lock();
	m_listAsyncDBCmd.AddTail(pCmd);
	m_csCmdSync.Unlock();
	return 0;
}

void CDatabaseIF_Imp::CLEAR_DB_QUEUE()
{
	m_csCmdSync.Lock();

	POSITION pos = m_listAsyncDBCmd.GetHeadPosition();

	while(pos){
		CADBC* pItem = (CADBC*)m_listAsyncDBCmd.GetNext(pos);
		delete pItem;
		pItem = NULL;
	}
	m_listAsyncDBCmd.RemoveAll();

	m_csCmdSync.Unlock();
}

int  CDatabaseIF_Imp::GetCountAsyncDbCmd()
{
	m_csCmdSync.Lock();
	int nCount = m_listAsyncDBCmd.GetCount();
	m_csCmdSync.Unlock();
	return nCount;
}


void CDatabaseIF_Imp::ThreadProc1()
{
	m_Logger.LOG("START - Async Command Execute Thread.");
	SetThreadInterval(0, 50);
	while( GetThreadAliveFlag(0) )
	{
		Sleep(50);
		if( GetPauseFlag(0) ) {
			Sleep(1);
			continue;
		}
		int nCmdCount = GetCountAsyncDbCmd();
		if( nCmdCount > 0 ){
			POSITION pos = m_listAsyncDBCmd.GetHeadPosition();
			while(pos){
				m_csCmdSync.Lock();
				POSITION pos_pre = pos;
				CADBC* pItem = (CADBC*)m_listAsyncDBCmd.GetNext(pos);
				pItem->Execute();
				delete pItem;
				pItem = NULL;
				m_listAsyncDBCmd.RemoveAt(pos_pre);
				m_csCmdSync.Unlock();
			}
		}
	}
	m_Logger.LOG("END - Async Command Execute Thread.");
}

// szIniFileName : .\\sys_data\\TW_LS_DRIVER.INI
int  CDatabaseIF_Imp::Create(const char* szIniFileName, int nFileNameLen, int nType)
{
	sprintf_s( m_szIniFileName, sizeof( m_szIniFileName ), "%s", szIniFileName );

	CConfigData ConfigData(m_szIniFileName);

	char szBuff[ 512 ]			= { 0, };
	char szBuff2[ 512 ]			= { 0, };
	char szSysDataPath[ 512 ]	= { 0, };

	char* p = NULL;
	ZeroMemory(szBuff, sizeof(szBuff));
	sprintf_s(szBuff, sizeof(szBuff), "%s", szIniFileName);
	p = strrchr(szBuff, '\\'); *(p+1) = '\0';

	char szMainTitle[512] = {0,};
	if(nType == eDB_TYPE_SYSTEM){
		g_DbIf = this;

		sprintf_s(szMainTitle, sizeof(szMainTitle), "DATABASE CONFIG");

		// DB File Name
		ConfigData.GetString( szMainTitle, "DB File Name of System DB", "TW_LH.db3", szBuff2);
		sprintf_s(m_szDatabaseFileName, sizeof(m_szDatabaseFileName), "%s%s",  szBuff, szBuff2);

		// LOG File Name
		char szLogFileName[ 512 ] = { 0, };
		ConfigData.GetString( szMainTitle, "Log File Name for System DB", "SYSDBIF.log", szBuff);
		sprintf_s(szLogFileName, sizeof(szLogFileName), "%s%s", SZ_LOG_FILE_PATH, szBuff);
		m_Logger.SetFileName(szLogFileName);
	}else if (nType == eDB_TYPE_LOG){
		sprintf_s(szMainTitle, sizeof(szMainTitle), "DATABASE CONFIG");

		// DB File Name
		ConfigData.GetString( szMainTitle, "DB File Name of LOG DB", "TW_LH_LOG.db3", szBuff2);
		sprintf_s(m_szDatabaseFileName, sizeof(m_szDatabaseFileName), "%s%s",  szBuff, szBuff2);

		// LOG File Name
		char szLogFileName[ 512 ] = { 0, };
		ConfigData.GetString( szMainTitle, "Log File Name for LOG DB", "LOGDBIF.log", szBuff);
		sprintf_s(szLogFileName, sizeof(szLogFileName), "%s%s", SZ_LOG_FILE_PATH, szBuff);
		m_Logger.SetFileName(szLogFileName);
	}else if (nType == eDB_TYPE_AARCHECK){ // add by SJI [160919] Add AAB Retest Rule Check
		sprintf_s(szMainTitle, sizeof(szMainTitle), "DATABASE CONFIG");

		//DB File Name
		ConfigData.GetString( szMainTitle, "DB FIle Name of AAB DB", "TW_LH_AAB.db3", szBuff2);
		sprintf_s(m_szDatabaseFileName, sizeof(m_szDatabaseFileName), "%s%s", szBuff,szBuff2);

		// LOG File Name
		char szLogFileName[ 512 ] = { 0, };
		ConfigData.GetString( szMainTitle, "Log File Name for AAB DB", "AABDBIF.log", szBuff);
		sprintf_s(szLogFileName, sizeof(szLogFileName), "%s%s", SZ_LOG_FILE_PATH, szBuff);
		m_Logger.SetFileName(szLogFileName);
	}
	else if (nType == eDB_TYPE_TEACH){ 
		sprintf_s(szMainTitle, sizeof(szMainTitle), "DATABASE CONFIG");

		//DB File Name
		ConfigData.GetString( szMainTitle, "DB FIle Name of TEACH DB", "TW_LH_TEACH.db3", szBuff2);
		sprintf_s(m_szDatabaseFileName, sizeof(m_szDatabaseFileName), "%s%s", szBuff,szBuff2);

		// LOG File Name
		char szLogFileName[ 512 ] = { 0, };
		ConfigData.GetString( szMainTitle, "Log File Name for TEACH DB", "TEACHDBIF.log", szBuff);
		sprintf_s(szLogFileName, sizeof(szLogFileName), "%s%s", SZ_LOG_FILE_PATH, szBuff);
		m_Logger.SetFileName(szLogFileName);
	}
	else if (nType == eDB_TYPE_LOTHISTORY) {
		sprintf_s(szMainTitle, sizeof(szMainTitle), "DATABASE CONFIG");

		//DB File Name
		ConfigData.GetString(szMainTitle, "DB FIle Name of LOTHISTORY DB", "TW_LH_LOTHISTORY.db3", szBuff2);
		sprintf_s(m_szDatabaseFileName, sizeof(m_szDatabaseFileName), "%s%s", szBuff, szBuff2);

		// LOG File Name
		char szLogFileName[512] = { 0, };
		ConfigData.GetString(szMainTitle, "Log File Name for LOTHISTORY DB", "LOTHISTORY.log", szBuff);
		sprintf_s(szLogFileName, sizeof(szLogFileName), "%s%s", SZ_LOG_FILE_PATH, szBuff);
		m_Logger.SetFileName(szLogFileName);
	}
	else if (nType == eDB_TYPE_VAT) {
		sprintf_s(szMainTitle, sizeof(szMainTitle), "DATABASE CONFIG");

		//DB File Name
		ConfigData.GetString(szMainTitle, "DB FIle Name of VAT DB", "TW_LH_VAT.db3", szBuff2);
		sprintf_s(m_szDatabaseFileName, sizeof(m_szDatabaseFileName), "%s%s", szBuff, szBuff2);

		// LOG File Name
		char szLogFileName[512] = { 0, };
		ConfigData.GetString(szMainTitle, "Log File Name for VAT DB", "VAT.log", szBuff);
		sprintf_s(szLogFileName, sizeof(szLogFileName), "%s%s", SZ_LOG_FILE_PATH, szBuff);
		m_Logger.SetFileName(szLogFileName);
	}
	ConnectDatabase( m_szDatabaseFileName );

	// Machine setup date config.
	ConfigData.GetString( "DATABASE CONFIG", "Machine Setup Date", "2015-07-22 00:00:00.000", m_szMachineSetupDate );

	m_db.execDML("vacuum");
	// Thread
	CreateThread(0);

	return 0;
}

void CDatabaseIF_Imp::Destroy()
{
	CloseThread();
	CLEAR_DB_QUEUE();
	CloseDatabase();
}

int  CDatabaseIF_Imp::ConnectDatabase(const char* szDBPath)
{
	try
	{
		if( m_bOpened ){
			CloseDatabase();
		}
		m_db.open(szDBPath);
	}
	catch(CppSQLite3Exception& e)
	{
		AfxMessageBox(e.errorMessage());
	}
	return 0;
}

void CDatabaseIF_Imp::CloseDatabase()
{
	m_db.close();
}

//------------------------------------------------------------------------------------------
// other....
int CDatabaseIF_Imp::CHECK_TABLE( CString strTableName )
{
	bool bExist = m_db.tableExists(strTableName);

	return !((int)bExist);
// 	CppSQLite3Query q;
// 	
// 	try{
// 		CString strQuery;
// 		strQuery.Format("select count(*) from sqlite_master where name = '%s'", strTableName );
// 		q = m_db.execQuery( strQuery );
// 
// 		
// 		
// 
// 		int nTableCount	= q.getIntField(0);
// 
// 		if( nTableCount == 0 )
// 		{
// 			q.finalize();
// 			return -1;
// 		}
// 	}
// 	catch( CppSQLite3Exception& e)
// 	{
// 		DWORD dwErr = ::GetLastError();
// 		m_Logger.LOG("CDatabaseIF_Imp::CHECK_TABLE(int nDir, CMyList& rDataList) Error, [DB ERR: %d = %s] [WIN ERR:%d] ", e.errorCode(), e.errorMessage(), dwErr);
// 	}
// 
// 	q.finalize();
//
//	return 0;
}


//------------------------------------------------------------------------------------------
// DIO_LS
int CDatabaseIF_Imp::SELECT_DIO_TBL(int nDir, CMyList& rDataList)
{
	m_csQuary.Lock();
	CppSQLite3Query q;
	
	try
	{
		CString strQuery;
		strQuery.Format("SELECT * FROM TBL_DIO WHERE DIRECTION = '%s' ORDER BY IDX", (nDir == _tDIO_LS::eDI ? _T("INPUT") : _T("OUTPUT")));
		q = m_db.execQuery( strQuery );
		while( !q.eof() )
		{
			_tDIO_LS* pRow = new _tDIO_LS;

			pRow->no = q.getIntField(_tDIO_LS::NO);
			pRow->dir = q.getStringField(_tDIO_LS::DIRECTION) == _T("INPUT") ? _tDIO_LS::eDI : _tDIO_LS::eDO;
			pRow->index = q.getIntField(_tDIO_LS::IDX);
			pRow->address = q.getIntField(_tDIO_LS::ADDRESS);
			pRow->part_no = q.getIntField(_tDIO_LS::PART_NO);
			pRow->label = q.getStringField(_tDIO_LS::LABEL);
			pRow->name = q.getStringField(_tDIO_LS::NAME);
			pRow->contact = (_tDIO_LS::eDIO_CT)q.getIntField(_tDIO_LS::CONTACT);
			pRow->dio_type = q.getStringField(_tDIO_LS::DIO_TYPE);


// 			pRow->nDir		= q.getIntField(0);
// 			pRow->nIndex	= q.getIntField(1);
// 			pRow->strName	= q.getStringField(2);
// 			pRow->strLabel	= q.getStringField(3);
// 			pRow->nContact	= q.getIntField(4);

			rDataList.m_fnAddData(pRow);
			q.nextRow();
		}
	}
	catch( CppSQLite3Exception& e )
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int CDatabaseIF_Imp::SELECT_DIO_TBL(int nDir, CMyList& rDataList) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
		//ret = e.errorCode();
	}	

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

int CDatabaseIF_Imp::SELECT_DIO_TBL( int nDir, int nFrmIdx, int nIdxCount, CMyList& rDataList )
{
	m_csQuary.Lock();
	CppSQLite3Query q;

	try
	{
		CString strQuery;
		strQuery.Format("SELECT * FROM TBL_DIO WHERE DIRECTION = '%s' AND IDX >= %d AND IDX < %d ORDER BY IDX", (nDir == _tDIO_LS::eDI ? _T("INPUT") : _T("OUTPUT")), nFrmIdx, nFrmIdx + nIdxCount );
		q = m_db.execQuery( strQuery );
		while( !q.eof() )
		{
			_tDIO_LS* pRow = new _tDIO_LS;

			pRow->no = q.getIntField(_tDIO_LS::NO);
			pRow->dir = q.getStringField(_tDIO_LS::DIRECTION) == _T("INPUT") ? _tDIO_LS::eDI : _tDIO_LS::eDO;
			pRow->index = q.getIntField(_tDIO_LS::IDX);
			pRow->address = q.getIntField(_tDIO_LS::ADDRESS);
			pRow->part_no = q.getIntField(_tDIO_LS::PART_NO);
			pRow->label = q.getStringField(_tDIO_LS::LABEL);
			pRow->name = q.getStringField(_tDIO_LS::NAME);
			pRow->contact = (_tDIO_LS::eDIO_CT)q.getIntField(_tDIO_LS::CONTACT);
			pRow->dio_type = q.getStringField(_tDIO_LS::DIO_TYPE);

// 			pRow->nDir		= q.getIntField(0);
// 			pRow->nIndex	= q.getIntField(1);
// 			pRow->strName	= q.getStringField(2);
// 			pRow->strLabel	= q.getStringField(3);
// 			pRow->nContact	= q.getIntField(4);

			rDataList.m_fnAddData(pRow);
			q.nextRow();
		}
	}
	catch( CppSQLite3Exception& e )
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int CDatabaseIF_Imp::SELECT_DIO_TBL(int nDir, CMyList& rDataList) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}	

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

//------------------------------------------------------------------------------------------
// GPIB_LOG
int CDatabaseIF_Imp::INSERT_GPIB_LOG_TBL(CString strMsg, int nDate)
{
	m_csQuary.Lock();
	CppSQLite3Query q;

	try{
		CString strQuery;
		strQuery.Format(_T("INSERT INTO TBL_GPIB_LOG(MESSAGE, DATE) VALUES('%s', %d)"), strMsg, nDate);
		q = m_db.execQuery( strQuery );
		m_Logger.LOG("SQL [%s]", strQuery);
	}catch( CppSQLite3Exception& e){
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("CDatabaseIF_Imp::INSERT_GPIB_LOG_TBL(CString strMsg, int nDate) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}	

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

//------------------------------------------------------------------------------------------
// TEMP_PID Table
int	CDatabaseIF_Imp::INSERT_TEMP_PID_TBL(int nCtrlNo, _tTEMP_PID& rTempPid)
{
	m_csQuary.Lock();
	CppSQLite3Query q;

	try
	{
		CString strQuery;
		strQuery.Format("INSERT INTO TBL_TEMP_PID(CTRL_NO, SET_TEMP, PID_P1, PID_P2, PID_I, PID_D) VALUES('%d', '%f', '%f', '%f', '%f', '%f')", 
				nCtrlNo, rTempPid.dSetTemp, rTempPid.dPidP1, rTempPid.dPidP2, rTempPid.dPidI, rTempPid.dPidD);
		q = m_db.execQuery( strQuery );
		m_Logger.LOG("SQL [%s]", strQuery);
	}
	catch( CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int	CDatabaseIF_Imp::INSERT_TEMP_PID_TBL(_tTEMP_PID& rTempPid) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}	

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

int	CDatabaseIF_Imp::DELETE_TEMP_PID_TBL(int nCtrlNo)
{
	m_csQuary.Lock();
	CppSQLite3Query q;

	try
	{
		CString strQuery;
		strQuery.Format("DELETE FROM TBL_TEMP_PID WHERE CTRL_NO = %d", nCtrlNo);
		q = m_db.execQuery( strQuery );
	}
	catch( CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int	CDatabaseIF::DELETE_TEMP_PID_TBL(int nCtrlNo) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}	

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

int	CDatabaseIF_Imp::UPDATE_TEMP_PID_TBL(CMyList& rDataList)
{
	for( int i = 0; i < rDataList.m_fnGetCount(); i++ )
	{
		_tTEMP_PID* pData = ( _tTEMP_PID* )rDataList.m_fnGetData( i );

		this->UPDATE_TEMP_PID_TBL( pData->nCtrlNo, *pData );
	}

	return 0;
}

int	CDatabaseIF_Imp::UPDATE_TEMP_PID_TBL(int nCtrlNo, _tTEMP_PID& rTempPid)
{
	m_csQuary.Lock();
	CppSQLite3Query q;

	try
	{
		CString strQuery;
		strQuery.Format("UPDATE TBL_TEMP_PID SET SET_TEMP = %f, PID_P1 = %f, PID_P2 = %f, PID_I = %f, PID_D = %f WHERE CTRL_NO = %d;", 
			rTempPid.dSetTemp, rTempPid.dPidP1, rTempPid.dPidP2, rTempPid.dPidI, rTempPid.dPidD, nCtrlNo);

		q = m_db.execQuery( strQuery );
		m_Logger.LOG("SQL [%s]", strQuery);
	}
	catch( CppSQLite3Exception& e )
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int	CDatabaseIF_Imp::UPDATE_TEMP_PID_TBL(CMyList& rDataList) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}	

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

int	CDatabaseIF_Imp::SELECT_TEMP_PID_TBL(CMyList& rDataList)
{
	m_csQuary.Lock();
	CppSQLite3Query q;

	try
	{
		CString strQuery;
		strQuery.Format("SELECT * FROM TBL_TEMP_PID ORDER BY CTRL_NO");
		q = m_db.execQuery( strQuery );

		while( !q.eof() )
		{
			_tTEMP_PID* pRow = new _tTEMP_PID;

			pRow->nCtrlNo	= q.getIntField(0);
			pRow->dSetTemp	= q.getFloatField(1);
			pRow->dPidP1	= q.getFloatField(2);
			pRow->dPidP2	= q.getFloatField(3);
			pRow->dPidI		= q.getFloatField(4);
			pRow->dPidD		= q.getFloatField(5);

			rDataList.m_fnAddData(pRow);
			q.nextRow();
		}
	}
	catch( CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int	CDatabaseIF_Imp::SELECT_TEMP_PID_TBL(CMyList& rDataList) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}	

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

int	CDatabaseIF_Imp::SELECT_TEMP_PID_TBL(int nCtrlNo, _tTEMP_PID& rTempPid)
{	
	m_csQuary.Lock();
	CppSQLite3Query q;

	try
	{
		CString strQuery;
		strQuery.Format("SELECT * FROM TBL_TEMP_PID WHERE CTRL_NO = %d", nCtrlNo);
		q = m_db.execQuery( strQuery );

		if( q.eof() == true )
		{
			q.finalize();
			m_csQuary.Unlock();
			return -1;
		}

		rTempPid.nCtrlNo	= q.getIntField(0);
		rTempPid.dSetTemp	= q.getFloatField(1);
		rTempPid.dPidP1		= q.getFloatField(2);
		rTempPid.dPidP2		= q.getFloatField(3);
		rTempPid.dPidI		= q.getFloatField(4);
		rTempPid.dPidD		= q.getFloatField(5);
	}
	catch( CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int	CDatabaseIF_Imp::SELECT_TEMP_PID_TBL(int nCtrlNo, _tTEMP_PID& rTempPid) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}	

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

//------------------------------------------------------------------------------------------
// MOTOR PARAM Table
int CDatabaseIF_Imp::SELECT_MOTORPARAM_TBL( CMyList& rMotorParamList )
{
	m_csQuary.Lock();
	CppSQLite3Query q;

	try
	{
		CString strQuery;
//		strQuery.Format("SELECT * FROM TBL_MOTOR_PARAM ORDER BY AXIS_NO_LOGICAL");
		strQuery.Format("SELECT * FROM TBL_AXIS_PARAM ORDER BY AXIS_NO_LOGICAL");

		q = m_db.execQuery( strQuery );

		while( !q.eof() )
		{
			// 반드시 정의된 Field(Column) 순서대로 순차적으로 읽어들어야 한다.
			_tMOTOR_PARAM* pRow = new _tMOTOR_PARAM;

			int nIndex = 0;
			pRow->strBoardType = q.getStringField(nIndex); nIndex++;
			pRow->nAxisNo_Logical = q.getIntField(nIndex); nIndex++;
			pRow->nAxisNo_Physical = q.getIntField(nIndex); nIndex++;
			pRow->nPartNo = q.getIntField(nIndex); nIndex++;
			pRow->strAxisName = q.getStringField(nIndex); nIndex++;
			pRow->dScaleFactor = q.getFloatField(nIndex); nIndex++;
			pRow->nPulsePerRound = q.getIntField(nIndex); nIndex++;
			pRow->nDir_ZR = q.getIntField(nIndex); nIndex++;
			pRow->nDirPulse = q.getIntField(nIndex); nIndex++;
			pRow->nDirMove = q.getIntField(nIndex); nIndex++;
			pRow->dLimit_Posi = q.getFloatField(nIndex); nIndex++;
			pRow->dLimit_Nega = q.getFloatField(nIndex); nIndex++;
			pRow->nSpd_Normal = q.getIntField(nIndex); nIndex++;
			pRow->nAcc_Normal = q.getIntField(nIndex); nIndex++;
			pRow->nDec_Normal = q.getIntField(nIndex); nIndex++;
			pRow->nSpd_ZR = q.getIntField(nIndex); nIndex++;
			pRow->nAcc_ZR = q.getIntField(nIndex); nIndex++;
			pRow->nDouble_Chk_Pulse = q.getIntField(nIndex); nIndex++;
			pRow->nMMC_Encoder_Dir = q.getIntField(nIndex); nIndex++;
			pRow->nEncoder_Type = q.getIntField(nIndex); nIndex++;
			pRow->nMMC_Coordinate_Dir = q.getIntField(nIndex); nIndex++;
			pRow->nMotorType = q.getIntField(nIndex); nIndex++;
			pRow->nSyncType = q.getIntField(nIndex); nIndex++;
			pRow->nSlaveNo = q.getIntField(nIndex); nIndex++;
			pRow->dHwInPositionWidth = q.getIntField(nIndex); nIndex++;
			pRow->eMotorCoordinate = (_tMOTOR_PARAM::_eMotorCoordinate)q.getIntField(nIndex); nIndex++;
			pRow->bDegree = q.getIntField(nIndex); nIndex++;

			rMotorParamList.m_fnAddData(pRow);
			q.nextRow();
		}
	}
	catch( CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int	CDatabaseIF_Imp::SELECT_MOTORPARAM_TBL(CMyList& rDataList) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}	

	q.finalize();
	m_csQuary.Unlock();
	
	return 0;
}

int CDatabaseIF_Imp::SELECT_MOTORPARAM_TBL( int nAxisNo, _tMOTOR_PARAM& rMotorParam )
{
	m_csQuary.Lock();

	CppSQLite3Query q;

	try
	{
		CString strQuery;
		strQuery.Format(_T("SELECT * FROM TBL_AXIS_PARAM WHERE AXIS_NO_LOGICAL = %d"), nAxisNo);
		q = m_db.execQuery(strQuery);

		if (q.eof() == true)
		{
			q.finalize();
			m_csQuary.Unlock();
			return -1;
		}

		int nIndex = 0;
		rMotorParam.strBoardType = q.getStringField(nIndex); nIndex++;
		rMotorParam.nAxisNo_Logical = q.getIntField(nIndex); nIndex++;
		rMotorParam.nAxisNo_Physical = q.getIntField(nIndex); nIndex++;
		rMotorParam.nPartNo = q.getIntField(nIndex); nIndex++;
		rMotorParam.strAxisName = q.getStringField(nIndex); nIndex++;
		rMotorParam.dScaleFactor = q.getFloatField(nIndex); nIndex++;
		rMotorParam.nPulsePerRound = q.getIntField(nIndex); nIndex++;
		rMotorParam.nDir_ZR = q.getIntField(nIndex); nIndex++;
		rMotorParam.nDirPulse = q.getIntField(nIndex); nIndex++;
		rMotorParam.nDirMove = q.getIntField(nIndex); nIndex++;
		rMotorParam.dLimit_Posi = q.getFloatField(nIndex); nIndex++;
		rMotorParam.dLimit_Nega = q.getFloatField(nIndex); nIndex++;
		rMotorParam.nSpd_Normal = q.getIntField(nIndex); nIndex++;
		rMotorParam.nAcc_Normal = q.getIntField(nIndex); nIndex++;
		rMotorParam.nDec_Normal = q.getIntField(nIndex); nIndex++;
		rMotorParam.nSpd_ZR = q.getIntField(nIndex); nIndex++;
		rMotorParam.nAcc_ZR = q.getIntField(nIndex); nIndex++;
		rMotorParam.nDouble_Chk_Pulse = q.getIntField(nIndex); nIndex++;
		rMotorParam.nMMC_Encoder_Dir = q.getIntField(nIndex); nIndex++;
		rMotorParam.nEncoder_Type = q.getIntField(nIndex); nIndex++;
		rMotorParam.nMMC_Coordinate_Dir = q.getIntField(nIndex); nIndex++;
		rMotorParam.nMotorType = q.getIntField(nIndex); nIndex++;
		rMotorParam.nSyncType = q.getIntField(nIndex); nIndex++;
		rMotorParam.nSlaveNo = q.getIntField(nIndex); nIndex++;
		rMotorParam.dHwInPositionWidth = q.getIntField(nIndex); nIndex++;
		rMotorParam.eMotorCoordinate = (_tMOTOR_PARAM::_eMotorCoordinate)q.getIntField(nIndex); nIndex++;
	}
	catch( CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int CDatabaseIF_Imp::SELECT_MOTORPARAM_TBL( int nAxisNo, _tMOTOR_PARAM& rMotorParam ) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}	

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

int CDatabaseIF_Imp::UPDATE_MOTORPARAM_TBL( CMyList& rMotorParamList )
{
	for( int i = 0; i < rMotorParamList.m_fnGetCount(); i++ )
	{
		_tMOTOR_PARAM* pData = ( _tMOTOR_PARAM* )rMotorParamList.m_fnGetData( i );

		this->UPDATE_MOTORPARAM_TBL( pData->nAxisNo_Logical, *pData );
	}

	return 0;
}

int CDatabaseIF_Imp::UPDATE_MOTORPARAM_TBL( int nAxisNo, _tMOTOR_PARAM& rMotorParamList )
{
	m_csQuary.Lock();
	CppSQLite3Query q;

	try
	{
		CString strQuery;
		strQuery.Format(_T("UPDATE TBL_AXIS_PARAM SET ")
			_T("Scale_Factor = %f, ")
			_T("SPD_Normal = %d, ")
			_T("ACC_Normal = %d, ")
			_T("DEC_Normal = %d, ")
			_T("LIMIT_POSI = %f, ")
			_T("LIMIT_NEGA = %f, ")
			_T("SPD_ZR = %d, ")
			_T("ACC_ZR = %d ")
			_T("WHERE AXIS_NO_LOGICAL = %d"),
			rMotorParamList.dScaleFactor,
			rMotorParamList.nSpd_Normal,
			rMotorParamList.nAcc_Normal,
			rMotorParamList.nDec_Normal,
			rMotorParamList.dLimit_Posi,
			rMotorParamList.dLimit_Nega,
			rMotorParamList.nSpd_ZR,
			rMotorParamList.nAcc_ZR,
			rMotorParamList.nAxisNo_Logical);

		q = m_db.execQuery(strQuery);
		m_Logger.LOG(_T("SQL [%s]"), strQuery);
	}
	catch( CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int CDatabaseIF_Imp::UPDATE_MOTORPARAM_TBL( int nAxisNo, _tMOTOR_PARAM& rMotorParamList ) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}	

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

int CDatabaseIF_Imp::UPDATE_MOTORPARAM_TBL(int nPhyNo, double dScaleFactor)
{
	m_csQuary.Lock();
	CppSQLite3Query q;

	try
	{
		CString strQuery;
		strQuery.Format(_T("UPDATE TBL_AXIS_PARAM SET Scale_Factor = %f WHERE Axis_No_Physical = %d"), dScaleFactor, nPhyNo);

		q = m_db.execQuery(strQuery);
		m_Logger.LOG(_T("SQL [%s]"), strQuery);
	}
	catch (CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int CDatabaseIF_Imp::UPDATE_MOTORPARAM_TBL( int nPhyNo, double dScaleFactor ) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}

	q.finalize();
	m_csQuary.Unlock();

	return 0;

}


//------------------------------------------------------------------------------------------
// Auto GRR Data Table
int CDatabaseIF_Imp::INSERT_GRRDATA_TBL(const _tGRR_DATA& rGrrData)
{
	CppSQLite3Query q;

	// 2DID 붙여서 쓰면 예약어에 걸리네..;
	try
	{
		CString strQuery;
		strQuery.Format("INSERT INTO TBL_GRR_INFO(UNIQUE_ID, [2DID], SITE_NUM, SUB_SITE_NUM, TEST_NO, TEST_RESULT, RESULT_DESCRIPTION) VALUES('%s', '%s', '%s', '%s', '%d', '%s', '%s')", 
			rGrrData.szUnique, rGrrData.sz2DID, rGrrData.szSiteNum, rGrrData.szSubSiteNo, rGrrData.nTestCntNo, rGrrData.szTestResult, rGrrData.szResultDescription);
		q = m_db.execQuery( strQuery );
		m_Logger.LOG("SQL [%s]", strQuery);
	}
	catch( CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int	CDatabaseIF_Imp::INSERT_GRRDATA_TBL(const _tGRR_DATA& rGrrData) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}

	q.finalize();

	return 0;
}

int CDatabaseIF_Imp::ASYNC_INSERT_GRRDATA_TBL(const _tGRR_DATA& rGrrData)
{
	CADBC* pGrrDbData = new CADBC_Insert_GrrData;
	pGrrDbData->SetInsertData(rGrrData);

	return EXEC_ASYNC_DB_CMD(pGrrDbData);
}

int CDatabaseIF_Imp::DELETE_ALL_GRRDATA_TBL()
{
	CppSQLite3Query q;

	try
	{
		CString strQuery = _T("DELETE FROM TBL_GRR_INFO");
		q = m_db.execQuery( strQuery );
		m_Logger.LOG("SQL [%s]", strQuery);
	}
	catch( CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int	CDatabaseIF_Imp::DELETE_ALL_GRRDATA_TBL() Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}

	q.finalize();

	return 0;
}

int CDatabaseIF_Imp::SELECT_GRRDATA_TBL(CMyList& rGrrDataList)
{
	CppSQLite3Query q;

	try
	{
		CString strQuery;
		//Sub로 바꿔
	/*	strQuery.Format("SELECT * FROM TBL_GRR_INFO ORDER BY CAST(UNIQUE_ID as integer) ASC, CAST(SITE_NUM as integer) ASC, CAST(SUB_SITE_NUM as integer) ASC");*/
		strQuery.Format("SELECT * FROM TBL_GRR_INFO ORDER BY UNIQUE_ID ASC, CAST(SITE_NUM as integer) ASC, CAST(SUB_SITE_NUM as integer) ASC ,CAST(TEST_NO as integer) ASC");

		//원본
		//strQuery.Format("SELECT * FROM TBL_GRR_INFO ORDER BY UNIQUE_ID ASC, CAST(SITE_NUM as integer) ASC, CAST(TEST_NO as integer) ASC");
		q = m_db.execQuery( strQuery );
		m_Logger.LOG("SQL [%s]", strQuery);

		int nIdx = 0;
		while( !q.eof() )
		{
			_tGRR_DATA* pRow = new _tGRR_DATA;

			nIdx = 0;
			strcpy_s(pRow->szUnique, q.getStringField(nIdx++));
			strcpy_s(pRow->sz2DID, q.getStringField(nIdx++));
			strcpy_s(pRow->szSiteNum, q.getStringField(nIdx++));
			strcpy_s(pRow->szSubSiteNo, q.getStringField(nIdx++));
			pRow->nTestCntNo = q.getIntField(nIdx++);
			strcpy_s(pRow->szTestResult, q.getStringField(nIdx++));
			strcpy_s(pRow->szResultDescription, q.getStringField(nIdx++));

			rGrrDataList.m_fnAddData(pRow);
			q.nextRow();
		}
	}
	catch( CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int	CDatabaseIF_Imp::SELECT_GRRDATA_TBL(CMyList& rGrrDataList) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}

	q.finalize();

	return 0;
}

int CDatabaseIF_Imp::SELECT_COUNT_GRRDATA_TBL()
{
	CppSQLite3Query q;

	int nCount =0;
	try
	{
		CString strQuery;
		strQuery.Format("SELECT COUNT(*) FROM TBL_GRR_INFO");
		q = m_db.execQuery( strQuery );
		m_Logger.LOG("SQL [%s]", strQuery);

		nCount = q.getIntField(0);
	}
	catch( CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int	CDatabaseIF_Imp::SELECT_GRRDATA_TBL(CMyList& rGrrDataList) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}
	q.finalize();

	return nCount;
}

int CDatabaseIF_Imp::CREATE_GRRDATA_TBL()
{
	try
	{
		CString strQuery;
		strQuery.Format("CREATE TABLE 'TBL_GRR_INFO' ('UNIQUE_ID' VARCHAR(10) NOT NULL ,'2DID' VARCHAR(64),'SITE_NUM' VARCHAR(32) NOT NULL , 'SUB_SITE_NUM' VARCHAR(32) NOT NULL ,'TEST_NO' INTEGER NOT NULL ,'TEST_RESULT' VARCHAR(16) NOT NULL ,'RESULT_DESCRIPTION' VARCHAR(128) DEFAULT (null) );");

		m_db.execDML( strQuery );		
	}
	catch( CppSQLite3Exception& e )
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int CDatabaseIF_Imp::CREATE_GRRDATA_TBL() Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}	
	return 0;
}

//------------------------------------------------------------------------------------------
// TBL_ERROR_LOG
int     CDatabaseIF_Imp::DELETE_ERROR_LOG_TBL(CString strDate_S, CString strDate_E)
{
	m_csQuary.Lock();

	int nStartYYYY = atoi( strDate_S.Mid( 0, 4 ) );
	int nStartMM = atoi( strDate_S.Mid( 5, 2 ) );

	int nEndYYYY = atoi( strDate_E.Mid( 0, 4 ) );
	int nEndMM = atoi( strDate_E.Mid( 5, 2 ) );

	int nYearGap = nEndYYYY - nStartYYYY;
	int nMonthGap = nEndMM - nStartMM;

	int nMonthCount = ( nYearGap * 12 ) + nMonthGap; // 시작일자와 종료일자 사이의 달 간격 수.

	CString strStart_YYYYMM;
	strStart_YYYYMM.Format( "%d%02d", nStartYYYY, nStartMM );

	CString strEnd_YYYYMM;
	strEnd_YYYYMM.Format( "%d%02d", nEndYYYY, nEndMM );

	CString strQuery = "";
	CString strCondTemp = "";

	CppSQLite3Query q;

	try
	{
		if( strStart_YYYYMM == strEnd_YYYYMM ) // 시작 일자와 종료일자가 같을 경우.
		{ 
			strQuery.Format( _T("DELETE FROM TBL_ERROR_LOG_%s WHERE OCCURRED_TIME >= '%s' AND OCCURRED_TIME <= '%s'"), strStart_YYYYMM, strDate_S, strDate_E );
			q = m_db.execQuery( strQuery );
		}
		else
		{
			strQuery.Format( _T("DELETE FROM TBL_ERROR_LOG_%s WHERE OCCURRED_TIME >= '%s'"), strStart_YYYYMM, strDate_S );
			q = m_db.execQuery( strQuery );

			int nTempYear = nStartYYYY;
			int nTempMonth = nStartMM;

			CString strMidTableQuery =_T("");
			CString strMidTableName = _T("");

			for( int i = 1; i < nMonthCount; i++ )
			{
				if( nTempMonth + 1 > 12 )
				{				
					nTempYear += 1;
					nTempMonth = 1;
				}
				else
				{
					nTempMonth += 1;
				}

				strMidTableName.Format( "%d%02d", nTempYear, nTempMonth );

				strMidTableQuery = _T( "DELETE FROM TBL_ERROR_LOG_" + strMidTableName );

				q = m_db.execQuery( strMidTableQuery );
			}

			CString strEndTableQuery;
			strEndTableQuery.Format( _T("DELETE FROM TBL_ERROR_LOG_%s WHERE OCCURRED_TIME <= '%s'"), strEnd_YYYYMM, strDate_E );

			q = m_db.execQuery( strEndTableQuery );
		}
	}
	catch( CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int     CDatabaseIF_Imp::DELETE_ERROR_LOG_TBL(CString strDate_S, CString strDate_E) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}	

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

int CDatabaseIF_Imp::SELECT_ERROR_LOG_TBL(CString strDate_S, CString strDate_E, CMyList& rLogList)
{
	CString strQuery = this->MAKE_SELECT_ERROR_LOG_QUERY( strDate_S, strDate_E );

	return this->EXEC_SELECT_ERROR_LOG_QUERY(strDate_S, strDate_E, rLogList, strQuery );
}

int CDatabaseIF_Imp::SELECT_ERROR_LOG_TBL(CString strDate_S, CString strDate_E, std::vector<ST_ERROR_MESSAGE>& vLogVector) {

	CString strQuery = this->MAKE_SELECT_ERROR_LOG_QUERY(strDate_S, strDate_E);

	return this->EXEC_SELECT_ERROR_LOG_QUERY(strDate_S, strDate_E, vLogVector, strQuery);
}

int CDatabaseIF_Imp::SELECT_ERROR_LOG_TBL(CMyList& rLogList)
{
	CTime time = CTime::GetCurrentTime();
	CString strDate_S = m_szMachineSetupDate;
	CString strDate_E;
	strDate_E.Format( "%04d-%02d-%02d %02d:%02d:%02d", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond() );

	return this->SELECT_ERROR_LOG_TBL( strDate_S, strDate_E, rLogList );
}

int CDatabaseIF_Imp::SELECT_ERROR_LOG_TBL_ERRORCODE(CString strDate_S, CString strDate_E, CString strErrorCode, CMyList& rLogList)
{
	CString strQuery = this->MAKE_SELECT_ERROR_LOG_QUERY( strDate_S, strDate_E, "EQP_ALID", strErrorCode );

	return this->EXEC_SELECT_ERROR_LOG_QUERY(strDate_S, strDate_E, rLogList, strQuery );
}

int CDatabaseIF_Imp::SELECT_ERROR_LOG_TBL_ERRORCODE(CString strErrorCode, CMyList& rLogList)
{
	CTime time = CTime::GetCurrentTime();
	CString strDate_S = m_szMachineSetupDate;
	CString strDate_E;
	strDate_E.Format( "%04d-%02d-%02d %02d:%02d:%02d", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond() );

	CString strQuery = this->MAKE_SELECT_ERROR_LOG_QUERY( strDate_S, strDate_E, "EQP_ALID", strErrorCode );

	return this->EXEC_SELECT_ERROR_LOG_QUERY(strDate_S, strDate_E, rLogList, strQuery );
}

int CDatabaseIF_Imp::SELECT_ERROR_LOG_TBL_PART     (CString strDate_S, CString strDate_E, CString strPart, CMyList& rLogList)
{
	CString strQuery = this->MAKE_SELECT_ERROR_LOG_QUERY( strDate_S, strDate_E, "PART", strPart  );

	return this->EXEC_SELECT_ERROR_LOG_QUERY(strDate_S, strDate_E, rLogList, strQuery );
}

int CDatabaseIF_Imp::SELECT_ERROR_LOG_TBL_PART     (CString strPart, CMyList& rLogList)
{
	CTime time = CTime::GetCurrentTime();
	CString strDate_S = m_szMachineSetupDate;
	CString strDate_E;
	strDate_E.Format( "%04d-%02d-%02d %02d:%02d:%02d", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond() );

	CString strQuery = this->MAKE_SELECT_ERROR_LOG_QUERY( strDate_S, strDate_E, "PART", strPart );

	return this->EXEC_SELECT_ERROR_LOG_QUERY(strDate_S, strDate_E, rLogList, strQuery );
}

int	CDatabaseIF_Imp::SELECT_ERROR_LOG_TBL_LEVEL    (CString strDate_S, CString strDate_E, int nLevel, CMyList& rLogList, BOOL bDaliyReport, BOOL bChk_OnLine_OffLine)
{
	CString strLevel;
	strLevel.Format( "%d", nLevel );

	CString strQuery = this->MAKE_SELECT_ERROR_LOG_QUERY( strDate_S, strDate_E, "ERROR_LEVEL", strLevel  );

	return this->EXEC_SELECT_ERROR_LOG_QUERY(strDate_S, strDate_E, rLogList, strQuery, bDaliyReport, bChk_OnLine_OffLine);
}

int	CDatabaseIF_Imp::SELECT_ERROR_LOG_TBL_LEVEL(CString strDate_S, CString strDate_E, int nLevel, std::vector<ST_ERROR_MESSAGE>& vLogVector, BOOL bDaliyReport, BOOL bChk_OnLine_OffLine)
{
	CString strLevel;
	strLevel.Format("%d", nLevel);

	CString strQuery = this->MAKE_SELECT_ERROR_LOG_QUERY(strDate_S, strDate_E, "ERROR_LEVEL", strLevel);

	return this->EXEC_SELECT_ERROR_LOG_QUERY(strDate_S, strDate_E, vLogVector, strQuery, bDaliyReport, bChk_OnLine_OffLine);
}

int	CDatabaseIF_Imp::SELECT_ERROR_LOG_TBL_LEVEL(int nLevel, CMyList& rLogList)
{
	CString strLevel;
	strLevel.Format( "%d", nLevel );

	CTime time = CTime::GetCurrentTime();
	CString strDate_S = m_szMachineSetupDate;
	CString strDate_E;
	strDate_E.Format( "%04d-%02d-%02d %02d:%02d:%02d", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond() );

	CString strQuery = this->MAKE_SELECT_ERROR_LOG_QUERY( strDate_S, strDate_E, "ERROR_LEVEL", strLevel  );

	return this->EXEC_SELECT_ERROR_LOG_QUERY(strDate_S, strDate_E, rLogList, strQuery );
}

int CDatabaseIF_Imp::INSERT_ERROR_LOG_TBL(_tERROR_LOG& tErrLog)
{
	m_csQuary.Lock();

	CString strYYYYMM;
	strYYYYMM.Format( _T("%s%s"), tErrLog.strOccurred_Time.Mid( 0, 4 ), tErrLog.strOccurred_Time.Mid( 5, 2 ) );
		 
	CString strTableName;
	strTableName.Format( _T("TBL_ERROR_LOG_%s"), strYYYYMM );  

	int nResult = this->CHECK_TABLE( strTableName );

	if( 0 != nResult )
	{
		this->CREATE_ERROR_LOG_TBL( strYYYYMM );
	}

	//insert
	CppSQLite3Query q;

	try
	{
		CString strQuery;
	 
		CString strEQP_ALID = tErrLog.szEQP_ALID;
		CString strPart = tErrLog.szPart;
		CString strMessage = tErrLog.szMessage;
		CString strRecovry = tErrLog.szRecovery;
		CString strUserInfo = tErrLog.szUser_Info;
		CString strLotNoInfo = tErrLog.szLotNo_Info;
	 
		// DB Data에 홑따옴표 "'" 가 들어가면 안된다 그래서 '' 쌍홀따옴표로 변경 [9/1/2020 donghyun.shin]
		strEQP_ALID.Replace("'", "''");
		strPart.Replace("'", "''");
		strMessage.Replace("'", "''");
		strRecovry.Replace("'", "''");
		strUserInfo.Replace("'", "''");
		strLotNoInfo.Replace("'", "''");
		 	     
		strQuery.Format(_T("INSERT INTO %s ( EQP_ALID, PART, MESSAGE, OCCURRED_TIME, RECOVERY, STOP_TIME, USER_INFO, ERROR_LEVEL, POSITION, DUPLICATE, ONLINE_OFFLINE, LOTNO) VALUES ( '%s', '%s', '%s', '%s', '%s', %d, '%s', %d, %d, %d, %d, '%s')"),
			strTableName, strEQP_ALID, strPart, strMessage, tErrLog.strOccurred_Time, strRecovry, tErrLog.nStop_Time, strUserInfo, tErrLog.nError_Level, tErrLog.nPosition, tErrLog.nDuplicate, tErrLog.nOnLine_OffLine, strLotNoInfo);

		m_Logger.LOG((LPSTR)(LPCTSTR)strQuery);
		 
		q = m_db.execQuery( strQuery );

		strQuery.Format("'%s', '%s', '%s', '%s', '%s', %d, '%s', %d , %d , %d , %d, %s ", tErrLog.szEQP_ALID, tErrLog.szRecovery, tErrLog.szPart, tErrLog.szMessage, tErrLog.strOccurred_Time, tErrLog.nStop_Time, tErrLog.szUser_Info, tErrLog.nError_Level, tErrLog.nPosition, tErrLog.nDuplicate, tErrLog.nOnLine_OffLine, tErrLog.szLotNo_Info);
		strQuery.Replace("\r", NULL);
		strQuery.Replace("\\r", NULL);
		strQuery.Replace("\n", NULL);
		strQuery.Replace("\\n", NULL);

		m_Logger.LOG((LPSTR)(LPCTSTR)strQuery);
	}
	catch( CppSQLite3Exception& e )
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int     CDatabaseIF_Imp::INSERT_ERROR_LOG_TBL(_tERROR_LOG& tErrLog) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

int CDatabaseIF_Imp::INSERT_ERROR_LOG_TBL(CString strErrCode, CString a_strMessage, SYSTEMTIME a_tOccurredTime, CString a_strUser)
{
	_tERROR_MSG tErrMsg;
	SELECT_ERROR_MSG_TBL(strErrCode, tErrMsg);

	_tERROR_LOG tErrLog;
	tErrLog.SetErrLogFromErrMsg(tErrMsg, a_strMessage, a_tOccurredTime, a_strUser);

	INSERT_ERROR_LOG_TBL( tErrLog );

	return 0;
}

int CDatabaseIF_Imp::CREATE_ERROR_LOG_TBL(CString strDateYYYYMM)
{
	try
	{
		CString strQuery;
		strQuery.Format("CREATE TABLE TBL_ERROR_LOG_%s ("
			"[EQP_ALID] VARCHAR(16) NOT NULL, "
			"[PART] VARCHAR(16) NOT NULL, "
			"[MESSAGE] VARCHAR(255), "
			"[OCCURRED_TIME] TEXT NOT NULL, "
			"[RECOVERY] VARCHAR(16), "
			"[STOP_TIME] INT, "
			"[USER_INFO] VARCHAR(64), "
			"[ERROR_LEVEL] INT,"
			"[POSITION] INT,"
			"[DUPLICATE] INT,"
			"[ONLINE_OFFLINE] INT,"
			"[LOTNO] VARCHAR(255));", strDateYYYYMM );

		m_db.execDML( strQuery );		
 	}
	catch( CppSQLite3Exception& e )
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int     CDatabaseIF_Imp::CREATE_ERROR_LOG_TBL(CString strDateYYYYMM) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}	

	return 0;
}

int CDatabaseIF_Imp::CREATE_LOTHISTORY_TBL()
{
	try
	{
		CString strQuery;
		strQuery.Format("CREATE TABLE LOTHISTORY ("
			"[EQP_NO] VARCHAR(255), "
			"[PKG_Name] VARCHAR(255), "
			"[DeviceSize_X] DOUBLE, "
			"[DeviceSize_Y] DOUBLE, "
			"[Lot_No] VARCHAR(255), "
			"[Date] VARCHAR(255), "
			"[Stacker_Conveyor1] INT, "
			"[Stacker_Conveyor2] INT, "
			"[Stacker_Conveyor3] INT, "
			"[Stacker_Conveyor4] INT, "
			"[Stacker_Conveyor5] INT, "
			"[Stacker_Conveyor6] INT, "
			"[Stacker_Conveyor7] INT, "
			"[Load_Table1] INT, "
			"[Load_Table2] INT, "
			"[Load_Table3] INT, "
			"[Load_Table4] INT, "
			"[Tray_Load_PP] INT,"
			"[Tray_Unload_PP] INT, "
			"[Tray_Feeder] INT,"
			"[Clean_Buffer_Table1] INT,"
			"[Clean_Buffer_Table2] INT,"
			"[Test_PP1] INT,"
			"[Test_PP2] INT,"
			"[Station1] INT,"
			"[Station2] INT,"
			"[Station3] INT,"
			"[Station4] INT,"
			"[Station5] INT,"
			"[Station6] INT,"
			"[Station7] INT,"
			"[Station8] INT,"
			"[Transfer] INT,"
			"[Load_Device] INT,"
			"[Unload_Device] INT,"
			"[Retest1_Device] INT,"
			"[Retest2_Device] INT,"
			"[Pass_Bin] INT,"
			"[Fail_Bin] INT,"
			"[START_Time] VARCHAR(255), "
			"[ONLINE_OFFLINE] INT, "
			"[RUN_Time] VARCHAR(255), "
			"[UPH] INT, "
			"[AvgIndexTime] DOUBLE);");

		m_db.execDML(strQuery);
	}
	catch (CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int     CDatabaseIF_Imp::CREATE_LOTHISTORY_TBL() Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}

	return 0;
}

int CDatabaseIF_Imp::EXEC_SELECT_ERROR_LOG_QUERY( CString strDate_S, CString strDate_E, CMyList& rLogList, CString strQuery,BOOL bDaliyReport, BOOL bChk_OnLine_OffLine)
{
	m_csQuary.Lock();

	CppSQLite3Query q;
	
	try
	{
		q = m_db.execQuery( strQuery );

		while( !q.eof() )
		{
			_tERROR_LOG* pData = new _tERROR_LOG;

			strcpy_s( pData->szEQP_ALID, q.getStringField( 0 ) );
			strcpy_s( pData->szPart, q.getStringField( 1 ) );
			strcpy_s( pData->szMessage, q.getStringField( 2 ) );
			pData->strOccurred_Time	= q.getStringField( 3 );
			strcpy_s( pData->szRecovery, q.getStringField( 4 ) );
			pData->nStop_Time = q.getIntField( 5 );
			strcpy_s( pData->szUser_Info, q.getStringField( 6 ) );
			pData->nError_Level	= q.getIntField( 7 );	
			pData->nPosition = q.getIntField(8);
			pData->nDuplicate = q.getIntField(9);
			pData->nOnLine_OffLine = q.getIntField(10);

			if(bDaliyReport == TRUE && pData->nDuplicate > 0)
				continue;

			if(bChk_OnLine_OffLine == TRUE && pData->nOnLine_OffLine != 0) // OnLine Mode가 아니면
				continue;

			rLogList.m_fnAddData(pData);

			q.nextRow();
		}
	}
	catch( CppSQLite3Exception& e )
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int CDatabaseIF_Imp::SELECT_ERROR_LOG( CString strDate_S, CString strDate_E, CMyList& rLogList, CString strConditionType , CString strCondition ) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}	

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

//vector 용 Error Message
int CDatabaseIF_Imp::EXEC_SELECT_ERROR_LOG_QUERY(CString strDate_S, CString strDate_E, std::vector<ST_ERROR_MESSAGE>& vLogVector, CString strQuery, BOOL bDaliyReport, BOOL bChk_OnLine_OffLine)
{
	m_csQuary.Lock();

	CppSQLite3Query q;

	try
	{
		q = m_db.execQuery(strQuery);

		while (!q.eof())
		{
			ST_ERROR_MESSAGE pData;

			strcpy_s(pData.szEQP_ALID, q.getStringField(0));
			strcpy_s(pData.szPart, q.getStringField(1));
			strcpy_s(pData.szMessage, q.getStringField(2));
			pData.strOccurred_Time = q.getStringField(3);
			strcpy_s(pData.szRecovery, q.getStringField(4));
			pData.nStop_Time = q.getIntField(5);
			strcpy_s(pData.szUser_Info, q.getStringField(6));
			pData.nError_Level = q.getIntField(7);
			pData.nPosition = q.getIntField(8);
			pData.nDuplicate = q.getIntField(9);
			pData.nOnLine_OffLine = q.getIntField(10);
			strcpy_s(pData.szLotNo_Info, q.getStringField(11));

			vLogVector.push_back(pData);

			if (bDaliyReport == TRUE && pData.nDuplicate > 0)
			{
				if(vLogVector.size() >0)
					vLogVector.erase(vLogVector.end() - 1);
			}

			if (bChk_OnLine_OffLine == TRUE && pData.nOnLine_OffLine != 0)
			{
				if (vLogVector.size() > 0)
					vLogVector.erase(vLogVector.end() - 1);
			}
			
			q.nextRow();
		}
	}
	catch (CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int CDatabaseIF_Imp::SELECT_ERROR_LOG( CString strDate_S, CString strDate_E, CMyList& rLogList, CString strConditionType , CString strCondition ) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

CString CDatabaseIF_Imp::MAKE_SELECT_ERROR_LOG_QUERY( CString strDate_S, CString strDate_E, CString strConditionType, CString strCondition )
{
	m_csQuary.Lock();

	int nStartYYYY = atoi( strDate_S.Mid( 0, 4 ) );
	int nStartMM = atoi( strDate_S.Mid( 5, 2 ) );

	int nEndYYYY = atoi( strDate_E.Mid( 0, 4 ) );
	int nEndMM = atoi( strDate_E.Mid( 5, 2 ) );

	int nYearGap = nEndYYYY - nStartYYYY;
	int nMonthGap = nEndMM - nStartMM;

	int nMonthCount = ( nYearGap * 12 ) + nMonthGap; // 시작일자와 종료일자 사이의 달 간격 수.

	CString strStart_YYYYMM;
	strStart_YYYYMM.Format( "%d%02d", nStartYYYY, nStartMM );

	CString strEnd_YYYYMM;
	strEnd_YYYYMM.Format( "%d%02d", nEndYYYY, nEndMM );

	CString strQuery = "";
	CString strCondTemp = "";
	CString strTableName = "";

	if( strStart_YYYYMM == strEnd_YYYYMM ) // 시작 일자와 종료일자가 같을 경우.
	{ 
		strTableName.Format( _T("TBL_ERROR_LOG_%s"), strStart_YYYYMM );

		int nReslut = this->CHECK_TABLE( strTableName );
		if( nReslut != 0 )
		{
			this->CREATE_ERROR_LOG_TBL( strStart_YYYYMM );
		}

		strQuery.Format( _T("SELECT * FROM %s WHERE OCCURRED_TIME >= '%s' AND OCCURRED_TIME <= '%s'"), strTableName, strDate_S, strDate_E );

		if( strConditionType != "" ) //조건이 있을 경우 해당 조건문을 추가한다.
		{
			strCondTemp.Format( _T(" AND %s = '%s'"), strConditionType, strCondition );
			strQuery += strCondTemp;
		}
		strQuery += _T("order by OCCURRED_TIME Desc");
	}
	else
	{
		strTableName.Format( _T("TBL_ERROR_LOG_%s"), strStart_YYYYMM );

		int nReslut = this->CHECK_TABLE( strTableName );
		if( nReslut != 0 )
		{
			this->CREATE_ERROR_LOG_TBL( strStart_YYYYMM );
		}

		strQuery.Format( _T("SELECT * FROM %s WHERE OCCURRED_TIME >= '%s'"), strTableName, strDate_S );

		if (strConditionType != "") //조건이 있을 경우 해당 조건문을 추가한다.
		{
			strCondTemp.Format(_T(" AND %s = '%s'"), strConditionType, strCondition);
			strQuery += strCondTemp;
		}

		int nTempYear = nStartYYYY;
		int nTempMonth = nStartMM;

		CString strMidTableQuery =_T("");
		CString strMidTableDate = _T("");

		for( int i = 1; i < nMonthCount; i++ )
		{
			if( nTempMonth + 1 > 12 )
			{				
				nTempYear += 1;
				nTempMonth = 1;
			}
			else
			{
				nTempMonth += 1;
			}

			strMidTableDate.Format( _T("%d%02d"), nTempYear, nTempMonth );

			strTableName.Format( _T("TBL_ERROR_LOG_%s"), strMidTableDate );

			nReslut = this->CHECK_TABLE( strTableName );
			if( nReslut != 0 )
			{
				this->CREATE_ERROR_LOG_TBL( strMidTableDate ); 
			}

			strMidTableQuery += _T( " UNION SELECT * FROM " + strTableName );

			if( strConditionType != "" )
			{
				strCondTemp.Format( _T(" WHERE %s = '%s'"), strConditionType, strCondition );
				strMidTableQuery += strCondTemp;
			}
		}

		strTableName.Format( _T("TBL_ERROR_LOG_%s"), strEnd_YYYYMM );

		nReslut = this->CHECK_TABLE( strTableName );
		if( nReslut != 0 )
		{
			this->CREATE_ERROR_LOG_TBL( strEnd_YYYYMM );
		}

		CString strEndTableQuery;
		strEndTableQuery.Format( _T(" UNION SELECT * FROM %s WHERE OCCURRED_TIME <= '%s'"), strTableName, strDate_E );

		if( strConditionType != "" ) //조건이 있을 경우 해당 조건문을 추가한다.
		{
			strCondTemp.Format( _T(" AND %s = '%s'"), strConditionType, strCondition );
			strEndTableQuery += strCondTemp;
		}

		strQuery += strMidTableQuery + strEndTableQuery +_T("order by OCCURRED_TIME Desc");
	}

	m_csQuary.Unlock();

	return strQuery;
}

CString CDatabaseIF_Imp::MAKE_SELECT_LOTHISTORY_QUERY(CString strDate_S, CString strDate_E)
{
	m_csQuary.Lock();

	int nStartYYYY = atoi(strDate_S.Mid(0, 4));
	int nStartMM = atoi(strDate_S.Mid(5, 2));

	int nEndYYYY = atoi(strDate_E.Mid(0, 4));
	int nEndMM = atoi(strDate_E.Mid(5, 2));

	int nYearGap = nEndYYYY - nStartYYYY;
	int nMonthGap = nEndMM - nStartMM;

	int nMonthCount = (nYearGap * 12) + nMonthGap; // 시작일자와 종료일자 사이의 달 간격 수.

	CString strStart_YYYYMM;
	strStart_YYYYMM.Format("%d%02d", nStartYYYY, nStartMM);

	CString strEnd_YYYYMM;
	strEnd_YYYYMM.Format("%d%02d", nEndYYYY, nEndMM);

	CString strQuery = "";
	CString strCondTemp = "";
	CString strTableName = "";

	strTableName.Format(_T("LOTHISTORY"));

	int nReslut = this->CHECK_TABLE(strTableName);
	if (nReslut != 0)
	{
		this->CREATE_LOTHISTORY_TBL();
	}

	strQuery.Format(_T("SELECT * FROM %s WHERE Date >= '%s' AND Date <= '%s'"), strTableName, strDate_S, strDate_E);
	strQuery += _T("order by Date Desc");

	m_csQuary.Unlock();

	return strQuery;
}

int CDatabaseIF_Imp::EXEC_SELECT_LOTHISTORY_QUERY(CString strDate_S, CString strDate_E, std::vector<ST_LOTHISTORY>& vLogVector, CString strQuery)
{
	m_csQuary.Lock();

	CppSQLite3Query q;

	try
	{
		q = m_db.execQuery(strQuery);

		while (!q.eof())
		{
			ST_LOTHISTORY pData;

			strcpy_s(pData.szEQP_ID, q.getStringField(0));
			strcpy_s(pData.szPKGName, q.getStringField(1));
			pData.dDeviceSizeX = q.getFloatField(2);
			pData.dDeviceSizeY = q.getFloatField(3);
			strcpy_s(pData.szLotNo_Info, q.getStringField(4));

			pData.strLotEnd_Time = q.getStringField(5);

			for (int i = 0; i < 27; i++)
			{
				pData.nPartJamCnt[i] = q.getIntField(6+i);
			}
			pData.nLoadCnt = q.getIntField(33);
			pData.nUnloadCnt = q.getIntField(34);
			pData.nRetest[0] = q.getIntField(35);
			pData.nRetest[1] = q.getIntField(36);
			pData.nPassCnt = q.getIntField(37);
			pData.nFailCnt = q.getIntField(38);

			pData.strLotStart_Time = q.getStringField(39);
			pData.nOnLine_OffLine = q.getIntField(40);
			pData.strLotRunning_Time = q.getStringField(41);
			pData.nUPH = q.getIntField(42);
			pData.dAvgIndexTime = q.getFloatField(43);

			vLogVector.push_back(pData);

			q.nextRow();
		}
	}
	catch (CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int CDatabaseIF_Imp::EXEC_SELECT_LOTHISTORY_QUERY(CString strDate_S, CString strDate_E, std::vector<_tLOTHISTORY_LOG>& vLogVector, CString strQuery) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

int CDatabaseIF_Imp::SELECT_ERROR_MSG_TBL( std::map<int, std::shared_ptr<_tERROR_MSG>>& rErrMsgList )
{
	m_csQuary.Lock();

	CppSQLite3Query q;

	try
	{
		CString strQuery;
		strQuery.Format( "SELECT * FROM TBL_ERROR_MSG ORDER BY EQP_ALID" );
		q = m_db.execQuery( strQuery );

		while( !q.eof() )
		{
			//_tERROR_MSG* pData = new _tERROR_MSG;
			std::shared_ptr<_tERROR_MSG > pData = std::make_shared<_tERROR_MSG>();

			int nALID = _ttoi( q.getStringField( 0 ) );

			strcpy_s( pData->szEQP_ALID, q.getStringField( 0 ) );
			strcpy_s( pData->szPart, q.getStringField( 1 ) );
			CString s1 = q.getStringField( 2 );
			pData->strKorea = decodeUTF8( s1 );
			CString s2 = q.getStringField( 3 );
			pData->strEnglish = decodeUTF8( s2 );
			CString s3 = q.getStringField( 4 );
			pData->strChina = decodeUTF8( s3 );
			CString s4 = q.getStringField( 5 );
			pData->strJapan = decodeUTF8( s4 );
			pData->nError_Level = q.getIntField( 6 );
			strcpy_s( pData->szALID, q.getStringField( 7 ) );
			pData->nALCD = q.getIntField( 8 );
			pData->nALED = q.getIntField( 9 );
			pData->nMB_Type = q.getIntField( 10 );
			pData->nPosition = q.getIntField( 11 );

			rErrMsgList[ nALID ] = pData;
			q.nextRow();
		}
	}
	catch( CppSQLite3Exception& e )
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG( "int     CDatabaseIF_Imp::SELECT_ERROR_MSG_TBL(CMyList& rErrMsgList) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr );
	}

	q.finalize();
	m_csQuary.Unlock();


	return 0;
}

//------------------------------------------------------------------------------------------
// TBL_ERROR_MSG
int CDatabaseIF_Imp::SELECT_ERROR_MSG_TBL(CMyList& rErrMsgList)
{
	m_csQuary.Lock();

	CppSQLite3Query q;

	try
	{
		CString strQuery;
		strQuery.Format("SELECT * FROM TBL_ERROR_MSG ORDER BY EQP_ALID");
		q = m_db.execQuery( strQuery );

		while( !q.eof() )
		{
			_tERROR_MSG* pData = new _tERROR_MSG;

			strcpy_s( pData->szEQP_ALID, q.getStringField( 0 ) );
			strcpy_s( pData->szPart, q.getStringField( 1 ) );
			CString s1 = q.getStringField( 2 );
			pData->strKorea			= decodeUTF8(s1);
			CString s2 = q.getStringField( 3 );
			pData->strEnglish		= decodeUTF8(s2);
			CString s3 = q.getStringField( 4 );
			pData->strChina			= decodeUTF8(s3);
			CString s4 = q.getStringField( 5 );
			pData->strJapan			= decodeUTF8(s4);
			pData->nError_Level		= q.getIntField( 6 );
			strcpy_s( pData->szALID, q.getStringField( 7 ) );
			pData->nALCD			= q.getIntField( 8 );
			pData->nALED			= q.getIntField( 9 );	
			pData->nMB_Type     = q.getIntField( 10 );
			pData->nPosition	= q.getIntField(11);

			rErrMsgList.m_fnAddData( pData );
			q.nextRow();
		}
	}
	catch( CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int     CDatabaseIF_Imp::SELECT_ERROR_MSG_TBL(CMyList& rErrMsgList) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}	

	q.finalize();
	m_csQuary.Unlock();


	return 0;
}

int CDatabaseIF_Imp::SELECT_ERROR_MSG_TBL(CString strEqpALID, _tERROR_MSG& rErrMsg)
{
	m_csQuary.Lock();
	CppSQLite3Query q;

	try
	{
		CString strQuery;
		strQuery.Format(_T("SELECT * FROM TBL_ERROR_MSG WHERE EQP_ALID = '%s'"), strEqpALID);
		q = m_db.execQuery( strQuery );

		if( q.eof() == true )
		{
			q.finalize();
			m_csQuary.Unlock();
			return -1;
		}

		strcpy_s( rErrMsg.szEQP_ALID, q.getStringField( 0 ) );
		strcpy_s( rErrMsg.szPart, q.getStringField( 1 ) );
		CString s1 = q.getStringField( 2 );
		rErrMsg.strKorea			= decodeUTF8(s1);
		CString s2 = q.getStringField( 3 );
		rErrMsg.strEnglish		= decodeUTF8(s2);
		CString s3 = q.getStringField( 4 );
		rErrMsg.strChina			= decodeUTF8(s3);
		CString s4 = q.getStringField( 5 );
		rErrMsg.strJapan			= decodeUTF8(s4);
		rErrMsg.nError_Level	= q.getIntField( 6 );
		strcpy_s( rErrMsg.szALID, q.getStringField( 7 ) );
		rErrMsg.nALCD			= q.getIntField( 8 );
		rErrMsg.nALED			= q.getIntField( 9 );	
		rErrMsg.nMB_Type        = q.getIntField( 10 );
		rErrMsg.nPosition	    = q.getIntField( 11 );
	}
	catch( CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int CDatabaseIF_Imp::SELECT_ERROR_MSG_TBL(CString strEqpALID, _tERROR_MSG& rErrMsg) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}	

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

int CDatabaseIF_Imp::SELECT_ERROR_MSG_TBL(int nALID, _tERROR_MSG& rErrMsg)
{
	m_csQuary.Lock();
	CppSQLite3Query q;

	try
	{
		CString strQuery;
		strQuery.Format("SELECT * FROM TBL_ERROR_MSG WHERE ALID = '%d'", nALID);
		q = m_db.execQuery( strQuery );

		if( q.eof() == true )
		{
			q.finalize();
			m_csQuary.Unlock();
			return -1;
		}

		strcpy_s( rErrMsg.szEQP_ALID, q.getStringField( 0, "0000" ) );
		strcpy_s( rErrMsg.szPart, q.getStringField( 1, "-" ) );
		CString s1 = q.getStringField( 2 );
		rErrMsg.strKorea			= decodeUTF8(s1);
		CString s2 = q.getStringField( 3 );
		rErrMsg.strEnglish		= decodeUTF8(s2);
		CString s3 = q.getStringField( 4 );
		rErrMsg.strChina			= decodeUTF8(s3);
		CString s4 = q.getStringField( 5 );
		rErrMsg.strJapan			= decodeUTF8(s4);
		rErrMsg.nError_Level	= q.getIntField( 6 );
		strcpy_s( rErrMsg.szALID, q.getStringField( 7, "0" ) );
		rErrMsg.nALCD			= q.getIntField( 8 );
		rErrMsg.nALED			= q.getIntField( 9 );	
		rErrMsg.nMB_Type        = q.getIntField( 10 );	
		rErrMsg.nPosition		= q.getIntField( 11 );
	}
	catch( CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int CDatabaseIF_Imp::SELECT_ERROR_MSG_TBL(int nALID, _tERROR_MSG& rErrMsg) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}	

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}


int CDatabaseIF_Imp::INSERT_ERROR_MSG_TBL(CString strEqpALID, _tERROR_MSG& rErrMsg)
{
	m_csQuary.Lock();
	CppSQLite3Query q;

	try
	{
		CString strQuery;
		rErrMsg.strKorea = encodeUTF8(rErrMsg.strKorea);
		rErrMsg.strEnglish = encodeUTF8(rErrMsg.strEnglish);
		rErrMsg.strChina = encodeUTF8(rErrMsg.strChina);
		rErrMsg.strJapan = encodeUTF8(rErrMsg.strJapan);
		strQuery.Format(_T("INSERT INTO TBL_ERROR_MSG (EQP_ALID, PART, KOREA, ENGLISH, CHINA, JAPAN, ERROR_LEVEL, ALID, ALCD, ALED, MBTYPE, POSITION) VALUES ( '%s', '%s', '%s', '%s', '%s', '%s', %d, '%s', %d, %d, %d, %d )"), 
			strEqpALID, rErrMsg.szPart, rErrMsg.strKorea, rErrMsg.strEnglish, rErrMsg.strChina, rErrMsg.strJapan, rErrMsg.nError_Level, rErrMsg.szALID, rErrMsg.nALCD, rErrMsg.nALED, rErrMsg.nMB_Type, rErrMsg.nPosition );

		q = m_db.execQuery( strQuery );
		m_Logger.LOG("SQL [%s]", strQuery);
	}
	catch( CppSQLite3Exception& e )
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int     CDatabaseIF_Imp::INSERT_ERROR_MSG_TBL(CString strEqpALID, _tERROR_MSG& rErrMsg) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}	

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

int CDatabaseIF_Imp::INSERT_ERROR_MSG_TBL(int nALID, _tERROR_MSG& rErrMsg)
{
	m_csQuary.Lock();
	CppSQLite3Query q;

	try
	{
		CString strQuery;
		rErrMsg.strKorea = encodeUTF8(rErrMsg.strKorea);
		rErrMsg.strEnglish = encodeUTF8(rErrMsg.strEnglish);
		rErrMsg.strChina = encodeUTF8(rErrMsg.strChina);
		rErrMsg.strJapan = encodeUTF8(rErrMsg.strJapan);
		strQuery.Format("INSERT INTO TBL_ERROR_MSG (EQP_ALID, PART, KOREA, ENGLISH, CHINA, JAPAN, ERROR_LEVEL, ALID, ALCD, ALED, MBTYPE, POSITION ) "
			"VALUES ( '%s', '%s', '%s', '%s', '%s', '%s', %d, '%s', %d, %d, %d ,%d)", 
			rErrMsg.szEQP_ALID, 
			rErrMsg.szPart, 
			rErrMsg.strKorea, 
			rErrMsg.strEnglish, 
			rErrMsg.strChina, 
			rErrMsg.strJapan, 
			rErrMsg.nError_Level, 
			rErrMsg.szALID, 
			rErrMsg.nALCD, 
			rErrMsg.nALED, 
			rErrMsg.nMB_Type,
			rErrMsg.nPosition);

		q = m_db.execQuery( strQuery );
		m_Logger.LOG("SQL [%s]", strQuery);
	}
	catch( CppSQLite3Exception& e )
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int CDatabaseIF_Imp::INSERT_ERROR_MSG_TBL(int nALID, _tERROR_MSG& rErrMsg) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}	

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}


int CDatabaseIF_Imp::UPDATE_ERROR_MSG_TBL(CString strEqpALID, _tERROR_MSG& rErrMsg)
{
	_tERROR_MSG err_msg;
	int nErr = SELECT_ERROR_MSG_TBL(strEqpALID, err_msg);
	if( nErr != 0 ){
		INSERT_ERROR_MSG_TBL(strEqpALID, rErrMsg);
	}

	m_csQuary.Lock();
	CppSQLite3Query q;

	try
	{
		CString strQuery;
		rErrMsg.strKorea = encodeUTF8(rErrMsg.strKorea);
		rErrMsg.strEnglish = encodeUTF8(rErrMsg.strEnglish);
		rErrMsg.strChina = encodeUTF8(rErrMsg.strChina);
		rErrMsg.strJapan = encodeUTF8(rErrMsg.strJapan);
		strQuery.Format(_T("UPDATE TBL_ERROR_MSG SET PART = '%s', KOREA = '%s', ENGLISH = '%s', CHINA = '%s', JAPAN = '%s', ERROR_LEVEL = %d, ALID = '%s', ALCD = %d, ALED = %d, MBTYPE = %d, POSITION = %d WHERE EQP_ALID = '%s'"), 
			rErrMsg.szPart, rErrMsg.strKorea, rErrMsg.strEnglish, rErrMsg.strChina, rErrMsg.strJapan, rErrMsg.nError_Level, rErrMsg.szALID, rErrMsg.nALCD, rErrMsg.nALED, rErrMsg.nMB_Type, rErrMsg.nPosition, strEqpALID );

		q = m_db.execQuery( strQuery );
		m_Logger.LOG("SQL [%s]", strQuery);
	}
	catch( CppSQLite3Exception& e )
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int CDatabaseIF_Imp::UPDATE_ERROR_MSG_TBL(CString strEqpALID, _tERROR_MSG& rErrMsg) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}	

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

int CDatabaseIF_Imp::UPDATE_ERROR_MSG_TBL(int nALID, _tERROR_MSG& rErrMsg)
{
	_tERROR_MSG err_msg;
	int nErr = SELECT_ERROR_MSG_TBL(nALID, err_msg);
	if( nErr != 0 ){
		INSERT_ERROR_MSG_TBL(nALID, rErrMsg);
	}

	m_csQuary.Lock();
	CppSQLite3Query q;

	try
	{
		CString strQuery;
		rErrMsg.strKorea = encodeUTF8(rErrMsg.strKorea);
		rErrMsg.strEnglish = encodeUTF8(rErrMsg.strEnglish);
		rErrMsg.strChina = encodeUTF8(rErrMsg.strChina);
		rErrMsg.strJapan = encodeUTF8(rErrMsg.strJapan);
		strQuery.Format(_T("UPDATE TBL_ERROR_MSG SET PART = '%s', KOREA = '%s', ENGLISH = '%s', CHINA = '%s', JAPAN = '%s', ERROR_LEVEL = %d, ALID = '%s', ALCD = %d, ALED = %d, MBTYPE = %d, POSITION = %d WHERE ALID = %d"), 
			rErrMsg.szPart, rErrMsg.strKorea, rErrMsg.strEnglish, rErrMsg.strChina, rErrMsg.strJapan, rErrMsg.nError_Level, rErrMsg.szALID, rErrMsg.nALCD, rErrMsg.nALED, rErrMsg.nMB_Type, rErrMsg.nPosition, nALID );

		q = m_db.execQuery( strQuery );
		m_Logger.LOG("SQL [%s]", strQuery);
	}
	catch( CppSQLite3Exception& e )
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int CDatabaseIF_Imp::UPDATE_ERROR_MSG_TBL(int nALID, _tERROR_MSG& rErrMsg) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}	

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}




//TBL_TEACH_LOG
int CDatabaseIF_Imp::CREATE_TEACH_LOG_TBL(CString strDateYYYYMM)
{
	try
	{
		CString strQuery;
		strQuery.Format(_T("Create Table 'TBL_TEACH_LOG_%s' ('ROBOT' TEXT, 'POSITION' TEXT, 'AXIS' TEXT, 'BEFORE' DOUBLE, 'AFTER' DOUBLE,'OCCURRED_TIME' TEXT);"), strDateYYYYMM);

		m_db.execDML(strQuery);
	}
	catch (CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int     CDatabaseIF_Imp::CREATE_ERROR_LOG_TBL(CString strDateYYYYMM) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}

	return 0;
}

int CDatabaseIF_Imp::INSERT_TEACH_LOG_TBL(_tTEACH_LOG& tTeachLog)
{
	m_csQuary.Lock();

	CString strYYYYMM;
	strYYYYMM.Format(_T("%s%s"), tTeachLog.strOccurred_Time.Mid(0, 4), tTeachLog.strOccurred_Time.Mid(5, 2));

	CString strTableName;
	strTableName.Format(_T("TBL_TEACH_LOG_%s"), strYYYYMM);

	int nResult = this->CHECK_TABLE(strTableName);

	if (0 != nResult)
	{
		this->CREATE_TEACH_LOG_TBL(strYYYYMM);
	}

	//insert
	CppSQLite3Query q;

	try
	{
		CString strQuery;

		CString strRobotName = tTeachLog.szRobot_Name;
		CString strPosName = tTeachLog.szPos_Name;
		CString strAxisName = tTeachLog.szAxis_Name;
		double dPosition_Before = tTeachLog.dPosition_Before;
		double dPosition_After = tTeachLog.dPosition_After;

		// DB Data에 홑따옴표 "'" 가 들어가면 안된다 그래서 '' 쌍홀따옴표로 변경 [9/1/2020 donghyun.shin]
		strRobotName.Replace("'", "''");
		strPosName.Replace("'", "''");
		strAxisName.Replace("'", "''");

		strQuery.Format(_T("INSERT INTO %s ( ROBOT, POSITION, AXIS, BEFORE, AFTER, OCCURRED_TIME) VALUES ( '%s', '%s', '%s', '%f', '%f', '%s')"),
			strTableName, strRobotName, strPosName, strAxisName, dPosition_Before, dPosition_After, tTeachLog.strOccurred_Time);

		m_Logger.LOG((LPSTR)(LPCTSTR)strQuery);

		q = m_db.execQuery(strQuery);

		strQuery.Format("'%s', '%s', '%s', '%f', '%f', '%s' ", strRobotName, strPosName, strAxisName, dPosition_Before, dPosition_After, tTeachLog.strOccurred_Time);
		strQuery.Replace("\r", NULL);
		strQuery.Replace("\\r", NULL);
		strQuery.Replace("\n", NULL);
		strQuery.Replace("\\n", NULL);

		m_Logger.LOG((LPSTR)(LPCTSTR)strQuery);
	}
	catch (CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int     CDatabaseIF_Imp::INSERT_TEACH_LOG_TBL(_tERROR_LOG& tErrLog) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

CString CDatabaseIF_Imp::MAKE_SELECT_TEACH_LOG_QUERY(CString strDate_S, CString strDate_E, CString strConditionType, CString strCondition)
{
	m_csQuary.Lock();

	int nStartYYYY = atoi(strDate_S.Mid(0, 4));
	int nStartMM = atoi(strDate_S.Mid(5, 2));

	int nEndYYYY = atoi(strDate_E.Mid(0, 4));
	int nEndMM = atoi(strDate_E.Mid(5, 2));

	int nYearGap = nEndYYYY - nStartYYYY;
	int nMonthGap = nEndMM - nStartMM;

	int nMonthCount = (nYearGap * 12) + nMonthGap; // 시작일자와 종료일자 사이의 달 간격 수.

	CString strStart_YYYYMM;
	strStart_YYYYMM.Format("%d%02d", nStartYYYY, nStartMM);

	CString strEnd_YYYYMM;
	strEnd_YYYYMM.Format("%d%02d", nEndYYYY, nEndMM);

	CString strQuery = "";
	CString strCondTemp = "";
	CString strTableName = "";

	if (strStart_YYYYMM == strEnd_YYYYMM) // 시작 일자와 종료일자가 같을 경우.
	{
		strTableName.Format(_T("TBL_TEACH_LOG_%s"), strStart_YYYYMM);

		int nReslut = this->CHECK_TABLE(strTableName);
		if (nReslut != 0)
		{
			this->CREATE_TEACH_LOG_TBL(strStart_YYYYMM);
		}

		strQuery.Format(_T("SELECT * FROM %s WHERE OCCURRED_TIME >= '%s' AND OCCURRED_TIME <= '%s'"), strTableName, strDate_S, strDate_E);

		if (strConditionType != "") //조건이 있을 경우 해당 조건문을 추가한다.
		{
			strCondTemp.Format(_T(" AND %s = '%s'"), strConditionType, strCondition);
			strQuery += strCondTemp;
		}
		strQuery += _T("order by OCCURRED_TIME Desc");
	}
	else
	{
		strTableName.Format(_T("TBL_TEACH_LOG_%s"), strStart_YYYYMM);

		int nReslut = this->CHECK_TABLE(strTableName);
		if (nReslut != 0)
		{
			this->CREATE_TEACH_LOG_TBL(strStart_YYYYMM);
		}

		strQuery.Format(_T("SELECT * FROM %s WHERE OCCURRED_TIME >= '%s'"), strTableName, strDate_S);

		if (strConditionType != "") //조건이 있을 경우 해당 조건문을 추가한다.
		{
			strCondTemp.Format(_T(" AND %s = '%s'"), strConditionType, strCondition);
			strQuery += strCondTemp;
		}

		int nTempYear = nStartYYYY;
		int nTempMonth = nStartMM;

		CString strMidTableQuery = _T("");
		CString strMidTableDate = _T("");

		for (int i = 1; i < nMonthCount; i++)
		{
			if (nTempMonth + 1 > 12)
			{
				nTempYear += 1;
				nTempMonth = 1;
			}
			else
			{
				nTempMonth += 1;
			}

			strMidTableDate.Format(_T("%d%02d"), nTempYear, nTempMonth);

			strTableName.Format(_T("TBL_TEACH_LOG_%s"), strMidTableDate);

			nReslut = this->CHECK_TABLE(strTableName);
			if (nReslut != 0)
			{
				this->CREATE_TEACH_LOG_TBL(strMidTableDate);
			}

			strMidTableQuery += _T(" UNION SELECT * FROM " + strTableName);

			if (strConditionType != "")
			{
				strCondTemp.Format(_T(" WHERE %s = '%s'"), strConditionType, strCondition);
				strMidTableQuery += strCondTemp;
			}
		}

		strTableName.Format(_T("TBL_TEACH_LOG_%s"), strEnd_YYYYMM);

		nReslut = this->CHECK_TABLE(strTableName);
		if (nReslut != 0)
		{
			this->CREATE_TEACH_LOG_TBL(strEnd_YYYYMM);
		}

		CString strEndTableQuery;
		strEndTableQuery.Format(_T(" UNION SELECT * FROM %s WHERE OCCURRED_TIME <= '%s'"), strTableName, strDate_E);

		if (strConditionType != "") //조건이 있을 경우 해당 조건문을 추가한다.
		{
			strCondTemp.Format(_T(" AND %s = '%s'"), strConditionType, strCondition);
			strEndTableQuery += strCondTemp;
		}

		strQuery += strMidTableQuery + strEndTableQuery + _T("order by OCCURRED_TIME Desc");
	}

	m_csQuary.Unlock();

	return strQuery;
}

int CDatabaseIF_Imp::EXEC_SELECT_TEACH_LOG_QUERY(CString strDate_S, CString strDate_E, std::vector<ST_TEACH_LOG>& vLogTeachVector, CString strQuery)
{
	m_csQuary.Lock();

	CppSQLite3Query q;

	try
	{
		q = m_db.execQuery(strQuery);

		while (!q.eof())
		{
			ST_TEACH_LOG pData;

			strcpy_s(pData.szRobot_Name, q.getStringField(0));
			strcpy_s(pData.szPos_Name, q.getStringField(1));
			strcpy_s(pData.szAxis_Name, q.getStringField(2));
			pData.dPosition_Before = q.getFloatField(3);
			pData.dPosition_After = q.getFloatField(4);
			pData.strOccurred_Time = q.getStringField(5);

			vLogTeachVector.push_back(pData);

			q.nextRow();
		}
	}
	catch (CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int CDatabaseIF_Imp::SELECT_TEACH_LOG( CString strDate_S, CString strDate_E, CMyList& rLogList, CString strConditionType , CString strCondition ) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

int CDatabaseIF_Imp::SELECT_TEACH_LOG_TBL(CString strDate_S, CString strDate_E, std::vector<_ST_TEACH_LOG>& vLogTeachVector)
{
	CString strQuery = this->MAKE_SELECT_TEACH_LOG_QUERY(strDate_S, strDate_E);

	return this->EXEC_SELECT_TEACH_LOG_QUERY(strDate_S, strDate_E, vLogTeachVector, strQuery);
}

// TBL_MOTOR_TEACHING
int CDatabaseIF_Imp::SELECT_MOTOR_TEACHING_TBL( char* szCokName, CMyList& rTeachingList )
{
	m_csQuary.Lock();
	CppSQLite3Query q;

	try
	{
		CString strQuery;
		strQuery.Format("SELECT * FROM TBL_MOTOR_TEACHING WHERE COK_NAME = '%s' ORDER BY AXIS_NO, POS_ID", szCokName );
		q = m_db.execQuery( strQuery );

		while( !q.eof() )
		{
			_tMOTOR_TEACHING* pData = new _tMOTOR_TEACHING;

			strcpy_s( pData->szCOK_Name, q.getStringField( 0 ) );
			pData->nAxis_No				= q.getIntField( 1 ) ;
			pData->nPos_ID				= q.getIntField( 2 );
			strcpy_s( pData->szPos_Name,  q.getStringField( 3 ) );
			pData->dPosition			= q.getFloatField( 4 );
			pData->nSpeed_Rate			= q.getIntField( 5 );
			pData->nAcc_Rate			= q.getIntField( 6 );
			pData->nDec_Rate			= q.getIntField( 7 );
			pData->strDescript			= q.getStringField( 8 );			

			rTeachingList.m_fnAddData( pData );
			q.nextRow();
		}
	}
	catch( CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int CDatabaseIF_Imp::SELECT_MOTOR_TEACHING_TBL( char* szCokName, CMyList& rTeachingList ) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}	

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

int CDatabaseIF_Imp::SELECT_MOTOR_TEACHING_TBL( char* szCokName, int nAxisNo, CMyList& rTeachingList )
{	
	m_csQuary.Lock();
	CppSQLite3Query q;

	try
	{
		CString strQuery;
		strQuery.Format("SELECT * FROM TBL_MOTOR_TEACHING WHERE COK_NAME = '%s' AND AXIS_NO = %d ORDER BY POS_ID", szCokName, nAxisNo );
		q = m_db.execQuery( strQuery );

		while( !q.eof() )
		{
			_tMOTOR_TEACHING* pData = new _tMOTOR_TEACHING;

			strcpy_s( pData->szCOK_Name, q.getStringField( 0 ) );
			pData->nAxis_No				= q.getIntField( 1 ) ;
			pData->nPos_ID				= q.getIntField( 2 );
			strcpy_s( pData->szPos_Name,  q.getStringField( 3 ) );
			pData->dPosition			= q.getFloatField( 4 );
			pData->nSpeed_Rate			= q.getIntField( 5 );
			pData->nAcc_Rate			= q.getIntField( 6 );
			pData->nDec_Rate			= q.getIntField( 7 );
			pData->strDescript			= q.getStringField( 8 );			

			rTeachingList.m_fnAddData( pData );
			q.nextRow();
		}
	}
	catch( CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int CDatabaseIF_Imp::SELECT_MOTOR_TEACHING_TBL( char* szCokName, int nAxisNo, CMyList& rTeachingList ) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}	

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

int CDatabaseIF_Imp::INSERT_MOTOR_TEACHING_TBL( char* szCokName, CMyList& rTeaching )
{
	m_csQuary.Lock();
	CppSQLite3Query q;

 	try
 	{
 		CString strQuery;
 		
 		for( int i = 0; i < rTeaching.m_fnGetCount(); i++ )
 		{
 			_tMOTOR_TEACHING* pData = ( _tMOTOR_TEACHING* )rTeaching.m_fnGetData( i );
 			strQuery.Format(_T("INSERT INTO TBL_MOTOR_TEACHING ( COK_NAME, AXIS_NO, POS_ID, POS_NAME, POSITION, SPEED_RATE, ACC_RATE, DEC_RATE, DESCRIPT ) VALUES ( '%s', '%d', '%d', '%s', '%f', '%d', %d, '%d', '%s' )"), 
 				szCokName, pData->nAxis_No, pData->nPos_ID, pData->szPos_Name, pData->dPosition, pData->nSpeed_Rate, pData->nAcc_Rate, pData->nDec_Rate, pData->strDescript );
 
 			q = m_db.execQuery( strQuery );
			m_Logger.LOG("SQL [%s]", strQuery);
 		}
 	}
 	catch( CppSQLite3Exception& e )
 	{
 		DWORD dwErr = ::GetLastError();
 		m_Logger.LOG("int CDatabaseIF_Imp::INSERT_MOTOR_TEACHING_TBL( char* szCokName, CMyList& rTeaching ) Error, [DB ERR: %d = %s] [WIN ERR:%d] ", e.errorCode(), e.errorMessage(), dwErr);
 	}	

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

int CDatabaseIF_Imp::UPDATE_MOTOR_TEACHING_TBL( char* szCokName, int nAxisNo, int nPos, _tMOTOR_TEACHING& rTeaching )
{
	m_csQuary.Lock();
	CppSQLite3Query q;

 	try
 	{
 		CString strQuery;
 		strQuery.Format(_T("UPDATE TBL_MOTOR_TEACHING SET POS_ID = '%d', POS_NAME = '%s', POSITION = '%f', SPEED_RATE = '%d', ACC_RATE = '%d', DEC_RATE = %d, DESCRIPT = '%s' WHERE COK_NAME = '%s' AND AXIS_NO = %d"), 
 			rTeaching.nPos_ID, rTeaching.szPos_Name, rTeaching.dPosition, rTeaching.nSpeed_Rate, rTeaching.nAcc_Rate, rTeaching.nDec_Rate, rTeaching.strDescript, szCokName, nAxisNo );
 
 		q = m_db.execQuery( strQuery );
		m_Logger.LOG("SQL [%s]", strQuery);
 	}
 	catch( CppSQLite3Exception& e )
 	{
 		DWORD dwErr = ::GetLastError();
 		m_Logger.LOG("int CDatabaseIF_Imp::UPDATE_MOTOR_TEACHING_TBL( char* szCokName, int nAxisNo, int nPos, _tMOTOR_TEACHING& rTeaching ) Error, [DB ERR: %d = %s] [WIN ERR:%d] ", e.errorCode(), e.errorMessage(), dwErr);
 	}	

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

int CDatabaseIF_Imp::DELETE_MOTOR_TEACHING_TBL( char* szCokName )
{
	m_csQuary.Lock();
	CppSQLite3Query q;

	try
	{
		CString strQuery;
		strQuery.Format("DELETE FROM TBL_MOTOR_TEACHING WHERE COK_NAME = '%s'", szCokName);
		q = m_db.execQuery( strQuery );
	}
	catch( CppSQLite3Exception& e )
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int CDatabaseIF_Imp::DELETE_MOTOR_TEACHING_TBL( char* szCokName ) Error, [DB ERR: %d = %s] [WIN ERR:%d] ", e.errorCode(), e.errorMessage(), dwErr);
	}	

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

int	CDatabaseIF_Imp::SELECT_MOTION_GROUP_SETTING_TBL( int nGroupNo, CMyList& rSetting )
{
	m_csQuary.Lock();
	CppSQLite3Query q;

	try
	{
		CString strQuery;
		strQuery.Format("SELECT * FROM TBL_MOTION_GROUP_SETTING WHERE AXIS_GROUP_NO = %d", nGroupNo );
		q = m_db.execQuery( strQuery );

		while( !q.eof() )
		{
			_tMOTION_GROUP_SETTING* pData = new _tMOTION_GROUP_SETTING();

			pData->nAxisGroupNo = q.getIntField( 0 );
			pData->nOutputNo = q.getIntField( 1 );
			pData->nInputNo = q.getIntField( 2 );
			pData->strCylinderCaption = q.getStringField( 3 );
			pData->strCylinderOnCaption = q.getStringField( 4 );
			pData->nCylinderOnOutputNo = q.getIntField( 5 );
			pData->nCylinderOnInputNo = q.getIntField( 6 );
			pData->strCylinderOffCaption = q.getStringField( 7 );
			pData->nCylinderOffOutputNo = q.getIntField( 8 );
			pData->nCylinderOffInputNo = q.getIntField( 9 );
			pData->strOptionType = q.getStringField( 10 );
			pData->nOptionNo = q.getIntField( 11 );
			pData->strOptionCaption = q.getStringField( 12 );

			rSetting.m_fnAddData( pData );
			q.nextRow();
		}
	}
	catch( CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int	CDatabaseIF_Imp::SELECT_MOTION_GROUP_SETTING_TBL( int nGroupNo, CMyList& rSetting ) Error, [DB ERR: %d = %s] [WIN ERR:%d] ", e.errorCode(), e.errorMessage(), dwErr);
	}	

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

//////////////////////////////////////////////////////////// TBL_TAG
int CDatabaseIF_Imp::SELECT_TAG_TBL(int nTagId, _tTAG& rValue)
{
	m_csQuary.Lock();
	CppSQLite3Query q;

	try
	{
		CString strQuery;
		strQuery.Format("SELECT * FROM TBL_TAG WHERE TAG_ID = '%d'", nTagId);
		q = m_db.execQuery( strQuery );

		rValue.nTagID             = q.getIntField   ( 0 );
		rValue.nGroupID           = q.getIntField   ( 1 );
		rValue.nFormat            = q.getIntField   ( 2 );
		strcpy_s( rValue.szName   , q.getStringField( 3 ));
		strcpy_s( rValue.szValue  , q.getStringField( 4 ));
		strcpy_s( rValue.szMin    , q.getStringField( 5 ));
		strcpy_s( rValue.szMax    , q.getStringField( 6 ));
		rValue.nDefaultUse        = q.getIntField   ( 7 );
		strcpy_s( rValue.szDefault, q.getStringField( 8 ));
		strcpy_s( rValue.szUnit   , q.getStringField( 9 ));
		strcpy_s( rValue.szDesc   , q.getStringField( 10 ));
	}
	catch( CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int CDatabaseIF_Imp::SELECT_TAG_TBL(int nTagId, _tTAG& rValue) Error, [DB ERR: %d = %s] [WIN ERR:%d] ", e.errorCode(), e.errorMessage(), dwErr);
	}	

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

int CDatabaseIF_Imp::SELECT_TAG_TBL(int nGroupId, CMyList& rTagList)
{
	m_csQuary.Lock();
	CppSQLite3Query q;

	try
	{
		CString strQuery;
		strQuery.Format("SELECT * FROM TBL_TAG WHERE TAG_GROUP_ID = '%d' ORDER BY TAG_ID", nGroupId);
		q = m_db.execQuery( strQuery );

		while( !q.eof() )
		{
			_tTAG* pData = new _tTAG;

			pData->nTagID             = q.getIntField   ( 0 );
			pData->nGroupID           = q.getIntField   ( 1 );
			pData->nFormat            = q.getIntField   ( 2 );
			strcpy_s( pData->szName   , q.getStringField( 3 ));
			strcpy_s( pData->szValue  , q.getStringField( 4 ));
			strcpy_s( pData->szMin    , q.getStringField( 5 ));
			strcpy_s( pData->szMax    , q.getStringField( 6 ));
			pData->nDefaultUse        = q.getIntField   ( 7 );
			strcpy_s( pData->szDefault, q.getStringField( 8 ));
			strcpy_s( pData->szUnit   , q.getStringField( 9 ));
			strcpy_s( pData->szDesc   , q.getStringField( 10 ));

			rTagList.m_fnAddData( pData );
			q.nextRow();
		}
	}
	catch( CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int CDatabaseIF_Imp::SELECT_TAG_TBL(int nGroupId, CMyList& rTagList) Error, [DB ERR: %d = %s] [WIN ERR:%d] ", e.errorCode(), e.errorMessage(), dwErr);
	}	

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

int CDatabaseIF_Imp::SELECT_TAG_TBL(CMyList& rTagList)
{
	m_csQuary.Lock();
	CppSQLite3Query q;

	try
	{
		CString strQuery;
		strQuery.Format("SELECT * FROM TBL_TAG ORDER BY TAG_GROUP_ID, TAG_ID");
		q = m_db.execQuery( strQuery );

		while( !q.eof() )
		{
			_tTAG* pData = new _tTAG;

			pData->nTagID             = q.getIntField   ( 0 );
			pData->nGroupID           = q.getIntField   ( 1 );
			pData->nFormat            = q.getIntField   ( 2 );
			strcpy_s( pData->szName   , q.getStringField( 3 ));
			strcpy_s( pData->szValue  , q.getStringField( 4 ));
			strcpy_s( pData->szMin    , q.getStringField( 5 ));
			strcpy_s( pData->szMax    , q.getStringField( 6 ));
			pData->nDefaultUse        = q.getIntField   ( 7 );
			strcpy_s( pData->szDefault, q.getStringField( 8 ));
			strcpy_s( pData->szUnit   , q.getStringField( 9 ));
			strcpy_s( pData->szDesc   , q.getStringField( 10 ));

			rTagList.m_fnAddData( pData );
			q.nextRow();
		}
	}
	catch( CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int CDatabaseIF_Imp::SELECT_TAG_TBL(CMyList& rTagList) Error, [DB ERR: %d = %s] [WIN ERR:%d] ", e.errorCode(), e.errorMessage(), dwErr);
	}	

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

int CDatabaseIF_Imp::INSERT_TAG_TBL(int nTagId, _tTAG& rValue)
{
	m_csQuary.Lock();
	CppSQLite3Query q;

	try
	{
		CString strQuery;
		strQuery.Format("INSERT INTO TBL_TAG (TAG_ID, TAG_GROUP_ID, TAG_FORMAT, TAG_NAME, TAG_VALUE, TAG_MIN, TAG_MAX, TAG_DEFAULT_USE, TAG_DEFAULT, TAG_UNIT, TAG_DESCRIPT) \
			VALUES ( %d, %d, %d, '%s', '%s', '%s', '%s', %d, '%s', '%s', '%s')", 
			rValue.nTagID,
			rValue.nGroupID,
			rValue.nFormat,
			rValue.szName,
			rValue.szValue,
			rValue.szMin,
			rValue.szMax,
			rValue.nDefaultUse,
			rValue.szDefault,
			rValue.szUnit,
			rValue.szDesc);

		q = m_db.execQuery( strQuery );
		m_Logger.LOG("SQL [%s]", strQuery);
	}
	catch( CppSQLite3Exception& e )
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int CDatabaseIF_Imp::INSERT_TAG_TBL(int nTagId, _tTAG& rValue) Error, [DB ERR: %d = %s] [WIN ERR:%d] ", e.errorCode(), e.errorMessage(), dwErr);
	}	

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

int CDatabaseIF_Imp::UPDATE_TAG_TBL(int nTagId, _tTAG& rValue)
{
	m_csQuary.Lock();
	CppSQLite3Query q;

	try
	{
		CString strQuery;
		strQuery.Format("UPDATE TBL_TAG SET \
						TAG_ID         =  %d  , \
						TAG_GROUP_ID    =  %d  , \
						TAG_FORMAT      =  %d  , \
						TAG_NAME        = '%s' , \
						TAG_VALUE       = '%s' , \
						TAG_MIN         = '%s' , \
						TAG_MAX         = '%s' , \
						TAG_DEFAULT_USE =  %d  , \
						TAG_DEFAULT     = '%s' , \
						TAG_UNIT        = '%s' , \
						TAG_DESCRIPT    = '%s'   \
						WHERE TAG_ID    = '%d'", 
						rValue.nTagID,
						rValue.nGroupID,
						rValue.nFormat,
						rValue.szName,
						rValue.szValue,
						rValue.szMin,
						rValue.szMax,
						rValue.nDefaultUse,
						rValue.szDefault,
						rValue.szUnit,
						rValue.szDesc,
						nTagId);

		q = m_db.execQuery( strQuery );
		m_Logger.LOG("SQL [%s]", strQuery);
	}
	catch( CppSQLite3Exception& e )
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int CDatabaseIF_Imp::UPDATE_TAG_TBL(int nTagId, _tTAG& rValue) Error, [DB ERR: %d = %s] [WIN ERR:%d] ", e.errorCode(), e.errorMessage(), dwErr);
	}	

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}



//////////////////////////////////////////////////////////// TBL_TAG_GROUP
int CDatabaseIF_Imp::SELECT_TAG_GROUP(int nGroupId, _tTAG_GROUP& rValue)
{
	m_csQuary.Lock();
	CppSQLite3Query q;

	try
	{
		CString strQuery;
		strQuery.Format("SELECT * FROM TBL_TAG_GROUP WHERE ID = '%d'", nGroupId);
		q = m_db.execQuery( strQuery );

		rValue.nID                = q.getIntField   ( 0 );
		strcpy_s( rValue.szName   , q.getStringField( 1 ));
		rValue.nBaseAddr          = q.getIntField   ( 2 );
		strcpy_s( rValue.szDesc   , q.getStringField( 3 ));
	}
	catch( CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int CDatabaseIF_Imp::SELECT_TAG_GROUP(int nGroupId, _tTAG_GROUP& rValue) Error, [DB ERR: %d = %s] [WIN ERR:%d] ", e.errorCode(), e.errorMessage(), dwErr);
	}	

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

int CDatabaseIF_Imp::SELECT_TAG_GROUP(CMyList& rTagGroupList)
{
	m_csQuary.Lock();
	CppSQLite3Query q;

	try
	{
		CString strQuery;
		strQuery.Format("SELECT * FROM TBL_TAG_GROUP ORDER BY ID");
		q = m_db.execQuery( strQuery );

		while( !q.eof() )
		{
			_tTAG_GROUP* pData = new _tTAG_GROUP;

			pData->nID                = q.getIntField   ( 0 );
			strcpy_s( pData->szName   , q.getStringField( 1 ));
			pData->nBaseAddr          = q.getIntField   ( 2 );
			strcpy_s( pData->szDesc   , q.getStringField( 3 ));

			rTagGroupList.m_fnAddData( pData );
			q.nextRow();
		}
	}
	catch( CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int CDatabaseIF_Imp::SELECT_TAG_GROUP(CMyList& rTagGroupList) Error, [DB ERR: %d = %s] [WIN ERR:%d] ", e.errorCode(), e.errorMessage(), dwErr);
	}	

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

int CDatabaseIF_Imp::INSERT_TAG_GROUP(int nGroupId, _tTAG_GROUP& rValue)
{
	m_csQuary.Lock();
	CppSQLite3Query q;

	try
	{
		CString strQuery;
		strQuery.Format("INSERT INTO TBL_TAG_GROUP (ID, NAME, BASE_ADDR, DESCRIPTION) \
						VALUES ( %d, '%s', %d, '%s')", 
						rValue.nID,
						rValue.szName,
						rValue.nBaseAddr,
						rValue.szDesc);

		q = m_db.execQuery( strQuery );
		m_Logger.LOG("SQL [%s]", strQuery);
	}
	catch( CppSQLite3Exception& e )
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int CDatabaseIF_Imp::INSERT_TAG_GROUP(int nGroupId, _tTAG_GROUP& rValue) Error, [DB ERR: %d = %s] [WIN ERR:%d] ", e.errorCode(), e.errorMessage(), dwErr);
	}	

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

int CDatabaseIF_Imp::UPDATE_TAG_GROUP(int nGroupId, _tTAG_GROUP& rValue)
{
	m_csQuary.Lock();
	CppSQLite3Query q;

	try
	{
		CString strQuery;
		strQuery.Format("UPDATE TBL_TAG_GROUP SET \
						(ID         =  %d  , \
						NAME        = '%s' , \
						BASE_ADDR   =  %d  , \
						DESCRIPTION = '%s')", 
						rValue.nID,
						rValue.szName,
						rValue.nBaseAddr,
						rValue.szDesc);

		q = m_db.execQuery( strQuery );
		m_Logger.LOG("SQL [%s]", strQuery);
	}
	catch( CppSQLite3Exception& e )
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int CDatabaseIF_Imp::UPDATE_TAG_GROUP(int nGroupId, _tTAG_GROUP& rValue) Error, [DB ERR: %d = %s] [WIN ERR:%d] ", e.errorCode(), e.errorMessage(), dwErr);
	}	

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

int CDatabaseIF_Imp::SELECT_YIELD_RETEST_MANAGEMENT(int nNumber, _tYIELD_RETEST_MANAGEMENT& rValue)
{
	m_csQuary.Lock();
	CppSQLite3Query q;

	try
	{
		CString strQuery;
		strQuery.Format("SELECT * FROM TBL_YIELD_RETEST_MANAGEMENT WHERE NUMBER = %d", nNumber);
		q = m_db.execQuery( strQuery );

		if(q.eof() == true)
		{
			q.finalize();
			m_csQuary.Unlock();
			return -1;
		}

		strcpy_s( rValue.szBuildName, q.getStringField( 1 ) );
		strcpy_s( rValue.szGAIAVersion, q.getStringField( 2 ) );
		strcpy_s( rValue.szGRPTestVersion, q.getStringField( 3 ) );
		strcpy_s( rValue.szG2Version, q.getStringField( 4 ) );
		strcpy_s( rValue.szUIExploreVersion, q.getStringField( 5 ) );
		strcpy_s( rValue.szARMVersion, q.getStringField( 6 ) );
		strcpy_s( rValue.szServerVersion, q.getStringField( 7 ) );
		strcpy_s( rValue.szSerialNum, q.getStringField( 8 ) );
		rValue.nTesterID = q.getIntField( 9 );
		rValue.nJavisID = q.getIntField( 10 );
		rValue.nServerID = q.getIntField( 11 );
		strcpy_s( rValue.szServerIP, q.getStringField( 12 ) );
		rValue.nErrCode  = q.getIntField( 15 );
		strcpy_s( rValue.szErrStr, q.getStringField( 16 ) );
		strcpy_s( rValue.szLogFileName, q.getStringField( 17 ) );
	}
	catch( CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int CDatabaseIF_Imp::SELECT_YIELD_RETEST_MANAGEMENT(int nNumber, _tYIELD_RETEST_MANAGEMENT& rValue) Error, [DB ERR: %d = %s] [WIN ERR:%d] ", e.errorCode(), e.errorMessage(), dwErr);
	}	

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

int CDatabaseIF_Imp::INSERT_YIELD_RETEST_MANAGEMENT(_tYIELD_RETEST_MANAGEMENT& rValue)
{
	m_csQuary.Lock();

	CString strTableName;
	strTableName.Format( "TBL_YIELD_RETEST_MANAGEMENT" );  

 	int nResult = this->CHECK_TABLE( strTableName );

	if( 0 != nResult )
	{
		this->CREATE_YIELD_RETEST_MANAGEMENT();
	}

	// Start Time, Stop Time 포맷 맞지 않을 때
	if( (rValue.strStartTime[4] != '-' || rValue.strStartTime[7] != '-' ||
		rValue.strStartTime[10] != ' ' || rValue.strStartTime[13] != ':' ||
		rValue.strStartTime[16] != ':') ||
		(rValue.strStopTime[4] != '-' || rValue.strStopTime[7] != '-' ||
		rValue.strStopTime[10] != ' ' || rValue.strStopTime[13] != ':' ||
		rValue.strStopTime[16] != ':') ){
			m_csQuary.Unlock();
			return -1;
	}

	CppSQLite3Query q;

	try
	{
		CString strQuery;
		strQuery.Format("SELECT COUNT(*) from TBL_YIELD_RETEST_MANAGEMENT");
		q = m_db.execQuery( strQuery );
		int nCount = q.getIntField(0);
		if(nCount<= 0) { nCount = 0; }

		strQuery.Format("INSERT INTO TBL_YIELD_RETEST_MANAGEMENT( \
			NUMBER,\
			BUILD_NAME, \
			GAIA_VERSION, \
			GRPTEST_VERSION, \
			G2_VERSION, \
			UIEXPLORE_VERSION, \
			AMR_VERSION, \
			SERVER_VERSION, \
			SERIAL_NUMBER, \
			TESTER_ID, \
			JAVIS_ID, \
			SERVER_ID, \
			SERVER_IP, \
			START_TIME, \
			STOP_TIME, \
			ERROR_CODE, \
			ERROR_STRING, \
			LOG_FILE_NAME) \
			VALUES (%d, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', %u, %u, %u, '%s', '%s', '%s', %u, '%s', '%s');",
						nCount,
						rValue.szBuildName,
						rValue.szGAIAVersion,
						rValue.szGRPTestVersion,
						rValue.szG2Version,
						rValue.szUIExploreVersion,
						rValue.szARMVersion,
						rValue.szServerVersion,
						rValue.szSerialNum,
						rValue.nTesterID,
						rValue.nJavisID,
						rValue.nServerID,
						rValue.szServerIP,
						rValue.strStartTime,
						rValue.strStopTime,
						rValue.nErrCode,
						rValue.szErrStr,
						rValue.szLogFileName
						);

		q = m_db.execQuery( strQuery );
		m_Logger.LOG("SQL [%s]", strQuery);
	}
	catch( CppSQLite3Exception& e )
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int CDatabaseIF_Imp::INSERT_YIELD_RETEST_MANAGEMENT(_tYIELD_RETEST_MANAGEMENT& rValue) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}
int CDatabaseIF_Imp::UPDATE_YIELD_RETEST_MANAGEMENT(int nNumber, _tYIELD_RETEST_MANAGEMENT& rValue)
{
	m_csQuary.Lock();
	CppSQLite3Query q;

	try
	{
		CString strQuery;
		strQuery.Format("UPDATE TBL_YIELD_RETEST_MANAGEMENT SET \
						BUILD_NAME					 = '%s' , \
						GAIA_VERSION				 = '%s' , \
						GRPTEST_VERSION				 = '%s' , \
						G2_VERSION					 = '%s' , \
						UIEXPLORE_VERSION			 = '%s' , \
						AMR_VERSION					 = '%s' , \
						SERVER_VERSION				 = '%s' , \
						SERIAL_NUMBER				 = '%s' , \
						TESTER_ID					 =  %u  , \
						JAVIS_ID					 =  %u  , \
						SERVER_ID					 =  %u  , \
						SERVER_IP					 = '%s' , \
						ERROR_CODE					 =  %u  , \
						ERROR_STRING				 = '%s' , \
						LOG_FILE_NAME				 = '%s' \
						WHERE NUMBER = %d", 
						rValue.szBuildName,
						rValue.szGAIAVersion,
						rValue.szGRPTestVersion,
						rValue.szG2Version,
						rValue.szUIExploreVersion,
						rValue.szARMVersion,
						rValue.szServerVersion,
						rValue.szSerialNum,
						rValue.nTesterID,
						rValue.nJavisID,
						rValue.nServerID,
						rValue.szServerIP,
						rValue.nErrCode,
						rValue.szErrStr,
						rValue.szLogFileName,
						nNumber
						);

		q = m_db.execQuery( strQuery );
		m_Logger.LOG("SQL [%s]", strQuery);
	}
	catch( CppSQLite3Exception& e )
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int CDatabaseIF_Imp::UPDATE_YIELD_RETEST_MANAGEMENT(int nNumber, _tYIELD_RETEST_MANAGEMENT& rValue) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}	

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

int CDatabaseIF_Imp::DELETE_YIELD_RETEST_MANAGEMENT(int nNumber)
{
	m_csQuary.Lock();
	CppSQLite3Query q;

	try
	{
		CString strQuery;
		strQuery.Format("DELETE FROM TBL_YIELD_RETEST_MANAGEMENT WHERE NUMBER = %d", nNumber);
		q = m_db.execQuery( strQuery );
	}
	catch( CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int	CDatabaseIF::DELETE_YIELD_RETEST_MANAGEMENT(int nNumber) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}	

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

int CDatabaseIF_Imp::CREATE_YIELD_RETEST_MANAGEMENT()
{
	//m_db.execDML("begin transaction;");
	try
	{
		CString strQuery;
		strQuery.Format(
			"CREATE TABLE [TBL_YIELD_RETEST_MANAGEMENT] (  \
			[NUMBER] UNSIGNED INT(8) UNIQUE,               \
			[BUILD_NAME] VARCHAR(32), 				       \
			[GAIA_VERSION] VARCHAR(128), 			       \
			[GRPTEST_VERSION] VARCHAR(128), 		       \
			[G2_VERSION] VARCHAR(128), 				       \
			[UIEXPLORE_VERSION] VARCHAR(128), 		       \
			[AMR_VERSION] VARCHAR(128), 			       \
			[SERVER_VERSION] VARCHAR(128), 			       \
			[SERIAL_NUMBER] VARCHAR(256), 			       \
			[TESTER_ID] UNSIGNED INT(4), 			       \
			[JAVIS_ID] UNSIGNED INT(4), 				   \
			[SERVER_ID] UNSIGNED INT(4), 			       \
			[SERVER_IP] VARCHAR(256), 				       \
			[START_TIME] [DATE TIME] UNIQUE,		       \
			[STOP_TIME] [DATE TIME], 				       \
			[ERROR_CODE] UNSIGNED INT(2), 			       \
			[ERROR_STRING] VARCHAR(256), 			       \
			[LOG_FILE_NAME] VARCHAR(4096) );");

		m_db.execDML( strQuery );		
	}
	catch( CppSQLite3Exception& e )
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int CDatabaseIF_Imp::CREATE_YIELD_RETEST_MANAGEMENT() Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}	

	//m_db.execDML("commit transaction;");

	return 0;
}

//-------------------------------------------------------------------------------- TBL_AIO
int CDatabaseIF_Imp::SELECT_AIO_TBL(int nDir, int nIndex, _tAIO_LS& rValue)
{
	m_csQuary.Lock();
	CppSQLite3Query q;

	try
	{
		CString strQuery;
		strQuery.Format("SELECT * FROM TBL_AIO WHERE AIO_DIR = %d ORDER BY AIO_IDX_LOGICAL", nDir );
		q = m_db.execQuery( strQuery );
		if( !q.eof() )
		{
			rValue.nDir          = q.getIntField   (0);
			rValue.nIdx_Logical  = q.getIntField   (1);
			rValue.nIdx_Physical = q.getIntField   (2);
			rValue.strName       = q.getStringField(3);
			rValue.strLabel      = q.getStringField(4);
			rValue.dMinVolt      = q.getFloatField (5);
			rValue.dMaxVolt      = q.getFloatField (6);
			rValue.dMinPhyVal    = q.getFloatField (7);
			rValue.dMaxPhyVal    = q.getFloatField (8);
			rValue.strUnit       = q.getStringField(9);
			rValue.strDesc       = q.getStringField(10);
		}
	}
	catch( CppSQLite3Exception& e )
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int CDatabaseIF_Imp::SELECT_AIO_TBL(int nDir, int nIndex, _tAIO_LS& rValue) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}	

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

int CDatabaseIF_Imp::SELECT_AIO_TBL(int nDir, CMyList& rDataList)
{
	m_csQuary.Lock();
	CppSQLite3Query q;

	try
	{
		CString strQuery;
		strQuery.Format("SELECT * FROM TBL_AIO WHERE AIO_DIR = %d ORDER BY AIO_IDX_LOGICAL", nDir );
		q = m_db.execQuery( strQuery );
		while( !q.eof() )
		{
			_tAIO_LS* pRow = new _tAIO_LS;

			pRow->nDir          = q.getIntField   (0);
			pRow->nIdx_Logical  = q.getIntField   (1);
			pRow->nIdx_Physical = q.getIntField   (2);
			pRow->strName       = q.getStringField(3);
			pRow->strLabel      = q.getStringField(4);
			pRow->dMinVolt      = q.getFloatField (5);
			pRow->dMaxVolt      = q.getFloatField (6);
			pRow->dMinPhyVal    = q.getFloatField (7);
			pRow->dMaxPhyVal    = q.getFloatField (8);
			pRow->strUnit       = q.getStringField(9);
			pRow->strDesc       = q.getStringField(10);

			rDataList.m_fnAddData(pRow);
			q.nextRow();
		}
	}
	catch( CppSQLite3Exception& e )
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int CDatabaseIF_Imp::SELECT_AIO_TBL(int nDir, CMyList& rDataList) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}	

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

int CDatabaseIF_Imp::SELECT_AIO_TBL(int nDir, int nFrmIdx, int nIdxCount, CMyList& rDataList)
{
	m_csQuary.Lock();
	CppSQLite3Query q;

	try
	{
		CString strQuery;
		strQuery.Format("SELECT * FROM TBL_AIO WHERE AIO_DIR = %d AND AIO_IDX_LOGICAL >= %d AND AIO_IDX_LOGICAL < %d ORDER BY AIO_IDX_LOGICAL", nDir, nFrmIdx, nFrmIdx + nIdxCount );
		q = m_db.execQuery( strQuery );
		while( !q.eof() )
		{
			_tAIO_LS* pRow = new _tAIO_LS;

			pRow->nDir          = q.getIntField   (0);
			pRow->nIdx_Logical  = q.getIntField   (1);
			pRow->nIdx_Physical = q.getIntField   (2);
			pRow->strName       = q.getStringField(3);
			pRow->strLabel      = q.getStringField(4);
			pRow->dMinVolt      = q.getFloatField (5);
			pRow->dMaxVolt      = q.getFloatField (6);
			pRow->dMinPhyVal    = q.getFloatField (7);
			pRow->dMaxPhyVal    = q.getFloatField (8);
			pRow->strUnit       = q.getStringField(9);
			pRow->strDesc       = q.getStringField(10);

			rDataList.m_fnAddData(pRow);
			q.nextRow();
		}
	}
	catch( CppSQLite3Exception& e )
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int CDatabaseIF_Imp::SELECT_AIO_TBL(int nDir, int nFrmIdx, int nIdxCount, CMyList& rDataList) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}	

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}


int CDatabaseIF_Imp::UPDATE_AIO_TBL(int nDir, int nIndex, _tAIO_LS& rValue)
{
	CMyList Prefind;
	SELECT_AIO_TBL(nDir, nIndex, 1, Prefind);
	if( Prefind.m_fnGetCount() == 0 ){
		INSERT_AIO_TBL(nDir, nIndex, rValue);
	}else{
		m_csQuary.Lock();
		CppSQLite3Query q;

		try
		{
			CString strQuery;
			strQuery.Format("UPDATE TBL_AIO SET       \
							(AIO_DIR          =  %d , \
							AIO_IDX_LOGICAL   =  %d , \
							AIO_IDX_PHYSICAL  =  %d , \
							AIO_NAME          = '%s', \
							AIO_LABEL         = '%s', \
							AIO_MIN_VOLT      =  %f , \
							AIO_MAX_VOLT      =  %f , \
							AIO_MIN_PHY_VAL   =  %f , \
							AIO_MAX_PHY_VAL   =  %f , \
							AIO_UNIT          = '%s', \
							AIO_DESCRIPT      = '%s',)",
							rValue.nDir,
							rValue.nIdx_Logical,
							rValue.nIdx_Physical,
							rValue.strName,
							rValue.strLabel,
							rValue.dMinVolt,
							rValue.dMaxVolt,
							rValue.dMinPhyVal,
							rValue.dMaxPhyVal,
							rValue.strUnit,
							rValue.strDesc);

			q = m_db.execQuery( strQuery );
			m_Logger.LOG("SQL [%s]", strQuery);
		}
		catch( CppSQLite3Exception& e )
		{
			DWORD dwErr = ::GetLastError();
			m_Logger.LOG("int CDatabaseIF_Imp::UPDATE_AIO_TBL(int nDir, int nIndex, _tAIO_LS& rValue) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
		}	

		q.finalize();
		m_csQuary.Unlock();
	}
	return 0;
}

int CDatabaseIF_Imp::INSERT_AIO_TBL(int nDir, int nIndex, _tAIO_LS& rValue)
{
	m_csQuary.Lock();
	CppSQLite3Query q;

	try
	{
		CString strQuery;
		strQuery.Format("INSERT INTO TBL_AIO (AIO_DIR, AIO_IDX_LOGICAL, AIO_IDX_PHYSICAL, AIO_NAME, AIO_LABEL, AIO_MIN_VOLT, AIO_MAX_VOLT, AIO_MIN_PHY_VAL, AIO_MAX_PHY_VAL, AIO_UNIT, AIO_DESCRIPT) \
						VALUES ( %d, %d, %d, '%s', '%s', %f, %f, %f, %f, '%s', '%s')", 
						rValue.nDir,
						rValue.nIdx_Logical,
						rValue.nIdx_Physical,
						rValue.strName,
						rValue.strLabel,
						rValue.dMinVolt,
						rValue.dMaxVolt,
						rValue.dMinPhyVal,
						rValue.dMaxPhyVal,
						rValue.strUnit,
						rValue.strDesc);

		q = m_db.execQuery( strQuery );
		m_Logger.LOG("SQL [%s]", strQuery);
	}
	catch( CppSQLite3Exception& e )
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int CDatabaseIF_Imp::INSERT_AIO_TBL(int nDir, int nIndex, _tAIO_LS& rValue) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}	
	q.finalize();
	m_csQuary.Unlock();

	return 0;
}


CString CDatabaseIF_Imp::decodeUTF8(CString &encoded)
{
	CArray<WCHAR,WCHAR> wc;
	INT_PTR n = ::MultiByteToWideChar(CP_UTF8,0,(LPCTSTR)encoded,(int)encoded.GetLength(),NULL,0);
	wc.SetSize(n);
	n = ::MultiByteToWideChar(CP_UTF8,0,(LPCSTR)encoded,(int)encoded.GetLength(),(LPWSTR)wc.GetData(),(int)n);
	n = ::WideCharToMultiByte(CP_ACP,0,(LPCWSTR)wc.GetData(),(int)wc.GetSize(),NULL,0,NULL,NULL);
	CString conv; 
	LPSTR p = conv.GetBuffer((int)n);
	n = ::WideCharToMultiByte(CP_ACP,0,wc.GetData(),(int)wc.GetSize(),p,(int)n,NULL,NULL);

	CString Msg_Return = conv.Left(n);
	conv.ReleaseBuffer();

	return Msg_Return;
}

CString CDatabaseIF_Imp::encodeUTF8(CString &dcoded)
{
	// 멀티바이트 -> 유니코드
	// by hyb [140925] 메시지 내용 길경우 디버그 알람 발생 관련 수정 start
	wchar_t strUni[MAX_BUFF] = {0,};
	char    strMultibyte[MAX_BUFF] = {0,};
	strcpy_s(strMultibyte, MAX_BUFF, dcoded);
	int nLen = MultiByteToWideChar(CP_ACP, 0, strMultibyte, strlen(strMultibyte), NULL, NULL);
	MultiByteToWideChar(CP_ACP, 0, strMultibyte, strlen(strMultibyte), strUni, nLen);

	// 유니코드 -> UTF-8
	char strUtf8[MAX_BUFF] ={0,};
	nLen = WideCharToMultiByte(CP_UTF8, 0, strUni, lstrlenW(strUni), NULL, 0, NULL, NULL);
	WideCharToMultiByte (CP_UTF8, 0, strUni, lstrlenW(strUni), strUtf8, nLen, NULL, NULL);
	// by hyb [140925] 메시지 내용 길경우 디버그 알람 발생 관련 수정 end
	return strUtf8;
}

int CDatabaseIF_Imp::INSERT_ADD_COLUMN(CString strAddColumn, CString strAddColumnType, CString strSelTable)
{
	m_csQuary.Lock();
	CppSQLite3Query q;

	CString strTableName = "";

	CString strColumnName[100] = { "", };

	try
	{
		CString strSql = "";
		strSql.Format("SELECT tbl_name FROM sqlite_master");
		q = m_db.execQuery(strSql);
		
		while(!q.eof())
		{
			strTableName = q.getStringField(0);

			if (strSelTable != "ALL" && strSelTable != strTableName)
			{
				q.nextRow();
				continue;
			}

			CppSQLite3Query q1;
			BOOL bAddColFlag = FALSE;

			strSql.Format("PRAGMA table_info(%s)",strTableName);
			q1 = m_db.execQuery(strSql); // 해당 Table 따로 확인

			while (!q1.eof())
			{
				if (q1.getStringField(1) == strAddColumn) //추가하려는 Column과 같은 이름이 있는지 확인
				{
					bAddColFlag = TRUE;
					break;
				}
				q1.nextRow();
			}

			if (bAddColFlag == FALSE)
			{
				strSql.Format("ALTER TABLE %s ADD COLUMN %s %s ", strTableName, strAddColumn, strAddColumnType);
				m_db.execDML(strSql);
			}
			
			q1.finalize();

			q.nextRow();
		}
	}
	catch (CppSQLite3Exception& e)
	{
		CString err;
		DWORD dwErr = ::GetLastError();
		err.Format("CSLT_MCView::INSERT_ADD_COLUMN() %d : %s, system error=%lu", e.errorCode(), e.errorMessage(), dwErr);
	}

	q.finalize();
	m_csQuary.Unlock();
	return 0;
}

int	CDatabaseIF_Imp::SELECT_BARCODE_2D_ALL(std::vector<ST_2D_BARCODE_INFO> &vec)
{
	m_csQuary.Lock();
	CppSQLite3Query q;
	try
	{
		q = m_db.execQuery( _T("SELECT * FROM BARCODE_2D;") );
		ST_2D_BARCODE_INFO data;
		while(!q.eof())
		{			
			data.clear();
			if( q.getStringField(1) != NULL){
				sprintf_s(data.szReadTime, sizeof(data.szReadTime), "%s", q.getStringField(1));
			}
			data.nIndexArmNo = q.getIntField(2);
			data.nSiteNo = q.getIntField(3);
			if( q.getStringField(4) != NULL){
				strcpy(data.szBarcode, q.getStringField(4));
			}
			vec.push_back(data);
			q.nextRow();
		}
	}
	catch(CppSQLite3Exception& e)
	{
		CString err;
		DWORD dwErr = ::GetLastError();
		err.Format("CVision_2dBarcode::SELECT_BARCODE_2D_ALL() %d : %s, system error=%lu", e.errorCode(), e.errorMessage(), dwErr );
	}

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

int CDatabaseIF_Imp::INSERT_BARCODE_2D(ST_2D_BARCODE_INFO& _data)
{
	m_csQuary.Lock();

	int nResult = this->CHECK_TABLE( _T("BARCODE_2D") );

	if( 0 != nResult )
	{
		CppSQLite3Query q1 = m_db.execQuery( _T("CREATE TABLE BARCODE_2D ([id] INTEGER PRIMARY KEY, [read_time] DATE_TIME, [index_arm_no] INTEGER, [site_no] INTEGER, [barcode] CHAR(128), [barcode_bak] CHAR(128), [teststart_time] DATE_TIME, [testend_time] DATE_TIME, [result_bin] INTEGER)") );
		q1.finalize();
	}

	CppSQLite3Query q;
	try{
		CString strSql		= "";
		strSql.Format(_T("SELECT * FROM BARCODE_2D WHERE barcode='%s';"), _data.szBarcode);
		q =  m_db.execQuery(strSql);
		CString strBarcode = _data.szBarcode;
		if(	!q.eof() )
		{
			q.finalize();
			ASSERT(0);
		}
		else
		{
			CString strTime		= "";
			strTime.Format(_T("%s"), _data.szReadTime);
			strSql.Format(_T("INSERT INTO BARCODE_2D (read_time, index_arm_no, site_no, barcode) VALUES('%s', '%d', '%d', '%s');"), 
				strTime, _data.nIndexArmNo, _data.nSiteNo, _data.szBarcode);
			q = m_db.execQuery( strSql );
			strSql.Empty();
		}
	}
	catch(CppSQLite3Exception& e)
	{
		CString err;
		DWORD dwErr = ::GetLastError();
		err.Format("CVision_2dBarcode::INSERT_BARCODE_2D() %d : %s, system error=%lu", e.errorCode(), e.errorMessage(), dwErr );
	}

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

int	CDatabaseIF_Imp::DELETE_BARCODE_2D(const char* sz2DID, const char* szReadTime)
{
	m_csQuary.Lock();

	int nResult = this->CHECK_TABLE( _T("BARCODE_2D") );
	if( 0 != nResult )
	{
		CppSQLite3Query q1 = m_db.execQuery( _T("CREATE TABLE BARCODE_2D ([id] INTEGER PRIMARY KEY, [read_time] DATE_TIME, [index_arm_no] INTEGER, [site_no] INTEGER, [barcode] CHAR(128), [barcode_bak] CHAR(128), [teststart_time] DATE_TIME, [testend_time] DATE_TIME, [result_bin] INTEGER)") );
		q1.finalize();
	}

	CppSQLite3Query q;
	try
	{
		CString strQuery;
		strQuery.Format("DELETE FROM BARCODE_2D WHERE barcode = '%s' AND read_time='%s';", sz2DID, szReadTime);
		q = m_db.execQuery( strQuery );
	}
	catch( CppSQLite3Exception& e)
	{
		CString err;
		DWORD dwErr = ::GetLastError();
		err.Format("CVision_2dBarcode::DELETE_BARCODE_2D() %d : %s, system error=%lu", e.errorCode(), e.errorMessage(), dwErr );
	}
	q.finalize();

	m_csQuary.Unlock();

	return 0;
}

void CDatabaseIF_Imp::DELETE_BARCODE_2D_ALL()
{
	m_csQuary.Lock();

	int nResult = this->CHECK_TABLE( _T("BARCODE_2D") );

	if( 0 == nResult){
		CppSQLite3Query q;
		q = m_db.execQuery( _T("DELETE FROM BARCODE_2D") );
		q.finalize();
	}
	m_csQuary.Unlock();
}

// add by SJI [160919] Add AAB Retest Rule Check
int CDatabaseIF_Imp::CREATE_DEVICE_DATA_TBL(CString strTableName)
{
	try
	{
		CString strQuery;
		
		strQuery.Format(_T("CREATE TABLE %s([barcode] CHAR(128) PRIMARY KEY, [site_no] CHAR(128), [error_str] CHAR(128),[error_code] CHAR(128), [testing_cnt] INTEGER,[result_bin] CHAR(128), [remark] CHAR(256));"), strTableName );
	
		m_db.execDML( strQuery );		
	}
	catch( CppSQLite3Exception& e )
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int     CDatabaseIF_Imp::CREATE_DEVICE_DATA_TBL() Error, [DB ERR: %d = %s] [WIN ERR:%ld] ", e.errorCode(), e.errorMessage(), dwErr);
	}	
	return 0;
}

// add by SJI [160919] Add AAB Retest Rule Check
int CDatabaseIF_Imp::INSERT_DEVICE_DATA_TBL(int nTableNum, char *szBarcode, char *szSiteNo, char *szErrorStr/*, char *szErrorCode*/, int nTestingCnt, char *szResultBin, CString strRemark)
{
	m_csQuary.Lock();

	CppSQLite3Query q;
	try
	{
		CString strTableName = _T("");

		if(nTableNum == eDB_AAB_TYPE_TEMP){
			strTableName =_T("TBL_DEVICE_DATA");
		}
		else if (nTableNum == eDB_AAB_TYPE_FAIL){
			strTableName =_T("TBL_VIOLATION_DATA");
		}

		int nResult= this->CHECK_TABLE(strTableName);
		if(nResult != 0)
		{
			this->CREATE_DEVICE_DATA_TBL(strTableName);
		}
		CString strQuery;
		
		strQuery.Format(_T("INSERT INTO %s (barcode, site_no, error_str, error_code, testing_cnt, result_bin, remark) VALUES ('%s', '%s', '%s', %d, '%s', '%s')"),strTableName, szBarcode, szSiteNo, szErrorStr/*, szErrorCode*/, nTestingCnt, szResultBin, strRemark);
		q = m_db.execQuery( strQuery );
		m_Logger.LOG("SQL [%s]", strQuery);
	}
	catch( CppSQLite3Exception& e )
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int CDatabaseIF_Imp::INSERT_DEVICE_DATA_TBL(int nSiteNo, CString strBarcode, CString strErrorStr, CString strErrorCode, int nTestingCnt, int nResultBin) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}	
	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

// add by SJI [160919] Add AAB Retest Rule Check
int CDatabaseIF_Imp::SELECT_DEVICE_DATA_TBL(int nTableNum, std::deque<ST_AAB_CHECK_DATA> &qDvcData)
{
	m_csQuary.Lock();
	CppSQLite3Query q;
	try
	{
		CString strTableName = _T("");
		CString strSql       = _T("");

		if(nTableNum == eDB_AAB_TYPE_TEMP){
			strTableName = _T("TBL_DEVICE_DATA");
		}
		else if (nTableNum == eDB_AAB_TYPE_FAIL){
			strTableName =_T("TBL_VIOLATION_DATA");
		}

		strSql.Format(_T("SELECT * FROM %s"),strTableName);

		q = m_db.execQuery(strSql);
		
		ST_AAB_CHECK_DATA data;

		while(!q.eof())
		{			
			data.clear();

			sprintf_s(data.szBarcode, sizeof(data.szBarcode), "%s", q.getStringField(0));
			sprintf_s(data.szSiteNo, sizeof(data.szSiteNo), "%s", q.getStringField(1));
			//sprintf_s(data.szErrorCode, sizeof(data.szErrorCode), "%s", q.getStringField(2));
			sprintf_s(data.szErrorStr, sizeof(data.szErrorStr), "%s", q.getStringField(3));
			data.nTestingCnt = q.getIntField(4);
			sprintf_s(data.szBin, sizeof(data.szBin), "%s", q.getStringField(5));
			data.strRemark = q.getStringField(6);
			
			qDvcData.push_back(data);
			q.nextRow();
		}
	}
	catch(CppSQLite3Exception& e)
	{
		CString err;
		DWORD dwErr = ::GetLastError();
		err.Format("CDatabaseIF_Imp::SELECT_DEVICE_DATA_TBL() %d : %s, system error=%lu", e.errorCode(), e.errorMessage(), dwErr );
	}
	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

// add by SJI [160919] Add AAB Retest Rule Check
int	CDatabaseIF_Imp::DELETE_DEVICE_DATA_TBL(int nTableNum, char *szBarcode)
{
	m_csQuary.Lock();
	CppSQLite3Query q;
	try
	{
		CString strQuery =_T("");
		CString strTableName = _T("");
		
		if(nTableNum == eDB_AAB_TYPE_TEMP){
			strTableName =_T("TBL_DEVICE_DATA");
		}
		else if(nTableNum == eDB_AAB_TYPE_FAIL){
			strTableName =_T("TBL_VIOLATION_DATA");
		}
		strQuery.Format(_T("DELETE FROM %s WHERE barcode = '%s'"),strTableName, szBarcode);
		q = m_db.execQuery( strQuery );
	}
	catch( CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int	CDatabaseIF::DELETE_DEVICE_DATA_TBL(int nTableNum, char szBarcode) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}	
	q.finalize();
	m_csQuary.Unlock();
	return 0;
}

// add by SJI [160919] Add AAB Retest Rule Check
int	CDatabaseIF_Imp::DELETE_DEVICE_DATA_TBL_ALL()
{
	m_csQuary.Lock();

	CppSQLite3Query q;
	CppSQLite3Query q1;

	try
	{
		CString strQuery =_T("");

		strQuery.Format("DELETE FROM TBL_DEVICE_DATA");
		q = m_db.execQuery( strQuery );
		strQuery.Format("DELETE FROM TBL_VIOLATION_DATA");
		q1 = m_db.execQuery( strQuery );
	}
	catch( CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int	CDatabaseIF::DELETE_DEVICE_DATA_TBL_ALL() Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}	
	q.finalize();
	q1.finalize();
	m_csQuary.Unlock();

	return 0;
}

int CDatabaseIF_Imp::DELETE_LOG_TBL_ALL()
{
	m_csQuary.Lock();

	CppSQLite3Query q;

	try
	{
		CString strQuery = _T("");

		strQuery.Format("SELECT tbl_name FROM sqlite_master");
		q = m_db.execQuery(strQuery);

		while (!q.eof())
		{
			strQuery.Format("DELETE FROM %s", q.getStringField(0));

			m_db.execQuery(strQuery);

			q.nextRow();
		}

	}
	catch (CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int	CDatabaseIF::DROP_LOG_TBL_ALL() Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}
	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

int CDatabaseIF_Imp::INSERT_LOTHISTORY_TBL(_tLOTHISTORY_LOG& tLotHistory)
{
	m_csQuary.Lock();

	CString strTableName;
	strTableName.Format(_T("LOTHISTORY"));

	int nResult = this->CHECK_TABLE(strTableName);

	if (0 != nResult)
	{
		this->CREATE_LOTHISTORY_TBL();
	}

	//insert
	CppSQLite3Query q;

	try
	{
		CString strQuery;

		CString strEQP_ID		= tLotHistory.szEQP_ID;
		CString strPKGName		= tLotHistory.szPKGName;
		CString strLotNo_Info	= tLotHistory.szLotNo_Info;
		CString strLotEndTime	= tLotHistory.strLotEnd_Time;
		CString strLotStartTime = tLotHistory.strLotStart_Time;
		CString strLotRunTime	= tLotHistory.strLotRunning_Time;

		// DB Data에 홑따옴표 "'" 가 들어가면 안된다 그래서 '' 쌍홀따옴표로 변경 [9/1/2020 donghyun.shin]
		strEQP_ID.Replace("'", "''");
		strPKGName.Replace("'", "''");
		strLotNo_Info.Replace("'", "''");
		strLotEndTime.Replace("'", "''");
		strLotStartTime.Replace("'", "''");
		strLotRunTime.Replace("'", "''");

		strQuery.Format(_T("INSERT INTO %s ( EQP_NO, PKG_Name, DeviceSize_X, DeviceSize_Y, Lot_No, Date, Stacker_Conveyor1, Stacker_Conveyor2, Stacker_Conveyor3, Stacker_Conveyor4, Stacker_Conveyor5, Stacker_Conveyor6, Stacker_Conveyor7, Load_Table1, Load_Table2, Load_Table3, Load_Table4, Tray_Load_PP, Tray_Unload_PP, Tray_Feeder, Clean_Buffer_Table1, Clean_Buffer_Table2, Test_PP1, Test_PP2, Station1, Station2, Station3, Station4, Station5, Station6, Station7, Station8, Transfer, Load_Device, Unload_Device, Retest1_Device, Retest2_Device, Pass_Bin, Fail_Bin, START_Time, ONLINE_OFFLINE, RUN_Time, UPH, AvgIndexTime) VALUES ( '%s', '%s', %.2f,  %.2f, '%s', '%s', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, '%s', %d, '%s', %d, %.2f)"),
			strTableName, strEQP_ID, 
			strPKGName, 
			tLotHistory.dDeviceSizeX, 
			tLotHistory.dDeviceSizeY, 
			strLotNo_Info, 
			strLotEndTime,
			tLotHistory.nPartJamCnt[0],
			tLotHistory.nPartJamCnt[1],
			tLotHistory.nPartJamCnt[2],
			tLotHistory.nPartJamCnt[3],
			tLotHistory.nPartJamCnt[4],
			tLotHistory.nPartJamCnt[5],
			tLotHistory.nPartJamCnt[6],
			tLotHistory.nPartJamCnt[7],
			tLotHistory.nPartJamCnt[8],
			tLotHistory.nPartJamCnt[9],
			tLotHistory.nPartJamCnt[10],
			tLotHistory.nPartJamCnt[11],
			tLotHistory.nPartJamCnt[12],
			tLotHistory.nPartJamCnt[13],
			tLotHistory.nPartJamCnt[14],
			tLotHistory.nPartJamCnt[15],
			tLotHistory.nPartJamCnt[16],
			tLotHistory.nPartJamCnt[17],
			tLotHistory.nPartJamCnt[18],
			tLotHistory.nPartJamCnt[19],
			tLotHistory.nPartJamCnt[20],
			tLotHistory.nPartJamCnt[21],
			tLotHistory.nPartJamCnt[22],
			tLotHistory.nPartJamCnt[23],
			tLotHistory.nPartJamCnt[24],
			tLotHistory.nPartJamCnt[25],
			tLotHistory.nPartJamCnt[26],
			tLotHistory.nLoadCnt,
			tLotHistory.nUnloadCnt,
			tLotHistory.nRetest[0],
			tLotHistory.nRetest[1],
			tLotHistory.nPassCnt,
			tLotHistory.nFailCnt,
			strLotStartTime,
			tLotHistory.nOnLine_OffLine,
			strLotRunTime,
			tLotHistory.nUPH,
			tLotHistory.dAvgIndexTime
			);

		m_Logger.LOG((LPSTR)(LPCTSTR)strQuery);

		q = m_db.execQuery(strQuery);

		CString strPartJamCnt = _T(""), tmpPartJamCnt = _T("");

		for (int j = 0; j < 27; j++)
		{
			strPartJamCnt.Format("%d,", tLotHistory.nPartJamCnt[j]);
			tmpPartJamCnt += strPartJamCnt;
		}

		strQuery.Format("'%s','%s',%.2f,%.2f,'%s','%s','%s',%d,%d,%d,%d,%d,%d,'%s',%d,%.2f,%d,'%s' ",
			
			tLotHistory.szEQP_ID,
			tLotHistory.szPKGName,
			tLotHistory.dDeviceSizeX,
			tLotHistory.dDeviceSizeY,
			tLotHistory.szLotNo_Info,
			tLotHistory.strLotEnd_Time,
			tmpPartJamCnt,
			tLotHistory.nLoadCnt,
			tLotHistory.nUnloadCnt,
			tLotHistory.nRetest[0],
			tLotHistory.nRetest[1],
			tLotHistory.nPassCnt,
			tLotHistory.nFailCnt,

			tLotHistory.strLotStart_Time,
			tLotHistory.nOnLine_OffLine,
			tLotHistory.dAvgIndexTime,
			tLotHistory.nUPH,
			tLotHistory.strLotRunning_Time);
			
		strQuery.Replace("\r", NULL);
		strQuery.Replace("\\r", NULL);
		strQuery.Replace("\n", NULL);
		strQuery.Replace("\\n", NULL);

		m_Logger.LOG((LPSTR)(LPCTSTR)strQuery);
	}
	catch (CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int     CDatabaseIF_Imp::INSERT_LOTHISTORY_TBL(_tLOTHISTORY_LOG& tLotHistory) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

int CDatabaseIF_Imp::SELECT_LOTHISTORY_TBL(CString strDate_S, CString strDate_E, std::vector<ST_LOTHISTORY>& vLogVector)
{
	CString strQuery = this->MAKE_SELECT_LOTHISTORY_QUERY(strDate_S, strDate_E);

	return this->EXEC_SELECT_LOTHISTORY_QUERY(strDate_S, strDate_E, vLogVector, strQuery);
}

// Daily Report
int CDatabaseIF_Imp::SELECT_DAILY_DATA_TBL(CString strStartDate, CString strEndDate, ST_DAILY_REPORT* stDailyData)
{
	m_csQuary.Lock();
	CppSQLite3Query q;

	try
	{
		int nResult = this->CHECK_TABLE("TBL_DAILY_REPORT");

		if (0 != nResult)
		{
			this->CREATE_DAILY_DATA_TBL();
		}

		CString strQuery;
		strQuery.Format("SELECT * FROM TBL_DAILY_REPORT WHERE DATE >= '%s' AND DATE <= '%s'", strStartDate, strEndDate);
		q = m_db.execQuery(strQuery);
		while (!q.eof())
		{
			_tDIO_LS* pRow = new _tDIO_LS;

			stDailyData->nLoadCnt += q.getIntField(1);
			stDailyData->nUnloadCnt += q.getIntField(2);
			stDailyData->nPassCnt += q.getIntField(3);
			stDailyData->nART_FT_Total_Cnt += q.getIntField(4);
			stDailyData->nART_FT_Pass_Cnt += q.getIntField(5);
			stDailyData->nART_RT1_Total_Cnt += q.getIntField(6);
			stDailyData->nART_RT1_Pass_Cnt += q.getIntField(7);
			stDailyData->nART_RT2_Total_Cnt += q.getIntField(8);
			stDailyData->nART_RT2_Pass_Cnt += q.getIntField(9);

			q.nextRow();
		}
	}
	catch (CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int CDatabaseIF_Imp::SELECT_DAILY_DATA_TBL(CString strStartDate, CString strEndDate, ST_DAILY_REPORT* stDailyData) Error, [DB ERR : %d = %s][WIN ERR : %lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

int CDatabaseIF_Imp::UPDATE_DAILY_DATA_TBL(CString strDate, ST_DAILY_REPORT stAddData)
{
	m_csQuary.Lock();
	CppSQLite3Query q;

	int nRowCnt = 0;
	try
	{
		int nResult = this->CHECK_TABLE("TBL_DAILY_REPORT");

		if (0 != nResult)
		{
			this->CREATE_DAILY_DATA_TBL();
		}

		CString strQuery;
		strQuery.Format(_T("SELECT COUNT(*) FROM TBL_DAILY_REPORT WHERE DATE = '%s'"), strDate);
		q = m_db.execQuery(strQuery);
		//m_Logger.LOG("SQL [%s]", strQuery);

		nRowCnt = q.getIntField(0);

		if (nRowCnt <= 0) { // Insert
			strQuery.Format(_T("INSERT INTO TBL_DAILY_REPORT(DATE, LOAD, UNLOAD, PASS, ART_FT_TOTAL, ART_FT_PASS, ART_RT1_TOTAL, ART_RT1_PASS, ART_RT2_TOTAL, ART_RT2_PASS) VALUES('%s', 0, 0, 0, 0, 0, 0, 0, 0, 0)"), strDate);
			m_db.execQuery(strQuery);
			//m_Logger.LOG("SQL [%s]", strQuery);
		}

		strQuery.Format(_T("SELECT * FROM TBL_DAILY_REPORT WHERE DATE = '%s'"),strDate);
		q = m_db.execQuery(strQuery);

		ST_DAILY_REPORT stBeforeDailyData;

		stBeforeDailyData.nLoadCnt = q.getIntField(1);
		stBeforeDailyData.nUnloadCnt = q.getIntField(2);
		stBeforeDailyData.nPassCnt = q.getIntField(3);
		stBeforeDailyData.nART_FT_Total_Cnt = q.getIntField(4);
		stBeforeDailyData.nART_FT_Pass_Cnt = q.getIntField(5);
		stBeforeDailyData.nART_RT1_Total_Cnt = q.getIntField(6);
		stBeforeDailyData.nART_RT1_Pass_Cnt = q.getIntField(7);
		stBeforeDailyData.nART_RT2_Total_Cnt = q.getIntField(8);
		stBeforeDailyData.nART_RT2_Pass_Cnt = q.getIntField(9);

		strQuery.Format(_T("UPDATE TBL_DAILY_REPORT SET LOAD = %d, UNLOAD = %d, PASS = %d, ART_FT_TOTAL = %d, ART_FT_PASS = %d, ART_RT1_TOTAL = %d, ART_RT1_PASS = %d, ART_RT2_TOTAL = %d, ART_RT2_PASS = %d WHERE DATE = '%s' ")
			, stBeforeDailyData.nLoadCnt + stAddData.nLoadCnt
			, stBeforeDailyData.nUnloadCnt + stAddData.nUnloadCnt
			, stBeforeDailyData.nPassCnt + stAddData.nPassCnt
			, stBeforeDailyData.nART_FT_Total_Cnt + stAddData.nART_FT_Total_Cnt
			, stBeforeDailyData.nART_FT_Pass_Cnt + stAddData.nART_FT_Pass_Cnt
			, stBeforeDailyData.nART_RT1_Total_Cnt + stAddData.nART_RT1_Total_Cnt
			, stBeforeDailyData.nART_RT1_Pass_Cnt + stAddData.nART_RT1_Pass_Cnt
			, stBeforeDailyData.nART_RT2_Total_Cnt + stAddData.nART_RT2_Total_Cnt
			, stBeforeDailyData.nART_RT2_Pass_Cnt + stAddData.nART_RT2_Pass_Cnt
			, strDate);

		q = m_db.execQuery(strQuery);
		//m_Logger.LOG("SQL [%s]", strQuery);

	}
	catch (CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int	CDatabaseIF_Imp::UPDATE_DAILY_DATA_TBL(CString strDate, ST_DAILY_REPORT stAddData) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}
	q.finalize();

	m_csQuary.Unlock();

	return 0;
}

int CDatabaseIF_Imp::CREATE_DAILY_DATA_TBL()
{
	m_csQuary.Lock();

	try
	{
		CString strQuery;
		strQuery.Format(_T("Create Table 'TBL_DAILY_REPORT' ( 'DATE' TEXT, 'LOAD' INTEGER, 'UNLOAD' INTEGER, 'PASS' INTEGER, 'ART_FT_TOTAL' INTEGER,'ART_FT_PASS' INTEGER,'ART_RT1_TOTAL' INTEGER,'ART_RT1_PASS' INTEGER,'ART_RT2_TOTAL' INTEGER,'ART_RT2_PASS' INTEGER );"));

		m_db.execDML(strQuery);
	}
	catch (CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int CDatabaseIF_Imp::CREATE_DAILY_DATA_TBL() Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}

	m_csQuary.Unlock();
	return 0;
}


int CDatabaseIF_Imp::CREATE_CYLINDER_TIME_TBL()
{
	m_csQuary.Lock();

	try
	{
		CString strQuery;
		strQuery.Format(_T("Create Table 'TBL_CYLINDER_TIME' ('UnitName' TEXT, 'SpecMin' DOUBLE, 'SpecMax' DOUBLE, 'Value' DOUBLE, 'Idx' INTEGER);"));

		m_db.execDML(strQuery);
	}
	catch (CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int CDatabaseIF_Imp::CREATE_CYLINDER_TIME_TBL() Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}

	m_csQuary.Unlock();
	return 0;
}

int CDatabaseIF_Imp::CHECK_CYLINDER_TIME_TBL()
{
	int nReslut = this->CHECK_TABLE("TBL_CYLINDER_TIME");
	if (nReslut != 0)
	{
		CREATE_CYLINDER_TIME_TBL();
		return 1;
	}

	return 0;
}
int CDatabaseIF_Imp::INSERT_CYLINDER_TIME_TBL(_tCYLINDER_TIME& rCylinderData)
{
	CppSQLite3Query q;

	int nReslut = this->CHECK_TABLE("TBL_CYLINDER_TIME");
	if (nReslut != 0)
	{
		CREATE_CYLINDER_TIME_TBL();
	}

	try
	{
		CString strQuery;
		strQuery.Format("INSERT INTO TBL_CYLINDER_TIME(UnitName, SpecMin, SpecMax, Value, Idx) VALUES('%s', '%f', '%f', '%f', '%d')",
			 rCylinderData.strUnitName, rCylinderData.dSpecMin, rCylinderData.dSpecMax,  rCylinderData.dValue, rCylinderData.nIdx);
		q = m_db.execQuery(strQuery);
		m_Logger.LOG("SQL [%s]", strQuery);
	}
	catch (CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int	CDatabaseIF_Imp::INSERT_CYLINDER_TIME_TBL(const _tCYLINDER_TIME& rCylinderData) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}

	q.finalize();

	return 0;
}

int CDatabaseIF_Imp::UPDATE_CYLINDER_TIME_TBL(int nInx, double dCylinderValue)
{
	m_csQuary.Lock();
	CppSQLite3Query q;

	int nRowCnt = 0;
	try
	{
		int nResult = this->CHECK_TABLE("TBL_CYLINDER_TIME");

		if (0 != nResult)
		{
			this->CREATE_CYLINDER_TIME_TBL();
		}

		CString strQuery;
		strQuery.Format(_T("UPDATE TBL_CYLINDER_TIME SET Value = %f  WHERE Idx = %d "),
			dCylinderValue,
			nInx);
		q = m_db.execQuery(strQuery);
		m_Logger.LOG("SQL [%s]", strQuery);

	}
	catch (CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int	CDatabaseIF_Imp::TBL_CYLINDER_TIME(int nInx, _tCYLINDER_TIME rCylinderData) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}
	q.finalize();

	m_csQuary.Unlock();

	return 0;
}

int CDatabaseIF_Imp::SELECT_CYLINDER_TIME_TBL(CMyList& rDataList)
{
	m_csQuary.Lock();
	CppSQLite3Query q;

	try
	{
		CString strQuery;
		strQuery.Format("SELECT * FROM TBL_CYLINDER_TIME ORDER BY Idx");
		q = m_db.execQuery(strQuery);
		while (!q.eof())
		{
			_tCYLINDER_TIME* pRow = new _tCYLINDER_TIME;

			pRow->strUnitName = q.getStringField(0);
			pRow->dSpecMin = q.getFloatField(1);
			pRow->dSpecMax = q.getFloatField(2);
			pRow->dValue = q.getFloatField(3);
			pRow->nIdx = q.getIntField(4);			
			rDataList.m_fnAddData(pRow);
			q.nextRow();
		}
	}
	catch (CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int CDatabaseIF_Imp::TBL_CYLINDER_TIME(int nDir, CMyList& rDataList) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

int CDatabaseIF_Imp::SELECT_VAT_BASE_INFO(int nLocIdx, double& nPosX, double& nPosY)
{
	m_csQuary.Lock();

	CppSQLite3Query q;

	try 
	{
		CString strQuery;
		strQuery.Format("SELECT * FROM TBL_VAT_BASE_INFO WHERE LOC_IDX = %d", nLocIdx);
		q = m_db.execQuery(strQuery);

		if (q.eof() == true)
		{
			q.finalize();
			m_csQuary.Unlock();
			return -1;
		}

		nPosX = q.getIntField(2);
		nPosY = q.getIntField(3);
	}
	catch (CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int CDatabaseIF_Imp::TBL_VAT_BASE_INFO(int nLocIdx, int& nPosX, int& nPosY) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

int CDatabaseIF_Imp::CHECK_VAT_TBL()
{
	int nReslut = this->CHECK_TABLE("Z_AUTO_FOCUS");
	nReslut += this->CHECK_TABLE("CAM_CENTER_CAL");
	nReslut += this->CHECK_TABLE("VISION_AUTO_TEACHING_LOAD_TESTPP1");
	nReslut += this->CHECK_TABLE("VISION_AUTO_TEACHING_LOAD_TESTPP2");
	nReslut += this->CHECK_TABLE("VISION_AUTO_TEACHING_LOAD_TRAYPP");
	nReslut += this->CHECK_TABLE("VISION_AUTO_TEACHING_UNLOAD_TESTPP1");
	nReslut += this->CHECK_TABLE("VISION_AUTO_TEACHING_UNLOAD_TESTPP2");
	nReslut += this->CHECK_TABLE("VISION_AUTO_TEACHING_UNLOAD_TRAYPP");

	if (nReslut != 0)
	{
		return 1;
	}

	return 0;
}

int CDatabaseIF_Imp::SELECT_VAT_Z_AUTO_FOCUS(double(*rVATGuiDataList)[3], int nType)
{
	m_csQuary.Lock();

	CppSQLite3Query q;

/*	double dVATGuiDataList[6][3] = {0,};*/
	

	try
	{
		CString strQuery;
		strQuery.Format("SELECT * FROM Z_AUTO_FOCUS WHERE TYPE = %d", nType);
		q = m_db.execQuery(strQuery);
		int nHand = 0;

		while (!q.eof())
		{
// 			dVATGuiDataList[nHand][0] = q.getIntField(2);
// 			dVATGuiDataList[nHand][1] = q.getIntField(3);
// 			dVATGuiDataList[nHand][2] = q.getIntField(4);

			rVATGuiDataList[nHand][0] = q.getIntField(2);
			rVATGuiDataList[nHand][1] = q.getIntField(3);
			rVATGuiDataList[nHand][2] = q.getIntField(4);

			nHand++;
			q.nextRow();
		}

		
	}
	catch (CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int CDatabaseIF_Imp::TBL_VAT(int nLocIdx, int& nPosX, int& nPosY) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

int CDatabaseIF_Imp::SELECT_VAT_CAM_CENTER_CAL(double(*rVATGuiDataList)[3])
{
	m_csQuary.Lock();

	CppSQLite3Query q;

	try
	{
		CString strQuery;
		strQuery.Format("SELECT * FROM CAM_CENTER_CAL");
		q = m_db.execQuery(strQuery);
		int nHand = 0;

		while (!q.eof())
		{
			rVATGuiDataList[nHand][0] = q.getIntField(1);
			rVATGuiDataList[nHand][1] = q.getIntField(2);
			rVATGuiDataList[nHand][2] = q.getIntField(3);

			nHand++;
			q.nextRow();
		}


	}
	catch (CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int CDatabaseIF_Imp::TBL_VAT(int nLocIdx, int& nPosX, int& nPosY) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}

int CDatabaseIF_Imp::SELECT_VAT_VISION_AUTO_TEACHING(double(*rVATGuiDataList)[3], int nHand)
{
	m_csQuary.Lock();

	CppSQLite3Query q;

	double dVATGuiDataList[13][3] = { 0, };

	CString strHandType;
	
	switch (nHand)
	{
	case 0:
	{
		strHandType.Format("LOAD_TRAYPP");
	}break;
	case 1:
	{
		strHandType.Format("UNLOAD_TRAYPP");
	}break;
	case 2:
	{
		strHandType.Format("LOAD_TESTPP1");
	}break;
	case 3:
	{
		strHandType.Format("UNLOAD_TESTPP1");
	}break;
	case 4:
	{
		strHandType.Format("LOAD_TESTPP2");
	}break;
	case 5:
	{
		strHandType.Format("UNLOAD_TESTPP2");
	}break;
	}
	

	try
	{
		CString strQuery;
		strQuery.Format("SELECT * FROM VISION_AUTO_TEACHING_%s ", strHandType);
		q = m_db.execQuery(strQuery);
		int nTeachingPart = 0;

		while (!q.eof())
		{
			
			dVATGuiDataList[nTeachingPart][0] = q.getIntField(2);
			dVATGuiDataList[nTeachingPart][1] = q.getIntField(3);
			dVATGuiDataList[nTeachingPart][2] = q.getIntField(4);

			rVATGuiDataList[nTeachingPart][0] = q.getIntField(2);
			rVATGuiDataList[nTeachingPart][1] = q.getIntField(3);
			rVATGuiDataList[nTeachingPart][2] = q.getIntField(4);

			nTeachingPart++;
			q.nextRow();
		}


	}
	catch (CppSQLite3Exception& e)
	{
		DWORD dwErr = ::GetLastError();
		m_Logger.LOG("int CDatabaseIF_Imp::TBL_VAT(int nLocIdx, int& nPosX, int& nPosY) Error, [DB ERR: %d = %s] [WIN ERR:%lu] ", e.errorCode(), e.errorMessage(), dwErr);
	}

	q.finalize();
	m_csQuary.Unlock();

	return 0;
}
