#pragma once

#include <random>

class VATSimul
{
public:
	VATSimul();
	~VATSimul();

public:
	BOOL m_bIsSimul;
	int m_nActionCnt;

	BOOL _IsSimul() { return m_bIsSimul; }

	void _WaitSeqControl(int wait_time);
	BOOL _IsRemainSeqCycle();
	void _ClearParam();

public:
	void _SetVisionRcvData(int nCam, int nStatus);

private:
	//Random offset
	std::mt19937 offset;
	std::uniform_real_distribution<> dist;
};

