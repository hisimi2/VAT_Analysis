#pragma once

//==============================================================================
//
//==============================================================================
#ifndef __DIO_TYPE__
#define __DIO_TYPE__
	#define _DIO_TYPE_SIMUL_		(0)
	#define _DIO_TYPE_SYCON_		(1)
	#define _DIO_TYPE_TECHWING_		(2)
	#define _DIO_TYPE_CONTEC_		(3)
#endif // __DIO_TYPE__

#ifndef UM_DIO_SIM_DO_CHANGED
	#define UM_DIO_SIM_DO_CHANGED       (WM_USER + 1000)
#endif

//==============================================================================
//
//==============================================================================
class AFX_EXT_CLASS CDI_Cont
{
public:
	BYTE operator[]( int nIndex );
	BYTE operator()( int nIndex );
	
};

class AFX_EXT_CLASS CDO_Cont
{
public:
	BYTE operator()( int nIndex ) const;
	void operator()( int nIndex, int nOnOff );
};

//==============================================================================
//
//==============================================================================
class AFX_EXT_CLASS CDIO_Cont
{
public:
	CDIO_Cont(void);
	~CDIO_Cont(void);	

	int		Create(const char* szDriverInitFileName, int nFileNameLen);
	void	Destroy();
	BOOL	IsInitBoard();

	void	_out(int nIndex, char cValue);
	char    _out(int nIndex);
	void	RESET_DO();

	char	_in(int nIndex);

	int     GetDiCount();
	int     GetDoCount();

	void    ShowDialog_IO_All();
	void    ShowDialogSimul();

	void    SetSimulDi(int nIndex, BOOL bValue);
	void    SetSimulWnd(CWnd* pSimWnd);
	int     GetDriverType();

	CDI_Cont in;
	CDO_Cont out;
};

/*
#define DO_LD_PUSHER   (0)

#define DI_LD_PUSHER   (0)

g_IO.out(DO_LD_PUSHER, DIO::ON);
BOOL bSensor = g_IO.in(DI_LD_PUSHER);
*/
