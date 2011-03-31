#if !defined(AFX_FORMGRAPHDATA_H__5BA42C83_9F26_431E_A9BC_123626E41479__INCLUDED_)
#define AFX_FORMGRAPHDATA_H__5BA42C83_9F26_431E_A9BC_123626E41479__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "FileGraph.h"
#include "Graph.h"
// FormGraphData.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormGraphData dialog

class CFormGraphData : public CDialog
{
// Construction
public:
	void DrawGraph(CFileGraph *data);	
	CFormGraphData(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFormGraphData)
	enum { IDD = IDD_FORM_GRAPH };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormGraphData)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFormGraphData)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void SetData();
	void SetMem();
	void SetText();
	CGraph* InitGraph();
private:
	BOOL IsPie();
	CFileGraph   *m_data;
    CGraph       *m_graph;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMGRAPHDATA_H__5BA42C83_9F26_431E_A9BC_123626E41479__INCLUDED_)
