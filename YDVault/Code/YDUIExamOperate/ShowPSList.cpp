// ShowPSList.cpp : implementation file
//

#include "stdafx.h"
#include "ShowPSList.h"
const int cMarkListSerialNo = 0;//题号
const int cMarkListRightNumber = 1;//正确人数
const int cMarkListErrorNumber = 2;// L"错误人数", LVCFMT_LEFT,iWidth*2/8);
const int cMarkListRightRate = 3;//, L"正确率", LVCFMT_LEFT,iWidth*2/8 -2 );
const int cMarkListRightAnswer = 4;//, L"正确答案", LVCFMT_LEFT,iWidth*2/8 -2 );
const int cMarkListAnswerA = 5;//答A的人数
const int cMarkListAnswerB = 6;//答B的人数
const int cMarkListAnswerC = 7;//答C的人数
const int cMarkListAnswerD = 8;//答D的人数
const int cMarkListNotFillAnswer = 9;//未填写

// CShowPSList dialog

IMPLEMENT_DYNAMIC(CShowPSList, CDialog)

CShowPSList::CShowPSList(CWnd* pParent /*=NULL*/)
	: CDialog(CShowPSList::IDD, pParent)
{

}

CShowPSList::~CShowPSList()
{
}

void CShowPSList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_OP_MARK, m_MarkListCtrl);
}


BEGIN_MESSAGE_MAP(CShowPSList, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CShowPSList message handlers

void CShowPSList::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if(m_MarkListCtrl.GetSafeHwnd() != NULL)
	{

		CRect rect;   //获取控件变化前的大小  
		GetWindowRect(&rect);

		rect.left += 5;
		rect.right -= 5;
		rect.top += 5;
		rect.bottom -= 10;
		m_MarkListCtrl.MoveWindow(rect);//设置控件大小
	}

}

BOOL CShowPSList::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_MarkListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	CRect rect;
	m_MarkListCtrl.GetWindowRect(rect);

	ShowWindow(SW_MAXIMIZE);

	long iWidth = rect.Width();
	m_MarkListCtrl.InsertColumn(cMarkListSerialNo, L"题号", LVCFMT_LEFT, 60);
	m_MarkListCtrl.InsertColumn(cMarkListRightNumber, L"正确人数", LVCFMT_LEFT,80);
	m_MarkListCtrl.InsertColumn(cMarkListErrorNumber, L"错误人数", LVCFMT_LEFT,80);
	m_MarkListCtrl.InsertColumn(cMarkListRightRate, L"正确率", LVCFMT_LEFT,80 );
	m_MarkListCtrl.InsertColumn(cMarkListRightAnswer, L"正确答案", LVCFMT_LEFT,80 );
	m_MarkListCtrl.InsertColumn(cMarkListAnswerA, L"A", LVCFMT_LEFT,100 );
	m_MarkListCtrl.InsertColumn(cMarkListAnswerB, L"B", LVCFMT_LEFT,100 );
	m_MarkListCtrl.InsertColumn(cMarkListAnswerC, L"C", LVCFMT_LEFT,100);
	m_MarkListCtrl.InsertColumn(cMarkListAnswerD, L"D", LVCFMT_LEFT,100 );
	m_MarkListCtrl.InsertColumn(cMarkListNotFillAnswer, YD_NOT_FILL_ANSWER, LVCFMT_LEFT,100 );

	m_MarkListCtrl.DeleteAllItems();
	std::map<UINT, CPaperAnsysHelperResult*>::const_iterator itrMap = m_pMapRes->begin();
	for (; itrMap != m_pMapRes->end(); ++itrMap)
	{
		CString strIndex;
		strIndex.Format(L"%d", itrMap->first);
		int index = m_MarkListCtrl.InsertItem(m_MarkListCtrl.GetItemCount(), strIndex);
		float fRight = 0.0000;
		if (m_Total != 0)
		{
			fRight = (itrMap->second)->m_iRight/float(m_Total);
		}

		CString strRight;
		strRight.Format(L"%d", (itrMap->second)->m_iRight);
		CString strError;
		strError.Format(L"%d", (itrMap->second)->m_iError);
		CString strRoate;
		float fRes = fRight * 100;
		strRoate.Format(L"%0.2f%c", fRes, 0x25);
		m_MarkListCtrl.SetItemText(index,cMarkListRightNumber, strRight);
		m_MarkListCtrl.SetItemText(index,cMarkListErrorNumber, strError);
		m_MarkListCtrl.SetItemText(index,cMarkListRightRate, strRoate);
		m_MarkListCtrl.SetItemText(index,cMarkListRightAnswer, (itrMap->second)->m_strRightAnswer);
		m_MarkListCtrl.SetItemData(index, (DWORD_PTR)(itrMap->second));
		UINT uTotalAnswerCount = 0;//答案的总数，要计算百分比
		for(std::map<CString, UINT>::const_iterator itrMapAnswer = (itrMap->second)->m_mapAnswer.begin();
			itrMapAnswer != (itrMap->second)->m_mapAnswer.end();++itrMapAnswer)
		{
			UINT uAnswerCount = itrMapAnswer->second;
			uTotalAnswerCount += uAnswerCount;
		}
		ASSERT(uTotalAnswerCount > 0);
		//各个答案的个数
		for(std::map<CString, UINT>::const_iterator itrMapAnswer = (itrMap->second)->m_mapAnswer.begin();
			itrMapAnswer != (itrMap->second)->m_mapAnswer.end();++itrMapAnswer)
		{
			CString strAnswer = itrMapAnswer->first;
			UINT uAnswerCount = itrMapAnswer->second;
			float fPercent = (float)((uAnswerCount /(uTotalAnswerCount * 1.0)) * 100);
			CString strAnswerCount;
			strAnswerCount.Format(_T("%d(%0.2f%%)"),uAnswerCount,fPercent);
			if(strAnswer.CompareNoCase(_T("A")) == 0)
			{
				m_MarkListCtrl.SetItemText(index,cMarkListAnswerA, strAnswerCount);
			}
			else if(strAnswer.CompareNoCase(_T("B")) == 0)
			{
				m_MarkListCtrl.SetItemText(index,cMarkListAnswerB, strAnswerCount);
			}
			else if(strAnswer.CompareNoCase(_T("C")) == 0)
			{
				m_MarkListCtrl.SetItemText(index,cMarkListAnswerC, strAnswerCount);
			}
			else if(strAnswer.CompareNoCase(_T("D")) == 0)
			{
				m_MarkListCtrl.SetItemText(index,cMarkListAnswerD, strAnswerCount);
			}
			else if(strAnswer.CompareNoCase(YD_NOT_FILL_ANSWER) == 0)
			{
				m_MarkListCtrl.SetItemText(index,cMarkListNotFillAnswer, strAnswerCount);
			}
			else
			{
				ASSERT(FALSE);
			}
		}
	}
	//修改颜色，每一行颜色都不相同，并且正确答案那一列要求为红色
	for(int iRow = 0; iRow < m_MarkListCtrl.GetItemCount();iRow++)
	{
		if(iRow % 2 == 0)
		{
			for(int iCol = 0; iCol < m_MarkListCtrl.GetHeaderCtrl().GetItemCount();iCol++)
			{
				if(iCol == cMarkListRightAnswer)
				{
					m_MarkListCtrl.SetItemColor(iRow,iCol,RGB(255,0,0),ITEM_TEXT_BK_COLOR,FALSE);
				}
				else
				{
					m_MarkListCtrl.SetItemColor(iRow,iCol,ITEM_TEXT_COLOR,ITEM_TEXT_BK_COLOR,FALSE);
				}
			}
		}
		else
		{
			for(int iCol = 0; iCol < m_MarkListCtrl.GetHeaderCtrl().GetItemCount();iCol++)
			{
				if(iCol == cMarkListRightAnswer)
				{
					m_MarkListCtrl.SetItemColor(iRow,iCol,RGB(255,0,0),ITEM_TEXT_BK_COLOR_DEFAULT,FALSE);
				}
				else
				{
					m_MarkListCtrl.SetItemColor(iRow,iCol,ITEM_TEXT_COLOR_DEFAULT,ITEM_TEXT_BK_COLOR_DEFAULT,FALSE);
				}
			}
		}
	}
	m_MarkListCtrl.Invalidate(TRUE);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
