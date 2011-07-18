// ArticleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ExerciseMain.h"
#include "ArticleWithImageDlg.h"
#include "../ObjRef/YDArticleQuestionRef.h"
#include "../Base/AutoClean.h"


// CArticleDlg dialog

IMPLEMENT_DYNAMIC(CArticleWithImageDlg, CQuestionDlg)

CArticleWithImageDlg::CArticleWithImageDlg(CWnd* pParent /*=NULL*/)
	: CQuestionDlg(CArticleWithImageDlg::IDD, pParent)

{

}

CArticleWithImageDlg::~CArticleWithImageDlg()
{
}

void CArticleWithImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CQuestionDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CArticleWithImageDlg, CQuestionDlg)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CArticleDlg message handlers

BOOL CArticleWithImageDlg::OnInitDialog()
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
	m_rect.right = rect.left + rectSelf.Width();
	m_rect.bottom = rect.top + rectSelf.Height();
	m_Area.MoveWindow(rect.left, rect.top, rectSelf.Width(), rectSelf.Height());
	m_Area.ShowWindow(SW_SHOW);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CArticleWithImageDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
 	if (bShow)
 	{
		if (m_pRecord->m_QTypeID != 8)//Ñ¡´ÊÌî¿Õ
		{
			m_pRecord->m_pQuestion->GetPropVal(L"article", m_strArticle);
		}
		else
		{
			HRESULT hr = m_pRecord->m_pQuestion->GetPropVal(L"ARTICLE", m_strArticle);
			if (FAILED(hr))
			{
				return;
			}
			m_strArticle += L"\r\n";

			std::list<CYDQuestionRef*> lstChildQuestions;
			CListAutoClean<CYDQuestionRef> clean1(lstChildQuestions);
			CYDArticleQuestionRef* pQRef = dynamic_cast<CYDArticleQuestionRef*>(m_pRecord->m_pQuestion);
			hr = pQRef->GetChildQueston(&lstChildQuestions, NULL);
			if (FAILED(hr))
			{
				return;
			}
			std::list<CYDQuestionRef*>::const_iterator itrQ = lstChildQuestions.begin();
			for (; itrQ != lstChildQuestions.end(); ++itrQ)
			{
				CString strQ;
				(*itrQ)->ToString(&strQ);
				m_strArticle += strQ;
				m_strArticle += L"\r\n";
			}
		}
		
 		m_Area.m_pRecord = m_pRecord;
		m_Area.MoveWindow(m_rect, TRUE);
		//m_Area.ShowWindow(SW_SHOW);
		m_Area.ShowQuestion();
		
 		UpdateData(FALSE);
 	}
}

HRESULT CArticleWithImageDlg::PersistData()
{
	m_pRecord->m_listUserAnswers.clear();
	m_Area.GetStuAnswers(&(m_pRecord->m_listUserAnswers));	
	

	return S_OK;
}