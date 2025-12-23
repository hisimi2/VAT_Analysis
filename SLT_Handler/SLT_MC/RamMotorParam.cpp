// RamMotorParam.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "RamMotorParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// CRamMotorParam

CRamMotorParam::CRamMotorParam()
{
	m_nAxisCount = 0;
	m_MotorParam = NULL;
}

CRamMotorParam::~CRamMotorParam()
{
	if( m_MotorParam != NULL ){
		delete [] m_MotorParam;
		m_MotorParam = NULL;
	}
}


// CRamMotorParam 멤버 함수

void CRamMotorParam::Load()
{
	m_MotorParamList.m_fnClear();
	g_DB.SELECT_MOTORPARAM_TBL(m_MotorParamList);
	m_nAxisCount = m_MotorParamList.m_fnGetCount();

	m_MotorParam = new _tMOTOR_PARAM [m_nAxisCount];

	for(int i=0; i<m_nAxisCount; i++){
		_tMOTOR_PARAM* pItem = (_tMOTOR_PARAM*)m_MotorParamList.m_fnGetData(i);
		if( pItem != NULL ){
				m_MotorParam[i] = *pItem;
		}
	}
}

int  CRamMotorParam::GetParam(int nAxisNo, _tMOTOR_PARAM** pMotorParam)
{
	// 1 안
	*pMotorParam = &m_MotorParam[nAxisNo];
	return 0;

	// 2 안
// 	int nCount = m_MotorParamList.m_fnGetCount();
// 	for(int i=0; i<nCount; i++){
// 		_tMOTOR_PARAM* pItem = (_tMOTOR_PARAM*)m_MotorParamList.m_fnGetData(i);
// 		if( pItem != NULL ){
// 			if( pItem->nAxisNo_Logical == nAxisNo ){
// 				pMotorParam = pItem;
// 				return 0;
// 			}
// 		}
// 	}
// 	return -1;

	// 3 안
// 	pMotorParam = (_tMOTOR_PARAM*)m_MotorParamList.m_fnGetData(nAxisNo);
// 	return 0;
}