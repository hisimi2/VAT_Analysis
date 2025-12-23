#pragma once

#include <vector>


//==============================================================================
//
//==============================================================================
class CAnalogIn
{
public:
	CAnalogIn(void);
	~CAnalogIn(void);

	virtual void ReadAll(std::vector<double>& vBuffer) = 0;
};

