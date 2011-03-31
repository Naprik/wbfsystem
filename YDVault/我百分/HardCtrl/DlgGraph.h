#if !defined(AFX_DLGGRAPH_H__9BC5E582_56D4_445D_B473_95A1F96A691B__INCLUDED_)
#define AFX_DLGGRAPH_H__9BC5E582_56D4_445D_B473_95A1F96A691B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "FormGraphData.h"
// DlgGraph.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgGraph dialog

class CDlgGraph : public CDialog
{
// Construction
public:
	void ShowGraph();	
	CDlgGraph(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgGraph)
	enum { IDD = IDD_DIG_GRAPH };
	CComboBox	m_type;
	BOOL	m_isLine;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgGraph)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	HICON m_hIcon;
	// Generated message map functions
	//{{AFX_MSG(CDlgGraph)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLine();
	afx_msg void OnSelchangeType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CFormGraphData  m_graph;
	CFileGraph  m_graphData;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGRAPH_H__9BC5E582_56D4_445D_B473_95A1F96A691B__INCLUDED_)
