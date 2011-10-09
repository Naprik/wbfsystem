#pragma once
#include "afxcmn.h"
#include "../UIBase/BCGPSortListCtrl.h"

// CSelLogDlg dialog

class CSelLogDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelLogDlg)

public:
	CSelLogDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelLogDlg();

// Dialog Data
	enum { IDD = IDD_DLG_SEL_LOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CBCGPSortListCtrl m_listLog;
	CString			m_strFilename;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
