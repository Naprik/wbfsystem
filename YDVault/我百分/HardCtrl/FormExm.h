//{{AFX_INCLUDES()
#include "shockwaveflash.h"
//}}AFX_INCLUDES
#if !defined(AFX_FORMEXM_H__9F2B2E8A_E202_41A0_9F89_B1F9AF548E96__INCLUDED_)
#define AFX_FORMEXM_H__9F2B2E8A_E202_41A0_9F89_B1F9AF548E96__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "cdxCSizingDialog.h"
#include "XPButton.h"
#include "DBStuInfo.h"
#include "BtnST.h"
#include "ShadeButtonST.h"
#include "ColorStatic.h"

class CFormExm : public cdxCSizingDialog
{
// Construction
public:

	void SetKind(int kind);
	void Show();
	COleDateTime 	m_starttime;
	COleDateTime 	m_endtime;
	CString			m_papername;
	CString			m_subject;
	CString			m_depart;
	CString         m_class;
	CString		    m_address;
	CString		    m_totalnum;
	int				m_startno;
	int				m_endno;
	int				m_maxno;
	int				m_account;
	int				m_paperid;
	BOOL			isSearch;
	BOOL		    isRoll;
	int				m_iTemp;
	CString			m_strTemp;
	CStringArray    m_classarry;
	CFormExm(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CFormExm)
	BOOL            bIsRoll;
	BOOL			bIsRolling;

	//{{AFX_DATA(CFormExm)
	enum { IDD = IDD_FORM_EXAM };
	CStatic		m_remind;
	CShadeButtonST	m_speroll;
	CShadeButtonST	m_late;
	CShadeButtonST	m_prerolling;
	CShadeButtonST	m_linkrolling;
	CShadeButtonST	m_linkroll;
	CShadeButtonST	m_dall;
	CShadeButtonST	m_all;
	CShadeButtonST	m_search;
	CShadeButtonST	m_setting;
	CShadeButtonST	m_start;
	CShadeButtonST	m_rolling;
	CShadeButtonST	m_roll;
	CSortListCtrl	m_DevList;
	CString	m_systime;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormExm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
protected:
	//{{AFX_MSG(CFormExm)
	virtual BOOL OnInitDialog();
	afx_msg void OnRoll();
	afx_msg void OnSetting();
	afx_msg void OnSearch();
	afx_msg void OnStart();
	afx_msg void OnRolling();
	afx_msg void OnDblclkDevlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnAll();
	afx_msg void OnDall();
	afx_msg void OnLate();
	afx_msg void OnPrerolling();
	afx_msg void OnSperoll();
	afx_msg void OnLinkroll();
	afx_msg void OnLinkrolling();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CToolBar * m_pToolBar;
	int			m_kind;
	CString  GetExePath();
	CColorStatic m_Static1;
	CColorStatic m_Static2;
	CColorStatic m_Static3;
	CColorStatic m_Static4;
	CColorStatic m_Static5;
	CColorStatic m_Static6;
	CColorStatic m_Static7;
	CColorStatic m_Static8;
	CColorStatic m_Static9;
	CColorStatic m_Static10;
	CColorStatic m_Static11;
	CColorStatic m_Static12;
	CColorStatic m_Static13;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMEXM_H__9F2B2E8A_E202_41A0_9F89_B1F9AF548E96__INCLUDED_)
