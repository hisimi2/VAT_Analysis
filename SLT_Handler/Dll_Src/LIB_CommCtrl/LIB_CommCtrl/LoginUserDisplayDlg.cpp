// LoginUserDisplayDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "LoginUserDisplayDlg.h"
#include "afxdialogex.h"


// CLoginUserDisplayDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLoginUserDisplayDlg, CDialog)

CLoginUserDisplayDlg::CLoginUserDisplayDlg(char* szFilePath, CWnd* pParent)//(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginUserDisplayDlg::IDD, pParent)
{
	memset(m_szFilePath, 0x00, sizeof(m_szFilePath));
	sprintf_s(m_szFilePath, sizeof(m_szFilePath), "%s", szFilePath);
}

CLoginUserDisplayDlg::~CLoginUserDisplayDlg()
{
}

void CLoginUserDisplayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_USER, m_UserList);
}


BEGIN_MESSAGE_MAP(CLoginUserDisplayDlg, CDialog)
END_MESSAGE_MAP()


// CLoginUserDisplayDlg 메시지 처리기입니다.


BOOL CLoginUserDisplayDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ShowUser();
	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CLoginUserDisplayDlg::ShowUser()
{
	CString strContent    = _T("");
	CString strFilePath   = _T("");
	CString strUserTemp   = _T("");
	CString strUserIDTemp = _T("");
	CString strUserPWTemp = _T("");
	CFile f;
	CFileStatus fs;


	m_UserList.DeleteAllItems();
	m_UserList.SetExtendedStyle(LVS_EX_GRIDLINES);

	//-- make column
	m_UserList.InsertColumn(0, _T("INDEX"), LVCFMT_CENTER, 90);
	m_UserList.InsertColumn(1, _T("ID"), LVCFMT_CENTER, 190);
	m_UserList.InsertColumn(2, _T("PW"), LVCFMT_CENTER, 200);
	m_UserList.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);

	strFilePath.Format("%s",m_szFilePath);
	//file 존재하는지 확인
	if(!f.Open(strFilePath, CFile::modeWrite))
	{
		//파일이 없으면 생성
		if(!f.Open(strFilePath, CFile::modeCreate))
		{
			//File Create Error 추가
			return;
		}
	}
	f.Close();

	CStdioFile sf(strFilePath, CFile::modeRead);
	sf.SeekToBegin();
	//확인 버튼 클릭 시 등록된 ID가 있는지 비교
	int nCount = 0;
	while(sf.ReadString(strUserTemp) != NULL)
	{
		strContent.Empty();
		AfxExtractSubString(strUserIDTemp, strUserTemp, 0, ' ');
		AfxExtractSubString(strUserPWTemp, strUserTemp, 1, ' ');

		strContent.Format("%d",nCount);		
		m_UserList.InsertItem(nCount, strContent);
		strContent.Format(_T("%s"),strUserIDTemp);		
		m_UserList.SetItem(nCount, 1,LVIF_TEXT,strContent, NULL, NULL, NULL, NULL );
		strContent.Format(_T("%s"),strUserPWTemp);
		m_UserList.SetItem(nCount, 2,LVIF_TEXT,strContent, NULL, NULL, NULL, NULL );
		nCount ++;
		Sleep(1);
	}
	sf.Close();
}