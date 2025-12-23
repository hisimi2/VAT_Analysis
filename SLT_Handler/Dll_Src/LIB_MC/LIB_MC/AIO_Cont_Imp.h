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
class CAnalogIn;
class CAnalogOut;
class CDlgDioAll;


//==============================================================================
//
//==============================================================================
class CAIO_Cont_Imp
{
public:
	CAIO_Cont_Imp(void);
	~CAIO_Cont_Imp(void);

	int		Initialize(const char* szDriverInitFileName, int nFileNameLen);
	void	Finalize();
	BOOL	IsInitBoard()				{ return m_bBoardInit; };

	void	out(int nIndex, double dValue);
	double  out(int nIndex);

	double  in(int nIndex);

	int     GetAiCount();
	int     GetAoCount();

	void    ShowDialog_IO_All();
	void    ShowDialogSimul();

	void	Read_AI_All();
	void	Write_AO_All();

	int GetIoDbInfo(int nIoType, int nIoIndex, _tAIO_LS* lpstIoInfo);
	int GetPhysicalValue(int nIoType, int nIndex, double* pdValue, CString& strUnit);
	int SetPhysicalValue(int nIndex, double dValue);

private:
	std::vector<double>	m_vAI;
	std::vector<double>	m_vAO;

	int             m_nAI_PointMax;
	int             m_nAO_PointMax;

	BOOL			m_bBoardInit;
	CAnalogIn*   	m_pAI;
	CAnalogOut*   	m_pAO;
	char			m_szInitFileName[512];

	// Logical Sorted
	std::vector<_tAIO_LS*> m_vAnalogInInfo;     
	std::vector<_tAIO_LS*> m_vAnalogOutInfo;

	CMyList m_AiDbInfoList;
	CMyList m_AoDbInfoList;
	CEvent m_evtThreadTerminated;
	CDlgDioAll* m_pDlgIoAll;

	// Physical Sorted

};

