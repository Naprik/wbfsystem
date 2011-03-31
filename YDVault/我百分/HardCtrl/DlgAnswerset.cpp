// DlgAnswerset.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "DlgAnswerset.h"
#include "DBStdanswer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAnswerset dialog


CDlgAnswerset::CDlgAnswerset(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAnswerset::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAnswerset)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgAnswerset::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAnswerset)
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAnswerset, CDialog)
	//{{AFX_MSG_MAP(CDlgAnswerset)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList)
	ON_NOTIFY(NM_CLICK, IDC_LIST, OnClickList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAnswerset message handlers

void CDlgAnswerset::OnOK() 
{
	UpdateData(TRUE);
	for(int t = 0; t < m_list.GetItemCount(); t++)
	{
		if(m_list.GetItemText(t,1) == "")
		{
			CString temp = "第"+m_list.GetItemText(t,0)+"题未设置答案！";
			AfxMessageBox(temp);
			return;
		}
		if(m_list.GetItemText(t,2) == "")
		{
			CString temp = "第"+m_list.GetItemText(t,0)+"题未设置分值！";
			AfxMessageBox(temp);
			return;
		}
	}


	CoInitialize(NULL);
	CDBStdanswer  m_Stdanswerdb;
	if ( !m_Stdanswerdb.Connect())
	{
		AfxMessageBox("Paperdb: 无法连接到数据库服务器");
	}
	else
	{
		for(int i = 0; i < m_list.GetItemCount(); i++)
		{
			m_Stdanswerdb.m_ydid = 0;
			m_Stdanswerdb.m_paperid = paperID;
			m_Stdanswerdb.m_frage = atoi(m_list.GetItemText(i,0));
			m_Stdanswerdb.m_res = m_list.GetItemText(i,1);
			m_Stdanswerdb.m_score = atof(m_list.GetItemText(i,2));
			if(!m_Stdanswerdb.Add())
			{
				CString temp;
				temp.Format("第%d题添加失败！", m_Stdanswerdb.m_frage);
				AfxMessageBox(temp);
			}
		}
		AfxMessageBox("添加成功！");
		m_Stdanswerdb.Close();
	}
	CoUninitialize();

	CDialog::OnOK();
}

BOOL CDlgAnswerset::OnInitDialog() 
{
	CDialog::OnInitDialog();

  
    m_Listedit.CreateEx(this, &m_list);
    m_ListComboBox.CreateEx(this, &m_list);
	
	m_ListComboBox.AddString("A");
	m_ListComboBox.AddString("B");
	m_ListComboBox.AddString("C");
	m_ListComboBox.AddString("D");
	m_Listedit.Insert(2);

	m_ListComboBox.Insert(1);


	m_list.InsertColumn(0,_T("题号"),LVCFMT_LEFT,100);
	m_list.InsertColumn(1,_T("答案"),LVCFMT_LEFT,100);
	m_list.InsertColumn(2,_T("分值"),LVCFMT_LEFT,100);

	int iTemp = startNO;
	for(; iTemp <= endNO; iTemp++)
	{
		CString temp;
		temp.Format("%d", iTemp);
		m_list.InsertItem(iTemp-startNO,_T(temp));
	}
	
	m_list.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	return TRUE;  
}

void CDlgAnswerset::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
    CListCtrlEditBase::OnOpen(&m_list, pNMHDR);	
	*pResult = 0;
}

void CDlgAnswerset::OnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int nItem, nSubItem;
    if (CListCtrlEditBase::HitTestEx(&m_list, pNMHDR, nItem, nSubItem) &&
	    nSubItem == 1)
	{
	   m_ListComboBox.ShowEdit(nItem, nSubItem);
	}
	*pResult = 0;
}
