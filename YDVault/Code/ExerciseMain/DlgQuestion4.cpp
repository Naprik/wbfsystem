// DlgQuestion4.cpp : implementation file
//

#include "stdafx.h"
#include "ExerciseMain.h"
#include "DlgQuestion4.h"


// CDlgQuestion4 dialog

IMPLEMENT_DYNAMIC(CDlgQuestion4, CQuestionAreaItem)

CDlgQuestion4::CDlgQuestion4(CWnd* pParent /*=NULL*/)
	: CQuestionAreaItem(CDlgQuestion4::IDD, pParent)
{

}

CDlgQuestion4::~CDlgQuestion4()
{
}

void CDlgQuestion4::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgQuestion4, CQuestionAreaItem)
END_MESSAGE_MAP()

HRESULT CDlgQuestion4::GetAnswers(CString* pStrAnswer)
{
	*pStrAnswer = TEXT("");
	CButton* pRadio = (CButton*)GetDlgItem(IDC_RADIO_A);
	if (pRadio->GetCheck() == BST_CHECKED)
	{
		*pStrAnswer += TEXT("A");
		return S_OK;
	}
	pRadio = (CButton*)GetDlgItem(IDC_RADIO_B);
	if (pRadio->GetCheck() == BST_CHECKED)
	{
		*pStrAnswer += TEXT("B");
		return S_OK;
	}
	pRadio = (CButton*)GetDlgItem(IDC_RADIO_C);
	if (pRadio->GetCheck() == BST_CHECKED)
	{
		*pStrAnswer += TEXT("C");
		return S_OK;
	}
	pRadio = (CButton*)GetDlgItem(IDC_RADIO_D);
	if (pRadio->GetCheck() == BST_CHECKED)
	{
		*pStrAnswer += TEXT("D");
		return S_OK;
	}

	pRadio = (CButton*)GetDlgItem(IDC_RADIO_E);
	if (pRadio->GetCheck() == BST_CHECKED)
	{
		*pStrAnswer += TEXT("E");
		return S_OK;
	}

	pRadio = (CButton*)GetDlgItem(IDC_RADIO_F);
	if (pRadio->GetCheck() == BST_CHECKED)
	{
		*pStrAnswer += TEXT("F");
		return S_OK;
	}
	return S_OK;
}
// CDlgQuestion4 message handlers

BOOL CDlgQuestion4::OnInitDialog()
{
	CDialog::OnInitDialog();

	CEdit* pText = (CEdit*)GetDlgItem(IDC_EDIT_TEXT);
	CString str;
	m_pQuestion->GetPropVal(TEXT("title"), str);
	CString strTilte;
	strTilte.Format(TEXT("(%d.%d) %s"),  m_mainIndex, m_index, str);
	pText->SetWindowText(strTilte);
	CButton* pRadioA = (CButton*)GetDlgItem(IDC_RADIO_A);
	CButton* pRadioB = (CButton*)GetDlgItem(IDC_RADIO_B);
	CButton* pRadioC = (CButton*)GetDlgItem(IDC_RADIO_C);
	CButton* pRadioD = (CButton*)GetDlgItem(IDC_RADIO_D);
	CButton* pRadioE = (CButton*)GetDlgItem(IDC_RADIO_E);
	CButton* pRadioF = (CButton*)GetDlgItem(IDC_RADIO_F);

	CEdit* pEditA = (CEdit*)GetDlgItem(IDC_EDIT_A);
	CEdit* pEditB = (CEdit*)GetDlgItem(IDC_EDIT_B);
	CEdit* pEditC = (CEdit*)GetDlgItem(IDC_EDIT_C);
	CEdit* pEditD = (CEdit*)GetDlgItem(IDC_EDIT_D);
	CEdit* pEditE = (CEdit*)GetDlgItem(IDC_EDIT_E);
	CEdit* pEditF = (CEdit*)GetDlgItem(IDC_EDIT_F);

	ASSERT(m_lstChoices.size() > 2);
	
	CRect rectMain;
	GetWindowRect(rectMain);
	std::list<CString>::const_iterator itrC = m_lstChoices.begin();
	if (m_lstChoices.size() == 1)
	{
		CRect rectRadio;
		pEditA->GetWindowRect(rectRadio);
		CRect rect(rectMain.left, rectMain.top, rectMain.right, rectRadio.bottom);
		MoveWindow(rect);
		CString strChoice = *itrC;
		pEditA->SetWindowText(strChoice);
	}
	else if (m_lstChoices.size() == 2)
	{
		CRect rectRadio;
		pEditB->GetWindowRect(rectRadio);
		CRect rect(rectMain.left, rectMain.top, rectMain.right, rectRadio.bottom);
		MoveWindow(rect);
		CString strChoice = *itrC;
		
		pEditA->SetWindowText(strChoice);
		++itrC;
		strChoice = *itrC;
		
		pEditB->SetWindowText(strChoice);
	}
	else if (m_lstChoices.size() == 3)
	{
		CRect rectRadio;
		pRadioC->GetWindowRect(rectRadio);
		CRect rect(rectMain.left, rectMain.top, rectMain.right, rectRadio.bottom);
		MoveWindow(rect);
		CString strChoice = *itrC;
		
		pEditA->SetWindowText(strChoice);
		++itrC;
		strChoice = *itrC;
		
		pEditB->SetWindowText(strChoice);
		++itrC;
		strChoice = *itrC;
		
		pEditC->SetWindowText(strChoice);
	}
	else if (m_lstChoices.size() == 4)
	{
		CRect rectRadio;
		pEditD->GetWindowRect(rectRadio);
		CRect rect(rectMain.left, rectMain.top, rectMain.right, rectRadio.bottom);
		MoveWindow(rect);
		CString strChoice = *itrC;
		
		pEditA->SetWindowText(strChoice);
		++itrC;
		strChoice = *itrC;
		
		pEditB->SetWindowText(strChoice);
		++itrC;
		strChoice = *itrC;
		
		pEditC->SetWindowText(strChoice);
		++itrC;
		strChoice = *itrC;
		
		pEditD->SetWindowText(strChoice);
	}
	else if (m_lstChoices.size() == 5)
	{
		CRect rectRadio;
		pRadioE->GetWindowRect(rectRadio);
		CRect rect(rectMain.left, rectMain.top, rectMain.right, rectRadio.bottom);
		MoveWindow(rect);
		CString strChoice = *itrC;
		
		pEditA->SetWindowText(strChoice);
		++itrC;
		strChoice = *itrC;
		
		pEditB->SetWindowText(strChoice);
		++itrC;
		strChoice = *itrC;
		
		pEditC->SetWindowText(strChoice);
		++itrC;
		
		pEditD->SetWindowText(strChoice);
		++itrC;
		
		pEditE->SetWindowText(strChoice);
	}
	else if (m_lstChoices.size() == 6)
	{
		CRect rectRadio;
		pRadioF->GetWindowRect(rectRadio);
		CRect rect(rectMain.left, rectMain.top, rectMain.right, rectRadio.bottom);
		MoveWindow(rect);
		CString strChoice = *itrC;
		pEditA->SetWindowText(strChoice);
		++itrC;
		strChoice = *itrC;
		pEditB->SetWindowText(strChoice);
		++itrC;
		strChoice = *itrC;
		pEditC->SetWindowText(strChoice);
		++itrC;
		strChoice = *itrC;
		pEditD->SetWindowText(strChoice);
		++itrC;
		strChoice = *itrC;
		pEditE->SetWindowText(strChoice);
		++itrC;
		strChoice = *itrC;
		pEditF->SetWindowText(strChoice);
	}
	
	m_strAnswer.Trim();
	if (m_strAnswer.GetLength() > 0)
	{
		if (m_strAnswer.GetAt(0) == TEXT('A'))
		{
			pRadioA->SetCheck(BST_CHECKED);
		}
		else if (m_strAnswer.GetAt(0) == TEXT('B'))
		{
			pRadioB->SetCheck(BST_CHECKED);
		}
		else if (m_strAnswer.GetAt(0) == TEXT('C'))
		{
			pRadioC->SetCheck(BST_CHECKED);
		}
		else if (m_strAnswer.GetAt(0) == TEXT('D'))
		{
			pRadioD->SetCheck(BST_CHECKED);
		}
		else if (m_strAnswer.GetAt(0) == TEXT('E'))
		{
			pRadioE->SetCheck(BST_CHECKED);
		}
		else if (m_strAnswer.GetAt(0) == TEXT('F'))
		{
			pRadioF->SetCheck(BST_CHECKED);
		}
		else
		{
			
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
