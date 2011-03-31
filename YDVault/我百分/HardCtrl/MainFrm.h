#if !defined(AFX_MAINFRM_H__D9917DB1_C97E_4509_8667_BD4C7AD689F0__INCLUDED_)
#define AFX_MAINFRM_H__D9917DB1_C97E_4509_8667_BD4C7AD689F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "CoolTabCtrl.h"
#include "LeftAudit.h"
#include "LeftReport.h"
#include "LeftManage.h"
#include "FormMain.h"
#include "MenuEx.h"
#include "DlgMsg.h"
#include "FormMenu.h"

class CMainFrame : public CFrameWnd
{	
public: 
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)
public:	
	CMenuEx			m_menu; 
	CFormMain       m_dlgMain;
	CDlgMsg    m_dlgMsg;
	CFormMenu       m_log;
	CCoolTabCtrl	m_TabCtrl;
public:
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
#endif
protected: 	
	CCoolBar        m_wndLeft;                //我们将它作为左边的窗口
	CStatusBar		m_wndStatusBar;
	CCoolBar        m_wndTop; 
	CCoolBar        m_wndBottom;              //停靠在下方的窗口
	CToolBar        m_wndToolBar;
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowleftbar();
	afx_msg void OnShowbottombar();
	afx_msg void OnUpdateShowleftbar(CCmdUI* pCmdUI);
	afx_msg void OnUpdateShowbottombar(CCmdUI* pCmdUI);
	afx_msg void OnMain();
	afx_msg void OnShowExam();
	afx_msg void OnShowDate();
	afx_msg void OnInitMenu(CMenu* pMenu);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP();
private:
	CImageList m_imgList;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__D9917DB1_C97E_4509_8667_BD4C7AD689F0__INCLUDED_)
