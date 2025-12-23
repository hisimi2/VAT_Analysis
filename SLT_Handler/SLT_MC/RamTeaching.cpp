// RamTeaching.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "RamTeaching.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

// CRamTeaching

CRamTeaching::CRamTeaching()
{
	m_bLoaded = FALSE;
}

CRamTeaching::~CRamTeaching()
{
}


// CRamTeaching 멤버 함수


int  CRamTeaching::Initialize(char* szDriverFileName, char* szCokName)
{
	return 	CTeachingFile::Initialize(szDriverFileName, szCokName);
}
void CRamTeaching::Finalize()
{
	CTeachingFile::Finalize();
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-10-09]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CRamTeaching::Load()
{
	CMyList cRobotNameList;
	CTeachingFile::ReadRobotNameList(cRobotNameList);
	int i;
	int nRobotCount = cRobotNameList.m_fnGetCount();
	for(i=0; i<nRobotCount; i++){
		TRobotName* pItem = (TRobotName*)cRobotNameList.m_fnGetData(i);
		if( pItem != NULL ){
			ZeroMemory(&m_stRobotTeach[pItem->nRobotId], sizeof(_ST_ROBOT_TEACHING));
			CTeachingFile::ReadTeachingData(m_szCokName, pItem->nRobotId, &m_stRobotTeach[pItem->nRobotId]);
		}
	}

	m_bLoaded = TRUE;
	return 0;
}
/*=============================================================================
  AUTHOR   : 손정일  [2017-04-11]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : Contact Control Menu에서 Press Contact Pos 변경이 되기 때문에 
             press Teaching Pos 를 다시 불러 온다.
==============================================================================*/
int CRamTeaching::PressTeachingDataLoad()
{
	for(int i =RIDX_PRESS_UNIT_1; i <= RIDX_PRESS_UNIT_8; i++){
		CTeachingFile::ReadTeachingData(m_szCokName, RID_PRESS_UNIT_(i), &m_stRobotTeach[RID_PRESS_UNIT_(i)]);
	}
	return 0;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-10-09]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CRamTeaching::Save()
{
	CMyList cRobotNameList;
	CTeachingFile::ReadRobotNameList(cRobotNameList);
	int i;
	int nRobotCount = cRobotNameList.m_fnGetCount();
	for(i=0; i<nRobotCount; i++){
		TRobotName* pItem = (TRobotName*)cRobotNameList.m_fnGetData(i);
		if( pItem != NULL ){
			CTeachingFile::WriteTeachingData(m_szCokName, pItem->nRobotId, &m_stRobotTeach[pItem->nRobotId]);
		}
	}

	m_bLoaded = TRUE;
	return 0;
}

int CRamTeaching::GetTeachPos(int nRobotID, int nLocIdx, ST_LOCATION* lpstData)
{
	ASSERT( nRobotID >= 0 && nRobotID < MAX_ROBOT_COUNT  );
	ASSERT( nLocIdx  >= 0 && nLocIdx  < MAX_LOC_IDX_COUNT);
	ASSERT( m_bLoaded == TRUE );

	memcpy_s(lpstData, sizeof(ST_LOCATION), &m_stRobotTeach[nRobotID].aLoc[nLocIdx], sizeof(ST_LOCATION));

	return 0;
}

double CRamTeaching::GetTeachPos(int nRobotID, int nLocIdx, int nPosIdx)
{
	ASSERT( nRobotID >= 0 && nRobotID < MAX_ROBOT_COUNT  );
	ASSERT( nLocIdx  >= 0 && nLocIdx  < MAX_LOC_IDX_COUNT);
	ASSERT( nPosIdx  >= 0 && nPosIdx  < MAX_POS_IDX_COUNT);
	ASSERT( m_bLoaded == TRUE );

	return m_stRobotTeach[nRobotID].aLoc[nLocIdx].dPos[nPosIdx];
}

int CRamTeaching::GetTeachPosHead(int nRobotID, ST_LOCATION_HEAD &tLocHead )
{
	ASSERT( nRobotID >= 0 && nRobotID < MAX_ROBOT_COUNT  );
	ASSERT( m_bLoaded == TRUE );

	tLocHead = m_stRobotTeach[nRobotID].stLocHead;

	return 0;
}


int CRamTeaching::SetTeachPos(int nRobotID, int nLocIdx, ST_LOCATION* lpstData)
{
	ASSERT( nRobotID >= 0 && nRobotID < MAX_ROBOT_COUNT  );
	ASSERT( nLocIdx  >= 0 && nLocIdx  < MAX_LOC_IDX_COUNT);

	memcpy_s((void*)&m_stRobotTeach[nRobotID].aLoc[nLocIdx], sizeof(ST_LOCATION), (void*)lpstData, sizeof(ST_LOCATION));

	return 0;
}

int CRamTeaching::SetTeachPos(int nRobotID, int nLocIdx, int nPosIdx, double dValue)
{
	ASSERT( nRobotID >= 0 && nRobotID < MAX_ROBOT_COUNT  );
	ASSERT( nLocIdx  >= 0 && nLocIdx  < MAX_LOC_IDX_COUNT);
	ASSERT( nPosIdx  >= 0 && nPosIdx  < MAX_POS_IDX_COUNT);
	ASSERT( m_bLoaded == TRUE );

	m_stRobotTeach[nRobotID].aLoc[nLocIdx].dPos[nPosIdx] = dValue;

	return 0;
}
