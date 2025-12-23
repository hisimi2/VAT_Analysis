#pragma once


// CRamMotorParam 명령 대상입니다.

class CRamMotorParam : public CObject
{
public:
	CRamMotorParam();
	virtual ~CRamMotorParam();

	void Load();
	int  GetParam(int nAxisNo, _tMOTOR_PARAM** pMotorParam);

	CMyList        m_MotorParamList;
	_tMOTOR_PARAM* m_MotorParam;
	int            m_nAxisCount;
};


