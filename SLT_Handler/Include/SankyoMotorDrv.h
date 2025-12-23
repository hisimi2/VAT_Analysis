#pragma once

#include <vector>
/*===============================================================================
header(protocol header(1)+receive address(1)) + 
data(ctrl_code(1)+cmd_code(1)+data(n)) + 
crc(2)
=================================================================================*/
namespace Sankyo
{
	const int LEN_DATA_CONTROL = 1;
	const int LEN_DATA_COMMAND = 1;
	const int LEN_FCS = 2;
	const int MAX_STATION_COUNT = 32;
	enum eErrCode { 
		_eErr_Successfull=0, _eErr_AbnormalComplete    , _eErr_NotDefinedCommand, 
		_eErr_MessageFormat, _eErr_InvalidOperationMode, _eErr_InvaidityInternalStatus, 
		_eErr_ParamRangeOver, _eErr_AccessDeny, _eErr_LockFreeFailed, _eErr_Reserved
	};
	enum eMsgDir { 
		_eMsgDir_Command=0, _eMsgDir_Reply=1	
	};
	enum eCmdCode {
		_eCmd_Code_NOP             =  (0x00),
		_eCmd_Code_GetParam2       =  (0x04),
		_eCmd_Code_GetParam4       =  (0x05),
		_eCmd_Code_SetParam2       =  (0x07),
		_eCmd_Code_SetParam4       =  (0x08),
		_eCmd_Code_UnlockParamAll  =  (0x0A),
		_eCmd_Code_SaveParamAll    =  (0x0B),
		_eCmd_Code_GetStateValue2  =  (0x10),
		_eCmd_Code_GetStateValue4  =  (0x11),
		_eCmd_Code_ReadEA05Data    =  (0x1E),
		_eCmd_Code_ClearEA05Data   =  (0x1F),
		_eCmd_Code_ReadEA05Data_Ex =  (0x62),
	};

	enum eStatusCode{
		_eStsCode_Alarm_0         = 0,
		_eStsCode_Alarm_1         = 1,
		_eStsCode_ParallerIO      = 16,
		_eStsCode_CommandTorque   = 113,
		_eStsCode_TorqueRate      = 131,
	};

	enum eOperatorSelection{
		eOpActionNoAction=-1, eOpActionSkip=0, eOpActionRetry,
	};

	enum eRunMode{eRunMode_None = -1, eRunMode_Manual=0,eRunMode_Auto};


	const int MAX_DATA_LEN = 29;

	typedef struct _ST_SANKYO_DATA{
		double dTorque;
		_ST_SANKYO_DATA()
		{
			dTorque = 999.0;
		}
	}ST_SANKYO_DATA;

	typedef struct _ST_SANKYO_INIT_INFO{
		int nPort;
		int nRate;
		int nData;
		int nStop;
		int nParity;
		char szLogFileName[256];
		int nLogKeepPeriod;
		bool bSimulMode;
		_ST_SANKYO_INIT_INFO()
		{
			ZeroMemory(this, sizeof(_ST_SANKYO_INIT_INFO));
		}
	}ST_SANKYO_INIT_INFO;


	//======================================
	//
	//======================================
	struct _ST_HEADER{
		// data
		union {
			unsigned char  _ucPH;
			struct {
				unsigned char _ucDataLen  : 5;
				unsigned char _ucType     : 3;
			}stProtocolHeader;
		}union_protocol_header;
		unsigned char  _ucAddress;

		// constructor
		_ST_HEADER()
		{
			union_protocol_header.stProtocolHeader._ucType = 1; // ∞Ì¡§
			union_protocol_header.stProtocolHeader._ucDataLen  = 0;
			_ucAddress = 0;
		}

		// member function
		void set(int nAddress, int nLen)
		{
			union_protocol_header.stProtocolHeader._ucDataLen  = nLen;
			_ucAddress = nAddress;
		}

		unsigned char get_data_len(){ return union_protocol_header.stProtocolHeader._ucDataLen; }
		unsigned char get_address() { return _ucAddress; }
	};

	//======================================
	//
	//======================================
	union ut_control_code{
		unsigned char _ucCtrl_Code;
		struct {
			unsigned char _bfError    : 4;
			unsigned char _bfReserved : 2;
			unsigned char _bfToggle   : 1;
			unsigned char _bfDir      : 1;
		}stCtrlCode;
		ut_control_code()
		{
			stCtrlCode._bfError    = 0;
			stCtrlCode._bfReserved = 0;
			stCtrlCode._bfToggle   = 0;
			stCtrlCode._bfDir      = _eMsgDir_Command;
		}
	};

	struct _ST_DATA{
		union ut_control_code union_control_code;
		unsigned char  _ucCommand_Code;
		unsigned char szData[32];
		_ST_DATA()
		{
			_ucCommand_Code = 0;
			ZeroMemory(szData, sizeof(unsigned char)*32 );
		}
		unsigned char get_error() { return union_control_code.stCtrlCode._bfError; }
		unsigned char get_dir() { return union_control_code.stCtrlCode._bfDir; }
		unsigned char get_toggle() { return union_control_code.stCtrlCode._bfToggle; }
		unsigned char get_command_code() { return _ucCommand_Code; }
	};

	//======================================
	//
	//======================================
	struct _ST_CRC{
		unsigned char  _Hi;
		unsigned char  _Lo;
		_ST_CRC()
		{
			_Hi = 0;
			_Lo = 0;
		}
	};

	struct _ST_PACKET{
		struct _ST_HEADER header;
		struct _ST_DATA   data;
		struct _ST_CRC    crc;
	};

	//=======================================================================================
	// CLASS : CSeriComFrame
	//=======================================================================================
	struct SeriComFrame
	{
		char  szFrame[256];
		SeriComFrame()
		{
			ZeroMemory(szFrame, sizeof(szFrame));
		}
	};

	union _SHORT_SWAP{
		short sValue;
		struct{
			unsigned short _bfHi : 8;
			unsigned short _bfLo : 8;
		}stValue;
		_SHORT_SWAP()
		{
			sValue = 0;
		}
		_SHORT_SWAP(unsigned short sLo, unsigned short sHi)
		{	
			stValue._bfLo = sLo;
			stValue._bfHi = sHi;
		};
	};
};

