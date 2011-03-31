#pragma once

#include "resource.h"
#include "afxcmn.h"
#include "../ObjRef/YDQuestionRef.h"

#include "QuestionDlg.h"
#include "DlgQuestionArea.h"
// CArticleDlg dialog

class CChoiceDlg : public CQuestionDlg
{
	DECLARE_DYNAMIC(CChoiceDlg)

public:
	CChoiceDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CChoiceDlg();

// Dialog Data
	enum { IDD = IDD_DLG_CHOICE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	HRESULT PersistData();

public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

private:
	CDlgQuestionArea	m_Area;
	CRect				m_rect;
};
