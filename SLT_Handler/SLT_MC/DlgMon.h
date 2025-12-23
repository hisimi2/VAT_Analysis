#pragma once

#include "DlgMonOverview.h"

// CDlgMon 대화 상자입니다.

enum eMonDlgRect {
	eMonDlgRect_Transfer = 0,
	eMonDlgRect_TrayPP,
	eMonDlgRect_LoadTbl1,
	eMonDlgRect_LoadTbl2,
	eMonDlgRect_TestPP1,
	eMonDlgRect_TestPP2,
	eMonDlgRect_Press1,
	eMonDlgRect_Press2,
	eMonDlgRect_Press3,
	eMonDlgRect_Press4,
	eMonDlgRect_Press5,
	eMonDlgRect_Press6,
	eMonDlgRect_Press7,
	eMonDlgRect_Press8,
	eMonDlgRect_Press9,
	eMonDlgRect_Press10,
	eMonDlgRect_Press11,
	eMonDlgRect_Press12,
	eMonDlgRect_Press13,
	eMonDlgRect_Press14,
	eMonDlgRect_Press15,
	eMonDlgRect_Press16,
	eMonDlgRectMax,
};

typedef struct _ST_MOTOR_DISPLAY
{
	double MoveValue; //실제 이동 값
	double MovePos;	//실제 이동위치
	double CurrentPos;	//현재 위치...
	double MoveTime;	//이동 시간...
	double speed;		//속도..

	int DisplayPos;	//현재의 화면 이동 위치 
	int DisplayValue;	//이동해야할 화면값

	int NowValue; //현재 화면 이동 값...

	int MoveOneValue;
	int while_flag;	//movewhile or move?
	int m_ActMotor;	// Is Motor Acting?
	int count;	//Display count
	int set;	//value setting
}ST_MOTOR_DISPLAY;


class CDlgMon : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMon)

public:
	CDlgMon(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgMon();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MON_OVERVIEW/*IDD_MON*/ };
	//enum { eSubView_OverView=0, eMaxSubViewCount };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();

private:

	CString m_Msg;
	CBitmap m_background;
	CRect m_bgRect;

	void GetBitmapSize(CRect *prect, CBitmap *pbitmap);
	void DrawDoalog();
	void GetMotorEncoder();

	CBitmap m_BtnImg[eMonDlgRectMax];
	CRect   m_ImgRect[eMonDlgRectMax];

	CPoint m_cPosOffSet[eMonDlgRectMax];
	double m_dTestHandOffSetZ[2];

	int m_nX;
	int m_nY;

	CDC m_memDC;

	static UINT DisplayUI(LPVOID lpVoid); 
	CWinThread* m_pThreadP1;
};
