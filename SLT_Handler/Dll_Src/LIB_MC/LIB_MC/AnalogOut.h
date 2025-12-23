#pragma once

class CAnalogOut
{
public:
	CAnalogOut(void);
	~CAnalogOut(void);

	virtual void WriteAll(std::vector<double>& vBuff)=0;
};

