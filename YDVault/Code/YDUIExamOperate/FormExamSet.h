#if !defined(AFX_FORMEXAMSET_H__771C9EE8_6BE7_4674_9FF4_C4D237FDB75B__INCLUDED_)
#define AFX_FORMEXAMSET_H__771C9EE8_6BE7_4674_9FF4_C4D237FDB75B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Resource.h"
#include "../YDFormUIBase\YdFormView.h"
#include "../UIBase/XPButton.h"
#include "../UIBase/ControlButton.h"
#include "../UIBase\SortListCtrl.h"
// FormExamSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormExamSet dialog

class CFormExamSet : public CYdFormView
{
// Construction
public:
	CFormExamSet();   // standard constructor
	virtual ~CFormExamSet();
	DECLARE_DYNCREATE(CFormExamSet)

	void SetKind(int kind);
	int m_kind;    //进入item的编号，来自mainfram
// Dialog Data
	//{{AFX_DATA(CFormExamSet)
	enum { IDD = IDD_YDUIEXAM_FORM_EXAMSET };
	CControlButton	m_set;
	CControlButton	m_up;
	CControlButton	m_showall;
	CControlButton	m_paperquery;
	CSortListCtrl	m_paperlist;
	CControlButton	m_next;
	CSortListCtrl	m_classlist;
	CComboBox	m_address;
	CControlButton	m_addpaper;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormExamSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual BOOL Create(LPCTSTR, LPCTSTR, DWORD,
		const RECT&, CWnd*, UINT, CCreateContext*);
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFormExamSet)
	virtual void OnInitialUpdate();
	afx_msg void OnUp();
	afx_msg void OnShowall();
	afx_msg void OnPaperquery();
	afx_msg void OnAddpaper();
	afx_msg void OnNext();
	afx_msg void OnDblclkPaperlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkClasslist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSet();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	std::list<CYDObjectRef*> m_lstClass;
	std::list<CYDObjectRef*> m_lstExamAddr;
	std::list<CYDObjectRef*> m_lstPaperRef;
	HRESULT Clear();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMEXAMSET_H__771C9EE8_6BE7_4674_9FF4_C4D237FDB75B__INCLUDED_)
