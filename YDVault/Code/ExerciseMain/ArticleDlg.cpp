// ArticleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ExerciseMain.h"
#include "ArticleDlg.h"
#include "../ObjRef/YDArticleQuestionRef.h"
#include "../Base/AutoClean.h"


// CArticleDlg dialog

IMPLEMENT_DYNAMIC(CArticleDlg, CQuestionDlg)

CArticleDlg::CArticleDlg(CWnd* pParent /*=NULL*/)
	: CQuestionDlg(CArticleDlg::IDD, pParent)
	, m_strArticle(_T(""))
{

}

CArticleDlg::~CArticleDlg()
{
}

void CArticleDlg::DoDataExchange(CDataExchange* pDX)
{
	CQuestionDlg::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_RICHEDIT_ARTICLE, m_strArticle);
}


BEGIN_MESSAGE_MAP(CArticleDlg, CQuestionDlg)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CArticleDlg message handlers

BOOL CArticleDlg::OnInitDialog()
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

void CArticleDlg::OnShowWindow(BOOL bShow, UINT nStatus)
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

HRESULT CArticleDlg::PersistData()
{
	m_pRecord->m_listUserAnswers.clear();
	m_Area.GetStuAnswers(&(m_pRecord->m_listUserAnswers));	
	

	return S_OK;
}