#pragma once

#include "resource.h"
#include "afxcmn.h"
#include "../ObjRef/YDQuestionRef.h"

#include "QuestionDlg.h"
#include "DlgQuestionArea.h"
// CArticleDlg dialog

class CArticleDlg : public CQuestionDlg
{
	DECLARE_DYNAMIC(CArticleDlg)

public:
	CArticleDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CArticleDlg();

// Dialog Data
	enum { IDD = IDD_DLG_ARTICLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	HRESULT PersistData();

public:
	CString m_strArticle;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

private:
	CDlgQuestionArea	m_Area;
	CRect				m_rect;
};
