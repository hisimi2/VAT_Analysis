#pragma once
#include "afxwin.h"


// CDeviceSetup 대화 상자입니다.

class CDeviceSetup : public CDialog
{
	DECLARE_DYNAMIC(CDeviceSetup)

public:
	CDeviceSetup(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDeviceSetup();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_DEVICE_SETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_strTrayFileName;

private:
	CShadeButtonST m_BtnCancel;
	CShadeButtonST m_BtnExport;
	CShadeButtonST m_BtnImport;
	CShadeButtonST m_BtnSave;

	CListBox m_ListCustomer;
	CListBox m_ListDvcName;
	CListBox m_ListPgmName;
	CListBox m_ListTrayFile;
	CListBox m_ListSLTPgm;
	CListBox m_ListCategory;
	CString  m_strTrayFileBak;
	CString  m_strSltPgmBak;
	CString  m_strCategoryBack;

	CParamList m_cParamList;
	char m_szCustomer[16];

	void OnBtnGUI();
	void LoadCustomer();
	void LoadDeviceName();
	void LoadProgramName();
	void LoadTrayFile();
	void LoadSltPgm();
	void LoadCategory();
	void InitalizeDialog_Amkor();
	void ChangeDialogSize(std::vector<int> vControl);
	void LotInfoDataClear();
	int CopyFolder(CString strFrom, CString strTo);
	int ChkCopyFolder(CString strFromPath);
	int ChkCopyFolderName(CString strFromPath,CString &strFolderName);

public:
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonImpo();
	afx_msg void OnBnClickedButtonExpo();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonRegCustom();

	afx_msg void OnBnClickedButtonRegDvc();
	afx_msg void OnBnClickedButtonRegPgm();
//	afx_msg void OnBnClickedButtonCustomModify();

	void MakeLog(LPCTSTR fmt, ...);
	void DeviceSetupChangeLog(std::vector<std::pair<CString,std::pair<CString,CString>>> vData);
};
