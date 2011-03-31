#pragma once
#include "mschart_paperansys.h"
#include "Resource.h"

// CShowChartDlg dialog
class CQueryMarkInfo;
class CShowChartDlg : public CDialog
{
	DECLARE_DYNAMIC(CShowChartDlg)

public:
	CShowChartDlg(BOOL bSearch, CWnd* pParent = NULL);   // standard constructor
	virtual ~CShowChartDlg();

// Dialog Data
	enum { IDD = IDD_YDUIEXAM_DLGSHOWCHART };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	
	virtual BOOL OnInitDialog();
private:
	HRESULT Draw(BOOL bSearch);

public:
	std::list<CQueryMarkInfo*>*	m_pLstQueryMarkInfo;
	CMschart_paperansys		m_chart;
	BOOL					m_bFlag;
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
