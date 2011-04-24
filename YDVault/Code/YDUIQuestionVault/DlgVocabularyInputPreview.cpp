// DlgVocabularyInputPreview.cpp : implementation file
//

#include "stdafx.h"
#include "DlgVocabularyInputPreview.h"
#include "afxdialogex.h"


// CDlgVocabularyInputPreview dialog

const int cColSerialNo = 0;//���
const int cColCaption = 1;//��Ŀ
const int cColOptionName = 2;//ѡ����
const int cColOption = 3;//ѡ��
const int cColAnswer = 4;//��
const int cColFactorName = 5;//ָ����
const int cColFactor = 6;//ָ��ֵ

IMPLEMENT_DYNAMIC(CDlgVocabularyInputPreview, CDialogEx)

CDlgVocabularyInputPreview::CDlgVocabularyInputPreview(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgVocabularyInputPreview::IDD, pParent)
{
	m_plstVocabularyQuestion = NULL;
}

CDlgVocabularyInputPreview::~CDlgVocabularyInputPreview()
{
	m_plstVocabularyQuestion = NULL;
}

void CDlgVocabularyInputPreview::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgVocabularyInputPreview, CDialogEx)
END_MESSAGE_MAP()


// CDlgVocabularyInputPreview message handlers


BOOL CDlgVocabularyInputPreview::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
		CRect rectBK;
	GetDlgItem(IDC_STATIC_BK)->GetWindowRect(&rectBK);
	GetDlgItem(IDC_STATIC_BK)->ShowWindow(SW_HIDE);
	// TODO:  Add extra initialization here
	ScreenToClient(&rectBK);
	if (!m_Grid.Create(WS_CHILD | WS_TABSTOP | WS_VISIBLE, rectBK, this,
		-1))
	{
		return FALSE;
	}
	m_Grid.EnableDragHeaderItems(FALSE);
	//m_Grid.EnableColumnAutoSize (TRUE);
	m_Grid.EnableGroupByBox (FALSE);
	m_Grid.SetWholeRowSel(FALSE);
	m_Grid.EnableHeader (TRUE, 0);

	CBCGPGridColors colors;
	colors.m_LeftOffsetColors.m_clrBackground = globalData.clrWindow;
	m_Grid.SetColorTheme (colors);
	int iWidthGrid = rectBK.Width();
	m_Grid.InsertColumn(cColSerialNo, L"���", 50);
	m_Grid.InsertColumn(cColCaption, L"��Ŀ", 200);
	m_Grid.InsertColumn(cColOptionName, L"ѡ����", 50);
	m_Grid.InsertColumn(cColOption, L"ѡ��", 80);
	m_Grid.InsertColumn(cColAnswer, L"��", 80);
	m_Grid.InsertColumn(cColFactorName, L"ָ����", 50);
	m_Grid.InsertColumn(cColFactor, L"ָ��ֵ", 80);


	ASSERT(m_plstVocabularyQuestion);
	for(std::list<CVocabularyQuestion*>::const_iterator itr = m_plstVocabularyQuestion->begin();
		itr != m_plstVocabularyQuestion->end();++itr)
	{
		InsertRowByVocabularyQuestion(*itr);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


HRESULT CDlgVocabularyInputPreview::InsertRowByVocabularyQuestion(CVocabularyQuestion* _pVocabularyQuestion)
{
	HRESULT hr = E_FAIL;
	int index = 0;
	for(int i = 0; i < m_Grid.GetRowCount();i++)
	{
		CBCGPGridRow* pRow = m_Grid.GetRow(i);
		ASSERT(pRow);
		CBCGPGridRow* pParentRow = pRow->GetParent();
		if(pParentRow == NULL)
		{
			index++;
		}
	}
	CString strSerial;
	strSerial.Format(_T("%d"),index+1);
	CBCGPGridRow* pRow = m_Grid.CreateRow(m_Grid.GetColumnCount());
	pRow->GetItem(cColSerialNo)->SetValue(CComVariant(strSerial));

	//������Ŀ
	pRow->GetItem(cColCaption)->SetValue(CComVariant(_pVocabularyQuestion->m_strCaption));
	pRow->AllowSubItems();
	m_Grid.AddRow(pRow);

	//����ѡ��
	index = 0;
	for(std::list<std::pair<CString,CString> >::const_iterator itr = _pVocabularyQuestion->m_lstOption.begin();
		itr != _pVocabularyQuestion->m_lstOption.end();++itr)
	{
		CBCGPGridRow* pChildRow = NULL;
		if(index == 0)
		{
			pChildRow = pRow;
		}
		else
		{
			if(pRow->GetSubItemsCount() > index - 1)
			{
				pChildRow = pRow->GetSubItem(index-1);
			}
			else
			{
				pChildRow = m_Grid.CreateRow(m_Grid.GetColumnCount());
				pChildRow->GetItem(cColSerialNo)->Enable(FALSE);
				pChildRow->GetItem(cColCaption)->Enable(FALSE);
				pChildRow->GetItem(cColAnswer)->Enable(FALSE);


				pChildRow->AllowSubItems();
				pRow->AddSubItem(pChildRow);
			}
		
		}
		
		index++;
		ASSERT(pChildRow);
		pChildRow->GetItem(cColOptionName)->SetValue(CComVariant((*itr).first));
		pChildRow->GetItem(cColOption)->SetValue(CComVariant((*itr).second));
	}
	//�����
	pRow->GetItem(cColAnswer)->SetValue(CComVariant(_pVocabularyQuestion->m_strAnswer));
	//����ָ��
	index = 0;
	for(std::list<std::pair<CString,CString> >::const_iterator itr = _pVocabularyQuestion->m_lstFactor.begin();
		itr != _pVocabularyQuestion->m_lstFactor.end();++itr)
	{
			CBCGPGridRow* pChildRow = NULL;
		if(index == 0)
		{
			pChildRow = pRow;
		}
		else
		{
			if(pRow->GetSubItemsCount() > index - 1)
			{
				pChildRow = pRow->GetSubItem(index-1);
			}
			else
			{
				pChildRow = m_Grid.CreateRow(m_Grid.GetColumnCount());
				pChildRow->GetItem(cColSerialNo)->Enable(FALSE);
				pChildRow->GetItem(cColCaption)->Enable(FALSE);
				pChildRow->GetItem(cColAnswer)->Enable(FALSE);


				pChildRow->AllowSubItems();
				pRow->AddSubItem(pChildRow);
			}
		
		}
		
		index++;
		ASSERT(pChildRow);
		pChildRow->GetItem(cColFactorName)->SetValue(CComVariant((*itr).first));
		pChildRow->GetItem(cColFactor)->SetValue(CComVariant((*itr).second));
	}
	return hr;
}