// ShowPSList.cpp : implementation file
//

#include "stdafx.h"
#include "ShowPSList.h"
const int cMarkListSerialNo = 0;//���
const int cMarkListRightNumber = 1;//��ȷ����
const int cMarkListErrorNumber = 2;// L"��������", LVCFMT_LEFT,iWidth*2/8);
const int cMarkListRightRate = 3;//, L"��ȷ��", LVCFMT_LEFT,iWidth*2/8 -2 );
const int cMarkListRightAnswer = 4;//, L"��ȷ��", LVCFMT_LEFT,iWidth*2/8 -2 );
const int cMarkListAnswerA = 5;//��A������
const int cMarkListAnswerB = 6;//��B������
const int cMarkListAnswerC = 7;//��C������
const int cMarkListAnswerD = 8;//��D������
const int cMarkListNotFillAnswer = 9;//δ��д

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

		CRect rect;   //��ȡ�ؼ��仯ǰ�Ĵ�С  
		GetWindowRect(&rect);

		rect.left += 5;
		rect.right -= 5;
		rect.top += 5;
		rect.bottom -= 10;
		m_MarkListCtrl.MoveWindow(rect);//���ÿؼ���С
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
	m_MarkListCtrl.InsertColumn(cMarkListSerialNo, L"���", LVCFMT_LEFT, 60);
	m_MarkListCtrl.InsertColumn(cMarkListRightNumber, L"��ȷ����", LVCFMT_LEFT,80);
	m_MarkListCtrl.InsertColumn(cMarkListErrorNumber, L"��������", LVCFMT_LEFT,80);
	m_MarkListCtrl.InsertColumn(cMarkListRightRate, L"��ȷ��", LVCFMT_LEFT,80 );
	m_MarkListCtrl.InsertColumn(cMarkListRightAnswer, L"��ȷ��", LVCFMT_LEFT,80 );
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
		UINT uTotalAnswerCount = 0;//�𰸵�������Ҫ����ٷֱ�
		for(std::map<CString, UINT>::const_iterator itrMapAnswer = (itrMap->second)->m_mapAnswer.begin();
			itrMapAnswer != (itrMap->second)->m_mapAnswer.end();++itrMapAnswer)
		{
			UINT uAnswerCount = itrMapAnswer->second;
			uTotalAnswerCount += uAnswerCount;
		}
		ASSERT(uTotalAnswerCount > 0);
		//�����𰸵ĸ���
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
	//�޸���ɫ��ÿһ����ɫ������ͬ��������ȷ����һ��Ҫ��Ϊ��ɫ
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
