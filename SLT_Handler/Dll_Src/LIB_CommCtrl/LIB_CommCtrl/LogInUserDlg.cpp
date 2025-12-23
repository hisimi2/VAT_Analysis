// LogInUserDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "LogInUserDlg.h"
#include "afxdialogex.h"
#include "DEF_LIB_COMMCTRL.h"


St_Keyboard g_KeyValue[MAX_KEY_VALUE] =
{
	{"1", "1", IDC_BUTTON_LOGON_KEY1}, {"2", "2", IDC_BUTTON_LOGON_KEY2},	{"3", "3", IDC_BUTTON_LOGON_KEY3}, {"4", "4", IDC_BUTTON_LOGON_KEY4},
	{"5", "5", IDC_BUTTON_LOGON_KEY5}, {"6", "6", IDC_BUTTON_LOGON_KEY6},	{"7", "7", IDC_BUTTON_LOGON_KEY7}, {"8", "8", IDC_BUTTON_LOGON_KEY8},
	{"9", "9", IDC_BUTTON_LOGON_KEY9}, {"0", "0", IDC_BUTTON_LOGON_KEY10},	{"q", "Q", IDC_BUTTON_LOGON_KEY15},	{"w", "W", IDC_BUTTON_LOGON_KEY16},
	{"e", "E", IDC_BUTTON_LOGON_KEY17}, {"r", "R", IDC_BUTTON_LOGON_KEY18},	{"t", "T", IDC_BUTTON_LOGON_KEY19}, {"y", "Y", IDC_BUTTON_LOGON_KEY20},
	{"u", "U", IDC_BUTTON_LOGON_KEY21}, {"i", "I", IDC_BUTTON_LOGON_KEY22},	{"o", "O", IDC_BUTTON_LOGON_KEY23}, {"p", "P", IDC_BUTTON_LOGON_KEY24},
	{"a", "A", IDC_BUTTON_LOGON_KEY27}, {"s", "S", IDC_BUTTON_LOGON_KEY28},	{"d", "D", IDC_BUTTON_LOGON_KEY29}, {"f", "F", IDC_BUTTON_LOGON_KEY30},
	{"g", "G", IDC_BUTTON_LOGON_KEY31}, {"h", "H", IDC_BUTTON_LOGON_KEY32},	{"j", "J", IDC_BUTTON_LOGON_KEY33}, {"k", "K", IDC_BUTTON_LOGON_KEY34},
	{"l", "L", IDC_BUTTON_LOGON_KEY35}, {"z", "Z", IDC_BUTTON_LOGON_KEY36},	{"x", "X", IDC_BUTTON_LOGON_KEY37}, {"c", "C", IDC_BUTTON_LOGON_KEY38},
	{"v", "V", IDC_BUTTON_LOGON_KEY39}, {"b", "B", IDC_BUTTON_LOGON_KEY40},	{"n", "N", IDC_BUTTON_LOGON_KEY41}, {"m", "M", IDC_BUTTON_LOGON_KEY42},
};
// CLogInUserDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLogInUserDlg, CDialog)

CLogInUserDlg::CLogInUserDlg(char* szFilePath, int szUseLv, CWnd* pParent)//CLogInUserDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLogInUserDlg::IDD, pParent)
{
	m_bTempLogOn = FALSE;
	m_nEditFocus = ID_EDIT_FOCUS;
	m_bIsShift = FALSE;

	memset(m_szUserID, 0x00, sizeof(m_szUserID));
	memset(m_szUserPW, 0x00, sizeof(m_szUserPW));

	memset(m_szFilePath, 0x00, sizeof(m_szFilePath));
	sprintf_s(m_szFilePath, sizeof(m_szFilePath), "%s", szFilePath);
	m_nUserLv    = szUseLv;
	
	memset(m_szGetUserID, 0x00, sizeof(m_szGetUserID));

	m_nLoginTime =0;
}

CLogInUserDlg::~CLogInUserDlg()
{
}

void CLogInUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LOGIN_USER_ID, m_userID);
	DDX_Control(pDX, IDC_EDIT_LOGIN_USER_PW, m_userPW);
}


BEGIN_MESSAGE_MAP(CLogInUserDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN_OK, &CLogInUserDlg::OnBnClickedButtonLoginOk)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN_EXIT, &CLogInUserDlg::OnBnClickedButtonLoginExit)

	// By jhjang [150320] [C-150223-01] Add Log On Dialog Keyboard <start>
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BUTTON_LOGON_KEY1, IDC_BUTTON_LOGON_KEY42, &CLogInUserDlg::OnBnClickedKey)
	ON_BN_CLICKED(IDC_BUTTON_LOGON_KEY13, &CLogInUserDlg::OnBnClicked_Delete)
	ON_BN_CLICKED(IDC_BUTTON_LOGON_KEY14, &CLogInUserDlg::OnBnClicked_Clear)
	ON_BN_CLICKED(IDC_BUTTON_LOGON_KEY45, &CLogInUserDlg::OnBnClicked_Shift)
	ON_EN_SETFOCUS(IDC_EDIT_LOGIN_USER_ID, &CLogInUserDlg::OnEnSetFocusEditID)
	ON_EN_SETFOCUS(IDC_EDIT_LOGIN_USER_PW, &CLogInUserDlg::OnEnSetFocusEditPW)
	// By jhjang [150320] [C-150223-01] Add Log On Dialog Keyboard <end>
	ON_EN_CHANGE(IDC_EDIT_LOGIN_USER_TIME, &CLogInUserDlg::OnEnChangeEditLoginUserTime)
END_MESSAGE_MAP()


// CLogInUserDlg 메시지 처리기입니다.


void CLogInUserDlg::OnBnClickedButtonLoginOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.	
	char szUserId[10]        = {0,};
	char szUserPw[10]        = {0,};
	CString strUserID        = _T("");
	CString strUserPW        = _T("");
	CString strUserTime = _T("");

	GetDlgItemText(IDC_EDIT_LOGIN_USER_ID, strUserID);
	GetDlgItemText(IDC_EDIT_LOGIN_USER_PW, strUserPW);

	if(strUserID.GetLength() > 10)
	{
		AfxMessageBox("ID length is too long. Please input the id length less than 10");
		return;
	}
	if(strUserPW.GetLength() > 10)
	{
		AfxMessageBox("Password length is too long. Please input the password length less than 10");
		return;
	}

	//ID, PW 입력값 받아오기
	GetDlgItemText(IDC_EDIT_LOGIN_USER_ID, szUserId, sizeof(szUserId));
	GetDlgItemText(IDC_EDIT_LOGIN_USER_PW, szUserPw, sizeof(szUserPw));
	GetDlgItemText(IDC_EDIT_LOGIN_USER_TIME, strUserTime);
	m_nLoginTime = atoi(strUserTime);

	//ID, PW 입력 확인
	if(strlen(szUserId) == 0)
	{
		AfxMessageBox("Please input the ID.");
		return;
	}
	if(strlen(szUserPw) == 0)
	{
		AfxMessageBox("Please input the Password.");
		return;
	}

	

	BOOL bFileOpen = FALSE;
	if(strcmp(szUserId, ADMIN_ACCOUNT) != 0)
	{
		bFileOpen  = FileOpenCheck();					// File Open Check
		if(bFileOpen == FALSE){
			return;
		}
	}	

	int  nCompID_PW = CompareUser(szUserId, szUserPw);	// User ID/PW check
	if(nCompID_PW != TRUE){
		return;
	}
	sprintf_s(m_szGetUserID,sizeof(m_szGetUserID),szUserId);
	//memcpy(m_szGetUserID, szUserId, sizeof(m_szGetUserID));
	CDialog::OnOK();
}

BOOL CLogInUserDlg::FileOpenCheck()
{
//	CFile       f;
	CFileStatus fs;

	//file 존재하는지 확인
	if(!CFile::GetStatus(m_szFilePath, fs))
	{
		AfxMessageBox("don't have file!! Please register first!!");
		return FALSE;
// 		if(!f.Open(m_szFilePath, CFile::modeCreate))
// 		{
// 			AfxMessageBox("File create failed.");
// 			return FALSE;
// 		}
// 		AfxMessageBox("User Information is not found. New file is created.");
// 		f.Close();
	}

	return TRUE;
}

/*==============================================================
  RETURN : ERRORCODE
           TRUE  = OK
		   FALSE  = PW error or ID error
==============================================================*/
BOOL CLogInUserDlg::CompareUser(char* szUserId, char* szUserPw)
{
	CString strUserIDTemp    = "";
	CString strUserPWTemp    = "";
	CString strUserLevelTemp = "";
	CString strUserInfoTemp  = "";

	CFile       f;
	CFileStatus fs;
	
	if(strUserIDTemp.Compare(szUserId) == 0 || strcmp(szUserId, ADMIN_ACCOUNT) == 0)
	{		
		if(strcmp(szUserPw, ADMIN_PASSWORD) == 0)
		{
			m_nUserLv = USERLEVEL_SUPERVISOR;
		}
		else if(strUserPWTemp.Compare(szUserPw) == 0)
		{
			m_nUserLv = atoi(strUserLevelTemp);	
		}
		else	//비밀번호 맞지 않음
		{
			AfxMessageBox("The password is incorrect.");
			return FALSE;
		}	
	}
	else
	{
		BOOL        bFindUserID  = FALSE;

		//User 정보 한 명씩 읽어와 입력 ID, PW 비교
		CStdioFile sf(m_szFilePath, CFile::modeRead);
		sf.SeekToBegin();
		while(sf.ReadString(strUserInfoTemp) != NULL)
		{
			AfxExtractSubString(strUserIDTemp, strUserInfoTemp, 0, ' ');
			AfxExtractSubString(strUserPWTemp, strUserInfoTemp, 1, ' ');
			AfxExtractSubString(strUserLevelTemp, strUserInfoTemp, 2, ' ');

			if(strUserIDTemp.Compare(szUserId) == 0 || strcmp(szUserId, ADMIN_ACCOUNT) == 0)
			{		
				if(strcmp(szUserPw, ADMIN_PASSWORD) == 0)
				{
					m_nUserLv = USERLEVEL_SUPERVISOR;
				}
				else if(strUserPWTemp.Compare(szUserPw) == 0)
				{
					m_nUserLv = atoi(strUserLevelTemp);	
				}
				else	//비밀번호 맞지 않음
				{
					AfxMessageBox("The password is incorrect.");
					sf.Close();
					return FALSE;
				}	

				bFindUserID = TRUE;
			}
			Sleep(1);
		}

		if(bFindUserID == FALSE)	//등록된 ID가 없음
		{
			AfxMessageBox("User ID is not found.");
			sf.Close();
			return FALSE;
		}
		sf.Close();
	}
	

	return TRUE;
}


void CLogInUserDlg::OnBnClickedButtonLoginExit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialog::OnCancel();
}

BOOL CLogInUserDlg::IsMaxCharLength()
{
	int	    nInputValue   = 0;
	char	szUserId[128] = {0, };
	char	szUserPw[128] = {0, };

	GetDlgItemText(IDC_EDIT_LOGIN_USER_ID, szUserId, sizeof(szUserId));
	GetDlgItemText(IDC_EDIT_LOGIN_USER_PW, szUserPw, sizeof(szUserPw));

	if(m_nEditFocus == ID_EDIT_FOCUS)
	{
		nInputValue = strlen(szUserId);
	}
	else if(m_nEditFocus == PW_EDIT_FOCUS)
	{
		nInputValue = strlen(szUserPw);
	}
	
	if(nInputValue > MAX_LOGON_TEXT_LENGTH)
	{
		return TRUE;
	}

	return FALSE;
}

void CLogInUserDlg::OnBnClicked_Shift()
{
	if(m_bIsShift != TRUE)
	{
		m_bIsShift = TRUE;
	}
	else
	{
		m_bIsShift = FALSE;
	}

	ChangeKey();
}

void CLogInUserDlg::AddInputedValue(char *pcInputedValue)
{
	char	szUserId[128] = {0, };
	char	szUserPw[128] = {0, };

	GetDlgItemText(IDC_EDIT_LOGIN_USER_ID, szUserId, sizeof(szUserId));
	GetDlgItemText(IDC_EDIT_LOGIN_USER_PW, szUserPw, sizeof(szUserPw));

	if(m_nEditFocus == ID_EDIT_FOCUS)
	{
		memset(m_szUserPW, 0x00, sizeof(m_szUserPW));
		strcpy_s(m_szUserPW, sizeof(szUserPw), szUserPw);

		memset(m_szUserID, 0x00, sizeof(m_szUserID));
		strcpy_s(m_szUserID, sizeof(m_szUserID), szUserId);
		strcat_s(m_szUserID, sizeof(m_szUserID), pcInputedValue);
	}
	if(m_nEditFocus == PW_EDIT_FOCUS)
	{
		memset(m_szUserID, 0x00, sizeof(m_szUserID));
		strcpy_s(m_szUserID, sizeof(m_szUserID), szUserId);

		memset(m_szUserPW, 0x00, sizeof(m_szUserPW));
		strcpy_s(m_szUserPW, sizeof(szUserPw), szUserPw);
		strcat_s(m_szUserPW, sizeof(m_szUserPW), pcInputedValue);
	}
}

void CLogInUserDlg::ChangeKey()
{
	for(int i=0; i<MAX_KEY_VALUE; i++)
	{
		if(m_bIsShift == TRUE)
		{
			SetDlgItemText(g_KeyValue[i].nKeyID, g_KeyValue[i].szSecondKey);
		}
		else
		{
			SetDlgItemText(g_KeyValue[i].nKeyID, g_KeyValue[i].szFirstKey);
		}		
	}
}

void CLogInUserDlg::OnBnClickedKey(UINT ID)
{
	BOOL	bIsMaxCharLength = FALSE;
	
	bIsMaxCharLength = IsMaxCharLength();

	if(bIsMaxCharLength == TRUE)
	{
		return;
	}

	if(ID == IDC_BUTTON_LOGON_KEY13)
	{
		OnBnClicked_Delete();
		return;
	}
	if(ID == IDC_BUTTON_LOGON_KEY14)
	{
		OnBnClicked_Clear();
		return;
	}

	for(int i=0; i<MAX_KEY_VALUE; i++)
	{
		if(g_KeyValue[i].nKeyID == ID)
		{
			if(m_bIsShift != TRUE)
			{
				AddInputedValue(g_KeyValue[i].szFirstKey);
			}
			else
			{
				AddInputedValue(g_KeyValue[i].szSecondKey);
			}			
		}
	}

	SetDlgItemText(IDC_EDIT_LOGIN_USER_ID, m_szUserID);
	SetDlgItemText(IDC_EDIT_LOGIN_USER_PW, m_szUserPW);
}

void CLogInUserDlg::OnBnClicked_Clear()
{
	if(m_nEditFocus == ID_EDIT_FOCUS)
	{
		memset(m_szUserID, 0x00, sizeof(m_szUserID));
		SetDlgItemText(IDC_EDIT_LOGIN_USER_ID, m_szUserID);
	}
	else
	{
		memset(m_szUserPW, 0x00, sizeof(m_szUserPW));
		SetDlgItemText(IDC_EDIT_LOGIN_USER_PW, m_szUserPW);
	}
}

void CLogInUserDlg::OnBnClicked_Delete()
{
	char	szUserId[128] = {0, };
	char	szUserPw[128] = {0, };
	
	int		nEditLength = 0;

	GetDlgItemText(IDC_EDIT_LOGIN_USER_ID, szUserId, sizeof(szUserId));
	GetDlgItemText(IDC_EDIT_LOGIN_USER_PW, szUserPw, sizeof(szUserPw));

	if(m_nEditFocus == ID_EDIT_FOCUS && strlen(szUserId) != 0)
	{
		nEditLength = strlen(szUserId);

		szUserId[nEditLength-1] = NULL;
		SetDlgItemText(IDC_EDIT_LOGIN_USER_ID, szUserId);
	}
	if(m_nEditFocus == PW_EDIT_FOCUS && strlen(szUserPw) != 0)
	{
		nEditLength = strlen(szUserPw);

		szUserPw[nEditLength-1] = NULL;
		SetDlgItemText(IDC_EDIT_LOGIN_USER_PW, szUserPw);
	}
}

void CLogInUserDlg::OnEnSetFocusEditID()
{
	if(m_nEditFocus == ID_EDIT_FOCUS)
	{
		return;
	}
	else
	{
		m_nEditFocus = ID_EDIT_FOCUS;
	}
}

void CLogInUserDlg::OnEnSetFocusEditPW()
{
	if(m_nEditFocus == PW_EDIT_FOCUS)
	{
		return;
	}
	else
	{
		m_nEditFocus = PW_EDIT_FOCUS;
	}
}

void CLogInUserDlg::OnEnChangeEditLoginUserTime()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
