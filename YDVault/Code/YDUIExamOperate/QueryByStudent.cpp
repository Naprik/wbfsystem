// QueryByStudent.cpp : implementation file
//

#include "stdafx.h"
#include "QueryByStudent.h"
#include "../Base/AutoClean.h"


// CQueryByStudent dialog

IMPLEMENT_DYNAMIC(CQueryByStudent, CDialog)

CQueryByStudent::CQueryByStudent(CWnd* pParent /*=NULL*/)
	: CDialog(CQueryByStudent::IDD, pParent)
{

}

CQueryByStudent::~CQueryByStudent()
{
	CListAutoClean<CQueryProp> clr(m_lstQueryProp);
}

void CQueryByStudent::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CQueryByStudent, CDialog)
	ON_BN_CLICKED(IDC_BTN_DM_STU_ADDQC, &CQueryByStudent::OnBnClickedBtnDmStuAddqc)
	ON_BN_CLICKED(IDC_BTN_DM_STU_DELQC, &CQueryByStudent::OnBnClickedBtnDmStuDelqc)
	ON_BN_CLICKED(IDC_BTN_DM_STU_CLEARQC, &CQueryByStudent::OnBnClickedBtnDmStuClearqc)
END_MESSAGE_MAP()


// CQueryByStudent message handlers

INT_PTR CQueryByStudent::DoModal()
{
	CExtDllState extdllstate;

	return CDialog::DoModal();
}

BOOL CQueryByStudent::OnInitDialog()
{
	CDialog::OnInitDialog();

	//初始化查询列表
	CRect rectGrid;
	GetDlgItem(IDC_STATIC_DM_QMARK_GRID)->GetWindowRect(&rectGrid);
	ScreenToClient(&rectGrid);
	if (!m_QueryGrid.Create(WS_CHILD | WS_TABSTOP | WS_VISIBLE, rectGrid, this,
		-1))
	{
		return FALSE;
	}

	m_QueryGrid.EnableDragHeaderItems(FALSE);
	m_QueryGrid.EnableColumnAutoSize (TRUE);
	m_QueryGrid.EnableGroupByBox (FALSE);
	m_QueryGrid.SetWholeRowSel(FALSE);
	m_QueryGrid.EnableHeader (TRUE, 0);

	CBCGPGridColors colors;
	colors.m_LeftOffsetColors.m_clrBackground = globalData.clrWindow;
	m_QueryGrid.SetColorTheme (colors);
	int iWidthGrid = rectGrid.Width();
	m_QueryGrid.InsertColumn(0, L"属性", iWidthGrid/4 - 2);
	m_QueryGrid.InsertColumn(1, L"操作", iWidthGrid/4);
	m_QueryGrid.InsertColumn(2, L"属性值", iWidthGrid*2/4);

	m_QueryGrid.AdjustLayout ();

	
	CreateQuestionQueryProp();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

HRESULT CQueryByStudent::CreateQuestionQueryProp()
{
	CQueryProp* pQueryProp = new CQueryProp;
	pQueryProp->m_strInternalName = FIELD_YDSTU_NAME;
	pQueryProp->m_strDisplayName = _T("姓名");
	pQueryProp->m_uType = VT_BSTR;
	m_lstQueryProp.push_back(pQueryProp);

	pQueryProp = new CQueryProp;
	pQueryProp->m_strInternalName = FIELD_YDSTU_ID;
	pQueryProp->m_strDisplayName = _T("学号");
	pQueryProp->m_uType = VT_BSTR;
	m_lstQueryProp.push_back(pQueryProp);

	pQueryProp = new CQueryProp;
	pQueryProp->m_strInternalName = FIELD_YDSTUMARK_EXAMID;
	pQueryProp->m_strDisplayName = _T("准考证号");
	pQueryProp->m_uType = VT_BSTR;
	m_lstQueryProp.push_back(pQueryProp);
	
	return S_OK;
}

void CQueryByStudent::OnBnClickedBtnDmStuAddqc()
{
	HRESULT hr = E_FAIL;
	CBCGPGridRow* pRow = m_QueryGrid.CreateRow(m_QueryGrid.GetColumnCount());
	pRow->GetItem(0)->SetValue(_T(""));
	pRow->GetItem(1)->SetValue(_T(""));
	pRow->GetItem(2)->SetValue(_T(""));


	CBCGPGridItem* pItem = new CBCGPGridItem("");
	for(std::list<CQueryProp*>::const_iterator itr = m_lstQueryProp.begin();
		itr != m_lstQueryProp.end();++itr)
	{
		pItem->AddOption((*itr)->m_strDisplayName);
	}
	pRow->ReplaceItem (0, pItem);
	//pRow->GetItem(0)->Enable(FALSE);

	pItem = new CBCGPGridItem(_T(""));
	pItem->AddOption(_T("等于"));
	pItem->AddOption(_T("不等于"));
	pItem->AddOption(_T("包含"));

	pRow->ReplaceItem (1, pItem);
	//pRow->GetItem(1)->Enable(FALSE);

	m_QueryGrid.AddRow(pRow);
	GetDlgItem(IDC_BTN_DM_STU_ADDQC)->EnableWindow(FALSE);
}

void CQueryByStudent::OnBnClickedBtnDmStuDelqc()
{
	CUIntArray nRows;
	CList<CBCGPGridItem*,CBCGPGridItem*> selItems;
	m_QueryGrid.GetSelectedItems(selItems);
	POSITION pos = selItems.GetHeadPosition();
	while(pos!=NULL)
	{
		CBCGPGridItem* pItem = selItems.GetNext(pos);
		CBCGPGridItemID &itemID = pItem->GetGridItemID();
		BOOL bFound = FALSE;
		for(int i=0;i<nRows.GetSize();i++)
		{
			if(itemID.m_nRow == nRows[i])
			{
				bFound = TRUE;
				break;
			}
		}
		if(!bFound) nRows.Add(itemID.m_nRow);
	}
	for(int i=nRows.GetSize()-1;i>=0;i--)
	{
		m_QueryGrid.RemoveRow(nRows[i],TRUE);
	}
	GetDlgItem(IDC_BTN_DM_STU_ADDQC)->EnableWindow(TRUE);
}

void CQueryByStudent::OnBnClickedBtnDmStuClearqc()
{
	while(m_QueryGrid.GetRowCount() > 0)
	{
		m_QueryGrid.RemoveRow(0);
	}
	GetDlgItem(IDC_BTN_DM_STU_ADDQC)->EnableWindow(TRUE);
}


HRESULT CQueryByStudent::CreatePropQueryContidion(std::list<CPropQueryContidition*>& lstPropQuery, BOOL* pbFromStu)
{
	UpdateData(TRUE);
	HRESULT hr = E_FAIL;
	
	*pbFromStu = TRUE;
	for(int i = 0; i< m_QueryGrid.GetRowCount();i++)
	{
		CBCGPGridRow* pRow = m_QueryGrid.GetRow(i);
		CBCGPGridItem* pFieldItem = pRow->GetItem(0);
		ASSERT(pFieldItem);
		CString strDisplayName = pFieldItem->GetLabel();
		CString strFieldName;
		UINT uFieldType = VT_I4;
		for(std::list<CQueryProp*>::const_iterator itr = m_lstQueryProp.begin();
			itr != m_lstQueryProp.end();++itr)
		{
			if(strDisplayName.CompareNoCase((*itr)->m_strDisplayName) == 0)
			{
				strFieldName = (*itr)->m_strInternalName;

				uFieldType = (*itr)->m_uType;
				if (strFieldName.CompareNoCase(FIELD_YDSTUMARK_EXAMID) == 0)
				{
					*pbFromStu = FALSE;
				}
				break;
			}
		}
		if(strFieldName.IsEmpty())
		{
			//没有找到字段
			continue;
		}
		CPropQueryContidition* pPropQuery = new CPropQueryContidition();
		
		pPropQuery->m_strFiledName = strFieldName;
		pPropQuery->m_uFieldType = uFieldType;
		UINT uOptype = 0;
		CBCGPGridItem* pOpItem = pRow->GetItem(1);
		ASSERT(pOpItem);
		CString strOp = pOpItem->GetLabel();
		if(strOp.CompareNoCase(_T("等于")) == 0)
		{
			pPropQuery->m_uOpType = Q_EQUAL;
		}
		else if(strOp.CompareNoCase(_T("不等于")) == 0)
		{
			pPropQuery->m_uOpType = Q_NOTEQUAL;
		}
		else if(strOp.CompareNoCase(_T("包含")) == 0)
		{
			pPropQuery->m_uOpType = Q_CONTAIN;
		}
		else
		{
			delete pPropQuery;
			pPropQuery = NULL;
			return S_OK;
		}
		lstPropQuery.push_back(pPropQuery);
		CBCGPGridItem* pValItem = pRow->GetItem(2);
		ASSERT(pValItem);
		if (strFieldName == FIELD_YDSTU_GENDER)
		{
			CString str = pValItem->GetLabel();
			if (str == L"男")
			{
				pPropQuery->m_strConVal = L"0";
			}
			else if (str == L"女")
			{
				pPropQuery->m_strConVal = L"1";
			}
		}
		else
		{
			pPropQuery->m_strConVal = pValItem->GetLabel();
		}

	}
	return S_OK;
}