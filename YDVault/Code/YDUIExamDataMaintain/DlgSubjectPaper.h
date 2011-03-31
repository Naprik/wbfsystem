#pragma once

#include "Resource.h"
#include "afxcmn.h"
#include "../UIBase\BCGPSortListCtrl.h"

// CDlgSubjectPaper dialog
class CYDObjectRef;
class CDlgSubjectPaper : public CDialog
{
	DECLARE_DYNAMIC(CDlgSubjectPaper)

public:
	CDlgSubjectPaper(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSubjectPaper();

// Dialog Data
	enum { IDD = IDD_YDUIEXAM_DATA_MAINTAIN_DLG_SUBJECT_EDITPAPER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual INT_PTR DoModal();
	CBCGPSortListCtrl m_lstAllPaper;
	CBCGPSortListCtrl m_lstSubjectPaper;
	virtual BOOL OnInitDialog();
public:
	CYDObjectRef*	m_pSubject;
	std::list<CYDObjectRef*> m_lstClear;
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDel();
};
