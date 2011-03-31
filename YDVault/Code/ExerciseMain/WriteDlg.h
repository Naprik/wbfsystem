#pragma once

#include "resource.h"
#include "afxcmn.h"
#include "../ObjRef/YDQuestionRef.h"

#include "QuestionDlg.h"
// CArticleDlg dialog

class CWriteDlg : public CQuestionDlg
{
	DECLARE_DYNAMIC(CWriteDlg)

public:
	CWriteDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWriteDlg();

// Dialog Data
	enum { IDD = IDD_DLG_WRITE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	HRESULT PersistData();

public:
	CString m_strArticle;
	CString m_strAnswer;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

};
