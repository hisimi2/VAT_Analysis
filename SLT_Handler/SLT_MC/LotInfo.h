#pragma once

class CLotInfo
{
public:
	CLotInfo(void);
	CLotInfo(const CLotInfo& src);
	~CLotInfo(void);

	CLotInfo& operator=(const CLotInfo& src);

	CString strLotNo;
	CString strScheduleNo;
	CString strOperID;
	CString strDeviceName;
	CString strProgName;
	CString strCustomer;
	int     nLotStartMode;		// enum eStartMode
	int     nRCMode;			// enum eLOT_START_MODE
	int     nVisionUse;			// 0:no use, 1:use
	int     nLotState;
};

