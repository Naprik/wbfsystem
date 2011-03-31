#pragma once
#include "resource.h"
#include "../UIBase\BCGPKeyGridCtrl.h"
#include <list>

class CYDObjectRef;

// CDlgInputStudentPreview dialog

class CDlgInputStudentPreview : public CDialog
{
	DECLARE_DYNAMIC(CDlgInputStudentPreview)

public:
	CDlgInputStudentPreview(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgInputStudentPreview();

// Dialog Data
	enum { IDD = IDD_YDUIEXAM_DATA_MAINTAIN_DLG_INPUT_STUDENT_PREVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	CBCGPKeyGridCtrl						m_Grid;
public:
	virtual BOOL OnInitDialog();
	std::list<std::list<std::pair<CString,CString> > > m_lstPropVal;
	CYDObjectRef*		m_pClass;		//当前的班级
	virtual INT_PTR DoModal();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedOk();
};
