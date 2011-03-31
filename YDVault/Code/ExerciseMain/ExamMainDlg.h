#pragma once
#include "afxwin.h"
#include "BCGPButton.h"
#include "resource.h"
// CExamMainDlg dialog

class CExamMainDlg : public CDialog
{
	DECLARE_DYNAMIC(CExamMainDlg)

public:
	CExamMainDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CExamMainDlg();

// Dialog Data
	enum { IDD = IDD_DLG_EM_EXAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CBCGPButton m_btnAbort;
	CBCGPButton m_btnBegin;
	CBCGPButton m_btnPrev;
	CBCGPButton m_btnNext;
	CBCGPButton m_btnStop;
	CBCGPButton m_btnStdAnswer;
	CBCGPButton m_btnStuAnswer;
	CBCGPButton m_btnFinish;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnEmExamStdanswer();
};
