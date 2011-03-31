#pragma once
#include "../UIBase/BCGPSortListCtrl.h"
#include "Resource.h"
#include "FormQueryMark.h"
// CShowListDlg dialog

class CShowListDlg : public CDialog
{
	DECLARE_DYNAMIC(CShowListDlg)

public:
	CShowListDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CShowListDlg();

// Dialog Data
	enum { IDD = IDD_DLG_SHOWLIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	std::list<CQueryMarkInfo*>*	m_pLstQueryMarkInfo;

private:
	CBCGPSortListCtrl			m_UnitList;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
