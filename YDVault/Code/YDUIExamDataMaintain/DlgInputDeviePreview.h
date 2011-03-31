#pragma once

#include "Resource.h"
#include "../UIBase\BCGPKeyGridCtrl.h"
// CDlgInputDeviePreview dialog

class CDlgInputDeviePreview : public CDialog
{
	DECLARE_DYNAMIC(CDlgInputDeviePreview)

public:
	CDlgInputDeviePreview(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgInputDeviePreview();

// Dialog Data
	enum { IDD = IDD_YDUIEXAM_DATA_MAINTAIN_DLG_INPUT_DEVICE_PREVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	CBCGPKeyGridCtrl						m_Grid;
public:
	std::list<std::list<std::pair<CString,CString> > > m_lstPropVal;
	virtual INT_PTR DoModal();
	virtual BOOL OnInitDialog();
protected:
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDel();
};
