// DlgSubject.cpp : implementation file
//

#include "stdafx.h"
#include "DlgPaper.h"
#include "../DBBase/DatabaseEx.h"
#include "../ObjRef/YDPaper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSubject dialog


CDlgPaper::CDlgPaper(OPERATION op, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPaper::IDD, pParent)
	, m_uBegin1(0)
	, m_uEnd1(0)
	, m_uCount1(0)
	, m_uBegin2(0)
	, m_uEnd2(0)
	, m_uCount2(0)
	, m_uBegin3(0)
	, m_uEnd3(0)
	, m_uCount3(0)
{
	//{{AFX_DATA_INIT(CDlgSubject)
	m_strCode = _T("");
	m_strName = _T("");
	m_op = op;
	//}}AFX_DATA_INIT
}


void CDlgPaper::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSubject)
	DDX_Text(pDX, IDC_DM_PAPER_CODE, m_strCode);
	DDX_Text(pDX, IDC_DM_PAPER_NAME, m_strName);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_BEGIN1, m_uBegin1);
	DDV_MinMaxUInt(pDX, m_uBegin1, 0, 300);
	DDX_Text(pDX, IDC_EDIT_END1, m_uEnd1);
	DDV_MinMaxUInt(pDX, m_uEnd1, 0, 300);
	DDX_Text(pDX, IDC_EDIT_COUNT1, m_uCount1);
	DDV_MinMaxUInt(pDX, m_uCount1, 0, 300);
	DDX_Control(pDX, IDC_CMB_MULTI1, m_cmbMuti1);

	DDX_Text(pDX, IDC_EDIT_BEGIN2, m_uBegin2);
	DDV_MinMaxUInt(pDX, m_uBegin2, 0, 300);
	DDX_Text(pDX, IDC_EDIT_END2, m_uEnd2);
	DDV_MinMaxUInt(pDX, m_uEnd2, 0, 300);
	DDX_Text(pDX, IDC_EDIT_COUNT2, m_uCount2);
	DDV_MinMaxUInt(pDX, m_uCount2, 0, 300);
	DDX_Control(pDX, IDC_CMB_MULTI2, m_cmbMuti2);

	DDX_Text(pDX, IDC_EDIT_BEGIN3, m_uBegin3);
	DDV_MinMaxUInt(pDX, m_uBegin3, 0, 300);
	DDX_Text(pDX, IDC_EDIT_END3, m_uEnd3);
	DDV_MinMaxUInt(pDX, m_uEnd3, 0, 300);
	DDX_Text(pDX, IDC_EDIT_COUNT3, m_uCount3);
	DDV_MinMaxUInt(pDX, m_uCount3, 0, 300);
	DDX_Control(pDX, IDC_CMB_MULTI3, m_cmbMuti3);
}


BEGIN_MESSAGE_MAP(CDlgPaper, CDialog)
	//{{AFX_MSG_MAP(CDlgSubject)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CDlgPaper::OnBnClickedOk)
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSubject message handlers
BOOL CDlgPaper::OnInitDialog()
{
	CDialog::OnInitDialog();
	if (m_op == OP_VIEW)
	{
		GetDlgItem(IDC_DM_PAPER_CODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_DM_PAPER_NAME)->EnableWindow(FALSE);
	}

	m_cmbMuti1.AddString(L"单选");
	m_cmbMuti1.AddString(L"多选");
	m_cmbMuti1.SetCurSel(m_uMuti1);
	m_cmbMuti2.AddString(L"单选");
	m_cmbMuti2.AddString(L"多选");
	m_cmbMuti2.SetCurSel(m_uMuti2);
	m_cmbMuti3.AddString(L"单选");
	m_cmbMuti3.AddString(L"多选");
	m_cmbMuti3.SetCurSel(m_uMuti3);

	UpdateData(FALSE);
	return TRUE;
}

void CDlgPaper::OnOK() 
{
	// TODO: Add extra validation here
	CString strTmpName = m_strName;
	CString strTmpCode = m_strCode;
	UpdateData(TRUE);
	m_uMuti1 = m_cmbMuti1.GetCurSel();
	m_uMuti2 = m_cmbMuti2.GetCurSel();
	m_uMuti3 = m_cmbMuti3.GetCurSel();
	m_strCode.Trim();
	m_strName.Trim();
	if(m_strCode.IsEmpty())
	{
		AfxMessageBox(_T("试卷编号不能为空！"));
		return;
	}
	if(m_strName.GetLength() == 0)
	{
		AfxMessageBox(L"试卷名称不能为空！");
		GetDlgItem(IDC_DM_PAPER_NAME)->SetFocus();
		return;
	}
	if (m_strName != strTmpName || m_op == OP_NEW)
	{
		CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
		ASSERT(pDB);
		BOOL bExist;
		CYDPaper paperRef(pDB);
		paperRef.IsFieldExist(FIELD_YDTEACHERDEV_NAME, m_strName, &bExist);
		if (bExist)
		{
			AfxMessageBox(L"试卷名称已存在，请重新输入！");
			m_strName = strTmpName;
			m_strCode = strTmpCode;
			UpdateData(FALSE);
			GetDlgItem(IDC_DM_PAPER_NAME)->SetFocus();
			return;
		}
	}

	if (m_strCode != strTmpCode || m_op == OP_NEW)
	{
		CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
		ASSERT(pDB);
		BOOL bExist;
		CYDPaper paperRef(pDB);
		paperRef.IsFieldExist(FIELD_YDPAPER_CODE, m_strCode, &bExist);
		if (bExist)
		{
			AfxMessageBox(L"试卷编号已存在，请重新输入！");
			m_strName = strTmpName;
			m_strCode = strTmpCode;
			UpdateData(FALSE);
			GetDlgItem(IDC_DM_PAPER_CODE)->SetFocus();
			return;
		}
	}

	CDialog::OnOK();
}

INT_PTR CDlgPaper::DoModal()
{
	CExtDllState extdllstate;

	return CDialog::DoModal();
}


void CDlgPaper::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CDlgPaper::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
	
}
