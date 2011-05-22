#pragma once

#include "resource.h"
#include "../YDFormUIBase/YdFormView.h"


// CHistoryInfoView form view

class CHistoryInfoView : public CYdFormView
{
	DECLARE_DYNCREATE(CHistoryInfoView)

protected:
	CHistoryInfoView();           // protected constructor used by dynamic creation
	virtual ~CHistoryInfoView();

public:
	enum { IDD = IDD_DLG_PERSIONINFO_HISTORY };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};


