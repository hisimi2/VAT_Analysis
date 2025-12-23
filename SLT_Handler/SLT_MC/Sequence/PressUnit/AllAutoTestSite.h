#pragma once

class CAllAutoTestSite
{
public :
	CAllAutoTestSite(void);
	~CAllAutoTestSite(void);

	int GetSeqCmdStatusAllPart();
	int OnLotCommand(eAutoLotCmd eLotCmd, WPARAM wParam, LPARAM lParam);
};