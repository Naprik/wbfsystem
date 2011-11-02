// GeneratePaperFormView.cpp : implementation file
//

#include "stdafx.h"
#include "../ObjRef/YDQuestionVault.h"
#include "../ObjRef/YDQuestionType.h"
#include "../ObjRef/YDQuestionRef.h"
#include "../ObjRef/YdObjWrapper.h"
#include "../ObjRef/YDPaper.h"
#include "GeneratePaperFormView.h"

#include "../Base/AutoClean.h"
#include "../Base/DataHandler.h"
#include "../UIBase/PdemWait.h"
#include "../WordBase/WordAppHelper.h"
#include "../Include/ShowErr.h"
#include "YDTranslationQuestionDlg.h"
#include "YDReadQuestionDlg.h"
#include "YDChoiceQuestionDlg.h"
#include "../Base/FilePathHelper.h"
#include "../DBBase/DBTransactionRef.h"

#include "SelFolderDlg.h"

#include "bcgpgridctrl.h"

// CGeneratePaperFormView

IMPLEMENT_DYNCREATE(CGeneratePaperFormView, CYdFormView)

CGeneratePaperFormView::CGeneratePaperFormView()
	: CYdFormView(CGeneratePaperFormView::IDD)
{

}

CGeneratePaperFormView::~CGeneratePaperFormView()
{
	delete m_pPaper;
	m_pPaper = NULL;
	CListAutoClean<CYDQuestionType> clean1(m_lstClean);
}

void CGeneratePaperFormView::DoDataExchange(CDataExchange* pDX)
{
	CYdFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_PAPERTEMP, m_cmbPaperTemp);
}

BEGIN_MESSAGE_MAP(CGeneratePaperFormView, CYdFormView)
	ON_BN_CLICKED(IDC_BTN_GP_OK, &CGeneratePaperFormView::OnBnClickedBtnGpOk)
	ON_BN_CLICKED(IDC_BTN_GP_CANCEL, &CGeneratePaperFormView::OnBnClickedBtnGpCancel)
	ON_BN_CLICKED(IDC_BUTTON_RESEL, &CGeneratePaperFormView::OnBnClickedButtonResel)
	ON_BN_CLICKED(IDC_BUTTON_INPUT, &CGeneratePaperFormView::OnBnClickedButtonInput)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, &CGeneratePaperFormView::OnBnClickedButtonExport)
	ON_BN_CLICKED(IDC_BTN_OBSOLETE, &CGeneratePaperFormView::OnBnClickedButtonObsolete)
	ON_CBN_SELCHANGE(IDC_CMB_PAPERTEMP, &CGeneratePaperFormView::OnCbnSelchangeCmbPapertemp)
END_MESSAGE_MAP()


// CGeneratePaperFormView diagnostics

#ifdef _DEBUG
void CGeneratePaperFormView::AssertValid() const
{
	CYdFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CGeneratePaperFormView::Dump(CDumpContext& dc) const
{
	CYdFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CGeneratePaperFormView message handlers

void CGeneratePaperFormView::OnInitialUpdate()
{
	CYdFormView::OnInitialUpdate();

	HRESULT hr = E_FAIL;

	CRect rect;
	GetDlgItem(IDC_STATIC_GRID)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	GetDlgItem(IDC_STATIC_GRID)->ShowWindow(SW_HIDE);
	if (!m_gridList.Create(WS_CHILD | WS_TABSTOP | WS_VISIBLE, rect, this,
		IDC_BCGPGRIDCTRL_ID_INPLACE))
	{
		return;
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

	CString strRelNum = L"每题问题数";
	nSize = GetDC()->GetTextExtent(strRelNum).cx;
	m_gridList.InsertColumn(2, strRelNum, nSize);

	CString strNum = L"出题题数";
	nSize = GetDC()->GetTextExtent(strNum).cx;
	m_gridList.InsertColumn(3, strNum, nSize);

	for (int iColumn = 0; iColumn < m_gridList.GetColumnCount(); iColumn++)
	{
		m_gridList.SetHeaderAlign(iColumn,HDF_CENTER);
		m_gridList.SetColumnAlign(iColumn, HDF_LEFT);
	}
	FillGrid();

	CRect rectPrev;
	GetDlgItem(IDC_STATIC_PREV)->GetWindowRect(&rectPrev);
	ScreenToClient(&rectPrev);
	if (!m_gridPreview.Create(WS_CHILD | WS_TABSTOP | WS_VISIBLE, rectPrev, this,
		IDC_PAPER_GCL_PREVIEW))
	{
		return;
	}

	m_gridPreview.EnableDragHeaderItems(TRUE);
	m_gridPreview.EnableColumnAutoSize (TRUE);
	m_gridPreview.EnableGroupByBox (FALSE);
	m_gridPreview.SetWholeRowSel(TRUE);
	m_gridPreview.EnableHeader (TRUE, 0);

	m_gridPreview.SetColorTheme (colors);

	m_gridPreview.InsertColumn(0, strName, rectPrev.Width()/10 -1);
	CString strNO = L"序号";
	m_gridPreview.InsertColumn(1, strNO, rectPrev.Width()/10 -1);
	CString strContent = L"内容";
	m_gridPreview.InsertColumn(2, strContent, rectPrev.Width()*8/10 -1);

	for (int iColumn = 0; iColumn < m_gridPreview.GetColumnCount(); iColumn++)
	{
		m_gridPreview.SetHeaderAlign(iColumn,HDF_CENTER);
		m_gridPreview.SetColumnAlign(iColumn, HDF_LEFT);
	}

	m_gridPreview.AdjustLayout();

	std::list<CString> lstTemplateName;
	hr = InitQuestionTemplate(lstTemplateName);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	for(std::list<CString>::const_iterator itr = lstTemplateName.begin();
		itr != lstTemplateName.end();++itr)
	{
		m_cmbPaperTemp.AddString(*itr);
	}
	if(lstTemplateName.size() > 0)
	{
		//初始化默认选中第一个
		m_cmbPaperTemp.SetCurSel(0);
		OnCbnSelchangeCmbPapertemp();
	}
}

HRESULT CGeneratePaperFormView::FillGrid()
{
	CYdObjWrapper* pObjWrapper = m_pSheet->GetCurObjWrapper();
	CYDQuestionVault* pVault = dynamic_cast<CYDQuestionVault*>(pObjWrapper->m_pObjRef);
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	m_pPaper = new CYDPaper(pDB);
	pVault->GetID(&m_VaultID);
	m_pPaper->SetQVault(m_VaultID);
	CFtpRef* pFtpRef = NULL;
	CWnd* pWnd = AfxGetMainWnd();
	if(pWnd != NULL && pWnd->GetSafeHwnd() != NULL)
	{
		pFtpRef = (CFtpRef*)pWnd->SendMessage(WM_YD_GET_FTPREF);
	}
	m_pPaper->SetFtpRef(pFtpRef);

	HRESULT hr = pVault->GetAllQuestionType(&m_lstClean);
	if (FAILED(hr))
	{
		return hr;
	}
	CBCGPGridRow* pRowReading = m_gridList.CreateRow(m_gridList.GetColumnCount());
	//题型
	pRowReading->GetItem(0)->SetValue(CComVariant(L"阅读理解题"));
	pRowReading->GetItem(0)->SetBackgroundColor(RGB(110,180,200));
	pRowReading->GetItem(0)->Enable(FALSE);
	//难度
	pRowReading->GetItem(1)->SetBackgroundColor(RGB(110,180,200));
	pRowReading->GetItem(1)->Enable(FALSE);
	//每题问题数
	pRowReading->GetItem(2)->SetBackgroundColor(RGB(110,180,200));
	pRowReading->GetItem(2)->Enable(FALSE);
	//题数
	pRowReading->GetItem(3)->SetBackgroundColor(RGB(110,180,200));
	pRowReading->GetItem(3)->Enable(FALSE);

	pRowReading->AllowSubItems();

	m_gridList.AddRow(pRowReading);
	std::list<CYDQuestionType*>::const_iterator itr = 
		m_lstClean.begin();
	for (; itr != m_lstClean.end(); ++itr)
	{
		CComVariant var;
		hr = (*itr)->GetPropVal(FIELD_QUESTIONTYPE_TYPE, &var);
		if (FAILED(hr))
		{
			return hr;
		}
		QTYPE qType = (QTYPE)CDataHandler::VariantToLong(var);
		if (qType == QTYPE_READINGVOCABULARY ||
			qType == QTYPE_READINGCOMPREHENSION ||
			qType == QTYPE_SKIMMING)
		{
			InsertItem(*itr, pRowReading);
		}
	}

	CBCGPGridRow* pRowListioning = m_gridList.CreateRow(m_gridList.GetColumnCount());
	//题型
	pRowListioning->GetItem(0)->SetValue(CComVariant(L"听力题"));
	pRowListioning->GetItem(0)->SetBackgroundColor(RGB(110,180,200));
	pRowListioning->GetItem(0)->Enable(FALSE);
	//难度
	pRowListioning->GetItem(1)->SetBackgroundColor(RGB(110,180,200));
	pRowListioning->GetItem(1)->Enable(FALSE);
	//每题问题数
	pRowListioning->GetItem(2)->SetBackgroundColor(RGB(110,180,200));
	pRowListioning->GetItem(2)->Enable(FALSE);
	//题数
	pRowListioning->GetItem(3)->SetBackgroundColor(RGB(110,180,200));
	pRowListioning->GetItem(3)->Enable(FALSE);

	pRowListioning->AllowSubItems();

	m_gridList.AddRow(pRowListioning);
	itr = m_lstClean.begin();
	for (; itr != m_lstClean.end(); ++itr)
	{
		CComVariant var;
		hr = (*itr)->GetPropVal(FIELD_QUESTIONTYPE_TYPE, &var);
		if (FAILED(hr))
		{
			return hr;
		}
		QTYPE qType = (QTYPE)CDataHandler::VariantToLong(var);
		if (qType == QTYPE_LISTENINGCOMPREHENSION ||
			qType == QTYPE_LISTENINGSHORT ||
			qType == QTYPE_LISTENINGLONG ||
			qType == QTYPE_LISTENINGVOCABULARY)
		{
			InsertItem(*itr, pRowListioning);
		}
	}

	itr = m_lstClean.begin();
	for (; itr != m_lstClean.end(); ++itr)
	{
		CComVariant var;
		hr = (*itr)->GetPropVal(FIELD_QUESTIONTYPE_TYPE, &var);
		if (FAILED(hr))
		{
			return hr;
		}
		QTYPE qType = (QTYPE)CDataHandler::VariantToLong(var);
		if (qType != QTYPE_LISTENINGCOMPREHENSION &&
			qType != QTYPE_LISTENINGSHORT &&
			qType != QTYPE_LISTENINGLONG &&
			qType != QTYPE_LISTENINGVOCABULARY &&
			qType != QTYPE_READINGVOCABULARY &&
			qType != QTYPE_READINGCOMPREHENSION &&
			qType != QTYPE_SKIMMING)
		{
			InsertItem(*itr, NULL);
		}
	}
	return S_OK;
}

HRESULT CGeneratePaperFormView::InsertItem(CYDQuestionType* pType, CBCGPGridRow* pParentRow)
{
	HRESULT hr = E_FAIL;
	hr = pType->Restore();
	if (FAILED(hr))
	{
		return hr;
	}
	std::list<UINT> lstRelateQuestionsCount;
	hr = pType->GetArticleRelateQuestionsCount(m_VaultID, &lstRelateQuestionsCount);
	if (FAILED(hr))
	{
		return hr;
	}
	std::list<UINT>::const_iterator itrCount = lstRelateQuestionsCount.begin();

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
	//每题问题数
	CComVariant varCount(*itrCount);
	pRow->GetItem(2)->SetValue(varCount);
	pRow->GetItem(2)->SetBackgroundColor(RGB(110,180,200));
	pRow->GetItem(2)->Enable(FALSE);
	//题数
	pRow->ReplaceItem(3, new CBCGPGridItem(long(0)));

	if (pParentRow != NULL)
	{
		pParentRow->AddSubItem(pRow);
	}
	else
	{
		m_gridList.AddRow(pRow);
	}
	long lLevel = CYDQuestionRef::TranslateHardLevel(*itr);
	std::list<UINT> lst;
	pCfg->AddConfigureItem(lLevel, *itrCount, 0, lst);

	++itr;
	for (; itr != lstHardLevel.end(); ++itr)
	{
		CBCGPGridRow* pChildRow = m_gridList.CreateRow(m_gridList.GetColumnCount());
		pChildRow->SetData(DWORD_PTR(pCfg));
		//难度
		pChildRow->GetItem(1)->SetValue(CComVariant((*itr)));
		pChildRow->GetItem(1)->Enable(FALSE);
		//每题问题数
		CComVariant varCount(*itrCount);
		pChildRow->GetItem(2)->SetValue(varCount);
		pChildRow->GetItem(2)->SetBackgroundColor(RGB(110,180,200));
		pChildRow->GetItem(2)->Enable(FALSE);
		//题数
		pChildRow->ReplaceItem(3, new CBCGPGridItem(long(0)));
		pRow->AddSubItem(pChildRow);
		lLevel = CYDQuestionRef::TranslateHardLevel(*itr);
		pCfg->AddConfigureItem(lLevel, *itrCount, 0, lst);
	}
	++itrCount;
	for (; itrCount != lstRelateQuestionsCount.end(); ++itrCount)
	{
		bool bFirst = true;
		for (itr = lstHardLevel.begin() ; itr != lstHardLevel.end(); ++itr)
		{
			CBCGPGridRow* pChildRow = m_gridList.CreateRow(m_gridList.GetColumnCount());
			pChildRow->SetData(DWORD_PTR(pCfg));
			//难度
			pChildRow->GetItem(1)->SetValue(CComVariant((*itr)));
			pChildRow->GetItem(1)->Enable(FALSE);
			if (bFirst)
			{
				pChildRow->GetItem(1)->SetBackgroundColor(RGB(110,180,200));
				bFirst = false;
			}
			//每题问题数
			CComVariant varCount(*itrCount);
			pChildRow->GetItem(2)->SetValue(varCount);
			pChildRow->GetItem(2)->SetBackgroundColor(RGB(110,180,200));
			pChildRow->GetItem(2)->Enable(FALSE);
			//题数
			pChildRow->ReplaceItem(3, new CBCGPGridItem(long(0)));
			pRow->AddSubItem(pChildRow);
			lLevel = CYDQuestionRef::TranslateHardLevel(*itr);
			pCfg->AddConfigureItem(lLevel, *itrCount, 0, lst);
		}
	}

	return S_OK;
}

HRESULT CGeneratePaperFormView::FillPreview()
{
	HRESULT hr = E_FAIL;
	m_gridPreview.RemoveAll();
	std::list<CPaperTemplateItem*>::const_iterator itr = m_pPaper->m_pTemplate->m_lstItem.begin();
	UINT uCount = 1;
	for (; itr != m_pPaper->m_pTemplate->m_lstItem.end(); ++itr)
	{
		CYDQuestionType qtype(m_pPaper->m_pDb);
		qtype.SetID((*itr)->id);
		CComVariant varQTypeName;
		hr = qtype.GetPropVal(L"NAME", &varQTypeName);
		if (FAILED(hr))
		{
			return hr;
		}
		CString strQType = CDataHandler::VariantToString(varQTypeName);
		CYDPaper::MapContent::const_iterator itrMap = m_pPaper->m_mapPaperContent.find(strQType);
		if (itrMap != m_pPaper->m_mapPaperContent.end())
		{
			std::list<CYDQuestionRef*>::const_iterator itrQ= (*itrMap).second.begin();

			CBCGPGridRow* pRow = m_gridPreview.CreateRow(m_gridPreview.GetColumnCount());
			pRow->AllowSubItems();
			pRow->SetData(DWORD_PTR(*itrQ));
			pRow->GetItem(0)->SetValue(_variant_t((*itrMap).first));
			pRow->GetItem(1)->SetValue(_variant_t(uCount++));
			CString strLable;
			(*itrQ)->GetLabel(&strLable);
			pRow->GetItem(2)->SetValue(_variant_t(strLable));

			++itrQ;
			m_gridPreview.AddRow(pRow);

			pRow->Enable(FALSE);
			for (; itrQ != (*itrMap).second.end(); ++itrQ)
			{
				CBCGPGridRow* pChildRow = m_gridPreview.CreateRow(m_gridPreview.GetColumnCount());
				pChildRow->SetData(DWORD_PTR(*itrQ));
				pChildRow->GetItem(1)->SetValue(_variant_t(uCount++));
				(*itrQ)->GetLabel(&strLable);
				pChildRow->GetItem(2)->SetValue(_variant_t(strLable));
				pChildRow->Enable(FALSE);
				pRow->AddSubItem(pChildRow);
			}
		}
		
	}
	
	return S_OK;
}

void CGeneratePaperFormView::OnBnClickedBtnGpOk()
{
	CString str;
	GetDlgItem(IDC_EDIT_Paper)->GetWindowText(str);
	if (str.GetLength() <= 0)
	{
		AfxMessageBox(L"请输入试卷名称");
		return;
	}
	if (m_pPaper->m_mapPaperContent.size() > 0)
	{
		if (AfxMessageBox(L"是否重新生成试卷？", MB_YESNO, MB_ICONWARNING) == IDNO)
		{
			return;
		}
	}
	CPdemWait wait(L"正在抽取试题请稍候...", FALSE, 3);
	wait.BeginWait();
	for (int i=0; i<m_gridList.GetRowCount(); ++i)
	{
		CYDPaperBlockConfigure* pCfg = (CYDPaperBlockConfigure*)m_gridList.GetRow(i)->GetData();
		if (pCfg != NULL)
		{
			_variant_t varVal = m_gridList.GetRow(i)->GetItem(1)->GetValue();
			CString str = CDataHandler::VariantToString(varVal);
			long lLevel = CYDQuestionRef::TranslateHardLevel(str);
			varVal.Clear();
			varVal = m_gridList.GetRow(i)->GetItem(2)->GetValue();
			long lRelCount = CDataHandler::VariantToLong(varVal);
			varVal.Clear();
			varVal = m_gridList.GetRow(i)->GetItem(3)->GetValue();
			long lCount = CDataHandler::VariantToLong(varVal);
			pCfg->UpdateConfigureItem(lLevel, lRelCount, lCount);
		}
	}
	wait.StepIt();
	m_pPaper->MakePaper(str);
	wait.StepIt();
	FillPreview();
	wait.StepIt();
	m_gridPreview.AdjustLayout();
	GetDlgItem(IDC_BUTTON_EXPORT)->EnableWindow(TRUE);
}


void CGeneratePaperFormView::OnBnClickedBtnGpCancel()
{
	GetDlgItem(IDC_EDIT_Paper)->SetWindowText(L"");
	for (int i=0; i<m_gridList.GetRowCount(); ++i)
	{
		CYDPaperBlockConfigure* pCfg = (CYDPaperBlockConfigure*)m_gridList.GetRow(i)->GetData();
		if (pCfg != NULL)
		{
			CComVariant var(long(0));
			m_gridList.GetRow(i)->GetItem(3)->SetValue(var);
		}
	}

	UpdateData(FALSE);
}

void CGeneratePaperFormView::OnBnClickedButtonResel()
{
	CBCGPGridRow* pGridRow =  m_gridPreview.GetCurSel();
	if(pGridRow == NULL)
	{
		return;
	}
	CYDQuestionRef* pQuestion = (CYDQuestionRef*) pGridRow->GetData();
	if (pQuestion == NULL)
	{
		return;
	}
	CYDQuestionRef* pReSelQuestion;
	CBCGPGridRow* pParentRow = pGridRow->GetParent();
	CString strQType;
	if (pParentRow != NULL)
	{
		_variant_t var = pParentRow->GetItem(0)->GetValue();
		strQType = CDataHandler::VariantToString(var);
	}
	else
	{
		_variant_t var = pGridRow->GetItem(0)->GetValue();
		strQType = CDataHandler::VariantToString(var);
	}
	HRESULT hr = m_pPaper->ReplaceByLeastUsedCountQuestion(strQType, pQuestion, &pReSelQuestion);
	if (FAILED(hr))
	{
		return;
	}
	pGridRow->SetData(DWORD_PTR(pReSelQuestion));
	CString strLable;
	pReSelQuestion->GetLabel(&strLable);
	pGridRow->GetItem(2)->SetValue(_variant_t(strLable));

	GetDlgItem(IDC_BUTTON_EXPORT)->EnableWindow(TRUE);
}

void CGeneratePaperFormView::OnBnClickedButtonInput()
{
	// TODO: Add your control notification handler code here
	CBCGPGridRow* pGridRow = m_gridPreview.GetCurSel();
	if(pGridRow == NULL)
	{
		AfxMessageBox(_T("请选择要替换的题目！"));
		return;
	}
	CYDQuestionRef* pOldQuestion = (CYDQuestionRef*)pGridRow->GetData();
	if(pOldQuestion == NULL)
	{
		return;
	}
	CBCGPGridRow* pParentRow = pGridRow->GetParent();
	CString strQType;
	if (pParentRow != NULL)
	{
		_variant_t var = pParentRow->GetItem(0)->GetValue();
		strQType = CDataHandler::VariantToString(var);
	}
	else
	{
		_variant_t var = pGridRow->GetItem(0)->GetValue();
		strQType = CDataHandler::VariantToString(var);
	}

	HRESULT hr = E_FAIL;
	CYDQuestionDlg* pDlg = NULL;
	hr = CreateQuestionDlg(pOldQuestion,pDlg);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	ASSERT(pDlg);
	CYDQuestionType* pQType = NULL;
	hr = GetQuestionType(pOldQuestion,pQType);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	ASSERT(pQType);
	hr = pDlg->SetYdQType(pQType);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	CYDQuestionVault* pQVault = NULL;
	hr = GetQuestionVault(pQVault);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	ASSERT(pQVault);
	hr = pDlg->SetYdQValut(pQVault);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
// 	hr = pDlg->SetListOperate(m_plstOperate,0);
// 	if(FAILED(hr))
// 	{
// 		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
// 		return;
// 	}
	CPtrAutoClean<CYDQuestionDlg> clr(pDlg);
	if(pDlg->DoModal() != IDOK)
	{
		return ;
	}
	CYdObjWrapper* pNewObjWrapper = pDlg->m_pInPaperGenrateObjWrapper;
	ASSERT(pNewObjWrapper);
	CPtrAutoClean<CYdObjWrapper> clr1(pNewObjWrapper);
	CYDObjectRef* pObjRef = pNewObjWrapper->m_pObjRef;
	ASSERT(pObjRef);
	ASSERT(pObjRef->IsKindOf(RUNTIME_CLASS(CYDQuestionRef)));
	pNewObjWrapper->m_pObjRef = NULL;
	CYDQuestionRef* pNewQuestionRef = (CYDQuestionRef*)pObjRef;
	hr = m_pPaper->ReplaceQuestion(strQType,pOldQuestion,pNewQuestionRef);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	CString strLable;
	hr = pNewQuestionRef->GetLabel(&strLable);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	pGridRow->GetItem(2)->SetValue(_variant_t(strLable));
	pGridRow->SetData((DWORD_PTR)pNewQuestionRef);

	GetDlgItem(IDC_BUTTON_EXPORT)->EnableWindow(TRUE);
}

HRESULT CGeneratePaperFormView::CreateQuestionDlg(CYDQuestionRef* _pQuestion,CYDQuestionDlg* &_pDlg)
{
	HRESULT hr = E_FAIL;
	ASSERT(_pQuestion);
	QTYPE qType = QTYPE_UNKNOWN;
	hr = _pQuestion->GetQuestionType(&qType);
	if(FAILED(hr))
	{
		return hr;
	}
	if(qType == QTYPE_WRITING)
	{
		//作文题

		_pDlg = new CYDTranslationQuestionDlg();
	}
	else if(qType == QTYPE_READINGCOMPREHENSION)
	{
		//阅读理解
		_pDlg = new CYDReadQuestionDlg();
	}
	else if(qType == QTYPE_SKIMMING)
	{
		//快速阅读
		_pDlg = new CYDReadQuestionDlg();
	}
	else if(qType == QTYPE_READINGVOCABULARY)
	{
		//阅读填空
		_pDlg = new CYDReadQuestionDlg();
	}
	else if(qType == QTYPE_LISTENINGCOMPREHENSION)
	{
		//听力
		_pDlg = new CYDReadQuestionDlg();
	}
	else if (qType == QTYPE_LISTENINGSHORT)
	{
		//听力
		_pDlg = new CYDReadQuestionDlg();
	}
	else if (qType == QTYPE_LISTENINGLONG)
	{
		//听力
		_pDlg = new CYDReadQuestionDlg();
	}
	else if(qType == QTYPE_LISTENINGVOCABULARY)
	{
		//听力
		_pDlg = new CYDReadQuestionDlg();
	}
	else if (qType == QTYPE_ERRORCORRECTION)
	{
		//改错
		_pDlg = new CYDReadQuestionDlg();
	}
	else if(qType == QTYPE_CLOZE)
	{
		//完形填空
		_pDlg = new CYDReadQuestionDlg();
	}
	else if(qType == QTYPE_TRANSLATION)
	{
		//翻译
		_pDlg = new CYDTranslationQuestionDlg();
	}
	else if(qType == QTYPE_VOCABULARY)
	{
		//选择题
		_pDlg = new CYDChoiceQuestionDlg();
	}
	else
	{
		ASSERT(FALSE);
	}
	_pDlg->m_bIsInPaperGenerate = TRUE;
	return S_OK;
}

HRESULT CGeneratePaperFormView::GetQuestionVault(CYDQuestionVault* &_pQV)
{
	HRESULT hr = E_FAIL;
	ASSERT(m_pSheet);
	CYdObjWrapper* pObjWrapper = m_pSheet->GetCurObjWrapper();
	ASSERT(pObjWrapper);
	CYDLinkRef* pLinkRef = pObjWrapper->m_pLinkRef;
	ASSERT(pLinkRef);
	CYDObjectRef* pObjA = NULL;
	CYDObjectRef* pObjB = NULL;
	hr = pLinkRef->GetObjRef(pObjA,pObjB);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(pObjB->IsKindOf(RUNTIME_CLASS(CYDQuestionVault)));
	_pQV = (CYDQuestionVault*)pObjB;
	return S_OK;
}

HRESULT CGeneratePaperFormView::GetQuestionType(CYDQuestionRef* _pQuestion,
												CYDQuestionType* &_pType)
{
	HRESULT hr = E_FAIL;
	_pType = NULL;
	CComVariant varType;
	hr = _pQuestion->GetPropVal(L"TYPEID", &varType);
	if (FAILED(hr))
	{
		return hr;
	}
	OBJID idtype = CDataHandler::VariantToLong(varType);
	for(std::list<CYDQuestionType*>::const_iterator itr = m_lstClean.begin();
		itr != m_lstClean.end();++itr)
	{
		UINT idItr = 0;
		hr = (*itr)->GetID(&idItr);
		if (FAILED(hr))
		{
			return hr;
		}
		if(idItr == idtype)
		{
			_pType = (*itr);
			return S_OK;
		}
	}
	ASSERT(FALSE);
	return S_FALSE;
}


void CGeneratePaperFormView::OnBnClickedButtonExport()
{
	//禁止 COM异步调用容易产生的 切换对话框
	AfxOleGetMessageFilter()->EnableBusyDialog(FALSE); 
	AfxOleGetMessageFilter()->SetBusyReply(SERVERCALL_RETRYLATER);   
	AfxOleGetMessageFilter()->EnableNotRespondingDialog(TRUE);   
	AfxOleGetMessageFilter()->SetMessagePendingDelay(-1); 

	COleDateTime date = COleDateTime::GetCurrentTime();
	CString strTime; 
	strTime = date.Format(L"%Y%m%d%H%M%S"); 

	int iCount = m_pPaper->m_mapPaperContent.size();
	++iCount;
	CPdemWait wait(L"正在导出试题请稍候...", FALSE, iCount);
	CComVariant varPaperName;
	m_pPaper->GetPropVal(L"NAME", &varPaperName);
	CString strPaperName = CDataHandler::VariantToString(varPaperName);
	strPaperName += strTime;
	CComVariant varNewName(strPaperName);
	m_pPaper->SetPropVal(L"NAME", &varNewName);
	CComVariant varCreator(DEFAULT_CREATOR);
	m_pPaper->SetPropVal(L"CREATOR", &varCreator);
	CComVariant valCreateDate;
	valCreateDate.vt = VT_DATE;
	valCreateDate.date = date;
	
	m_pPaper->SetPropVal(L"CREATEDATE",&valCreateDate);
	
	CString strPath;
	CFilePathHelper::GetMainCfgPath(strPath);
	strPath += L"//temp//paper//";
	
	CString strFolder = strPaperName + strTime;
	CFilePathHelper pathHelper;
	CString strFolderPath = strPath + strFolder;
	CString s = strFolderPath + L"/";
	if (!pathHelper.CreateFolder(s))
	{
		AfxMessageBox(L"创建文件夹失败！");
		return;
	}
	m_strPaperFolder = strFolderPath;

	strPath = strFolderPath;
	strPath += L"/";
	strPath += strPaperName;
	CString strAnswer = strPath;
	strPath += L".doc";
	strAnswer += L"答案.doc";

	wait.BeginWait();
	{
		CWordAppHelper wordHelper;
		CWordAppHelper wordAnswer;
		wordHelper.Create();
		wordAnswer.Create();
		CFtpRef* pFtpRef = NULL;
		CWnd* pWnd = AfxGetMainWnd();
		if(pWnd != NULL && pWnd->GetSafeHwnd() != NULL)
		{
			pFtpRef = (CFtpRef*)pWnd->SendMessage(WM_YD_GET_FTPREF);
		}
		ASSERT(pFtpRef);
		m_pPaper->SetFtpRef(pFtpRef);
		m_pPaper->PersistToMSWord(&wordHelper, &wordAnswer, strFolderPath, &wait);
		
		wordHelper.Save(strPath);
		wordAnswer.Save(strAnswer);
		wait.StepIt();
		wait.Close();

	}
	GetDlgItem(IDC_BTN_OBSOLETE)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_EXPORT)->EnableWindow(FALSE);
	DWORD_PTR dw = (DWORD_PTR)ShellExecute(NULL, L"open", strPath, NULL, NULL , SW_SHOWNORMAL);

}

HRESULT CGeneratePaperFormView::InitQuestionTemplate(std::list<CString> &_lstTemplateName)
{
	HRESULT hr = E_FAIL;
	CString strFindFile;
	CFilePathHelper::GetMainCfgPath(strFindFile);
	strFindFile += _T("\\template\\paper\\*.XML");
	CFileFind finder;
	BOOL bWorking = finder.FindFile(strFindFile);

	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		// skip . and .. files; otherwise, we'd
		// recur infinitely!

		if (finder.IsDots())
			continue;

		// if it's a directory, recursively search it

		if (finder.IsDirectory())
		{
			continue;
		}
		CString strFindName = finder.GetFileName();
		_lstTemplateName.push_back(strFindName);
	}

	finder.Close();

	return S_OK;
}


void CGeneratePaperFormView::OnCbnSelchangeCmbPapertemp()
{
	// TODO: Add your control notification handler code here
	if(m_cmbPaperTemp.GetCurSel() == -1)
	{
		return;
	}
	CString strFileName;
	m_cmbPaperTemp.GetLBText(m_cmbPaperTemp.GetCurSel(),strFileName);
	ASSERT(!strFileName.IsEmpty());
	HRESULT hr = E_FAIL;
	hr = m_pPaper->m_pTemplate->Load(strFileName);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	
}

void CGeneratePaperFormView::OnBnClickedButtonObsolete()
{
	HRESULT hr = E_FAIL;
	CSelFolderDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		//设置guidname
		
		CComVariant varPaperName;
		m_pPaper->GetPropVal(L"NAME", &varPaperName);
		CString strPaperName = CDataHandler::VariantToString(varPaperName);
		CString strFileExt = L"doc";
		GUID guid;
		hr = CoCreateGuid(&guid);
		if(FAILED(hr))
		{
			return;
		}
		CString   strGUID; 
		strGUID.Format( _T("%08X%04X%04x%02X%02X%02X%02X%02X%02X%02X%02X.") 
			,   guid.Data1 
			,   guid.Data2 
			,   guid.Data3 
			,   guid.Data4[0],   guid.Data4[1] 
		,   guid.Data4[2],   guid.Data4[3],   guid.Data4[4],   guid.Data4[5] 
		,   guid.Data4[6],   guid.Data4[7] 
		); 
		strGUID += strFileExt;
		CComVariant varName(strGUID);
		m_pPaper->SetPropVal(L"GUIDNAME", &varName);
		CString strGUIDAnswer = L"";
		hr = CoCreateGuid(&guid);
		if(FAILED(hr))
		{
			return;
		}
		strGUIDAnswer.Format( _T("%08X%04X%04x%02X%02X%02X%02X%02X%02X%02X%02X.") 
			,   guid.Data1 
			,   guid.Data2 
			,   guid.Data3 
			,   guid.Data4[0],   guid.Data4[1] 
		,   guid.Data4[2],   guid.Data4[3],   guid.Data4[4],   guid.Data4[5] 
		,   guid.Data4[6],   guid.Data4[7] 
		); 
		strGUIDAnswer += strFileExt;
		CComVariant varAnswerName(strGUIDAnswer);
		m_pPaper->SetPropVal(L"GUIDANSWERNAME", &varAnswerName);
		//上传文件
		CString strPaperPath = m_strPaperFolder;
		strPaperPath += L"/";
		strPaperPath += strPaperName;
		CString strAnswerPath = strPaperPath;
		strPaperPath += L".doc";
		strAnswerPath += L"答案.doc";
		CString strPaperVaultName, strPaperAnswerVaultName;
		m_pPaper->CreateVaultName(strPaperVaultName, strPaperAnswerVaultName);
		hr = m_pPaper->m_pFtp->Upload(strPaperPath, strPaperVaultName);
		if (FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
			return;
		}
		hr = m_pPaper->m_pFtp->Upload(strAnswerPath, strPaperAnswerVaultName);
		if (FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
			return;
		}
		//持久化paper；
		CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
		ASSERT(pDB);
		CDBTransactionRef trans(pDB, TRUE);
		trans.Begin();
		hr = m_pPaper->Save();
		if (FAILED(hr))
		{
			trans.Rollback();
			return;
		}
		//持久化关系；
		CYDLinkRef linkRef(pDB, DB_PFOLDERPAPER);
		linkRef.PutObjRef(dlg.m_pPaperFolder, m_pPaper);
		hr = linkRef.Save();
		if (FAILED(hr))
		{
			trans.Rollback();
			return ;
		}
		hr = trans.Commit();
		if (FAILED(hr))
		{
			trans.Rollback();
			return ;
		}
		GetDlgItem(IDC_BTN_OBSOLETE)->EnableWindow(FALSE);
		AfxMessageBox(_T("试卷归档成功！"));
	}
}

BOOL CGeneratePaperFormView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
										 DWORD dwRequestedStyle, const RECT& rect, CWnd* pParentWnd, UINT nID,
										 CCreateContext* pContext)
{
	CExtDllState state;
	return CFormView::Create(lpszClassName,
		lpszWindowName,
		dwRequestedStyle,
		rect,
		pParentWnd,
		nID,
		pContext);
}