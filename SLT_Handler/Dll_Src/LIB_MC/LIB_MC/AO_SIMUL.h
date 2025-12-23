#pragma once

#include <vector>

class CAnalogOut;

class CAO_SIMUL : public CAnalogOut
{
public:
	CAO_SIMUL(void);
	~CAO_SIMUL(void);
	void WriteAll(std::vector<double>& vBuff);
};

