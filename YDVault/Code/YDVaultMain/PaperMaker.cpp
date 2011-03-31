// PaperMaker.cpp : implementation file
//

#include "stdafx.h"
#include "YDVaultMain.h"
#include "PaperMaker.h"
#include "../ObjRef/YDQuestionVault.h"
#include "../ObjRef/YDQuestionType.h"
#include "../ObjRef/YDQuestionRef.h"
#include "../Base/AutoClean.h"
#include "../ObjRef/YDPaper.h"
#include "../Base/DataHandler.h"

#include "bcgpgridctrl.h"

// CPaperMaker dialog

IMPLEMENT_DYNAMIC(CPaperMaker, CDialog)

CPaperMaker::CPaperMaker(CYDQuestionVault*& pVault, CWnd* pParent /*=NULL*/)
	: CDialog(CPaperMaker::IDD, pParent)
	, m_pVault(pVault)
{
	m_pPaper = new CYDPaper(theApp.m_pDatabase);
	OBJID objID;
	pVault->GetID(&objID);
	m_pPaper->SetQVault(objID);
}

CPaperMaker::~CPaperMaker()
{
	CListAutoClean<CYDQuestionType> clean1(m_lstClean);
	m_pVault = NULL;

	CPtrAutoClean<CYDPaper> clean2(m_pPaper);
}

void CPaperMaker::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPaperMaker, CDialog)
	ON_BN_CLICKED(IDC_BTN_GP_OK, &CPaperMaker::OnBnClickedBtnGpOk)
	ON_BN_CLICKED(IDC_BTN_GP_CANCEL, &CPaperMaker::OnBnClickedBtnGpCancel)
	ON_BN_CLICKED(IDC_BTN_GP_SAVE, &CPaperMaker::OnBnClickedBtnGpSave)
END_MESSAGE_MAP()


// CPaperMaker message handlers

BOOL CPaperMaker::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect rect;
	GetDlgItem(IDC_STATIC_GRID)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	if (!m_gridList.Create(WS_CHILD | WS_TABSTOP | WS_VISIBLE, rect, this,
		BCGPGRIDCTRL_ID_INPLACE))
	{
		return FALSE;
	}

	m_gridList.EnableDragHeaderItems(FALSE);
	m_gridList.EnableColumnAutoSize (TRUE);
	m_gridList.EnableGroupByBox (FALSE);
	m_gridList.SetWholeRowSel(FALSE);
	m_gridList.EnableHeader (TRUE, 0);

	CBCGPGridColors colors;
	colors.m_LeftOffsetColors.m_clrBackground = globalData.clrWindow;
	m_gridList.SetColorTheme (colors);

	int iCol = 0;
	CString strName = L"题型";
	int nSize = GetDC()->GetTextExtent(strName).cx;
	m_gridList.InsertColumn(0, strName, nSize*3);

	CString strHardLevel = L"难度";
	nSize = GetDC()->GetTextExtent(strHardLevel).cx;
	m_gridList.InsertColumn(1, strHardLevel, nSize*2);

	CString strNum = L"配置题数";
	nSize = GetDC()->GetTextExtent(strNum).cx;
	m_gridList.InsertColumn(2, strNum, nSize);
	
	for (int iColumn = 0; iColumn < m_gridList.GetColumnCount(); iColumn++)
	{
		m_gridList.SetHeaderAlign(iColumn,HDF_CENTER);
		m_gridList.SetColumnAlign(iColumn, HDF_LEFT);
	}
	FillGrid();
	m_gridList.AdjustLayout();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

HRESULT CPaperMaker::FillGrid()
{
	HRESULT hr = m_pVault->GetAllQuestionType(&m_lstClean);
	if (FAILED(hr))
	{
		return hr;
	}
	std::list<CYDQuestionType*>::const_iterator itr = 
		m_lstClean.begin();
	for (; itr != m_lstClean.end(); ++itr)
	{
		InsertItem(*itr);
	}
	return S_OK;
}

HRESULT CPaperMaker::InsertItem(CYDQuestionType* pType)
{
	HRESULT hr = E_FAIL;
	hr = pType->Restore();
	if (FAILED(hr))
	{
		return hr;
	}
	CBCGPGridRow* pRow = m_gridList.CreateRow(m_gridList.GetColumnCount());
	CYDPaperBlockConfigure* pCfg = new CYDPaperBlockConfigure();
	m_pPaper->AddPaperBlockConfigure(pCfg);	
	OBJID objID = ID_EMPTY;
	pType->GetID(&objID);
	pCfg->SetQType(objID);

	pRow->AllowSubItems();
	pRow->SetData(DWORD_PTR(pCfg));
	
	std::list<CString> lstHardLevel;
	CYDQuestionRef::GetAllHardLevel(&lstHardLevel);
	std::list<CString>::const_iterator itr = lstHardLevel.begin();
	//题型
	CComVariant varVal;
	hr = pType->GetPropVal(L"NAME", &varVal);
	if (FAILED(hr))
	{
		return hr;
	}
	pRow->GetItem(0)->SetValue(varVal);
	pRow->GetItem(0)->SetBackgroundColor(RGB(110,180,200));
	pRow->GetItem(0)->Enable(FALSE);
	//难度
	pRow->GetItem(1)->SetValue(CComVariant((*itr)));
	pRow->GetItem(1)->SetBackgroundColor(RGB(110,180,200));
	pRow->GetItem(1)->Enable(FALSE);
	//题数
	pRow->ReplaceItem(2, new CBCGPGridItem(long(0)));
	
	m_gridList.AddRow(pRow);
	long lLevel = CYDQuestionRef::TranslateHardLevel(*itr);
	std::list<UINT> lst;
	pCfg->AddConfigureItem(lLevel, 0, lst);

	++itr;
	for (; itr != lstHardLevel.end(); ++itr)
	{
		CBCGPGridRow* pChildRow = m_gridList.CreateRow(m_gridList.GetColumnCount());
		pChildRow->SetData(DWORD_PTR(pCfg));
		//难度
		pChildRow->GetItem(1)->SetValue(CComVariant((*itr)));
		pChildRow->GetItem(1)->Enable(FALSE);
		//题数
		pChildRow->ReplaceItem(2, new CBCGPGridItem(long(0)));
		pRow->AddSubItem(pChildRow);
		lLevel = CYDQuestionRef::TranslateHardLevel(*itr);
		pCfg->AddConfigureItem(lLevel, 0, lst);
	}
	
	return S_OK;
}
void CPaperMaker::OnBnClickedBtnGpOk()
{
	for (int i=0; i<m_gridList.GetColumnCount(); ++i)
	{
		CYDPaperBlockConfigure* pCfg = (CYDPaperBlockConfigure*)m_gridList.GetRow(i)->GetData();
		ASSERT(pCfg != NULL);
		_variant_t varVal = m_gridList.GetRow(i)->GetItem(1)->GetValue();
		CString str = CDataHandler::VariantToString(varVal);
		long lLevel = CYDQuestionRef::TranslateHardLevel(str);
		varVal.Clear();
		varVal = m_gridList.GetRow(i)->GetItem(2)->GetValue();
		long lCount = CDataHandler::VariantToLong(varVal);
		pCfg->UpdateConfigureItem(lLevel, lCount);
	}
	CString str;
	GetDlgItem(IDC_EDIT_Paper)->GetWindowText(str);
	m_pPaper->MakePaper(str);

	OnOK();
}

void CPaperMaker::OnBnClickedBtnGpCancel()
{
	OnCancel();
}

void CPaperMaker::OnBnClickedBtnGpSave()
{
	// TODO: Add your control notification handler code here
}
