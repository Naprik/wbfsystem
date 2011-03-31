// FormResult.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "FormResult.h"
#include "MainFrm.h"
#include "DBStdanswer.h"
#include "DBResult.h"
#include "DlgQuerystudentresult.h"
#include "DlgResDetail.h"
#include "DlgGraph.h"
#include "APPGraph.h"
#include "PreParent.h"		//打印预览父窗口
#include "ExportToExcel.h"
#include "BackUp.h"
#pragma   warning(disable:   4244)   

extern CHardCtrlApp theApp;

IMPLEMENT_DYNCREATE(CFormResult, CDialog)


/////////////////////////////////////////////////////////////////////////////
// CFormResult dialog


CFormResult::CFormResult(CWnd* pParent /*=NULL*/)
	: cdxCSizingDialog(CFormResult::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormResult)
	//}}AFX_DATA_INIT
}


void CFormResult::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormResult)
	DDX_Control(pDX, IDC_EXPORT, m_export);
	DDX_Control(pDX, IDC_PRINT, m_print);
	DDX_Control(pDX, IDC_SATA, m_stat);
	DDX_Control(pDX, IDC_TYPE, m_type);
	DDX_Control(pDX, IDC_QUERY, m_query);
	DDX_Control(pDX, IDC_RESULTLIST, m_resultlist);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormResult, cdxCSizingDialog)
	//{{AFX_MSG_MAP(CFormResult)
	ON_BN_CLICKED(IDC_QUERY, OnQuery)
	ON_NOTIFY(NM_DBLCLK, IDC_RESULTLIST, OnDblclkResultlist)
	ON_BN_CLICKED(IDC_SATA, OnSata)
	ON_BN_CLICKED(IDC_PRINT, OnPrint)
	ON_BN_CLICKED(IDC_EXPORT, OnExport)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormResult message handlers

BOOL CFormResult::OnInitDialog()
{
	cdxCSizingDialog::OnInitDialog();

	m_export.SetShade(CShadeButtonST::SHS_DIAGSHADE,0,10,0,RGB(0,0,0));
	m_export.SetIcon(IDI_EXCEL);
	m_export.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(0, 178, 255));
	m_export.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 255));

	m_print.SetShade(CShadeButtonST::SHS_DIAGSHADE,0,10,0,RGB(0,0,0));
	m_print.SetIcon(IDI_PRINT);
	m_print.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(0, 178, 255));
	m_print.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 255));

	m_stat.SetShade(CShadeButtonST::SHS_DIAGSHADE,0,10,0,RGB(0,0,0));
	m_stat.SetIcon(IDI_CHART);
	m_stat.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(0, 178, 255));
	m_stat.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 255));

	m_query.SetShade(CShadeButtonST::SHS_DIAGSHADE,0,10,0,RGB(0,0,0));
	m_query.SetIcon(IDI_SEARCHPAPER);
	m_query.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(0, 178, 255));
	m_query.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 255));

	m_Static1.SubclassDlgItem(IDC_STATIC1,this);
	m_Static1.SetCaption("统计图类别");
	m_Static1.SetBackColor(RGB(0,0,0));

	AddSzControl(*GetDlgItem(IDC_QUERY),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_TYPE),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_SATA),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_STATIC1),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_EXPORT),mdNone,mdRepos);

	
	
	AddSzControl(*GetDlgItem(IDC_PRINT),mdNone,mdRepos);
	AddSzControl(m_resultlist,mdResize,mdResize);	

	DWORD dwStyle=GetWindowLong(m_resultlist.GetSafeHwnd(),GWL_STYLE);
	dwStyle&=~LVS_TYPEMASK;
	dwStyle|=LVS_REPORT;
	SetWindowLong(m_resultlist.GetSafeHwnd(),GWL_STYLE,dwStyle);  

	m_resultlist.SetExtendedStyle(LVS_EX_GRIDLINES);
    ::SendMessage(m_resultlist.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
     LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);


    ::SetWindowPos(m_hWnd,HWND_TOPMOST,0, 0, 0, 0,SWP_NOSIZE | SWP_NOMOVE  );

 	m_resultlist.ClearHeadings();
	m_resultlist.SetHeadings("试卷编号,0;学号,100; 姓名,100; 科目,100;试卷名称,150;开考时间,150;正确题数,100;错误题数,100;分数,100");   

	return TRUE;
}

/*************************

*************************/
void CFormResult::OnQuery() 
{
	CBackUp bk;
	bk.Backup("哈哈","asdasd");
	bk.Remove();
	
	CDlgQuerystudentresult dlg;
	if(dlg.DoModal() == IDOK)
	{
		m_resultlist.DeleteAllItems();
		CString strStuID , strStuname;
		CString strSubject,strPaper;
		COleDateTime tTime;
		tTime.SetDate(dlg.m_datetime.GetYear(),dlg.m_datetime.GetMonth(),dlg.m_datetime.GetDay());
		BOOL bMark = FALSE;
// 		CoInitialize(NULL);
// 		CDBStuInfo  m_StuInfodb;
// 		if ( !m_StuInfodb.Connect())
// 		{
// 			AfxMessageBox("StuInfodb:无法连接到数据库服务器");
// 		}
// 		else
// 		{
// 			BOOL flag = m_StuInfodb.LoadByStu(dlg.strID, dlg.strName);
// 			if(flag)
// 			{
// 				strStuID = m_StuInfodb.m_stuid;
// 				strStuname = m_StuInfodb.m_stuname;
// 				bMark = TRUE;
// 			}
// 			else
// 			{
// 				AfxMessageBox("没有该学生信息!");
// 			}
// 			m_StuInfodb.Close();
// 		}
// 		CoUninitialize();
		
		if(!bMark)
			return;

// 		CoInitialize(NULL);
// 		CDBPaper m_Paperdb;
// 		CDBResult m_Resultdb;
// 		if(!m_Paperdb.Connect() )
// 		{
// 			AfxMessageBox("Paperdb:无法连接到数据库服务器");
// 		}
// 		else
// 		{
// 			if(!m_Resultdb.Connect())
// 			{
// 				AfxMessageBox("Resultdb:无法连接到数据库服务器");
// 			}
// 			else
// 			{
// 				if(dlg.strSubject == "" && dlg.bIstime == FALSE)
// 				{
// 					BOOL flag = m_Resultdb.LoadByQuery(strStuID,0);
// 					while(flag)
// 					{
// 						int paperid = m_Resultdb.m_paperid;
// 						CString stuanswer = m_Resultdb.m_stuanswer;
// 						CString temp;
// 						temp.Format("%d",paperid);
// 						CString strat = m_Resultdb.m_starttime.Format();
// 						BOOL flag1 = m_Paperdb.LoadByPaperID(paperid);
// 						if(flag1)
// 						{
// 							strSubject = m_Paperdb.m_subject;
// 							strPaper = m_Paperdb.m_papername;
// 							int startNO = m_Paperdb.m_startNO;
// 							int endNO = m_Paperdb.m_endNO;
// 							int right = 0;
// 							int worng = 0;
// 							double sorct = 0;
// 							if(!calculate(stuanswer, startNO,endNO,paperid,right,worng,sorct))
// 								m_resultlist.AddItem(0,temp,strStuID,strStuname,strSubject,strPaper,strat,"","","");
// 							else
// 							{
// 								CString sRight,sWorng,sSocrt;
// 								sRight.Format("%d",right);
// 								sWorng.Format("%d",worng);
// 								sSocrt.Format("%0.2f",sorct);
// 								m_resultlist.AddItem(0,temp,strStuID,strStuname,strSubject,strPaper,strat,sRight,sWorng,sSocrt);
// 							}
// 						}
// 						flag = m_Resultdb.Move();
// 					}
// 				}
// 				else
// 				{
// 					BOOL flag = m_Paperdb.LoadByQuery(dlg.strPaper,dlg.strSubject,"","",tTime,dlg.bIstime);
// 					while(flag)
// 					{
// 						int paperid = m_Paperdb.m_ydid;
// 						int startNO = m_Paperdb.m_startNO;
// 						int endNO = m_Paperdb.m_endNO;
// 						CString temp;
// 						temp.Format("%d",paperid);
// 						strSubject = m_Paperdb.m_subject;
// 						strPaper = m_Paperdb.m_papername;
// 						BOOL flag1 = m_Resultdb.LoadByQuery(strStuID,paperid);
// 						if(flag1)
// 						{
// 							CString strat = m_Resultdb.m_starttime.Format();
// 							CString stuanswer = m_Resultdb.m_stuanswer;
// 							int right = 0;
// 							int worng = 0;
// 							double sorct = 0;
// 							if(!calculate(stuanswer, startNO,endNO,paperid,right,worng,sorct))
// 								m_resultlist.AddItem(0,temp,strStuID,strStuname,strSubject,strPaper,strat,"","","");
// 							else
// 							{
// 								CString sRight,sWorng,sSocrt;
// 								sRight.Format("%d",right);
// 								sWorng.Format("%d",worng);
// 								sSocrt.Format("%0.2f",sorct);
// 								m_resultlist.AddItem(0,temp,strStuID,strStuname,strSubject,strPaper,strat,sRight,sWorng,sSocrt);
// 							}
// 						}
// 						flag = m_Paperdb.Move();
// 					}
// 				}
// 				m_Resultdb.Close();
// 			}
// 			m_Paperdb.Close();
// 		}
// 		CoUninitialize();
	}
}

BOOL CFormResult::calculate(CString strStuanswer, int start, int end, int paperid, int &right, int &worng, double &score)
{
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
void CFormResult::OnDblclkResultlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int item =pNMListView->iItem;	
	CString strPaperid = m_resultlist.GetItemText(item,0);
	if(strPaperid != "")
	{
		CDlgResDetail  dlg;
		dlg.iPaperID = atoi(strPaperid);
		dlg.strStuid = m_resultlist.GetItemText(item,1);
		dlg.strStuname = m_resultlist.GetItemText(item,2);
		dlg.strSubject = m_resultlist.GetItemText(item,3);
		dlg.strPaper = m_resultlist.GetItemText(item,4);
		dlg.strRight = m_resultlist.GetItemText(item,6);
		dlg.strWorng = m_resultlist.GetItemText(item,7);
		dlg.strSocrt = m_resultlist.GetItemText(item,8);
		dlg.DoModal();
	}
	

	*pResult = 0;
}

void CFormResult::OnSata() 
{
	int iType = m_type.GetCurSel();
	if(iType < 0 )
	{
		AfxMessageBox("请选择统计图的种类！");
		return;
	}

	if(m_resultlist.GetItemCount() <= 0 )
	{
		AfxMessageBox("没有查询到结果，无法显示统计图！");
		return;
	}
	CStringArray  arryPaper, arrySocre;
	arryPaper.RemoveAll();
	arrySocre.RemoveAll();
	CString strStuname;
	strStuname = m_resultlist.GetItemText(0,2);
	for(int i = 0; i < m_resultlist.GetItemCount(); i++)
	{
		CString strPaper = m_resultlist.GetItemText(i,4);
		CString strSocre = m_resultlist.GetItemText(i,8);
		arryPaper.Add(strPaper);
		arrySocre.Add(strSocre);
	}
	CDlgGraph dlg;
	CAPPGraph  gra;
	if(iType == 0)
		gra.Boxscore(strStuname,arryPaper,arrySocre,dlg.m_graphData);
	dlg.DoModal();	
}

void CFormResult::OnPrint() 
{
	if(m_resultlist.GetItemCount()<= 0)
		return;
	PRNINFO PrnInfo = {0};
	PrnInfo.hListView = m_resultlist.m_hWnd;
	PrnInfo.hWnd = this->m_hWnd;
	PrnInfo.IsPrint = FALSE;
	PrnInfo.nCurPage = 1;
	PrnInfo.nMaxLine = m_resultlist.GetItemCount();
	
	CPreParent DlgPreView;
	DlgPreView.SetCallBackFun(DrawInfo, PrnInfo);
	DlgPreView.DoModal();	
	
}
void CFormResult::DrawInfo(CDC &memDC, PRNINFO PrnInfo)
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
	TCHAR name[32]={0};
	ListView_GetItemText(hList, 0, 2, name, 32);
	CString temp = name;
	
	TCHAR szTitle[32] = {0};
	strcpy(szTitle,name);

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
						ListView_GetItemText(hList, i+iStart, 6, Right, 32);
						ListView_GetItemText(hList, i+iStart, 7, Worng, 32);
						ListView_GetItemText(hList, i+iStart, 8, Res, 32);
						
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
			ListView_GetItemText(hList, i+iStart, 6, Right, 32);
			ListView_GetItemText(hList, i+iStart, 7, Worng, 32);
			ListView_GetItemText(hList, i+iStart, 8, Res, 32);
			
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

void CFormResult::OnExport() 
{
	CExportToExcel  ete;
	CString strName = m_resultlist.GetItemText(0,2);
	strName.Replace("-","");
	strName += "的成绩";
	ete.ExportListToExcel(&m_resultlist,strName);	
}
CString CFormResult::GetExePath()
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
void CFormResult::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rc;
    CJahCoverCapture capture;
	GetClientRect(&rc);
	capture.m_bmpFile =GetExePath()+"formback.bmp" ;
    if(!capture.LoadBMPImage(capture.m_bmpFile))
   		return;
    if (capture.m_bitmap.m_hObject == 0 )
    	return;
   capture.DrawBitmap(&dc,rc);
}
