#include "StdAfx.h"
#include "Motion_Cont_Imp.h"

#include "ConfigData.h"
#include "OneAxis_Sim.h"

#include "OneAxis_Z_PickerMotor.h"


#include "OneAxis_SoftServoEtherCAT.h"

/*=============================================================================
  AUTHOR   : 나명진  [2015-07-28]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CMotion_Cont_Imp::CMotion_Cont_Imp(void)
{
	m_nSimulResponseMode = 0;
	m_pDlgSimul = NULL;
	m_bInitBoardStatus = TRUE;
	m_bInitAxisStatus = FALSE;
	for(int i=0; i<MAX_AXIS_COUNT_ONE_PC; i++){
		m_aAxis[i] = NULL;
	}
	m_nDriverType = 0;
	m_nAxisCount = 0;
	m_nBoardType = 0;
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-07-28]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CMotion_Cont_Imp::~CMotion_Cont_Imp(void)
{
	if( m_pDlgSimul ){
		m_pDlgSimul->DestroyWindow();
		delete m_pDlgSimul;
		m_pDlgSimul = NULL;
	}

	for(int i=0; i<MAX_AXIS_COUNT_ONE_PC; i++){
		if( m_aAxis[i] != NULL ){
			delete m_aAxis[i];
			m_aAxis[i] = NULL;
		}
	}
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : 
  PARAMS   : szIniFileName = ini 파일 path
             nFileLen      = file length
  RETURN   : 0 = create fail
			 1  = create Success
  COMMENTS : Motion Board Open
			 //== MMC Board Return value
			 0: MMC_OK // 1: MMC_NOT_INITIALIZED // 2: MMC_TIMEOUT_ERR // 10: MMC_NON_EXIST
			 11:MMC_BOOT_OPEN_ERROR // 12 :MMC_CHKSUM_OPEN_ERROR

  COMMENTS : DB Data Read하여 OneAxis에 Data Setting
==============================================================================*/
int CMotion_Cont_Imp::Initialize(char* szIniFileName, int nFileLen)
{
	BOOL bInitStat = FALSE;
	CConfigData		cConfigHwData(szIniFileName);
	long lBoardAddr[MMC_BOARD_MAX_CNT] = {0,};
	char szSubTitle[128] = {0,};
	int  nReturn    = 0; 
	int  i;

	m_nBoardType = cConfigHwData.GetInt( "Motion Board",		 "Type",		    "0");
	m_nDriverType = m_nBoardType;
	int nBoardCount = 0;

	CDatabaseIF* pDbIf = g_SingleDB.CreateDB(szIniFileName, nFileLen);
	pDbIf->SELECT_MOTORPARAM_TBL(m_ListAxisInfoDb); // 모터 정보를 Logical Idx 오름 차순으로 불러 온다.

	g_bbxmnpClient.Initialize("sys_data\\PickerZAxisMotor.ini");
	g_bbxmnpClient.SetHandMatrix();

	if (m_nBoardType == DEF_MOTION_TYPE_ECAT_SOFT_SERVO)
	{
		m_nAxisCount = m_ListAxisInfoDb.m_fnGetCount();

		int nModuleServoCount = 0;
		std::map<int, ST_MOTION_INIT_PARAM> AxisMap;

		for (i = 0; i < m_nAxisCount; i++) {
			_tMOTOR_PARAM* pAxisInfo = (_tMOTOR_PARAM*)m_ListAxisInfoDb.m_fnGetData(i);

			if(pAxisInfo->eMotorCoordinate != _tMOTOR_PARAM::_eMotorCoordinate::Picker_Z)
				m_aAxis[i] = new OneAxis_SoftServoEtherCAT();
			else
				m_aAxis[i] = new OneAxis_Z_PickerMotor(); // Z Picker Motor


#if 0 // [FIXME][imsi] scale factor에 따른 motor in-position width 설정 변경. 검증 시 1로 수정 후 검증 필요.
			// um -> pulse : um * pulse/um = um * 1/(um/pulse)
			double dHwInPositionWidth_um = (MOTOR_INPOSITION_um < pAxisInfo->dScaleFactor) ? pAxisInfo->dScaleFactor * 2 : MOTOR_INPOSITION_um * 2;
			pAxisInfo->dHwInPositionWidth = dHwInPositionWidth_um / pAxisInfo->dScaleFactor;

			ST_MOTION_INIT_PARAM Param;
			Param.dInPosWidth = pAxisInfo->dScaleFactor * 10;//pAxisInfo->dHwInPositionWidth;
			Param.nMoveDirection = pAxisInfo->nMMC_Coordinate_Dir;
			Param.nPulsePerRound = pAxisInfo->nPulsePerRound;
			Param.nSlaveNumber = pAxisInfo->nAxisNo_Physical;
			AxisMap[pAxisInfo->nAxisNo_Physical] = Param;
#else
			ST_MOTION_INIT_PARAM Param;
			Param.dInPosWidth = MOTOR_INPOSITION_PULSE_COUNT;/*pAxisInfo->dScaleFactor * 10;*//*MOTOR_INPOSITION_PULSE_COUNT;*/
			Param.nMoveDirection = pAxisInfo->nMMC_Coordinate_Dir;
			Param.nPulsePerRound = pAxisInfo->nPulsePerRound;
			Param.nSlaveNumber = pAxisInfo->nAxisNo_Physical;
			AxisMap[pAxisInfo->nAxisNo_Physical] = Param;

			// In-Position width는 고정 10pulse로 한다.
			pAxisInfo->dHwInPositionWidth = MOTOR_INPOSITION_PULSE_COUNT;

			if (pAxisInfo->eMotorCoordinate == _tMOTOR_PARAM::_eMotorCoordinate::Picker_Z)
			{
				m_aAxis[i]->bbwmnpAxisInfo(pAxisInfo->nPartNo,
					pAxisInfo->dScaleFactor,
					pAxisInfo->nDir_ZR,
					pAxisInfo->nDirPulse,
					pAxisInfo->nDirMove);

				m_aAxis[i]->SetInitMotorParam(pAxisInfo->nSpd_Normal,
					pAxisInfo->nAcc_Normal,
					pAxisInfo->nDec_Normal,
					pAxisInfo->nSpd_ZR,
					pAxisInfo->nAcc_ZR,
					pAxisInfo->dLimit_Posi,
					pAxisInfo->dLimit_Nega,
					pAxisInfo->dScaleFactor);
				
			}
#endif

			m_aAxis[i]->SetAxisInitInfo(const_cast<TCHAR*>(pAxisInfo->strAxisName.GetString()),
				pAxisInfo->nAxisNo_Physical,
				i,
				pAxisInfo->dScaleFactor,
				pAxisInfo->nDir_ZR,
				pAxisInfo->nMMC_Encoder_Dir,
				pAxisInfo->nDirMove,
				pAxisInfo->nEncoder_Type,
				pAxisInfo->nMMC_Coordinate_Dir,
				pAxisInfo->nDirPulse,
				pAxisInfo->nDouble_Chk_Pulse,
				(MOTOR_TYPE)pAxisInfo->nMotorType,
				(MOTOR_SYNC_TYPE)pAxisInfo->nSyncType,
				pAxisInfo->nSlaveNo,
				pAxisInfo->bDegree,
				pAxisInfo->nPulsePerRound,
				pAxisInfo->dHwInPositionWidth);

			m_aAxis[i]->SetPartNo(pAxisInfo->nPartNo);


			m_aAxis[i]->SetVelAcc(pAxisInfo->nSpd_Normal,
				pAxisInfo->nAcc_Normal,
				pAxisInfo->nDec_Normal,
				pAxisInfo->nSpd_ZR,
				pAxisInfo->nAcc_ZR,
				pAxisInfo->dLimit_Posi,
				pAxisInfo->dLimit_Nega);


			m_aAxis[i]->m_strServoMaker = pAxisInfo->strBoardType;	//motor type을 설정 한다.
		}

		
		OneAxis_SoftServoEtherCAT::InitBoard(AxisMap);
		m_bInitAxisStatus = TRUE;

		int nSlaveCount = OneAxis_SoftServoEtherCAT::GetOnlineAxisCount();

		for (int i = 0; i < m_nAxisCount; i++)
		{
			auto* pAxis = m_aAxis[i];

			if (nModuleServoCount <= i)
			{// soft servo
				pAxis->EnableSWLimit(TRUE);
				pAxis->ApplyPlviModuleConfigration(false);
				if (pAxis->ApplyConfiguration(eAxisServoConfiguration::Default) == false)
				{
					m_bInitAxisStatus = FALSE;
					break;
				}
			}
			else
				pAxis->ApplyPlviModuleConfigration(true);
			//{// plvi module
			//	if (pAxis->ApplyPlviModuleConfigration(i) == false)
			//	{
			//		m_bInitAxisStatus = FALSE;
			//		break;
			//	}
			//}
		}

		bInitStat = m_bInitBoardStatus;

		if ((nSlaveCount- DEF_IO_MODULE_CNT) != (m_nAxisCount - DEF_ZAXIS_PIKCER_CNT))
		{
			m_bInitAxisStatus = FALSE;
			m_bInitBoardStatus = FALSE;
			return -1;
		}
		
	}
	else // Simulation
	{
		m_nAxisCount = cConfigHwData.GetLong("SIMUL MOTION CONFIG", "Axis Count",		"0");
		if( m_nAxisCount > m_ListAxisInfoDb.m_fnGetCount() ){
			m_nAxisCount = m_ListAxisInfoDb.m_fnGetCount();
		}

		for (i = 0; i < m_nAxisCount; i++) {
			m_aAxis[i] = new OneAxis_Sim;
			_tMOTOR_PARAM* pAxisInfo = (_tMOTOR_PARAM*)m_ListAxisInfoDb.m_fnGetData(i);

			m_aAxis[i]->SetAxisInitInfo(const_cast<TCHAR*>(pAxisInfo->strAxisName.GetString()),
				                        pAxisInfo->nAxisNo_Physical,
				                        i,
				                        pAxisInfo->dScaleFactor,
				                        pAxisInfo->nDir_ZR,
				                        pAxisInfo->nMMC_Encoder_Dir,
				                        pAxisInfo->nDirMove,
				                        pAxisInfo->nEncoder_Type,
				                        pAxisInfo->nMMC_Coordinate_Dir,
				                        pAxisInfo->nDirPulse,
				                        pAxisInfo->nDouble_Chk_Pulse,
				                        (MOTOR_TYPE)pAxisInfo->nMotorType,
				                        (MOTOR_SYNC_TYPE)pAxisInfo->nSyncType,
				                        pAxisInfo->nSlaveNo,
				                        pAxisInfo->bDegree,
				                        pAxisInfo->nPulsePerRound,
				                        pAxisInfo->dHwInPositionWidth);

			m_aAxis[i]->SetPartNo(pAxisInfo->nPartNo);

			m_aAxis[i]->SetVelAcc(pAxisInfo->nSpd_Normal,
				                  pAxisInfo->nAcc_Normal,
				                  pAxisInfo->nDec_Normal,
				                  pAxisInfo->nSpd_ZR,
				                  pAxisInfo->nAcc_ZR,
				                  pAxisInfo->dLimit_Posi,
				                  pAxisInfo->dLimit_Nega);
		}
		m_pDlgSimul = new CDlgMotionSim;
		m_pDlgSimul->Create(IDD_MOTION_SIM);
		m_pDlgSimul->ShowWindow(SW_HIDE);
		m_bInitBoardStatus = TRUE;

		bInitStat = m_bInitBoardStatus;
		m_bInitAxisStatus = TRUE;
	}	

	if (bInitStat == TRUE && m_nBoardType == DEF_MOTION_TYPE_SIM)
	{
		m_pDlgSimul = new CDlgMotionSim;
		m_pDlgSimul->Create(IDD_MOTION_SIM);
		m_pDlgSimul->ShowWindow(SW_HIDE);
	}
	
	return (int)bInitStat;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-28]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CMotion_Cont_Imp::Finalize()
{
	if (m_nBoardType == DEF_MOTION_TYPE_ECAT_SOFT_SERVO)
	{
		OneAxis_SoftServoEtherCAT::Close();
	}
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : GetAxisCount
  PARAMS   : 
  RETURN   : 
  COMMENTS : Axis count
==============================================================================*/
int CMotion_Cont_Imp::GetAxisCount()
{
	return m_nAxisCount;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : DB_Select_RobotTbl
  PARAMS   : nRobotNo =
             lpstData = Robot setting 에 필요한 struct data
  RETURN   : 
  COMMENTS : DB Robot Table Load
==============================================================================*/
int CMotion_Cont_Imp::GetMotorParam(int nRobotNo, _tMOTOR_PARAM& rData)
{
	_tMOTOR_PARAM* pMotorParam = (_tMOTOR_PARAM*)m_ListAxisInfoDb.m_fnGetData(nRobotNo);

	if( pMotorParam ){
		rData = *pMotorParam;
	}
	return 0;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-28]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
OneAxis* CMotion_Cont_Imp::operator [](int nIndex)
{
	if( nIndex < 0 || nIndex > m_nAxisCount ) return NULL;	
	return m_aAxis[nIndex];
}	


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
OneAxis* CMotion_Cont_Imp::GetAxis(int nIndex)
{
	if( nIndex < 0 || nIndex > m_nAxisCount ) return NULL;
	return m_aAxis[nIndex];
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CMotion_Cont_Imp::ShowDialogSimul()
{
	if( m_pDlgSimul ){
		m_pDlgSimul->ShowWindow(SW_SHOW);
	}
}

