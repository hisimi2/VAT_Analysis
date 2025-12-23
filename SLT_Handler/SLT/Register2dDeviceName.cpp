// Register2dDeviceName.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "Register2dDeviceName.h"
#include "afxdialogex.h"


// CRegister2dDeviceName 대화 상자입니다.

IMPLEMENT_DYNAMIC(CRegister2dDeviceName, CDialogEx)

CRegister2dDeviceName::CRegister2dDeviceName(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_REG_2D_DEVICE_NAME, pParent)
{

}

CRegister2dDeviceName::~CRegister2dDeviceName()
{
}

void CRegister2dDeviceName::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL, m_BtnCancel);
	DDX_Control(pDX, IDC_BTN_DEVICE_NAME_SAVE, m_BtnSave);
}


BEGIN_MESSAGE_MAP(CRegister2dDeviceName, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_DEVICE_NAME_SAVE, &CRegister2dDeviceName::OnBnClickedBtnDeviceNameSave)
END_MESSAGE_MAP()


// CRegister2dDeviceName 메시지 처리기입니다.

BOOL CRegister2dDeviceName::OnInitDialog()
{
	CDialog::OnInitDialog();

	OnBtnGUI();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CRegister2dDeviceName::OnBtnGUI()
{
	m_BtnCancel.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnCancel.SetIcon(IDI_BTN_EXIT);
	m_BtnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnSave.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnSave.SetIcon(IDI_BTN_SAVE);
	m_BtnSave.SetAlign(CButtonST::ST_ALIGN_VERT);
}


void CRegister2dDeviceName::OnBnClickedBtnDeviceNameSave()
{
 	char sz2dMarkPath[2048] = { 0, };
 	char szCurrentDir[512] = { 0, };
 	::GetCurrentDirectory(sizeof(szCurrentDir), szCurrentDir);
 
 	char sz2dDeviceName[MAX_PATH] = { 0, };
 	CString strAddDeviceName = _T("");
 	GetDlgItem(IDC_EDIT_REG_2D_DEVICENAME)->GetWindowText(strAddDeviceName);
	strAddDeviceName.Replace(" ", "");
 	strcpy_s(sz2dDeviceName, sizeof(sz2dDeviceName), strAddDeviceName);
 
	if (strAddDeviceName == _T(""))
		return;


 	char szCokName[MAX_PATH] = { 0, };
 	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));
 	sprintf_s(sz2dMarkPath, sizeof(sz2dMarkPath), "%s\\sys_data\\handlerRecipe\\%s\\%s\\%s\\%s.dat", szCurrentDir, SZ_TRAY_FOLDER, szCokName, SZ_2D_MARK_FOLDER, sz2dDeviceName);
 
 	CFileFind findfile;
 	BOOL bFind = findfile.FindFile(sz2dMarkPath);
 	if (bFind) {
 		CString strErrMsg = _T("");
 		strErrMsg.Format("%s 2d Mark Device Name File is already exist!! ", strAddDeviceName);
 		AfxMessageBox(strErrMsg);
 		MakeLog(strErrMsg);
 		return;
 	}
 
 	ST_2D_MARK_INFO st2dMark;
	sprintf_s(st2dMark.szDeviceName, sizeof(st2dMark.szDeviceName), sz2dDeviceName);

 	g_COK_FileMgr.Save2dMarkInfo(szCokName, &st2dMark);
 
 
 
 
 	MakeLog("[MENU = 2d Mark][ %s Device Name ADD]", sz2dDeviceName);


	CDialogEx::OnCancel();
}


void CRegister2dDeviceName::MakeLog(LPCTSTR fmt, ...)
{
	TCHAR tmpMsg[1024] = { 0, };
	if (fmt)
	{
		va_list argList;
		va_start(argList, fmt);
		_vstprintf(tmpMsg, fmt, argList);
		va_end(argList);
	}
	TWLOG.MakeLog(static_cast<int>(eLogID_System_GUI), Debug, "", __LINE__, NULL, tmpMsg);
}