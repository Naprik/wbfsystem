// DlgErrorDetail.cpp : implementation file
//

#include "stdafx.h"
#include "DlgErrorDetail.h"
#include "../ObjHelper\PaperErrorResult.h"
#include "../ObjRef\YDPaper.h"
#include "../YDExamObjRef\YdStudentRef.h"
#include "../YDExamObjRef\YDStuMark.h"
#include "../Base\AutoClean.h"

// CDlgErrorDetail dialog

IMPLEMENT_DYNAMIC(CDlgErrorDetail, CBCGPDialog)

CDlgErrorDetail::CDlgErrorDetail(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgErrorDetail::IDD, pParent)
{

}

CDlgErrorDetail::~CDlgErrorDetail()
{
}

void CDlgErrorDetail::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgErrorDetail, CBCGPDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgErrorDetail message handlers

BOOL CDlgErrorDetail::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();
	ASSERT(m_pResult);
	int iColCount = m_pResult->m_lstErrorItemNo.size() + 2;
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
	HRESULT hr = E_FAIL;
	m_Grid.EnableDragHeaderItems(FALSE);
	m_Grid.EnableColumnAutoSize (TRUE);
	m_Grid.EnableGroupByBox (FALSE);
	m_Grid.SetWholeRowSel(FALSE);
	m_Grid.EnableHeader (TRUE, 0);

	CBCGPGridColors colors;
	colors.m_LeftOffsetColors.m_clrBackground = globalData.clrWindow;
	m_Grid.SetColorTheme (colors);
	int iWidthGrid = rectBK.Width();
	m_Grid.InsertColumn(0, L"题号", 50);
	int iColIndex = 1;
	for(std::list<UINT>::const_iterator itr = m_pResult->m_lstErrorItemNo.begin();
		itr != m_pResult->m_lstErrorItemNo.end();++itr,++iColIndex)
	{
		CString strColName;
		strColName.Format(_T("%d"),(*itr));
		m_Grid.InsertColumn(iColIndex, strColName, 50);
	}
	//插入标准答案
	{
		ASSERT(m_pResult->m_pPaper);
		CBCGPGridRow* pStandRow = m_Grid.CreateRow(m_Grid.GetColumnCount());
		ASSERT(pStandRow);
		pStandRow->GetItem(0)->SetValue(CComVariant(L"标准答案"));
		int iColIndex = 1;
		for(std::list<UINT>::const_iterator itr = m_pResult->m_lstErrorItemNo.begin();
			itr != m_pResult->m_lstErrorItemNo.end();++itr,++iColIndex)
		{
			CString strStandAnswer;
			hr = m_pResult->m_pPaper->GetStdAsnwerByItemNo((*itr),strStandAnswer);
			if(FAILED(hr))
			{
				DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
				return FALSE;
			}
			pStandRow->GetItem(iColIndex)->SetValue(CComVariant(strStandAnswer));
		}
		int index = m_Grid.AddRow(pStandRow);
	}
	//插入学生考试信息
	{
		for(std::list<CYDStuMark*>::const_iterator itr = m_pResult->m_lstStuMark.begin();
			itr != m_pResult->m_lstStuMark.end();++itr)
		{
			hr = InsertListByStuMark(*itr);
			if(FAILED(hr))
			{
				DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
				return FALSE;
			}
		}
	}
	m_Grid.AdjustLayout ();
	//插入第一列
	//插入标准答案
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

HRESULT CDlgErrorDetail::InsertListByStuMark(CYDStuMark* _pMark)
{
	HRESULT hr = E_FAIL;
	CBCGPGridRow* pRow = m_Grid.CreateRow(m_Grid.GetColumnCount());
	ASSERT(pRow);
	//第一列显示学生姓名+考试时间
	CYdStudentRef* pStuRef = NULL;
	hr = _pMark->GetStudent(pStuRef);
	if(FAILED(hr))
	{
		return hr;
	}
	CPtrAutoClean<CYdStudentRef> clr(pStuRef);
	CString strStuName;
	hr = pStuRef->GetPropVal(FIELD_YDSTU_NAME,strStuName);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strExamDate;
	hr = _pMark->GetPropVal(FIELD_YDSTUMARK_EXAMDATE,strExamDate);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strNameDate = strStuName + strExamDate;
	pRow->GetItem(0)->SetValue(CComVariant(strNameDate));
	//得到具体答案
	std::map<int, CString> mapAnswer;
	hr = _pMark->TransferAnswerByString(&mapAnswer);
	if(FAILED(hr))
	{
		return hr;
	}
	int iColInex = 1;
	for(std::list<UINT>::const_iterator itr = m_pResult->m_lstErrorItemNo.begin();
		itr != m_pResult->m_lstErrorItemNo.end();++itr,++iColInex)
	{
		UINT uItemNo = (*itr);
		std::map<int, CString>::iterator itrMap = mapAnswer.find(uItemNo);
		if (itrMap != mapAnswer.end())
		{
			CString strAnswer = (*itrMap).second;
			pRow->GetItem(iColInex)->SetValue(CComVariant(strAnswer));
		}
	}
	m_Grid.AddRow(pRow);
	return S_OK;
}
INT_PTR CDlgErrorDetail::DoModal()
{
	// TODO: Add your specialized code here and/or call the base class
	CExtDllState state;
	return CBCGPDialog::DoModal();
}

void CDlgErrorDetail::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDialog::OnSize(nType, cx, cy);

	CRect rect;   //获取控件变化前的大小  
	GetWindowRect(&rect);

	rect.left += 5;
	rect.right -= 5;
	rect.top += 5;
	rect.bottom -= 10;
	if(m_Grid.GetSafeHwnd() != NULL)
	{
		m_Grid.MoveWindow(rect);//设置控件大小
		m_Grid.UpdateData();
	}
}
