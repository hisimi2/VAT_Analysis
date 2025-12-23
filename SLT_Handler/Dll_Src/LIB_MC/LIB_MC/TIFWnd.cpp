#include "StdAfx.h"
#include "TIFWnd.h"


CTIFWnd::CTIFWnd(void)
{
}


CTIFWnd::~CTIFWnd(void)
{
}

BEGIN_MESSAGE_MAP(CTIFWnd, CWnd)
	//{{AFX_MSG_MAP(CTIFWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP	
	ON_MESSAGE(UM_GPIB_FULLSITE,    OnRcvGpibAdvan_FULLSITE)
	ON_MESSAGE(UM_GPIB_FULLSITE_OK, OnRcvGpibAdvan_FULLSITE_OK)
	ON_MESSAGE(UM_GPIB_BINON,       OnRcvGpibAdvan_BINON)
	ON_MESSAGE(UM_GPIB_ECHO,        OnRcvGpibAdvan_ECHO)
	ON_MESSAGE(UM_GPIB_ECHO_OK,     OnRcvGpibAdvan_ECHOOK)
	ON_MESSAGE(UM_GPIB_ECHO_NG,     OnRcvGpibAdvan_ECHONG)
	ON_MESSAGE(UM_GPIB_TMP,         OnRcvGpibAdvan_TMP)
	ON_MESSAGE(UM_GPIB_MAP,         OnRcvGpibAdvan_MAP)
	ON_MESSAGE(UM_GPIB_SOAK,        OnRcvGpibAdvan_SOAK)
	ON_MESSAGE(UM_GPIB_FR,          OnRcvGpibAdvan_FR)
	ON_MESSAGE(UM_GPIB_IDN,         OnRcvGpibAdvan_IDN)
	ON_MESSAGE(UM_GPIB_NAME,        OnRcvGpibAdvan_NAME)	
END_MESSAGE_MAP()