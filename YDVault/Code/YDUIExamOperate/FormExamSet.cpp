#include "stdafx.h"
#include "FormExamSet.h"
#include "DlgPaperQuery.h"
#include "DlgPaperset.h"
#include "../ydcom/ExamDetail.h"
#include "DlgTeacherSet.h"
#include "../Base\AutoClean.h"
#include "../DBBase\DatabaseEx.h"
#include "../ObjHelper\StaticObjHelper.h"
#include "../YDExamObjRef\YDClassRef.h"
#include "../ObjRef/YDPaper.h"


IMPLEMENT_DYNCREATE(CFormExamSet, CYdFormView)

CFormExamSet::CFormExamSet()
	: CYdFormView(CFormExamSet::IDD)
{
	//{{AFX_DATA_INIT(CFormExamSet)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CFormExamSet::~CFormExamSet()
{
	Clear();
}


HRESULT CFormExamSet::Clear()
{
	HRESULT hr = E_FAIL;
	CListAutoClean<CYDObjectRef> clr1(m_lstClass);
	CListAutoClean<CYDObjectRef> clr2(m_lstExamAddr);
	CListAutoClean<CYDObjectRef> clr3(m_lstPaperRef);
	return S_OK;
}
void CFormExamSet::DoDataExchange(CDataExchange* pDX)
{
	CYdFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormExamSet)
	DDX_Control(pDX, IDC_SET, m_set);
	DDX_Control(pDX, IDC_UP, m_up);
	DDX_Control(pDX, IDC_SHOWALL, m_showall);
	DDX_Control(pDX, IDC_PAPERQUERY, m_paperquery);
	DDX_Control(pDX, IDC_PAPERLIST, m_paperlist);
	DDX_Control(pDX, IDC_NEXT, m_next);
	DDX_Control(pDX, IDC_CLASSLIST, m_classlist);
	DDX_Control(pDX, IDC_ADDR, m_address);
	DDX_Control(pDX, IDC_ADDPAPER, m_addpaper);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormExamSet, CYdFormView)
	//{{AFX_MSG_MAP(CFormExamSet)
	ON_BN_CLICKED(IDC_UP, OnUp)
	ON_BN_CLICKED(IDC_SHOWALL, OnShowall)
	ON_BN_CLICKED(IDC_PAPERQUERY, OnPaperquery)
	ON_BN_CLICKED(IDC_ADDPAPER, OnAddpaper)
	ON_BN_CLICKED(IDC_NEXT, OnNext)
	ON_NOTIFY(NM_DBLCLK, IDC_PAPERLIST, OnDblclkPaperlist)
	ON_NOTIFY(NM_DBLCLK, IDC_CLASSLIST, OnDblclkClasslist)
	ON_BN_CLICKED(IDC_SET, OnSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormExamSet message handlers

void CFormExamSet::OnInitialUpdate() 
{
	CYdFormView::OnInitialUpdate();
	
	
	CRect btnRect;  
	m_next.GetWindowRect(btnRect); 
	ScreenToClient(btnRect); 
	m_next.SetBitmapId(IDB_NEXT,IDB_NEXT,IDB_NEXT_DOWN,IDB_NEXT); 

	m_showall.GetWindowRect(btnRect); 
	ScreenToClient(btnRect); 
	m_showall.SetBitmapId(IDB_SHOWALL,IDB_SHOWALL,IDB_SHOWALL_DOWN,IDB_SHOWALL); 

	m_paperquery.GetWindowRect(btnRect); 
	ScreenToClient(btnRect); 
	m_paperquery.SetBitmapId(IDB_PAPERQUERY,IDB_PAPERQUERY,IDB_PAPERQUERY_DOWN,IDB_PAPERQUERY); 

	m_addpaper.GetWindowRect(btnRect); 
	ScreenToClient(btnRect); 
	m_addpaper.SetBitmapId(IDB_ADDPAPER,IDB_ADDPAPER,IDB_ADDPAPER_DOWN,IDB_ADDPAPER); 

	m_up.GetWindowRect(btnRect); 
	ScreenToClient(btnRect); 
	m_up.SetBitmapId(IDB_UP,IDB_UP,IDB_UP_DOWN,IDB_UP); 

	m_set.GetWindowRect(btnRect); 
	ScreenToClient(btnRect); 
	m_set.SetBitmapId(IDB_SET,IDB_SET,IDB_SET_DOWN,IDB_SET); 

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
	m_paperlist.SetHeadings(_T("考卷名称,150;考试科目,100; 开考时间,150;收卷试卷,150;开始题号,100;结束题号,100;最大题号,100")); 	
	

	dwStyle=GetWindowLong(m_classlist.GetSafeHwnd(),GWL_STYLE);
	dwStyle&=~LVS_TYPEMASK;
	dwStyle|=LVS_REPORT;
	SetWindowLong(m_classlist.GetSafeHwnd(),GWL_STYLE,dwStyle);  

	m_classlist.SetExtendedStyle(LVS_EX_GRIDLINES);
    ::SendMessage(m_classlist.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
     LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	dwStyle = m_classlist.GetExtendedStyle();
	dwStyle |= LVS_EX_CHECKBOXES;
    m_classlist.SetExtendedStyle(dwStyle);


    ::SetWindowPos(m_hWnd,HWND_TOPMOST,0, 0, 0, 0,SWP_NOSIZE | SWP_NOMOVE  );

 	m_classlist.ClearHeadings();
	m_classlist.SetHeadings(_T("班级编号,100;班级名称,100; 所属院系,100; 班主任,100")); 


	
// 		strSql.Format("select cl.classid, cl.classname, de.departname, cl.head \
// 			from ydclass cl, yddepartment de \
// 			where cl.DEPARTID = de.DEPARTID;");

	//显示所有班级？要问wht
	HRESULT hr = E_FAIL;
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	hr = CStaticObjHelper::GetObjRef(DB_YDCLASS,pDB,&m_lstClass);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	for(std::list<CYDObjectRef*>::const_iterator itr = m_lstClass.begin();
		itr != m_lstClass.end();++itr)
	{
		CYDClassRef* pClassRef = (CYDClassRef*)(*itr);
		ASSERT(pClassRef);
		CString strClassID;
		hr = pClassRef->GetPropVal(FIELD_YDCLASS_CLASSID,strClassID);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		CString strClassName;
		hr = pClassRef->GetPropVal(FIELD_YDCLASS_NAME,strClassName);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		CYDObjectRef* pDepartRef = NULL;
//		hr = pClassRef->GetDepartRef(pDepartRef);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		CPtrAutoClean<CYDObjectRef> clr(pDepartRef);
		CString strDepartName;
		hr = pDepartRef->GetPropVal(FIELD_YDTEACHERDEV_NAME,strDepartName);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		CString strClassHead;
		hr = pClassRef->GetPropVal(FIELD_YDCLASS_HEAD,strClassHead);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		int index = m_classlist.AddItem(0, strClassID, strClassName,strDepartName,strClassHead);
		m_classlist.SetItemData(index,(DWORD_PTR)pClassRef);
	}
	//strSql.Format("select examaddr from ydexamaddr;");
	
	hr = CStaticObjHelper::GetObjRef(_T("ydexamaddr"),pDB,&m_lstExamAddr);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	for(std::list<CYDObjectRef*>::const_iterator itr = m_lstExamAddr.begin();
		itr != m_lstExamAddr.end();++itr)
	{
		CString sAddr;
		hr = (*itr)->GetPropVal(L"examaddr",sAddr);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		sAddr.TrimLeft();
		sAddr.TrimRight();
		if(sAddr != "")
		{
			int temp = m_address.FindStringExact(-1, sAddr);
			if(temp == -1)
			{
				int index = m_address.AddString(sAddr);
				m_address.SetItemData(index,(DWORD_PTR)(*itr));
			}
		}
	}
	return ;  
}

void CFormExamSet::OnUp() 
{
// 	HRESULT hr = E_FAIL;
// 	CGExamStruct* pGExamStruct = NULL;
// 	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
// 	if(FAILED(hr))
// 	{
// 		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
// 		return ;
// 	}
// 	ASSERT(pGExamStruct);
// 	pGExamStruct->m_pExamDetail->nSetp = 1;
// 	if (m_kind == 17)
// 	{
// // 		CMainFrame *fram = (CMainFrame *)AfxGetApp()->m_pMainWnd;	
// // 		fram->m_dlgMain.ShowDlg(IDD_FORM_EXMA_INFO_BACK);
// 	}
// 	else
// 	{
// // 		CMainFrame *fram = (CMainFrame *)AfxGetApp()->m_pMainWnd;	
// // 		fram->m_dlgMain.ShowDlg(IDD_FORM_EXAM_MODE);
// 
// 	}
}

void CFormExamSet::OnShowall() 
{
	m_paperlist.DeleteAllItems();
// 	strSql.Format("select pa.papername, sub.subject, 
	//to_char(pa.starttime, 'yyyy-mm-dd hh24:mi:ss'),
	//to_char(pa.endtime, 'yyyy-mm-dd hh24:mi:ss'),
	//pa.startno, pa.endno, pa.maxno \
// 		from ydpaper pa, ydsubject sub, ydclass cl \
// 		where pa.SUBJECTID = sub.SUBJECTID and pa.CLASSID = cl.CLASSID;");
	HRESULT hr = E_FAIL;
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	hr = CStaticObjHelper::GetObjRef(DB_PAPER,pDB,&m_lstPaperRef);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	for(std::list<CYDObjectRef*>::const_iterator itr = m_lstPaperRef.begin();
		itr != m_lstPaperRef.end();++itr)
	{
		CYDPaper* pPaperRef = (CYDPaper*)(*itr);
		ASSERT(pPaperRef);
		CString strPaperName;
		hr = pPaperRef->GetPropVal(FIELD_YDPAPER_NAME,strPaperName);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		CYDObjectRef* pSubjectRef = NULL;
	/*	hr = pPaperRef->GetYDSubjectRef(pSubjectRef);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}*/
		CPtrAutoClean<CYDObjectRef> clr(pSubjectRef);
		CString strSubject;
		hr = pSubjectRef->GetPropVal(FIELD_YDSUBJECT_NAME,strSubject);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		CString strStartTime;
	/*	hr = pPaperRef->GetPropVal(FIELD_YDPAPER_STARTTIME,strStartTime);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}*/
		CString strEndTime;
	/*	hr = pPaperRef->GetPropVal(FIELD_YDPAPER_ENDTIME,strEndTime);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}*/
		CString strStartNo;
		/*hr = pPaperRef->GetPropVal(FIELD_YDPAPER_STARTNO,strStartNo);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}*/
		CString strEndNo;
		/*hr = pPaperRef->GetPropVal(FIELD_YDPAPER_ENDNO,strEndNo);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}*/
		CString strMaxNo;
	/*	hr = pPaperRef->GetPropVal(FIELD_YDPAPER_MAXNO,strMaxNo);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}*/
		int index = m_paperlist.AddItem(0,
										strPaperName, 
										strSubject,
										strStartTime,
										strEndTime,
										strStartNo,
										strEndNo,
										strMaxNo
										);
		m_paperlist.SetItemData(index,(DWORD_PTR)pPaperRef);
	}
}

void CFormExamSet::OnPaperquery() 
{
	m_paperlist.DeleteAllItems();
	CDlgPaperQuery  dlg;
	if(dlg.DoModal() == IDOK)
	{
		CString strPapername = dlg.strPaper;
		CString strSubject = dlg.strSubject;
		CString	strClass = dlg.strClass;
		CString strDepart = dlg.strDepart;
		CString strTime = dlg.m_time.Format();
		CString strSql;
		strSql.Format(_T("select pa.PAPERNAME, sub.SUBJECT, pa.STARTTIME, pa.ENDTIME, pa.STARTNO, pa.ENDNO, pa.MAXNO \
			from ydpaper pa, ydclass cl, yddepartment de, ydsubject sub where "));
		
		CString strFilter1, strFilter2, strFilter3, strFilter4, strFilter5;
		if(strPapername != "")
		{
			strFilter1.Format(_T("pa.PAPERNAME = '%s' "), strPapername);
		}
		else
		{
			strFilter1.Format(_T("pa.PAPERID >=0 "));
		}

		if(strSubject != "")
		{
			strFilter2.Format(_T("and sub.SUBJECT = '%s' and pa.SUBJECTID = sub.SUBJECTID "), strSubject);
		}
		else
		{
			strFilter2.Format(_T("and pa.PAPERID >=0 and pa.SUBJECTID = sub.SUBJECTID "));
		}

		if(strDepart != "")
		{
			strFilter3.Format(_T("and de.DEPARTNAME = '%s' and cl.DEPARTID = de.DEPARTID and pa.CLASSID = cl.CLASSID "), strDepart);
		}
		else
		{
			strFilter3.Format(_T("and pa.PAPERID >=0 and cl.DEPARTID = de.DEPARTID and pa.CLASSID = cl.CLASSID "));
		}

		if(strClass != "")
		{
			strFilter4.Format(_T("and cl.CLASSNAME = '%s' and cl.CLASSID = pa.CLASSID "), strClass);
		}
		else
		{
			strFilter4.Format(_T("and pa.PAPERID >=0 and cl.CLASSID = pa.CLASSID "));
		}

		if(dlg.bIstime)
		{
			strFilter5.Format(_T("and pa.starttime >=to_date('%d-%d-%d','yyyy-mm-dd') and pa.starttime < to_date('%d-%d-%d','yyyy-mm-dd')+1"),
							dlg.m_time.GetYear(), 
							dlg.m_time.GetMonth(),
							dlg.m_time.GetDay(),
							dlg.m_time.GetYear(),
							dlg.m_time.GetMonth(),
							dlg.m_time.GetDay());
		}
		else
			strFilter5.Format(_T("and pa.PAPERID >=0 "));

		strSql += strFilter1 + strFilter2 + strFilter3 +strFilter4 +strFilter5;

// 		CMyODBC db;
// 		BOOL flag =  db.ConnectDB();
// 		if(flag)
// 		{
// 			CODBCSet set;
// 			db.PrepareSql(strSql, set);
// 			while(db.FetchData())	
// 			{
// 				m_paperlist.AddItem(0, set.m_coldata[0].value, set.m_coldata[1].value,set.m_coldata[2].value,set.m_coldata[3].value,set.m_coldata[4].value,set.m_coldata[5].value,set.m_coldata[6].value,set.m_coldata[7].value);			
// 			}
// 			set.Empty();
// 			db.DisConnect();
// 		}
	}	
}

void CFormExamSet::OnAddpaper() 
{
	CDlgPaperset dlg;
	if(dlg.DoModal() == IDOK)
	{
		CString strPapername = dlg.strPaper;
		CString strSubject = dlg.strSubject;
		CString strClass = dlg.strClass;
		CString strStart = dlg.tStart.Format();
		CString strEnd = dlg.tEnd.Format();
		CString strStartNo, strEndNo, strMaxNo;
		strStartNo.Format(_T("%d"), dlg.m_startNO);
		strEndNo.Format(_T("%d"), dlg.m_endNO);
		strMaxNo.Format(_T("%d"), dlg.m_maxNO);

// 		CString strSql;
// 		strSql.Format("insert into ydpaper(papername,subjectid, classid, starttime, endtime, startno, endno, maxno) \
// 			values('%s', (select subjectid from ydsubject where subject = '%s'), (select classid from ydclass where classname = '%s'), to_date('%s', 'yyyy-mm-dd hh24:mi:ss'), to_date('%s', 'yyyy-mm-dd hh24:mi:ss'), %d, %d, %d);", 
// 			strPapername, strSubject, strClass, strStart, strEnd, dlg.m_startNO, dlg.m_endNO, dlg.m_maxNO);	
// 
// 		CMyODBC db;
// 		BOOL flag =  db.ConnectDB();
// 		if(flag)
// 		{	
// 			if(db.ExeSqlDirect(strSql))
// 			{
// 				AfxMessageBox("增加成功!");
// 			}
// 			else
// 			{
// 				AfxMessageBox("增加失败!");
// 			}
// 			db.DisConnect();
// 		}
	}
}

void CFormExamSet::OnNext() 
{
	HRESULT hr = E_FAIL;
// 	CGExamStruct* pGExamStruct = NULL;
// 	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
// 	if(FAILED(hr))
// 	{
// 		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
// 		return ;
// 	}
// 	ASSERT(pGExamStruct);
// 	pGExamStruct->m_pExamDetail->nSetp = 3;
// 	int iCount = 0;
// 	for(int i = 0; i < m_paperlist.GetItemCount(); i++)
// 	{
// 		if(m_paperlist.GetCheck(i))
// 			iCount++;
// 	}
// 	if(iCount == 0)
// 	{
// 		AfxMessageBox(_T("没有选择试卷，请返回选择需要考试的试卷！"));
// 		return;
// 	}
// 	if(iCount > 1)
// 	{
// 		AfxMessageBox(_T("选择了多张试卷，请返回重新选择！"));
// 		return;
// 	}
// 
// 	iCount = 0;
// 	for(int i = 0; i < m_classlist.GetItemCount(); i++)
// 	{
// 		if(m_classlist.GetCheck(i))
// 			iCount++;
// 	}
// 	if(iCount == 0)
// 	{
// 		AfxMessageBox(_T("没有选择本场考试的班级，请返回选择需要考试的班级！"));
// 		return;
// 	}
// 	CString sPaper,sSubject,sStarttime,sEndtime,sStartNO,sEndNO,sMaxNO;
// 	for(int i = 0; i < m_paperlist.GetItemCount(); i++)
// 	{
// 		if(m_paperlist.GetCheck(i))
// 		{
// 			sPaper = m_paperlist.GetItemText(i,0);
// 			sSubject = m_paperlist.GetItemText(i,1);
// 			sStarttime = m_paperlist.GetItemText(i,2);
// 			sEndtime = m_paperlist.GetItemText(i,3);
// 			sStartNO = m_paperlist.GetItemText(i,4);
// 			sEndNO = m_paperlist.GetItemText(i,5);
// 			sMaxNO = m_paperlist.GetItemText(i,6);
		
// 			CMyODBC db;
// 			BOOL flag =  db.ConnectDB();
// 			if(flag)
// 			{
// 				CString strSql;
// 				strSql.Format("select paperid from ydpaper where papername = '%s';", sPaper);
// 				CODBCSet set;
// 				db.PrepareSql(strSql, set);
// 				if(db.FetchData())
// 				{
// 					pGExamStruct->m_pExamDetail->nPaperid = atoi(set.m_coldata[0].value);
// 				}
// 				set.Empty();
// 				db.DisConnect();
// 			}
// 			pGExamStruct->m_pExamDetail->sPapername = sPaper;
// 			pGExamStruct->m_pExamDetail->sSubject = sSubject;
// 			pGExamStruct->m_pExamDetail->sStart = sStarttime;
// 			pGExamStruct->m_pExamDetail->sEnd = sEndtime;
// 			pGExamStruct->m_pExamDetail->nMaxNO = atoi(sMaxNO);
// 			pGExamStruct->m_pExamDetail->nStartNO = atoi(sStartNO);
// 			pGExamStruct->m_pExamDetail->nEndNO = atoi(sEndNO);
// 		}
// 	}
// 
// 	int index = m_address.GetCurSel();
// 	if(index != -1)
// 		m_address.GetLBText(index,pGExamStruct->m_pExamDetail->sAddress);
// 	else
// 		pGExamStruct->m_pExamDetail->sAddress = "";

// 	pGExamStruct->m_pExamDetail->arryClass.RemoveAll();
// 	for(int i = 0; i < m_classlist.GetItemCount(); i++)
// 	{
// 		if(m_classlist.GetCheck(i))
// 			pGExamStruct->m_pExamDetail->arryClass.Add(m_classlist.GetItemText(i,1));
// 	}

// 	if(!(((CButton *)GetDlgItem(IDC_RADIO_NORMAL))->GetCheck()) && !(((CButton *)GetDlgItem(IDC_RADIO_STANDARD))->GetCheck()))
// 	{
// 		AfxMessageBox(_T("没有选择本场考试性质，无法进入下一步！"));
// 		return;
// 	}
// 
// 	if(((CButton *)GetDlgItem(IDC_RADIO_NORMAL))->GetCheck())
// 	{
// 		pGExamStruct->m_pExamDetail->m_uExamKind = YDEXAM_KIND_NORMAL;
// 	}
// 	if(((CButton *)GetDlgItem(IDC_RADIO_STANDARD))->GetCheck())
// 	{
// 		pGExamStruct->m_pExamDetail->m_uExamKind = YDEXAM_KIND_TEST;
// 	}

// 	CMainFrame *fram = (CMainFrame *)AfxGetApp()->m_pMainWnd;
// 	fram->m_dlgMain.m_examtime.SetKind(m_kind);
// 	fram->m_dlgMain.m_examtime.SetInfo();
// 	fram->m_dlgMain.ShowDlg(IDD_FORM_EXAM_TIME);		
}

void CFormExamSet::SetKind(int kind)
{
	m_kind = kind;
}

void CFormExamSet::OnDblclkPaperlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int item =pNMListView->iItem;
	m_paperlist.SelectAll();
	m_paperlist.DeSelectAll();
	m_paperlist.SetCheck(item);
	
	*pResult = 0;
}

void CFormExamSet::OnDblclkClasslist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int item =pNMListView->iItem;
	m_classlist.SetCheck(item);
	
	*pResult = 0;
}

void CFormExamSet::OnSet() 
{
	CDlgTeacherSet  dlg;
	dlg.DoModal();
	 
}

BOOL CFormExamSet::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
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