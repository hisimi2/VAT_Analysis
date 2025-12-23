#include "StdAfx.h"
#include "MotionManager.h"

#include "ConfigData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMotionManager::CMotionManager(void)
	:  m_pMotionConfig(nullptr), m_pAbnormal(nullptr)
{
	m_nNumOfBoard = 0;
	m_nNumOfAxes = 0;

	m_pAbnormal = new CMotionConfig;
}


CMotionManager::~CMotionManager(void)
{
	if (m_pMotionConfig != nullptr)
	{
		delete [] m_pMotionConfig;
		m_pMotionConfig = nullptr;
	}

	if (m_pAbnormal != nullptr)
	{
		delete m_pAbnormal;
		m_pAbnormal = nullptr;
	}
}

int CMotionManager::Initialize( char* szFilePath )
{
	int nRet = 0;

	CConfigData cfData;
	cfData.SetFileName(szFilePath);

	int nNumOfBoard = cfData.GetInt("COMMON", "Number of Board", "0");
	if (nNumOfBoard <= 0) return -1;
	SetNumOfBoard(nNumOfBoard);

	int nNumOfAxes = cfData.GetInt("COMMON", "Number of Axes", "0");
	if (nNumOfAxes <= 0) return -2;
	SetNumOfAxes(nNumOfAxes);

	m_pMotionConfig = new CMotionConfig[nNumOfAxes];

	char szSection[256] = {0};
	char szKey[256] = {0};
	for (int i = 0; i < nNumOfAxes; i++)
	{
		sprintf_s(szSection, sizeof(szSection), "AXIS_%02d", i);
		m_pMotionConfig[i].m_nBoardNum = cfData.GetInt(szSection, "Board Number", "0");
		m_pMotionConfig[i].m_nAxisNum = cfData.GetInt(szSection, "Axis Number", "0");
		m_pMotionConfig[i].m_nPulseType = cfData.GetInt(szSection, "Pulse Type", "0");
		m_pMotionConfig[i].m_nEncoderExist = cfData.GetInt(szSection, "Encoder Exist", "1");
		cfData.GetString(szSection, "Axis Name", "", m_pMotionConfig[i].m_strAxisName.GetBuffer(0));
	}

	return nRet;
}

CMotionConfig& CMotionManager::operator[]( _eBbxmnpAxis eIndex ) const
{
	if (m_pMotionConfig == nullptr) {
		return *m_pAbnormal;
	}
	else {
		return m_pMotionConfig[eIndex];
	}
}
