#pragma once

#include "DEF_LIB_TESTER_IF.h"

class AFX_EXT_CLASS CTIFWnd : public CWnd
{
public:
	CTIFWnd(void);
	~CTIFWnd(void);

protected:
	virtual afx_msg LRESULT OnRcvGpibAdvan_FULLSITE(WPARAM wParam, LPARAM lParam)=0;
	virtual afx_msg LRESULT OnRcvGpibAdvan_FULLSITE_OK(WPARAM wParam, LPARAM lParam)=0;
	virtual afx_msg LRESULT OnRcvGpibAdvan_BINON(WPARAM wParam, LPARAM lParam)=0;
	virtual afx_msg LRESULT OnRcvGpibAdvan_ECHO(WPARAM wParam, LPARAM lParam)=0;
	virtual afx_msg LRESULT OnRcvGpibAdvan_TMP(WPARAM wParam, LPARAM lParam)=0;
	virtual afx_msg LRESULT OnRcvGpibAdvan_MAP(WPARAM wParam, LPARAM lParam)=0;
	virtual afx_msg LRESULT OnRcvGpibAdvan_SOAK(WPARAM wParam, LPARAM lParam)=0;
	virtual afx_msg LRESULT OnRcvGpibAdvan_FR(WPARAM wParam, LPARAM lParam)=0;
	virtual afx_msg LRESULT OnRcvGpibAdvan_IDN(WPARAM wParam, LPARAM lParam)=0;
	virtual afx_msg LRESULT OnRcvGpibAdvan_NAME(WPARAM wParam, LPARAM lParam)=0;
	virtual afx_msg LRESULT OnRcvGpibAdvan_ECHOOK(WPARAM wParam, LPARAM lParam)=0;
	virtual afx_msg LRESULT OnRcvGpibAdvan_ECHONG(WPARAM wParam, LPARAM lParam)=0;

	DECLARE_MESSAGE_MAP()
};