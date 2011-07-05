
// DBUpdateDlg.h : header file
//

#pragma once
#include "afxwin.h"
class CDatabaseEx;


// CDBUpdateDlg dialog
class CDBUpdateDlg : public CDialogEx
{
// Construction
public:
	CDBUpdateDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_DBUPDATE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonUpdateDb();
	CListBox m_lstContent;
	CDatabaseEx* m_pDatabase;
private:
	BOOL ExecuteSQL(CString _strSQL,CString &_strError);
};
