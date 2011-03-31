// ShowPSChart.cpp : implementation file
//

#include "stdafx.h"
#include "ShowPSChart.h"

// CShowPSChart dialog

IMPLEMENT_DYNAMIC(CShowPSChart, CDialog)

CShowPSChart::CShowPSChart(CWnd* pParent /*=NULL*/)
	: CDialog(CShowPSChart::IDD, pParent)
{

}

CShowPSChart::~CShowPSChart()
{
}

void CShowPSChart::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSCHART_SHOW_PS, m_chart);
}


BEGIN_MESSAGE_MAP(CShowPSChart, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CShowPSChart message handlers

BOOL CShowPSChart::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_chart.put_ColumnCount(1);
	m_chart.put_TitleText(_T("错题分布图"));
	m_chart.put_ShowLegend(FALSE);
	//	VARIANT var;
	m_chart.put_RowCount(0);
	m_chart.put_ShowLegend(TRUE);
	int iXcount = m_pMapRes->size();				
	m_chart.put_RowCount(iXcount);       //设置X轴的步数
	for (int i = 1; i <= iXcount; i++)    //为X轴设置值
	{
		std::map<UINT, CPaperAnsysHelperResult*>::const_iterator itrMap = 
			m_pMapRes->find(i);
		if (itrMap != m_pMapRes->end())
		{
			m_chart.put_Row(i);
			CString str;
			str.Format(_T("%d"), i);
			m_chart.put_RowLabel(str);		//设置X每步的标题  这里就是是题号
			CString strErrorCount;

			strErrorCount.Format(_T("%d"), (itrMap->second)->m_iError);
			m_chart.put_Data(strErrorCount);			//设置X的值
		}
		else
		{
			m_chart.put_Row(i);
			CString str;
			str.Format(_T("%d"), i);
			m_chart.put_RowLabel(str);		//设置X每步的标题  这里就是是题号
			CString strErrorCount = L"0";

			m_chart.put_Data(strErrorCount);			//设置X的值
		}
	}	


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CShowPSChart::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if(m_chart.GetSafeHwnd() != NULL)
	{
		CRect rect;   //获取控件变化前的大小  
		GetWindowRect(&rect);

		rect.left += 5;
		rect.right -= 5;
		rect.top += 5;
		rect.bottom -= 10;
		m_chart.MoveWindow(rect);//设置控件大小
	}


}
