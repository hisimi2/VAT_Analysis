
// MainFrm.h : CMainFrame 클래스의 인터페이스
//

#pragma once


//==============================================================================
//
//==============================================================================
#define ID_TRAYICON_NOTIFY				(WM_USER+100)

#define GUI_SIZE_X						(0)
#define GUI_SIZE_Y						(124)
#define GUI_SIZE_W						(1280)
#define GUI_SIZE_H						(900)


//==============================================================================
//
//==============================================================================
#define UM_ALARM_REPORT                  (WM_USER + 9000)
//==============================================================================
//
//==============================================================================
class CMainFrame : public CFrameWnd
{
	CDlgHomeCheckStatus*      m_pDlgHomeCheckStatus;
	BOOL		m_bManualCmdExcute;
protected: // serialization에서만 만들어집니다.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 특성입니다.
public:
	NOTIFYICONDATA m_objTaskTrayIcon;
	void           MoveToTrayIcon();
	void           ReleaseTrayIcon();

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 컨트롤 모음이 포함된 멤버입니다.
	CStatusBar        m_wndStatusBar;

private:
// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnClose();
	afx_msg LRESULT OnReceivedAlarm(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnGuiEvent(WPARAM wParma, LPARAM lParam);
    afx_msg LRESULT OnGuiEventClose(WPARAM wParma, LPARAM lParam);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg LRESULT OnMultiOriginEvent(WPARAM wParma, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


