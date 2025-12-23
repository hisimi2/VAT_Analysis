// RegisterDeviceName.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "RegisterDeviceName.h"
#include "afxdialogex.h"


// CRegisterDeviceName 대화 상자입니다.

IMPLEMENT_DYNAMIC(CRegisterDeviceName, CDialog)

CRegisterDeviceName::CRegisterDeviceName(CWnd* pParent /*=NULL*/)
	: CDialog(CRegisterDeviceName::IDD, pParent)
{

}

CRegisterDeviceName::~CRegisterDeviceName()
{
}

void CRegisterDeviceName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL, m_BtnCancel);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_BtnSave);
}


BEGIN_MESSAGE_MAP(CRegisterDeviceName, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CRegisterDeviceName::OnBnClickedButtonSave)
END_MESSAGE_MAP()


// CRegisterDeviceName 메시지 처리기입니다.


BOOL CRegisterDeviceName::OnInitDialog()
{
	CDialog::OnInitDialog();

	OnBtnGUI();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CRegisterDeviceName::OnBtnGUI()
{
	m_BtnCancel.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnCancel.SetIcon(IDI_BTN_EXIT);
	m_BtnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnSave.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnSave.SetIcon(IDI_BTN_SAVE);
	m_BtnSave.SetAlign(CButtonST::ST_ALIGN_VERT);
}


void CRegisterDeviceName::OnBnClickedButtonSave()
{
    int nRet = AfxMessageBox("Do you wan's save?", MB_TOPMOST | MB_YESNO);
    if(nRet != IDYES){
        return;
    }

	char szCurrentDir[512] = {0,};
	char szFileName[512]   = {0,};
	char szData[2048]      = {0,};
	char szSaveData[2048]  = {0,};
	::GetCurrentDirectory(sizeof(szCurrentDir), szCurrentDir);
	sprintf_s(szFileName, sizeof(szFileName), "%s\\sys_data\\%s", szCurrentDir, SZ_CUST_DVC_PGM_FILE_NAME);

	CConfigData		CRegisterDVC(szFileName);
	int nCount = CRegisterDVC.GetInt("DEVICE" , "COUNT", "0");	// 등록 된 Count를 가져온다.

	CString strGetRegData = _T("");
	GetDlgItem(IDC_EDIT_REG_DEVICENAME)->GetWindowText(strGetRegData);

	strcpy_s(szData, sizeof(szData), strGetRegData);
	char szSubTitle[256] = {0,};
	sprintf_s(szSubTitle, sizeof(szSubTitle), "NAME%03d",nCount+1);

	CRegisterDVC.Set("DEVICE" , szSubTitle, szData);
	CRegisterDVC.Set("DEVICE" , "COUNT", nCount+1);

	MakeLog("[MENU = Register Device Name ][ %s Device Name ADD ]", szData);
}


void CRegisterDeviceName::MakeLog(LPCTSTR fmt, ...)
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