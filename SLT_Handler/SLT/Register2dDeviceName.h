#pragma once


// CRegister2dDeviceName 대화 상자입니다.

class CRegister2dDeviceName : public CDialogEx
{
	DECLARE_DYNAMIC(CRegister2dDeviceName)

public:
	CRegister2dDeviceName(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CRegister2dDeviceName();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_REG_2D_DEVICE_NAME };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

private:

	CShadeButtonST m_BtnCancel;
	CShadeButtonST m_BtnSave;

	void OnBtnGUI();
public:
	afx_msg void OnBnClickedBtnDeviceNameSave();
	void MakeLog(LPCTSTR fmt, ...);
};
