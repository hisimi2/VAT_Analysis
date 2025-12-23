#pragma once


// CDlgSelectItem 대화 상자입니다.
class CDlgSelectItem : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSelectItem)
	typedef std::vector<CString> ItemArray;
	typedef std::vector<CWnd*> BtnArray;
	ItemArray	vecItems;
	BtnArray	vecBtns;
	int selected_index;
public:
	CDlgSelectItem(CWnd* pParent, ItemArray items);
	virtual ~CDlgSelectItem();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_SELECT_ITEM };

	CString GetValue();
	int GetSelIndex();

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	void OnBtnClick(HWND sender);
	afx_msg void OnClose();
public:
	afx_msg void OnBnClickedCancel();
};
