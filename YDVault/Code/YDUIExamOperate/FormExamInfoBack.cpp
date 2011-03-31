// FormExamInfoBack.cpp : implementation file
//

#include "stdafx.h"
#include "FormExamInfoBack.h"
#include "../ydcom/ExamDetail.h"


IMPLEMENT_DYNCREATE(CFormExamInfoBack, CYdFormView)


CFormExamInfoBack::CFormExamInfoBack()
	: CYdFormView(CFormExamInfoBack::IDD)
{
	//{{AFX_DATA_INIT(CFormExamInfoBack)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFormExamInfoBack::DoDataExchange(CDataExchange* pDX)
{
	CYdFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormExamInfoBack)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormExamInfoBack, CYdFormView)
	//{{AFX_MSG_MAP(CFormExamInfoBack)
	ON_BN_CLICKED(IDC_NEXT, OnNext)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormExamInfoBack message handlers



void CFormExamInfoBack::OnInitialUpdate() 
{
	CYdFormView::OnInitialUpdate();
	

	return ;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFormExamInfoBack::OnNext() 
{
	HRESULT hr = E_FAIL;
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	if(!(((CButton *)GetDlgItem(IDC_RADIO_BLUE2))->GetCheck()) && !(((CButton *)GetDlgItem(IDC_RADIO_INFRA2))->GetCheck()) && !(((CButton *)GetDlgItem(IDC_RADIO_USB2))->GetCheck()))
	{
		AfxMessageBox(_T("û��ѡ�񱾳����Կ���ģʽ���޷�������һ����"));
		return;
	}

	if(!(((CButton *)GetDlgItem(IDC_RADIO_BLUE))->GetCheck()) && !(((CButton *)GetDlgItem(IDC_RADIO_INFRA))->GetCheck()) && !(((CButton *)GetDlgItem(IDC_RADIO_USB))->GetCheck()))
	{
		AfxMessageBox(_T("û��ѡ�񱾳������վ�ģʽ���޷�������һ����"));
		return;
	}
	

	if(((CButton *)GetDlgItem(IDC_RADIO_BLUE2))->GetCheck())
	{
		pGExamStruct->m_pExamDetail->m_uRollCallType = ROLLCALL_TYPE_BLUE;
		//msg2 = "��������";
	}
	else if(((CButton *)GetDlgItem(IDC_RADIO_INFRA2))->GetCheck())
	{
		pGExamStruct->m_pExamDetail->m_uRollCallType = ROLLCALL_TYPE_INFRAR;
		//msg2 = "�������";
	}
	else
	{
		pGExamStruct->m_pExamDetail->m_uRollCallType = ROLLCALL_TYPE_USB;
		//msg2 = "USB����";
	}	


	if(((CButton *)GetDlgItem(IDC_RADIO_BLUE))->GetCheck())
	{
		pGExamStruct->m_pExamDetail->m_uRollingType = ROLLING_TYPE_BLUE;
		//msg2 = "�����վ�";
	}
	else if(((CButton *)GetDlgItem(IDC_RADIO_INFRA))->GetCheck())
	{
		pGExamStruct->m_pExamDetail->m_uRollingType = ROLLING_TYPE_INFRAR;
		//msg2 = "�����վ�";
	}
	else
	{
		pGExamStruct->m_pExamDetail->m_uRollingType = ROLLING_TYPE_USB;
		//msg2 = "USB�վ�";
	}

// 	CMainFrame *fram = (CMainFrame *)AfxGetApp()->m_pMainWnd;
// 	fram->m_dlgMain.m_examset.SetKind(m_kind);
// 	fram->m_dlgMain.ShowDlg(IDD_FORM_EXAMSET);

}

void CFormExamInfoBack::SetKind(int kind)
{
	m_kind = kind;
}
BOOL CFormExamInfoBack::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
											 DWORD dwRequestedStyle, const RECT& rect, CWnd* pParentWnd, UINT nID,
											 CCreateContext* pContext)
{
	CExtDllState state;
	return CFormView::Create(lpszClassName,
		lpszWindowName,
		dwRequestedStyle,
		rect,
		pParentWnd,
		nID,
		pContext);
}