// DlgPaperEditAnswer.cpp : implementation file
//

#include "stdafx.h"
#include "DlgPaperEditAnswer.h"
#include "../Base\DataHandler.h"
#include "../YDExamObjRef\AnswerRecord.h"


// CDlgPaperEditAnswer dialog

IMPLEMENT_DYNAMIC(CDlgPaperEditAnswer, CDialog)

CDlgPaperEditAnswer::CDlgPaperEditAnswer(std::list<CAnswerRecord*>* _pListRecord,
											CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPaperEditAnswer::IDD, pParent)
{
	m_pListRecord = _pListRecord;
}

CDlgPaperEditAnswer::~CDlgPaperEditAnswer()
{
}

void CDlgPaperEditAnswer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgPaperEditAnswer, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgPaperEditAnswer::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgPaperEditAnswer message handlers

BOOL CDlgPaperEditAnswer::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CRect rect;
	GetDlgItem(IDC_STATIC_BK)->GetWindowRect(&rect);
	GetDlgItem(IDC_STATIC_BK)->ShowWindow(SW_HIDE);
	ScreenToClient(&rect);
	if (!m_lstGridAnswer.Create(WS_CHILD | WS_TABSTOP | WS_VISIBLE, rect, this,
		-1))
	{
		return FALSE;
	}

	m_lstGridAnswer.EnableDragHeaderItems(FALSE);
	m_lstGridAnswer.EnableColumnAutoSize (TRUE);
	m_lstGridAnswer.EnableGroupByBox (FALSE);
	m_lstGridAnswer.SetWholeRowSel(FALSE);
	m_lstGridAnswer.EnableHeader (TRUE, 0);

	CBCGPGridColors colors;
	colors.m_LeftOffsetColors.m_clrBackground = globalData.clrWindow;
	m_lstGridAnswer.SetColorTheme (colors);
	int iWidth = rect.Width();
	m_lstGridAnswer.InsertColumn(0, L"序号", iWidth/4 - 2);
	m_lstGridAnswer.InsertColumn(1, L"题号", iWidth/4);
	m_lstGridAnswer.InsertColumn(2, L"答案", iWidth/4);
	m_lstGridAnswer.InsertColumn(3, L"分值", iWidth/4);

	for (int iColumn = 0; iColumn < m_lstGridAnswer.GetColumnCount(); iColumn++)
	{
		m_lstGridAnswer.SetHeaderAlign(iColumn,HDF_CENTER);
		m_lstGridAnswer.SetColumnAlign(iColumn, HDF_LEFT);
	}
	HRESULT hr = E_FAIL;
	ASSERT(m_pListRecord);
	for(std::list<CAnswerRecord*>::const_iterator itr = m_pListRecord->begin();
		itr != m_pListRecord->end();++itr)
	{
		hr = InsertItem(*itr);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
			return FALSE;
		}
	}
	m_lstGridAnswer.AdjustLayout();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

HRESULT CDlgPaperEditAnswer::InsertItem(CAnswerRecord* _pRecord)
{
	HRESULT hr = E_FAIL;
	CBCGPGridRow* pRow = m_lstGridAnswer.CreateRow(m_lstGridAnswer.GetColumnCount());

	pRow->SetData(DWORD_PTR(_pRecord));

	UINT iSerNO = m_lstGridAnswer.GetRowCount();
	++iSerNO;
	COLORREF bg;
	if (iSerNO%2 == 0)
	{
		bg = RGB(110,180,200);
	}
	else
	{
		bg = RGB(225, 225, 225);
	}
	
	//序号
	CString strSerNo;
	strSerNo.Format(L"%d", iSerNO);
	_variant_t varSerNO;
	CDataHandler::StringToVariant(strSerNo, VT_I4, &varSerNO);
	pRow->GetItem(0)->SetValue(varSerNO);
	pRow->GetItem(0)->SetBackgroundColor(bg);
	pRow->GetItem(0)->Enable(FALSE);
	pRow->ReplaceItem(1, new CBCGPGridItem(_bstr_t(_pRecord->m_strNO)));
	pRow->ReplaceItem(2, new CBCGPGridItem(_bstr_t(_pRecord->m_strAnswer)));
	pRow->ReplaceItem(3, new CBCGPGridItem(_bstr_t(_pRecord->m_strMark)));

	pRow->GetItem(1)->SetBackgroundColor(bg);
	pRow->GetItem(2)->SetBackgroundColor(bg);
	pRow->GetItem(3)->SetBackgroundColor(bg);
	m_lstGridAnswer.AddRow(pRow);
	return S_OK;
}
void CDlgPaperEditAnswer::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	HRESULT hr = E_FAIL;
	VARIANT_BOOL bValidate = VARIANT_FALSE;
	hr = ValidateData(&bValidate);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	if(!bValidate)
	{
		return ;
	}
	for(int i = 0; i < m_lstGridAnswer.GetRowCount();i++)
	{
		CBCGPGridRow* pRow = m_lstGridAnswer.GetRow(i);
		ASSERT(pRow);
		CAnswerRecord* pRecord = (CAnswerRecord*)pRow->GetData();
		ASSERT(pRecord);
		_variant_t varAnswer = pRow->GetItem(2)->GetValue();
		_variant_t varMark = pRow->GetItem(3)->GetValue();
		pRecord->m_strAnswer = CDataHandler::VariantToString(varAnswer);
		pRecord->m_strMark = CDataHandler::VariantToString(varMark);
	}
	OnOK();
}

HRESULT CDlgPaperEditAnswer::ValidateData(VARIANT_BOOL* _bValidate)
{
	HRESULT hr = E_FAIL;
	*_bValidate = VARIANT_FALSE;
	for (int i=0; i<m_lstGridAnswer.GetRowCount(); ++i)
	{
		CBCGPGridRow* pRow = m_lstGridAnswer.GetRow(i);
		_variant_t varNO = pRow->GetItem(1)->GetValue();
		CString strNO = varNO;
		_variant_t varAnswer = pRow->GetItem(2)->GetValue();
		CString strAnswer = varAnswer;
		_variant_t varMark = pRow->GetItem(3)->GetValue();


		int cEmpty = 0;
		CString strMakr = varMark;
		if (strNO.IsEmpty())
		{
			++cEmpty;
		}
		if (strAnswer.IsEmpty())
		{
			++cEmpty;
		}
		if (strMakr.IsEmpty())
		{
			++cEmpty;
		}
// 		if (cEmpty != 0 && cEmpty != 3)
// 		{
// 			_variant_t varSerNo = pRow->GetItem(0)->GetValue();
// 			CString strSerNo = varSerNo;
// 			CString strInfo = L"第" +  strSerNo + L"填写不完整！";
// 			AfxMessageBox(strInfo);
// 			return S_FALSE;
// 		}
		if (!strMakr.IsEmpty())
		{
			if(!CDataHandler::StringIsDigit(strMakr))
			{
				_variant_t varSerNo = pRow->GetItem(0)->GetValue();
				CString strSerNo = varSerNo;
				CString strInfo = L"第" +  strSerNo + L"分数不能非数值型！";
				AfxMessageBox(strInfo);
				return S_FALSE;
			}
			long lMark = CDataHandler::VariantToLong(varMark);
			if(lMark < 0)
			{
				_variant_t varSerNo = pRow->GetItem(0)->GetValue();
				CString strSerNo = varSerNo;
				CString strInfo = L"第" +  strSerNo + L"分数不能为负！";
				AfxMessageBox(strInfo);
				return S_FALSE;
			}
		}
	}
	*_bValidate = VARIANT_TRUE;
	return S_OK;
}