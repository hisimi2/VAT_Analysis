#pragma once

#include <vector>


class CTeachingFile;
// CRamTeaching 명령 대상입니다.

class CRamTeaching : public CTeachingFile
{
public:
	CRamTeaching();
	virtual ~CRamTeaching();

	int  Initialize(char* szDriverFileName, char* szCokName);
	void Finalize();

	int Load();
	int Save();
	int GetTeachPos(int nRobotID, int nLocIdx, ST_LOCATION* lpstData);
	int SetTeachPos(int nRobotID, int nLocIdx, ST_LOCATION* lpstData);
	double GetTeachPos(int nRobotID, int nLocIdx, int nPosIdx);
	int GetTeachPosHead(int nRobotID, ST_LOCATION_HEAD &tLocHead );
	int SetTeachPos(int nRobotID, int nLocIdx, int nPosIdx, double dValue);
	int PressTeachingDataLoad();
private:
	BOOL              m_bLoaded;
	ST_ROBOT_TEACHING m_stRobotTeach[MAX_ROBOT_COUNT];
};


/*========================================================================= Guide Code
  vector<double> vPos;
  GetTeachPos(RID_PRESS_UNIT_DN_L_(0), LOC_IDX_PRESS_UNIT_CONTACT, vPos);
  double dContact = vPos[LOC_IDX_PRESS_UNIT_CONTACT];
=========================================================================*/