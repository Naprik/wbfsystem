#pragma once


// CDlgCfgExamTime dialog

class CDlgCfgExamTime : public CDialog
{
	DECLARE_DYNAMIC(CDlgCfgExamTime)

public:
	CDlgCfgExamTime(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgCfgExamTime();

// Dialog Data
	enum { IDD = IDD_DLG_EM_CFG_EXAM_TIME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	UINT m_uTimeSpan;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
