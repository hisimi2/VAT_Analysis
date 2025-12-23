#pragma once

#include "DefineBbxmnp.h"

class CMotionConfig
{
public:
	CMotionConfig()
		: m_nBoardNum(0), m_nAxisNum(0), m_strAxisName(""), m_nPulseType(0), m_nEncoderExist(0)
	{
	}

	~CMotionConfig() {}

private:
public:
	int m_nBoardNum;
	int m_nAxisNum;
	CString m_strAxisName;
	int m_nPulseType;
	int m_nEncoderExist;
};

class CMotionManager
{
public:
	CMotionManager(void);
	~CMotionManager(void);

	int Initialize(char* szFilePath);
	CMotionConfig& operator[](_eBbxmnpAxis eIndex) const;

	int GetNumOfAxes() const { return m_nNumOfAxes; }
	void SetNumOfAxes(int val) { m_nNumOfAxes = val; }
	int GetNumOfBoard() const { return m_nNumOfBoard; }
	void SetNumOfBoard(int val) { m_nNumOfBoard = val; }

private:
	int m_nNumOfBoard;
	int m_nNumOfAxes;
	CMotionConfig* m_pMotionConfig;			// Axis의 정보를 가지고 있고, BbxmnpWnd에 정의된 enum과 Matching됨
	CMotionConfig* m_pAbnormal;				// File load 실패시에 사용할 Empty Class
};

