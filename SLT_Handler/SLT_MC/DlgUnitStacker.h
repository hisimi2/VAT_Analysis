#pragma once


// CDlgUnitStacker 대화 상자입니다.

class CDlgUnitStacker : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgUnitStacker)

public:
	CDlgUnitStacker(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgUnitStacker();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_UNIT_STACKER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
