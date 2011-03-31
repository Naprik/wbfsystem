#pragma once

#include "mschart_paperansys.h"

#include "FormPaperAnsys.h"
#include "Resource.h"
// CShowPSChart dialog

class CShowPSChart : public CDialog
{
	DECLARE_DYNAMIC(CShowPSChart)

public:
	CShowPSChart(CWnd* pParent = NULL);   // standard constructor
	virtual ~CShowPSChart();

// Dialog Data
	enum { IDD = IDD_YDUIEXAM_DLGSHOWCHART_PS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	CMschart_paperansys		m_chart;
	std::map<UINT, CPaperAnsysHelperResult*>* m_pMapRes;
	long						m_Total;
};
