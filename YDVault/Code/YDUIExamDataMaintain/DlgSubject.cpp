// DlgSubject.cpp : implementation file
//

#include "stdafx.h"
#include "DlgSubject.h"
#include "../DBBase/DatabaseEx.h"
#include "../YDExamObjRef/YDSubjectRef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSubject dialog


CDlgSubject::CDlgSubject(OPERATION op, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSubject::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSubject)
	m_subject = _T("");
	m_subjectid = _T("");
	m_op = op;
	//}}AFX_DATA_INIT
}


void CDlgSubject::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSubject)
	DDX_Text(pDX, IDC_SUBJECT, m_subject);
	DDX_Text(pDX, IDC_SUBJECTID, m_subjectid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSubject, CDialog)
	//{{AFX_MSG_MAP(CDlgSubject)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSubject message handlers
BOOL CDlgSubject::OnInitDialog()
{
	CDialog::OnInitDialog();
	if (m_op == OP_VIEW)
	{
		GetDlgItem(IDC_SUBJECT)->EnableWindow(FALSE);
		GetDlgItem(IDC_SUBJECTID)->EnableWindow(FALSE);
	}

	return TRUE;
}

void CDlgSubject::OnOK() 
{
	// TODO: Add extra validation here
	CString strTempId = m_subjectid;
	UpdateData(TRUE);

	m_subjectid.Trim();
	if(m_subjectid.GetLength() == 0)
	{	
		AfxMessageBox(L"科目编号不能为空！");
		GetDlgItem(IDC_SUBJECTID)->SetFocus();
		return;
	}
	if (m_subjectid != strTempId || m_op == OP_NEW)
	{
		CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
		ASSERT(pDB);
		BOOL bExist;
		CYDSubjectRef subRef(pDB);
		subRef.IsFieldExist(FIELD_YDSUBJECT_ID, m_subjectid, &bExist);
		if (bExist)
		{
			AfxMessageBox(L"科目编号已存在，请重新输入！");
			m_subjectid = strTempId;
			UpdateData(FALSE);
			GetDlgItem(IDC_SUBJECTID)->SetFocus();
			return;
		}
	}
	
	m_subject.Trim();
	if(m_subject.GetLength() == 0)
	{
		AfxMessageBox(L"科目名称不能为空！");
		GetDlgItem(IDC_SUBJECT)->SetFocus();
		return;
	}

	CDialog::OnOK();
}

INT_PTR CDlgSubject::DoModal()
{
	CExtDllState extdllstate;

	return CDialog::DoModal();
}
