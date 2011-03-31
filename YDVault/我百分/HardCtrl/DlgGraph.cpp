// DlgGraph.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "DlgGraph.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgGraph dialog


CDlgGraph::CDlgGraph(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGraph::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgGraph)
	m_isLine = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgGraph::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgGraph)
	DDX_Control(pDX, IDC_TYPE, m_type);
	DDX_Check(pDX, IDC_LINE, m_isLine);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgGraph, CDialog)
	//{{AFX_MSG_MAP(CDlgGraph)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_LINE, OnLine)
	ON_CBN_SELCHANGE(IDC_TYPE, OnSelchangeType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgGraph message handlers

int CDlgGraph::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rc;
	GetClientRect(rc);
	m_graph.Create(IDD_FORM_GRAPH,this);
	m_graph.ShowWindow(true);
	return 0;
}

void CDlgGraph::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);	
	if ( ::IsWindow(m_graph.m_hWnd) )
	{	
	//	if ( GetDlgItem(IDC_REG) != NULL )
		{
			CRect lpRect;	
		    GetClientRect(&lpRect);	
		    lpRect.top = 30;
		    m_graph.MoveWindow(lpRect);
		}		
	}
}

BOOL CDlgGraph::OnInitDialog() 
{
	CDialog::OnInitDialog();
/*	m_graphData.m_textTitle = "成绩比对";
	m_graphData.m_textX = "分数段";
	m_graphData.m_textY = "人数";

	CString testx1 = "90-100";
	CString testx2 = "80-89";
	CString testx3 = "70-79";
	CString testx4 = "60-68";
	CString testx5 = "60以下";
	m_graphData.m_xList.Add(testx1);
	m_graphData.m_xList.Add(testx2);
	m_graphData.m_xList.Add(testx3);
	m_graphData.m_xList.Add(testx4);
	m_graphData.m_xList.Add(testx5);

	DWORD	testy1 = 10;
	DWORD	testy2 = 20;
	DWORD	testy3 = 30;
	DWORD	testy4 = 20;
	DWORD	testy5 = 5;
	m_graphData.m_yList[0].Add(testy1);
	m_graphData.m_yList[1].Add(testy2);
	m_graphData.m_yList[2].Add(testy3);
	m_graphData.m_yList[3].Add(testy4);
	m_graphData.m_yList[4].Add(testy5);

	m_graphData.m_pos[0] = 0;
	m_graphData.m_pos[1] = 50;
	m_graphData.m_pos[2] = 10;*/
    m_graphData.SetType(0);
	ShowGraph();		
	
	
	return TRUE;  
}

void CDlgGraph::ShowGraph()
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

void CDlgGraph::OnPaint() 
{
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

void CDlgGraph::OnLine() 
{
   UpdateData( TRUE );
  
   m_graphData.SetLine(m_isLine);
   UpdateData( FALSE );
   OnSelchangeType() ;		
}



void CDlgGraph::OnSelchangeType() 
{
	if ( m_type.GetCurSel() >= 0 )
  		m_graphData.SetType( m_type.GetCurSel());
	ShowGraph();
}
