#include "DEF_DB_TBL.h"


//==============================================================================
// TBL_MOTOR_PARAM
//==============================================================================
_tMOTOR_PARAM::_tMOTOR_PARAM()
{
	Init();
}

_tMOTOR_PARAM::_tMOTOR_PARAM(const _tMOTOR_PARAM& src)
{
	Copy(src);
}

_tMOTOR_PARAM& _tMOTOR_PARAM::operator = (const _tMOTOR_PARAM& src)
{
	if( this == &src ) return *this;
	Copy(src);
	return *this;
}

_tMOTOR_PARAM::~_tMOTOR_PARAM()
{
}

void _tMOTOR_PARAM::Init()
{
// 	ZeroMemory(szAxisGroup, sizeof(szAxisGroup));
// 	ZeroMemory(szCaption, sizeof(szCaption));
// 	nAxisNo_Logical			= 0;
// 	nAxisNo_Physical		= 0;
// 	bDisableAxis			= FALSE;
// 	dScaleFactor			= 0.0;
// 	dAngle					= 0.0;
// 	dMotorRate				= 0.0;
// 	dPulleyRate				= 0.0;
// 	dServoPulse				= 0.0;
// 	nDir_ZR					= 0;
// 	nDirPulse				= 0;
// 	nDirMove				= 0;
// 	nDirType				= 0;
// 	nEncoderType			= 0;
// 	dLimit_Posi				= 0.0;
// 	dLimit_Nega				= 0.0;
// 	nSpd_Normal				= 0;
// 	nAcc_Normal				= 0;
// 	nDec_Normal				= 0;
// 	nSpd_ZR					= 0;
// 	nAcc_ZR					= 0;
// 	eMotorCoordinate        = _tMOTOR_PARAM::X;

	strBoardType.Empty();
	nAxisNo_Logical = 0;
	nAxisNo_Physical = 0;
	nPartNo = 0;
	strAxisName.Empty();
	dScaleFactor = 0.0;
	nPulsePerRound = 0;
	nDir_ZR = 0;
	nDirPulse = 0;
	nDirMove = 0;
	dLimit_Posi = 0.0;
	dLimit_Nega = 0.0;
	nSpd_Normal = 0;
	nAcc_Normal = 0;
	nDec_Normal = 0;
	nSpd_ZR = 0;
	nAcc_ZR = 0;
	nDouble_Chk_Pulse = 0;
	nMMC_Encoder_Dir = 0;
	nEncoder_Type = 0;
	nMMC_Coordinate_Dir = 0;
	nMotorType = 0;
	nSyncType = 0;
	nSlaveNo = 0;
	dHwInPositionWidth = 0.0;
	eMotorCoordinate = _tMOTOR_PARAM::X;
}

void _tMOTOR_PARAM::Copy(const _tMOTOR_PARAM& src)
{
// 	memcpy(szAxisGroup, src.szAxisGroup, sizeof(szAxisGroup));
// 	memcpy(szCaption, src.szCaption, sizeof(szCaption));
// 	nAxisNo_Logical			= src.nAxisNo_Logical	;
// 	nAxisNo_Physical		= src.nAxisNo_Physical  ;
// 	bDisableAxis			= src.bDisableAxis	    ;
// 	dScaleFactor			= src.dScaleFactor	    ;
// 	dAngle					= src.dAngle			;
// 	dMotorRate				= src.dMotorRate		;
// 	dPulleyRate				= src.dPulleyRate		;
// 	dServoPulse				= src.dServoPulse		;
// 	nDir_ZR					= src.nDir_ZR			;
// 	nDirPulse				= src.nDirPulse		    ;
// 	nDirMove				= src.nDirMove		    ;
// 	nDirType				= src.nDirType		    ;
// 	nEncoderType			= src.nEncoderType	    ;
// 	dLimit_Posi				= src.dLimit_Posi		;
// 	dLimit_Nega				= src.dLimit_Nega		;
// 	nSpd_Normal				= src.nSpd_Normal		;
// 	nAcc_Normal				= src.nAcc_Normal		;
// 	nDec_Normal				= src.nDec_Normal		;
// 	nSpd_ZR					= src.nSpd_ZR			;
// 	nAcc_ZR					= src.nAcc_ZR			;
// 	eMotorCoordinate        = src.eMotorCoordinate  ;

	strBoardType = src.strBoardType;
	nAxisNo_Logical = src.nAxisNo_Logical;
	nAxisNo_Physical = src.nAxisNo_Physical;
	nPartNo = src.nPartNo;
	strAxisName = src.strAxisName;
	dScaleFactor = src.dScaleFactor;
	nPulsePerRound = src.nPulsePerRound;
	nDir_ZR = src.nDir_ZR;
	nDirPulse = src.nDirPulse;
	nDirMove = src.nDirMove;
	dLimit_Posi = src.dLimit_Posi;
	dLimit_Nega = src.dLimit_Nega;
	nSpd_Normal = src.nSpd_Normal;
	nAcc_Normal = src.nAcc_Normal;
	nDec_Normal = src.nDec_Normal;
	nSpd_ZR = src.nSpd_ZR;
	nAcc_ZR = src.nAcc_ZR;
	nDouble_Chk_Pulse = src.nDouble_Chk_Pulse;
	nMMC_Encoder_Dir = src.nMMC_Encoder_Dir;
	nEncoder_Type = src.nEncoder_Type;
	nMMC_Coordinate_Dir = src.nMMC_Coordinate_Dir;
	nMotorType = src.nMotorType;
	nSyncType = src.nSyncType;
	nSlaveNo = src.nSlaveNo;
	dHwInPositionWidth = src.dHwInPositionWidth;
	eMotorCoordinate = src.eMotorCoordinate;
}


//==============================================================================
//
//==============================================================================
