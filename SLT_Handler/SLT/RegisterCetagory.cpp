// CRegisterCategory.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "RegisterCetagory.h"
#include "afxdialogex.h"


// CRegisterCategory 대화 상자입니다.

IMPLEMENT_DYNAMIC(CRegisterCategory, CDialog)

CRegisterCategory::CRegisterCategory(CWnd* pParent /*=NULL*/)
	: CDialog(CRegisterCategory::IDD, pParent)
{

}

CRegisterCategory::~CRegisterCategory()
{
}

void CRegisterCategory::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL, m_BtnCancel);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_BtnSave);
}


BEGIN_MESSAGE_MAP(CRegisterCategory, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CRegisterCategory::OnBnClickedButtonSave)
END_MESSAGE_MAP()


// CRegisterCategory 메시지 처리기입니다.


BOOL CRegisterCategory::OnInitDialog()
{
	CDialog::OnInitDialog();

	OnBtnGUI();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CRegisterCategory::OnBtnGUI()
{
	m_BtnCancel.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnCancel.SetIcon(IDI_BTN_EXIT);
	m_BtnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnSave.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnSave.SetIcon(IDI_BTN_SAVE);
	m_BtnSave.SetAlign(CButtonST::ST_ALIGN_VERT);
}


void CRegisterCategory::OnBnClickedButtonSave()
{
	char szCategoryPath[2048] = {0,};
	char szCurrentDir[512]={0,};
	::GetCurrentDirectory(sizeof(szCurrentDir), szCurrentDir);

	char szData[2048]      = {0,};
	CString strGetRegData = _T("");
	GetDlgItem(IDC_EDIT_REG_DEVICENAME)->GetWindowText(strGetRegData);

	strGetRegData.Replace(" ","");

	strcpy_s(szData, sizeof(szData), strGetRegData);
	
	sprintf_s( szCategoryPath, sizeof( szCategoryPath ), "%s\\sys_data\\handlerRecipe\\%s\\%s.csv",szCurrentDir, SZ_CATEGORY_FOLDER, szData );

	CFileFind findfile;
	BOOL bFind =findfile.FindFile(szCategoryPath);
	if(bFind){
		CString strErrMsg = _T("");
		strErrMsg.Format("%s Category File is already exist!! ",strGetRegData);
		AfxMessageBox(strErrMsg);
		MakeLog(strErrMsg);
		return;
	}

	ST_CATE_INFO stCategory;

	g_COK_FileMgr.SaveCategoryInfo(szData,&stCategory);

	MakeLog("[MENU = Cetagory ][ %s Category Name ADD ]", szData);
}


void CRegisterCategory::MakeLog(LPCTSTR fmt, ...)
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