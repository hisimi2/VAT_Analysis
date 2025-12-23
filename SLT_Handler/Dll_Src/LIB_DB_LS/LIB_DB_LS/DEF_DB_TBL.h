/********************************************************************
	created:	2015/07/24
	created:	24:7:2015   15:22
	filename: 	SLT_Handler\Include\DEF_DB_TBL.h
	file path:	SLT_Handler\Include
	file base:	DEF_DB_TBL
	file ext:	h
	author:		나명진
	
	purpose:	LIB_DB_LS Dll에서 사용하는 매크로 모음
*********************************************************************/
#pragma once

#include <afx.h>

#define STATION_MAX_X_SITE_GRR_DB 4
#define STATION_MAX_Y_SITE_GRR_DB 2

enum eCREATE_DB_TYPE
{
	eDB_TYPE_SYSTEM = 0,
	eDB_TYPE_LOG,
	eDB_TYPE_AARCHECK,
	eDB_TYPE_TEACH,
	eDB_TYPE_LOTHISTORY,
	eDB_TYPE_VAT,
};

enum eAlarmState{
	eALST_CLEAR = 0,eALST_SET,
};

enum eAAB_DB_TYPE
{
	eDB_AAB_TYPE_TEMP =0,
	eDB_AAB_TYPE_FAIL,
};

//==============================================================================
// DIO_LS
//==============================================================================
class AFX_EXT_CLASS _tDIO_LS : public CObject
{
public:
	enum ColumnName : int
	{
		NO,
		DIRECTION,
		IDX,
		ADDRESS,
		PART_NO,
		LABEL,
		NAME,
		CONTACT,
		DIO_TYPE,
		GUI_LOC_X,
		GUI_LOC_Y,
	};

	enum eDIO_DIR { eDI = 0, eDO };
	enum eDIO_CT { eContact_A = 0, eContact_B };	// contact type

	_tDIO_LS()
	{
		no = 0;
		dir = eDIO_DIR::eDI;
		index = 0;
		address = 0;
		part_no = 0;
		name = _T("");
		label = _T("");
		dio_type = _T("");
		contact = eDIO_CT::eContact_A;
	};

	~_tDIO_LS() {};

	_tDIO_LS& operator=(const _tDIO_LS& src)
	{
		if (this == &src) {
			return *this;
		}
		no = src.no;
		dir = src.dir;
		index = src.index;
		address = src.address;
		part_no = src.part_no;
		name = src.name;
		label = src.label;
		contact = src.contact;
		dio_type = src.dio_type;
		return *this;
	}

// 	static CString GetColumnName(_tDIO_LS::ColumnName _column_name)
// 	{
// 		BEGIN_CASE_TO_STR(_column_name);
// 		CASE_TO_STR(NO);
// 		CASE_TO_STR(DIRECTION);
// 		CASE_TO_STR(IDX);
// 		CASE_TO_STR(ADDRESS);
// 		CASE_TO_STR(PART_NO);
// 		CASE_TO_STR(LABEL);
// 		CASE_TO_STR(NAME);
// 		CASE_TO_STR(CONTACT);
// 		CASE_TO_STR(GUI_LOC_X);
// 		CASE_TO_STR(GUI_LOC_Y);
// 		ELSE_CASE_STR(_T("unkown"));
// 		END_CASE_TO_STR();
// 	}


	int			no;
	eDIO_DIR	dir;
	int			index;
	int			address;
	int			part_no;
	CString		name;
	CString		label;
	CString		dio_type;
	eDIO_CT		contact;
};

/*
class AFX_EXT_CLASS _tDIO_LS : public CObject
{
public:
	_tDIO_LS()
	{
		nDir		= 0;
		nIndex		= 0;
		nContact	= 0;
		strName		= _T("");
		strLabel	= _T("");
		nAddress	= 0;
	};
	
	_tDIO_LS(const _tDIO_LS& src)
	{
		nDir     = src.nDir    ;	
		nIndex   = src.nIndex  ;	
		strName  = src.strName ;
		strLabel = src.strLabel;
		nContact = src.nContact;
		nAddress = src.nAddress;
	}
	~_tDIO_LS(){};

	_tDIO_LS& operator=(const _tDIO_LS& src)
	{
		if( this == &src ){
			return *this;
		}
		nDir     = src.nDir    ;	
		nIndex   = src.nIndex  ;	
		strName  = src.strName ;
		strLabel = src.strLabel;
		nContact = src.nContact;
		nAddress = src.nAddress;
		return *this;
	}

	enum eDIO_DIR {eDI=0, eDO};
	enum eDIO_CT  {eContact_A = 0, eContact_B};	// contact type

	short		nDir;			// 0='DI', 1='DO'
	int			nIndex;			// Bit Index
	CString		strName;		// IO Name
	CString		strLabel;		// 전장 Label
	short		nContact;		// 0='A'접점(arbeit contact) Norma Open, 1='B'접점 (break contact) Normal Close
	int			nAddress;
};
*/

//==============================================================================
// TBL_AIO
//==============================================================================
class AFX_EXT_CLASS _tAIO_LS : public CObject
{
public:
	_tAIO_LS()
	{
		nDir          = 0;
		nIdx_Logical  = 0;
		nIdx_Physical = 0;
		strName       = _T("");
		strLabel      = _T("");
		dMinVolt      = 0.0;
		dMaxVolt      = 0.0;
		dMinPhyVal    = 0.0;
		dMaxPhyVal    = 0.0;
		strUnit       = _T("");
		strDesc       = _T("");
	};

	_tAIO_LS(const _tAIO_LS& src)
	{
		nDir          = src.nDir;
		nIdx_Logical  = src.nIdx_Logical;
		nIdx_Physical = src.nIdx_Physical;
		strName       = src.strName;
		strLabel      = src.strLabel;
		dMinVolt      = src.dMinVolt;
		dMaxVolt      = src.dMaxVolt;
		dMinPhyVal    = src.dMinPhyVal;
		dMaxPhyVal    = src.dMaxPhyVal;
		strUnit       = src.strUnit;
		strDesc       = src.strDesc;
	}

	~_tAIO_LS(){};


	_tAIO_LS& operator=(const _tAIO_LS& src)
	{
		if( this == &src ){
			return *this;
		}
		nDir          = src.nDir;
		nIdx_Logical  = src.nIdx_Logical;
		nIdx_Physical = src.nIdx_Physical;
		strName       = src.strName;
		strLabel      = src.strLabel;
		dMinVolt      = src.dMinVolt;
		dMaxVolt      = src.dMaxVolt;
		dMinPhyVal    = src.dMinPhyVal;
		dMaxPhyVal    = src.dMaxPhyVal;
		strUnit       = src.strUnit;
		strDesc       = src.strDesc;
		return *this;
	}

	enum eAIO_DIR {eAI=0, eAO};

	short		nDir;			// 0='DI', 1='DO'
	int			nIdx_Logical;	// Logical Index
	int			nIdx_Physical;	// Physical Index
	CString		strName;		// IO Name
	CString		strLabel;		// 전장 Label
	double      dMinVolt;
	double      dMaxVolt;
	double      dMinPhyVal;
	double      dMaxPhyVal;
	CString     strUnit;        // 물리 단위
	CString     strDesc;        // IO 설명
};


//==============================================================================
// TBL_MOTOR_PARAM
//==============================================================================
/*
class AFX_EXT_CLASS _tMOTOR_PARAM : public CObject
{
public:
	enum _eMotorCoordinate{
		X = 0, Y, Z, PITCH_X, PITCH_Y
	};

public:
	_tMOTOR_PARAM();
	_tMOTOR_PARAM(const _tMOTOR_PARAM& src);
	_tMOTOR_PARAM& operator = (const _tMOTOR_PARAM& src);
	~_tMOTOR_PARAM();
	void Init();
	void Copy(const _tMOTOR_PARAM& src);

	int		nAxisNo_Logical;		// 프로그램에서 사용하는 논리적인 축번호 ( 0 Base )
	char    szAxisGroup[64];		// 축 그룹
	int		nAxisNo_Physical;		// Board상의 axis 번호
	char    szCaption[64];			// Display Name
	BOOL	bDisableAxis;			// 해당 축의 사용 유/무
	double	dScaleFactor;			// um/pulse
	double  dAngle;					// Pulley의 원주, mm
	double  dMotorRate;				//
	double  dPulleyRate;			//
	double  dServoPulse;			// motor가 1회전하기위해 필요한 pulse 수
	int		nDir_ZR;					// -1:역 , 1:정
	int		nDirPulse;				// -1:역 , 1:정
	int		nDirMove;				// -1:역 , 1:정
	int		nDirType;				// 0=UP/DOWN 1=FOR/BACK 2=LEFT/RIGHT 3=NARROW/WIDE
	int		nEncoderType;			// 0:mmc type
	double	dLimit_Posi;			// um
	double	dLimit_Nega;			// um
	int		nSpd_Normal;			// pps
	int		nAcc_Normal;			// msec
	int		nDec_Normal;			// msec
	int		nSpd_ZR;				// pps
	int		nAcc_ZR;				// msec
	_eMotorCoordinate     eMotorCoordinate;
};*/

class AFX_EXT_CLASS _tMOTOR_PARAM : public CObject
{
public:
	enum _eMotorCoordinate {
		X = 0, Y, Z, Rotator, Picker_Z,
	};
	_tMOTOR_PARAM();
	_tMOTOR_PARAM(const _tMOTOR_PARAM& src);
	_tMOTOR_PARAM& operator = (const _tMOTOR_PARAM& src);
	~_tMOTOR_PARAM();
	void Init();
	void Copy(const _tMOTOR_PARAM& src);

	CString strBoardType;			// Board Type(SOFT_SERVO, EZI_SERVO)
	int		nAxisNo_Logical;		// 프로그램에서 사용하는 논리적인 축번호 ( 0 Base )
	int		nAxisNo_Physical;		// 보드에서 사용되는 물리적 축번호 
	int		nPartNo;				// 장비 내의 파트 번호.
	CString strAxisName;			// 축이름.
	double	dScaleFactor;			// um/pulse
	int     nPulsePerRound;			// [19-11-27] 회전 당 pulse 수.
	int		nDir_ZR;				// -1:역 , 1:정
	int		nDirPulse;				// -1:역 , 1:정
	int		nDirMove;				// -1:역 , 1:정
	double	dLimit_Posi;			// um
	double	dLimit_Nega;			// um
	int		nSpd_Normal;			// pps
	int		nAcc_Normal;			// msec
	int		nDec_Normal;			// msec
	int		nSpd_ZR;				// pps
	int		nAcc_ZR;				// msec
	int		nDouble_Chk_Pulse;		// pulse 2중 origin check 진행 시 사용.
	int		nMMC_Encoder_Dir;		// mmc 보드에 설정하는 encoder 방향.
	int		nEncoder_Type;			// encoder type에 대한 특수 처리가 필요 할떄 사용.
	int		nMMC_Coordinate_Dir;	// mmc 보드에 설정하는 coordiante 방향.
	int		nMotorType;				// 0:servo / 1:step
	int		nSyncType;				// 0:no use / 1:master / 2:slave
	int		nSlaveNo;				// Master 축과 연결된 Slave No.
	double  dHwInPositionWidth;		// Hardware in-position width
	_eMotorCoordinate     eMotorCoordinate;
	BOOL	bDegree;
};


//==============================================================================
// TBL_GRR_DATA
//==============================================================================
class AFX_EXT_CLASS _tGRR_DATA : public CObject
{
public:
	char szUnique[10];
	char sz2DID[64];
	char szSiteNum[32];
	char szSubSiteNo[4];
	int  nTestCntNo;
	char szTestResult[16];
	char szResultDescription[128];

	_tGRR_DATA()
	{
		memset(szUnique, 0, sizeof(szUnique));
		memset(sz2DID, 0, sizeof(sz2DID));
		memset(szSiteNum, 0, sizeof(szSiteNum));
		memset(szSubSiteNo, 0, sizeof(szSubSiteNo));
		nTestCntNo = 0;
		memset(szTestResult, 0, sizeof(szTestResult));
		memset(szResultDescription, 0, sizeof(szResultDescription));
	}

	_tGRR_DATA( const _tGRR_DATA& other )
	{
		*this = other;
	}

	_tGRR_DATA& operator=(const _tGRR_DATA& other)
	{
		if (this == &other)
			return *this;

		sprintf_s(szUnique, sizeof(szUnique), other.szUnique);
		sprintf_s(sz2DID, sizeof(sz2DID), other.sz2DID);
		sprintf_s(szSiteNum, sizeof(szSiteNum), other.szSiteNum);
		sprintf_s(szSubSiteNo, sizeof(szSubSiteNo), other.szSubSiteNo);
		nTestCntNo = other.nTestCntNo;		
		sprintf_s(szTestResult, sizeof(szTestResult), other.szTestResult);
		sprintf_s(szResultDescription, sizeof(szResultDescription), other.szResultDescription);

		return *this;
	}
};

//==============================================================================
// TEMP_PID
//==============================================================================

class AFX_EXT_CLASS _tTEMP_PID : public CObject
{
public:
	_tTEMP_PID()
	{
		nCtrlNo		= 0;
		dSetTemp	= 0;
		dPidP1		= 0.0;
		dPidP2		= 0.0;
		dPidI		= 0.0;
		dPidD		= 0.0;
	};
	~_tTEMP_PID(){};

public:
	int		nCtrlNo;
	double	dSetTemp;
	double	dPidP1;
	double	dPidP2;
	double	dPidI;
	double	dPidD;
};

//==============================================================================
// ROBOT
//==============================================================================
class AFX_EXT_CLASS _tRobot : public CObject
{
public:
	_tRobot()
	{
	
	};
	~_tRobot(){};
};

/*
 * MessageBox() Flags
 */
enum eAlarmMsgBoxFlag{
	TWMB_OK=0,
	TWMB_OKCANCEL,
	TWMB_RETRY,
	TWMB_RETRYSKIP,
	TWMB_CLEANOUTRETRYSKIP,
    TWMB_SKIP_PASSWORD,
    TWMB_TRAY_END_FEED,
	TWMB_LOAD_PARTIAL_TRAY_END,
	TWMB_ID_SKIP_TO_DATA_ASSIGN,
	TWMB_OK_CLEAR,
	TWMB_SKIP,
	TWMB_AXIS_MOVE,
	TWMB_QA_QTY,
	TWMB_MAX_COUNT,
};

enum eAlarmMsgBoxReturn{
	TWMB_ID_OK = 0,
	TWMB_ID_CANCEL,
	TWMB_ID_RETRY,
	TWMB_ID_SKIP,
	TWMB_ID_CLEANOUT,
    TWMB_ID_P_SKIP,
    TWMB_ID_TRAYEND,
    TWMB_ID_TRAYFEED,
	TWMB_ID_PARTIAL_TRAYEND,
	TWMB_ID_SKIP_TO_ASSIGN,
	TWMB_ID_CLEAR,
	TWMB_ID_AXIS_MOVE,
	TWMB_ID_QA_QTY,
	TWMB_ID_MAX_COUNT,
};


//==============================================================================
// ERROR_MSG_TBL
//==============================================================================
class AFX_EXT_CLASS _tERROR_MSG : public CObject
{
public:
	_tERROR_MSG()
	{
		clear();
	}
	_tERROR_MSG(const _tERROR_MSG& src)
	{
		clear();
		strcpy_s(szEQP_ALID, sizeof(szEQP_ALID), src.szEQP_ALID);
		strcpy_s(szPart, sizeof(szPart), src.szPart);
		strcpy_s(szALID, sizeof(szALID), src.szALID);
		strKorea		= src.strKorea	   ;
		strEnglish		= src.strEnglish   ;
		strChina		= src.strChina	   ;
		strJapan		= src.strJapan	   ;
		nError_Level	= src.nError_Level ;
		nALCD			= src.nALCD		   ;
		nALED			= src.nALED		   ;
		bAlarmState     = src.bAlarmState;
		nMB_Type        = src.nMB_Type;
		nPosition		= src.nPosition;

	}
	~_tERROR_MSG(){};

	_tERROR_MSG& operator = (const _tERROR_MSG& src)
	{
		if( this == &src ){
			return *this;
		}

		clear();
		strcpy_s(szEQP_ALID, sizeof(szEQP_ALID), src.szEQP_ALID);
		strcpy_s(szPart, sizeof(szPart), src.szPart);
		strcpy_s(szALID, sizeof(szALID), src.szALID);
		strKorea		= src.strKorea	   ;
		strEnglish		= src.strEnglish   ;
		strChina		= src.strChina	   ;
		strJapan		= src.strJapan	   ;
		nError_Level	= src.nError_Level ;
		nALCD			= src.nALCD		   ;
		nALED			= src.nALED		   ;
		bAlarmState     = src.bAlarmState  ;
		nMB_Type        = src.nMB_Type;
		nPosition		= src.nPosition;
		//strcpy_s(szLotNo_Info, sizeof(szLotNo_Info), src.szLotNo_Info);
		return *this;
	}

	void clear()
	{
		ZeroMemory(szEQP_ALID, sizeof(szEQP_ALID));
		ZeroMemory(szPart, sizeof(szPart));
		ZeroMemory(szALID, sizeof(szALID));
		strKorea		= _T("");
		strEnglish		= _T("");
		strChina		= _T("");
		strJapan		= _T("");
		nError_Level	= 0;
		nALCD			= 0;
		nALED			= 0;
		bAlarmState     = FALSE;
		nMB_Type        = 0;
		nPosition		= 0;
	}

	char		szEQP_ALID[32];		// 장비에서 사용하는 Alarm ID
	char		szPart[32];
	CString		strKorea;
	CString		strEnglish;
	CString		strChina;
	CString		strJapan;
	int			nError_Level;		// 0 = Log, 1 = Message, 2 = Warning, 3 = Jam, 4 = Critical
	char		szALID[32];			// secs2에서 사용하는 Alarm ID
	int			nALCD;				// secs2에서 사용하는 Alarm Code
	int			nALED;				// secs2에서 사용하는 Alarm Enable
	BOOL        bAlarmState;		// 0:clear, 1:set, db field에는 존재하지 않고 메모리 상으로만 가지고 있음.
	int         nMB_Type;           // message box type
	int         nPosition;          // Error Message Unit Position
	//char		szLotNo_Info[64];	// 해당 Error 발생 시 Lot기록
};

//==============================================================================
// ERROR_LOG_TBL
//==============================================================================
class AFX_EXT_CLASS _tERROR_LOG : public CObject
{
public:
	_tERROR_LOG()
	{
		ZeroMemory(szEQP_ALID, sizeof(szEQP_ALID));
		ZeroMemory(szPart, sizeof(szPart));
		ZeroMemory(szMessage, sizeof(szMessage));
		ZeroMemory(szRecovery, sizeof(szRecovery));
		ZeroMemory(szUser_Info, sizeof(szUser_Info));
		strOccurred_Time = _T("");
		nStop_Time = 0;
		nError_Level = 0;
		nPosition = 0;
		nDuplicate = 0;
		nOnLine_OffLine = 0;
		ZeroMemory(szLotNo_Info, sizeof(szLotNo_Info));
	}
	~_tERROR_LOG(){};

	// Err Msg에 있는 정보를 Err Log에 셋팅 한다.
	void SetErrLogFromErrMsg(_tERROR_MSG& tErrMsg, CString a_strMessage, SYSTEMTIME a_tOccurredTime, CString a_strUser)
	{
		strcpy_s(szEQP_ALID, sizeof(szEQP_ALID), tErrMsg.szEQP_ALID);
		strcpy_s(szPart, sizeof(szPart), tErrMsg.szPart);
		nError_Level = tErrMsg.nError_Level;
		SetOccurredTime(a_tOccurredTime);
		SetErrLogMsg(a_strMessage);
		sprintf_s(szUser_Info, sizeof(szUser_Info), "%s", a_strUser.GetString() );
		nPosition = tErrMsg.nPosition;
		//strcpy_s(szLotNo_Info, sizeof(szLotNo_Info), tErrMsg.szLotNo_Info);
	}
	void SetErrLogMsg(CString strMessage)
	{
		memset(szMessage, 0x00, sizeof(szMessage));
		strncpy_s(szMessage, sizeof(szMessage), strMessage, sizeof(szMessage)-1);
	}
	void SetOccurredTime(SYSTEMTIME t)
	{
		strOccurred_Time.Format("%04d-%02d-%02d %02d:%02d:%02d.%03d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
		tOccured_Time = t;
	}
	void SetStopTime(SYSTEMTIME t)
	{
		CTime tClearTime(t);
		CTimeSpan tStop = tClearTime - tOccured_Time;
		nStop_Time = (int)tStop.GetTotalSeconds();
	}

	char	szEQP_ALID[32];		// 
	char	szPart[32];
	char	szMessage[450];		// 현재 장비 언어 설정에 따른 알람메세지
	CString	strOccurred_Time;	// YYYY-MM-DD HH:MM:SS.sss
	char	szRecovery[32];		// 알람 발생 후 사용자 조작 정보 기록.( SKIP, RETRY, DONE... )
	int		nStop_Time;			// [ SEC ] 장비가 정지되어있던 시간.
	char	szUser_Info[64];	// ex) ID : techwing, Level : Service
	int		nError_Level;		// 0 = Log, 1 = Message, 2 = Warning, 3 = Jam, 4 = Critical
	CTime   tOccured_Time;
	int     nPosition;          // Error 발생 Unit Postion 
	int		nDuplicate;
	int		nOnLine_OffLine;
	char	szLotNo_Info[64];	// 해당 Error 발생 시 Lot기록
};

//==============================================================================
// LOTHISTORY_TBL
//==============================================================================
class AFX_EXT_CLASS _tLOTHISTORY_LOG : public CObject
{
public:
	_tLOTHISTORY_LOG()
	{
		ZeroMemory(szEQP_ID, sizeof(szEQP_ID));
		ZeroMemory(szPKGName, sizeof(szPKGName));
		dDeviceSizeX = 0;
		dDeviceSizeY = 0;
		ZeroMemory(szLotNo_Info, sizeof(szLotNo_Info));
		strLotEnd_Time = _T("");
		ZeroMemory(nPartJamCnt, sizeof(nPartJamCnt));
		nLoadCnt=0;
		nUnloadCnt=0;
		ZeroMemory(nRetest, sizeof(nRetest));
		nPassCnt=0;
		nFailCnt=0;
		strLotStart_Time = _T("");
		nOnLine_OffLine = 0;
		dAvgIndexTime = 0;
		nUPH = 0;
		strLotRunning_Time = _T("");
	}
	~_tLOTHISTORY_LOG() {};

	void SetLotEndTime(SYSTEMTIME t)
	{
		strLotEnd_Time.Format("%04d-%02d-%02d %02d:%02d:%02d.%03d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
	}
	void SetLotStartTime(SYSTEMTIME t)
	{
		strLotStart_Time.Format("%04d-%02d-%02d %02d:%02d:%02d.%03d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
	}

	char	szEQP_ID[32];		// 장비 번호
	char	szPKGName[32];
	double	dDeviceSizeX;
	double	dDeviceSizeY;
	char	szLotNo_Info[64];
	CString	strLotEnd_Time;	// YYYY-MM-DD HH:MM:SS.sss
	int		nPartJamCnt[28];    //
	int		nLoadCnt;
	int		nUnloadCnt;
	int		nRetest[2];
	int		nPassCnt;
	int		nFailCnt;
	CString	strLotStart_Time;	// YYYY-MM-DD HH:MM:SS.sss
	int		nOnLine_OffLine;
	double	dAvgIndexTime;
	int		nUPH;
	CString	strLotRunning_Time;	// HH:MM:SS.sss
};


//==============================================================================
// TBL_MOTOR_TEACHING
//==============================================================================
class AFX_EXT_CLASS _tMOTOR_TEACHING : public CObject
{
public:
	_tMOTOR_TEACHING()
	{
		ZeroMemory( szCOK_Name, sizeof( szCOK_Name ) );
		nAxis_No = 0;			
		nPos_ID = 0;			
		ZeroMemory( szPos_Name, sizeof( szPos_Name ) );
		dPosition = 0.0;			
		nSpeed_Rate = 0;		
		nAcc_Rate = 0;			
		nDec_Rate = 0;			
		strDescript = _T( "" );
	}
	~_tMOTOR_TEACHING(){};

	char		szCOK_Name[ 64 ];
	int			nAxis_No;				// TBL_MOTOR_PARAM.AXIS_NO_LOGICAL 
	int			nPos_ID;				// 해당 위치의 ID
	char		szPos_Name[ 64 ];		// 위치 명칭
	double		dPosition;				// Teaching Position [um]
	int			nSpeed_Rate;			// TBL_MOTOR_PARAM.SPD_NORMAL의 백분율
	int			nAcc_Rate;				// TBL_MOTOR_PARAM.ACC_NORMAL의 백분율
	int			nDec_Rate;				// TBL_MOTOR_PARAM.DEC_NORMAL의 백분율
	CString		strDescript;
};

//==============================================================================
// TBL_TEACH
//==============================================================================
class AFX_EXT_CLASS _tTEACH_LOG : public CObject
{
public:
	_tTEACH_LOG()
	{
		clear();
	}

// 	_tTEACH_LOG& operator = (const _tTEACH_LOG& src)
// 	{
// 		if (this == &src) {
// 			return *this;
// 		}
// 
// 		clear();
// 		strcpy_s(szRobot_Name, sizeof(szRobot_Name), src.szRobot_Name);
// 		strcpy_s(szPos_Name, sizeof(szPos_Name), src.szPos_Name);
// 		strcpy_s(szAxis_Name, sizeof(szAxis_Name), src.szAxis_Name);
// 		dPosition_Before = src.dPosition_Before;
// 		dPosition_After = src.dPosition_After;
// 		strOccurred_Time = src.strOccurred_Time;
// 		return *this;
// 	}
	~_tTEACH_LOG() {};

	void clear()
	{
		ZeroMemory(szRobot_Name, sizeof(szRobot_Name));
		ZeroMemory(szPos_Name, sizeof(szPos_Name));
		ZeroMemory(szAxis_Name, sizeof(szAxis_Name));
		dPosition_Before = 0.0;
		dPosition_After = 0.0;

		strOccurred_Time = _T("");
	}

	char		szRobot_Name[64];
	char		szPos_Name[64];				// 해당 위치의 ID
	char		szAxis_Name[64];				// TBL_MOTOR_PARAM.AXIS_NO_LOGICAL 
	
	double		dPosition_Before;		// Teaching Position 이전값[um]
	double		dPosition_After;		// Teaching Position 변경값[um]

	CString		strOccurred_Time;	// YYYY-MM-DD HH:MM:SS.sss

};

//==============================================================================
// TBL_MOTION_GROUP_SETTING
//==============================================================================
class AFX_EXT_CLASS _tMOTION_GROUP_SETTING : public CObject
{
public:
	_tMOTION_GROUP_SETTING()
	{
		//::ZeroMemory( this, sizeof( *this ) ); << CString 과 함께 초기화 해버리면 사용하는 쪽에서 에러발생. 
		strCylinderCaption = _T( "" );
		strCylinderOnCaption = _T( "" );
		strCylinderOffCaption = _T( "" );
		strOptionType = _T( "" );
		strOptionCaption = _T( "" );

		nAxisGroupNo = 0;
		nOutputNo = 0;
		nInputNo = 0;

		nCylinderOnOutputNo = 0;
		nCylinderOnInputNo = 0;

		nCylinderOffOutputNo = 0;
		nCylinderOffInputNo = 0;

		nOptionNo = 0;
	}
	~_tMOTION_GROUP_SETTING(){};

	int		nAxisGroupNo;
	int		nOutputNo;					// 해당 그룹 관련된 output 번호 
	int		nInputNo;					// 해당 그룹 관련된 input 번호

	CString	strCylinderCaption;

	CString	strCylinderOnCaption;		// forward, push, up, 등등..
	int		nCylinderOnOutputNo;		// 실린더에 에어 넣는 output 번호
	int		nCylinderOnInputNo;			// 해당 감지 센서 번호

	CString	strCylinderOffCaption;		// backward, pull, down, 등등...
	int		nCylinderOffOutputNo;		// 실린더에 에어 빼는 output 번호
	int		nCylinderOffInputNo;		// 해당 감시 센서 번호.

	CString	strOptionType;				// INT, DOUBLE, BOOL 등등
	int		nOptionNo;					// enum으로 찾을수 있도록..
	CString strOptionCaption;			// 화면 표시 이름
};

//==============================================================================
// TBL_TAG
//==============================================================================
class AFX_EXT_CLASS _tTAG : public CObject
{
public:
	_tTAG()
	{
		nTagID      = 0;
		nGroupID    = 0;
		nFormat     = 0;
		nDefaultUse = 0;
		memset(szName   , 0x00, sizeof(szName)   );
		memset(szValue  , 0x00, sizeof(szValue)  );
		memset(szMin    , 0x00, sizeof(szMin)    );
		memset(szMax    , 0x00, sizeof(szMax)    );
		memset(szDefault, 0x00, sizeof(szDefault));
		memset(szUnit   , 0x00, sizeof(szUnit)   );
		memset(szDesc   , 0x00, sizeof(szDesc)   );
	}
	~_tTAG(){};

	_tTAG( const _tTAG& _src ) {
		*this = _src;
	}

	int        nTagID;
	int        nGroupID;
	int        nFormat;
	char       szName[64];
	char       szValue[256];
	char       szMin[256];
	char       szMax[256];
	int        nDefaultUse;
	char       szDefault[256];
	char       szUnit[256];
	char       szDesc[256];

	_tTAG& operator = (const _tTAG& src)
	{
		if( this == &src ) return *this;

		this->nTagID      = 0;
		this->nGroupID    = 0;
		this->nFormat     = 0;
		this->nDefaultUse = 0;
		memset(this->szName   , 0x00, sizeof(this->szName)   );
		memset(this->szValue  , 0x00, sizeof(this->szValue)  );
		memset(this->szMin    , 0x00, sizeof(this->szMin)    );
		memset(this->szMax    , 0x00, sizeof(this->szMax)    );
		memset(this->szDefault, 0x00, sizeof(this->szDefault));
		memset(this->szUnit   , 0x00, sizeof(this->szUnit)   );
		memset(this->szDesc   , 0x00, sizeof(this->szDesc)   );

		this->nTagID   = src.nTagID ;
		this->nGroupID = src.nGroupID;
		this->nFormat  = src.nFormat;
		strcpy_s(this->szName, sizeof(szName), src.szName);
		strcpy_s(this->szValue, sizeof(szValue), src.szValue);
		strcpy_s(this->szMin, sizeof(szMin), src.szMin);
		strcpy_s(this->szMax, sizeof(szMax), src.szMax);
		this->nDefaultUse  = src.nDefaultUse;
		strcpy_s(this->szDefault, sizeof(szDefault), src.szDefault);
		strcpy_s(this->szUnit, sizeof(szUnit), src.szUnit);
		strcpy_s(this->szDesc, sizeof(szDesc), src.szDesc);

		return *this;
	}
};


//==============================================================================
// TBL_TAG_GROUP
//==============================================================================
class AFX_EXT_CLASS _tTAG_GROUP : public CObject
{
public:
	_tTAG_GROUP()
	{
		nID       = 0;
		nBaseAddr = 0;
		memset(szName   , 0x00, sizeof(szName)   );
		memset(szDesc   , 0x00, sizeof(szDesc)   );
	}
	~_tTAG_GROUP(){};

	int        nID;
	char       szName[64];
	int        nBaseAddr;
	char       szDesc[256];
};

//==============================================================================
// TBL_YIELD_RETEST_MANAGEMENT
//==============================================================================
class AFX_EXT_CLASS _tYIELD_RETEST_MANAGEMENT : public CObject
{
public:
	_tYIELD_RETEST_MANAGEMENT()
	{
		// imsi format slmoon
		CTime time = CTime::GetCurrentTime();
		strStartTime.Format( "%04d-%02d-%02d %02d:%02d:%02d", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond() );
		strStopTime.Format( "%04d-%02d-%02d %02d:%02d:%02d", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond() );
		nTestResultNum	= 0;
		nTesterID		= 0;
		nJavisID		= 0;
		nServerID		= 0;
		nErrCode		= 0;
		memset(szBuildName			, 0x00, sizeof(szBuildName)          );
		memset(szGAIAVersion		, 0x00, sizeof(szGAIAVersion)        );
		memset(szGRPTestVersion		, 0x00, sizeof(szGRPTestVersion)     );
		memset(szG2Version			, 0x00, sizeof(szG2Version)          );
		memset(szUIExploreVersion	, 0x00, sizeof(szUIExploreVersion)   );
		memset(szARMVersion			, 0x00, sizeof(szARMVersion)         );
		memset(szServerVersion		, 0x00, sizeof(szServerVersion)      );
		memset(szSerialNum			, 0x00, sizeof(szSerialNum)          );
		memset(szServerIP			, 0x00, sizeof(szServerIP)           );
		memset(szErrStr				, 0x00, sizeof(szErrStr)             );
		memset(szLogFileName		, 0x00, sizeof(szLogFileName)        );
	}
	~_tYIELD_RETEST_MANAGEMENT(){};

	unsigned int nTestResultNum;
	char		 szBuildName[32];
	char		 szGAIAVersion[128];
	char		 szGRPTestVersion[128];
	char		 szG2Version[128];
	char		 szUIExploreVersion[128];
	char		 szARMVersion[128];
	char		 szServerVersion[128];
	char		 szSerialNum[256];
	unsigned int nTesterID;
	unsigned int nJavisID;
	unsigned int nServerID;
	char		 szServerIP[256];
	unsigned int nErrCode;
	char		 szErrStr[256];
	char		 szLogFileName[4096];
	CString		 strStartTime;
	CString		 strStopTime;
};


#define MAX_2D_BARCODE_LEN (128)

typedef struct _ST_2D_BARCODE_INFO{
	int  nIndexArmNo;					// Arm number
	int  nSiteNo;						// user setting dut number	
	char szBarcode[MAX_2D_BARCODE_LEN]; // barcode string
	char szReadTime[64];

	_ST_2D_BARCODE_INFO(){
		memset(this, 0x00, sizeof(_ST_2D_BARCODE_INFO));
	}

	void clear()
	{
		nIndexArmNo = 0;
		nSiteNo     = 0;
		ZeroMemory(szBarcode, sizeof(szBarcode));
		ZeroMemory(szReadTime, sizeof(szReadTime));
	}
}ST_2D_BARCODE_INFO;

// add by SJI [160919] Add AAB Retest Rule Check
typedef struct _ST_AAB_CHECK_DATA{
	char szBarcode[MAX_2D_BARCODE_LEN];
	char szSiteNo[32];
	char szErrorStr[128];
	//char szErrorCode[64];
	char szBin[32];
	CString strRemark;
	int nTestingCnt;
	int nRealTestingCnt;

	void clear()
	{
		nTestingCnt = 0;
		nRealTestingCnt= 0;
		strRemark = _T("");
		ZeroMemory(szBarcode,   sizeof(szBarcode));
		ZeroMemory(szSiteNo,    sizeof(szSiteNo));
		ZeroMemory(szErrorStr,  sizeof(szErrorStr));
	//	ZeroMemory(szErrorCode, sizeof(szErrorCode));
		ZeroMemory(szBin,       sizeof(szBin));
	}

	_ST_AAB_CHECK_DATA(){
		clear();
	}
}ST_AAB_CHECK_DATA;

typedef struct _ST_DAILY_REPORT
{
	int nLoadCnt;
	int nUnloadCnt;
	int nPassCnt;
	int nART_FT_Total_Cnt;
	int nART_FT_Pass_Cnt;
	int nART_RT1_Total_Cnt;
	int nART_RT1_Pass_Cnt;
	int nART_RT2_Total_Cnt;
	int nART_RT2_Pass_Cnt;

	void Clear() {
		nLoadCnt = 0;
		nUnloadCnt = 0;
		nPassCnt = 0;
		nART_FT_Total_Cnt = 0;
		nART_FT_Pass_Cnt = 0;
		nART_RT1_Total_Cnt = 0;
		nART_RT1_Pass_Cnt = 0;
		nART_RT2_Total_Cnt = 0;
		nART_RT2_Pass_Cnt = 0;
	}
	_ST_DAILY_REPORT() {
		Clear();
	}

}ST_DAILY_REPORT;

typedef struct _ST_ERROR_MESSAGE {

	char	szEQP_ALID[32];		// 
	char	szPart[32];
	char	szMessage[450];		// 현재 장비 언어 설정에 따른 알람메세지
	CString	strOccurred_Time;	// YYYY-MM-DD HH:MM:SS.sss
	char	szRecovery[32];		// 알람 발생 후 사용자 조작 정보 기록.( SKIP, RETRY, DONE... )
	int		nStop_Time;			// [ SEC ] 장비가 정지되어있던 시간.
	char	szUser_Info[64];	// ex) ID : techwing, Level : Service
	int		nError_Level;		// 0 = Log, 1 = Message, 2 = Warning, 3 = Jam, 4 = Critical
	CTime   tOccured_Time;
	int     nPosition;          // Error 발생 Unit Postion
	int		nDuplicate;
	int		nOnLine_OffLine;
	char	szLotNo_Info[64];	// 해당 Error 발생 시 Lot기록

	_ST_ERROR_MESSAGE() {
		clear();
	}

	void clear() {
		ZeroMemory(szEQP_ALID, sizeof(szEQP_ALID));
		ZeroMemory(szPart, sizeof(szPart));
		ZeroMemory(szMessage, sizeof(szMessage));
		ZeroMemory(szRecovery, sizeof(szRecovery));
		ZeroMemory(szUser_Info, sizeof(szUser_Info));
		strOccurred_Time = _T("");
		nStop_Time = 0;
		nError_Level = 0;
		nPosition = 0;
		nDuplicate = 0;
		nOnLine_OffLine = 0;
		ZeroMemory(szLotNo_Info, sizeof(szLotNo_Info));
	}
}ST_ERROR_MESSAGE;

typedef struct _ST_LOTHISTORY {

	char	szEQP_ID[32];		// 장비 번호
	char	szPKGName[32];
	double	dDeviceSizeX;
	double	dDeviceSizeY;
	char	szLotNo_Info[64];
	CString	strLotEnd_Time;	// YYYY-MM-DD HH:MM:SS.sss
	int		nPartJamCnt[28];    //
	int		nLoadCnt;
	int		nUnloadCnt;
	int		nRetest[2];
	int		nPassCnt;
	int		nFailCnt;
	CString	strLotStart_Time;	// YYYY-MM-DD HH:MM:SS.sss
	int		nOnLine_OffLine;
	double	dAvgIndexTime;
	int		nUPH;
	CString	strLotRunning_Time; // HH:MM:SS.sss

	_ST_LOTHISTORY() {
		clear();
	}

	void clear() {
		ZeroMemory(szEQP_ID, sizeof(szEQP_ID));
		ZeroMemory(szPKGName, sizeof(szPKGName));
		dDeviceSizeX = 0;
		dDeviceSizeY = 0;
		ZeroMemory(szLotNo_Info, sizeof(szLotNo_Info));
		strLotEnd_Time = _T("");
		ZeroMemory(nPartJamCnt, sizeof(nPartJamCnt));
		nLoadCnt = 0;
		nUnloadCnt = 0;
		ZeroMemory(nRetest, sizeof(nRetest));
		nPassCnt = 0;
		nFailCnt = 0;
		strLotStart_Time = _T("");
		nOnLine_OffLine = 0;
		dAvgIndexTime = 0;
		nUPH = 0;
		strLotRunning_Time = _T("");
	}
}ST_LOTHISTORY;


typedef struct _ST_TEACH_LOG {

	void clear()
	{
		ZeroMemory(szRobot_Name, sizeof(szRobot_Name));
		ZeroMemory(szPos_Name, sizeof(szPos_Name));
		ZeroMemory(szAxis_Name, sizeof(szAxis_Name));
		dPosition_Before = 0.0;
		dPosition_After = 0.0;

		strOccurred_Time = _T("");
	}

	char		szRobot_Name[64];
	char		szPos_Name[64];				// 해당 위치의 ID
	char		szAxis_Name[64];				// TBL_MOTOR_PARAM.AXIS_NO_LOGICAL 

	double		dPosition_Before;		// Teaching Position 이전값[um]
	double		dPosition_After;		// Teaching Position 변경값[um]

	CString		strOccurred_Time;	// YYYY-MM-DD HH:MM:SS.sss

	_ST_TEACH_LOG() {
		clear();
	}
}ST_TEACH_LOG;


//==============================================================================
// TBL_CYLINDER_TIME
//==============================================================================
class AFX_EXT_CLASS _tCYLINDER_TIME : public CObject
{
public:
	_tCYLINDER_TIME()
	{
		nIdx = 0;
		strUnitName = _T("");
		dSpecMin = 0.0;
		dSpecMax = 0.0;
		dValue = 0.0;	
	}
	~_tCYLINDER_TIME() {};

	int			nIdx;
	CString     strUnitName;
	double		dSpecMin;
	double		dSpecMax;
	double		dValue;
};