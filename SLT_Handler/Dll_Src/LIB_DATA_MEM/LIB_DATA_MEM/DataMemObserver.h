#pragma once

#include "afx.h"

class CDataMem;

class CDataMemObserver : public CObject
{
public:
	CDataMemObserver() {
		m_pDataMem = NULL; m_nType = 0; m_nOffset = 0;
	};
	CDataMemObserver(CDataMem* pDM, int nType, int nOffset) : m_pDataMem(pDM), m_nType(nType), m_nOffset(nOffset){};
	~CDataMemObserver(){};

	virtual void OnUpdate(WPARAM wParam, LPARAM lParam)=0;

	int			SetRamData(CDataMem* pRamData);
	CDataMem*	GetRamData();

	int GetDataType();
	int SetDataType(int nType);

	int GetDataOffset();
	int SetDataOffset(int noffset);  

private:
	CDataMem* 	m_pDataMem;
	int			m_nType;
	int  		m_nOffset;
};

