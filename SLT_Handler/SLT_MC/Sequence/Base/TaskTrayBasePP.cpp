#include "StdAfx.h"
#include "TaskTrayBasePP.h"
#include <vector>
#include <algorithm>

CTaskTrayBasePP::CTaskTrayBasePP()
{
	m_eMyDeviceStageNum = eDeviceDataStage( -1 );
	m_nRobotId_Idx = -1;
	NDM_TrayPP_RetryCnt = -1;
	NDM_TM_TrayPP_CylDownWait = -1;
	NDM_TM_TrayPP_Vacuum = -1;
	NDM_TM_TrayPP_Blow = -1;
	m_nLoadTrayAutoSkipCount = 0;
	m_bCyclePause = FALSE;
	m_vImpactPkr.clear();
	m_bInitialized = FALSE;

	m_vSafetyFailPicker[ePickerSafetySensorCheckType::eSafetySensorCheck].clear();
	m_vSafetyFailPicker[ePickerSafetySensorCheckType::ePlaceSafetySensorCheck].clear();

	m_sSaftyFailPicker[ePickerSafetySensorCheckType::eSafetySensorCheck] = _T("");
	m_sSaftyFailPicker[ePickerSafetySensorCheckType::ePlaceSafetySensorCheck] = _T("");

	ZeroMemory(m_nHand_i_VacSen, sizeof(m_nHand_i_VacSen));
	ZeroMemory(m_nHand_o_Vac, sizeof(m_nHand_o_Vac));
	ZeroMemory(m_nHand_o_Blw, sizeof(m_nHand_o_Blw));
	ZeroMemory(m_nHand_o_Pick, sizeof(m_nHand_o_Pick));
	ZeroMemory(m_nHand_o_Reset, sizeof(m_nHand_o_Reset));
	ZeroMemory(m_nVATvisionLED_o_ONOFF, sizeof(m_nVATvisionLED_o_ONOFF));
	ZeroMemory(m_nBbxmnpAxis, sizeof(m_nBbxmnpAxis));

}

CTaskTrayBasePP::~CTaskTrayBasePP()
{

}


void CTaskTrayBasePP::ASSERT_AXIS_IDX( eMotor nAxisIdx )
{
	ASSERT( m_vMotor.empty() == false );
	ASSERT( ( nAxisIdx >= 0 ) && ( nAxisIdx < CTaskTrayBasePP::eAxisMax ) );
}

int CTaskTrayBasePP::GetMotorCount()
{
	int nCount = 0;
	for( auto motor : m_vMotor ) {
		if( motor != nullptr ) nCount++;
	}
	return nCount;
}

bool CTaskTrayBasePP::EmptyMotor( eMotor eAxisIdx )
{
	return ( m_vMotor[ eAxisIdx ] == nullptr );
}

/*=============================================================================
	AUTHOR   : 나명진  [2023-05-15]

	PARAMS   : [in] nAxisIdx :
				   motor 축의 논리적인 id

	RETURN   : OneAxis 객체에 대한 포인터.

	COMMENTS : motor축 상태 취득 및 제어를 위한 OneAxis 객체에 대한 포인터를 반환
==============================================================================*/
OneAxis* CTaskTrayBasePP::GetAxis( eMotor nAxisIdx )
{
	ASSERT_AXIS_IDX( nAxisIdx );

	return m_vMotor[ nAxisIdx ];
}

void CTaskTrayBasePP::DivSpecOutPicker(std::vector<CPoint> &vPickerNormal, std::vector<CPoint> &vPickerSpecOut)
{
	std::vector<CPoint> vPickerTmp;
	vPickerTmp.clear();

	for (int nI = 0; nI < (int)vPickerNormal.size(); nI++)
	{
		if (GetPickerAlignStatus(vPickerNormal[nI].x, vPickerNormal[nI].y) == TRUE)
		{
			vPickerSpecOut.push_back(CPoint(vPickerNormal[nI].x, vPickerNormal[nI].y));
		}
		else
		{
			vPickerTmp.push_back(CPoint(vPickerNormal[nI].x, vPickerNormal[nI].y));
		}
	}
	if (vPickerTmp.size() != 0)
	{
		vPickerNormal.clear();
		vPickerNormal = vPickerTmp;
	}
}


/*=============================================================================
	AUTHOR   : 나명진  [2023-05-15]
	PARAMS   : [in] nLocStage :
				   Hand가 작업 할 Stage에 대한 Index

			   [in] vPicker   :
				   8개의 Picker 중 작업 할 Picker 정보를 (0,0) ~ (3,1) 형태로 전달

			   [out] pdOffsetX,pdOffsetY :
				   작업 할 Picker를 바탕으로 X,Y축을 얼마나 Shift 해야 하는지 계산하여 반환
	RETURN   : error code
	COMMENTS :
==============================================================================*/
int CTaskTrayBasePP::GetPickerOffset( eLocIdxTrayPP nLocStage, std::vector<CPoint> vPicker, double* pdOffsetX, double* pdOffsetY )
{
	double dPickerDist[ 2 ] = { 0.0, };
	int nHandMatrix[ MAX_TRAY_PP_CNT_Y ][ MAX_TRAY_PP_CNT_X ];

	// [2x-pitch]
	int nX_PitchType = 1;
	int nY_PitchType = 1;

	// C-Tray에서 작업
	if( ( ( nLocStage >= eLocIdxTrayPP::LOC_IDX_TRAY_PP_CONV_1 ) && ( nLocStage <= eLocIdxTrayPP::LOC_IDX_TRAY_PP_CONV_7 ) ) ||
		( nLocStage == eLocIdxTrayPP::LOC_IDX_TRAY_PP_FEEDER ) )
	{
		dPickerDist[ 0 ] = g_DMCont.m_dmTrayPP.GN( NDM5_Tray_Pitch_X );
		dPickerDist[ 1 ] = g_DMCont.m_dmTrayPP.GN( NDM5_Tray_Pitch_Y );

		if( g_DMCont.m_dmTrayPP.GN( NDM5_Tray_Pitch_X ) < DEF_TRAY_PP_MAX_X_PITCH_NARROW ) {
			nX_PitchType = 2;
		}
		if( g_DMCont.m_dmTrayPP.GN( NDM5_Tray_Pitch_Y ) < DEF_TRAY_PP_MAX_Y_PITCH_NARROW ) {
			nY_PitchType = 2;
		}
	}
	// Loading Table에서 작업
	else if( ( nLocStage >= eLocIdxTrayPP::LOC_IDX_TRAY_PP_TBL_1 ) && ( nLocStage <= eLocIdxTrayPP::LOC_IDX_TRAY_PP_TBL_4 ) )
	{
		dPickerDist[ 0 ] = g_DMCont.m_dmTrayPP.GN( NDM5_TrayPP_LdTbl_Pitch_X );
		dPickerDist[ 1 ] = g_DMCont.m_dmTrayPP.GN( NDM5_TrayPP_LdTbl_Pitch_Y );
	}
	// Buffer Table에서 작업
	else
	{
		dPickerDist[ 0 ] = g_DMCont.m_dmTrayPP.GN( NDM5_TrayPP_Buffer_Pitch_X );
		dPickerDist[ 1 ] = g_DMCont.m_dmTrayPP.GN( NDM5_TrayPP_Buffer_Pitch_Y );
	}

	// Matrix 0로 초기화
	memset( nHandMatrix, 0x00, sizeof( nHandMatrix ) );

	if( vPicker.empty() == false )
	{
		for( std::vector<CPoint>::size_type i = 0; i < vPicker.size(); i++ ) {
			int x = vPicker[ i ].x;
			int y = vPicker[ i ].y;
			nHandMatrix[ y ][ x ] = 1;
		}
		// hand의 Rear Picker 전체가 비었으면 Y축 pitch 만큼 더 이동 시킨다.
		int nSum = 0;
		for( int i = 0; i < MAX_TRAY_PP_CNT_X; i++ ) {
			nSum += nHandMatrix[ 0 ][ i ];
		}
		if( nSum == 0 ) {
			*pdOffsetY += ( dPickerDist[ 1 ] * nY_PitchType );
		}

		//int nOffSetXPos = 0;
		// hand의 1열~8열까지 비었으면 X축 pitch 만큼 더 이동 시킨다.
		for( int x = 0; x < MAX_TRAY_PP_CNT_X; x++ ) {
			nSum = 0;
			BOOL bStopCalc = FALSE;
			for( int y = 0; y < MAX_TRAY_PP_CNT_Y; y++ ) {

				BOOL bChkVac = FALSE;
// 				if (ChkVacuum(eOnOffState::eOn, CPoint(x, y)) != ERR_NO_ERROR) {
// 					bChkVac = TRUE;
// 				}
				

				nSum += nHandMatrix[y][x];
				

				if( nHandMatrix[ y ][ x ] || bChkVac == TRUE) { // Left 부터 확인하여 둘다 없는 경우가 아니라면 빠져 나가야 한다.
					bStopCalc = TRUE;
				}
			}

			if( bStopCalc == TRUE ) {
				//nOffSetXPos = x;
				break;
			}

			if( nSum == 0 ) {
				// [2x-pitch] 2배수 hand type일 경우 2배수 하도록 한번 더 빼줌.
				if( (nLocStage >= eLocIdxTrayPP::LOC_IDX_TRAY_PP_CONV_1 && nLocStage <= eLocIdxTrayPP::LOC_IDX_TRAY_PP_CONV_7) /*&& nX_PitchType == 2*/
					|| nLocStage == eLocIdxTrayPP::LOC_IDX_TRAY_PP_FEEDER)
				{
					*pdOffsetX -= ( dPickerDist[ 0 ] * nX_PitchType );
				} else
				{
					*pdOffsetX -= dPickerDist[ 0 ];
				}
			}
		}

		if( nLocStage >= eLocIdxTrayPP::LOC_IDX_TRAY_PP_TBL_1 && nLocStage <= eLocIdxTrayPP::LOC_IDX_TRAY_PP_TBL_4 )
		{
			//*pdOffsetX += m_stOffSetPickerX.stPickerX[ nLocStage - LOC_IDX_TRAY_PP_SHT_1 ][ nOffSetXPos ];
		}

		// 그 이외에는 Rear Left를 기준으로 이동한다.
	}

	return ERR_NO_ERROR;
}

/*=============================================================================
	AUTHOR   : 나명진  [2023-05-15]
	PARAMS   :
	RETURN   : error code
	COMMENTS :
==============================================================================*/
int CTaskTrayBasePP::GetTeachPos( eLocIdxTrayPP			nLocStage,
								  CPoint				Pocket,
								  std::vector<CPoint>	vPicker,
								  eTeachPosZ			nLocZ,
								  double*				pdPosX,
								  double*				pdPosY,
								  double*				pdPosZ,
								  double*				pdPitchX,
								  double*				pdPitchY )
{
	ASSERT( nLocStage >= 0 && nLocStage < MAX_TRAY_PP_LOC_COUNT );
	ASSERT( nLocZ >= 0 && nLocZ < CTaskTrayBasePP::eMAX_ZPOS_COUNT );

	int _idx = m_nRobotId_Idx;
	
	int nLocStageConv = TrayHandLocIdxConvertor(_idx, nLocStage);

	double dTPos[ CTaskTrayBasePP::eAxisMax ] = { 0.0, };
	if (_idx == RIDX_TRAY_PP_1)
	{
		dTPos[CTaskTrayBasePP::eAxisX] = g_DMCont.m_Teach.GetTeachPos(RID_TRAY_PP_(_idx), nLocStageConv, POS_IDX_TRAY_LOAD_PP_X);

		if (nLocZ == CTaskTrayBasePP::eZPOS_PICK)
		{
			dTPos[CTaskTrayBasePP::eAxisZ] = g_DMCont.m_Teach.GetTeachPos(RID_TRAY_PP_(_idx), nLocStageConv, POS_IDX_TRAY_LOAD_PP_Z_PICK);
		}
		else if (nLocZ == CTaskTrayBasePP::eZPOS_PLACE)
		{
			dTPos[CTaskTrayBasePP::eAxisZ] = g_DMCont.m_Teach.GetTeachPos(RID_TRAY_PP_(_idx), nLocStageConv, POS_IDX_TRAY_LOAD_PP_Z_PLACE);
		}
		else
		{
			dTPos[CTaskTrayBasePP::eAxisZ] = g_DMCont.m_Teach.GetTeachPos(RID_TRAY_PP_(_idx), nLocStageConv, POS_IDX_TRAY_LOAD_PP_Z_SAFETY);
		}

		dTPos[CTaskTrayBasePP::eAxisPitchX] = g_DMCont.m_Teach.GetTeachPos(RID_TRAY_PP_(_idx), nLocStageConv, POS_IDX_TRAY_LOAD_PP_X_PITCH);
		dTPos[CTaskTrayBasePP::eAxisPitchY] = g_DMCont.m_Teach.GetTeachPos(RID_TRAY_PP_(_idx), nLocStageConv, POS_IDX_TRAY_LOAD_PP_Y_PITCH);
	}
	else
	{
		dTPos[CTaskTrayBasePP::eAxisX] = g_DMCont.m_Teach.GetTeachPos(RID_TRAY_PP_(_idx), nLocStageConv, POS_IDX_TRAY_PP_X);
		dTPos[CTaskTrayBasePP::eAxisY] = g_DMCont.m_Teach.GetTeachPos(RID_TRAY_PP_(_idx), nLocStageConv, POS_IDX_TRAY_PP_Y);

		if (nLocZ == CTaskTrayBasePP::eZPOS_PICK)
		{
			dTPos[CTaskTrayBasePP::eAxisZ] = g_DMCont.m_Teach.GetTeachPos(RID_TRAY_PP_(_idx), nLocStageConv, POS_IDX_TRAY_PP_Z_PICK);
		}
		else if (nLocZ == CTaskTrayBasePP::eZPOS_PLACE)
		{
			dTPos[CTaskTrayBasePP::eAxisZ] = g_DMCont.m_Teach.GetTeachPos(RID_TRAY_PP_(_idx), nLocStageConv, POS_IDX_TRAY_PP_Z_PLACE);
		}
		else
		{
			dTPos[CTaskTrayBasePP::eAxisZ] = g_DMCont.m_Teach.GetTeachPos(RID_TRAY_PP_(_idx), nLocStageConv, POS_IDX_TRAY_PP_Z_SAFETY);
		}

		dTPos[CTaskTrayBasePP::eAxisPitchX] = g_DMCont.m_Teach.GetTeachPos(RID_TRAY_PP_(_idx), nLocStageConv, POS_IDX_TRAY_PP_X_PITCH);
		dTPos[CTaskTrayBasePP::eAxisPitchY] = g_DMCont.m_Teach.GetTeachPos(RID_TRAY_PP_(_idx), nLocStageConv, POS_IDX_TRAY_PP_Y_PITCH);
	}
	// 1. Picker Rear Left(좌상)을 기준으로 거리 계산
	double dPocketDist[ 2 ] = { 0.0, };
	double dLoadingTablePitch[ 2 ] = { 0.0, };

	dLoadingTablePitch[ 0 ] = g_DMCont.m_dmShuttle.GN( NDM3_LdTbl_Pitch_X );
	dLoadingTablePitch[ 1 ] = g_DMCont.m_dmShuttle.GN( NDM3_LdTbl_Pitch_Y );

	double dCTrayPitch[ 2 ] = { 0.0, };
	dCTrayPitch[ 0 ] = g_DMCont.m_dmTrayPP.GN( NDM5_Tray_Pitch_X );
	dCTrayPitch[ 1 ] = g_DMCont.m_dmTrayPP.GN( NDM5_Tray_Pitch_Y );

	if( ( nLocStage >= LOC_IDX_TRAY_PP_CONV_1 && nLocStage <= LOC_IDX_TRAY_PP_CONV_7 ) ||
		( nLocStage == LOC_IDX_TRAY_PP_FEEDER ) )
	{
		dPocketDist[ 0 ] += ( dCTrayPitch[ 0 ] * Pocket.x );
		dPocketDist[ 1 ] -= ( dCTrayPitch[ 1 ] * Pocket.y );
	} else if( nLocStage >= LOC_IDX_TRAY_PP_TBL_1 && nLocStage <= LOC_IDX_TRAY_PP_TBL_4 )
	{
		dPocketDist[ 0 ] += ( dLoadingTablePitch[ 0 ] * Pocket.x );
		dPocketDist[ 1 ] -= ( dLoadingTablePitch[ 1 ] * Pocket.y );
	} else
	{
		dCTrayPitch[ 0 ] = g_DMCont.m_dmTrayPP.GN( NDM5_TrayPP_Buffer_Pitch_X );
		dCTrayPitch[ 1 ] = g_DMCont.m_dmTrayPP.GN( NDM5_TrayPP_Buffer_Pitch_Y );

		dPocketDist[ 0 ] += ( dCTrayPitch[ 0 ] * Pocket.x );
		dPocketDist[ 1 ] -= ( dCTrayPitch[ 1 ] * Pocket.y );
	}

	dTPos[ 0 ] += dPocketDist[ 0 ];
	dTPos[ 1 ] += dPocketDist[ 1 ];

	// 2. 집어야 할 Picker 감안하여 Offset 계산.
	double dPickerOffset[ 2 ] = { 0.0, };
	GetPickerOffset( nLocStage, vPicker, &dPickerOffset[ 0 ], &dPickerOffset[ 1 ] );
	dTPos[ 0 ] += dPickerOffset[ 0 ];
	dTPos[ 1 ] += dPickerOffset[ 1 ];

	// 3. ideal pos -> calibration
	*pdPosX = dTPos[ 0 ];
	*pdPosY = dTPos[ 1 ];
	*pdPosZ = dTPos[ 2 ];
	*pdPitchX = dTPos[ 3 ];
	*pdPitchY = dTPos[ 4 ];

	return ERR_NO_ERROR;
}

/*=============================================================================
	AUTHOR   : 나명진  [2023-05-15]
	PARAMS   :
	RETURN   : error code
	COMMENTS :
==============================================================================*/
double CTaskTrayBasePP::GetTeachPos( eLocIdxTrayPP       nLocStage,
									 CPoint              Pocket,
									 std::vector<CPoint> vPicker,
									 eTeachPosZ          nLocZ,
									 eMotor              eAxisIdx )
{
	ASSERT_AXIS_IDX( eAxisIdx );

	double dTPos[ CTaskTrayBasePP::eAxisMax ] = { 0.0, };
	GetTeachPos( nLocStage, Pocket, vPicker, nLocZ, &dTPos[ eAxisX ], &dTPos[ eAxisY ], &dTPos[ eAxisZ ], &dTPos[ eAxisPitchX ], &dTPos[ eAxisPitchY ] );

	return dTPos[ eAxisIdx ];
}

/*=============================================================================
  AUTHOR   : 나명진  [2023-05-15]

  PARAMS   : [in] eLocStage
				 :

			 [in] Pocket
				 :

			 [in] vPicker
				 :

			 [in] eLocZ
				 :

			 [in] eAxis
				 : -1 = 모든 축의 위치가 맞는지 점검 한다.

  RETURN   :  0 = OK
			 -1 = Motion 동작 중 임.
			 -2 = 동작은 멈췄으나 Position Range 안에 들어오지 않음.

  COMMENTS : Motor의 현재 위치가 지정한 위치에 있는지 점검 한다.
==============================================================================*/
int CTaskTrayBasePP::ChkMotorPos( eLocIdxTrayPP       eLocStage,
								  CPoint              Pocket,
								  std::vector<CPoint> vPicker,
								  eTeachPosZ          eLocZ,
								  eMotor              eAxisIdx /*= eAxisAll */ )
{
	ASSERT( eLocStage >= 0 && eLocStage < MAX_TRAY_PP_LOC_COUNT );
	ASSERT_AXIS_IDX( eAxisIdx );

	int i = 0;
	double dTeachPos[ CTaskTrayBasePP::eAxisMax ] = { 0.0, };
	double dCurPos[ CTaskTrayBasePP::eAxisMax ] = { 0.0, };

	GetTeachPos( eLocStage, Pocket, vPicker, eLocZ, &dTeachPos[ eAxisX ], &dTeachPos[ eAxisY ], &dTeachPos[ eAxisZ ], &dTeachPos[ eAxisPitchX ], &dTeachPos[ eAxisPitchY ] );

	for( i = 0; i < CTaskTrayBasePP::eAxisMax; i++ ) {
		OneAxis* pMotor = static_cast< OneAxis* >( m_vMotor[ i ] );
		if( pMotor == nullptr ) continue;
			pMotor->GetCurrentPos(dCurPos[i]);
	}

	// all axis
	if( eAxisIdx == CTaskTrayBasePP::eAxisAll ) {
		for( i = 0; i < CTaskTrayBasePP::eAxisMax; i++ ) {
			OneAxis* pMotor = static_cast< OneAxis* >( m_vMotor[ i ] );
			if( pMotor == nullptr ) continue;
			if( pMotor->MotionDone() != ERR_NO_ERROR ) {
				return -1;
			}

			if( fabs( dCurPos[ i ] - dTeachPos[ i ] ) > MOTOR_INPOSITION ) {
				return -2;
			}
		}
	} else {
		OneAxis* pMotor = static_cast< OneAxis* >( m_vMotor[ eAxisIdx ] );
		if( pMotor == nullptr ) {
			return -3;
		}
		if( pMotor->MotionDone() != ERR_NO_ERROR) {
			return -1;
		}

		if( fabs( dCurPos[ eAxisIdx ] - dTeachPos[ eAxisIdx ] ) > MOTOR_INPOSITION ) {
			return -2;
		}
	}

	return ERR_NO_ERROR;
}

int CTaskTrayBasePP::ChkMotorPos( eLocIdxTrayPP eLocStage, CPoint Pocket, std::vector<CPoint> vPicker, eTeachPosZ eLocZ, double dOffset, eMotor eAxisIdx /*= eAxisAll */ )
{
	ASSERT( eLocStage >= 0 && eLocStage < MAX_TRAY_PP_LOC_COUNT );
	ASSERT_AXIS_IDX( eAxisIdx );

	int i = 0;
	double dTeachPos[ CTaskTrayBasePP::eAxisMax ] = { 0.0, };
	double dCurPos[ CTaskTrayBasePP::eAxisMax ] = { 0.0, };

	GetTeachPos( eLocStage, Pocket, vPicker, eLocZ, &dTeachPos[ 0 ], &dTeachPos[ 1 ], &dTeachPos[ 2 ], &dTeachPos[ 3 ], &dTeachPos[ 4 ] );

	for( i = 0; i < CTaskTrayBasePP::eAxisMax; i++ ) {
		OneAxis* pMotor = static_cast< OneAxis* >( m_vMotor[ i ] );
		pMotor->GetCurrentPos(dCurPos[i]);
	}

	if( eAxisIdx == CTaskTrayBasePP::eAxisAll ) {
		for( i = 0; i < CTaskTrayBasePP::eAxisMax; i++ ) {
			OneAxis* pMotor = static_cast< OneAxis* >( m_vMotor[ i ] );
			if( pMotor == nullptr ) continue;
			if( pMotor->MotionDone() != ERR_NO_ERROR) {
				return -1;
			}

			if( fabs( dCurPos[ i ] - ( dTeachPos[ i ] + dOffset ) ) > MOTOR_INPOSITION ) {
				return -2;
			}
		}
	} else {
		OneAxis* pMotor = static_cast< OneAxis* >( m_vMotor[ eAxisIdx ] );
		if( pMotor == nullptr ) {
			return -3;
		}
		if( pMotor->MotionDone() != ERR_NO_ERROR) {
			return -1;
		}

		if( fabs( dCurPos[ eAxisIdx ] - ( dTeachPos[ eAxisIdx ] + dOffset ) ) > MOTOR_INPOSITION ) {
			return -2;
		}
	}

	return ERR_NO_ERROR;
}



bool CTaskTrayBasePP::WhereIsMotorPosZ()
{
	// get need data
	double dCurPosZ = 0;
	GetAxis(eAxisZ)->GetCurrentPos(dCurPosZ);

	std::vector<CPoint> vCmdPicker;
	vCmdPicker.push_back( CPoint( 0, 0 ) );

	// get all load table z safety pos
	std::vector<double> vLdTblZPos;
	double dTmpPosZ = 0.0;
	for( int i = 0; i < eMaxLoadTblCount; i++ ) {
		dTmpPosZ = GetTeachPos( static_cast< eLocIdxTrayPP >( LOC_IDX_TRAY_PP_TBL_1 + i ), CPoint( 0, 0 ), vCmdPicker, eZPOS_SAFETY, eAxisZ );
		//if(dTmpPosZ == 0) continue;
		vLdTblZPos.push_back( dTmpPosZ );
	}

	//변경 필요
	//table 두개만 들어오도록

	// calc min z pos
	std::vector<double>::iterator vdRetMinZ;
	vdRetMinZ = std::min_element( std::begin( vLdTblZPos ), std::end( vLdTblZPos ) );

	// checking z axis is safety??
	if( fabs( dCurPosZ - ( *vdRetMinZ ) ) < 500.0 || ( *vdRetMinZ >= dCurPosZ ) ) {
		return true;
	}

	return false;
}

bool CTaskTrayBasePP::WhereIsMotorPosZEach(CBbxmnpClient::_ePICK_MOTOR eMotor, _eBbxmnpAxis eAxis)
{
	// get need data
	double dCurPosZ = g_pBbxmnpClient->GetActualPos(eMotor, eAxis);
	/*GetAxis(eAxisZ)->GetCurrentPos(dCurPosZ);*/

	std::vector<CPoint> vCmdPicker;
	vCmdPicker.push_back(CPoint(0, 0));

	// get all load table z safety pos
	std::vector<double> vLdTblZPos;
	double dTmpPosZ = 0.0;
	for (int i = 0; i < eMaxLoadTblCount; i++) {
		dTmpPosZ = GetTeachPos(static_cast<eLocIdxTrayPP>(LOC_IDX_TRAY_PP_TBL_1 + i), CPoint(0, 0), vCmdPicker, eZPOS_SAFETY, eAxisZ);
		//if(dTmpPosZ == 0) continue;
		vLdTblZPos.push_back(dTmpPosZ);
	}

	//변경 필요
	//table 두개만 들어오도록

	// calc min z pos
	std::vector<double>::iterator vdRetMinZ;
	vdRetMinZ = std::min_element(std::begin(vLdTblZPos), std::end(vLdTblZPos));

	// checking z axis is safety??
	if (fabs(dCurPosZ - (*vdRetMinZ)) < 500.0 || (*vdRetMinZ >= dCurPosZ)) {
		return true;
	}

	return false;
}

/*=============================================================================
	AUTHOR   : 나명진  [2023-05-15]
	PARAMS   :
	RETURN   : error code
	COMMENTS : 현재 로봇의 위치가 어느 지역에 있는지 확인하기 위한 함수.
==============================================================================*/
int CTaskTrayBasePP::WhereIsMotorPos( eLocIdxTrayPP& pos_xy, bool& pos_z_safety )
{
	WhereIsMotorPosXY( pos_xy );
	pos_z_safety = WhereIsMotorPosZ();

	return ERR_NO_ERROR;
}

int CTaskTrayBasePP::Chk_Z_Safety_Sen_All()
{
	std::vector<CPoint> vPicker;
	for (int y = 0; y < MAX_TRAY_PP_CNT_Y; y++) {
		for (int x = 0; x < MAX_TRAY_PP_CNT_X; x++) {
			vPicker.push_back(CPoint(x, y));
		}
	}
	return Chk_Z_Safety_Sen(vPicker);
}

int CTaskTrayBasePP::Chk_Z_Safety_Sen(std::vector<CPoint> vPicker)
{
	bool bSafety = false;
	_eBbxmnpAxis picker_base;

	if (GetRobotIdIdx() == 0) {
		picker_base = eBMAxis_LOAD_TRAY_Picker01;
	}
	else {
		picker_base = eBMAxis_UNLOAD_TRAY_Picker01;
	}

	// picker를 지정하지 않았으면 모든 picker를 확인하는 것으로 본다.
	if (vPicker.empty())
	{
		for (int y = 0; y < MAX_TRAY_PP_CNT_Y; y++) {
			for (int x = 0; x < MAX_TRAY_PP_CNT_X; x++) {
				vPicker.push_back(CPoint(x, y));
			}
		}
	}

	int nChk = 0;
	for (auto it = vPicker.begin(); it != vPicker.end(); it++) {
		_eBbxmnpAxis picker = (_eBbxmnpAxis)((int)picker_base + (*it).x + ((*it).y*MAX_TRAY_PP_CNT_X));
		if (m_vMotor[eMotor::eAxisZ]->IsHomeSensorOn(picker) == TRUE) {
			nChk++;
		}

	}

	int nErr = -1;
	if (GetRobotIdIdx() == 0) { nErr = ERR_TRAY_PP_1_Z_PICKER_SAFETY_SENSOR_NOT_DETECTED; }
	else { nErr = ERR_TRAY_PP_2_Z_PICKER_SAFETY_SENSOR_NOT_DETECTED; }

	// home sensor가 모두 감지되어 있으면 safety
	if (nChk == vPicker.size())
	{
		bSafety = true;
	}

	return (bSafety==true)?ERR_NO_ERROR : nErr;
}

int CTaskTrayBasePP::Chk_Z_Safety_Sen_Check_Type(std::vector<CPoint> vPicker, ePickerSafetySensorCheckType _ePickerHomeSensorCheckType)
{
	bool bSafety = false;
	m_vSafetyFailPicker[_ePickerHomeSensorCheckType].clear();
	m_sSaftyFailPicker[_ePickerHomeSensorCheckType] = _T("");
	CString strPlaceErrPkr = _T("");

	_eBbxmnpAxis picker_base;

	if (GetRobotIdIdx() == 0) {
		picker_base = eBMAxis_LOAD_TRAY_Picker01;
	}
	else {
		picker_base = eBMAxis_UNLOAD_TRAY_Picker01;
	}

	// picker를 지정하지 않았으면 모든 picker를 확인하는 것으로 본다.
	if (vPicker.empty())
	{
		for (int y = 0; y < MAX_TRAY_PP_CNT_Y; y++) {
			for (int x = 0; x < MAX_TRAY_PP_CNT_X; x++) {
				vPicker.push_back(CPoint(x, y));
			}
		}
	}

	int nChk = 0;
	for (auto it = vPicker.begin(); it != vPicker.end(); it++) {
		_eBbxmnpAxis picker = (_eBbxmnpAxis)((int)picker_base + (*it).x + ((*it).y*MAX_TRAY_PP_CNT_X));
		if (m_vMotor[eMotor::eAxisZ]->IsHomeSensorOn(picker) == TRUE) {
			nChk++;
			if (g_IO.GetDriverType() != _DIO_TYPE_SIMUL_)
			{
				if (_ePickerHomeSensorCheckType == ePlaceSafetySensorCheck) //Place Check
				{
					// Place 위치로 이동 후, Home Sensor 감지 되면 알람 처리
					strPlaceErrPkr.Format("[X:%d, Y:%d]", (*it).x + 1, (*it).y + 1);
					m_sSaftyFailPicker[_ePickerHomeSensorCheckType] += strPlaceErrPkr;
					m_vSafetyFailPicker[_ePickerHomeSensorCheckType].push_back(CPoint((*it).x, (*it).y));
				}
			}
		}
		else {
			if (_ePickerHomeSensorCheckType == eSafetySensorCheck) //Safety Sensor Check
			{
				strPlaceErrPkr.Format("[X:%d, Y:%d]", (*it).x + 1, (*it).y + 1);
				m_sSaftyFailPicker[_ePickerHomeSensorCheckType] += strPlaceErrPkr;
				m_vSafetyFailPicker[_ePickerHomeSensorCheckType].push_back(CPoint((*it).x, (*it).y));
			}
		}

	}

	// home sensor가 모두 감지되어 있으면 safety
	if (nChk == vPicker.size())
	{
		bSafety = true;
	}

	int nErr = -1;
	if (GetRobotIdIdx() == 0) { nErr = ERR_TRAY_PP_1_Z_PICKER_SAFETY_SENSOR_NOT_DETECTED; }
	else{ nErr = ERR_TRAY_PP_2_Z_PICKER_SAFETY_SENSOR_NOT_DETECTED; }
	

	return (bSafety == true) ? ERR_NO_ERROR : nErr;
}




int CTaskTrayBasePP::do_MovePos( eLocIdxTrayPP nLocStage, CPoint Pocket, std::vector<CPoint> vPicker, eTeachPosZ nLocZ, eMotor nAxisIdx, int nSeqSpdRate /*= 100*/, double dPosOffset /*= 0.0 */ )
{
	ASSERT( 0 <= nLocStage && nLocStage < eLocIdxTrayPP::MAX_TRAY_PP_LOC_COUNT );
	ASSERT( 0 <= nAxisIdx && nAxisIdx < eMotor::eAxisMax );
	ASSERT( 0 <= nSeqSpdRate && nSeqSpdRate <= 100 );

	OneAxis* pMotor = GetAxis( nAxisIdx );
	if( pMotor == nullptr ) {
		return ERR_MOTOR_DATA_IS_NULL_POINTER;
	}

	double dTPos = GetTeachPos( nLocStage, Pocket, vPicker, nLocZ, nAxisIdx );

	int nIndexSpd[ eMaxTrayPPCount ][ eAxisMax ] = {
		{ NDM5_SPD_LD_TrayPP_X, NDM5_SPD_Gripper_Y, NDM5_SPD_LD_TrayPP_Z, NDM5_SPD_LD_TrayPP_X_Pitch, NDM5_SPD_LD_TrayPP_Y_Pitch },
		{ NDM5_SPD_UL_TrayPP_X, NDM5_SPD_UL_TrayPP_Y, NDM5_SPD_UL_TrayPP_Z, NDM5_SPD_UL_TrayPP_X_Pitch, NDM5_SPD_UL_TrayPP_Y_Pitch }
	};
	int nIndexAcc[ eMaxTrayPPCount ][ eAxisMax ] = {
		{ NDM5_ACC_LD_TrayPP_X, NDM5_ACC_Gripper_Y, NDM5_ACC_LD_TrayPP_Z, NDM5_ACC_LD_TrayPP_X_Pitch, NDM5_ACC_LD_TrayPP_Y_Pitch },
		{ NDM5_ACC_UL_TrayPP_X, NDM5_ACC_UL_TrayPP_Y, NDM5_ACC_UL_TrayPP_Z, NDM5_ACC_UL_TrayPP_X_Pitch, NDM5_ACC_UL_TrayPP_Y_Pitch }
	};
	int nIndexDcc[ eMaxTrayPPCount ][ eAxisMax ] = {
		{ NDM5_DCC_LD_TrayPP_X, NDM5_DCC_Gripper_Y, NDM5_DCC_LD_TrayPP_Z, NDM5_DCC_LD_TrayPP_X_Pitch, NDM5_DCC_LD_TrayPP_Y_Pitch },
		{ NDM5_DCC_UL_TrayPP_X, NDM5_DCC_UL_TrayPP_Y, NDM5_DCC_UL_TrayPP_Z, NDM5_DCC_UL_TrayPP_X_Pitch, NDM5_DCC_UL_TrayPP_Y_Pitch }
	};


	double dRateSpd = ( double )g_DMCont.m_dmTrayPP.GN( nIndexSpd[ m_nRobotId_Idx ][ nAxisIdx ] );
	int    nRateAcc = g_DMCont.m_dmTrayPP.GN( nIndexAcc[ m_nRobotId_Idx ][ nAxisIdx ] );
	int    nRateDcc = g_DMCont.m_dmTrayPP.GN( nIndexDcc[ m_nRobotId_Idx ][ nAxisIdx ] );

	double dRetSpd = 0;
	int    nRetAcc = 0, nRetDcc = 0;

	double dMtSpd = pMotor->GetSpeed();
	double dMtAcc = pMotor->GetAcc();
	double dMtDcc = pMotor->GetDcc();


	dRetSpd = ( dMtSpd * ( dRateSpd / 100.0 ) ) * ( nSeqSpdRate / 100.0 );
	nRetAcc = ( int )( dMtAcc - ( dMtAcc * 0.75 ) * ( nRateAcc / 100.0 ) ); // 설정되어 있는 값에서 spd rate가 높을 수록 acc/dcc를 감소 시켜준다. 가장 빠를 때가 설정 속도의 95%뺀 acc/dcc로 함.
	nRetDcc = ( int )( dMtDcc - ( dMtDcc * 0.75 ) * ( nRateDcc / 100.0 ) );

	if (nAxisIdx == eMotor::eAxisZ) {}
	else {
		if (nRetAcc < 100) nRetAcc = 100;
		if (nRetDcc < 100) nRetDcc = 100;
	}
	

	//거리에 따른 속도 조절 [ X Axis, Y Axis ]

	double dCurPos = 0;
	pMotor->GetCurrentPos(dCurPos);
	double dGapPos = fabs(dTPos - dCurPos);
	if (dGapPos < 100000.0)
	{
		dRetSpd = dRetSpd * 0.75;
	}
	

	// Tray Unload PP Stacker 영역 Linear Y Offset 추가 [7/3/2025 donghyun.shin]
	double dLinearYOffset = 0.0;
	double dTrayCntY = (double)g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_Y);
	double dPocketY = (double)Pocket.y + 1;

	if (m_nRobotId_Idx == eTrayPP_2_UD 
		&& nAxisIdx == eAxisY 
		&& nLocStage >= eLocIdxTrayPP::LOC_IDX_TRAY_PP_CONV_1 && nLocStage <= LOC_IDX_TRAY_PP_CONV_7)
	{		
		double dULTrayPP_Linear_Offset = (double)g_DMCont.m_dmTrayPP.GN(NDM5_TM_UDTrayPP_Linear_Offset);
		dLinearYOffset = (double)(dPocketY / dTrayCntY) * (dULTrayPP_Linear_Offset);
	}


	double dFTPos = dTPos + dPosOffset + dLinearYOffset;
	MakeLog( "%s : Final Target Pos : %.3fmm, Org Target Pos : %.3fmm, Pos Offset : %.3fmm, dLinearYOffset : =%.3fmm", pMotor->GetAxisName(), dFTPos / 1000.0, dTPos / 1000.0, dPosOffset / 1000.0, dLinearYOffset / 1000.0);
	int nErr = -1;

	BOOL bZpickerNouseOffset = FALSE;

	if (nLocStage == eLocIdxTrayPP::LOC_IDX_TRAY_PP_SAFETY || nLocZ == eZPOS_SAFETY)
	{
		bZpickerNouseOffset = TRUE;
	}

	//VAT 측정 시, 속도 조절
//  	if (g_DMCont.m_dmVAT.GB(BDM14_VAT_TEACHING_USE) == TRUE) {
//  		dRetSpd = dMtSpd * (VAT_MOTOR_SPEED / 100.0);
//  		bZpickerNouseOffset = TRUE;		
//  	}
//  	else { //Running시, Picker Gap 저장
// 		if (nAxisIdx == eAxisZ && nLocStage != LOC_IDX_TRAY_PP_SAFETY) {
// 			g_pBbxmnpClient->SetZAutoTeachingData((GetRobotIdIdx() == RIDX_TRAY_PP_1) ? g_VATCommonData.m_stVATZTeachInfo_temp.dLoadTrayPp_Loc_PickerGap[nLocStage] : g_VATCommonData.m_stVATZTeachInfo_temp.dUnloadTrayPp_Loc_PickerGap[nLocStage]);
// 		}
//  	}

	BOOL bApplyBacklash = FALSE;
	if (nAxisIdx == eAxisX || nAxisIdx == eAxisY)
	{
		bApplyBacklash = TRUE;
	}


	int nBoardType = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);
	//BOOL bVatOffsetUse = g_DMCont.m_dmVAT.GB(BDM14_VAT_MD_USE);

	if (nAxisIdx == eAxisZ) {	
		if (nLocStage != LOC_IDX_TRAY_PP_SAFETY) {
			_SetFlatnessOffset(nLocStage); //현재 LocStage에 맞는 Offset 저장.
		}

		nErr = pMotor->Move(vPicker, dFTPos, dRetSpd, nRetAcc, nRetDcc, nBoardType, bZpickerNouseOffset/*bVatOffsetUse*/);
	}
	else {
		nErr = pMotor->Move(dFTPos, dRetSpd, nRetAcc, nRetDcc, bApplyBacklash);
	}

	return nErr;
}

int CTaskTrayBasePP::do_MovePos_Pcd_backlash(std::vector<CPoint> vPicker, double dAbsPosition, eMotor nAxisIdx, int nSeqSpdRate /*= 5*/, double dPosOffset /*= 0.0*/, BOOL bIsApplyBcklsh /*= FALSE*/ )
{
	ASSERT( nAxisIdx >= 0 && nAxisIdx < eAxisMax );
	ASSERT( nSeqSpdRate >= 0 && nSeqSpdRate <= 100 );

	OneAxis* pMotor = ( OneAxis* )m_vMotor[ nAxisIdx ];

	/*User Spd, Acc, Dcc & Seq Spd, Acc, Dcc 를 가지고 계산*/

	int nIndexSpd[ eMaxTrayPPCount ][ eAxisMax ] = { {NDM5_SPD_LD_TrayPP_X, NDM5_SPD_Gripper_Y, NDM5_SPD_LD_TrayPP_Z, NDM5_SPD_LD_TrayPP_X_Pitch, NDM5_SPD_LD_TrayPP_Y_Pitch},
												 {NDM5_SPD_UL_TrayPP_X, NDM5_SPD_UL_TrayPP_Y, NDM5_SPD_UL_TrayPP_Z, NDM5_SPD_UL_TrayPP_X_Pitch, NDM5_SPD_UL_TrayPP_Y_Pitch } };
	int nIndexAcc[ eMaxTrayPPCount ][ eAxisMax ] = { {NDM5_ACC_LD_TrayPP_X, NDM5_ACC_Gripper_Y, NDM5_ACC_LD_TrayPP_Z, NDM5_ACC_LD_TrayPP_X_Pitch, NDM5_ACC_LD_TrayPP_Y_Pitch},
												 {NDM5_ACC_UL_TrayPP_X, NDM5_ACC_UL_TrayPP_Y, NDM5_ACC_UL_TrayPP_Z, NDM5_ACC_UL_TrayPP_X_Pitch, NDM5_ACC_UL_TrayPP_Y_Pitch } };
	int nIndexDcc[ eMaxTrayPPCount ][ eAxisMax ] = { {NDM5_DCC_LD_TrayPP_X, NDM5_DCC_Gripper_Y, NDM5_DCC_LD_TrayPP_Z, NDM5_DCC_LD_TrayPP_X_Pitch, NDM5_DCC_LD_TrayPP_Y_Pitch},
												 {NDM5_DCC_UL_TrayPP_X, NDM5_DCC_UL_TrayPP_Y, NDM5_DCC_UL_TrayPP_Z, NDM5_DCC_UL_TrayPP_X_Pitch, NDM5_DCC_UL_TrayPP_Y_Pitch} };



	double dRateSpd = ( double )g_DMCont.m_dmTrayPP.GN( nIndexSpd[ m_nRobotId_Idx ][ nAxisIdx ] );
	int    nRateAcc = g_DMCont.m_dmTrayPP.GN( nIndexAcc[ m_nRobotId_Idx ][ nAxisIdx ] );
	int    nRateDcc = g_DMCont.m_dmTrayPP.GN( nIndexDcc[ m_nRobotId_Idx ][ nAxisIdx ] );

	double dRetSpd = 0;
	int    nRetAcc = 0, nRetDcc = 0;

	double dMtSpd = pMotor->GetSpeed();
	double dMtAcc = pMotor->GetAcc();
	double dMtDcc = pMotor->GetDcc();


	dRetSpd = ( dMtSpd * ( dRateSpd / 100.0 ) ) * ( nSeqSpdRate / 100.0 );
	nRetAcc = ( int )( dMtAcc - ( dMtAcc * 0.75 ) * ( nRateAcc / 100.0 ) ); // 설정되어 있는 값에서 spd rate가 높을 수록 acc/dcc를 감소 시켜준다. 가장 빠를 때가 설정 속도의 95%뺀 acc/dcc로 함.
	nRetDcc = ( int )( dMtDcc - ( dMtDcc * 0.75 ) * ( nRateDcc / 100.0 ) );

	if( CTaskTrayBasePP::eAxisZ != nAxisIdx ) {
		if( nRetAcc < 100 ) nRetAcc = 100; //200
		if( nRetDcc < 100 ) nRetDcc = 100;
	}
	else {
		if( nRetAcc <= 100 ) nRetAcc = 100; //190
		if( nRetDcc <= 100 ) nRetDcc = 100;
	}
	int nErr = -1;
	if(nAxisIdx == CTaskTrayBasePP::eAxisZ)
		nErr = pMotor->Move(vPicker,dAbsPosition + dPosOffset, dRetSpd, nRetAcc, nRetDcc);
	else
		nErr = pMotor->Move( dAbsPosition + dPosOffset, dRetSpd, nRetAcc, nRetDcc );

	return nErr;
}


int CTaskTrayBasePP::do_VacBlow( eVacBlowState nVacBlow, CPoint Picker )
{
	ASSERT( Picker.x >= 0 && Picker.x < MAX_TRAY_PP_CNT_X );
	ASSERT( Picker.y >= 0 && Picker.y < MAX_TRAY_PP_CNT_Y );

	if( g_IO.GetDriverType() != _DIO_TYPE_SIMUL_ )
	{
		BOOL bLotState = g_DMCont.m_dmEQP.GB( BDM0_SYS_LOT_STATUS );
		int nDeviceMode = g_DMCont.m_dmHandlerOpt.GN( NDM11_SYS_MD_DEVICE_CHK );
		if( nDeviceMode != eDeviceMode && g_START_SW.GetStatus() == TRUE )
		{
			return ERR_NO_ERROR;
		}
	}

	if( nVacBlow == eVacBlwSt_Off ) {
		SetDO( m_nHand_o_Vac[ Picker.y ][ Picker.x ], eOff );
		SetDO( m_nHand_o_Blw[ Picker.y ][ Picker.x ], eOff );
		SetDO( m_nHand_o_Reset[ Picker.y ][ Picker.x ], eOff );
	} else if( nVacBlow == eVacBlwSt_VacOn ) {
		SetDO( m_nHand_o_Blw[ Picker.y ][ Picker.x ], eOff );  
		SetDO( m_nHand_o_Reset[ Picker.y ][ Picker.x ], eOff ); 
		SetDO( m_nHand_o_Vac[ Picker.y ][ Picker.x ], eOn );  
	} else if( nVacBlow == eVacBlwSt_BlwOn ) {
		SetDO( m_nHand_o_Vac[ Picker.y ][ Picker.x ], eOff );
		SetDO( m_nHand_o_Reset[ Picker.y ][ Picker.x ], eOn );
		SetDO( m_nHand_o_Blw[ Picker.y ][ Picker.x ], eOn );
	}

	if( g_IO.GetDriverType() == _DIO_TYPE_SIMUL_ )
		NEXT;

	return ERR_NO_ERROR;
}

int CTaskTrayBasePP::do_VacBlow( eVacBlowState nVacBlow, std::vector<CPoint> vPicker )
{
	Sleep( EJECTOR_CHANGE_MIN_TIME );
	for( std::vector<CPoint>::size_type i = 0; i < vPicker.size(); i++ ) {
		do_VacBlow( nVacBlow, vPicker[ i ] );
	}

	if (nVacBlow == eVacBlwSt_BlwOn) {
		int nSleep = g_DMCont.m_dmTrayPP.GN(NDM_TM_TrayPP_Blow);
		if (nSleep <= 0) { nSleep = MIN_VAC_BLOW_TIME; }
		Sleep(nSleep);
		//Sleep(EJECTOR_CHANGE_MIN_TIME);
		for (std::vector<CPoint>::size_type i = 0; i < vPicker.size(); i++) {
			do_VacBlow(eVacBlwSt_Off, vPicker[i]);
 		}
 	}

	return ERR_NO_ERROR;
}

int CTaskTrayBasePP::do_VacBlow_All( eVacBlowState nVacBlow )
{
	std::vector<CPoint> vPicker;
	for( int y = 0; y < MAX_TRAY_PP_CNT_Y; y++ ) {
		for( int x = 0; x < MAX_TRAY_PP_CNT_X; x++ ) {
			vPicker.push_back( CPoint( x, y ) );
		}
	}
	return do_VacBlow( nVacBlow, vPicker );
}

int CTaskTrayBasePP::do_NoDeviceResetAll()
{
	std::vector<CPoint> vPicker;
	std::vector<CPoint> vPicker_Act;
	MakeAllPicker( vPicker );
	for( std::vector<CPoint>::size_type i = 0; i < vPicker.size(); i++ ) {
		int nVac = GetVacuum( vPicker[ i ] );
		if( nVac != DEF_ON ) {
			do_VacBlow( eVacBlwSt_BlwOn, vPicker[ i ] );
			vPicker_Act.push_back( vPicker[ i ] );
		}
	}
	Sleep( 50 );
	for( std::vector<CPoint>::size_type i = 0; i < vPicker_Act.size(); i++ )
	{
		do_VacBlow( eVacBlwSt_Off, vPicker_Act[ i ] );
	}

	return ERR_NO_ERROR;
}

eOnOffState CTaskTrayBasePP::GetVacuum( CPoint Picker )
{
	ASSERT( Picker.x >= 0 && Picker.x < MAX_TRAY_PP_CNT_X );
	ASSERT( Picker.y >= 0 && Picker.y < MAX_TRAY_PP_CNT_Y );

	return GetDI( m_nHand_i_VacSen[ Picker.y ][ Picker.x ] );
}

int CTaskTrayBasePP::GetVacuum_All( eOnOffState bOnOff )
{
	std::vector<CPoint> vPicker;
	MakeAllPicker( vPicker );
	for( std::vector<CPoint>::size_type i = 0; i < vPicker.size(); i++ ) {
		int nVac = GetVacuum( vPicker[ i ] );
		if( nVac != bOnOff ) {
			m_vFailPicker.push_back( vPicker[ i ] );
			return ERR_TRAY_PP_1_VAC_SEN_ERR + m_nRobotId_Idx;
		}
	}

	return ERR_NO_ERROR;
}

int CTaskTrayBasePP::ChkVacuum( eOnOffState bOnOff, CPoint Picker )
{
	if( g_IO.GetDriverType() != _DIO_TYPE_SIMUL_ ) {
		int nDeviceMode = g_DMCont.m_dmHandlerOpt.GN( NDM11_SYS_MD_DEVICE_CHK );
		BOOL vat_in_process = g_DMCont.m_dmVAT.GB(BDM14_VAT_TEACHING_USE);
		if( nDeviceMode != eDeviceMode && vat_in_process != TRUE)
		{
			return ERR_NO_ERROR;
		}
	}

	ASSERT( Picker.x >= 0 && Picker.x < MAX_TRAY_PP_CNT_X );
	ASSERT( Picker.y >= 0 && Picker.y < MAX_TRAY_PP_CNT_Y );

	if( g_IO.GetDriverType() == _DIO_TYPE_SIMUL_ ) {
		ChkDI( m_nHand_i_VacSen[ Picker.y ][ Picker.x ], bOnOff );
		while( GetDI( m_nHand_i_VacSen[ Picker.y ][ Picker.x ] ) != bOnOff ) {
			ChkDI( m_nHand_i_VacSen[ Picker.y ][ Picker.x ], bOnOff );
		}
	}


	if( GetDI( m_nHand_i_VacSen[ Picker.y ][ Picker.x ] ) != bOnOff ) {
		return ERR_TRAY_PP_1_VAC_SEN_ERR + m_nRobotId_Idx;
	}
	return ERR_NO_ERROR;
}

int CTaskTrayBasePP::ChkVacuum( eOnOffState bOnOff, std::vector<CPoint> vPicker )
{
	for( std::vector<CPoint>::size_type i = 0; i < vPicker.size(); i++ ) {
		int nErr = ChkVacuum( bOnOff, vPicker[ i ] );
		if( nErr != ERR_NO_ERROR ) {
			m_vFailPicker.push_back( vPicker[ i ] );
			return nErr;
		}
	}

	return ERR_NO_ERROR;
}

int CTaskTrayBasePP::ChkVacuum_All( eOnOffState bOnOff )
{
	std::vector<CPoint> vPicker;
	MakeAllPicker( vPicker );
	return ChkVacuum( bOnOff, vPicker );
}

int CTaskTrayBasePP::ChkDI( eDI nIndex, eOnOffState bValue )
{
	ASSERT( nIndex >= ( eDI )0 && nIndex < ( eDI )m_vX.size() );

	if( g_IO.GetDriverType() == _DIO_TYPE_SIMUL_ ) {
		g_IO.SetSimulDi( m_vX[ ( int )nIndex ], bValue );
		Sleep( SIMUL_SLEEP_TIME );
	}

	if( g_IO.in( m_vX[ ( int )nIndex ] ) != bValue ) {
		return -1;
	}

	return ERR_NO_ERROR;
}

eOnOffState CTaskTrayBasePP::GetDI( eDI nIndex )
{
	ASSERT( nIndex >= ( eDI )0 && nIndex < ( eDI )m_vX.size() );
	return ( eOnOffState )g_IO.in( m_vX[ ( int )nIndex ] );
}

int CTaskTrayBasePP::SetDO( eDO nIndex, eOnOffState eOnOff )
{
	ASSERT( nIndex >= ( eDO )0 && nIndex < ( eDO )m_vY.size() );
	g_IO.out( m_vY[ ( int )nIndex ], eOnOff );

	return ERR_NO_ERROR;
}

BOOL CTaskTrayBasePP::IsInitNeed()
{
	for( std::vector<OneAxis*>::size_type i = 0; i < m_vMotor.size(); i++ ) {
		OneAxis* pMotor = m_vMotor[ i ];
		if( pMotor->ChkMotorUsable() != ERR_NO_ERROR ) {
			return TRUE;
		}
	}

	return FALSE;
}

int CTaskTrayBasePP::GetDeviceDataExist( CPoint Picker, eExistState* pnExist )
{
	*pnExist = ( eExistState )g_DMCont.m_dmDevice.GetDeviceExist( m_eMyDeviceStageNum, Picker.x, Picker.y );
	return ERR_NO_ERROR;
}

int CTaskTrayBasePP::SetDeviceDataExist( CPoint Picker, eExistState nExist )
{
	g_DMCont.m_dmDevice.SetDeviceExist( m_eMyDeviceStageNum, Picker.x, Picker.y, nExist );
	return ERR_NO_ERROR;
}

void CTaskTrayBasePP::MakeAllPicker( std::vector<CPoint>& vPicker )
{
	for( int y = 0; y < MAX_TRAY_PP_CNT_Y; y++ ) {
		for( int x = 0; x < MAX_TRAY_PP_CNT_X; x++ ) {
			vPicker.push_back( CPoint( x, y ) );
		}
	}
}

int CTaskTrayBasePP::GetTrayPpDvcCnt()
{
	int nCntPicker = 0;
	ST_DD_DEVICE stDvc;
	for( int nY = 0; nY < MAX_TRAY_PP_CNT_Y; nY++ )
	{
		for( int nX = 0; nX < MAX_TRAY_PP_CNT_X; nX++ )
		{
			char setting = this->m_TrayPP_picker_setting[ nY ][ nX ];
			g_DMCont.m_dmDevice.GetDeviceData( m_eMyDeviceStageNum, nX, nY, &stDvc );
			if( stDvc.sExist == eDeviceData_Exist )
			{
				bool bCond1 = false, bCond2 = false;
				if( ( setting == 'L') )
				{
					nCntPicker++;
				}			
			}
		}
	}

	return nCntPicker;
}


int CTaskTrayBasePP::GetTrayPpFrontPickerDvcCnt()
{
	int nCntPicker = 0;
	ST_DD_DEVICE stDvc;
	for (int nX = 0; nX < MAX_TRAY_PP_CNT_X; nX++)
	{
		char setting = this->m_TrayPP_picker_setting[MAX_TRAY_PP_CNT_Y-1][nX];
		g_DMCont.m_dmDevice.GetDeviceData(m_eMyDeviceStageNum, nX, MAX_TRAY_PP_CNT_Y-1, &stDvc);
		if (stDvc.sExist == eDeviceData_Exist)
		{
			bool bCond1 = false, bCond2 = false;
			if ((setting == 'L'))
			{
				nCntPicker++;
			}
		}		
	}

	return nCntPicker;
}

void CTaskTrayBasePP::AddLoadTrayPickFailCnt(int nCnt)
{
	m_nLoadTrayAutoSkipCount += nCnt;
}

//
int	CTaskTrayBasePP::GetPickerErrorCase(int nAxisDone, int nErrSen, std::vector<CPoint> vAxisDonePkr, std::vector<CPoint> vErrSen, std::vector<CPoint>& vErrPicker, CString& sAlarmData)
{
	int nErr = -1;
	CString strErrPkr = _T(""), strErrTempPkr = _T("");
	// AxisDone ERR, Safety Sensor NO_ERR
	if ((nAxisDone != ERR_NO_ERROR && nErrSen == ERR_NO_ERROR) || (nAxisDone != ERR_NO_ERROR && nErrSen != ERR_NO_ERROR))
	{
		for (auto it = vAxisDonePkr.begin(); it != vAxisDonePkr.end(); it++) {
			strErrTempPkr.Format("[X:%d, Y:%d]", (*it).x + 1, (*it).y + 1);
			strErrPkr += strErrTempPkr;
		}
		sAlarmData.Format("%s z axis move safety position move time over!! [%s]", GetRobotIdIdx() == 0 ? "tray load pp" : "tray unload pp", strErrPkr);
		nErr = GetRobotIdIdx() == 0 ? ERR_TRAY_PP_1_Z_PICKER_MOTOR_MOVE_TIMEOUT : ERR_TRAY_PP_2_Z_PICKER_MOTOR_MOVE_TIMEOUT;
		vErrPicker = vAxisDonePkr;

	}// AxisDone NO_ERR, Safety Sensor ERR
	else if (nAxisDone == ERR_NO_ERROR && nErrSen != ERR_NO_ERROR)
	{
		for (auto it = vErrSen.begin(); it != vErrSen.end(); it++) {
			strErrTempPkr.Format("[X:%d, Y:%d]", (*it).x + 1, (*it).y + 1);
			strErrPkr += strErrTempPkr;
		}

		sAlarmData.Format("%s z axis safety sensor not detected time over!! [%s]", GetRobotIdIdx() == 0 ? "tray load pp" : "tray unload pp", strErrPkr);
		nErr = GetRobotIdIdx() == 0 ? ERR_TRAY_PP_1_Z_PICKER_SAFETY_SENSOR_NOT_DETECTED : ERR_TRAY_PP_2_Z_PICKER_SAFETY_SENSOR_NOT_DETECTED;
		vErrPicker = vErrSen;

	}
	else { // 정상
		nErr = ERR_NO_ERROR;
	}

	return nErr;
}

/*=============================================================================
	DESCRIPT : Tray Pp 가 Auto Skip count를 넘겼는지에 대한 검사
	KEY WORD :
	ARGUMENT :
	RETURN   : TRUE = Over / FALSE = Not Over
==============================================================================*/
BOOL CTaskTrayBasePP::ChkLoadTrayPickFailCntIsOver()
{
	int nRecCnt = g_DMCont.m_dmHandlerOpt.GN(NDM11_AutoSkipCount);

	if (nRecCnt >= m_nLoadTrayAutoSkipCount || nRecCnt <= 0) {
		return FALSE;
	}

	return TRUE;
}

void CTaskTrayBasePP::ResetLoadTrayPickFailCnt()
{
	m_nLoadTrayAutoSkipCount = 0;
}

int CTaskTrayBasePP::GetDeviceData_Picker(int nX, int nY, ST_DD_DEVICE* lpstBuff)
{
	int nErr = g_DMCont.m_dmDevice.GetDeviceData(m_eMyDeviceStageNum, nX, nY, lpstBuff);
	return nErr;
}

int CTaskTrayBasePP::SetDeviceData_Picker(int nX, int nY, ST_DD_DEVICE* lpstBuff)
{
	int nErr = g_DMCont.m_dmDevice.SetDeviceData(m_eMyDeviceStageNum, nX, nY, lpstBuff);
	return nErr;
}

/*=============================================================================
	AUTHOR   : 나명진
	NAME     :
	PARAMS   :
	RETURN   :
	COMMENTS : Device Data를 Target Cmd Stage에 따라서 데이터 처리 Load Tbl->Test PP or Test PP->Test Site
==============================================================================*/
int CTaskTrayBasePP::ProcDeviceData(eLocIdxTrayPP       eDestStage,
									int                 nDestPocektX,
									int                 nDestPocketY,
									std::vector<CPoint> vCmdPicker,
									ePPCmd              ePickPlace,
									std::vector<CString>& vErrString,
	                                 int                 nMisMatchErase )
{
	ASSERT(eDestStage >= 0);
	ASSERT((nDestPocektX >= 0) && (nDestPocektX < DEVICE_DATA_MAX_X));
	ASSERT((nDestPocketY >= 0) && (nDestPocketY < DEVICE_DATA_MAX_Y));
	ASSERT(vCmdPicker.size() != 0);

	m_vDevLossX.clear();
	m_vDevLossY.clear();
	m_vFailPicker.clear();

	ST_DD_DEVICE stSourceData;
	ST_DD_DEVICE stEmptyDevice;
	eDeviceDataStage eDvcDataStageIdx = eDeviceStage_Undefine;
	int nRetError = ERR_NO_ERROR;

	switch (eDestStage)
	{
	case LOC_IDX_TRAY_PP_CONV_1:
	case LOC_IDX_TRAY_PP_CONV_2:
	case LOC_IDX_TRAY_PP_CONV_3:
	case LOC_IDX_TRAY_PP_CONV_4:
	case LOC_IDX_TRAY_PP_CONV_5:
	case LOC_IDX_TRAY_PP_CONV_6:
	case LOC_IDX_TRAY_PP_CONV_7:
	case LOC_IDX_TRAY_PP_TBL_1:
	case LOC_IDX_TRAY_PP_TBL_2:
	case LOC_IDX_TRAY_PP_TBL_3:
	case LOC_IDX_TRAY_PP_TBL_4:
	case LOC_IDX_TRAY_PP_FEEDER:
	{
		typedef std::vector<CPoint>::const_iterator vIterPoint;
		ST_DD_DEVICE stTargetData;

		eStacker_Type stacker_type = (eStacker_Type)-1;
		eStacker_Idx stacker_idx   = (eStacker_Idx)-1;
		if (LOC_IDX_TRAY_PP_CONV_1 <= eDestStage && eDestStage <= LOC_IDX_TRAY_PP_CONV_7) {
			stacker_type = g_TaskAutoStacker[eDestStage - LOC_IDX_TRAY_PP_CONV_1].GetStackerType();
			stacker_idx = g_TaskAutoStacker[eDestStage - LOC_IDX_TRAY_PP_CONV_1].GetStackerIndex();
		}
		else if(eDestStage == LOC_IDX_TRAY_PP_FEEDER)
			stacker_type = eStackerType_Load;

		std::vector< std::pair< CPoint, CPoint > > vPickerToPocket = change_picker_to_pocket_point(eDestStage, nDestPocektX, nDestPocketY, vCmdPicker);

		ST_DAILY_REPORT stDailyData;

		if (ePickPlace == DEF_PICK) { // Pick loop
			for (int i = 0; i < (int)vCmdPicker.size(); i++)
			{
				auto iter_current_picker_pocket = std::find_if(vPickerToPocket.begin(), vPickerToPocket.end()
					, [&](std::pair< CPoint, CPoint > _picker_to_pocket) -> bool
				{
					if (_picker_to_pocket.first == vCmdPicker[i])
						return true;

					return false;
				});

				CPoint pocket = iter_current_picker_pocket->second;
				int nPosX = pocket.x;
				int nPosY = pocket.y;

				ConvertCmdIdxToStageIdx(eDestStage, &eDvcDataStageIdx);

				// Get Source data ( Source = Setplate or Table or Buff ) 					
				GetTargetCmdStageDvcData(eDvcDataStageIdx, nPosX, nPosY, &stSourceData);

				// Get Target data ( Target = Hand )
				stTargetData.clear();
				GetDeviceData_Picker((vCmdPicker[i].x), (vCmdPicker[i].y), &stTargetData);

				// checking hand picker vacuum sensor 
				int nChkSen = ChkVacuum(eOnOffState::eOn, vCmdPicker[i]);
				stEmptyDevice.clear();

				// ============================================================================================================
				// case 1 :: Source device exist, hand device sensing
				// ============================================================================================================
				if (stSourceData.sExist == eDeviceData_Exist && stTargetData.sExist != eDeviceData_Exist && nChkSen == ERR_NO_ERROR) {
					//Device Pick History
					stSourceData.stHistory.nStage = eDestStage;
					stSourceData.stHistory.cPocketNum = CPoint(nPosX, nPosY);
					stSourceData.stHistory.cPicker = vCmdPicker[i];
					//

					SetDeviceData_Picker((vCmdPicker[i].x), (vCmdPicker[i].y), &stSourceData);
					SetTargetCmdStageDvcData(eDvcDataStageIdx, nPosX, nPosY, &stEmptyDevice);
				}
				// ============================================================================================================
				// case 2 :: Source device exist, hand device not sensing [ pick fail ]
				// ============================================================================================================
				else if (stSourceData.sExist == eDeviceData_Exist && nChkSen != ERR_NO_ERROR) {

					if (nMisMatchErase != eDeviceData_Nothing && !(stacker_type == eStackerType_Unload)) {
						SetDeviceData_Picker((vCmdPicker[i].x), (vCmdPicker[i].y), &stEmptyDevice);

						if (nMisMatchErase == eDeviceData_EraseAndAutoSkip)
							StringCchPrintf(stEmptyDevice.szBinResult, _countof(stEmptyDevice.szBinResult), DEF_TRAY_PICK_ERR_DEVICE);

						SetTargetCmdStageDvcData(eDvcDataStageIdx, nPosX, nPosY, &stEmptyDevice);
					}
					else {
						m_vFailPicker.push_back(vCmdPicker[i]);
						nRetError = ERR_TRAY_PP_1_DEVICE_PICK_FAIL+ m_nRobotId_Idx;
					}
				}
				// ============================================================================================================
				// case 3 :: Source device no-exist, hand device exist, hand device not sensing [ device lost ]
				// ============================================================================================================
				else if (stSourceData.sExist != eDeviceData_Exist && stTargetData.sExist == eDeviceData_Exist && nChkSen != ERR_NO_ERROR) {
					if (nMisMatchErase != eDeviceData_Nothing) {
						SetDeviceData_Picker((vCmdPicker[i].x), (vCmdPicker[i].y), &stEmptyDevice);

						if (nMisMatchErase == eDeviceData_EraseAndAutoSkip)
							StringCchPrintf(stEmptyDevice.szBinResult, _countof(stEmptyDevice.szBinResult), DEF_TRAY_PICK_ERR_DEVICE);

						SetTargetCmdStageDvcData(eDvcDataStageIdx, nPosX, nPosY, &stEmptyDevice);
					}
					else {
						nRetError = ERR_TRAY_PP_1_DEVICE_MISS + m_nRobotId_Idx;
						m_vDevLossX.push_back(vCmdPicker[i].x);
						m_vDevLossY.push_back(vCmdPicker[i].y);
						m_vFailPicker.push_back(vCmdPicker[i]);

						CString strPkr;
						strPkr.Format("[X=%d,Y=%d][Barcode=%s]", vCmdPicker[i].x+1, vCmdPicker[i].y+1, stTargetData.sz2DBarcode);
						vErrString.push_back(strPkr);

					}
				}
				// ============================================================================================================
				// case 4 :: Source device no-exist, hand device exist, hand device sensing
				// ============================================================================================================
				else if (stSourceData.sExist != eDeviceData_Exist && stTargetData.sExist == eDeviceData_Exist && nChkSen == ERR_NO_ERROR) {
					continue;
				}
				// ============================================================================================================
				// case 5 :: ??
				// ============================================================================================================
				else {
					ASSERT(0);
					m_vFailPicker.push_back(vCmdPicker[i]);
					nRetError = ERR_TRAY_PP_1_DEVICE_PICK_FAIL + m_nRobotId_Idx;
				}
			}
		}
		else { // Place loop
			for (int i = 0; i < (int)vCmdPicker.size(); i++)
			{
				auto iter_current_picker_pocket = std::find_if(vPickerToPocket.begin(), vPickerToPocket.end()
					, [&](std::pair< CPoint, CPoint > _picker_to_pocket) -> bool
				{
					if (_picker_to_pocket.first == vCmdPicker[i])
						return true;

					return false;
				});

				CPoint pocket = iter_current_picker_pocket->second;
				int nPosX = pocket.x;
				int nPosY = pocket.y;

				// Get Source data ( Source = Hand )
				GetDeviceData_Picker((vCmdPicker[i].x), (vCmdPicker[i].y), &stSourceData);

				// Get Target data ( Target = Setplate or Table or Buff )
				stTargetData.clear();
				ConvertCmdIdxToStageIdx(eDestStage, &eDvcDataStageIdx);
				GetTargetCmdStageDvcData(eDvcDataStageIdx, nPosX, nPosY, &stTargetData);

				// checking hand picker vacuum sensor
				int nChkSen = ChkVacuum(eOnOffState::eOff, vCmdPicker[i]);

				// ============================================================================================================
				// case 1 :: hand device data exist, Target device data empty, hand not sensing device
				// ============================================================================================================
				if (stSourceData.sExist == eDeviceData_Exist && stTargetData.sExist != eDeviceData_Exist && nChkSen == ERR_NO_ERROR && nMisMatchErase == eDeviceData_Nothing) {
					SetTargetCmdStageDvcData(eDvcDataStageIdx, nPosX, nPosY, &stSourceData);
					SetDeviceData_Picker((vCmdPicker[i].x), (vCmdPicker[i].y), &stEmptyDevice);

					//========================================================================================
					// Product Loading Count
					//========================================================================================
					int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
					if (eDestStage >= LOC_IDX_TRAY_PP_TBL_1 && eDestStage <= LOC_IDX_TRAY_PP_TBL_4)
					{
						int nAccLotLoadDvcCnt = g_DMCont.m_dmEQP.GN(NDM0_AccLotLoadingCnt);   // 누적 Lot
						int nDailyLotLoadDvcCnt = g_DMCont.m_dmEQP.GN(NDM0_DailyLotLoadingCnt); // DailyLot                             
						int nCurLotLoadDvcCnt = g_DMCont.m_dmEQP.GN(NDM0_CurLotLoadingCnt);   // 현재 Lot
						int nTotalLoadDvcCnd = g_DMCont.m_dmEQP.GN(NDM0_TotalLoadCnt); // Total Load Cnt

						g_DMCont.m_dmEQP.SN(NDM0_CurLotLoadingCnt, ++nCurLotLoadDvcCnt);
						g_DMCont.m_dmEQP.SN(NDM0_AccLotLoadingCnt, ++nAccLotLoadDvcCnt);
						g_DMCont.m_dmEQP.SN(NDM0_DailyLotLoadingCnt, ++nDailyLotLoadDvcCnt);
						g_DMCont.m_dmEQP.SN(NDM0_TotalLoadCnt, ++nTotalLoadDvcCnd);

						stDailyData.nLoadCnt++;

						if (nLotStartMode == eStartMD_Retest || nLotStartMode == eStartMD_RetestContinue) {
							int nRetestLotLoadDvcCnt = g_DMCont.m_dmEQP.GN(NDM0_CurLotRT_LoadingCnt);
							g_DMCont.m_dmEQP.SN(NDM0_CurLotRT_LoadingCnt, ++nRetestLotLoadDvcCnt);
						}

						if (nLotStartMode == eStartMD_QA || nLotStartMode == eStartMD_QA_Continue) {
							int nQaLoadCnt = g_DMCont.m_dmEQP.GN(NDM0_QaSampleLoadCnt);
							g_DMCont.m_dmEQP.SN(NDM0_QaSampleLoadCnt, ++nQaLoadCnt);
						}

						//BOOL bGrrDvcSwap = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrDeviceSwapUse);
						BOOL bGrrUse = g_TaskSystemCtrl.GetAutoGrrUse();

						BOOL bAutoLoopUse = g_TaskSystemCtrl.GetAutoLoopUse();


						if (bGrrUse == TRUE)//&& bGrrDvcSwap == FALSE)
						{
							int nIdx = 0;
							if (eDestStage == LOC_IDX_TRAY_PP_TBL_1 || eDestStage == LOC_IDX_TRAY_PP_TBL_2) {
								nIdx = 0;
							}
							else {
								nIdx = 1;
							}
								

							int nCountInsert = g_DMCont.m_dmTestPP.GN(NDM04_SITEGROUP1_INSERTED_DEVICE_CNT + nIdx/*(eDestStage - LOC_IDX_TRAY_PP_TBL_1)*/);
							g_DMCont.m_dmTestPP.SN(NDM04_SITEGROUP1_INSERTED_DEVICE_CNT + nIdx/*(eDestStage - LOC_IDX_TRAY_PP_TBL_1)*/, ++nCountInsert);

							int nMaxTestCount = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoGrrTestCount);
							for (int nTestCnt = 0; nTestCnt < nMaxTestCount; nTestCnt++)
							{
								switch (eDestStage)
								{
								case LOC_IDX_TRAY_PP_TBL_2:
								{
									stSourceData.stGrrTestInfo[ePressUnit_Down_5].testDoneAll();
									stSourceData.stGrrTestInfo[ePressUnit_Down_6].testDoneAll();
									stSourceData.stGrrTestInfo[ePressUnit_Down_7].testDoneAll();
									stSourceData.stGrrTestInfo[ePressUnit_Down_8].testDoneAll();


								}break;
								default:
								{
									stSourceData.stGrrTestInfo[ePressUnit_Down_1].testDoneAll();
									stSourceData.stGrrTestInfo[ePressUnit_Down_2].testDoneAll();
									stSourceData.stGrrTestInfo[ePressUnit_Down_3].testDoneAll();
									stSourceData.stGrrTestInfo[ePressUnit_Down_4].testDoneAll();
								}break;
								}
							}
							SetTargetCmdStageDvcData(eDvcDataStageIdx, nPosX, nPosY, &stSourceData);
						}
						else if (bAutoLoopUse == TRUE)
						{
							int nCountInsert = g_DMCont.m_dmTestPP.GN(NDM04_SITEGROUP1_INSERTED_DEVICE_CNT + (eDestStage - LOC_IDX_TRAY_PP_TBL_1));
							g_DMCont.m_dmTestPP.SN(NDM04_SITEGROUP1_INSERTED_DEVICE_CNT + (eDestStage - LOC_IDX_TRAY_PP_TBL_1), ++nCountInsert);

							int nMaxTestCount = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoLoopTestCount);
							for (int nTestCnt = 0; nTestCnt < nMaxTestCount; nTestCnt++)
							{
								switch (eDestStage)
								{
								case LOC_IDX_TRAY_PP_TBL_1:
								case LOC_IDX_TRAY_PP_TBL_2:
								{
									stSourceData.stLoopTestInfo[ePressUnit_Down_1].testDoneAll();
									stSourceData.stLoopTestInfo[ePressUnit_Down_2].testDoneAll();
									stSourceData.stLoopTestInfo[ePressUnit_Down_3].testDoneAll();
									stSourceData.stLoopTestInfo[ePressUnit_Down_4].testDoneAll();

								}break;
								default:
								{
									stSourceData.stLoopTestInfo[ePressUnit_Down_5].testDoneAll();
									stSourceData.stLoopTestInfo[ePressUnit_Down_6].testDoneAll();
									stSourceData.stLoopTestInfo[ePressUnit_Down_7].testDoneAll();
									stSourceData.stLoopTestInfo[ePressUnit_Down_8].testDoneAll();
								}break;
								}
							}
							SetTargetCmdStageDvcData(eDvcDataStageIdx, nPosX, nPosY, &stSourceData);
						}
					}
					//========================================================================================
					// Product Unloading Count
					//========================================================================================
					else if (stacker_type == eStackerType_Unload)
					{
						std::vector< CTaskAutoStacker* >unload_conveyor;

						for (int i = 0; i < eMaxStackerCount; i++) {
							eStacker_Type eType = g_TaskAutoStacker[i].GetStackerType();
							switch (eType)
							{
							case eStackerType_Unload: unload_conveyor.push_back(&g_TaskAutoStacker[i]); break;
							}
						}

						int unload_idx = -1;
						for (size_t i = 0; i < unload_conveyor.size(); i++) {
							if (stacker_idx == unload_conveyor[i]->GetStackerIndex()) {
								unload_idx = i;
								break;
							}
						}
						ASSERT(0 <= unload_idx);

						//================================= [ RT ]
						if (nLotStartMode == eStartMD_Retest || nLotStartMode == eStartMD_RetestContinue)
						{
							int nRetestLotSortDvcCnt = g_DMCont.m_dmEQP.GN(NDM0_CurLotRT_SortingCnt);
							g_DMCont.m_dmEQP.SN(NDM0_CurLotRT_SortingCnt, ++nRetestLotSortDvcCnt);
							// Pass/Fail Bin Count
							if (stSourceData.nBinColor == eFailBin) {
								int nRTLotFail = g_DMCont.m_dmEQP.GN(NDM0_CurLotRT_FailBinCnt);
								g_DMCont.m_dmEQP.SN(NDM0_CurLotRT_FailBinCnt, ++nRTLotFail);
							}
							else {
								int nRtLotPass = g_DMCont.m_dmEQP.GN(NDM0_CurLotRT_PassBinCnt);
								g_DMCont.m_dmEQP.SN(NDM0_CurLotRT_PassBinCnt, ++nRtLotPass);
							}
							// Unloading Setplate 1~4 Count
							//int nTargetStageIdx = nCmdStage - LOC_IDX_TRAY_PP_SETPLATE_3;
							int nTargetStageIdx = unload_idx;
							int nCurAutoBin = g_DMCont.m_dmEQP.GN(NDM0_CurLotRT_Output1Cnt + nTargetStageIdx);
							g_DMCont.m_dmEQP.SN(NDM0_CurLotRT_Output1Cnt + nTargetStageIdx, ++nCurAutoBin);
						}

						//================================= [ FT && Acc RT ]
						int nSortCnt = g_DMCont.m_dmEQP.GN(NDM0_CurLotSortingCnt);
						g_DMCont.m_dmEQP.SN(NDM0_CurLotSortingCnt, ++nSortCnt);
						// Pass/Fail Bin Count
						if (stSourceData.nBinColor == eFailBin) {
							int nAccLotFail = g_DMCont.m_dmEQP.GN(NDM0_AccLotFailBinCnt);
							int nCurLotFail = g_DMCont.m_dmEQP.GN(NDM0_CurLotFailBinCnt);
							int nDailyLotFail = g_DMCont.m_dmEQP.GN(NDM0_DailyLotFailBinCnt);

							g_DMCont.m_dmEQP.SN(NDM0_AccLotFailBinCnt, ++nAccLotFail);
							g_DMCont.m_dmEQP.SN(NDM0_CurLotFailBinCnt, ++nCurLotFail);
							g_DMCont.m_dmEQP.SN(NDM0_DailyLotFailBinCnt, ++nDailyLotFail);
						}
						else {
							int nAccLotPass = g_DMCont.m_dmEQP.GN(NDM0_AccLotPassBinCnt);
							int nCurLotPass = g_DMCont.m_dmEQP.GN(NDM0_CurLotPassBinCnt);
							int nDailyLotPass = g_DMCont.m_dmEQP.GN(NDM0_DailyLotPassBinCnt);

							g_DMCont.m_dmEQP.SN(NDM0_AccLotPassBinCnt, ++nAccLotPass);
							g_DMCont.m_dmEQP.SN(NDM0_CurLotPassBinCnt, ++nCurLotPass);
							g_DMCont.m_dmEQP.SN(NDM0_DailyLotPassBinCnt, ++nDailyLotPass);

							stDailyData.nPassCnt++;
						}

						stDailyData.nUnloadCnt++;

						// Unloading Setplate 1~4 Count
						//int nUnloadIdx = nCmdStage - LOC_IDX_TRAY_PP_SETPLATE_3;
						int nUnloadIdx = unload_idx;
						// Get Record Data
						int nCurAutoBin = g_DMCont.m_dmEQP.GN(NDM0_CurLotOutput1Cnt + nUnloadIdx);
						int nDailyAutoBin = g_DMCont.m_dmEQP.GN(NDM0_DailyLotOutput1Cnt + nUnloadIdx);
						// Set Record Data
						g_DMCont.m_dmEQP.SN(NDM0_CurLotOutput1Cnt + nUnloadIdx, ++nCurAutoBin);
						g_DMCont.m_dmEQP.SN(NDM0_DailyLotOutput1Cnt + nUnloadIdx, ++nDailyAutoBin);


						if(stSourceData.stRecInfo.stDvcInfo[0].bQAEmpty == FALSE)
							g_JamRate.PushDeviceData(stSourceData.stRecInfo);
						else {
							int nCurLotQAEmptyBin = g_DMCont.m_dmEQP.GN(NDM0_CurLotQAEmptyLoadCnt + nUnloadIdx);
							g_DMCont.m_dmEQP.SN(NDM0_CurLotQAEmptyLoadCnt + nUnloadIdx,++nCurLotQAEmptyBin);
						}
							
						// function violation // By SJI
						BOOL bAutoRetestUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);
						BOOL b2dBarcodeMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_2D_BarCode_Use);
						if (bAutoRetestUseMode == TRUE && b2dBarcodeMode == eOPTION_USE) {
							//g_ART_Rule.Insert_VLTD( stSourceData );
						}
					}
					g_TaskSystemCtrl.CalcUPH();	// uph
				}
				// ============================================================================================================
				// case 2 :: hand device data exist, but hand don't have device [device lost] 
				//           [Alarm Device Skip시 Device Data 삭제]
				// ============================================================================================================
				else if (stSourceData.sExist == eDeviceData_Exist && nChkSen == ERR_NO_ERROR && stTargetData.sExist != eDeviceData_Exist && nMisMatchErase == eDeviceData_Erase) {
					nRetError = ERR_TRAY_PP_1_DEVICE_MISS + m_nRobotId_Idx;

					// 							if (nMisMatchErase == eDeviceData_Erase)
					// 							{
					//SetTargetCmdStageDvcData(StageIdx, nPosX, nPosY, &stEmptyDevice);
					SetDeviceData_Picker((vCmdPicker[i].x), (vCmdPicker[i].y), &stEmptyDevice);
					//							}
					m_vDevLossX.push_back(vCmdPicker[i].x);
					m_vDevLossY.push_back(vCmdPicker[i].y);

					CString strPkr;
					strPkr.Format("[X=%d,Y=%d][Barcode=%s]", vCmdPicker[i].x + 1, vCmdPicker[i].y + 1, stTargetData.sz2DBarcode);
					vErrString.push_back(strPkr);

				}
				// ============================================================================================================
				// case 3 :: hand device data no-exist, Target pocket device data exist, hand sensing device
				// ============================================================================================================
				else if (stSourceData.sExist != eDeviceData_Exist && stTargetData.sExist == eDeviceData_Exist && nChkSen != ERR_NO_ERROR) {
					nRetError = ERR_TRAY_PP_1_DEVICE_PLACE_FAIL + m_nRobotId_Idx;
					m_vFailPicker.push_back(vCmdPicker[i]);
				}
				// ============================================================================================================
				// case 4 :: hand device data no-exist, Target pocket device data exist, hand don't have device 
				//           [ normal complete ]
				// ============================================================================================================
				else if (stSourceData.sExist != eDeviceData_Exist && stTargetData.sExist == eDeviceData_Exist && nChkSen == ERR_NO_ERROR) {
					continue;
				}
				// ============================================================================================================
				// case 5 :: hand device data exist, Target pocket device data no-exist, hand have device 
				//           [ Alarm Device Skip시 정상적인 경우]
				//           [ Alarm Skip이 아닐 경우 Place Fail, Place를 했는데 Device가 hand에 감지된다.]
				// ============================================================================================================
				else if (stSourceData.sExist == eDeviceData_Exist && stTargetData.sExist != eDeviceData_Exist  && nChkSen != ERR_NO_ERROR) {
					if (nMisMatchErase != eDeviceData_Nothing) {
						continue;
					}
					else {
						nRetError = ERR_TRAY_PP_1_DEVICE_PLACE_FAIL + m_nRobotId_Idx;
					}
				}
				// ============================================================================================================
				// case 6 :: ??
				// ============================================================================================================
				else {
					ASSERT(0);
					m_vFailPicker.push_back(vCmdPicker[i]);
					nRetError = ERR_TRAY_PP_1_DEVICE_PLACE_FAIL + m_nRobotId_Idx;
				}
			}
		}
		//Daily Report Data
		g_TaskSystemCtrl.AddDailyReportData(stDailyData);

	}break;
	}

	return nRetError;
}

int CTaskTrayBasePP::ConvertCmdIdxToStageIdx( eLocIdxTrayPP nCmdStage, eDeviceDataStage* nStageIdx )
{
	switch( nCmdStage )
	{
	case LOC_IDX_TRAY_PP_CONV_1:
	case LOC_IDX_TRAY_PP_CONV_2:
	case LOC_IDX_TRAY_PP_CONV_3:
	case LOC_IDX_TRAY_PP_CONV_4:
	case LOC_IDX_TRAY_PP_CONV_5:
	case LOC_IDX_TRAY_PP_CONV_6:
	case LOC_IDX_TRAY_PP_CONV_7:
		{
			*nStageIdx = eDeviceDataStage( eDeviceStage_STACKER_CONV_1 + ( nCmdStage - LOC_IDX_TRAY_PP_CONV_1 ) );
		}break;
	case LOC_IDX_TRAY_PP_TBL_1:
	case LOC_IDX_TRAY_PP_TBL_2:
	case LOC_IDX_TRAY_PP_TBL_3:
	case LOC_IDX_TRAY_PP_TBL_4:
		{
			*nStageIdx = eDeviceDataStage( eDeviceStage_LD_TBL_1 + ( nCmdStage - LOC_IDX_TRAY_PP_TBL_1 ) );
		}break;
	case LOC_IDX_TRAY_PP_FEEDER:
		{
			*nStageIdx = eDeviceDataStage( eDeviceStage_TRAY_FEEDER + ( nCmdStage - LOC_IDX_TRAY_PP_FEEDER ) );
		}break;
	default:
		{
			return ERR_FAILURE;
		}
	}

	return ERR_NO_ERROR;
}

int CTaskTrayBasePP::GetTargetCmdStageDvcData( eDeviceDataStage nStageIdx, int nX, int nY, ST_DD_DEVICE* lpstBuff )
{
	int nErr = g_DMCont.m_dmDevice.GetDeviceData( nStageIdx, nX, nY, lpstBuff );
	return nErr;
}

int CTaskTrayBasePP::GetTargetCmdStageDvcData(int nX, int nY, ST_DD_DEVICE* lpstBuff)
{
	return GetTargetCmdStageDvcData(m_eMyDeviceStageNum, nX, nY, lpstBuff);
}

int CTaskTrayBasePP::SetTargetCmdStageDvcData( eDeviceDataStage nStageIdx, int nX, int nY, ST_DD_DEVICE* lpstBuff )
{
	int nErr = g_DMCont.m_dmDevice.SetDeviceData( nStageIdx, nX, nY, lpstBuff );
	return nErr;
}

int CTaskTrayBasePP::SetTargetCmdStageDvcData(int nX, int nY, ST_DD_DEVICE* lpstBuff)
{
	return SetTargetCmdStageDvcData(m_eMyDeviceStageNum, nX, nY, lpstBuff);
}

CString CTaskTrayBasePP::ChkLostDevice( eLocIdxTrayPP nCmdStage, int nTargetX, int nTargetY, std::vector<CPoint> vCmdPicker, ePPCmd nPickPlace )
{
	CString strChkPicker = _T( "" );
	for( int i = 0; i < ( int )vCmdPicker.size(); i++ ) {
		int nRetErr = ChkVacuum( eOn, vCmdPicker[ i ] );
		int nExistDvc = g_DMCont.m_dmDevice.GetDeviceExist( m_eMyDeviceStageNum, vCmdPicker[ i ].x, vCmdPicker[ i ].y );

		// device data exist but not sensing deivce
		if( nExistDvc == eDvcExist && nRetErr != ERR_NO_ERROR )
		{
			CString strData = _T( "" );
			strData.Format( "(x:%d y:%d)", vCmdPicker[ i ].x, vCmdPicker[ i ].y );
			strChkPicker += strData;
		}
	}
	return strChkPicker;
}

int CTaskTrayBasePP::GetLostDvcCount( std::vector<CPoint> vCmdPicker, ePPCmd nPickPlace )
{
	int nLostDvcCnt = 0;
	for( int i = 0; i < ( int )vCmdPicker.size(); i++ ) {
		int nRetErr = ChkVacuum( eOn, vCmdPicker[ i ] );
		int nExistDvc = g_DMCont.m_dmDevice.GetDeviceExist( m_eMyDeviceStageNum, vCmdPicker[ i ].x, vCmdPicker[ i ].y );

		// device data exist but not sensing deivce
		if( nExistDvc == eDvcExist && nRetErr != ERR_NO_ERROR && nPickPlace == DEF_PICK ) {
			nLostDvcCnt++;
		}

		if( nExistDvc != eDvcExist && nRetErr == ERR_NO_ERROR && nPickPlace == DEF_PLACE ) {
			nLostDvcCnt++;
		}
	}
	return nLostDvcCnt;
}

CString CTaskTrayBasePP::GetLocName( eLocIdxTrayPP nCmdStage )
{
	CString strLocName = _T( "" );

	switch( nCmdStage )
	{
	case LOC_IDX_TRAY_PP_CONV_1: { strLocName.Format( "Conveyor1" ); } break;
	case LOC_IDX_TRAY_PP_CONV_2: { strLocName.Format( "Conveyor2" ); } break;
	case LOC_IDX_TRAY_PP_CONV_3: { strLocName.Format( "Conveyor3" ); } break;
	case LOC_IDX_TRAY_PP_CONV_4: { strLocName.Format( "Conveyor4" ); } break;
	case LOC_IDX_TRAY_PP_CONV_5: { strLocName.Format( "Conveyor5" ); } break;
	case LOC_IDX_TRAY_PP_CONV_6: { strLocName.Format( "Conveyor6" ); } break;
	case LOC_IDX_TRAY_PP_CONV_7: { strLocName.Format( "Conveyor7" ); } break;
	case LOC_IDX_TRAY_PP_TBL_1: { strLocName.Format( "LoadTable1" ); } break;
	case LOC_IDX_TRAY_PP_TBL_2: { strLocName.Format( "LoadTable2" ); } break;
	case LOC_IDX_TRAY_PP_TBL_3: { strLocName.Format( "LoadTable3" ); } break;
	case LOC_IDX_TRAY_PP_TBL_4: { strLocName.Format( "LoadTable4" ); } break;
	case LOC_IDX_TRAY_PP_SAFETY: { strLocName.Format( "SAFETY" ); } break;
	case LOC_IDX_TRAY_PP_FEEDER: { strLocName.Format( "FEEDER" ); } break;
	default: { strLocName.Format( "Target" ); };
	}

	return strLocName;
}

//알고리즘 Check일 경우, 전체 Picker 확인.
int CTaskTrayBasePP::CheckDevice(CString& strMsg, std::vector<CString>& vErrString)
{
	m_vDevLossX.clear();
	m_vDevLossY.clear();

	int nDeviceMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK);
	if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_ || nDeviceMode != eDeviceMode) {
		return ERR_NO_ERROR;
	}

	CString strTmp = _T(""), strLossDevice = _T(""), strMisData = _T(""), strErrBinCate = _T("");
	ST_DD_DEVICE stDvc;
	ST_DD_DEVICE stEmptyDevice;
	std::vector<CPoint> vLossDevice, vMisMatchDevice, vCategoryMisMatchDevice;
	vLossDevice.clear(); vMisMatchDevice.clear(); vCategoryMisMatchDevice.clear();
	m_vFailPicker.clear();
//	int nSortType = g_DMCont.m_dmEQP.GN(NDM0_SortType);// enum eSortingType{   

// 	std::vector<CString> vBin;
// 	for (int i = 0; i < g_BinCategory.nIndexCount; i++) {
// 		if (nSortType == eSortType_SwBin) {
// 			vBin.push_back(g_BinCategory.aCateInfo[i].szSWBin);
// 		}
// 		else {
// 			vBin.push_back(g_BinCategory.aCateInfo[i].szDesc);
// 		}
// 	}

 	for (int nPickY = 0; nPickY < MAX_TRAY_PP_CNT_Y; nPickY++)
 	{
 		for (int nPickX = 0; nPickX < MAX_TRAY_PP_CNT_X; nPickX++)
 		{
			CString strPkr = _T("");
 			CPoint Picker(nPickX, nPickY);
 			eExistState nExist;
 			GetDeviceDataExist(Picker, &nExist);
 			int nErr = ChkVacuum((eOnOffState)DEF_ON, Picker);

 			// case 1
 			strTmp.Empty(); // Vacuum이 감지 OFF, Data가 있는 상태
 			if (nErr != ERR_NO_ERROR && nExist == DEF_EXIST) {
 				strTmp.Format("[X:%d,Y:%d]", nPickX + 1, nPickY + 1);
 				strLossDevice += strTmp;
 				m_vDevLossX.push_back(nPickX);
 				m_vDevLossY.push_back(nPickY);
				vLossDevice.push_back(CPoint(nPickX, nPickY));
				if (GetRobotIdIdx() == eTrayPP_2_UD)
				{
					ST_DD_DEVICE stDevice;
					GetTargetCmdStageDvcData(nPickX, nPickY, &stDevice);
					strPkr.Format("[%s][Barcode=%s]", strTmp, stDevice.sz2DBarcode);
					vErrString.push_back(strPkr);
				}

 			}
 
 			// case 2
 			strTmp.Empty(); // Vacuum이 감지 ON, Data가 없는 상태
 			if (nErr == ERR_NO_ERROR && nExist != DEF_EXIST) {
 				strTmp.Format("[X:%d,Y:%d]", nPickX + 1, nPickY + 1);
 				strMisData += strTmp;
				vMisMatchDevice.push_back(CPoint(nPickX, nPickY));
 			}
 
//  			// case 3
//  			strTmp.Empty();
//  			if (nPickX >= (MAX_TRAY_PP_CNT_X)) {
//  				if (nExist == eDeviceData_Exist) {
//  					GetDeviceData_Picker(nPickX, nPickY, &stDvc);
//  					std::vector<CString>::iterator it = find(vBin.begin(), vBin.end(), (nSortType == eSortType_SwBin ? stDvc.szBinResult : stDvc.szErrString));
//  					if (it == vBin.end()) {
//  						SetDeviceData_Picker(nPickX, nPickY, &stEmptyDevice);
//  						g_JamRate.PushInOutJam(eJamType_Output, 1);
//  						strTmp.Format("[X:%d,Y:%d]", nPickX + 1, nPickY + 1);
//  						strErrBinCate += strTmp;
//  					}
//  				}
//  			}
 		}
 	}

	if (strLossDevice.IsEmpty() != TRUE) {
		if (GetRobotIdIdx() == eTrayPP_1_LD) {
			strMsg = strLossDevice;
		}

		if (vLossDevice.size() > 0)
			m_vFailPicker = vLossDevice;

		return ERR_TRAY_PP_1_DEVICE_MISS + m_nRobotId_Idx;
	}
	if (strMisData.IsEmpty() != TRUE) {
		strMsg = strMisData;
		if (vMisMatchDevice.size() > 0)
			m_vFailPicker = vMisMatchDevice;

		return ERR_TRAY_PP_1_DEVICE_INFO_MISSMATCH + m_nRobotId_Idx;
	}
// 	if (strErrBinCate.IsEmpty() != TRUE) {
// 		strMsg = strErrBinCate;
// 		if (vCategoryMisMatchDevice.size() > 0)
// 			m_vFailPicker = vCategoryMisMatchDevice;
// 
// 		return ERR_TRAY_PP_1_DEVICE_DATA_ERROR + m_nRobotId_Idx;
// 	}

	return ERR_NO_ERROR;
}

//Cmd Picker 확인.
int CTaskTrayBasePP::CheckDevice(CString& strMsg, std::vector<CPoint> vCmdPicker, std::vector<CString>& vErrString)
{
	m_vDevLossX.clear();
	m_vDevLossY.clear();

	int nDeviceMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK);
	if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_ || nDeviceMode != eDeviceMode) {
		return ERR_NO_ERROR;
	}

	CString strTmp = _T(""), strLossDevice = _T(""), strMisData = _T(""), strErrBinCate = _T("");
	ST_DD_DEVICE stDvc;
	ST_DD_DEVICE stEmptyDevice;
	int nSortType = g_DMCont.m_dmEQP.GN(NDM0_SortType);// enum eSortingType{   

	std::vector<CPoint> vLossDevice, vMisMatchDevice, vCategoryMisMatchDevice;
	vLossDevice.clear(); vMisMatchDevice.clear(); vCategoryMisMatchDevice.clear();
	m_vFailPicker.clear();

	std::vector<CString> vBin;
	for (int i = 0; i < g_BinCategory.nIndexCount; i++) {
		if (nSortType == eSortType_SwBin) {
			vBin.push_back(g_BinCategory.aCateInfo[i].szSWBin);
		}
		else {
			vBin.push_back(g_BinCategory.aCateInfo[i].szDesc);
		}
	}
	
	for (int i = 0; i < (int)vCmdPicker.size(); i++)
	{
		/*CPoint Picker(nPickX, nPickY);*/
		eExistState nExist;
		GetDeviceDataExist(vCmdPicker[i], &nExist);
		int nErr = ChkVacuum((eOnOffState)DEF_ON, vCmdPicker[i]);
		CString strPkr = _T("");
		// case 1
		strTmp.Empty();
		if (nErr != ERR_NO_ERROR && nExist == DEF_EXIST) {
			strTmp.Format("[X:%d,Y:%d]", vCmdPicker[i].x + 1, vCmdPicker[i].y + 1);
			strLossDevice += strTmp;
			m_vDevLossX.push_back(vCmdPicker[i].x);
			m_vDevLossY.push_back(vCmdPicker[i].y);
			vLossDevice.push_back(CPoint(vCmdPicker[i].x, vCmdPicker[i].y));
			if (GetRobotIdIdx() == eTrayPP_2_UD)
			{
				ST_DD_DEVICE stDevice;
				GetTargetCmdStageDvcData(vCmdPicker[i].x, vCmdPicker[i].y, &stDevice);
				strPkr.Format("[%s][Barcode=%s]", strTmp, stDevice.sz2DBarcode);
				vErrString.push_back(strPkr);
			}
		}

		// case 2
		strTmp.Empty();
		if (nErr == ERR_NO_ERROR && nExist != DEF_EXIST) {
			strTmp.Format("[X:%d,Y:%d]", vCmdPicker[i].x + 1, vCmdPicker[i].y + 1);
			strMisData += strTmp;	
			vMisMatchDevice.push_back(CPoint(vCmdPicker[i].x, vCmdPicker[i].y));
		}

		// case 3
// 		strTmp.Empty();
// 		if (vCmdPicker[i].x >= (MAX_TRAY_PP_CNT_X)) {
// 			if (nExist == eDeviceData_Exist) {
// 				GetDeviceData_Picker(vCmdPicker[i].x, vCmdPicker[i].y, &stDvc);
// 				std::vector<CString>::iterator it = find(vBin.begin(), vBin.end(), (nSortType == eSortType_SwBin ? stDvc.szBinResult : stDvc.szErrString));
// 				if (it == vBin.end()) {
// 					SetDeviceData_Picker(vCmdPicker[i].x, vCmdPicker[i].y, &stEmptyDevice);
// 					g_JamRate.PushInOutJam(eJamType_Output, 1);
// 					strTmp.Format("[X:%d,Y:%d]", vCmdPicker[i].x + 1, vCmdPicker[i].y + 1);
// 					strErrBinCate += strTmp;
// 					vCategoryMisMatchDevice.push_back(CPoint(vCmdPicker[i].x, vCmdPicker[i].y));
// 				}
// 			}
// 		}
	}

	if (strLossDevice.IsEmpty() != TRUE) {
		if (GetRobotIdIdx() == eTrayPP_1_LD) {
			strMsg = strLossDevice;
		}
		if (vLossDevice.size() > 0)
			m_vFailPicker = vLossDevice;

		return ERR_TRAY_PP_1_DEVICE_MISS + m_nRobotId_Idx;
	}
	if (strMisData.IsEmpty() != TRUE) {
		strMsg = strMisData;
		if (vMisMatchDevice.size() > 0)
			m_vFailPicker = vMisMatchDevice;

		return ERR_TRAY_PP_1_DEVICE_INFO_MISSMATCH + m_nRobotId_Idx;
	}
// 	if (strErrBinCate.IsEmpty() != TRUE) {
// 		strMsg = strErrBinCate;
// 		if (vCategoryMisMatchDevice.size() > 0)
// 			m_vFailPicker = vCategoryMisMatchDevice;
// 
// 		return ERR_TRAY_PP_1_DEVICE_DATA_ERROR + m_nRobotId_Idx;
// 	}

	return ERR_NO_ERROR;
}

void CTaskTrayBasePP::LossDevice()
{
	if (m_vDevLossX.empty() || m_vDevLossY.empty()) {
		return;
	}

	ST_DD_DEVICE stEmptyDevice;
	for (int nX = 0; nX < (int)m_vDevLossX.size(); nX++) {
		for (int nY = 0; nY < (int)m_vDevLossY.size(); nY++) {
			SetTargetCmdStageDvcData(m_vDevLossX[nX], m_vDevLossY[nY], &stEmptyDevice);
			g_JamRate.PushInOutJam((m_nRobotId_Idx == eTrayPP_1_LD ? eJamType_Input : eJamType_Output), 1);
		}
	}
	m_vDevLossX.clear();
	m_vDevLossY.clear();
}

void CTaskTrayBasePP::VAT_VisionLED_ONOFF( eOnOffState nCmdOnOff, eVATLED nCamUpDown )
{
	SetDO( m_nVATvisionLED_o_ONOFF[ nCamUpDown ], nCmdOnOff );
}

void CTaskTrayBasePP::VAT_VisionLED_AllOff()
{
	for( auto idx : m_nVATvisionLED_o_ONOFF ) {
		SetDO( idx, eOff );
	}
}


int CTaskTrayBasePP::GetHandPitchType(int nAxis, int nLocIdx)
{
	ASSERT(nAxis == CTaskTrayBasePP::eAxisPitchX || nAxis == CTaskTrayBasePP::eAxisPitchY);

	int nPocketPitchX = 0, nPocketPitchY = 0;
	int nPitchType_X = 0, nPitchType_Y = 0;
	int nHanPitchX = DEF_TRAY_PP_MAX_X_PITCH_WIDE, nHanPitchY = DEF_TRAY_PP_MAX_Y_PITCH_WIDE;

	switch (nLocIdx)
	{
	case LOC_IDX_TRAY_PP_TBL_1:
	case LOC_IDX_TRAY_PP_TBL_2:
	case LOC_IDX_TRAY_PP_TBL_3:
	case LOC_IDX_TRAY_PP_TBL_4:
	{
		nPocketPitchX = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Pitch_X);
		nPocketPitchY = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Pitch_Y);

		nHanPitchX = g_DMCont.m_dmTrayPP.GN(NDM5_TrayPP_LdTbl_Pitch_X);
		nHanPitchY = g_DMCont.m_dmTrayPP.GN(NDM5_TrayPP_LdTbl_Pitch_Y);

		// == calc pitch x type
		if (nPocketPitchX >= DEF_TRAY_PP_MAX_X_PITCH_NARROW && nPocketPitchX <= DEF_TRAY_PP_MAX_X_PITCH_WIDE)
		{
			double dContainerPitch_X_Cnt = (double)nHanPitchX / (double)nPocketPitchX;
			double Remainder_X = dContainerPitch_X_Cnt - (int)dContainerPitch_X_Cnt;	// one by one 인지 확인 한다.

			nPitchType_X = (int)dContainerPitch_X_Cnt;
			if (Remainder_X != 0) {				// 나머지가 있어서 one by one 진행 하여야한다. 한번에 내려놓을수 없는 조건
				nPitchType_X = 0;
			}
		}
		else if (nPocketPitchX > DEF_TRAY_PP_MAX_X_PITCH_WIDE)
		{
			double dContainerPitch_X_Cnt = (double)nPocketPitchX / (double)nHanPitchX;
			double Remainder_X = dContainerPitch_X_Cnt - (int)dContainerPitch_X_Cnt;	// one by one 인지 확인 한다.

			nPitchType_X = (int)dContainerPitch_X_Cnt * -1;
			if (Remainder_X != 0) {				// 나머지가 있어서 one by one 진행 하여야한다. 한번에 내려놓을수 없는 조건
				nPitchType_X = 0;
			}
		}
		// == calc pitch y type
		if (nPocketPitchY >= DEF_TRAY_PP_MAX_Y_PITCH_NARROW && nPocketPitchY <= DEF_TRAY_PP_MAX_Y_PITCH_WIDE)
		{
			double dContainerPitch_Y_Cnt = (double)nHanPitchY / (double)nPocketPitchY;
			double Remainder_Y = dContainerPitch_Y_Cnt - (int)dContainerPitch_Y_Cnt;	// one by one 인지 확인 한다.

			nPitchType_Y = (int)dContainerPitch_Y_Cnt;
			if (Remainder_Y != 0) {				// 나머지가 있어서 one by one 진행 하여야한다. 한번에 내려놓을수 없는 조건
				nPitchType_Y = 0;
			}
		}
		else if (nPocketPitchY > DEF_TRAY_PP_MAX_Y_PITCH_WIDE)
		{
			nPitchType_Y = 0;
		}
		if (nAxis == CTaskTrayBasePP::eAxisPitchX)
			return nPitchType_X;
		if (nAxis == CTaskTrayBasePP::eAxisPitchY)
			return nPitchType_Y;
	}break;
	case LOC_IDX_TRAY_PP_CONV_1:
	case LOC_IDX_TRAY_PP_CONV_2:
	case LOC_IDX_TRAY_PP_CONV_3:
	case LOC_IDX_TRAY_PP_CONV_4:
	case LOC_IDX_TRAY_PP_CONV_5:
	case LOC_IDX_TRAY_PP_CONV_6:
	case LOC_IDX_TRAY_PP_CONV_7:
	case LOC_IDX_TRAY_PP_FEEDER:
	{
		nPocketPitchX = g_DMCont.m_dmTrayPP.GN(NDM5_Tray_Pitch_X);
		nPocketPitchY = g_DMCont.m_dmTrayPP.GN(NDM5_Tray_Pitch_Y);

		if (nAxis == CTaskTrayBasePP::eAxisPitchX)
		{
			if (nPocketPitchX >= DEF_TRAY_PP_MAX_X_PITCH_NARROW && nPocketPitchX <= DEF_TRAY_PP_MAX_X_PITCH_WIDE)
				return 1; // 1x
			else if (nPocketPitchX < DEF_TRAY_PP_MAX_X_PITCH_NARROW) {
				if (nPocketPitchX * 2 >= DEF_TRAY_PP_MAX_X_PITCH_NARROW && nPocketPitchX * 2 <= DEF_TRAY_PP_MAX_X_PITCH_WIDE)
					return 2;  // 2x
				else if (nPocketPitchX * 3 >= DEF_TRAY_PP_MAX_X_PITCH_NARROW && nPocketPitchX * 3 <= DEF_TRAY_PP_MAX_X_PITCH_WIDE)
					return 3;  // 3x
			}
			// 그 외는 모두 one by one
		}

		if (nAxis == CTaskTrayBasePP::eAxisPitchY)
		{
			if (nPocketPitchY >= DEF_TRAY_PP_MAX_Y_PITCH_NARROW && nPocketPitchY <= DEF_TRAY_PP_MAX_Y_PITCH_WIDE)
				return 1; // 1x
			else if (nPocketPitchY < DEF_TRAY_PP_MAX_Y_PITCH_NARROW) {
				if (nPocketPitchY * 2 >= DEF_TRAY_PP_MAX_Y_PITCH_NARROW && nPocketPitchY * 2 <= DEF_TRAY_PP_MAX_Y_PITCH_WIDE)
					return 2;  // 2x
				else if (nPocketPitchY * 3 >= DEF_TRAY_PP_MAX_Y_PITCH_NARROW && nPocketPitchY * 3 <= DEF_TRAY_PP_MAX_Y_PITCH_WIDE)
					return 3;  // 3x
			}
			// 그 외는 모두 one by one
		}
	}break;
	}

	return 0;	// one by one
}

std::vector< std::pair< CPoint, CPoint > >
CTaskTrayBasePP::change_picker_to_pocket_point(int nCmdStage
	                                         , int nTargetX
	                                         , int nTargetY
	                                         , std::vector< CPoint > vCmdPicker)
{

	// [2x-pitch]
	int nX_PitchType = GetHandPitchType(CTaskTrayBasePP::eAxisPitchX, nCmdStage);
	int nY_PitchType = GetHandPitchType(CTaskTrayBasePP::eAxisPitchY, nCmdStage);

	typedef std::vector<CPoint>::const_iterator vIterPoint;
	std::pair<vIterPoint, vIterPoint> minX;
	std::pair<vIterPoint, vIterPoint> minY;
	if (m_vFullPkr.size() != vCmdPicker.size() && m_vFullPkr.size() > 0)
	{
		minX = std::minmax_element(m_vFullPkr.begin(), m_vFullPkr.end(), LessByMinX);
		minY = std::minmax_element(m_vFullPkr.begin(), m_vFullPkr.end(), LessByMinY);
	}
	else
	{
		minX = std::minmax_element(vCmdPicker.begin(), vCmdPicker.end(), LessByMinX);
		minY = std::minmax_element(vCmdPicker.begin(), vCmdPicker.end(), LessByMinY);
	}
	std::vector< std::pair< CPoint, CPoint > > vPickerToPocket;

	for each(auto picker in vCmdPicker) {
		// change picker position
		int nStartPickerX = picker.x - minX.first->x;
		int nStartPickerY = picker.y - minY.first->y;

		if (picker.x != minX.first->x)
		{
			if (nX_PitchType >= 0)
				nStartPickerX *= nX_PitchType;
			else {
				nStartPickerX /= abs(nX_PitchType);
			}
		}
		if (picker.y != minY.first->y)
		{
			if (nY_PitchType >= 0)
				nStartPickerY *= nY_PitchType;
			else {
				nStartPickerY /= abs(nY_PitchType);
			}
		}

		int nPosX = nTargetX + nStartPickerX;
		int nPosY = nTargetY + nStartPickerY;

		auto picker_pocket_pair = std::make_pair(picker, CPoint(nPosX, nPosY));
		vPickerToPocket.push_back(picker_pocket_pair);
	}

	return vPickerToPocket;
}

// Hand가 Unload용으로 짚고 있는 Device 수량 확인.
int CTaskTrayBasePP::GetTrayPpUnloadDvcCnt()
{
	// =====================================
	// check Tray PP unload picker Data
	//======================================
	int nCntUnloadPicker = 0;
	ST_DD_DEVICE stDvc;
	for (int nY = 0; nY < MAX_TRAY_PP_CNT_Y; nY++)
	{
		for (int nX = 0; nX < MAX_TRAY_PP_CNT_X; nX++)
		{
			char setting = m_TrayPP_picker_setting[nY][nX];
			g_DMCont.m_dmDevice.GetDeviceData(m_eMyDeviceStageNum, nX, nY, &stDvc);
			if (setting == 'U' && stDvc.sExist == eDeviceData_Exist)
			{
				nCntUnloadPicker++;
			}
		}
	}

	return nCntUnloadPicker;
}

// Hand가 Load용으로 짚고 있는 Device 수량 확인.
int CTaskTrayBasePP::GetTrayPpLoadDvcCnt()
{
	//==================================
	// check Tray PP load picker Data
	//==================================
	int nCntLoadPicker = 0;
	ST_DD_DEVICE stDvc;
	for (int nY = 0; nY < MAX_TRAY_PP_CNT_Y; nY++)
	{
		for (int nX = 0; nX < MAX_TRAY_PP_CNT_X; nX++)
		{
			char setting = m_TrayPP_picker_setting[nY][nX];
			g_DMCont.m_dmDevice.GetDeviceData(m_eMyDeviceStageNum, nX, nY, &stDvc);
			if (setting == 'L' && stDvc.sExist == eDeviceData_Exist)
			{
				nCntLoadPicker++;
			}
		}
	}

	return nCntLoadPicker;
}

int	CTaskTrayBasePP::GetBufferDeviceCnt()
{
	int nCnt = 0;
	ST_DD_DEVICE stDvc;
	for (int nY = 0; nY < 4; nY++)
	{
		for (int nX = 0; nX < 4; nX++)
		{
			g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TRAY_BUFF, nX, nY, &stDvc);
			if (stDvc.sExist == eDeviceData_Exist)
				nCnt++;
		}
	}

	return nCnt;
}

int CTaskTrayBasePP::TrayHandLocIdxConvertor(int nRobotID,int nLocIdx)
{
	int nLocConvertor = -1;

	if (nRobotID == RIDX_TRAY_PP_1)
	{
		switch (nLocIdx)
		{
		case LOC_IDX_TRAY_PP_FEEDER: {nLocConvertor = eLocIdxTrayLoadPP::LOC_IDX_TRAY_LOAD_PP_FEEDER; }break;
		case LOC_IDX_TRAY_PP_CONV_3: {nLocConvertor = eLocIdxTrayLoadPP::LOC_IDX_TRAY_LOAD_PP_CONV_3; }break;
		case LOC_IDX_TRAY_PP_TBL_1: {nLocConvertor = eLocIdxTrayLoadPP::LOC_IDX_TRAY_LOAD_PP_TBL_1; }break;
		case LOC_IDX_TRAY_PP_TBL_2: {nLocConvertor = eLocIdxTrayLoadPP::LOC_IDX_TRAY_LOAD_PP_TBL_2; }break;
		case LOC_IDX_TRAY_PP_TBL_3: {nLocConvertor = eLocIdxTrayLoadPP::LOC_IDX_TRAY_LOAD_PP_TBL_3; }break;
		case LOC_IDX_TRAY_PP_TBL_4: {nLocConvertor = eLocIdxTrayLoadPP::LOC_IDX_TRAY_LOAD_PP_TBL_4; }break;
		case LOC_IDX_TRAY_PP_SAFETY: { nLocConvertor = eLocIdxTrayLoadPP::LOC_IDX_TRAY_LOAD_PP_SAFETY; }break;
		default: { nLocConvertor = eLocIdxTrayLoadPP::LOC_IDX_TRAY_LOAD_PP_SAFETY; } break;
		}
	}
	else
	{
		switch (nLocIdx)
		{
		case LOC_IDX_TRAY_PP_CONV_4: {nLocConvertor = eLocIdxTrayUnLoadPP::LOC_IDX_TRAY_UNLOAD_PP_CONV_4; }break;
		case LOC_IDX_TRAY_PP_CONV_5: {nLocConvertor = eLocIdxTrayUnLoadPP::LOC_IDX_TRAY_UNLOAD_PP_CONV_5; }break;
		case LOC_IDX_TRAY_PP_CONV_6: {nLocConvertor = eLocIdxTrayUnLoadPP::LOC_IDX_TRAY_UNLOAD_PP_CONV_6; }break;
		case LOC_IDX_TRAY_PP_CONV_7: {nLocConvertor = eLocIdxTrayUnLoadPP::LOC_IDX_TRAY_UNLOAD_PP_CONV_7; }break;
		case LOC_IDX_TRAY_PP_TBL_1: {nLocConvertor = eLocIdxTrayUnLoadPP::LOC_IDX_TRAY_UNLOAD_PP_TBL_1; }break;
		case LOC_IDX_TRAY_PP_TBL_2: {nLocConvertor = eLocIdxTrayUnLoadPP::LOC_IDX_TRAY_UNLOAD_PP_TBL_2; }break;
		case LOC_IDX_TRAY_PP_TBL_3: {nLocConvertor = eLocIdxTrayUnLoadPP::LOC_IDX_TRAY_UNLOAD_PP_TBL_3; }break;
		case LOC_IDX_TRAY_PP_TBL_4: {nLocConvertor = eLocIdxTrayUnLoadPP::LOC_IDX_TRAY_UNLOAD_PP_TBL_4; }break;
		case LOC_IDX_TRAY_PP_SAFETY: { nLocConvertor = eLocIdxTrayUnLoadPP::LOC_IDX_TRAY_UNLOAD_PP_SAFETY; }break;
		default: { nLocConvertor = eLocIdxTrayUnLoadPP::LOC_IDX_TRAY_UNLOAD_PP_SAFETY; } break;
		}
	}

	return nLocConvertor;
}


int CTaskTrayBasePP::TrayHandManuLocIdxConvertor(int nRobotID, int nLocIdx)
{
	int nLocConvertor = -1;

	if (nRobotID == RIDX_TRAY_PP_1)
	{
		switch (nLocIdx)
		{
		case  eLocIdxTrayLoadPPManual::LOC_IDX_TRAY_LOAD_PP_MANUAL_FEEDER: {nLocConvertor = LOC_IDX_TRAY_PP_FEEDER; }break;
		case eLocIdxTrayLoadPPManual::LOC_IDX_TRAY_LOAD_PP_MANUAL_CONV_3: {nLocConvertor = LOC_IDX_TRAY_PP_CONV_3; }break;
//		case eLocIdxTrayLoadPP::LOC_IDX_TRAY_LOAD_PP_TBL_1: {nLocConvertor = LOC_IDX_TRAY_PP_TBL_1; }break;
		case eLocIdxTrayLoadPPManual::LOC_IDX_TRAY_LOAD_PP_MANUAL_TBL_2: {nLocConvertor = LOC_IDX_TRAY_PP_TBL_2; }break;
		case  eLocIdxTrayLoadPPManual::LOC_IDX_TRAY_LOAD_PP_MANUAL_TBL_3: {nLocConvertor = LOC_IDX_TRAY_PP_TBL_3; }break;
//		case eLocIdxTrayLoadPP::LOC_IDX_TRAY_LOAD_PP_TBL_4: {nLocConvertor = LOC_IDX_TRAY_PP_TBL_4; }break;
		case eLocIdxTrayLoadPPManual::LOC_IDX_TRAY_LOAD_PP_MANUAL_SAFETY: { nLocConvertor = LOC_IDX_TRAY_PP_SAFETY; }break;
		default:
			break;
		}
	}
	else
	{
		switch (nLocIdx)
		{
		case eLocIdxTrayUnLoadPPManual::LOC_IDX_TRAY_UNLOAD_PP_MANUAL_CONV_4: {nLocConvertor = LOC_IDX_TRAY_PP_CONV_4; }break;
		case eLocIdxTrayUnLoadPPManual::LOC_IDX_TRAY_UNLOAD_PP_MANUAL_CONV_5: {nLocConvertor = LOC_IDX_TRAY_PP_CONV_5; }break;
		case eLocIdxTrayUnLoadPPManual::LOC_IDX_TRAY_UNLOAD_PP_MANUAL_CONV_6: {nLocConvertor = LOC_IDX_TRAY_PP_CONV_6; }break;
		case eLocIdxTrayUnLoadPPManual::LOC_IDX_TRAY_UNLOAD_PP_MANUAL_CONV_7: {nLocConvertor = LOC_IDX_TRAY_PP_CONV_7; }break;
		case eLocIdxTrayUnLoadPPManual::LOC_IDX_TRAY_UNLOAD_PP_MANUAL_TBL_1: {nLocConvertor = LOC_IDX_TRAY_PP_TBL_1; }break;
// 		case eLocIdxTrayUnLoadPP::LOC_IDX_TRAY_UNLOAD_PP_TBL_2: {nLocConvertor = LOC_IDX_TRAY_PP_TBL_2; }break;
// 		case eLocIdxTrayUnLoadPP::LOC_IDX_TRAY_UNLOAD_PP_TBL_3: {nLocConvertor = LOC_IDX_TRAY_PP_TBL_3; }break;
		case eLocIdxTrayUnLoadPPManual::LOC_IDX_TRAY_UNLOAD_PP_MANUAL_TBL_4: {nLocConvertor = LOC_IDX_TRAY_PP_TBL_4; }break;
		case eLocIdxTrayUnLoadPPManual::LOC_IDX_TRAY_UNLOAD_PP_MANUAL_SAFETY: { nLocConvertor = LOC_IDX_TRAY_PP_SAFETY; }break;
		default:
			break;
		}
	}

	return nLocConvertor;
}


CString CTaskTrayBasePP::GetPickerName(CPoint Picker)
{
	CString strPicker = _T("");
	if (Picker.y == 0) {
		strPicker += _T("[R");
	}
	else {
		strPicker += _T("[F");
	}

	CString strPickerX = _T("");
	strPickerX.Format("L%d]", Picker.x + 1);
	strPicker += strPickerX;

	return strPicker;
}

CString CTaskTrayBasePP::GetPickerName(std::vector<CPoint> vPicker)
{
	CString strPicker = _T("");

	for (std::vector<CPoint>::size_type i = 0; i < vPicker.size(); i++) {
		strPicker += GetPickerName(vPicker[i]);
	}

	return strPicker;
}

void CTaskTrayBasePP::SetPkrImpactFeeder(int nPkrIdx)
{
	m_vImpactPkr.push_back(nPkrIdx);
}
void CTaskTrayBasePP::GetPkrImpactFeeder(std::vector<int> &vPkr)
{
	vPkr = m_vImpactPkr;
}

void CTaskTrayBasePP::ClearImpactFeeder()
{
	m_vImpactPkr.clear();
}

int CTaskTrayBasePP::ProcVacuumCheckMem(eLocIdxTrayPP       eDestStage,
	int                 nDestPocektX,
	int                 nDestPocketY,
	std::vector<CPoint> vCmdPicker,
	ePPCmd              ePickPlace)
{
	ASSERT(eDestStage >= 0);
	ASSERT((nDestPocektX >= 0) && (nDestPocektX < DEVICE_DATA_MAX_X));
	ASSERT((nDestPocketY >= 0) && (nDestPocketY < DEVICE_DATA_MAX_Y));
	ASSERT(vCmdPicker.size() != 0);

	m_vDevLossX.clear();
	m_vDevLossY.clear();
	m_vFailPicker.clear();

	ST_DD_DEVICE stSourceData;
	ST_DD_DEVICE stEmptyDevice;
	eDeviceDataStage eDvcDataStageIdx = eDeviceStage_Undefine;
	int nRetError = ERR_NO_ERROR;

	switch (eDestStage)
	{
	case LOC_IDX_TRAY_PP_TBL_1:
	case LOC_IDX_TRAY_PP_TBL_2:
	case LOC_IDX_TRAY_PP_TBL_3:
	case LOC_IDX_TRAY_PP_TBL_4:

	{
		typedef std::vector<CPoint>::const_iterator vIterPoint;
		ST_DD_DEVICE stTargetData;

		std::vector< std::pair< CPoint, CPoint > > vPickerToPocket = change_picker_to_pocket_point(eDestStage, nDestPocektX, nDestPocketY, vCmdPicker);

		ST_DAILY_REPORT stDailyData;

		if (ePickPlace == DEF_PICK) { // Pick loop
			for (int i = 0; i < (int)vCmdPicker.size(); i++)
			{
				auto iter_current_picker_pocket = std::find_if(vPickerToPocket.begin(), vPickerToPocket.end()
					, [&](std::pair< CPoint, CPoint > _picker_to_pocket) -> bool
				{
					if (_picker_to_pocket.first == vCmdPicker[i])
						return true;

					return false;
				});

				CPoint pocket = iter_current_picker_pocket->second;
				int nPosX = pocket.x;
				int nPosY = pocket.y;
				// Get Source data ( Source = Setplate or Table or Buff ) 					
				g_TaskLoadTbl[eDestStage].Get_Table_DM_Vacuum_Check(nPosX, nPosY);

				// checking hand picker vacuum sensor 
				int nChkSen = g_IO.GetDriverType() == _DIO_TYPE_SIMUL_ ? GetVacuum(vCmdPicker[i]) : ChkVacuum(eOnOffState::eOff, vCmdPicker[i]);

				if (nChkSen == ERR_NO_ERROR) { // Table Pocket을 Pick하고 나서 Hand에 Sensing이 되지 않아야 지만 공유 메모리 TRUE로 만들어 준다. 
					g_TaskLoadTbl[eDestStage].Set_Table_DM_Vacuum_Check(nPosX, nPosY, TRUE);
				}
				else {
					m_vFailPicker.push_back(CPoint(vCmdPicker[i].x, vCmdPicker[i].y));
					nRetError = ERR_TRAY_PP_1_TABLE_VACUUM_CHECK_DEVICE_SENSING + m_nRobotId_Idx;
				}
			}
		}
	}break;
	}

	return nRetError;
}





//====================================================================================================================================
//Vision Auto Teaching
//====================================================================================================================================

int	CTaskTrayBasePP::do_VATMovePos(eMotor nAxisIdx, double dTargetPos, double dPosOffset/* = 0.0*/)
{
	//ASSERT(0 <= nLocStage && nLocStage < eLocIdxTrayPP::MAX_TRAY_PP_LOC_COUNT);
	ASSERT(0 <= nAxisIdx && nAxisIdx < eMotor::eAxisMax);
	//ASSERT(0 <= nSeqSpdRate && nSeqSpdRate <= 100);

	OneAxis* pMotor = GetAxis(nAxisIdx);
	if (pMotor == nullptr) {
		return ERR_MOTOR_DATA_IS_NULL_POINTER;
	}

	int	   nRateSpd = DEF_VAT_MOTOR_SPEED_RATE;/*(double)g_DMCont.m_dmTrayPP.GN(nIndexSpd[m_nRobotId_Idx][nAxisIdx]);*/
	int    nRateAcc = DEF_VAT_MOTOR_ACC_RATE;/*g_DMCont.m_dmTrayPP.GN(nIndexAcc[m_nRobotId_Idx][nAxisIdx]);*/
	int    nRateDcc = DEF_VAT_MOTOR_DCC_RATE;/*g_DMCont.m_dmTrayPP.GN(nIndexDcc[m_nRobotId_Idx][nAxisIdx]);*/

	double dRetSpd = 0;
	int    nRetAcc = 0, nRetDcc = 0;

	double dMtSpd = pMotor->GetSpeed();
	double dMtAcc = pMotor->GetAcc();
	double dMtDcc = pMotor->GetDcc();


	dRetSpd = (dMtSpd * (nRateSpd / 100.0)); /** (nSeqSpdRate / 100.0);*/
	nRetAcc = (int)(dMtAcc - (dMtAcc * 0.75) *(nRateAcc / 100.0)); // 설정되어 있는 값에서 spd rate가 높을 수록 acc/dcc를 감소 시켜준다. 가장 빠를 때가 설정 속도의 95%뺀 acc/dcc로 함.
	nRetDcc = (int)(dMtDcc - (dMtDcc * 0.75) *(nRateDcc / 100.0));

	if (nRetAcc < 100) nRetAcc = 100;
	if (nRetDcc < 100) nRetDcc = 100;

	double dFTPos = dTargetPos + dPosOffset;
	MakeVatLog("%s : Final Target Pos : %.3fmm, Org Target Pos : %.3fmm, Pos Offset : %.3fmm", pMotor->GetAxisName(), dFTPos / 1000.0, dTargetPos / 1000.0, dPosOffset / 1000.0);

	int nErr = -1;
	//	BOOL bZpickerNouseOffset = FALSE;

	// 	if (nLocStage == eLocIdxTrayPP::LOC_IDX_TRAY_PP_SAFETY || nLocZ == eZPOS_SAFETY)
	// 	{
	// 		bZpickerNouseOffset = TRUE;
	// 	}

	//VAT 측정 시, 속도 조절
	// 	if (g_DMCont.m_dmVAT.GB(BDM14_VAT_TEACHING_USE) == TRUE) {
	// 		dRetSpd = dMtSpd * (VAT_MOTOR_SPEED / 100.0);
	// 		bZpickerNouseOffset = TRUE;
	// 	}
	// 	else { //Running시, Picker Gap 저장
	// 		if (nAxisIdx == eAxisZ && nLocStage != LOC_IDX_TRAY_PP_SAFETY)
	// 			g_pBbxmnpClient->SetZAutoTeachingData((m_nRobotId_Idx == RIDX_TRAY_PP_1) ? g_VATCommonData.m_stVATZTeachInfo.dLoadTrayPp_Loc_PickerGap[nLocStage] : g_VATCommonData.m_stVATZTeachInfo.dUnloadTrayPp_Loc_PickerGap[nLocStage]);
	// 	}

	//	int nBoardType = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);
	//	BOOL bVatOffsetUse = g_DMCont.m_dmVAT.GB(BDM14_VAT_MD_USE);

	// 	if (nAxisIdx == eAxisZ) {
	// 		nErr = pMotor->Move(vPicker, dFTPos, dRetSpd, nRetAcc, nRetDcc, nBoardType, bZpickerNouseOffset, bVatOffsetUse);
	// 	}
	// 	else {
	// 		nErr = pMotor->Move(dFTPos, dRetSpd, nRetAcc, nRetDcc, TRUE);
	// 	}

	nErr = pMotor->Move(dFTPos, dRetSpd, nRetAcc, nRetDcc, TRUE); // ApplyBacklash = TRUE


	return nErr;
}


int	CTaskTrayBasePP::do_VATMovePos_Z(std::vector<CPoint> vPicker, double dTargetPos, double dPosOffset/* = 0.0*/)
{
	//ASSERT(0 <= nLocStage && nLocStage < eLocIdxTrayPP::MAX_TRAY_PP_LOC_COUNT);
	//ASSERT(0 <= nAxisIdx && nAxisIdx < eMotor::eAxisMax);
	//ASSERT(0 <= nSeqSpdRate && nSeqSpdRate <= 100);

	OneAxis* pMotor = GetAxis(CTaskTrayBasePP::eAxisZ);
	if (pMotor == nullptr) {
		return ERR_MOTOR_DATA_IS_NULL_POINTER;
	}


	int	   nRateSpd = DEF_VAT_MOTOR_SPEED_RATE;/*(double)g_DMCont.m_dmTrayPP.GN(nIndexSpd[m_nRobotId_Idx][nAxisIdx]);*/
	int    nRateAcc = DEF_VAT_MOTOR_ACC_RATE;/*g_DMCont.m_dmTrayPP.GN(nIndexAcc[m_nRobotId_Idx][nAxisIdx]);*/
	int    nRateDcc = DEF_VAT_MOTOR_DCC_RATE;/*g_DMCont.m_dmTrayPP.GN(nIndexDcc[m_nRobotId_Idx][nAxisIdx]);*/

	double dRetSpd = 0;
	int    nRetAcc = 0, nRetDcc = 0;

	double dMtSpd = pMotor->GetSpeed();
	double dMtAcc = pMotor->GetAcc();
	double dMtDcc = pMotor->GetDcc();


	dRetSpd = (dMtSpd * (nRateSpd / 100.0)); /** (nSeqSpdRate / 100.0);*/
	nRetAcc = (int)(dMtAcc - (dMtAcc * 0.75) *(nRateAcc / 100.0)); // 설정되어 있는 값에서 spd rate가 높을 수록 acc/dcc를 감소 시켜준다. 가장 빠를 때가 설정 속도의 95%뺀 acc/dcc로 함.
	nRetDcc = (int)(dMtDcc - (dMtDcc * 0.75) *(nRateDcc / 100.0));

	if (nRetAcc < 100) nRetAcc = 100;
	if (nRetDcc < 100) nRetDcc = 100;

	double dFTPos = dTargetPos + dPosOffset;
	MakeVatLog("%s : Final Target Pos : %.3fmm, Org Target Pos : %.3fmm, Pos Offset : %.3fmm", pMotor->GetAxisName(), dFTPos / 1000.0, dTargetPos / 1000.0, dPosOffset / 1000.0);

	int nErr = -1;


	// 	if (nLocStage == eLocIdxTrayPP::LOC_IDX_TRAY_PP_SAFETY || nLocZ == eZPOS_SAFETY)
	// 	{
	// 		bZpickerNouseOffset = TRUE;
	// 	}

	//VAT 측정 시, 속도 조절
	// 	if (g_DMCont.m_dmVAT.GB(BDM14_VAT_TEACHING_USE) == TRUE) {
	// 		dRetSpd = dMtSpd * (VAT_MOTOR_SPEED / 100.0);
	// 		bZpickerNouseOffset = TRUE;
	// 	}
	// 	else { //Running시, Picker Gap 저장
	// 		if (nAxisIdx == eAxisZ && nLocStage != LOC_IDX_TRAY_PP_SAFETY)
	// 			g_pBbxmnpClient->SetZAutoTeachingData((m_nRobotId_Idx == RIDX_TRAY_PP_1) ? g_VATCommonData.m_stVATZTeachInfo.dLoadTrayPp_Loc_PickerGap[nLocStage] : g_VATCommonData.m_stVATZTeachInfo.dUnloadTrayPp_Loc_PickerGap[nLocStage]);
	// 	}

	//	int nBoardType = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);
	//	BOOL bVatOffsetUse = g_DMCont.m_dmVAT.GB(BDM14_VAT_MD_USE);

	// 	if (nAxisIdx == eAxisZ) {
	// 		nErr = pMotor->Move(vPicker, dFTPos, dRetSpd, nRetAcc, nRetDcc, nBoardType, bZpickerNouseOffset, bVatOffsetUse);
	// 	}
	// 	else {
	// 		nErr = pMotor->Move(dFTPos, dRetSpd, nRetAcc, nRetDcc, TRUE);
	// 	}

	BOOL bZpickerNouseOffset = TRUE;
	int nBoardType = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);
	if (vPicker.size() == 1) {
		//Picker 1개 제어
		// 		int nMotor = (m_nRobotId_Idx)
		// 		int nStartPicker = g_pBbxmnpClient->ZAxisStartPicker(eMotor);
		int nPickerNum = vPicker[0].x + 4 * (vPicker[0].y);

		nErr = pMotor->Move((_eBbxmnpAxis)m_nBbxmnpAxis[nPickerNum], dFTPos, dRetSpd, nRetAcc, nRetDcc, nBoardType, bZpickerNouseOffset);
	}
	else {
		//Picker 여러개 제어
		nErr = pMotor->Move(vPicker, dFTPos, dRetSpd, nRetAcc, nRetDcc, nBoardType, bZpickerNouseOffset);
	}

	return nErr;
}


void CTaskTrayBasePP::_SetFlatnessOffset(int nStage)
{
	ASSERT(VAT_IDX_TRAY_PP_TBL_1 <= nStage && nStage <= VAT_IDX_TRAY_PP_FEEDER);

	double dPickerGap[VAT_PICKER_MAX] = { 0, };

	int nHand = (GetRobotIdIdx() == RIDX_TRAY_PP_1) ? VAT_HAND_LOAD_TRAYPP : VAT_HAND_UNLOAD_TRAYPP;

	for (int i = 0; i < VAT_PICKER_MAX; i++) {
		dPickerGap[i] = g_VATCommonData.m_stVATZTeachInfo.stPickerGap.getPickerGap(nHand, nStage, i);
	}

	g_pBbxmnpClient->SetFlatnessOffset(nHand, dPickerGap);
}