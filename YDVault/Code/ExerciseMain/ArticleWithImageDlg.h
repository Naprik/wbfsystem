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
	HBITMAP BufferToHBITMAP();
	HRESULT PersistData();

public:
	CString m_strArticle;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	afx_msg void OnPaint();
	afx_msg void OnShow();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

private:
	CDlgQuestionArea	m_Area;
	CRect				m_rect;
	CRect				rectStaticClient;
	SCROLLINFO			horz,vert;
	BOOL	flag, erase;
	int sourcex, sourcey,offsetx,offsety;
	CStatic	m_st1;
	CScrollBar	m_vbar;
	CScrollBar	m_hbar;

	CDC m_dcMem;            // Compatible Memory DC for dialog
	
	HBITMAP			m_hPhotoBitmap;
	char			*m_pBMPBuffer;
	DWORD			m_dwFileLen;
	BITMAP m_bmInfo;        // Bitmap Information structure
	CPoint m_pt;            // Position for upper left corner of bitmap
	CSize m_size;  
};
