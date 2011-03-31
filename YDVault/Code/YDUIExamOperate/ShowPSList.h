#pragma once

#include "../UIBase/BCGPSortListCtrl.h"
#include "Resource.h"
#include "FormQueryMark.h"
#include "FormPaperAnsys.h"
// CShowPSList dialog

class CShowPSList : public CDialog
{
	DECLARE_DYNAMIC(CShowPSList)

public:
	CShowPSList(CWnd* pParent = NULL);   // standard constructor
	virtual ~CShowPSList();

// Dialog Data
	enum { IDD = IDD_DLG_SHOWLIST_PS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();

public:
	CBCGPSortListCtrl			m_MarkListCtrl;
	std::map<UINT, CPaperAnsysHelperResult*>* m_pMapRes;
	long						m_Total;
};
