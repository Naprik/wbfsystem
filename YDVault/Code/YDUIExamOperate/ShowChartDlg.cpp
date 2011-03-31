// ShowChartDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ShowChartDlg.h"
#include "../Base/AutoClean.h"
#include "FormQueryMark.h"
#include "../ObjRef/YDPaper.h"

// CShowChartDlg dialog

IMPLEMENT_DYNAMIC(CShowChartDlg, CDialog)

CShowChartDlg::CShowChartDlg(BOOL bSearch, CWnd* pParent /*=NULL*/)
	: CDialog(CShowChartDlg::IDD, pParent)
{
	m_bFlag = bSearch;
}

CShowChartDlg::~CShowChartDlg()
{
}

void CShowChartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSCHART_SHOW, m_chart);
}


BEGIN_MESSAGE_MAP(CShowChartDlg, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CShowChartDlg message handlers

BOOL CShowChartDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_chart.put_ColumnCount(1);
	//	m_chart.put_TitleText(_T("错题分布图"));
	m_chart.put_ShowLegend(FALSE);
	m_chart.put_RowCount(0);
	Draw(m_bFlag);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

HRESULT CShowChartDlg::Draw(BOOL bSearch)
{
	if (bSearch)
	{
		HRESULT hr = E_FAIL;
		m_chart.put_ColumnLabel(_T("人数/次数"));
		int i90 = 0;
		int i80 = 0;
		int i70 = 0;
		int i60 = 0;
		int i50 = 0;
		if(m_pLstQueryMarkInfo->size() <= 0)
		{
			return S_OK;
		}
		//判断是否有多份试卷,如果有多份成绩，按100分计算，分段
		//否则按试卷的分数实际分段
		BOOL bHasMulityPaper = FALSE;
		{
			BOOL bFirst = TRUE;
			long lPrevPaperID = 0;
			for(std::list<CQueryMarkInfo*>::const_iterator itr = m_pLstQueryMarkInfo->begin();
				itr != m_pLstQueryMarkInfo->end(); ++itr)
			{
				if(bFirst)
				{
					lPrevPaperID = (*itr)->m_lPaperID;
					bFirst = FALSE;
				}
				else
				{
					if(lPrevPaperID != (*itr)->m_lPaperID)
					{
						bHasMulityPaper = TRUE;
						break;
					}
					else
					{
						lPrevPaperID = (*itr)->m_lPaperID;
					}
				}
			}
		}
		int iSpace90 = 90;
		int iSpace80 = 80;
		int iSpace70 = 70;
		int iSpace60 = 60;
		int iSpace50 = 0;
		if(!bHasMulityPaper)
		{
			//只有一张试卷时，统计师按照试卷的分值智能分为5段
			//试卷分值*0.6一下为不及格
			//剩下的分四段
			long lPaperMarkVale = 100;//试卷分值
			CQueryMarkInfo* pMarkInfo = (CQueryMarkInfo*)(*(m_pLstQueryMarkInfo->begin()));
			CYDPaper* pPaper = NULL;
			hr = pMarkInfo->GetPaper(pPaper);
			if(FAILED(hr))
			{
				return hr;
			}
			CPtrAutoClean<CYDPaper> clr(pPaper);
			hr = pPaper->GetPaperMarkVal(lPaperMarkVale);
			if(FAILED(hr))
			{
				return hr;
			}
			iSpace90 = (int)(lPaperMarkVale * 0.9);
			iSpace80 = (int)(lPaperMarkVale * 0.8);
			iSpace70 = (int)(lPaperMarkVale * 0.7);
			iSpace60 = (int)(lPaperMarkVale * 0.6);
		}


		for(std::list<CQueryMarkInfo*>::const_iterator itr = m_pLstQueryMarkInfo->begin();
			itr != m_pLstQueryMarkInfo->end(); ++itr)
		{
			float fMark = (*itr)->m_fMark;
			int iMark = (int)fMark;
			if (iMark >= iSpace90)
			{
				i90++;
			}
			else if (iMark >= iSpace80)
			{
				i80++;
			}
			else if (iMark >= iSpace70)
			{
				i70++;
			}
			else if (iMark >= iSpace60)
			{
				i60++;
			}
			else
			{
				i50++;
			}

		}

		//向chart中添加数据
		m_chart.put_ShowLegend(TRUE);
		int iXcount = 5;				
		m_chart.put_RowCount(iXcount);       //设置X轴的步数
		for (int i = 1; i <= iXcount; i++)    //为X轴设置值
		{

			m_chart.put_Row(i);
			CString str;
			int iCount = 0;
			if (i == 1)
			{
				str.Format(_T("%d以上"),iSpace90);
				iCount = i90;
			}
			else if (2 == i)
			{
				str.Format(_T("%d-%d"),iSpace80,iSpace90-1);
				//str = L"80-89";
				iCount = i80;
			}
			else if (3 == i)
			{
				str.Format(_T("%d-%d"),iSpace70,iSpace80-1);
				//str = L"70-79";
				iCount = i70;
			}
			else if (4 == i)
			{
				str.Format(_T("%d-%d"),iSpace60,iSpace70-1);
				//str = L"60-69";
				iCount = i60;
			}
			else
			{
				str.Format(_T("%d以下"),iSpace60);
				//str = L"60以下";
				iCount = i50;
			}
			CString strErrorCount;
			strErrorCount.Format(_T("%d"), iCount);
			str += _T("(");
			str += strErrorCount;
			str += _T(")");
			m_chart.put_RowLabel(str);		//设置X每步的标题  这里就是是题号
			m_chart.put_Data(strErrorCount);			//设置X的值
		}
	}
	else
	{
		HRESULT hr = E_FAIL;
		//向chart中添加数据
		m_chart.put_ColumnLabel(_T("试卷名/分数"));
		//m_chart.put_TitleText()
		m_chart.put_ShowLegend(TRUE);
		int iXcount = m_pLstQueryMarkInfo->size();				
		m_chart.put_RowCount(iXcount);
		int iRow = 1;
		for (std::list<CQueryMarkInfo*>::const_iterator itr = m_pLstQueryMarkInfo->begin();
			itr != m_pLstQueryMarkInfo->end(); ++itr,iRow++)
		{
			CString strPaperName;
			hr = (*itr)->GetPaperName(strPaperName);
			if(FAILED(hr))
			{
				return hr;
			}
			CString strMark;
			strMark.Format(_T("%f"),(*itr)->m_fMark);

			m_chart.put_Row(iRow);
			m_chart.put_RowLabel(strPaperName);		//设置X每步的标题  这里就是试卷名
			m_chart.put_Data(strMark);			//设置X的值,就是成绩

		}
	}

	return S_OK;
}
void CShowChartDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	CRect rect;   //获取控件变化前的大小  
	GetWindowRect(&rect);

	rect.left += 5;
	rect.right -= 5;
	rect.top += 5;
	rect.bottom -= 10;
	if(m_chart.GetSafeHwnd() != NULL)
	{
		m_chart.MoveWindow(rect);//设置控件大小
	}
// TODO: Add your message handler code here
}
