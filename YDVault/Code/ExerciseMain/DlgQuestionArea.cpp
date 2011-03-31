// DlgQuestionArea.cpp : implementation file
//

#include "stdafx.h"
#include "ExerciseMain.h"
#include "DlgQuestionArea.h"
#include "../Base/AutoClean.h"
#include "../ObjRef/YDArticleQuestionRef.h"
#include "../ObjRef/YDChoiceQuestionRef.h"

#include "DlgQuestion4.h"

#include "DlgQuestion0.h"
#include "DlgQuestion1.h"


// CDlgQuestionArea dialog

IMPLEMENT_DYNAMIC(CDlgQuestionArea, CDialog)

CDlgQuestionArea::CDlgQuestionArea(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgQuestionArea::IDD, pParent)
{

}

CDlgQuestionArea::~CDlgQuestionArea()
{
	CleanDlg();
}

void CDlgQuestionArea::CleanDlg()
{
	std::list<CQuestionAreaItem*>::iterator itr = m_lstDlgChoice.begin();
	for (; itr != m_lstDlgChoice.end(); ++itr)
	{
		(*itr)->DestroyWindow();
		delete (*itr);
	}
	m_lstDlgChoice.clear();
}
BOOL CDlgQuestionArea::Reset()
{
	CleanDlg();	
	
	return TRUE;
}

void CDlgQuestionArea::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgQuestionArea, CDialog)
	ON_WM_VSCROLL()
	ON_WM_SHOWWINDOW()
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CDlgQuestionArea message handlers
void CDlgQuestionArea::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nMaxPos = m_MainRect.Height() - m_nCurrentHeight;

	SCROLLINFO scrollinfo;  
	GetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
	switch   (nSBCode)  
	{  
	case   SB_BOTTOM:  
		ScrollWindow(0,(scrollinfo.nPos-scrollinfo.nMax)*10);  
		scrollinfo.nPos = scrollinfo.nMax;  
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
		break;  
	case   SB_TOP:  
		ScrollWindow(0,(scrollinfo.nPos-scrollinfo.nMin)*10);  
		scrollinfo.nPos = scrollinfo.nMin;  
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
		break;  
	case   SB_LINEUP:  
		scrollinfo.nPos -= 1;  
		if   (scrollinfo.nPos<scrollinfo.nMin)  
		{  
			scrollinfo.nPos = scrollinfo.nMin;  
			break;  
		}  
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
		ScrollWindow(0,10);  
		break;  
	case   SB_LINEDOWN:  
		scrollinfo.nPos += 1;  
		if   (scrollinfo.nPos>scrollinfo.nMax)  
		{  
			scrollinfo.nPos = scrollinfo.nMax;  
			break;  
		}  
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
		ScrollWindow(0,-10);  
		break;  
	case   SB_PAGEUP:  
		scrollinfo.nPos -= 5;  
		if   (scrollinfo.nPos<scrollinfo.nMin)  
		{  
			scrollinfo.nPos = scrollinfo.nMin;  
			break;  
		}  
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
		ScrollWindow(0,10*5);  
		break;  
	case   SB_PAGEDOWN:  
		scrollinfo.nPos += 5;  
		if   (scrollinfo.nPos>scrollinfo.nMax)  
		{  
			scrollinfo.nPos = scrollinfo.nMax;  
			break;  
		}  
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
		ScrollWindow(0,-10*5);  
		break;  
	case   SB_ENDSCROLL:  
		// MessageBox("SB_ENDSCROLL");  
		break;  
	case   SB_THUMBPOSITION:  
		// ScrollWindow(0,(scrollinfo.nPos-nPos)*10);  
		// scrollinfo.nPos = nPos;  
		// SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
		break;  
	case   SB_THUMBTRACK:  
		ScrollWindow(0,(scrollinfo.nPos-nPos)*10);  
		scrollinfo.nPos = nPos;  
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
		break;   
	}

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CDlgQuestionArea::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetWindowRect(&m_MainRect);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
BOOL CDlgQuestionArea::ShowQuestion()
{
	m_nCurrentHeight = 0;
	CleanDlg();
	CYDArticleQuestionRef* pArticle = dynamic_cast<CYDArticleQuestionRef*>(m_pRecord->m_pQuestion);

	std::list<CString>::const_iterator itrAns = m_pRecord->m_listUserAnswers.begin();
	if (m_pRecord->m_QTypeID == 8 ||
		m_pRecord->m_QTypeID == 9 ||
		m_pRecord->m_QTypeID == 12)
	{
		int nCount = 0;
		pArticle->GetUnderLineNum(&nCount);
		for (int i=0; i<nCount; ++i)
		{
			CQuestionAreaItem* pItemDlg = new CDlgQuestion0();	
			pItemDlg->m_index = i+1;
			pItemDlg->m_pQuestion = NULL;
			if (itrAns != m_pRecord->m_listUserAnswers.end())
			{
				pItemDlg->m_strAnswer = *itrAns;
				++itrAns;
			}
			pItemDlg->Create(CDlgQuestion0::IDD, this);
			CRect rectSelf;
			pItemDlg->GetWindowRect(&rectSelf);
			LONG top = m_MainRect.top + rectSelf.Height() * (i);
			pItemDlg->MoveWindow(m_MainRect.left, top, rectSelf.Width(), rectSelf.Height());
			pItemDlg->ShowWindow(SW_SHOW);
			m_lstDlgChoice.push_back(pItemDlg);
		}
	}
	else
	{
		std::list<CYDQuestionRef*> lstChildren;
		BOOL bClean = FALSE;
		
		if (pArticle != NULL)
		{
			bClean = TRUE;
			pArticle->GetChildQueston(&lstChildren);
		}
		else
		{
			lstChildren.push_back(m_pRecord->m_pQuestion);
		}
		std::list<CYDQuestionRef*>::const_iterator itrQ = lstChildren.begin();

		UINT uIndex = 1;
		for (; itrQ != lstChildren.end(); ++itrQ, ++uIndex)
		{
			CYDChoiceQuestionRef* pQ = dynamic_cast<CYDChoiceQuestionRef*>(*itrQ);
			std::list<CString> lstChoices;
			pQ->GetOptionList(&lstChoices);
			CQuestionAreaItem* pItemDlg = NULL;
			if (lstChoices.size() < 2)
			{
				pItemDlg = new CDlgQuestion1();	
				pItemDlg->m_index = uIndex;
				pItemDlg->m_pQuestion = *itrQ;
				pItemDlg->m_lstChoices = lstChoices;
				if (itrAns != m_pRecord->m_listUserAnswers.end())
				{
					pItemDlg->m_strAnswer = *itrAns;
					++itrAns;
				}
				pItemDlg->Create(CDlgQuestion1::IDD, this);
			}
			else
			{
				pItemDlg = new CDlgQuestion4();	
				pItemDlg->m_index = uIndex;
				pItemDlg->m_pQuestion = *itrQ;
				pItemDlg->m_lstChoices = lstChoices;
				if (itrAns != m_pRecord->m_listUserAnswers.end())
				{
					pItemDlg->m_strAnswer = *itrAns;
					++itrAns;
				}
				pItemDlg->Create(CDlgQuestion4::IDD, this);
			}

			CRect rectSelf;
			pItemDlg->GetWindowRect(&rectSelf);
			LONG top = m_MainRect.top + rectSelf.Height() * (uIndex -1);
			pItemDlg->MoveWindow(m_MainRect.left, top, rectSelf.Width(), rectSelf.Height());
			pItemDlg->ShowWindow(SW_SHOW);
			m_lstDlgChoice.push_back(pItemDlg);
		}
		if (bClean)
		{
			CListAutoClean<CYDQuestionRef> clr(lstChildren);
		}
	}
	
	UpdateData(FALSE);

	return TRUE;
}
void CDlgQuestionArea::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

}

HRESULT CDlgQuestionArea::GetStuAnswers(std::list<CString>* pListAns)
{
	std::list<CQuestionAreaItem*>::const_iterator itr = m_lstDlgChoice.begin();
	for (; itr != m_lstDlgChoice.end(); ++itr)
	{
		CString strAns;
		(*itr)->GetAnswers(&strAns);
		pListAns->push_back(strAns);
	}

	return S_OK;
}
void CDlgQuestionArea::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
