#pragma once


// CTrayViewStacker 대화 상자입니다.

class CTrayViewStacker : public CDialogEx
{
	DECLARE_DYNAMIC(CTrayViewStacker)
	enum eTrayViewStacker
	{
		eViewTrayFeeder =0,
		eViewTransfer,
		eViewTransferArea,
		eMaxViewStacker,
	};
public:
	CTrayViewStacker(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTrayViewStacker();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TRAYVIEW_STACKER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void UpdateStatus();
	void SendMcComm();
public:
	int m_nFrontConveyorExistCtrlId[eMaxStackerCount];
	int m_nFrontConveyorDataCtrlId[eMaxStackerCount];
	int m_nRearConveyorExistCtrlId[eMaxStackerCount];
	int m_nRearConveyorDataCtrlId[eMaxStackerCount];
// 	int m_nFeederExistCtrlId;
// 	int m_nFeederDataCtrlId;
// 	int m_nTransferExistCtrlId;
// 	int m_nTransferDataCtrlId;

	CEditTrans m_editFrontConveyorExist[eMaxStackerCount];
	CEditTrans m_editFrontConveyorData[eMaxStackerCount];
	CEditTrans m_editRearConveyorExist[eMaxStackerCount];
	CEditTrans m_editRearConveyorData[eMaxStackerCount];
	CEditTrans m_editTransferAreaExist;
	CEditTrans m_editTransferAreaData;
	CEditTrans m_editFeederExist;
	CEditTrans m_editFeederData;
	CEditTrans m_editTransferExist;
	CEditTrans m_editTransferData;

	COLORREF beforeStacker_F_Sen_color[eMaxStackerCount];
	COLORREF beforeStacker_R_Sen_color[eMaxStackerCount];
	COLORREF beforeStacker_F_Data_color[eMaxStackerCount];
	COLORREF beforeStacker_R_Data_color[eMaxStackerCount];

	COLORREF beforeViewStacker_Sen_color[eMaxViewStacker];
	COLORREF beforeViewStacker_Data_color[eMaxViewStacker];

};
