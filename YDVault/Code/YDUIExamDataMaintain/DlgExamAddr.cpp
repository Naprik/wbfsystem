// DlgSubject.cpp : implementation file
//

#include "stdafx.h"
#include "DlgExamAddr.h"
#include "../DBBase/DatabaseEx.h"
#include "../YDExamObjRef/YDExamAddress.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSubject dialog


CDlgExamAddr::CDlgExamAddr(OPERATION op, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgExamAddr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSubject)
	m_strCode = _T("");
	m_strAddress = _T("");
	m_op = op;
	//}}AFX_DATA_INIT
}


void CDlgExamAddr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSubject)
	DDX_Text(pDX, IDC_DM_EXAMADDR_CODE, m_strCode);
	DDX_Text(pDX, IDC_DM_EXAMADDR_ADDR, m_strAddress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgExamAddr, CDialog)
	//{{AFX_MSG_MAP(CDlgSubject)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSubject message handlers
BOOL CDlgExamAddr::OnInitDialog()
{
	CDialog::OnInitDialog();
	if (m_op == OP_VIEW)
	{
		GetDlgItem(IDC_DM_EXAMADDR_CODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_DM_EXAMADDR_ADDR)->EnableWindow(FALSE);
	}

	return TRUE;
}

void CDlgExamAddr::OnOK() 
{
	// TODO: Add extra validation here
	CString strTmpCode = m_strCode;
	CString strTmpAddr = m_strAddress;
	UpdateData(TRUE);

	m_strCode.Trim();
	if(m_strCode.GetLength() == 0)
	{
		AfxMessageBox(L"考场编号不能为空！");
		GetDlgItem(IDC_SUBJECT)->SetFocus();
		return;
	}
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	BOOL bExist;
	CYDExamAddress examAddrRef(pDB);
	if (strTmpCode != m_strCode || m_op == OP_NEW)
	{
		examAddrRef.IsFieldExist(FIELD_YDEXAMADDR_ADDR, m_strCode, &bExist);
		if (bExist)
		{
			AfxMessageBox(L"考场编号已存在，请重新输入！");
			m_strAddress = strTmpAddr;
			UpdateData(FALSE);
			GetDlgItem(IDC_DM_EXAMADDR_CODE)->SetFocus();
			return;
		}
	}
		
	m_strAddress.Trim();
	if(m_strAddress.GetLength() == 0)
	{
		AfxMessageBox(L"试卷名称不能为空！");
		GetDlgItem(IDC_SUBJECT)->SetFocus();
		return;
	}
	if (m_strAddress != strTmpAddr || m_op == OP_NEW)
	{
		examAddrRef.IsFieldExist(FIELD_YDEXAMADDR_ADDR, m_strAddress, &bExist);
		if (bExist)
		{
			AfxMessageBox(L"考场地点已存在，请重新输入！");
			m_strAddress = strTmpAddr;
			UpdateData(FALSE);
			GetDlgItem(IDC_DM_EXAMADDR_ADDR)->SetFocus();
			return;
		}
	}

	CDialog::OnOK();
}

INT_PTR CDlgExamAddr::DoModal()
{
	CExtDllState extdllstate;

	return CDialog::DoModal();
}
