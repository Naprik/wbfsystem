// FormAnswer.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "FormAnswer.h"
#include "DlgAnswer.h"
#include "DlgPaperQuery.h"
#include "DlgAnswerset.h"
#include "DBClass.h"

extern CHardCtrlApp theApp;

IMPLEMENT_DYNCREATE(CFormAnswer, CDialog)

/////////////////////////////////////////////////////////////////////////////
// CFormAnswer dialog


CFormAnswer::CFormAnswer(CWnd* pParent /*=NULL*/)
	: cdxCSizingDialog(CFormAnswer::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormAnswer)
	//}}AFX_DATA_INIT
}


void CFormAnswer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormAnswer)
	DDX_Control(pDX, IDC_UPDATE, m_update);
	DDX_Control(pDX, IDC_SHOWALL, m_showall);
	DDX_Control(pDX, IDC_SAVE, m_save);
	DDX_Control(pDX, IDC_QUERYPAPER, m_querypaper);
	DDX_Control(pDX, IDC_DELETE, m_delete);
	DDX_Control(pDX, IDC_ADDANSWER, m_addanswer);
	DDX_Control(pDX, IDC_ADD, m_add);
	DDX_Control(pDX, IDC_PAPERLIST, m_paperlist);
	DDX_Control(pDX, IDC_ANSWERLIST, m_answerlist);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormAnswer, cdxCSizingDialog)
	//{{AFX_MSG_MAP(CFormAnswer)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_UPDATE, OnUpdate)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_NOTIFY(NM_CLICK, IDC_STDANSWERLIST, OnClickStdanswerlist)
	ON_BN_CLICKED(IDC_SHOWALL, OnShowall)
	ON_NOTIFY(NM_DBLCLK, IDC_PAPERLIST, OnDblclkPaperlist)
	ON_BN_CLICKED(IDC_ADDANSWER, OnAddanswer)
	ON_BN_CLICKED(IDC_QUERYPAPER, OnQuerypaper)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormAnswer message handlers

BOOL CFormAnswer::OnInitDialog()
{
	cdxCSizingDialog::OnInitDialog();



	m_Static1.SubclassDlgItem(IDC_STATIC3,this);
	m_Static1.SetCaption("双击列表可查看该试卷答案详情");
	m_Static1.SetBackColor(RGB(0,0,0));

	AddSzControl(*GetDlgItem(IDC_ADD),mdNone,mdRepos);	
	AddSzControl(*GetDlgItem(IDC_DELETE),mdNone,mdRepos);
    AddSzControl(*GetDlgItem(IDC_UPDATE),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_SAVE),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_SHOWALL),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_QUERYPAPER),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_ADDANSWER),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_STATIC1),mdNone,mdNone);
	AddSzControl(*GetDlgItem(IDC_STATIC2),mdNone,mdNone);
	AddSzControl(*GetDlgItem(IDC_STATIC3),mdNone,mdRepos);
	AddSzControl(m_answerlist,mdResize,mdResize);
	AddSzControl(m_paperlist,mdResize,mdResize);

    DWORD dwStyle=GetWindowLong(m_paperlist.GetSafeHwnd(),GWL_STYLE);
	dwStyle&=~LVS_TYPEMASK;
	dwStyle|=LVS_REPORT;
	SetWindowLong(m_paperlist.GetSafeHwnd(),GWL_STYLE,dwStyle);  
    
	m_paperlist.SetExtendedStyle(LVS_EX_GRIDLINES);
    ::SendMessage(m_paperlist.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
     LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	dwStyle = m_paperlist.GetExtendedStyle();
	dwStyle |= LVS_EX_CHECKBOXES;
    m_paperlist.SetExtendedStyle(dwStyle);

    ::SetWindowPos(m_hWnd,HWND_TOPMOST,0, 0, 0, 0,SWP_NOSIZE | SWP_NOMOVE  );

	m_paperlist.ClearHeadings();
	m_paperlist.SetHeadings("考卷名称,100;考试科目,100; 参考班级,100;开考时间,150;收卷试卷,150;开始题号,100;结束题号,100;最大题号,100");

    dwStyle=GetWindowLong(m_answerlist.GetSafeHwnd(),GWL_STYLE);
	dwStyle&=~LVS_TYPEMASK;
	dwStyle|=LVS_REPORT;
	SetWindowLong(m_answerlist.GetSafeHwnd(),GWL_STYLE,dwStyle);  
    
	m_answerlist.SetExtendedStyle(LVS_EX_GRIDLINES);
    ::SendMessage(m_answerlist.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
     LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	dwStyle = m_answerlist.GetExtendedStyle();
	dwStyle |= LVS_EX_CHECKBOXES;
    m_answerlist.SetExtendedStyle(dwStyle);

    ::SetWindowPos(m_hWnd,HWND_TOPMOST,0, 0, 0, 0,SWP_NOSIZE | SWP_NOMOVE  );

	m_answerlist.ClearHeadings();
	m_answerlist.SetHeadings("题号,50;答案,100;分值,100");	


/*	m_ListComboBox.CreateEx(this, &m_list);

	m_ListComboBox.AddString("A");
	m_ListComboBox.AddString("B");
	m_ListComboBox.AddString("C");
	m_ListComboBox.AddString("D");
	m_list.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_list.InsertColumn(0,_T("题号"),LVCFMT_LEFT,100);
	m_list.InsertColumn(1,_T("答案"),LVCFMT_LEFT,100);*/

	return TRUE;
}

void CFormAnswer::OnAdd() 
{
/*	CDlgAnswer  dlg;
	int iStart, iEnd, index;
	CString strStart, strEnd;
	CString strSubject;
	if(dlg.DoModal() == IDOK)
	{
		iStart = dlg.m_start;
		iEnd = dlg.m_end;
		strSubject = dlg.strSubject;
		index = dlg.index;
	
		CString stdanswer;
		CoInitialize(NULL);
		CDBStdanswer stdanswerdb;
		if ( !stdanswerdb.Connect())
		{
			AfxMessageBox("DBStdanswer 无法连接到数据库服务器");
		}		
		else
		{
			int flag = stdanswerdb.LoadBySubject(strSubject);
			if(flag)
			{
				stdanswer = stdanswerdb.m_stdanswer;
				strStart = stdanswerdb.m_startno;
				strEnd = stdanswerdb.m_endno;
			}
		}
		stdanswerdb.Close();
		CoUninitialize();	

		m_list.DeleteAllItems();

		if(stdanswer != "")
		{
			AfxMessageBox("该门科目成绩已存在！如需重新输入请先删除此科目答案！");
			for(int i = 0; i < stdanswer.GetLength(); i++)
			{
				int iNO = i+atoi(strStart);
				CString temp;
				temp.Format("%d", iNO);
				m_list.InsertItem(i,_T(temp));
				m_list.SetItemText(i,1,_T(stdanswer.Mid(i,1)));
			}
		}
		else
		{
			int iTemp = iStart;
			for( ; iStart <= iEnd; iStart++)
			{
				CString temp;
				temp.Format("%d", iStart);
				m_list.InsertItem(iStart-iTemp,_T(temp));
			}
		}
	}
	m_subject.SetCurSel(index);*/
}

void CFormAnswer::OnDelete() 
{
/*	CString strSubject;
	int i = m_subject.GetCurSel();
	if(i != -1)
	{
		m_subject.GetLBText(i,strSubject);
	}

	CString msg;
	msg.Format("真的要删除 %s 此科目答案吗？",strSubject);
	if ( MessageBox(msg,"确认",MB_OKCANCEL) != IDOK)
		return;	

	CoInitialize(NULL);
	CDBStdanswer stdanswerdb;
	if ( !stdanswerdb.Connect())
	{
		AfxMessageBox("DBStdanswer 无法连接到数据库服务器");
	}		
	else
	{
		int flag = stdanswerdb.LoadBySubject(strSubject);
		if(flag)
		{
			int id = stdanswerdb.m_ydid;
			if(stdanswerdb.Delete(id))
			{
				AfxMessageBox("删除成功！");
			}
			else
			{
				AfxMessageBox("删除失败！");
			}
		}
		stdanswerdb.Close();
	}
	CoUninitialize();
	
*/
}

void CFormAnswer::OnUpdate() 
{

	
}

void CFormAnswer::OnSave() 
{
/*	CString strStdanwer = "";	
	int iCount = m_list.GetItemCount();
	CString strIndex = m_list.GetItemText(0,0);
	int index = atoi(strIndex);
	int iEnd = index + iCount - 1;
	CString strEnd;
	strEnd.Format("%d",iEnd);
	for(int i = 0; i < iCount; i++)
	{
		if(m_list.GetItemText(i,1) == "")
		{
			AfxMessageBox("有题目答案未设置！");
			return;
		}
		strStdanwer += m_list.GetItemText(i,1);
	}
	AfxMessageBox(strStdanwer);

	CString strSubject;
	int t = m_subject.GetCurSel();
	if(t != -1)
	{
		m_subject.GetLBText(t,strSubject);
	}

	CoInitialize(NULL);
	CDBStdanswer stdanswerdb;
	if ( !stdanswerdb.Connect())
	{
		AfxMessageBox("DBStdanswer 无法连接到数据库服务器");
	}		
	else
	{
		stdanswerdb.m_ydid = 0;
		stdanswerdb.m_subject = strSubject;
		stdanswerdb.m_stdanswer = strStdanwer;
		stdanswerdb.m_startno = strIndex;
		stdanswerdb.m_endno = strEnd;
		if(stdanswerdb.Add())
		{
			AfxMessageBox("添加成功！");
		}
		else
			AfxMessageBox("添加失败！");
		stdanswerdb.Close();
	}
	CoUninitialize();*/
}





void CFormAnswer::OnClickStdanswerlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
/*	int nItem, nSubItem;
    if (CListCtrlEditBase::HitTestEx(&m_list, pNMHDR, nItem, nSubItem) &&
	    nSubItem == 1)
	{
	   m_ListComboBox.ShowEdit(nItem, nSubItem);
	}*/
	*pResult = 0;
}

void CFormAnswer::OnShowall() 
{
	m_paperlist.DeleteAllItems();
	CoInitialize(NULL);
	CDBPaper       m_Paperdb;
	CDBSubject     m_subjectdb;
	CDBClass       m_classdb;
	if ( !m_Paperdb.Connect())
	{
		AfxMessageBox("Paperdb: 无法连接到数据库服务器");
	}	
	else
	{
		if ((!m_subjectdb.Connect()) || (!m_classdb.Connect()))
		{
			AfxMessageBox("m_subjectdb: 无法连接到数据库服务器");
		}
		else
		{
			BOOL paperflag = m_Paperdb.LoadByAll();
			while (paperflag)
			{
				CString tep1,tep2,tep3,strSubject,strclass;
				tep1.Format("%d",m_Paperdb.m_startNO);
				tep2.Format("%d",m_Paperdb.m_endNO);
				tep3.Format("%d",m_Paperdb.m_maxNO);
				CString strSubjectid = m_Paperdb.m_subjectid;
				CString strClassid = m_Paperdb.m_classid;
				BOOL subjectflag = m_subjectdb.LoadBySubjectid(strSubjectid);
				if (subjectflag)
				{
					strSubject = m_subjectdb.m_subject;
				}
				BOOL classflag = m_classdb.LoadByClassid(strClassid);
				if (classflag)
				{
					strclass = m_classdb.m_classname;
				}
				m_paperlist.AddItem(0,m_Paperdb.m_papername,
					strSubject,
					strclass,
					m_Paperdb.m_starttime.Format(),
					m_Paperdb.m_endtime.Format(),
					tep1,tep2,tep3);
				paperflag = m_Paperdb.Move();
			}
			m_classdb.Close();
			m_subjectdb.Close();
		}
		m_Paperdb.Close();
	}
	CoUninitialize();

//	m_paperlist.DeleteAllItems();
// 	CoInitialize(NULL);
// 	CDBPaper       m_Paperdb;
// 	if ( !m_Paperdb.Connect())
// 	{
// 		AfxMessageBox("Paperdb: 无法连接到数据库服务器");
// 	}	
// 	else
// 	{
// 		BOOL flag = m_Paperdb.LoadByAll();
// 		while(flag)
// 		{
// 			CString tep1,tep2,tep3;
// 			tep1.Format("%d",m_Paperdb.m_startNO);
// 			tep2.Format("%d",m_Paperdb.m_endNO);
// 			tep3.Format("%d",m_Paperdb.m_maxNO);
// 			m_paperlist.AddItem(0,m_Paperdb.m_papername,
// 							m_Paperdb.m_subject,
// 							m_Paperdb.m_depart,
// 							m_Paperdb.m_class,
// 							m_Paperdb.m_starttime.Format(),
// 							m_Paperdb.m_endtime.Format(),
// 							tep1,tep2,tep3);
// 			flag = m_Paperdb.Move();
// 		}
// 		m_Paperdb.Close();
// 	}
// 	CoUninitialize();	
}

void CFormAnswer::OnDblclkPaperlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int index =pNMListView->iItem;
	CString strSubject = m_paperlist.GetItemText(index,1);
	CString strClass = m_paperlist.GetItemText(index,3);
	CString strStart = m_paperlist.GetItemText(index,4);
	CString strPapername = m_paperlist.GetItemText(index,0);
	if(strSubject == "")
		return;
	DWORD paperID = 0;
	CoInitialize(NULL);
	CDBPaper m_paperdb;
	if (!m_paperdb.Connect())
	{
		AfxMessageBox("Paperdb: 无法连接到数据库服务器");
	}
	else
	{
		BOOL flag = m_paperdb.LoadByPaperName(strPapername);
		if (flag)
		{
			paperID = m_paperdb.m_ydid;
		}
		m_paperdb.Close();
	}

	CDBStdanswer m_stdanswerdb;
	if (!m_stdanswerdb.Connect())
	{
		AfxMessageBox("m_stdanswerdb: 无法连接到数据库服务器");
	}
	else
	{
		m_answerlist.DeleteAllItems();
		BOOL flag = m_stdanswerdb.LoadByPaperID(paperID);
		if (!flag)
		{
			CString msg;
			msg.Format("没有该门试卷答案，需要添加该试卷答案吗？");
 			if ( MessageBox(msg,"确认",MB_OKCANCEL) == IDOK)
			{
				CDlgAnswerset dlg;
				CString strStart = m_paperlist.GetItemText(index,5);
				CString strEnd = m_paperlist.GetItemText(index,6);
				dlg.startNO = atoi(strStart);
				dlg.endNO = atoi(strEnd);
				dlg.paperID = paperID;
				if (dlg.DoModal() == IDOK)
				{
				}
			}
		}
		else
		{
 			while(flag)
 			{
 				int num = m_stdanswerdb.m_frage;
 				double value = m_stdanswerdb.m_score;
 				CString sNum, sValue;
 				sNum.Format("%d", num);
 				sValue.Format("%0.2f", value);
 				m_answerlist.AddItem(0, sNum, m_stdanswerdb.m_res, sValue);
 				flag = m_stdanswerdb.Move();
 			}	
		}
		
		m_stdanswerdb.Close();
	}

	CoUninitialize();

// 	CoInitialize(NULL);
// 	CDBPaper       m_Paperdb;
// 	if ( !m_Paperdb.Connect())
// 	{
// 		AfxMessageBox("Paperdb: 无法连接到数据库服务器");
// 	}
// 	else
// 	{
// 		BOOL flag = m_Paperdb.LoadByPaper(strSubject,strClass,strStart);
// 		if(flag)
// 		{
// 			paperID = m_Paperdb.m_ydid;
// 		}
// 		m_Paperdb.Close();
// 	}
// 	CoUninitialize();
	

// 	CoInitialize(NULL);
// 	CDBStdanswer  m_Stdanswerdb;
// 	if ( !m_Stdanswerdb.Connect())
// 	{
// 		AfxMessageBox("Paperdb: 无法连接到数据库服务器");
// 	}
// 	else
// 	{
// 		m_answerlist.DeleteAllItems();
// 		BOOL flag = m_Stdanswerdb.LoadByPaperID(paperID);
// 		if(!flag)
// 		{
// 			CString msg;
// 			msg.Format("没有该门试卷答案，需要添加该试卷答案吗？");
// 			if ( MessageBox(msg,"确认",MB_OKCANCEL) == IDOK)
// 			{
// 				CDlgAnswerset dlg;
// 				CString strStart = m_paperlist.GetItemText(index,6);
// 				CString strEnd = m_paperlist.GetItemText(index,7);
// 				dlg.startNO = atoi(strStart);
// 				dlg.endNO = atoi(strEnd);
// 				dlg.paperID = paperID;
// 				if(dlg.DoModal() == IDOK)
// 				{
// 					//AfxMessageBox("添加成功！");
// 				}
// 			}	
// 		}
// 		else
// 		{
// 			while(flag)
// 			{
// 				int num = m_Stdanswerdb.m_no;
// 				double value = m_Stdanswerdb.m_value;
// 				CString sNum, sValue;
// 				sNum.Format("%d", num);
// 				sValue.Format("%0.2f", value);
// 				m_answerlist.AddItem(0, sNum, m_Stdanswerdb.m_key, sValue);
// 				flag = m_Stdanswerdb.Move();
// 			}	
// 		}
// 		m_Stdanswerdb.Close();
// 	}
// 	CoUninitialize();

	
	*pResult = 0;
}

void CFormAnswer::OnAddanswer() 
{
	
	CDlgAnswerset dlg;
//	dlg.DoModal();
/*	CoInitialize(NULL);
	CDBStdanswer  m_Stdanswerdb;
	if ( !m_Stdanswerdb.Connect())
	{
		AfxMessageBox("Stdanswerdb: 无法连接到数据库服务器");
		CoUninitialize();
		return;
	}
	m_Stdanswerdb.m_ydid = 0;
	m_Stdanswerdb.m_paperid = 1;
	m_Stdanswerdb.m_no = 1;
	m_Stdanswerdb.m_key = "A";
	double t = 11.1;
	m_Stdanswerdb.m_value = t;
	m_Stdanswerdb.Add();

	m_Stdanswerdb.Close();
	CoUninitialize()*/
}

void CFormAnswer::OnQuerypaper() 
{
	CDlgPaperQuery  dlg;
	if(dlg.DoModal() == IDOK)
	{
		m_paperlist.DeleteAllItems();
		COleDateTime time;
		time.SetDate(dlg.m_time.GetYear(),dlg.m_time.GetMonth(),dlg.m_time.GetDay());
// 		CoInitialize(NULL);
// 		CDBPaper       m_Paperdb;
// 		if ( !m_Paperdb.Connect())
// 		{
// 			AfxMessageBox("Paperdb: 无法连接到数据库服务器");
// 		}
// 		else
// 		{
// 			BOOL flag = m_Paperdb.LoadByQuery(dlg.strPaper,
// 											  dlg.strSubject,
// 											  dlg.strDepart,
// 											  dlg.strClass,
// 											  time,dlg.bIstime);
// 			if(!flag)
// 				AfxMessageBox("没有搜索到！");
// 			while(flag)
// 			{
// 				CString tep1,tep2,tep3;
// 				tep1.Format("%d",m_Paperdb.m_startNO);
// 				tep2.Format("%d",m_Paperdb.m_endNO);
// 				tep3.Format("%d",m_Paperdb.m_maxNO);
// 				m_paperlist.AddItem(0,m_Paperdb.m_papername,
// 								m_Paperdb.m_subject,
// 								m_Paperdb.m_depart,
// 								m_Paperdb.m_class,
// 								m_Paperdb.m_starttime.Format(),
// 								m_Paperdb.m_endtime.Format(),
// 								tep1,tep2,tep3);
// 				flag = m_Paperdb.Move();
// 			}
// 			m_Paperdb.Close();
// 		}
// 		CoUninitialize();
	}			
}
CString CFormAnswer::GetExePath()
{
	CString file;
	char process[MAX_PATH];
	GetModuleFileName(NULL, process, MAX_PATH);
    for ( int i = strlen(process);i>0 ;i --)
	if ( process[i] =='\\')
	{
		process[i+1]='\0';
		break;
	}
	file.Format(process);
	return file;
}
void CFormAnswer::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
/*	CRect rc;
    CJahCoverCapture capture;
	GetClientRect(&rc);
	capture.m_bmpFile =GetExePath()+"formback.bmp" ;
    if(!capture.LoadBMPImage(capture.m_bmpFile))
   		return;
    if (capture.m_bitmap.m_hObject == 0 )
    	return;
   capture.DrawBitmap(&dc,rc);*/
}
