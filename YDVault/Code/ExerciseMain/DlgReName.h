#pragma once


// CDlgReName dialog

class CDlgReName : public CDialog
{
	DECLARE_DYNAMIC(CDlgReName)

public:
	CDlgReName(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgReName();

// Dialog Data
	enum { IDD = IDD_DIALOG_RENAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_strName;
	virtual BOOL OnInitDialog();
};
