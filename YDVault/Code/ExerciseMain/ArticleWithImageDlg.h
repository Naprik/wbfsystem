#pragma once

#include "resource.h"
#include "afxcmn.h"
#include "../ObjRef/YDQuestionRef.h"

#include "QuestionDlg.h"
#include "DlgQuestionArea.h"
// CArticleDlg dialog

class CArticleWithImageDlg : public CQuestionDlg
{
	DECLARE_DYNAMIC(CArticleWithImageDlg)

public:
	CArticleWithImageDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CArticleWithImageDlg();

// Dialog Data
	enum { IDD = IDD_DLG_ARTICLE_IMAGE };

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
