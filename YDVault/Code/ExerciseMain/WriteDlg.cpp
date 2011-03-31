// ArticleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ExerciseMain.h"
#include "WriteDlg.h"
#include "../ObjRef/YDArticleQuestionRef.h"
#include "../Base/AutoClean.h"


// CArticleDlg dialog

IMPLEMENT_DYNAMIC(CWriteDlg, CQuestionDlg)

CWriteDlg::CWriteDlg(CWnd* pParent /*=NULL*/)
	: CQuestionDlg(CWriteDlg::IDD, pParent)
	, m_strArticle(_T(""))
{

}

CWriteDlg::~CWriteDlg()
{
}

void CWriteDlg::DoDataExchange(CDataExchange* pDX)
{
	CQuestionDlg::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_RICHEDIT_ARTICLE, m_strArticle);
	DDX_Text(pDX, IDC_RICHEDIT_ANSWER, m_strAnswer);
}


BEGIN_MESSAGE_MAP(CWriteDlg, CQuestionDlg)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CArticleDlg message handlers

BOOL CWriteDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CWriteDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
 	if (bShow)
 	{
 		m_pRecord->m_pQuestion->GetPropVal(L"title", m_strArticle);
		m_strAnswer = L"";
 		if (m_pRecord->m_listUserAnswers.begin() != m_pRecord->m_listUserAnswers.end())
 		{
			m_strAnswer = *(m_pRecord->m_listUserAnswers.begin());
 		}
		
 		UpdateData(FALSE);
 	}
}

HRESULT CWriteDlg::PersistData()
{
	UpdateData(TRUE);
	m_pRecord->m_listUserAnswers.clear();
	m_pRecord->m_listUserAnswers.push_back(m_strAnswer);
	return S_OK;
}