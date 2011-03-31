// DlgResDetail.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "DlgResDetail.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgResDetail dialog


CDlgResDetail::CDlgResDetail(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgResDetail::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgResDetail)
	//}}AFX_DATA_INIT
}


void CDlgResDetail::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgResDetail)
	DDX_Control(pDX, IDC_RESLIST, m_reslist);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgResDetail, CDialog)
	//{{AFX_MSG_MAP(CDlgResDetail)
	ON_NOTIFY(NM_DBLCLK, IDC_RESLIST, OnDblclkReslist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgResDetail message handlers
BOOL CDlgResDetail::OnInitDialog()
{
	CDialog::OnInitDialog();

  /*  DWORD dwStyle=GetWindowLong(m_reslist.GetSafeHwnd(),GWL_STYLE);
	dwStyle&=~LVS_TYPEMASK;
	dwStyle|=LVS_REPORT;
	SetWindowLong(m_reslist.GetSafeHwnd(),GWL_STYLE,dwStyle);  
    
	m_reslist.SetExtendedStyle(LVS_EX_GRIDLINES);
    ::SendMessage(m_reslist.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
     LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	dwStyle = m_reslist.GetExtendedStyle();*/
//	dwStyle |= LVS_EX_CHECKBOXES;
 //   m_reslist.SetExtendedStyle(dwStyle);

 //   ::SetWindowPos(m_hWnd,HWND_TOPMOST,0, 0, 0, 0,SWP_NOSIZE | SWP_NOMOVE  );
	
//	m_reslist.ClearHeadings();
//	m_reslist.SetHeadings("题号,138;学生答案,138;标准答案,138");

//	m_reslist.Create(WS_CHILD|WS_VISIBLE|WS_BORDER|LVS_REPORT, CRect(10,10,300,200), this,1);
/*	m_reslist.SetExtendedStyle(m_reslist.GetExtendedStyle() | LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);
	CRect rt;
	m_reslist.GetClientRect(&rt);

	m_reslist.InsertColumn(0,"题号",LVCFMT_LEFT,rt.right/3,0);
	m_reslist.InsertColumn(1,"学生答案",LVCFMT_LEFT,rt.right/3,0);
	m_reslist.InsertColumn(2,"标准答案",LVCFMT_LEFT,rt.right/3,0);

	GetDlgItem(IDC_STATIC_ID)->SetWindowText(m_id);
	GetDlgItem(IDC_STATIC_NAME)->SetWindowText(m_name);
	GetDlgItem(IDC_STATIC_SUBJECT)->SetWindowText(m_subject);
	GetDlgItem(IDC_STATIC_CLASS)->SetWindowText(m_class);
	GetDlgItem(IDC_STATIC_RIGHT)->SetWindowText(m_right);
	GetDlgItem(IDC_STATIC_WRONG)->SetWindowText(m_wrong);

	float rate = (float)atoi(m_right)/(float)(atoi(m_right)+(float)atoi(m_wrong))*100;

	CString strscore;
	strscore.Format("%0.2f",rate);
	CString strRate = strscore + "%";
	GetDlgItem(IDC_STATIC_RIGHT_RATE)->SetWindowText(strRate);
	GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(strscore);


	for(int i = 0; i < m_sturesult.GetLength();i++)
	{
		CString strNo;
		strNo.Format("%d",(i+1));
		m_reslist.InsertItem(i,strNo);//插入行
		m_reslist.SetItemText(i,1,m_sturesult.Mid(i,1));
		m_reslist.SetItemText(i,2,m_stdresult.Mid(i,1));
		if(m_sturesult.Mid(i,1) != m_stdresult.Mid(i,1))
		{*/
//			m_reslist.SetItemColor(i/*行*/,(DWORD)RGB(255,0,0)/*颜色*/);
//		}

//		m_reslist.AddItem(0,strNo,m_sturesult.Mid(i,1),m_stdresult.Mid(i,1));
//	}

	m_reslist.SetExtendedStyle(m_reslist.GetExtendedStyle() | LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);
	CRect rt;
	m_reslist.GetClientRect(&rt);

	GetDlgItem(IDC_STATIC_ID)->SetWindowText(strStuid);
	GetDlgItem(IDC_STATIC_NAME)->SetWindowText(strStuname);
	GetDlgItem(IDC_STATIC_SUBJECT)->SetWindowText(strSubject);
	GetDlgItem(IDC_STATIC_PAPER)->SetWindowText(strPaper);
	GetDlgItem(IDC_STATIC_RIGHT)->SetWindowText(strRight);
	GetDlgItem(IDC_STATIC_WRONG)->SetWindowText(strWorng);
	GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(strSocrt);

	m_reslist.InsertColumn(0,"题号",LVCFMT_LEFT,rt.right/4,0);
	m_reslist.InsertColumn(1,"学生答案",LVCFMT_LEFT,rt.right/4,0);
	m_reslist.InsertColumn(2,"标准答案",LVCFMT_LEFT,rt.right/4,0);
	m_reslist.InsertColumn(3,"分值",LVCFMT_LEFT,rt.right/4,0);


	int startNO,endNO;
	CoInitialize(NULL);
	CDBPaper m_Paperdb;	
	if(!m_Paperdb.Connect())
	{
		AfxMessageBox("Paperdb:无法连接到数据库服务器");
	}
	else
	{	
		BOOL flag = m_Paperdb.LoadByPaperID(iPaperID);
		if(flag)
		{
			startNO = m_Paperdb.m_startNO;
			endNO = m_Paperdb.m_endNO;
		}
		m_Paperdb.Close();
	}
 
 
	CDBResult m_Resultdb;
	if(!m_Resultdb.Connect())
	{
		AfxMessageBox("Resultdb:无法连接到数据库服务器");
	}
	else
	{
		BOOL flag = m_Resultdb.LoadByQuery(strStuid,iPaperID);
		if(flag)
		{
			CString stuanswer = m_Resultdb.m_stuanswer;
			for(int i = 0; i < endNO-startNO+1;i++)
			{
				CString no;
				no.Format("%d", startNO+i);
				m_reslist.InsertItem(i,no);//插入行
				m_reslist.SetItemText(i,1,stuanswer.Mid(i,1));
			}
		}
		m_Resultdb.Close();
	}
 
 	double total = 0;
	CDBStdanswer m_Stdanswerdb;
	if(!m_Stdanswerdb.Connect())
	{
		AfxMessageBox("Stdanswerdb:无法连接到数据库服务器");
	}
	else
	{
		int temp = startNO;
		for(temp; temp <= endNO; temp++)
		{
			BOOL flag = m_Stdanswerdb.LoadByQuery(iPaperID,temp);
			if(flag)
			{
				CString key = m_Stdanswerdb.m_res;
				double value = m_Stdanswerdb.m_score;
				total += value;
				CString sValue;
				sValue.Format("%0.2f",value);
				m_reslist.SetItemText(temp-startNO,2,key);
				m_reslist.SetItemText(temp-startNO,3,sValue);
			}
		}
		m_Stdanswerdb.Close();
	}
	CoUninitialize();
	CString strTotal;
	strTotal.Format("%0.2f",total);
	GetDlgItem(IDC_STATIC_TOTAL)->SetWindowText(strTotal);

	for(int i = 0; i < m_reslist.GetItemCount(); i++)
	{
		if(m_reslist.GetItemText(i,1) != m_reslist.GetItemText(i,2))
			m_reslist.SetItemColor(i/*行*/,(DWORD)RGB(255,0,0)/*颜色*/);
	}

	return TRUE;
}




void CDlgResDetail::OnDblclkReslist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}
