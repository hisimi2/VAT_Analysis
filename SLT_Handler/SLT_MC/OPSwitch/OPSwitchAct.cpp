// OPSwitchAct.cpp : implementation file
//

#include "stdafx.h"
#include "OPSwitchAct.h"
#include "TimeCheck.h"
#include <assert.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



// OPSwitchAct


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-16]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
COPSwitchAct::COPSwitchAct()
{
	Init();
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-16]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
COPSwitchAct::COPSwitchAct(CString strName, int nSw_IO_Addr, int nLed_IO_Addr, eOP_OUT_ATTRIB eOut_Attrib, BOOL bBlink, int nLedType/*=0*/)
{
	Init();

	m_bBlinkFlag = bBlink;
	m_eOut_Attrib = eOut_Attrib;
	m_nOut_Addr = nLed_IO_Addr;
	m_nIn_Addr = nSw_IO_Addr;
	m_strName = strName;
	m_nLedType = nLedType;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-16]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void COPSwitchAct::OPSwitchSet(CString strName, int nSw_IO_Addr, int nLed_IO_Addr, eOP_OUT_ATTRIB eOut_Attrib, BOOL bBlink, int nLedType/*=0*/)
{
	Init();
	m_bBlinkFlag = bBlink;
	m_eOut_Attrib = eOut_Attrib;
	m_nOut_Addr = nLed_IO_Addr;
	m_nIn_Addr = nSw_IO_Addr;
	m_strName = strName;
	m_nLedType = nLedType;
	if( m_nLedType == LED_B ){
		OutPutLed(DEF_OFF);
	}
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-16]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void COPSwitchAct::Init()
{
	m_bTemp_Memo_Flag = FALSE;
	m_bLockMode = FALSE;
	m_bDF_ON = FALSE;
	m_bDF_OFF = FALSE;
	m_bDF_DOWN = FALSE;
	m_bDF_UP = FALSE;
	m_nAddedNumI = 0;
	m_nAddedNumO = 0;
	m_bBlinkFlag = FALSE;
	m_nRunExitFlag = 0;
	m_bOnOffFlag = FALSE;
	m_bStatus = FALSE;
	m_pcGroup = NULL;
	m_eOut_Attrib = eOP_NORMAL;
	m_nOut_Addr = 0;
	m_nIn_Addr = 0;
	ZeroMemory(m_anAdded_In_IO, sizeof(m_anAdded_In_IO));
	ZeroMemory(m_anAdded_Out_IO, sizeof(m_anAdded_Out_IO));
	m_ctBlink.SetCheckTime(0.5);
	m_ctBlink.StartTimer();
	m_nOperationStep = 0;
	m_strName = _T("");
	m_bTemp_key = FALSE;

	m_nLedType = 0;
	m_bSWStatus_pre = FALSE;
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-09-16]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
COPSwitchAct::~COPSwitchAct()
{
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-16]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void COPSwitchAct::SetGroupSw(COPSwitchAct *lpOpSwitch)
{
	m_pcGroup = lpOpSwitch;	
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-16]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
BOOL COPSwitchAct::GetStatus()
{
	return m_bStatus;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-16]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void COPSwitchAct::SetStatus(BOOL bStat)
{
	if(m_bLockMode){	//Lock되어 있으면 아무것도 하지 않는다.
		return;
	}

	//프로 그램에서 강제로 셋팅될때는 실제동작 변수는 클리어 한다.
	//즉 더이상 사용 할수 없다.
	m_bDF_UP = FALSE;
	m_bDF_DOWN = FALSE;

	if(bStat && !m_bStatus)	//처음  DF_ON
	{
		m_bTemp_Memo_Flag = TRUE;
		m_bDF_OFF = FALSE;
		m_bDF_ON = TRUE;
		m_bStatus = bStat;
		OutPutLed(m_bStatus);
	}
	else if(!bStat && m_bStatus)	//죽을때 한번 DF_OFF
	{
		m_bTemp_Memo_Flag = FALSE;
		m_bDF_ON = FALSE;
		m_bDF_OFF = TRUE;
		m_bStatus = bStat;
		OutPutLed(m_bStatus);
	}
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-16]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 모든 등록된 스위치들의 체크 기능은 OR 모드이다.
==============================================================================*/
BOOL COPSwitchAct::GetSWStatus()
{
	int res=0;
	//////// 입력 번호가 혹시 음수이면 입력이 없는 단순 램프만 제어하는 경우이다.
	//////// 이경우에는 반드시 외부에서 SetStatus(TRUE) 함수를 제어하여 주어야 램프가 동작한다.
	if( m_nIn_Addr < 0 ){
		return FALSE;
	}
	
	if(m_nIn_Addr == DI_START_SW_SEN2 /*&& g_IO.in(DI_SELECT_FRONT_SW_SEN) == DEF_ON*/ ){
		res = !g_IO.in(m_nIn_Addr); // START Switch 한쪽 방향만 사용 한다.	
		res += g_IO.in(DI_START_SW_SEN1);
	}else if(m_nIn_Addr == DI_COVER_SW_SEN /*&& g_IO.in(DI_SELECT_FRONT_SW_SEN) == DEF_ON*/ ){
		res = g_IO.in(m_nIn_Addr); // COVER Switch 한쪽 방향만 사용 한다.
	}else if(m_nIn_Addr != DI_START_SW_SEN2 && m_nIn_Addr != DI_COVER_SW_SEN ){
		res = g_IO.in(m_nIn_Addr);
	}
	BOOL bRearRightSW = g_DMCont.m_dmEQP.GB(BDM0_MD_REAR_RIGHT_SWITCH);

	if(m_nAddedNumI){			//같은 기능의 모든 스위치들을의 동작 상태도 검사 한다.
		for(int i=0; i<m_nAddedNumI; i++){
			if(m_anAdded_In_IO[i] == DI_REAR_LEFT_START_SW_SEN2 /*&& g_IO.in(DI_SELECT_REAR_SW_SEN) == DEF_ON*/ ){
				res += !g_IO.in(m_anAdded_In_IO[i]);
				res += g_IO.in(DI_REAR_LEFT_START_SW_SEN1);
			}else if(m_anAdded_In_IO[i] == DI_REAR_LEFT_COVER_SW_SEN /*&& g_IO.in(DI_SELECT_REAR_SW_SEN) == DEF_ON*/ ){
				res += g_IO.in(m_anAdded_In_IO[i]);
			}
			else if (m_anAdded_In_IO[i] == DI_REAR_LEFT_LAMP_SW_SEN) {
				res += g_IO.in(m_anAdded_In_IO[i]);
			}
			else if (m_anAdded_In_IO[i] == DI_REAR_LEFT_SOUND_SW_SEN) {
				res += g_IO.in(m_anAdded_In_IO[i]);
			}
			else if(bRearRightSW == TRUE && m_anAdded_In_IO[i] == DI_REAR_RIGHT_START_SW_SEN2 /*&& g_IO.in(DI_SELECT_REAR_SW_SEN) == DEF_ON*/){
				res += !g_IO.in(m_anAdded_In_IO[i]);
				res += g_IO.in(DI_REAR_RIGHT_START_SW_SEN1);
			}else if(bRearRightSW == TRUE && m_anAdded_In_IO[i] == DI_REAR_RIGHT_COVER_SW_SEN /*&& g_IO.in(DI_SELECT_REAR_SW_SEN) == DEF_ON*/){
				res += g_IO.in(m_anAdded_In_IO[i]);
			}
			else if (bRearRightSW == TRUE && m_anAdded_In_IO[i] == DI_REAR_RIGHT_LAMP_SW_SEN) {
				res += g_IO.in(m_anAdded_In_IO[i]);
			}
			else if (bRearRightSW == TRUE && m_anAdded_In_IO[i] == DI_REAR_RIGHT_SOUND_SW_SEN) {
				res += g_IO.in(m_anAdded_In_IO[i]);
			}
			
			
			
			else if(m_anAdded_In_IO[i] != DI_REAR_LEFT_START_SW_SEN2 && m_anAdded_In_IO[i] != DI_REAR_LEFT_COVER_SW_SEN 
				     && m_anAdded_In_IO[i] != DI_REAR_LEFT_LAMP_SW_SEN && m_anAdded_In_IO[i] != DI_REAR_LEFT_SOUND_SW_SEN
					&& m_anAdded_In_IO[i] != DI_REAR_RIGHT_LAMP_SW_SEN && m_anAdded_In_IO[i] != DI_REAR_RIGHT_SOUND_SW_SEN
				     && (m_anAdded_In_IO[i] != DI_REAR_RIGHT_START_SW_SEN2 && m_anAdded_In_IO[i] != DI_REAR_RIGHT_COVER_SW_SEN)){
			    res += g_IO.in(m_anAdded_In_IO[i]);
			}			
		}
	}
	
	if( m_bSWStatus_pre == FALSE && res )
	{
		m_bDF_UP   = TRUE;
		m_bDF_DOWN = FALSE;	    //본 변수는 실제의 IO action 이 발생할때만 셋팅된다.
	}
	if( m_bSWStatus_pre == TRUE && !res )
	{
		m_bDF_UP   = FALSE;
		m_bDF_DOWN = TRUE;
	}
	m_bSWStatus_pre = res;

//	if(res && !m_bStatus)	//처음 눌러지면 DF_UP
//	{
//		m_bDF_UP   = TRUE;
//		m_bDF_DOWN = FALSE;	    //본 변수는 실제의 IO action 이 발생할때만 셋팅된다.
//	}
//	else if(!res && m_bStatus)	//눌렀다가 뗄때 한번 DF_DOWN
//	{
//		m_bDF_UP   = FALSE;
//		m_bDF_DOWN = TRUE;
//	}
	if(res){ 
		return TRUE;
	}
	return FALSE;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-16]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 프로그램을 스레드화하였다.
==============================================================================*/
int COPSwitchAct::Run()
{	
	m_nRunExitFlag = 0;
	AfxBeginThread(OpSwitchThreadProc, this);
	return TRUE;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-16]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int COPSwitchAct::Stop()
{
	m_nRunExitFlag = 1;
	while(1)
	{
		if(m_nRunExitFlag == 2){
			break;
		}
		Sleep(10);
	}
	return TRUE;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-16]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : Rear 판넬 스위치같은 경우 전면조작 판과 같은 기능이 대부분이므로 이를 등록하여 
             Rear 판넬의 스위치들은 따로 등록 하지 않고 동작시키기 위한 부분의 입력번호 등록 부분임.
==============================================================================*/
void COPSwitchAct::Add_In(int Add_IO_num)
{
	m_anAdded_In_IO[m_nAddedNumI++] = Add_IO_num;
	ASSERT(m_nAddedNumI < MAX_ADD_NUM);
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-16]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : Rear 판넬 스위치같은 경우 전면조작 판과 같은 기능이 대부분이므로 이를 등록하여 
             Rear 판넬의 스위치들은 따로 등록 하지 않고 동작시키기 위한 부분의 출력번호 등록 부분임.
==============================================================================*/
void COPSwitchAct::Add_Out(int Add_IO_num)
{
	m_anAdded_Out_IO[m_nAddedNumO++] = Add_IO_num;
	ASSERT(m_nAddedNumO < MAX_ADD_NUM);
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-16]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void COPSwitchAct::OutPutLed(int nOn_Off)
{
	if( m_nOut_Addr < 0 ){
		return;
	}
	if( m_nLedType == LED_B ){
		nOn_Off = (nOn_Off==1)?0:1;
	}
	g_IO.out(m_nOut_Addr, nOn_Off);
	if(m_nAddedNumO > 0){			//같은 기능의 모든 램프들을 동작한다.
		for(int i=0; i<m_nAddedNumO; i++){
			g_IO.out(m_anAdded_Out_IO[i], nOn_Off);
		}
	}
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-16]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
BOOL COPSwitchAct::GetBlink()
{
	return m_bBlinkFlag;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-16]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void COPSwitchAct::SetBlink(BOOL bMode)
{
	m_bBlinkFlag = bMode;
}


void COPSwitchAct::Switch_Operation()
{
	switch( m_nOperationStep )
	{
	case 0:
		{
			m_bTemp_key = FALSE;
			m_bTemp_Memo_Flag = FALSE;
			m_nOperationStep = 1000;
		}break;

	case 1000:
		{
			if( !m_bLockMode )	//Lock되어 있으면 아무것도 하지 않는다.
			{
				if(eOP_NORMAL == m_eOut_Attrib)
				{
					if(GetSWStatus())
					{
						SetStatus(TRUE);
						if(!m_bBlinkFlag){
							OutPutLed(1);
						}
					}
					else
					{
						SetStatus(FALSE);
						if(!m_bBlinkFlag){
							OutPutLed(0);
						}
					}
				}

				//본속성에서는 반드시 다른 객체의 스위치가 제공 되어야 off 할수 있다.
				//스위치를 누를 때마다 자신의 램프는 on 하고 남의 램프를 off 하기 때문임...
				//예).... 운전/정지 등 쌍으로 이루어 지는 스위치류.
				else if( eOP_KEEP == m_eOut_Attrib )
				{
					if(GetSWStatus())
					{
						SetStatus(TRUE);
						if(!m_bBlinkFlag){
							OutPutLed(1);
						}
						ASSERT(m_pcGroup);
						if(m_pcGroup)	{	//본키가 눌렸을때 꺼져야할 스위치의 포인터
							m_pcGroup->SetStatus(FALSE);
						}
					}
				}
				else if(eOP_TOGGLE==m_eOut_Attrib)
				{
					if(GetSWStatus()&&m_bTemp_key==FALSE)
					{
						m_bTemp_key = TRUE;	//한번만 작업시키기 위해서.....
						if(GetStatus()){
							m_bTemp_Memo_Flag = TRUE;	//이미 온 되어 있으면 바로 꺼야 한다.
						}
						if(m_bTemp_Memo_Flag==FALSE)
						{
							m_bTemp_Memo_Flag = TRUE;
							SetStatus(TRUE);
							if(!m_bBlinkFlag){
								OutPutLed(1);
							}
						}
						else
						{
							m_bTemp_Memo_Flag = FALSE;
							SetStatus(FALSE);
							if(!m_bBlinkFlag){
								OutPutLed(0);
							}
						}
					}
					else if(!GetSWStatus())
					{
						m_bTemp_key = FALSE;
					}
				}
			}
			// 램프를 깜빡인다.
			if(m_bBlinkFlag && GetStatus())
			{
				if(m_ctBlink.CheckOverTime())
				{
					m_ctBlink.StartTimer();
					if(!m_bOnOffFlag)
					{
						m_bOnOffFlag = TRUE;
						OutPutLed(1);
					}
					else
					{
						m_bOnOffFlag = FALSE;
						OutPutLed(0);
					}
				}
			}
			//깜박이가 아닐때에 On 되면 무조건 불을 켜야한다.
			if(!m_bBlinkFlag && GetStatus())
			{
				OutPutLed(GetStatus());
			}
		}break;        
	}// end of switch
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-16]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
UINT COPSwitchAct::OpSwitchThreadProc(LPVOID lpVoid)
{
	//모체가 되는 OPSwitchAct 포인터를 인계 받아야 접근이 가능하다.
	COPSwitchAct* pSelf = (COPSwitchAct *)lpVoid;
	while( pSelf->m_nRunExitFlag == 0 ){
		pSelf->Switch_Operation();
		Sleep(10);	//1회전후 스레드를 양보 한다.
	}
	pSelf->m_nRunExitFlag = 2;
	return 0;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-16]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 스위치를 누를때 한번만 On 한다. 본명령 실행후에는 리셋되므로 
             다른곳에서 (두번이상) 이명령을 쓰면 두번째의 것은 무효화된다.
==============================================================================*/
BOOL COPSwitchAct::GetDF_Up()
{
	if(m_bDF_UP)
	{
		m_bDF_UP = FALSE;
		return TRUE;
	}
	return FALSE;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-16]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 스위치를 뗄때 한번만 On 한다. 본명령 실행후에는 리셋되므로 
             다른곳에서 (두번이상) 이명령을 쓰면 두번째의 것은 무효화된다.
==============================================================================*/
BOOL COPSwitchAct::GetDF_Down()
{
	if(m_bDF_DOWN)
	{
		m_bDF_DOWN = FALSE;
		return TRUE;
	}
	return FALSE;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-16]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 스위치의 ON 이벤트
==============================================================================*/
BOOL COPSwitchAct::GetDF_On()
{
	if(m_bDF_ON)
	{
		m_bDF_ON = FALSE;
		return TRUE;
	}
	return FALSE;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-16]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 스위치의 OFF 이벤트
==============================================================================*/
BOOL COPSwitchAct::GetDF_Off()
{
	if(m_bDF_OFF)
	{
		m_bDF_OFF = FALSE;
		return TRUE;
	}
	return FALSE;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-16]
  NAME     : 
  PARAMS   : bMode : 0 = Unlock, 1 = Lock
  RETURN   : 
  COMMENTS : 버튼을 누르더라도 동작되지 않토록 버튼을 잠금.
==============================================================================*/
void COPSwitchAct::SetLockMode(BOOL bMode)
{
	m_bLockMode = bMode;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-16]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 버튼 락 상태 반환.
==============================================================================*/
BOOL COPSwitchAct::GetLockMode()
{
	return m_bLockMode;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-11-02]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CString COPSwitchAct::GetName()
{
	return m_strName;
}
void COPSwitchAct::SwitchLEDOnOff(BOOL OnOff)
{
	OutPutLed(OnOff);
}
