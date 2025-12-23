#pragma once

#include <afx.h>

#define EVT_TESTERIF_RS232_RECEIVE_CMD	(1000)
#define EVT_TESTERIF_RS232_SEND_CMD		(1001)

#define FIND_AUTO_SPEED_CHECK_CNT		(5)
#define UPH_COUNT						(10)

// GPIB COMMAND USER MESSAGE
#define UM_GPIB_BINON					(WM_USER + 1000)
#define UM_GPIB_ECHO					(WM_USER + 1001)
#define UM_GPIB_ECHO_OK					(WM_USER + 1002)
#define UM_GPIB_ECHO_NG					(WM_USER + 1003)
#define UM_GPIB_FULLSITE				(WM_USER + 1003)
#define UM_GPIB_FULLSITE_OK				(WM_USER + 1004)
#define UM_GPIB_TMP						(WM_USER + 1005)
#define UM_GPIB_SOAK					(WM_USER + 1006)
#define UM_GPIB_MAP						(WM_USER + 1007)
#define UM_GPIB_FR						(WM_USER + 1008)
#define UM_GPIB_IDN						(WM_USER + 1009)
#define UM_GPIB_NAME					(WM_USER + 1010)

/////////////////////////////////////////////////////////////////////////////////== GPIB COMMAND <START>
// All Handler


#define TESTER_CMD_HTOT_SRQ41					_T("SQR41")				// H->T
#define TESTER_CMD_HTOT_SRQ44					_T("SQR44")				// H->T
#define TESTER_CMD_HTOT_FULLSITE				_T("FULLSITES")			// H->T
#define TESTER_CMD_HTOT_ECHO					_T("ECHO:")				// H->T
#define TESTER_CMD_HTOT_VERSION					_T("VERSION")			// H->T
#define TESTER_CMD_HTOT_CURRENTALM				_T("CURRENTALM?")		// H->T
#define TESTER_CMD_HTOT_LOTCLEARED				_T("LOTCLEARED")
#define TESTER_CMD_HTOT_FR0						_T("FR0")


////////////////////////////////////////////////////////////////////////////////////

#define TESTER_CMD_TTOH_FULLSITE_UPPER			_T("FULLSITES?")		// T->H
#define TESTER_CMD_TTOH_FULLSITE_LOWER			_T("Fullsites?")		// T->H
#define TESTER_CMD_TTOH_BINON					_T("BINON:")			// T->H
#define TESTER_CMD_TTOH_ECHO_OK					_T("ECHOOK")			// T->H
#define TESTER_CMD_TTOH_ECHO_NG					_T("ECHONG")			// T->H
#define TESTER_CMD_TTOH_CURRENTALM				_T("CURRENTALM_")		// T->H
#define TESTER_CMD_TTOH_LOTCLEAR				_T("LOTCLEAR?")
#define TESTER_CMD_TTOH_VERSION					_T("VERSION?")	
#define TESTER_CMD_TTOH_FR						_T("FR?")




#define TESTER_CMD_FULLSITE_OK				_T("FULLSITES_OK")		// T->H

// ========================================================= << Logic Handler >> =====
#define TESTER_CMD_TMP						_T("TMP?")				// T->H
#define TESTER_CMD_MAP						_T("MAP?")				// T->H
#define TESTER_CMD_SOAK						_T("SOAK?")				// T->H
#define TESTER_CMD_FR						_T("FR?")				// T->H
#define TESTER_CMD_IDN						_T("*IDN?")				// T->H
#define TESTER_CMD_NAME						_T("NAME?")				// T->H
// ========================================================= << Memory Handler >> ====
#define TESTER_CMD_FULLSITE_NG0				_T("FULLSITES_NG0")
#define TESTER_CMD_FULLSITE_NG1				_T("FULLSITES_NG1")
#define TESTER_CMD_FULLSITE_NG2				_T("FULLSITES_NG2")
#define TESTER_CMD_LOTNAME					_T("LOTNAME")
#define TESTER_CMD_LOT_NAME					_T("LOTNAME:")
#define TESTER_CMD_TRAY_ID					_T("TRAYID?")
#define TESTER_CMD_ENABLE_DUT				_T("ENABLEDUT?")
#define TESTER_CMD_ERR_AUTOCAL				_T("ERRAUTOTCAL:")
#define TESTER_CMD_TESTER_ERR				_T("TESTER_ERR")
#define TESTER_CMD_HDLS_STATUS				_T("HDLSTATUS?")
#define TESTER_CMD_HDLS_SETTEMP				_T("HDLSETTEMP?")
#define TESTER_CMD_STOP						_T("STOP")
#define TESTER_CMD_RECIPE_NAME				_T("RECIPENAME")
#define TESTER_CMD_SET_TEMP					_T("SETTEMP ?")
#define TESTER_CMD_TEMP_MODE				_T("TEMPMODE ?")
#define TESTER_CMD_LOTTOTAL					_T("LOTTOTAL ?")
#define TESTER_CMD_LOTTOTAL2				_T("LOTTOTAL2?")
#define TESTER_CMD_LOTENDTOTAL				_T("LOTENDTOTAL?")
#define TESTER_CMD_JAMCODE					_T("JAMCODE?")
#define TESTER_CMD_JAMCOUNT					_T("JAMCOUNT?")
#define TESTER_CMD_JAMQUE					_T("JAMQUE?")
#define TESTER_CMD_LOTJAM					_T("LOTJAM?")
#define TESTER_CMD_JAM						_T("JAM?")
#define TESTER_CMD_LOTERROR					_T("LOTERROR?")
#define TESTER_CMD_LOTCLEAR					_T("LOTCLEAR?")
#define TESTER_CMD_LOTRETESTCLEAR			_T("LOTRETESTCLEAR ?")
#define TESTER_CMD_TESTALARM				_T("TESTALARM")
#define TESTER_CMD_ALARM_FFFF				_T("ALARM:FFFF")
#define TESTER_CMD_ALARM					_T("ALARM")
#define TESTER_CMD_VERSION_UPPER_SPACE		_T("VERSION ?")
#define TESTER_CMD_VERSION_UPPER			_T("VERSION?")
#define TESTER_CMD_VERSION_LOWER_SPACE		_T("Version ?")
#define TESTER_CMD_VERSION_LOWER			_T("Version?")
#define TESTER_CMD_BARCODE					_T("BARCODE ?")
#define TESTER_CMD_HANDLERLOTEND0			_T("HANDLERLOTEND 0")
#define TESTER_CMD_HANDLERLOTEND1			_T("HANDLERLOTEND 1")
/////////////////////////////////////////////////////////////////////////////////== GPIB COMMAND <END>
//enum TIF_COMM_DATA		
//{ 
//	eTIF_CMD_GPIB_SRQ = 0, 
//	eTIF_CMD_GPIB_FULLSITE, 
//	eTIF_CMD_GPIB_ECHO 
//};

enum COMMUNICATION_TYPE	{ eCOM_GPIB = 0,	eCOM_RS232C, eCOM_DIO, eCOM_SIMUL };
enum GP_BOARD			{ eNIGPIB = 0, eCTGPIB };																//GPIB
enum RS_PARITY			{ RS_PARITY_NONE = 0, RS_PARITY_ODD, RS_PARITY_EVEN, RS_PARITY_MARK, RS_PARITY_SPACE };	//RS
enum RS_STOPBIT			{ RS_STOP1 = 0, RS_STOP1_5, RS_STOP2 };													//RS
enum eSIGNAL_LEVEL		{ ePOSITIVE = 0, eNEGATIVE };															//DIO
enum eSIGNAL_CHANNEL	{ eEACH = 0, eSINGLE };																	//DIO
enum eSAMPLING_TIMES	{ eTWO_TIMES = 0, eONE_TIME };															//DIO
enum eMULTIPLE_OPT		{ eNONE = 0, eMULTIPLE_CATE };															//DIO
enum eTESTER_CALIB_OPT	{ eINVALID = 0, eVALID, eSITEMAP };														//DIO
enum eCATE_DATA_TYPE	{ eONE_BY_ONE = 0, eBINARY };															//DIO
enum eDUT_SIG_TYPE{																								//DIO
	eDST_NONE = 0, eDST_POSITIVE_LEVEL, eDST_POSITIVE_PULSE,
	eDST_NEGATIVE_LEVEL, eDST_NEGATIVE_PULSE, eDST_ADVAN_2CH, eDST_ADVAN_4CH, eDST_SITE_CHANGE_INFORMATION,
	eDST_MAX
};
enum eCATE_CH_TYPE{																								//DIO
	e4BITS_4CH          = 0,	e5BITS_4CH             ,
	e8BITS_2CH             ,	e10BITS_2CH            ,
	e5BITSPE_4CH           ,	e5BITSPO_4CH           ,
	e10BITSPE_2CH          ,	e10BITSPO_2CH          ,
};

class AFX_EXT_CLASS CTesterIFCmd
{
public:
	CTesterIFCmd(){
		strCmd.Empty();
		strSndData.Empty();
		strRcvData.Empty();
	};
	~CTesterIFCmd(){};

	CString strCmd;
	CString strSndData;	// Handler에서 Tester로 보낼 Data
	CString strRcvData;	// Tester에서 Handler로 받은 Data

	void TransReciveData( CString strReciveData );
};

typedef struct _ST_TESTERIF_PARAM_RCV_CMD
{
	int  nCmd;
	int  nLen;
	char szBin[128];		// BIN 결과를 1,1,2,3 형식으로 받음.
	int  nPara_X;
	int  nPara_Y;
	BOOL bSiteData[256];	// Site 사용 유무,, 1번 site TRUE, 2번 Site FALSE

	_ST_TESTERIF_PARAM_RCV_CMD()
	{
		memset(this, 0x00, sizeof(_ST_TESTERIF_PARAM_RCV_CMD));
	}
}ST_TESTERIF_PARAM_RCV_CMD, *LPST_TESTERIF_PARAM_RCV_CMD;

#ifndef __FN_CALLBACK_TESTER_RECEIVE__
typedef int (*FN_CALLBACK_TESTER_RECEIVE)(CTesterIFCmd*);
#endif //__FN_CALLBACK_TESTER_RECEIVE__

typedef struct _ST_TESTER_MESSAGE_MAP{
	CString strCmd;
	FN_CALLBACK_TESTER_RECEIVE fnReceiveCallback;
}ST_TESTER_MESSAGE_MAP;

typedef struct _ST_COMM_DATA
{
	int nIF_Type;		// 0: GPIB, 1:RS232C, 2:DIO
	int nGPIB_Adress;	//==GPIB initialize data
	//==RS232C initialize data
	int nPort;
	int nRate;
	int nParity;
	int nSize;
	int nStop;

	int		nID;							// 중복 불가
	char	szName[MAX_PATH];				// 중복 가능
	int		nSTART_SIG_LOGIC;				// Positive, Negative
	int		nSTART_SIG_CHANNEL;				// Each, Single
	int		nSTART_SIG_PULSEWIDTH;			// usec
	int		nDUT_SIG_TYPE;					// 
	int		nDUT_SIG_BEFORE_ON;				// usec
	int		nDUT_SIG_AFTER_OFF;				// usec
	int		nEND_SIG_LOGIC;					// Positive, Negative
	int		nEND_SIG_SAMPLING;				// Two times, One times
	int		nEND_SIG_SAMPLING_LIMIT_TIME;	// usec
	int		nEND_SIG_AFTER_SAMPLING_TIME;	// usec
	int		nMULTIPULE;						// None, Multiple Category
	int		nTESTER_CAL;					// Tester Calibration
	int		nCATEGORY_SIG_LOGIC;			// Positive, Negative
	int     nCATEGORY_SIG_CH_BIT_LEN;		// 4, 5, 8, 10, 5PE, 5PO, 10PE, 10PO
	int		nCATEGORY_SIG_DATA_TYPE;		// OneByOne, Binary
	int		nCATEGORY_SIG_1CH_READ_TIMEING;	// usec
	int		nCATEGORY_SIG_2CH_READ_TIMEING;	// usec
	int		nCATEGORY_SIG_3CH_READ_TIMEING;	// usec
	int		nCATEGORY_SIG_4CH_READ_TIMEING;	// usec

	char*	szInitFileName;					// log file name
	int		nFileNameLen;					// log file name length
	bool	bSimulation;					// simulation 사용여부.

	_ST_COMM_DATA()
	{
		memset(this, 0x00, sizeof(_ST_COMM_DATA));
	}

	void MakeDefault()
	{
		nGPIB_Adress					= 16;

		nPort							= 9;
		nRate							= 9600;
		nParity							= RS_PARITY_NONE;
		nSize							= 8;
		nStop							= RS_STOP1;

		nSTART_SIG_LOGIC				= eNEGATIVE;
		nSTART_SIG_CHANNEL				= eEACH;
		nSTART_SIG_PULSEWIDTH			= 1000;
		nDUT_SIG_TYPE					= eNONE;
		nDUT_SIG_BEFORE_ON				= 200;
		nDUT_SIG_AFTER_OFF				= 200;
		nEND_SIG_LOGIC					= eNEGATIVE;
		nEND_SIG_SAMPLING				= eTWO_TIMES;
		nEND_SIG_SAMPLING_LIMIT_TIME	= 500000;
		nEND_SIG_AFTER_SAMPLING_TIME	= 30000;
		nMULTIPULE						= eNONE;
		nTESTER_CAL						= eINVALID;
		nCATEGORY_SIG_LOGIC				= eNEGATIVE;
		nCATEGORY_SIG_CH_BIT_LEN		= e5BITS_4CH;
		nCATEGORY_SIG_DATA_TYPE			= eBINARY;
		nCATEGORY_SIG_1CH_READ_TIMEING	= 500;
		nCATEGORY_SIG_2CH_READ_TIMEING	= 500;
		nCATEGORY_SIG_3CH_READ_TIMEING	= 500;
		nCATEGORY_SIG_4CH_READ_TIMEING	= 500;

		szInitFileName					= _T( "" );
		nFileNameLen					= 0;
	};
}ST_COMM_DATA;