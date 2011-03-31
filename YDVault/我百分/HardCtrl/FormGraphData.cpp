// FormGraphData.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "FormGraphData.h"

CFormGraphData::CFormGraphData(CWnd* pParent /*=NULL*/)
	: CDialog(CFormGraphData::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormGraphData)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFormGraphData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormGraphData)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormGraphData, CDialog)
	//{{AFX_MSG_MAP(CFormGraphData)
	ON_WM_PAINT()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormGraphData message handlers

BOOL CFormGraphData::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_data = NULL;
	return TRUE; 
}

void CFormGraphData::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if ( m_data )
	{
		m_graph->DrawGraph(GetDC());
	    ReleaseDC(GetDC());	
	}
}

void CFormGraphData::OnSize(UINT nType, int cx, int cy) 
{	
	CDialog::OnSize(nType, cx, cy);
}

void CFormGraphData::SetData()
{
	for ( int i = 0 ; i < m_data->m_xList.GetSize() ; i ++ )
	{
		CGraphSeries* series = new CGraphSeries();
		series->SetLabel(m_data->m_xList.GetAt(i));
		for( int j = 0 ; j < m_data->m_yList[i].GetSize();j++)
			series->SetData(j,m_data->m_yList[i].GetAt(j));
		m_graph->AddSeries(series);
	}
}
void CFormGraphData::SetMem()
{
	if ( m_data->m_legList.GetSize() <= 0 )
		return;
	for (int i = 0 ; i < m_data->m_legList.GetSize() ; i ++ )
		m_graph->SetLegend(i, m_data->m_legList.GetAt(i));	
}
BOOL CFormGraphData::IsPie()
{
	if((m_data->m_type==PIE_GRAPH)||(m_data->m_type==PIE_GRAPH_3D))
		return TRUE;
	return FALSE;
}
void CFormGraphData::SetText()
{
	m_graph->SetGraphTitle(m_data->m_textTitle);
	m_graph->SetXAxisLabel(m_data->m_textX);
	m_graph->SetYAxisLabel(m_data->m_textY);
}

CGraph* CFormGraphData::InitGraph()
{
    CGraph* graph = new CGraph(m_data->m_type);	
	graph->SetTickLimits(m_data->m_pos[0], m_data->m_pos[1], m_data->m_pos[2]);
	graph->SetXAxisAlignment(HORIZONTAL_ALIGN);
	graph->SetGridLines(m_data->m_isLine);

	graph->SetLegendFontSize(16);
    graph->SetXTickFontSize(14);
	graph->SetYTickFontSize(14);
	graph->SetColor(0, RED);
	graph->SetColor(1, GREEN);
	graph->SetColor(2, BLUE);
	graph->SetColor(3, HOT_PINK);
	graph->SetColor(4, LAVENDER);
	graph->SetColor(5, DUSK);
	graph->SetColor(6, BROWN);
	graph->SetColor(7, MAROON);
	graph->SetColor(8, GREY);
	graph->SetColor(9, TAN);
	for ( int i = 10 ; i < 100 ; i ++ )
		graph->SetColor(i, RGB(255-i*10,i*10+100,i*10+100));
	return graph;
}
void CFormGraphData::DrawGraph(CFileGraph *data)
{
	if ( data == NULL )
		return;
	if ( data->m_xList.GetSize() <0 )
		return;
	m_data = data;
	m_graph =InitGraph();
    SetText();
	SetMem();
    SetData();
	CDC* pDC = GetDC();
	m_graph->DrawGraph(pDC);
	ReleaseDC(pDC);	
}
