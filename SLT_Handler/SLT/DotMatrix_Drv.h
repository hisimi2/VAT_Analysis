#pragma once
#include "vector"
#include "DotMatrix_Comm.h"

#define DOT_STRING_L	(76)
#define DOT_STRING_E1	(33)
#define DOT_STRING_E2	(34)
#define DOT_STRING_START_A (65)
#define DOT_NUMBER_START_1 (48)
#define DOT_STRING_START_X  (45)

namespace DOT_SETTING
{
	enum DOT_COLOR
	{
		eDOT_COLOR_NONE = 0,
		eDOT_COLOR_GREEN,
		eDOT_COLOR_RED,
		eDOT_COLOR_ORANGE,
	};

	enum DOT_ROTATE
	{
		eDOT_ROTATE_0 = 0,
		eDOT_ROTATE_90,
		eDOT_ROTATE_180,
		eDOT_ROTATE_270,
	};
}


typedef struct stDotMatrix
{
	int nAddress;
	DOT_SETTING::DOT_COLOR DotColor;
	std::vector<int> vData;
}stDotMatrix;

class CDotMatrix_Drv
{
public:
	CDotMatrix_Drv();
	~CDotMatrix_Drv();
	int Initialize(const char * szConfigFileName);
	int	ComOpen();
	int ComClose();
	int SendData(stDotMatrix stData);
private:
	CDotMatrix_Comm* m_Comm;
};

