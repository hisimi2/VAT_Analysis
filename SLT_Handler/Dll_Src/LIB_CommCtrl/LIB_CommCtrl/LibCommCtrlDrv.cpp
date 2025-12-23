#include "StdAfx.h"
#include "LibCommCtrlDrv.h"
#include "ShowMessageBox.h"
#include "StopMsgBox.h"

CLibCommCtrlDrv::CLibCommCtrlDrv(void)
{
}


CLibCommCtrlDrv::~CLibCommCtrlDrv(void)
{
}


void CLibCommCtrlDrv::AddShowMsgBox(ST_SMB_INFO& MsgInfo)
{
	ST_SMB_INFO* pItem = new ST_SMB_INFO;
	*pItem = MsgInfo;
	m_listSMB.AddTail(pItem);

	CShowMsgBox* pShowMessageBox = new CShowMsgBox;
	pShowMessageBox->m_pMsgInfo = pItem;
	pShowMessageBox->Create(IDD_DLG_ERROR);
	pShowMessageBox->ShowWindow(SW_SHOW);	
}

void CLibCommCtrlDrv::DelShowMsgBox(ST_SMB_INFO* pItemSrc)
{
	int nCount = m_listSMB.GetCount();

	POSITION pos = m_listSMB.GetHeadPosition();
	for(int i=0; i<nCount; i++)
	{
		POSITION pre_pos = pos;
		ST_SMB_INFO* pItem = (ST_SMB_INFO*)m_listSMB.GetNext(pos);
		if( pItem == pItemSrc ){
			::SendMessage((HWND)pItem->hWnd, UM_SHOW_MSG_BOX_RSP, (WPARAM)pItem, NULL);
			m_listSMB.RemoveAt(pre_pos);
			delete pItem;
			pItem = NULL;
		}
	}
}

void CLibCommCtrlDrv::ClearShowMsgBox()
{
	int nCount = m_listSMB.GetCount();
	POSITION pos = m_listSMB.GetHeadPosition();

	for(int i=0; i<nCount; i++)
	{
		ST_SMB_INFO* pItem = (ST_SMB_INFO*)m_listSMB.GetNext(pos);
		if( pItem != NULL ){
			delete pItem;
			pItem = NULL;
		}
	}
	m_listSMB.RemoveAll();
}

UINT CLibCommCtrlDrv:: MyMsg3(char* p_szTitle, char* p_szCode, char* p_szPart, char* p_szDefaulMsg, char *pcSecondaryMsg, UINT error_type,
	                              UINT type,char* Fist, char* Sencond, char*  Third, int nImageShow, HBITMAP hBitmap, int FontHeight, int FontWidth)
{
	// 같은 메세지가 출력되어 있으면 생성안함
	HWND hWnd = ::FindWindow(NULL, p_szTitle);
	if( hWnd )
	{
		return FALSE;
	}

	CStopMsgBox* dlg = new CStopMsgBox;

	dlg->m_nFontHeight = FontHeight;
	dlg->m_nFontWidth  = FontWidth;	

	if		( type & M_ICONINFORMATION )	dlg->m_icontype = M_ICONINFORMATION;
	else if	( type & M_ICONQUESTION )		dlg->m_icontype = M_ICONQUESTION;
	else if	( type & M_ICONSTOP )			dlg->m_icontype = M_ICONSTOP;
	else if	( type & M_ICONERROR )			dlg->m_icontype = M_ICONERROR;
	else									dlg->m_icontype = M_ICONINFORMATION;

	if		( type & M_OK )					dlg->m_type = M_OK;
	else if	( type & M_OKCANCEL )			dlg->m_type = M_OKCANCEL;
	else if	( type & M_YESNO )				dlg->m_type = M_YESNO;
	else if	( type & M_ABORTRETRYIGNORE )	dlg->m_type = M_ABORTRETRYIGNORE;
	else if	( type & M_RETRYCANCEL )		dlg->m_type = M_RETRYCANCEL;
	else									dlg->m_type = M_OK;

	dlg->m_error_type = error_type;

	CString strMsg = (LPCSTR)(LPSTR)p_szDefaulMsg;
	strMsg.Replace(_T("\\r\\n"), _T("\r\n"));


	strcpy(dlg->m_szTitle,        p_szTitle);
	strcpy(dlg->m_szFirstMsg,     p_szDefaulMsg);
	strcpy(dlg->m_szSecondaryMsg, pcSecondaryMsg);
	strcpy(dlg->m_szErrorCode,    p_szCode);
	strcpy(dlg->m_szSubCode,      p_szCode);
	strcpy(dlg->m_strYes,         Fist);
	strcpy(dlg->m_strNo,          Sencond);	
	strcpy(dlg->m_strRetry,       Third);	
	strcpy(dlg->m_szUnitName,     p_szPart);
	dlg->m_nImagePos   = nImageShow;
	dlg->m_BitMap      = hBitmap;

	dlg->Create(IDD_DLG_STOPMSG, NULL);
	dlg->ShowWindow(SW_SHOW);

	// 다이얼로그가 종료할때까지 기다린다.
	dlg->m_nRes = 0;
	while(!dlg->m_nRes)
	{
		MSG message;

		if(::PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&message);
			::DispatchMessage(&message);
		}
		Sleep(1);
	}

	int m_Res = dlg->m_nRes;
	delete dlg;

	return m_Res;
}

UINT CLibCommCtrlDrv:: MyMsg2(char* p_szTitle, char* p_szCode, char* p_szPart, char* p_szDefaulMsg, char *pcSecondaryMsg, UINT error_type,
	UINT type,char* Fist, char* Sencond, int nImageShow, HBITMAP hBitmap, int FontHeight, int FontWidth)
{
	// 같은 메세지가 출력되어 있으면 생성안함
	HWND hWnd = ::FindWindow(NULL, p_szTitle);
	if( hWnd )
	{
		return FALSE;
	}

	CStopMsgBox* dlg = new CStopMsgBox;

	dlg->m_nFontHeight = FontHeight;
	dlg->m_nFontWidth  = FontWidth;	

	if		( type & M_ICONINFORMATION )	dlg->m_icontype = M_ICONINFORMATION;
	else if	( type & M_ICONQUESTION )		dlg->m_icontype = M_ICONQUESTION;
	else if	( type & M_ICONSTOP )			dlg->m_icontype = M_ICONSTOP;
	else if	( type & M_ICONERROR )			dlg->m_icontype = M_ICONERROR;
	else									dlg->m_icontype = M_ICONINFORMATION;

	if		( type & M_OK )					dlg->m_type = M_OK;
	else if	( type & M_OKCANCEL )			dlg->m_type = M_OKCANCEL;
	else if	( type & M_YESNO )				dlg->m_type = M_YESNO;
	else if	( type & M_ABORTRETRYIGNORE )	dlg->m_type = M_ABORTRETRYIGNORE;
	else if	( type & M_RETRYCANCEL )		dlg->m_type = M_RETRYCANCEL;
	else									dlg->m_type = M_OK;

	dlg->m_error_type = error_type;

	CString strMsg = (LPCSTR)(LPSTR)p_szDefaulMsg;
	strMsg.Replace(_T("\\r\\n"), _T("\r\n"));


	strcpy(dlg->m_szTitle,        p_szTitle);
	strcpy(dlg->m_szFirstMsg,     p_szDefaulMsg);
	strcpy(dlg->m_szSecondaryMsg, pcSecondaryMsg);
	strcpy(dlg->m_szErrorCode,    p_szCode);
	strcpy(dlg->m_szSubCode,      p_szCode);
	strcpy(dlg->m_strYes,         Fist);
	strcpy(dlg->m_strNo,          Sencond);	
	strcpy(dlg->m_szUnitName,     p_szPart);
	dlg->m_nImagePos   = nImageShow;
	dlg->m_BitMap      = hBitmap;

	dlg->Create(IDD_DLG_STOPMSG, NULL);
	dlg->ShowWindow(SW_SHOW);

	// 다이얼로그가 종료할때까지 기다린다.
	dlg->m_nRes = 0;
	while(!dlg->m_nRes)
	{
		MSG message;
		
		if(::PeekMessage(&message, dlg->GetSafeHwnd(), NULL, NULL, PM_REMOVE))
		{
			::TranslateMessage(&message);
			::DispatchMessage(&message);

 			::SetWindowPos(dlg->GetSafeHwnd(), HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
			//::SetWindowPos(dlg->GetSafeHwnd(), HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOSIZE);
		}
		Sleep(1);
	}

	int m_Res = dlg->m_nRes;
	delete dlg;
	dlg = nullptr;

	return m_Res;
}