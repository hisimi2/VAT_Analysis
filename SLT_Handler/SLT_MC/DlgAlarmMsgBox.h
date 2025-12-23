#pragma once

#include "ParamList.h"
#include <vector>
#include "EditTrans.h"

// CDlgAlarmMsgBox 대화 상자입니다.

class CDlgAlarmMsgBox : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAlarmMsgBox)

public:
	CDlgAlarmMsgBox(ST_ALARM_RPT_OBJ* pstAlarmObj, CParamList* pAlarmData=NULL, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgAlarmMsgBox();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ALARM_MSG};
	enum {
		eUserSelect_OK = 0, eUserSelect_Cancel, eUserSelect_Retry, eUserSelect_Skip, eUserSelect_CleanOut, eUserSelect_PSkip, eUserSelect_TrayEnd, eUserSelect_TrayFeed, eUserSelect_PartialTrayEnd, eUserSelect_SkiptoAssign, eUserSelect_Clear, eUserSelect_AxisMove, eUserSelect_QA_QTY};
private:
	void DisplayErrMsg();
	void UserSelection(int nSelection);
	void InitUserSelectionButton(int nMBTYPE, int nAlid);
	int ConvertUserSelection(CString strUserSelection);
	void OnBnClickedButtonUserSel(int nButtonIdx);
	void InitControls();
	void InitSpread();
	void InitReSize();
	void InitPosition();
	BOOL UserSelect(int nAlid, int nSelection);
	void LoadTableErrorPocketNumberSet(_eCw_CCw bCW, CSpreadSheet* SprdPocket);

	CParamList m_AlarmData; // 부가적인 정보를 "data name1=value1;data name2=value2"

	CBaseTaskManager* m_pSender;
	int m_nAlid;
	_tERROR_MSG m_tErrorMsg;
	_tERROR_LOG m_tErrorLog;
	CString m_strMessageSysLang;
	CString m_strMessageEng;
	std::vector<int> m_vUserSelectButtonCtrl;
	std::vector<int> m_vMessageBoxId;
	std::vector<CPoint> m_vFailPicker;
	CSpreadSheet m_SprdPicker;
	int m_nPosition;
	CStatic m_bitPic1;
	CEditTrans m_editPosition[ePos_MaxPosition];
	BOOL bIsRed;
	BOOL m_bDuplicatealarm;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedOk();
	virtual void PostNcDestroy();
	afx_msg void OnBnClickedButtonUserSel1();
	afx_msg void OnBnClickedButtonUserSel2();
	afx_msg void OnBnClickedButtonUserSel3();
	afx_msg void OnBnClickedButtonUserSel4();
	afx_msg void OnBnClickedButtonSetMbtype();
	afx_msg void OnPaint();

	BOOL IsRightCompany(char * pCompany);
	UINT m_nProcessState;

};
