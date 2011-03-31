// FormClassResult.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "FormClassResult.h"
#include "DlgClassResult.h"
#include "DBResult.h"
#include "DBStdanswer.h"
#include "DBStuInfo.h"
#include "DlgResDetail.h"
#include "DlgGraph.h"
#include "APPGraph.h"
#include "ExportToExcel.h"
#include "PreParent.h"		//打印预览父窗口

#pragma   warning(disable:   4244)
extern CHardCtrlApp theApp;

CAPPGraph  gra;
CString stdanswer;
int startNO;
int endNO;
BOOL b_init = FALSE;

IMPLEMENT_DYNCREATE(CFormClassResult, CDialog)

/////////////////////////////////////////////////////////////////////////////
// CFormClassResult dialog


CFormClassResult::CFormClassResult(CWnd* pParent /*=NULL*/)
	: cdxCSizingDialog(CFormClassResult::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormClassResult)
	m_isline = FALSE;
	//}}AFX_DATA_INIT
}


void CFormClassResult::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormClassResult)
	DDX_Control(pDX, IDC_CHECK1, m_check);
	DDX_Control(pDX, IDC_LIST2, m_classreslist2);
	DDX_Control(pDX, IDC_GRAPH_TYPE, m_gratype);
	DDX_Control(pDX, IDC_PRINT, m_print);
	DDX_Control(pDX, IDC_EXPORT, m_export);
	DDX_Control(pDX, IDC_TYPE, m_type);
	DDX_Control(pDX, IDC_SATA, m_sata);
	DDX_Control(pDX, IDC_QUERY, m_query);
	DDX_Control(pDX, IDC_LIST1, m_classreslist);
	DDX_Check(pDX, IDC_CHECK1, m_isline);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormClassResult, cdxCSizingDialog)
	//{{AFX_MSG_MAP(CFormClassResult)
	ON_BN_CLICKED(IDC_QUERY, OnQuery)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	ON_BN_CLICKED(IDC_SATA, OnSata)
	ON_BN_CLICKED(IDC_EXPORT, OnExport)
	ON_BN_CLICKED(IDC_PRINT, OnPrint)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_GRAPH_TYPE, OnSelchangeGraphType)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, OnDblclkList2)
	ON_CBN_SELCHANGE(IDC_TYPE, OnSelchangeType)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormClassResult message handlers

BOOL CFormClassResult::OnInitDialog()
{
	cdxCSizingDialog::OnInitDialog();

	b_stat = FALSE;
	GetDlgItem(IDC_SATA)->SetWindowText("显示统计图");
	m_gratype.ShowWindow(FALSE);
	m_check.ShowWindow(FALSE);
	m_type.ShowWindow(FALSE);
	m_classreslist.ShowWindow(FALSE);

	CRect btnRect; 
	m_print.GetWindowRect(btnRect); 
	ScreenToClient(btnRect); 
	m_print.SetBitmapId(IDB_PRINT,IDB_PRINT,IDB_PRINT_DOWN,IDB_PRINT); 

	m_query.GetWindowRect(btnRect); 
	ScreenToClient(btnRect); 
	m_query.SetBitmapId(IDB_QUERY1,IDB_QUERY1,IDB_QUERY_DOWN1,IDB_QUERY1); 

	m_export.GetWindowRect(btnRect); 
	ScreenToClient(btnRect); 
	m_export.SetBitmapId(IDB_REPORT,IDB_REPORT,IDB_REPORT_DOWN,IDB_REPORT); 

	m_sata.GetWindowRect(btnRect); 
	ScreenToClient(btnRect); 
	m_sata.SetBitmapId(IDB_STATIC,IDB_STATIC,IDB_STATIC_DOWN,IDB_STATIC); 

	m_Static1.SubclassDlgItem(IDC_STATIC1,this);
	m_Static1.SetCaption("");
	m_Static1.SetBackColor(RGB(0,0,0));

	m_Static2.SubclassDlgItem(IDC_STATIC2,this);
	m_Static2.SetCaption("");
	m_Static2.SetBackColor(RGB(0,0,0));	

	AddSzControl(*GetDlgItem(IDC_QUERY),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_SATA),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_PRINT),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_TYPE),mdRepos,mdRepos);
	AddSzControl(*GetDlgItem(IDC_EXPORT),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_GRAPH_TYPE),mdRepos,mdRepos);
	
	AddSzControl(*GetDlgItem(IDC_STATIC1),mdRepos,mdRepos);
	AddSzControl(*GetDlgItem(IDC_STATIC2),mdRepos,mdRepos);
	AddSzControl(*GetDlgItem(IDC_CHECK1),mdRepos,mdRepos);
	
	
	AddSzControl(m_classreslist,mdResize,mdResize);	
	AddSzControl(m_classreslist2,mdResize,mdResize);
	DWORD dwStyle=GetWindowLong(m_classreslist.GetSafeHwnd(),GWL_STYLE);
	dwStyle&=~LVS_TYPEMASK;
	dwStyle|=LVS_REPORT;
	SetWindowLong(m_classreslist.GetSafeHwnd(),GWL_STYLE,dwStyle);  

	m_classreslist.SetExtendedStyle(LVS_EX_GRIDLINES);
    ::SendMessage(m_classreslist.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
     LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);


    ::SetWindowPos(m_hWnd,HWND_TOPMOST,0, 0, 0, 0,SWP_NOSIZE | SWP_NOMOVE  );

 	m_classreslist.ClearHeadings();
	m_classreslist.SetHeadings("试卷编号,0;学号,100; 姓名,100; 科目,100;试卷名称,150;开考时间,150;收卷时间,150;正确题数,100;错误题数,100;分数,100"); 

	dwStyle=GetWindowLong(m_classreslist2.GetSafeHwnd(),GWL_STYLE);
	dwStyle&=~LVS_TYPEMASK;
	dwStyle|=LVS_REPORT;
	SetWindowLong(m_classreslist2.GetSafeHwnd(),GWL_STYLE,dwStyle);  

	m_classreslist2.SetExtendedStyle(LVS_EX_GRIDLINES);
    ::SendMessage(m_classreslist2.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
     LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);


    ::SetWindowPos(m_hWnd,HWND_TOPMOST,0, 0, 0, 0,SWP_NOSIZE | SWP_NOMOVE  );

 	m_classreslist2.ClearHeadings();
	m_classreslist2.SetHeadings("试卷编号,0;学号,100; 姓名,100; 科目,100;试卷名称,150;开考时间,150;收卷时间,150;正确题数,100;错误题数,100;分数,100"); 
	return TRUE;
}

void CFormClassResult::OnQuery() 
{
	CDlgClassResult  dlg;
	if(dlg.DoModal() == IDOK)
	{
		m_classreslist.DeleteAllItems();
		m_classreslist2.DeleteAllItems();
		CStringArray arryStuid,arryStuname;
		arryStuid.RemoveAll();
		arryStuname.RemoveAll();
		BOOL bMark = FALSE;
// 		CoInitialize(NULL);
// 		CDBStuInfo m_StuInfodb;
// 		if(!m_StuInfodb.Connect())
// 		{
// 			AfxMessageBox("StuInfodb:无法连接到数据库服务器");
// 		}
// 		else
// 		{
// 			BOOL flag = m_StuInfodb.LoadByDepClass(dlg.strDepart, dlg.strClass);
// 			if(flag)
// 			{
// 				while(flag)
// 				{
// 					arryStuid.Add(m_StuInfodb.m_stuid);
// 					arryStuname.Add(m_StuInfodb.m_stuname);
// 					flag = m_StuInfodb.Move();
// 				}
// 				bMark = TRUE;
// 			}
// 			else
// 			{
// 				AfxMessageBox("该班级无学生信息!");
// 			}
// 			m_StuInfodb.Close();
// 		}
// 		CoUninitialize();
		if(!bMark)
			return;

		bMark = FALSE; 
		CStringArray arryPaperid, arryStartNO, arryEndNO;
		arryPaperid.RemoveAll();
		arryStartNO.RemoveAll();
		arryEndNO.RemoveAll();
// 		CoInitialize(NULL);
// 		CDBPaper m_Paperdb;
// 		if(!m_Paperdb.Connect() )
// 		{
// 			AfxMessageBox("Paperdb:无法连接到数据库服务器");
// 		}
// 		else
// 		{
// 			BOOL  flag = m_Paperdb.LoadByPaperSub(dlg.strSubject, dlg.strPaper);
// 			if(flag)
// 			{
// 				while(flag)
// 				{
// 					CString temp,temp1,temp2;
// 					temp.Format("%d", m_Paperdb.m_ydid);
// 					temp1.Format("%d", m_Paperdb.m_startNO);
// 					temp2.Format("%d", m_Paperdb.m_endNO);
// 					arryPaperid.Add(temp);
// 					arryStartNO.Add(temp1);
// 					arryEndNO.Add(temp2);
// 					flag = m_Paperdb.Move();
// 				}
// 				bMark = TRUE; 
// 			}
// 			else
// 			{
// 				AfxMessageBox("无该门试卷信息!");
// 			}
// 			m_Paperdb.Close();
// 		}
// 		CoUninitialize();
		if(!bMark)
			return;
		
// 		CoInitialize(NULL);
// 		CDBResult m_Resultdb;
// 		if(!m_Resultdb.Connect())
// 		{
// 			AfxMessageBox("Resultdb:无法连接到数据库服务器");
// 		}
// 		else
// 		{
// 			for(int i = 0; i < arryStuid.GetSize(); i++)
// 			{
// 				for(int j = 0; j < arryPaperid.GetSize(); j++)
// 				{
// 					int papetid = atoi(arryPaperid[j]);
// 					BOOL flag = m_Resultdb.LoadByQuery(arryStuid[i], papetid);
// 					if(flag)
// 					{
// 						int right = 0;
// 						int worng = 0;
// 						double score = 0;
// 						calculate(m_Resultdb.m_stuanswer, atoi(arryStartNO[j]),atoi(arryEndNO[j]),papetid,right,worng,score);
// 						CString sRight,sWorng,sSocrt;
// 						sRight.Format("%d",right);
// 						sWorng.Format("%d",worng);
// 						sSocrt.Format("%0.2f",score);						
// 						m_classreslist.AddItem(0,arryPaperid[j],arryStuid[i],arryStuname[i],
// 												dlg.strSubject, dlg.strPaper,
// 												m_Resultdb.m_starttime.Format(),
// 												m_Resultdb.m_endtime.Format(),
// 												sRight,sWorng,sSocrt);
// 						m_classreslist2.AddItem(0,arryPaperid[j],arryStuid[i],arryStuname[i],
// 												dlg.strSubject, dlg.strPaper,
// 												m_Resultdb.m_starttime.Format(),
// 												m_Resultdb.m_endtime.Format(),
// 												sRight,sWorng,sSocrt);
// 					}
// 				}
// 			}
// 			m_Resultdb.Close();
// 		}
// 		CoUninitialize();
	}


	if(m_classreslist2.GetItemCount() != 0 && b_stat == TRUE)
	{
		OnSelchangeType();
	}

}

BOOL CFormClassResult::calculate(CString strStuanswer, int start, int end, int paperid, int &right, int &worng, double &score)
{
	stdanswer = "";
	startNO = start;
	endNO = end;
	BOOL ret = TRUE;
// 	CDBStdanswer m_Stdanswerdb;
// 	if(!m_Stdanswerdb.Connect())
// 	{
// 		ret = FALSE;
// 		AfxMessageBox("StuInfodb:无法连接到数据库服务器");
// 	}
// 	else
// 	{
// 		BOOL flag = m_Stdanswerdb.LoadByPaperID(paperid);
// 		if(flag)
// 		{
// 			int temp = start;
// 			for(temp; temp <= end; temp++)
// 			{
// 				flag = m_Stdanswerdb.LoadByQuery(paperid, temp);
// 				if(flag)
// 				{
// 					CString key = m_Stdanswerdb.m_key;
// 					stdanswer += key;
// 					key.TrimLeft();
// 					key.TrimRight();
// 					CString stukey = strStuanswer.Mid(temp-start,1);
// 					if(stukey == key)
// 					{
// 						right++;
// 						score += m_Stdanswerdb.m_value;
// 					}
// 					else
// 						worng++;
// 				}
// 			}
// 		}
// 		else
// 		{
// 			ret = FALSE;
// 			AfxMessageBox("没有该门科目的标准答案，成绩将不做比对！");
// 		}
// 	}
	return ret;
}

void CFormClassResult::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int item =pNMListView->iItem;	
	CString strPaperid = m_classreslist.GetItemText(item,0);
	if(strPaperid != "")
	{
		CDlgResDetail  dlg;
		dlg.iPaperID = atoi(strPaperid);
		dlg.strStuid = m_classreslist.GetItemText(item,1);
		dlg.strStuname = m_classreslist.GetItemText(item,2);
		dlg.strSubject = m_classreslist.GetItemText(item,3);
		dlg.strPaper = m_classreslist.GetItemText(item,4);
		dlg.strRight = m_classreslist.GetItemText(item,7);
		dlg.strWorng = m_classreslist.GetItemText(item,8);
		dlg.strSocrt = m_classreslist.GetItemText(item,9);
		dlg.DoModal();
	}
	*pResult = 0;
}
void CFormClassResult::OnDblclkList2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int item =pNMListView->iItem;	
	CString strPaperid = m_classreslist2.GetItemText(item,0);
	if(strPaperid != "")
	{
		CDlgResDetail  dlg;
		dlg.iPaperID = atoi(strPaperid);
		dlg.strStuid = m_classreslist2.GetItemText(item,1);
		dlg.strStuname = m_classreslist2.GetItemText(item,2);
		dlg.strSubject = m_classreslist2.GetItemText(item,3);
		dlg.strPaper = m_classreslist2.GetItemText(item,4);
		dlg.strRight = m_classreslist2.GetItemText(item,7);
		dlg.strWorng = m_classreslist2.GetItemText(item,8);
		dlg.strSocrt = m_classreslist2.GetItemText(item,9);
		dlg.DoModal();
	}
	*pResult = 0;
}

void CFormClassResult::OnSata() 
{
	int iType = 0;
	if(m_classreslist.GetItemCount() <= 0 )
	{
		AfxMessageBox("没有查询到结果，无法显示统计图！");
		return;
	}

	if(b_stat == TRUE)
	{
		
		GetDlgItem(IDC_SATA)->SetWindowText("显示统计图");		
		b_stat = FALSE;

		m_gratype.ShowWindow(FALSE);
		m_type.ShowWindow(FALSE);
		
		m_check.ShowWindow(FALSE);
		m_Static1.SubclassDlgItem(IDC_STATIC1,this);
		m_Static1.SetCaption("");
		m_Static1.SetBackColor(RGB(0,0,0));

		m_Static2.SubclassDlgItem(IDC_STATIC2,this);
		m_Static2.SetCaption("");
		m_Static2.SetBackColor(RGB(0,0,0));

		m_classreslist.ShowWindow(FALSE);
		m_classreslist2.ShowWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_SATA)->SetWindowText("隐藏统计图");
		b_stat = TRUE;

		
		m_gratype.ShowWindow(TRUE);
		m_type.ShowWindow(TRUE);
		m_check.ShowWindow(TRUE);
		m_Static1.SubclassDlgItem(IDC_STATIC1,this);
		m_Static1.SetCaption("统计内容");
		m_Static1.SetBackColor(RGB(0,0,0));

		m_Static2.SubclassDlgItem(IDC_STATIC2,this);
		m_Static2.SetCaption("统计内容");
		m_Static2.SetBackColor(RGB(0,0,0));

		m_classreslist.ShowWindow(TRUE);
		m_classreslist2.ShowWindow(FALSE);
	}

	if(!b_init)
	{
		CStringArray  arryResult, arrtStuanswer;
		arryResult.RemoveAll();
		arrtStuanswer.RemoveAll();
		b_init = TRUE;
		if(iType == 0)
		{
			for(int i = 0; i < m_classreslist.GetItemCount(); i++)
			{
				CString strPerres = m_classreslist.GetItemText(i,9);
				arryResult.Add(strPerres);
			}		
			gra.ResultRange(arryResult,m_graphData);
		}
		if(iType == 1)
		{
// 			CoInitialize(NULL);
// 			CDBResult m_Resultdb;
// 			if(!m_Resultdb.Connect())
// 			{
// 				AfxMessageBox("Resultdb:无法连接到数据库服务器");
// 			}
// 			else
// 			{
// 				for(int i = 0; i < m_classreslist.GetItemCount(); i++)
// 				{
// 					CString strPaperid = m_classreslist.GetItemText(i,0);
// 					CString strStuid = m_classreslist.GetItemText(i,1);
// 					BOOL flag = m_Resultdb.LoadByQuery(strStuid, atoi(strPaperid));
// 					if(flag)
// 					{
// 						arrtStuanswer.Add(m_Resultdb.m_stuanswer);
// 					}
// 				}
// 				m_Resultdb.Close();
// 			}
// 			CoUninitialize();		
			gra.Wrongstat(startNO, endNO, stdanswer, arrtStuanswer, m_graphData);
		}
	}
	m_graphData.SetType(0);
	ShowGraph();
}

void CFormClassResult::OnExport() 
{
	CExportToExcel  ete;
	ete.ExportListToExcel(&m_classreslist,"成绩分布");	
	
}

void CFormClassResult::OnPrint() 
{
	if(m_classreslist.GetItemCount()<= 0)
		return;
	PRNINFO PrnInfo = {0};
	PrnInfo.hListView = m_classreslist.m_hWnd;
	PrnInfo.hWnd = this->m_hWnd;
	PrnInfo.IsPrint = FALSE;
	PrnInfo.nCurPage = 1;
	PrnInfo.nMaxLine = m_classreslist.GetItemCount();
	
	CPreParent DlgPreView;
	DlgPreView.SetCallBackFun(DrawInfo, PrnInfo);
	DlgPreView.DoModal();		
	
}
CString CFormClassResult::GetExePath()
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
void CFormClassResult::OnPaint() 
{
/*	CPaintDC dc(this); // device context for painting
	CRect rc;
    CJahCoverCapture capture;
	GetClientRect(&rc);
	capture.m_bmpFile =GetExePath()+"formback.bmp" ;
    if(!capture.LoadBMPImage(capture.m_bmpFile))
   		return;
    if (capture.m_bitmap.m_hObject == 0 )
    	return;
   capture.DrawBitmap(&dc,rc);*/

	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

int CFormClassResult::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rc;
	GetClientRect(rc);
	m_graph.Create(IDD_FORM_GRAPH,this);
	m_graph.ShowWindow(true);
	
	return 0;
}

void CFormClassResult::OnSize(UINT nType, int cx, int cy) 
{
	cdxCSizingDialog::OnSize(nType, cx, cy);
//	CRect lpRect1;
//	GetDlgItem(IDC_LIST1)->GetWindowRect(&lpRect1);
//	m_classreslist.GetWindowRect(&lpRect1);
//	ScreenToClient(&lpRect1);
//	int y = lpRect1.left;
	if ( ::IsWindow(m_graph.m_hWnd) )
	{	
	//	if ( GetDlgItem(IDC_REG) != NULL )
		{
			CRect lpRect;	
		    GetWindowRect(&lpRect);
			int t = lpRect.left;
		    lpRect.top = 220;
			lpRect.bottom = 440;
			lpRect.left = 10;
			lpRect.right = 800;
		    m_graph.MoveWindow(lpRect);
		}		
	}
	
}
void CFormClassResult::ShowGraph()
{
  if((m_graphData.m_type==PIE_GRAPH)||(m_graphData.m_type==PIE_GRAPH_3D))
	{
		CFileGraph data;
		data.Clear(); 		
		data.SetText(m_graphData.m_textTitle,m_graphData.m_textX,data.m_textY); 
		data.SetType(m_graphData.m_type);
		data.SetStep(0,100,10);	
        CStringArray mem;
		mem.RemoveAll();
		CUIntArray yList;
		yList.RemoveAll();
		for ( int i = 0 ; i < m_graphData.m_xList.GetSize() ; i ++ )
		{
			mem.Add(m_graphData.m_xList.GetAt(i));
			for ( int j= 0 ; j < m_graphData.m_yList[i].GetSize() ; j ++ )
			{
				yList.Add( m_graphData.m_yList[i].GetAt(j));
			}
		}
		data.AddData("对比图",yList);
	//	if ( mem.GetSize() < 15 )
			data.SetMem(mem);	  

		m_graph.DrawGraph(&data);
	}
	else
	{
		m_graph.DrawGraph(&m_graphData);
	}
}

void CFormClassResult::OnSelchangeGraphType() 
{
	if ( m_gratype.GetCurSel() >= 0 )
  		m_graphData.SetType( m_gratype.GetCurSel());
	ShowGraph();	
}
void CFormClassResult::OnCheck1() 
{
   UpdateData( TRUE );
  
   m_graphData.SetLine(m_isline);
   UpdateData( FALSE );
   OnSelchangeGraphType() ;		
}

void CFormClassResult::OnSelchangeType() 
{
	int iType = m_type.GetCurSel();
	if(iType < 0 )
	{
		AfxMessageBox("请选择统计图的种类！");
		return;
	}	

	m_graphData.Clear();
	CStringArray  arryResult, arrtStuanswer;
	arryResult.RemoveAll();
	arrtStuanswer.RemoveAll();

		if(iType == 0)
		{
			for(int i = 0; i < m_classreslist.GetItemCount(); i++)
			{
				CString strPerres = m_classreslist.GetItemText(i,9);
				arryResult.Add(strPerres);
			}		
			gra.ResultRange(arryResult,m_graphData);
		}
		if(iType == 1)
		{
// 			CoInitialize(NULL);
// 			CDBResult m_Resultdb;
// 			if(!m_Resultdb.Connect())
// 			{
// 				AfxMessageBox("Resultdb:无法连接到数据库服务器");
// 			}
// 			else
// 			{
// 				for(int i = 0; i < m_classreslist.GetItemCount(); i++)
// 				{
// 					CString strPaperid = m_classreslist.GetItemText(i,0);
// 					CString strStuid = m_classreslist.GetItemText(i,1);
// 					BOOL flag = m_Resultdb.LoadByQuery(strStuid, atoi(strPaperid));
// 					if(flag)
// 					{
// 						arrtStuanswer.Add(m_Resultdb.m_stuanswer);
// 					}
// 				}
// 				m_Resultdb.Close();
// 			}
// 			CoUninitialize();		
			gra.Wrongstat(startNO, endNO, stdanswer, arrtStuanswer, m_graphData);
		}

	m_graphData.SetType(0);
	ShowGraph();
	
}
void CFormClassResult::DrawInfo(CDC &memDC, PRNINFO PrnInfo)
{
	if(memDC.m_hDC == NULL)
		return;

	int nCurPage = PrnInfo.nCurPage;	//当前页
	BOOL IsPrint = PrnInfo.IsPrint;		//是否打印
	int nMaxPage = PrnInfo.nCountPage;	//最大页码
	HWND hWnd = PrnInfo.hWnd;
	HWND hList = PrnInfo.hListView;
	CString csLFinality, csRFinality;
	CTime time;
	time=CTime::GetCurrentTime();
	csLFinality = time.Format("报表日期:%Y-%m-%d");
	csRFinality.Format("第 %i 页/共 %i 页", nCurPage, nMaxPage);

	//CString temp = m_resultlist.GetItemText(0,2);

	
	TCHAR szTitle[32] = TEXT("成绩表");

	CRect rc, rt1, rt2, rt3, rt4, rt5, rt6, rt7, rt8;
	CPen *hPenOld;
	CPen		cPen;
	CFont TitleFont, DetailFont, *oldfont;
	//标题字体
	TitleFont.CreateFont(-MulDiv(14,memDC.GetDeviceCaps(LOGPIXELSY),72),
		0,0,0,FW_NORMAL,0,0,0,GB2312_CHARSET,
		OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,DRAFT_QUALITY,
		VARIABLE_PITCH|FF_SWISS,_T("黑体"));
	//细节字体
	DetailFont.CreateFont(-MulDiv(7,memDC.GetDeviceCaps(LOGPIXELSY),72),
		0,0,0,FW_NORMAL,0,0,0,GB2312_CHARSET,
		OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,DRAFT_QUALITY,
		VARIABLE_PITCH|FF_SWISS,_T("宋体"));
	//粗笔
	cPen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));

 	int xP = GetDeviceCaps(memDC.m_hDC, LOGPIXELSX);	//x方向每英寸像素点数
	int yP = GetDeviceCaps(memDC.m_hDC, LOGPIXELSY);	//y方向每英寸像素点数

	DOUBLE xPix = (DOUBLE)xP*10/254;	//每 mm 宽度的像素
	DOUBLE yPix = (DOUBLE)yP*10/254;	//每 mm 高度的像素
	DOUBLE fAdd = 7*yPix;		//每格递增量
	DOUBLE nTop = 25*yPix;		//第一页最上线
	int	  iStart = 0;			//从第几行开始读取
	DOUBLE nBottom = nTop+B5_ONELINE*fAdd;
	if(nCurPage != 1)
		nTop = 25*yPix-fAdd;	//非第一页最上线
	if(nCurPage == 2)
		iStart = B5_ONELINE;
	if(nCurPage>2)
		iStart = B5_ONELINE+(nCurPage - 2)*B5_OTHERLINE;

	DOUBLE nLeft = 10*xPix;			//最左线
	DOUBLE nRight = xPix*(B5_W-10);	//最右线



	DOUBLE nTextAdd = 1.5*xPix;
	if(IsPrint)
	{
		//真正打印部分
		static DOCINFO di = {sizeof (DOCINFO),  szTitle} ;
		//开始文档打印
		if(memDC.StartDoc(&di)<0)
		{
			::MessageBox(hWnd, "连接到打印机化败!", "错误", MB_ICONSTOP);
		}
		else
		{
			iStart = 0;
			nTop = 25*yPix;		//第一页最上线
			for(int iTotalPages = 1; iTotalPages<=nMaxPage; iTotalPages++)
			{
				int nCurPage = iTotalPages;
				csRFinality.Format("第 %i 页/共 %i 页", nCurPage, nMaxPage);
				time=CTime::GetCurrentTime();
				csLFinality = time.Format("报表日期:%Y-%m-%d");

				if(nCurPage != 1)
					nTop = 25*yPix-fAdd;	//非第一页最上线
				if(nCurPage == 2)
					iStart = B5_ONELINE;
				if(nCurPage>2)
					iStart = B5_ONELINE+(nCurPage - 2)*B5_OTHERLINE;
				//开始页
				if(memDC.StartPage() < 0)
				{
					::MessageBox(hWnd, _T("打印失败!"), "错误", MB_ICONSTOP);
					memDC.AbortDoc();
					return;
				}
				else
				{
					//打印
					//标题
					oldfont = memDC.SelectObject(&TitleFont);
					int nItem = B5_OTHERLINE;
					if(nCurPage == 1)
					{
						nItem = B5_ONELINE;
						rc.SetRect(0, yPix*10, B5_W*xPix, yPix*20);
						memDC.DrawText(szTitle, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					}
					//细节
 					memDC.SelectObject(&DetailFont);
					rc.SetRect(nLeft, nTop, nRight, nTop+fAdd);
					//上横线
					memDC.MoveTo(rc.left, rc.top);
					memDC.LineTo(rc.right, rc.top);

					rt1.SetRect(nLeft, nTop, nLeft+20*xPix, nTop+fAdd);					//学号
					rt2.SetRect(rt1.right, rt1.top, rt1.right + 20*xPix, rt1.bottom);	//姓名
					rt3.SetRect(rt2.right, rt1.top, rt2.right + 30*xPix, rt1.bottom);	//科目
					rt4.SetRect(rt3.right, rt1.top, rt3.right + 30*xPix, rt1.bottom);	//试卷名称
					rt5.SetRect(rt4.right, rt1.top, rt4.right + 30*xPix, rt1.bottom);	//开考时间
					rt6.SetRect(rt5.right, rt1.top, rt5.right + 10*xPix, rt1.bottom);	//正确题数
					rt7.SetRect(rt6.right, rt1.top, rt6.right + 10*xPix, rt1.bottom);	//错误题数
					rt8.SetRect(rt7.right, rt1.top, rc.right, rt1.bottom);	//分数

					memDC.DrawText("学号", &rt1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					memDC.DrawText("姓名", &rt2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					memDC.DrawText("科目", &rt3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					memDC.DrawText("试卷名称", &rt4, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					memDC.DrawText("开考时间", &rt5, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					memDC.DrawText("正确题数", &rt6, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					memDC.DrawText("错误题数", &rt7, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					memDC.DrawText("分数", &rt8, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

					memDC.MoveTo(rt1.right, rt1.top);
					memDC.LineTo(rt1.right, rt1.bottom);
					memDC.MoveTo(rt2.right, rt1.top);
					memDC.LineTo(rt2.right, rt1.bottom);
					memDC.MoveTo(rt3.right, rt1.top);
					memDC.LineTo(rt3.right, rt1.bottom);
					memDC.MoveTo(rt4.right, rt1.top);
					memDC.LineTo(rt4.right, rt1.bottom);
					memDC.MoveTo(rt5.right, rt1.top);
					memDC.LineTo(rt5.right, rt1.bottom);
					memDC.MoveTo(rt6.right, rt1.top);
					memDC.LineTo(rt6.right, rt1.bottom);
					memDC.MoveTo(rt7.right, rt1.top);
					memDC.LineTo(rt7.right, rt1.bottom);
					memDC.MoveTo(rt8.right, rt1.top);
					memDC.LineTo(rt8.right, rt1.bottom);
					memDC.MoveTo(rc.left, rt1.bottom);
					memDC.LineTo(rc.right, rt1.bottom);

					TCHAR szID[32]={0}, szName[32]={0}, szSub[32]={0}, szPP[32]={0}, szStart[32]={0}, Right[32]={0}, Worng[32]={0}, Res[32]={0};
					rc.SetRect(nLeft, nTop+fAdd, nRight, nTop+2*fAdd);
					rt1.SetRect(nLeft+nTextAdd, rc.top, nLeft+20*xPix, rc.bottom);				//学号
					rt2.SetRect(rt1.right+nTextAdd, rt1.top, rt1.right + 20*xPix, rt1.bottom);	//姓名
					rt3.SetRect(rt2.right+nTextAdd, rt1.top, rt2.right + 30*xPix, rt1.bottom);	//性别
					rt4.SetRect(rt3.right+nTextAdd, rt1.top, rt3.right + 30*xPix, rt1.bottom);	//专业
					rt5.SetRect(rt4.right+nTextAdd, rt1.top, rt4.right + 30*xPix, rt1.bottom);	//年级
					rt6.SetRect(rt5.right+nTextAdd, rt1.top, rt5.right + 10*xPix, rt1.bottom);				//班级
					rt7.SetRect(rt6.right+nTextAdd, rt1.top, rt6.right + 10*xPix, rt1.bottom);	//年级
					rt8.SetRect(rt7.right+nTextAdd, rt1.top, rc.right, rt1.bottom);				//班级

					int nCountItem = ListView_GetItemCount(hList);
					for(int i=0;i<nItem; i++)
					{
						ListView_GetItemText(hList, i+iStart, 1, szID, 32);
						ListView_GetItemText(hList, i+iStart, 2, szName, 32);
						ListView_GetItemText(hList, i+iStart, 3, szSub, 32);
						ListView_GetItemText(hList, i+iStart, 4, szPP, 32);
						ListView_GetItemText(hList, i+iStart, 5, szStart, 32);
						ListView_GetItemText(hList, i+iStart, 7, Right, 32);
						ListView_GetItemText(hList, i+iStart, 8, Worng, 32);
						ListView_GetItemText(hList, i+iStart, 9, Res, 32);
						
						memDC.DrawText(szID, &rt1, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
						memDC.DrawText(szName, &rt2, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
						memDC.DrawText(szSub, &rt3, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
						memDC.DrawText(szPP, &rt4, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
						memDC.DrawText(szStart, &rt5, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
						memDC.DrawText(Right, &rt6, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
						memDC.DrawText(Worng, &rt7, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
						memDC.DrawText(Res, &rt8, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
						//下横线
						memDC.MoveTo(rc.left, rc.bottom);
						memDC.LineTo(rc.right, rc.bottom);
						memDC.MoveTo(rt1.right, rt1.top);
						memDC.LineTo(rt1.right, rt1.bottom);
						memDC.MoveTo(rt2.right, rt1.top);
						memDC.LineTo(rt2.right, rt1.bottom);
						memDC.MoveTo(rt3.right, rt1.top);
						memDC.LineTo(rt3.right, rt1.bottom);
						memDC.MoveTo(rt4.right, rt1.top);
						memDC.LineTo(rt4.right, rt1.bottom);
						memDC.MoveTo(rt5.right, rt1.top);
						memDC.LineTo(rt5.right, rt1.bottom);
						memDC.MoveTo(rt6.right, rt1.top);
						memDC.LineTo(rt6.right, rt1.bottom);
						memDC.MoveTo(rt7.right, rt1.top);
						memDC.LineTo(rt7.right, rt1.bottom);
						memDC.MoveTo(rt8.right, rt1.top);
						memDC.LineTo(rt8.right, rt1.bottom);
						memDC.MoveTo(rc.left, rt1.bottom);
						memDC.LineTo(rc.right, rt1.bottom);
						
						rc.top += fAdd;
						rc.bottom += fAdd;
						rt1.top = rc.top;
						rt1.bottom = rc.bottom;
						rt2.top = rt1.top;
						rt2.bottom = rt1.bottom;
						rt3.top = rt1.top;
						rt3.bottom = rt1.bottom;
						rt4.top = rt1.top;
						rt4.bottom = rt1.bottom;
						rt5.top = rt1.top;
						rt5.bottom = rt1.bottom;
						rt6.top = rt1.top;
						rt6.bottom = rt1.bottom;
						rt7.top = rt1.top;
						rt7.bottom = rt1.bottom;
						rt8.top = rt1.top;
						rt8.bottom = rt1.bottom;
						
						if((i+iStart+1)>=nCountItem)
							break;
 					}
					//结尾
					memDC.MoveTo(rc.left, nTop);
					memDC.LineTo(rc.left, rc.top);
					memDC.MoveTo(rc.right, nTop);
					memDC.LineTo(rc.right, rc.top);
					memDC.DrawText(csLFinality, &rc, DT_LEFT| DT_VCENTER | DT_SINGLELINE);
					memDC.DrawText(csRFinality, &rc, DT_RIGHT| DT_VCENTER | DT_SINGLELINE);
					memDC.EndPage();
					memDC.SelectObject(oldfont);
				}
			}
			memDC.EndDoc();
		}
	}
	else
	{
		//打印预览
		
		//边框线
		hPenOld = memDC.SelectObject(&cPen);
		rc.SetRect(0, 0, B5_W*xPix, B5_H*yPix);
		memDC.Rectangle(&rc);
		memDC.SelectObject(hPenOld);	
		//标题
		oldfont = memDC.SelectObject(&TitleFont);
		int nItem = B5_OTHERLINE;
		if(nCurPage == 1)
		{
			nItem = B5_ONELINE;
			rc.SetRect(0, yPix*10, B5_W*xPix, yPix*20);
			memDC.DrawText(szTitle, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		//细节
 		memDC.SelectObject(&DetailFont);
		rc.SetRect(nLeft, nTop, nRight, nTop+fAdd);
		//上横线
		memDC.MoveTo(rc.left, rc.top);
		memDC.LineTo(rc.right, rc.top);

		rt1.SetRect(nLeft, nTop, nLeft+20*xPix, nTop+fAdd);					//学号
		rt2.SetRect(rt1.right, rt1.top, rt1.right + 20*xPix, rt1.bottom);	//姓名
		rt3.SetRect(rt2.right, rt1.top, rt2.right + 30*xPix, rt1.bottom);	//科目
		rt4.SetRect(rt3.right, rt1.top, rt3.right + 30*xPix, rt1.bottom);	//试卷名称
		rt5.SetRect(rt4.right, rt1.top, rt4.right + 30*xPix, rt1.bottom);	//开考时间
		rt6.SetRect(rt5.right, rt1.top, rt5.right + 10*xPix, rt1.bottom);	//正确题数
		rt7.SetRect(rt6.right, rt1.top, rt6.right + 10*xPix, rt1.bottom);	//错误题数
		rt8.SetRect(rt7.right, rt1.top, rc.right, rt1.bottom);	//分数
		memDC.DrawText("学号", &rt1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		memDC.DrawText("姓名", &rt2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		memDC.DrawText("科目", &rt3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		memDC.DrawText("试卷名称", &rt4, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		memDC.DrawText("开考时间", &rt5, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		memDC.DrawText("正确题数", &rt6, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		memDC.DrawText("错误题数", &rt7, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		memDC.DrawText("分数", &rt8, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		memDC.MoveTo(rt1.right, rt1.top);
		memDC.LineTo(rt1.right, rt1.bottom);
		memDC.MoveTo(rt2.right, rt1.top);
		memDC.LineTo(rt2.right, rt1.bottom);
		memDC.MoveTo(rt3.right, rt1.top);
		memDC.LineTo(rt3.right, rt1.bottom);
		memDC.MoveTo(rt4.right, rt1.top);
		memDC.LineTo(rt4.right, rt1.bottom);
		memDC.MoveTo(rt5.right, rt1.top);
		memDC.LineTo(rt5.right, rt1.bottom);
		memDC.MoveTo(rt6.right, rt1.top);
		memDC.LineTo(rt6.right, rt1.bottom);
		memDC.MoveTo(rt7.right, rt1.top);
		memDC.LineTo(rt7.right, rt1.bottom);
		memDC.MoveTo(rt8.right, rt1.top);
		memDC.LineTo(rt8.right, rt1.bottom);
		memDC.MoveTo(rc.left, rt1.bottom);
		memDC.LineTo(rc.right, rt1.bottom);

		TCHAR szID[32]={0}, szName[32]={0}, szSub[32]={0}, szPP[32]={0}, szStart[32]={0}, Right[32]={0}, Worng[32]={0}, Res[32]={0};
		rc.SetRect(nLeft, nTop+fAdd, nRight, nTop+2*fAdd);
		rt1.SetRect(nLeft+nTextAdd, rc.top, nLeft+20*xPix, rc.bottom);				//学号
		rt2.SetRect(rt1.right+nTextAdd, rt1.top, rt1.right + 20*xPix, rt1.bottom);	//姓名
		rt3.SetRect(rt2.right+nTextAdd, rt1.top, rt2.right + 30*xPix, rt1.bottom);	//性别
		rt4.SetRect(rt3.right+nTextAdd, rt1.top, rt3.right + 30*xPix, rt1.bottom);	//专业
		rt5.SetRect(rt4.right+nTextAdd, rt1.top, rt4.right + 30*xPix, rt1.bottom);	//年级
		rt6.SetRect(rt5.right+nTextAdd, rt1.top, rt5.right + 10*xPix, rt1.bottom);				//班级
		rt7.SetRect(rt6.right+nTextAdd, rt1.top, rt6.right + 10*xPix, rt1.bottom);	//年级
		rt8.SetRect(rt7.right+nTextAdd, rt1.top, rc.right, rt1.bottom);				//班级

		int nCountItem = ListView_GetItemCount(hList);
		for(int i=0;i<nItem; i++)
		{
			ListView_GetItemText(hList, i+iStart, 1, szID, 32);
			ListView_GetItemText(hList, i+iStart, 2, szName, 32);
			ListView_GetItemText(hList, i+iStart, 3, szSub, 32);
			ListView_GetItemText(hList, i+iStart, 4, szPP, 32);
			ListView_GetItemText(hList, i+iStart, 5, szStart, 32);
			ListView_GetItemText(hList, i+iStart, 7, Right, 32);
			ListView_GetItemText(hList, i+iStart, 8, Worng, 32);
			ListView_GetItemText(hList, i+iStart, 9, Res, 32);
			
			memDC.DrawText(szID, &rt1, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			memDC.DrawText(szName, &rt2, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			memDC.DrawText(szSub, &rt3, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			memDC.DrawText(szPP, &rt4, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			memDC.DrawText(szStart, &rt5, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			memDC.DrawText(Right, &rt6, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			memDC.DrawText(Worng, &rt7, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			memDC.DrawText(Res, &rt8, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			//下横线
			memDC.MoveTo(rc.left, rc.bottom);
			memDC.LineTo(rc.right, rc.bottom);
			memDC.MoveTo(rt1.right, rt1.top);
			memDC.LineTo(rt1.right, rt1.bottom);
			memDC.MoveTo(rt2.right, rt1.top);
			memDC.LineTo(rt2.right, rt1.bottom);
			memDC.MoveTo(rt3.right, rt1.top);
			memDC.LineTo(rt3.right, rt1.bottom);
			memDC.MoveTo(rt4.right, rt1.top);
			memDC.LineTo(rt4.right, rt1.bottom);
			memDC.MoveTo(rt5.right, rt1.top);
			memDC.LineTo(rt5.right, rt1.bottom);
			memDC.MoveTo(rt6.right, rt1.top);
			memDC.LineTo(rt6.right, rt1.bottom);
			memDC.MoveTo(rt7.right, rt1.top);
			memDC.LineTo(rt7.right, rt1.bottom);
			memDC.MoveTo(rt8.right, rt1.top);
			memDC.LineTo(rt8.right, rt1.bottom);
			memDC.MoveTo(rc.left, rt1.bottom);
			memDC.LineTo(rc.right, rt1.bottom);
			
			rc.top += fAdd;
			rc.bottom += fAdd;
			rt1.top = rc.top;
			rt1.bottom = rc.bottom;
			rt2.top = rt1.top;
			rt2.bottom = rt1.bottom;
			rt3.top = rt1.top;
			rt3.bottom = rt1.bottom;
			rt4.top = rt1.top;
			rt4.bottom = rt1.bottom;
			rt5.top = rt1.top;
			rt5.bottom = rt1.bottom;
			rt6.top = rt1.top;
			rt6.bottom = rt1.bottom;
			rt7.top = rt1.top;
			rt7.bottom = rt1.bottom;
			rt8.top = rt1.top;
			rt8.bottom = rt1.bottom;
			
			if((i+iStart+1)>=nCountItem)
				break;
 		}
		//结尾
		memDC.MoveTo(rc.left, nTop);
		memDC.LineTo(rc.left, rc.top);
		memDC.MoveTo(rc.right, nTop);
		memDC.LineTo(rc.right, rc.top);
		memDC.DrawText(csLFinality, &rc, DT_LEFT| DT_VCENTER | DT_SINGLELINE);
		memDC.DrawText(csRFinality, &rc, DT_RIGHT| DT_VCENTER | DT_SINGLELINE);

		memDC.SelectObject(oldfont);
		memDC.SelectObject(hPenOld);
	}
	TitleFont.DeleteObject();
	DetailFont.DeleteObject();
	cPen.DeleteObject();
}


