#pragma once

//==============================================================================
//
//==============================================================================
#ifndef __AIO_TYPE__
	#define __AIO_TYPE__
	#define _AIO_TYPE_SIMUL_		(0)
	#define _AIO_TYPE_SYCON_		(1)
#endif // __AIO_TYPE__

//==============================================================================
//
//==============================================================================
class AFX_EXT_CLASS CAIO_Cont
{
public:
	CAIO_Cont(void);
	~CAIO_Cont(void);

	int		Initialize(const char* szDriverInitFileName, int nFileNameLen);
	void	Finalize();
	BOOL	IsInitBoard();

	void	out(int nIndex, double dValue);
	double  out(int nIndex);
	void	RESET_AO();

	double  in(int nIndex);
	
	int     GetPhysicalValue(int nIoType, int nIndex, double* pdValue, CString& strUnit);
	int     SetPhysicalValue(int nIndex, double dValue);

	int     GetAiCount();
	int     GetAoCount();

	void    ShowDialog_IO_All();
	void    ShowDialogSimul();
};

