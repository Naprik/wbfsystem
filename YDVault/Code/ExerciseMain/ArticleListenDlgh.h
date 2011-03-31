#pragma once
#include "resource.h"
#include "afxcmn.h"
#include "../ObjRef/YDQuestionRef.h"

#include "QuestionDlg.h"
#include "DlgQuestionArea.h"
#include "wmpplayer4.h"
// CListenDlg dialog

class CArticleListenDlg : public CQuestionDlg
{
	DECLARE_DYNAMIC(CArticleListenDlg)

public:
	CArticleListenDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CArticleListenDlg();

// Dialog Data
	enum { IDD = IDD_DLG_LISTEN_ARTICLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	HRESULT PersistData();
	HRESULT PlayMedia();
	HRESULT PauseMedia();
	HRESULT StopMedia();

public:
	CString m_strArticle;

private:
	CDlgQuestionArea	m_Area;
	CWMPPlayer4	    m_WMPlay;
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
