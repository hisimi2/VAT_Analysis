#if !defined(AFX_OPSWITCHACT_H__D5AFF422_1F1C_11D3_809D_00AA00A18C69__INCLUDED_)
#define AFX_OPSWITCHACT_H__D5AFF422_1F1C_11D3_809D_00AA00A18C69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OPSwitchAct.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// OPSwitchAct window
/*
* Normal : 스위치 입력이 들어오면 출력을 내보낸다. 스위치를 떼면 다시 off된다.
* Keep   : start, stop 스위치와 같이 두개의 스위치가 쌍을 이루는 타입
* Toggle : 스위치 하나가 한번 누르면 on이 되고 다시 한번 더 누르면 off 되는 타입
*/
enum eOP_OUT_ATTRIB{
	eOP_NORMAL=0, //
	eOP_KEEP,     // 다른 Switch와 쌍을 이루는 타입.
	eOP_TOGGLE,   // 자기유지 하는 타입.
};

#define MAX_ADD_NUM			(10)

/*
* 스위치 입력에 따라 출력을 내보낸다. 
*/
class COPSwitchAct
{
public:
	enum _eLedType{
		LED_A = 0, LED_B,
	};

public:
	COPSwitchAct();
	COPSwitchAct(CString strName, int nSw_IO_Addr, int nLed_IO_Addr, eOP_OUT_ATTRIB eOut_Attrib, BOOL bBlink=FALSE, int nLedType=0);
	~COPSwitchAct();

	virtual void OPSwitchSet(CString strName, int nSw_IO_Addr, int nLed_IO_Addr, eOP_OUT_ATTRIB eOut_Attrib, BOOL bBlink=FALSE, int nLedType=0);

public:
	BOOL GetLockMode();
	void SetLockMode(BOOL nMode);
	BOOL GetDF_On();
	BOOL GetDF_Off();
	BOOL GetDF_Down();
	BOOL GetDF_Up();
	void Add_In(int nAdd_IO_num);
	void Add_Out(int nAdd_IO_num);
	int  Stop();
	int  Run();
	BOOL GetBlink();
	void SetBlink(BOOL bMode);
	BOOL GetSWStatus();
	void SetStatus(BOOL bStat);
	BOOL GetStatus();
	void SetGroupSw(COPSwitchAct * lpOpSwitch);
	void SwitchLEDOnOff(BOOL Onoff);

	virtual void Switch_Operation();
	CString GetName();

	int m_nOperationStep;
	CTimerTickCheck m_ctBlink;
	BOOL m_bTemp_key;



protected:
	static UINT OpSwitchThreadProc(LPVOID lpVoid);
	void OutPutLed(int nOn_Off);
	void Init();

	int            m_nLedType;
	BOOL           m_bTemp_Memo_Flag;
	BOOL           m_bLockMode;
	BOOL           m_bDF_ON;
	BOOL           m_bDF_OFF;
	BOOL           m_bDF_DOWN;
	BOOL           m_bDF_UP;
	int            m_nAddedNumI;
	int            m_anAdded_In_IO[MAX_ADD_NUM];
	int            m_nAddedNumO;
	int            m_anAdded_Out_IO[MAX_ADD_NUM];
	BOOL           m_bBlinkFlag;
	int            m_nRunExitFlag;
	BOOL           m_bOnOffFlag;
	BOOL           m_bStatus;
	BOOL           m_bSWStatus_pre;
	int            m_nOut_Addr;
	int            m_nIn_Addr;
	COPSwitchAct * m_pcGroup;
	eOP_OUT_ATTRIB m_eOut_Attrib;
	CString        m_strName;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPSWITCHACT_H__D5AFF422_1F1C_11D3_809D_00AA00A18C69__INCLUDED_)
