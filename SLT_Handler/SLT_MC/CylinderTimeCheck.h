#pragma once
class CCylinderTimeCheck : public CWnd
{
public:
	CCylinderTimeCheck();
	~CCylinderTimeCheck();

	void initialize();
	
	_tCYLINDER_TIME m_stCylinderTime[MAX_CYLINDER_COUNT];
};

