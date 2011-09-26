// ArticleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ExerciseMain.h"
#include "ChoiceDlg.h"
#include "../ObjRef/YDArticleQuestionRef.h"
#include "../Base/AutoClean.h"


// CArticleDlg dialog

IMPLEMENT_DYNAMIC(CChoiceDlg, CQuestionDlg)

CChoiceDlg::CChoiceDlg(CWnd* pParent /*=NULL*/)
	: CQuestionDlg(CChoiceDlg::IDD, pParent)
{

}

CChoiceDlg::~CChoiceDlg()
{
}

void CChoiceDlg::DoDataExchange(CDataExchange* pDX)
{
	CQuestionDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CChoiceDlg, CQuestionDlg)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CArticleDlg message handlers

BOOL CChoiceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Area.Create(CDlgQuestionArea::IDD, this);
	CRect rect, rectSelf;
	CWnd* pTmpWnd = GetDlgItem(IDC_STATIC_QUESTION);
	pTmpWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	m_Area.GetWindowRect(&rectSelf);
	m_rect.left = rect.left;
	m_rect.top = rect.top;
	m_rect.right = rect.left + rect.Width();
	m_rect.bottom = rect.top + rect.Height();
	m_Area.MoveWindow(rect.left, rect.top, rect.Width(), rect.Height());
	m_Area.ShowWindow(SW_SHOW);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CChoiceDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
 	if (bShow)
 	{
 		m_Area.m_pRecord = m_pRecord;
		m_Area.MoveWindow(m_rect, TRUE);
		//m_Area.ShowWindow(SW_SHOW);
		m_Area.ShowQuestion();
		
 		UpdateData(FALSE);
 	}
}

HRESULT CChoiceDlg::PersistData()
{
	m_pRecord->m_listUserAnswers.clear();
	m_Area.GetStuAnswers(&(m_pRecord->m_listUserAnswers));	
	return S_OK;
}