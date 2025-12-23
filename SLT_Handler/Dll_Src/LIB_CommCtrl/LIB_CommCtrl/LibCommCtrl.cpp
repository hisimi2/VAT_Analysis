#include "StdAfx.h"
#include "LibCommCtrl.h"

#include "KeyPadDlg.h"
#include "NumberBox.h"
#include "LogInUserDlg.h"
#include "RegisterUserDlg.h"
#include "ShowMessageBox.h"
#include "LibCommCtrlDrv.h"
#include "CalenderDlg.h"
#include "ProgressDlg.h"
#include "UserConfigDlg.h"

CLibCommCtrlDrv g_LibCommCtrlDrv;
CProgressDlg	g_Progress;
CProgressDlg*	g_pProgress;

CLibCommCtrl::CLibCommCtrl(void)
{
}


CLibCommCtrl::~CLibCommCtrl(void)
{
}

int  CLibCommCtrl::Initialize(CWnd* pParentWnd)
{
	return 0;
}

void CLibCommCtrl::Close()
{

}



BOOL CLibCommCtrl::GetValueBox(char* szVal, int maxchar, char* getval, char* title, BOOL password)
{
	CKeyPadDlg dlg(maxchar, getval, title, password, NULL);

	if( dlg.DoModal() == IDOK )
	{
		lstrcpy(szVal, dlg.m_strGetVal);
		return RETURN_OK;
	}else{
		return RETURN_CANCEL;
	}
}

/*================================================================================
  DESCRIPT : 전달 받은 입력 값을 범위 허용 값인지를 확인 후 반환
  KEY WORD : GetNumberBox
  RETURN   : -2  = Resource Error
			 -1  = Range over
			 0   = Cancel
			 1   = not problem
  ARGUMENT : szNum : 입력 값, maxchar : 입력 값의 Lens, getnum : 현재 값, title : Dlg Static, cMax : 최대 값, cMin : 최소 값
  CONTENT  : 입력 값이 최대, 최소 값의 범위를 벗어난 경우 Error 처리 후 FALSE 반환, 허용 범위 값인 경우 TRUE 반환
=================================================================================*/
int CLibCommCtrl::GetNumberBox(CWnd* cWnd, char* szRetrunNum, int maxchar, char* getnum, char* title,char* cMax, char* cMin)
{
	if(cWnd->GetDlgItem(IDOK) != NULL){
		cWnd->GetDlgItem(IDOK)->SetFocus();
	}else if(cWnd->GetDlgItem(IDCANCEL) != NULL){
		cWnd->GetDlgItem(IDCANCEL)->SetFocus();
	}
	else{
		return RETURN_RESOURCE_ERR;
	}

	CNumberBoxDlg dlg(maxchar, getnum, title, cWnd, cMax, cMin);

	if( dlg.DoModal() == IDOK )
	{
		lstrcpy(szRetrunNum, dlg.m_strGetNum);
		double dMax = atof(cMax);
		double dMin = atof(cMin);
		double dInputNum = atof(szRetrunNum);
		//CString strRangemsg = "";

		if(dInputNum < dMin || dInputNum > dMax)
		{
			strcpy(szRetrunNum, getnum);
			return RETURN_ERR;
		}
		else
		{
			return RETURN_OK;
		}
	}
	else
	{
		strcpy(szRetrunNum, getnum);
		return RETURN_CANCEL;
	}
}
/*================================================================================
  DESCRIPT : 최소값, 최대값 없을 경우 사용
  KEY WORD : // by jhjo[141105] 범위 값 없을 경우 사용
  RETURN   : Ok 버튼 입력하면 True, Cancle 입력 하면 False
  ARGUMENT : szNum : 입력 값, maxchar : 입력 값의 Lens, getnum : 현재 값, title : Dlg Static
  CONTENT  : 최소값, 최대값 범위가 없을 경우 입력받은 값을 저장.
=================================================================================*/
int CLibCommCtrl::GetNumberBox2(CWnd* cWnd, char* szRetrunNum, int maxchar, char* getnum, char* title)
{
	if(cWnd->GetDlgItem(IDOK) != NULL){
		cWnd->GetDlgItem(IDOK)->SetFocus();
	}else if(cWnd->GetDlgItem(IDCANCEL) != NULL){
		cWnd->GetDlgItem(IDCANCEL)->SetFocus();
	}
	else{
		return RETURN_RESOURCE_ERR;
	}

	CNumberBoxDlg dlg(maxchar, getnum, title, NULL, "", "");

	if( dlg.DoModal() == IDOK )
	{
		lstrcpy(szRetrunNum, dlg.m_strGetNum);
		return RETURN_OK;
	}
	else
	{
		strcpy(szRetrunNum,getnum);
		return RETURN_CANCEL;
	}
}

BOOL  CLibCommCtrl::ShowLogInBox2(/*char* szFilePath,*/ int nUserLv, int* nReturnLv, char* szGetUserID , int* nLoginTime)
{
	CLogInUserDlg dlg(/*szFilePath*/PATH_LOGIN_FILE, nUserLv, NULL);

	if( dlg.DoModal() == IDOK )
	{
		*nReturnLv = dlg.m_nUserLv;
		memcpy(szGetUserID, dlg.m_szGetUserID, sizeof(szGetUserID));
		*nLoginTime = dlg.m_nLoginTime;
		return RETURN_OK;
	}
	else
	{
		*nReturnLv = nUserLv;
		return RETURN_CANCEL;
	}
}

BOOL  CLibCommCtrl::ShowLogInBox(/*char* szFilePath,*/ int nUserLv, int* nReturnLv)
{
	CLogInUserDlg dlg(/*szFilePath*/PATH_LOGIN_FILE, nUserLv, NULL);

	if( dlg.DoModal() == IDOK )
	{
		*nReturnLv = dlg.m_nUserLv;
		return RETURN_OK;
	}
	else
	{
		*nReturnLv = nUserLv;
		return RETURN_CANCEL;
	}
}

void CLibCommCtrl::ShowRegisterUser(/*char* szFilePath*/)
{
	CRegisterUserDlg dlg(/*szFilePath*/PATH_LOGIN_FILE);

    dlg.DoModal();

	m_strCreateID = dlg.m_strCreateID;
}

void CLibCommCtrl::ShowMessageBox(ST_SMB_INFO& MsgInfo)
{
	g_LibCommCtrlDrv.AddShowMsgBox(MsgInfo);
}

CString CLibCommCtrl::GetSelectDate()
{
	CString strData = _T("");
	CCalenderDlg DO;
	
	if(DO.DoModal()==IDOK)
	{
		strData = DO.GetSelectedDate();
	}
	
	return strData;
}

void CLibCommCtrl::ProgressBarCreate( bool _cancel_visible, void (*_cancel_callback)( int ), int _axis_no )
{
	g_Progress.Create( nullptr, _cancel_callback, _axis_no );
	g_Progress.ShowWindow(SW_SHOW);
	g_Progress.SetVisibleCancelButton(_cancel_visible);	
}
void CLibCommCtrl::ProgressBarDestroy()
{
	g_Progress.DestroyWindow();
}

void CLibCommCtrl::ProgressBarSetPos(int nPos)
{
	g_Progress.SetPos(nPos);
}





void CLibCommCtrl::ProgressBarSetStatus(CString strStatus)
{
	g_Progress.SetStatus(strStatus);
}

void CLibCommCtrl::ProgressBarNewCreate()
{
	g_pProgress = new CProgressDlg();
	g_pProgress->CDialog::Create(IDD_DLG_PROGRESS_BAR);
	g_pProgress->ShowWindow(SW_SHOW);
}

void CLibCommCtrl::ProgressBarShow()
{
	g_pProgress->ShowWindow(SW_SHOW);
	::SetWindowPos(g_pProgress->GetSafeHwnd(), HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
}
void CLibCommCtrl::ProgressBarNewDestroy()
{
	g_pProgress->DestroyWindow();
	g_pProgress = NULL;
}

void CLibCommCtrl::ProgressBarNewSetPos(int nPos)
{
	g_pProgress->SetPos(nPos);
	::SetWindowPos(g_pProgress->GetSafeHwnd(), HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
}

void CLibCommCtrl::ProgressBarNewSetStatus(CString strStatus)
{
	g_pProgress->SetStatus(strStatus);
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : 
  PARAMS   : p_szTitle      : Message Box Title
             p_szCode       : Message Code
             p_szPart       : Message Part
			 p_szDefaulMsg  : Default 가 되는 Message
			 pcSecondaryMsg : Second로 표기 되는 Message
			 error_type     : Error Tpye(??)
			 Type           : Button 3개냐 2개냐에 따른 Type
			 p_szYes        : 1번째 button에 기록 될 문자 
			 p_szNo         : 2번째 button에 기록 될 문자 
			 Third          : 3번째 button에 기록 될 문자 
			 nImageShow     : Image가 표기 되어야 되는지 아닌지
			 FontHeight     : Font의 높이
			 FontWidth     : Font의 넓이
  RETURN   : 
  COMMENTS : 
==============================================================================*/
UINT CLibCommCtrl::MyMsg3(char* p_szTitle, char* p_szCode, char* p_szPart, char* p_szDefaulMsg, char *pcSecondaryMsg, UINT error_type,
	UINT type, char* p_szYes, char* p_szNo, char*  Third, int nImageShow, HBITMAP hBitMap, int FontHeight, int FontWidth)
{
	int nReturn = g_LibCommCtrlDrv.MyMsg3(p_szTitle, p_szCode, p_szPart, p_szDefaulMsg, pcSecondaryMsg, error_type, type, p_szYes, p_szNo, Third, nImageShow, hBitMap, FontHeight, FontWidth);

	return nReturn;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
UINT CLibCommCtrl::MyMsg2(char* p_szTitle, char* p_szCode, char* p_szPart, char* p_szDefaulMsg, char *pcSecondaryMsg, UINT error_type,
	UINT type, char* p_szYes, char* p_szNo, int nImageShow, HBITMAP hBitMap, int FontHeight, int FontWidth)
{
	int nReturn = g_LibCommCtrlDrv.MyMsg2(p_szTitle, p_szCode, p_szPart, p_szDefaulMsg, pcSecondaryMsg, error_type, type, p_szYes, p_szNo, nImageShow, hBitMap, FontHeight, FontWidth);

	return nReturn;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : 
  PARAMS   : StUserConfigSet : System Option parameter struct
  RETURN   : None
  COMMENTS : System에서 사용하는 Function을 인자로 받아서 다이얼로그에 보여주며, 셋팅 설정 이후 멤버 변수에 
             해당 값을 가진 상태로 둔다.
==============================================================================*/
void CLibCommCtrl::ShowUserConfig(ST_USER_CONFIG_ARRAY  StUserConfigSet)
{

	CUserConfigDlg DO(StUserConfigSet);
	DO.DoModal();

	m_UserConfigSet = DO.m_StUserConfigSet;
}


/*=============================================================================
  AUTHOR   : 김선진
  NAME     : 
  PARAMS   : 
  RETURN   : 셋팅 설정 이후 멤버 변수에 저장 된 값.
  COMMENTS : System에서 사용하는 Function을 멤버로 저장 한 것을 Return
==============================================================================*/
ST_USER_CONFIG_ARRAY CLibCommCtrl::GetUserConfig()
{
	return m_UserConfigSet;
}