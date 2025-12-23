// RegisterUserDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "RegisterUserDlg.h"
#include "afxdialogex.h"
#include "LoginUserDisplayDlg.h"


// CRegisterUserDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CRegisterUserDlg, CDialog)

CRegisterUserDlg::CRegisterUserDlg(char* szFilePath, CWnd* pParent)//(CWnd* pParent /*=NULL*/)
	: CDialog(CRegisterUserDlg::IDD, pParent)
{
	memset(m_szFilePath, 0x00, sizeof(m_szFilePath));
	sprintf_s(m_szFilePath, sizeof(m_szFilePath), "%s", szFilePath);
	m_bCrateSuccess = FALSE;
}

CRegisterUserDlg::~CRegisterUserDlg()
{
}

void CRegisterUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_REGISTER_USER_LEVEL, m_userLevelCombo);
	DDX_Control(pDX, IDC_EDIT_REGISTER_USER_ID, m_userID);
	DDX_Control(pDX, IDC_EDIT_REGISTER_USER_PW, m_userPW);
}


BEGIN_MESSAGE_MAP(CRegisterUserDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_REGISTER_OK, &CRegisterUserDlg::OnBnClickedButtonRegisterOk)
	ON_BN_CLICKED(IDC_BUTTON_REGISTER_EXIT, &CRegisterUserDlg::OnBnClickedButtonRegisterExit)
	ON_BN_CLICKED(IDC_BTN_REGISTER_USER_DISPLAY, &CRegisterUserDlg::OnBnClickedBtnRegisterUserDisplay)
END_MESSAGE_MAP()


// CRegisterUserDlg 메시지 처리기입니다.


void CRegisterUserDlg::OnBnClickedButtonRegisterOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strID = "";
	CString strPW = "";
	CString strUserLevel = "";
	CString strFilePath = "";
	CString strUserTemp = "";
	CString strUserIDTemp = "";
	CFile f;
	CFileStatus fs;

	m_strCreateID = _T("");
	// 20140720 Jangjh ID/PW/등급을 입력한다..
	GetDlgItemText(IDC_EDIT_REGISTER_USER_ID, strID);
	GetDlgItemText(IDC_EDIT_REGISTER_USER_PW, strPW);

	CComboBox *pComboBox = (CComboBox *)GetDlgItem(IDC_COMBO_REGISTER_USER_LEVEL);
	strUserLevel.Format("%d\r\n", pComboBox->GetCurSel());

	//ID/PW NULL 확인
	if(strID.GetLength() == 0)
	{
		AfxMessageBox("Please input the ID.");
		return;
	}

	if(strPW.GetLength() == 0)
	{
		AfxMessageBox("Please input the Password.");
		return;
	}

	// 20140730 jhjang ID/PW Lenght Check
	if(strID.GetLength() > 10)
	{
		AfxMessageBox("ID length is too long. Please input the id length less than 10");
		return;
	}
	if(strPW.GetLength() > 10)
	{
		AfxMessageBox("Password length is too long. Please input the password length less than 10");
		return;
	}

	// By jhjang [140902]: Admin Password Add & Not Register Admin Account <start>
	if(strID.Compare("techwing") == 0){
		AfxMessageBox("Do not reigster Admin ID");
		return;
	}
	// By jhjang [140902]: Admin Password Add & Not Register Admin Account <end>

	//st_AccessLevel.dat 파일 정보 Load
	strFilePath.Format("%s",m_szFilePath);
	//strcpy(strFilePath, "\\st_AccessLevel.txt");
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
	while(sf.ReadString(strUserTemp) != NULL)
	{
		AfxExtractSubString(strUserIDTemp, strUserTemp, 0, ' ');

		if(strUserIDTemp.Compare(strID) == 0)
		{
			//이미 등록된 ID가 있으니 에러 메시지 출력 후 종료
			AfxMessageBox("User ID already exist.");
			sf.Close();
			return;
		}
		Sleep(1);
	}
	sf.Close();

	//File Write를 위해 Open
	f.Open(strFilePath, CFile::modeWrite);
	f.SeekToEnd();
	//등록된 ID가 없을 경우 파일에 정보 저장
	f.Write(strID+" ", strlen(strID)+1);
	f.Write(strPW+" ", strlen(strPW)+1);
	f.Write(strUserLevel, strlen(strUserLevel));
	f.Close();

	m_strCreateID = strID;
	//ID 정상 등록 성공 메시지 출력 후 화면 종료.
	AfxMessageBox("Register User Success.");
	CDialog::OnCancel();
}
// 
// void CRegisterUserDlg::GetUser()
// {
// 	CString strID = "";
// 	CString strPW = "";
// 	CString strUserLevel = "";
// 	CString strFilePath = "";
// 	CString strUserTemp = "";
// 	CString strUserIDTemp = "";
// 	CFile f;
// 	CFileStatus fs;
// 
// 	strFilePath.Format("%s",m_szFilePath);
// 	//strcpy(strFilePath, "\\st_AccessLevel.txt");
// 	//file 존재하는지 확인
// 	if(!f.Open(strFilePath, CFile::modeWrite))
// 	{
// 		//파일이 없으면 생성
// 		if(!f.Open(strFilePath, CFile::modeCreate))
// 		{
// 			//File Create Error 추가
// 			return;
// 		}
// 	}
// 	f.Close();
// 
// 	CStdioFile sf(strFilePath, CFile::modeRead);
// 	sf.SeekToBegin();
// 	//확인 버튼 클릭 시 등록된 ID가 있는지 비교
// 	int nCount = 0;
// 	while(sf.ReadString(strUserTemp) != NULL)
// 	{
// 		AfxExtractSubString(strUserIDTemp, strUserTemp, 0, ' ');
// 
// 		m_StLoginUser[nCount].nIndex = nCount;
// 		strcpy_s(m_StLoginUser[nCount].szLoginID, sizeof(m_StLoginUser[nCount].szLoginID), (LPSTR)(LPCTSTR)strUserIDTemp);
// 		strcpy_s(m_StLoginUser[nCount].szLoginPW,   sizeof(m_StLoginUser[nCount].szLoginPW), (LPSTR)(LPCTSTR)strUserTemp);
// 	}
// 	sf.Close();
// 	
// }


void CRegisterUserDlg::OnBnClickedButtonRegisterExit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialog::OnCancel();
}

BOOL CRegisterUserDlg::OnInitDialog()
{
	CComboBox *pComboBox = (CComboBox *)GetDlgItem(IDC_COMBO_REGISTER_USER_LEVEL);

	pComboBox->AddString("Operator");
	pComboBox->AddString("Technician");
	pComboBox->AddString("Engineer");
	pComboBox->AddString("Supervisor");
	pComboBox->SetCurSel(0);

	//GetUser();
	
	return TRUE;
}

void CRegisterUserDlg::OnBnClickedBtnRegisterUserDisplay()
{
	CLoginUserDisplayDlg DO(m_szFilePath);
	DO.DoModal();
}