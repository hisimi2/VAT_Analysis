#pragma once
#include "afxcmn.h"
#include "ConectSocket.h"


// CDlgDisplay 대화 상자입니다.

class CDlgDisplay : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgDisplay)

public:
	CDlgDisplay(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgDisplay();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TesterInfo
	};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void showTesterInfo(CConectSocket* sock ,  int nActionFlag);

	CString  getStrStationNo(CConectSocket* sock);
	CString  getStrState(CConectSocket* sock);
	CString  getStrActMode(CConectSocket* sock);
	CString  getStrIp(CConectSocket* sock);
	CString  getStrSocketId(CConectSocket* sock , int nI);
	CString  getStrVersion(CConectSocket* sock);

	CListCtrl m_ListCtrl;

};
