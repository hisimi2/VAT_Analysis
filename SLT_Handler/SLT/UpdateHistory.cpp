// UpdateHistory.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "UpdateHistory.h"
#include "afxdialogex.h"
#include <fstream>
#include <locale.h>
// CUpdateHistory 대화 상자입니다.

IMPLEMENT_DYNAMIC(CUpdateHistory, CDialog)

CUpdateHistory::CUpdateHistory(CWnd* pParent /*=NULL*/)
	: CDialog(CUpdateHistory::IDD, pParent)
{

}

CUpdateHistory::~CUpdateHistory()
{
}

void CUpdateHistory::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	 DDX_Control(pDX, IDC_EDIT_HISTORY, m_EditHistory);
	 DDX_Control(pDX, IDCANCEL, m_BtnCancel);
}


BEGIN_MESSAGE_MAP(CUpdateHistory, CDialog)
END_MESSAGE_MAP()


// CUpdateHistory 메시지 처리기입니다.


BOOL CUpdateHistory::OnInitDialog()
{
	CDialog::OnInitDialog();
	MakeLog("Update History Dialog Show");

	m_EditHistory.SetBackColor(COLOR_WWHITE);
	m_BtnCancel.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnCancel.SetIcon(IDI_BTN_EXIT);
	m_BtnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);	

	InitHistory();
	return TRUE;
}

BOOL CUpdateHistory::InitHistory()
{
	char szPath[512] = {0,};
	::GetCurrentDirectory(sizeof(szPath), szPath);

	CString strHistory = _T("");

	CString strFilePath = _T("");
	strFilePath.Format("%s\\UpdateHistory.txt",szPath);
	CFile		fUpdateHistory;
	char		*pUpdateHistory	= NULL;
	int         nFileLength;

	//_tsetlocale(LC_ALL, "korean");
	if(fUpdateHistory.Open(strFilePath,CFile::modeRead)){
		nFileLength = (int)fUpdateHistory.GetLength();
		pUpdateHistory = new char[nFileLength+1];

		ZeroMemory(pUpdateHistory,sizeof(char)*nFileLength+1);
		try
		{
			fUpdateHistory.Read(pUpdateHistory,nFileLength);
			m_EditHistory.SetWindowText(pUpdateHistory);
		}
		catch (CFileException* e)
		{
			char szErr[1024] = {0,};
			CString strErrMsg = "";
			e->GetErrorMessage(szErr, 1024);
			strErrMsg.Format("CFile exception error! Error code : %d, Error msg : %s", e->m_cause, szErr);
			MakeLog("CFile exception Error");
		}
		fUpdateHistory.Close();
		if(pUpdateHistory != NULL){
			delete [] pUpdateHistory;
			pUpdateHistory = NULL;
		}
	}else{
		CString strErr = _T("");
		strErr.Format("%s 경로에 UpdateHistory.txt 파일이 존재 하지 않습니다.",szPath);
		AfxMessageBox(strErr);
	}
	return TRUE;
}

void CUpdateHistory::MakeLog(LPCTSTR fmt, ...)
{
	TCHAR tmpMsg[1024] = {0,};
	if(fmt)
	{
		va_list argList;
		va_start(argList, fmt);
		_vstprintf(tmpMsg, fmt, argList);
		va_end(argList);
	}
	TWLOG.MakeLog(static_cast<int>(eLogID_System_GUI), Debug, "", __LINE__, NULL, tmpMsg);
}