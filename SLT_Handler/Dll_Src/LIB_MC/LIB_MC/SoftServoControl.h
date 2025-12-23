#pragma once
// #define _DIO_TYPE_SIMUL_		(0)
// #define _DIO_TYPE_SYCON_		(1)
// #define _DIO_TYPE_TECHWING_		(2)
// #define _DIO_TYPE_CONTEC_		(3)
// #define _DIO_TYPE_MMC_ECAT_	    (4)
// #define _DIO_TYPE_PORT_			(5)
// #define _DIO_TYPE_MODE_BUS_		(6)
//  #define _DIO_TYPE_SOFTSERVO		(7)

class AFX_EXT_CLASS  SoftServoControl
{
public:
	SoftServoControl();
	~SoftServoControl();

	int CreateDev(TCHAR* szIniFileName, int nFileLen);
	int StartCommunication();
	void DestroyEtherCat(void);
	void CloseDevice();
	TCHAR			m_szInitFileName[512];
};

