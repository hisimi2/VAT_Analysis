#pragma once
#include <afxwin.h>
#include <afxmt.h>
#include <WinError.h>

#include "DEF_LIB_TESTER_IF.h"
#include "Logger.h"
#include "ParamList.h"
#include "ConfigData.h"

// 인터페이스 정의
class CIFComm : public CWnd
{
public: //
	virtual int		Init_Board() = 0;
	virtual void	Final_Board() = 0;

	virtual void 	GetBoardRcvCMD() = 0;
	virtual int		do_SendCMD(CTesterIFCmd& Packet) = 0;

	virtual int		SRQLineCheck() = 0;

////////////////////////////////////////////////////////////////////////////////////////////

	CIFComm(void);
	virtual ~CIFComm(void);

	int				Initialize(CWnd* pParentWnd, HWND hClientWnd, CWnd* pClientWnd, ST_COMM_DATA stCommData, ST_TESTER_MESSAGE_MAP* pMessageMap );
	void			Finalize();

	bool			IsConnected();

	int				GetRcvCMDCount();					// Count
	void			AddRcvCMD(CTesterIFCmd* pPacket);	// Add
	CTesterIFCmd*	GetRcvCMD();						// Recieve
	void			DelRcvCMD();						// Del
	void			ClearRcvCMD();						// Clear
	void			PopRcvCMD();						// Pop

	void			AddSndCMD_Fast(CTesterIFCmd* pPacket);
	void			AddSndCMD(CTesterIFCmd* pPacket);
	int				GetSndCMDCount();
	CTesterIFCmd*	GetSndCMD();
	void			DelSndCMD();
	void			ClearSndCMD();
	void			PopSndCMD();

	int				SendToClient(CTesterIFCmd* pPacket);

	CTesterIFCmd*	m_fnMakeFullsite(CParamList& cParamList);
	CTesterIFCmd*	m_fnMakeEcho(CParamList& cParamList);

private:
	static UINT		m_fnRcvPacketProc(void* lpVoid);
	static UINT		m_fnSndPacketProc(void* lpVoid);

protected:
	char			m_szInitFileName[MAX_PATH];

	bool			m_bConnected;
	bool			m_bStopRcvCmdProc;
	bool			m_bTerminatedRcvCmdProc;
	bool			m_bStopSndCmdProc;
	bool			m_bTerminatedSndCmdProc;

	int				m_IF_Type;
	int				m_nTIF_StdPara;

	CWnd*           m_pClientWnd;
	CWnd*			m_pParentWnd;

	HWND			m_hClientWnd;

	CEvent			m_evtThreadTerminated_Rcv;
	CEvent			m_evtThreadTerminated_Snd;

	// Receive Packet Buffer
	CRITICAL_SECTION*				m_pCSRcv;
	CWinThread*						m_pThreadRcv;
	CList<CTesterIFCmd*, CTesterIFCmd*>	m_QRcv;

	// Send Packet Buffer
	CRITICAL_SECTION*				m_pCSSnd;
	CWinThread*						m_pThreadSnd;
	CList<CTesterIFCmd*, CTesterIFCmd*>		m_QSnd;

	CLogger							m_Logger;

	ST_COMM_DATA	m_stCommData;

	ST_TESTER_MESSAGE_MAP	m_MessageMap[1000];
};

