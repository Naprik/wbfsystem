// DlgCfgQuestion.cpp : implementation file
//

#include "stdafx.h"
#include "ExerciseMain.h"
#include "DlgCfgQuestion.h"
#include "SelectQuestionCfgMgr.h"
#include "../Base\AutoClean.h"
#include "../ObjRef\YDQuestionType.h"
#include "../Base\DataHandler.h"
#include "../ObjRef\YDQuestionRef.h"
#include "../ObjHelper\StaticObjHelper.h"
#include "../ObjRef\YDQuestionVault.h"
#include "../ObjRef\YDPaper.h"
#include "QuestionCfgStruct.h"

const int COL_QUESTION_TYPE = 0;		//题型那一列
const int COL_QUESTION_HARDLEVEL = 1;	//难度
const int COL_QUESTION_EACH_NUM = 2;    //每题问题数
const int COL_QUESTION_NUM = 3;         //出题题数
const int COL_QUESTION_MARK = 4;        //分数

// CDlgCfgQuestion dialog

IMPLEMENT_DYNAMIC(CDlgCfgQuestion, CDialog)

CDlgCfgQuestion::CDlgCfgQuestion(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCfgQuestion::IDD, pParent)
{
	m_pSelQCfgMgr = NULL;
}

CDlgCfgQuestion::~CDlgCfgQuestion()
{
	CListAutoClean<CYDQuestionType> clean1(m_lstClean);
	CListAutoClean<CYDObjectRef> clr2(m_lstQuestionVault);
	CListAutoClean<CYDPaperBlockConfigure> clr3(m_lstCfg);
}

void CDlgCfgQuestion::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_QUESTION_VAULT, m_cmbQuestionVault);
}


BEGIN_MESSAGE_MAP(CDlgCfgQuestion, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgCfgQuestion::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_QUESTION_VAULT, &CDlgCfgQuestion::OnCbnSelchangeComboQuestionVault)
END_MESSAGE_MAP()


// CDlgCfgQuestion message handlers

BOOL CDlgCfgQuestion::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	ASSERT(m_pSelQCfgMgr);
	if(m_pSelQCfgMgr->m_uSelQuesitonMode == SEL_QUESTION_EXAM)
	{
		SetWindowText(_T("考试抽题配置"));
	}
	else if(m_pSelQCfgMgr->m_uSelQuesitonMode == SEL_QUESTION_EXERCISE)
	{
		SetWindowText(_T("练习抽题配置"));
	}
	else
	{
		ASSERT(FALSE);
	}
	CRect rectBK;
	GetDlgItem(IDC_STATIC_BK)->GetWindowRect(&rectBK);
	GetDlgItem(IDC_STATIC_BK)->ShowWindow(SW_HIDE);
	// TODO:  Add extra initialization here
	ScreenToClient(&rectBK);
	if (!m_gridList.Create(WS_CHILD | WS_TABSTOP | WS_VISIBLE, rectBK, this,
		-1))
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
	m_gridList.InsertColumn(COL_QUESTION_TYPE, strName, nSize*3);

	CString strHardLevel = L"难度";
	nSize = GetDC()->GetTextExtent(strHardLevel).cx;
	m_gridList.InsertColumn(COL_QUESTION_HARDLEVEL, strHardLevel, nSize*2);

	CString strRelNum = L"每题问题数";
	nSize = GetDC()->GetTextExtent(strRelNum).cx;
	m_gridList.InsertColumn(COL_QUESTION_EACH_NUM, strRelNum, nSize);

	CString strNum = L"出题题数";
	nSize = GetDC()->GetTextExtent(strNum).cx;
	m_gridList.InsertColumn(COL_QUESTION_NUM, strNum, nSize);
	
	CString strMark = L"分数";
	nSize = GetDC()->GetTextExtent(strMark).cx;
	m_gridList.InsertColumn(COL_QUESTION_MARK, strMark, nSize);
	

	for (int iColumn = 0; iColumn < m_gridList.GetColumnCount(); iColumn++)
	{
		m_gridList.SetHeaderAlign(iColumn,HDF_CENTER);
		m_gridList.SetColumnAlign(iColumn, HDF_LEFT);
	}
	HRESULT hr = E_FAIL;
	hr = InitQuestionVault();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

HRESULT CDlgCfgQuestion::FillGrid()
{
// 	CYdObjWrapper* pObjWrapper = m_pSheet->GetCurObjWrapper();
// 	CYDQuestionVault* pVault = dynamic_cast<CYDQuestionVault*>(pObjWrapper->m_pObjRef);
// 	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
// 	ASSERT(pDB);
// 	m_pPaper = new CYDPaper(pDB);
// 	pVault->GetID(&m_VaultID);
// 	m_pPaper->SetQVault(m_VaultID);
// 	CFtpRef* pFtpRef = NULL;
// 	CWnd* pWnd = AfxGetMainWnd();
// 	if(pWnd != NULL && pWnd->GetSafeHwnd() != NULL)
// 	{
// 		pFtpRef = (CFtpRef*)pWnd->SendMessage(WM_YD_GET_FTPREF);
// 	}
// 	m_pPaper->SetFtpRef(pFtpRef);
// 
// 	HRESULT hr = pVault->GetAllQuestionType(&m_lstClean);
// 	if (FAILED(hr))
// 	{
// 		return hr;
// 	}
	CWaitCursor wati;
	HRESULT hr = E_FAIL;
	if(m_cmbQuestionVault.GetCurSel() == CB_ERR)
	{
		return S_FALSE;
	}
	int iSel = m_cmbQuestionVault.GetCurSel();
	CYDQuestionVault* pVault = (CYDQuestionVault*)m_cmbQuestionVault.GetItemData(iSel);
	ASSERT(pVault);
	{
		CListAutoClean<CYDQuestionType> clean1(m_lstClean);
		CListAutoClean<CYDPaperBlockConfigure> clr3(m_lstCfg);
	}
	m_gridList.RemoveAll();
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	hr = pVault->GetAllQuestionType(&m_lstClean);
	if(FAILED(hr))
	{
		return hr;
	}
	CBCGPGridRow* pRowReading = m_gridList.CreateRow(m_gridList.GetColumnCount());
	//题型
	pRowReading->GetItem(COL_QUESTION_TYPE)->SetValue(CComVariant(L"阅读理解题"));
	pRowReading->GetItem(COL_QUESTION_TYPE)->SetBackgroundColor(RGB(110,180,200));
	pRowReading->GetItem(COL_QUESTION_TYPE)->Enable(FALSE);
	//难度
	pRowReading->GetItem(COL_QUESTION_HARDLEVEL)->SetBackgroundColor(RGB(110,180,200));
	pRowReading->GetItem(COL_QUESTION_HARDLEVEL)->Enable(FALSE);
	//每题问题数
	pRowReading->GetItem(COL_QUESTION_EACH_NUM)->SetBackgroundColor(RGB(110,180,200));
	pRowReading->GetItem(COL_QUESTION_EACH_NUM)->Enable(FALSE);
	//题数
	pRowReading->GetItem(COL_QUESTION_NUM)->SetBackgroundColor(RGB(110,180,200));
	pRowReading->GetItem(COL_QUESTION_NUM)->Enable(FALSE);

	//分数
	pRowReading->GetItem(COL_QUESTION_MARK)->SetBackgroundColor(RGB(110,180,200));
	pRowReading->GetItem(COL_QUESTION_MARK)->Enable(FALSE);

	pRowReading->AllowSubItems();

	m_gridList.AddRow(pRowReading);
	for (std::list<CYDQuestionType*>::const_iterator itr = m_lstClean.begin(); 
		itr != m_lstClean.end(); ++itr)
	{
		CComVariant var;
		hr = (*itr)->GetPropVal(L"TYPE", &var);
		if (FAILED(hr))
		{
			return hr;
		}
		QTYPE qType = (QTYPE)CDataHandler::VariantToLong(var);
		if (qType == QTYPE_READINGVOCABULARY ||
			qType == QTYPE_READINGCOMPREHENSION ||
			qType == QTYPE_SKIMMING)
		{
			InsertItem(*itr,pVault, pRowReading);
		}
	}

	CBCGPGridRow* pRowListioning = m_gridList.CreateRow(m_gridList.GetColumnCount());
	//题型
	pRowListioning->GetItem(COL_QUESTION_TYPE)->SetValue(CComVariant(L"听力题"));
	pRowListioning->GetItem(COL_QUESTION_TYPE)->SetBackgroundColor(RGB(110,180,200));
	pRowListioning->GetItem(COL_QUESTION_TYPE)->Enable(FALSE);
	//难度
	pRowListioning->GetItem(COL_QUESTION_HARDLEVEL)->SetBackgroundColor(RGB(110,180,200));
	pRowListioning->GetItem(COL_QUESTION_HARDLEVEL)->Enable(FALSE);
	//每题问题数
	pRowListioning->GetItem(COL_QUESTION_EACH_NUM)->SetBackgroundColor(RGB(110,180,200));
	pRowListioning->GetItem(COL_QUESTION_EACH_NUM)->Enable(FALSE);
	//题数
	pRowListioning->GetItem(COL_QUESTION_NUM)->SetBackgroundColor(RGB(110,180,200));
	pRowListioning->GetItem(COL_QUESTION_NUM)->Enable(FALSE);
	//分数
	pRowListioning->GetItem(COL_QUESTION_MARK)->SetBackgroundColor(RGB(110,180,200));
	pRowListioning->GetItem(COL_QUESTION_MARK)->Enable(FALSE);

	pRowListioning->AllowSubItems();

	m_gridList.AddRow(pRowListioning);
	for (std::list<CYDQuestionType*>::const_iterator itr = m_lstClean.begin();
		itr != m_lstClean.end(); ++itr)
	{
		CComVariant var;
		hr = (*itr)->GetPropVal(L"TYPE", &var);
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
			hr = InsertItem(*itr, pVault,pRowListioning);
			if(FAILED(hr))
			{
				return hr;
			}
		}
	}

	for (std::list<CYDQuestionType*>::const_iterator itr = m_lstClean.begin();
		itr != m_lstClean.end(); ++itr)
	{
		CComVariant var;
		hr = (*itr)->GetPropVal(L"TYPE", &var);
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
			hr = InsertItem(*itr,pVault, NULL);
			if(FAILED(hr))
			{
				return hr;
			}
		}
	}
	return S_OK;
}

HRESULT CDlgCfgQuestion::InsertItem(CYDQuestionType* pType,
									CYDObjectRef* _pVault,
									CBCGPGridRow* pParentRow)
{
	HRESULT hr = E_FAIL;
	hr = pType->Restore();
	if (FAILED(hr))
	{
		return hr;
	}
	ASSERT(_pVault);
	OBJID idVault = 0;
	hr = _pVault->GetID(&idVault);
	if (FAILED(hr))
	{
		return hr;
	}
	std::list<UINT> lstRelateQuestionsCount;
	hr = pType->GetArticleRelateQuestionsCount(idVault, &lstRelateQuestionsCount);
	if (FAILED(hr))
	{
		return hr;
	}
	std::list<UINT>::const_iterator itrCount = lstRelateQuestionsCount.begin();

	CBCGPGridRow* pRow = m_gridList.CreateRow(m_gridList.GetColumnCount());
	
 	CYDPaperBlockConfigure* pCfg = new CYDPaperBlockConfigure();
	m_lstCfg.push_back(pCfg);
// 	m_pPaper->AddPaperBlockConfigure(pCfg);	
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
	pRow->GetItem(COL_QUESTION_TYPE)->SetValue(varVal);
	pRow->GetItem(COL_QUESTION_TYPE)->SetBackgroundColor(RGB(110,180,200));
	pRow->GetItem(COL_QUESTION_TYPE)->Enable(FALSE);
	//难度
	pRow->GetItem(COL_QUESTION_HARDLEVEL)->SetValue(CComVariant((*itr)));
	pRow->GetItem(COL_QUESTION_HARDLEVEL)->SetBackgroundColor(RGB(110,180,200));
	pRow->GetItem(COL_QUESTION_HARDLEVEL)->Enable(FALSE);
	//每题问题数
	CComVariant varCount(*itrCount);
	pRow->GetItem(COL_QUESTION_EACH_NUM)->SetValue(varCount);
	pRow->GetItem(COL_QUESTION_EACH_NUM)->SetBackgroundColor(RGB(110,180,200));
	pRow->GetItem(COL_QUESTION_EACH_NUM)->Enable(FALSE);
	//题数
	long lNum = 0;
	double fMark = 0.0;
	hr = GetQNum(pType,CYDQuestionRef::TranslateHardLevel(*itr),*itrCount,lNum,fMark);
	if(FAILED(hr))
	{
		return hr;
	}
	pRow->ReplaceItem(COL_QUESTION_NUM, new CBCGPGridItem(long(lNum)));
	pRow->ReplaceItem(COL_QUESTION_MARK, new CBCGPGridItem(fMark));

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
		pChildRow->GetItem(COL_QUESTION_HARDLEVEL)->SetValue(CComVariant((*itr)));
		pChildRow->GetItem(COL_QUESTION_HARDLEVEL)->Enable(FALSE);
		//每题问题数
		CComVariant varCount(*itrCount);
		pChildRow->GetItem(COL_QUESTION_EACH_NUM)->SetValue(varCount);
		pChildRow->GetItem(COL_QUESTION_EACH_NUM)->SetBackgroundColor(RGB(110,180,200));
		pChildRow->GetItem(COL_QUESTION_EACH_NUM)->Enable(FALSE);
		//题数
		long lNum = 0;
		double fMark = 0.0;
		hr = GetQNum(pType,CYDQuestionRef::TranslateHardLevel(*itr),*itrCount,lNum,fMark);
		if(FAILED(hr))
		{
			return hr;
		}
		pChildRow->ReplaceItem(COL_QUESTION_NUM, new CBCGPGridItem(lNum));
		pChildRow->ReplaceItem(COL_QUESTION_MARK, new CBCGPGridItem(fMark));
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
			pChildRow->GetItem(COL_QUESTION_HARDLEVEL)->SetValue(CComVariant((*itr)));
			pChildRow->GetItem(COL_QUESTION_HARDLEVEL)->Enable(FALSE);
			if (bFirst)
			{
				pChildRow->GetItem(COL_QUESTION_HARDLEVEL)->SetBackgroundColor(RGB(110,180,200));
				bFirst = false;
			}
			//每题问题数
			CComVariant varCount(*itrCount);
			pChildRow->GetItem(COL_QUESTION_EACH_NUM)->SetValue(varCount);
			pChildRow->GetItem(COL_QUESTION_EACH_NUM)->SetBackgroundColor(RGB(110,180,200));
			pChildRow->GetItem(COL_QUESTION_EACH_NUM)->Enable(FALSE);
			//题数
			long lNum = 0;
			double fMark = 0.0;
			hr = GetQNum(pType,CYDQuestionRef::TranslateHardLevel(*itr),*itrCount,lNum,fMark);
			if(FAILED(hr))
			{
				return hr;
			}
			pChildRow->ReplaceItem(COL_QUESTION_NUM, new CBCGPGridItem(lNum));
			pChildRow->ReplaceItem(COL_QUESTION_MARK, new CBCGPGridItem(fMark));
			pRow->AddSubItem(pChildRow);
			lLevel = CYDQuestionRef::TranslateHardLevel(*itr);
			pCfg->AddConfigureItem(lLevel, *itrCount, 0, lst);
		}
	}

	return S_OK;
}

HRESULT CDlgCfgQuestion::GetQNum(CYDQuestionType* _pType,
				HARDLEVEL _level,
				UINT      _uEachNum,
				long &_lNum,
				double &_fMark)
{
// 	HRESULT hr = E_FAIL;
// 	_lNum = 0;//默认为0
// 	_fMark = 0.0;
// 	ASSERT(_pType);
// 	OBJID idType = 0;
// 	hr = _pType->GetID(&idType);
// 	if(FAILED(hr))
// 	{
// 		return hr;
// 	}
// 	for(std::list<CQuestionCfgStruct*>::const_iterator itr = m_pSelQCfgMgr->m_lstQuestionStruct.begin();
// 		itr != m_pSelQCfgMgr->m_lstQuestionStruct.end();++itr)
// 	{
// 		if((*itr)->m_QTypeID == idType)
// 		{
// 			for(std::list<CQuestionLevelNum*>::const_iterator itrLevelNum = (*itr)->m_lstLevelNum.begin();
// 				itrLevelNum != (*itr)->m_lstLevelNum.end();++itrLevelNum)
// 			{
// 				if((*itrLevelNum)->m_level == _level && 
// 					(*itrLevelNum)->m_uEachNum == _uEachNum)
// 				{
// 					_lNum = (*itrLevelNum)->m_uNumber;
// 					_fMark = (*itrLevelNum)->m_fMark;
// 					return S_OK;
// 				}
// 			}
// 			break;
// 		}
// 	}
	return S_OK;
}

HRESULT CDlgCfgQuestion::InitQuestionVault()
{
	HRESULT hr = E_FAIL;
	{
		CListAutoClean<CYDObjectRef> clr(m_lstQuestionVault);
	}
	m_cmbQuestionVault.ResetContent();
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	hr = CStaticObjHelper::GetObjRef(DB_QUESTIONVAULT, pDB, &m_lstQuestionVault);
	if(FAILED(hr))
	{
		return hr;
	}
	for(std::list<CYDObjectRef*>::const_iterator itr = m_lstQuestionVault.begin();
		itr != m_lstQuestionVault.end();++itr)
	{
		CString strName;
		hr = (*itr)->GetPropVal(FIELD_YDVAULT_NAME,strName);
		if(FAILED(hr))
		{
			return hr;
		}
		int index = m_cmbQuestionVault.InsertString(0,strName);
		m_cmbQuestionVault.SetItemData(index,(DWORD_PTR)(*itr));
		OBJID id = 0;
		hr =(*itr)->GetID(&id);
		if(FAILED(hr))
		{
			return hr;
		}
		if(id == m_pSelQCfgMgr->m_uQuestionVaultID)
		{
			m_cmbQuestionVault.SetCurSel(index);
		}
	}
	OnCbnSelchangeComboQuestionVault();
	return S_OK;
}

void CDlgCfgQuestion::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_cmbQuestionVault.GetCurSel() == CB_ERR)
	{
		AfxMessageBox(_T("请选择题库！"));
		return;
	}
	HRESULT hr = E_FAIL;
	
	hr = UpdateQuestionCfg();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONEXCLAMATION);
		return ;
	}
	hr = m_pSelQCfgMgr->Save();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONEXCLAMATION);
		return ;
	}
	OnOK();
}

HRESULT CDlgCfgQuestion::UpdateQuestionCfg()
{
// 	HRESULT hr = E_FAIL;
// 	CYDQuestionVault* pVault = (CYDQuestionVault*)m_cmbQuestionVault.GetItemData(m_cmbQuestionVault.GetCurSel());
// 	ASSERT(pVault);
// 	OBJID id = 0;
// 	hr = pVault->GetID(&id);
// 	if(FAILED(hr))
// 	{
// 		return hr;
// 	}
// 	m_pSelQCfgMgr->m_uQuestionVaultID = id;
// 	hr= m_pSelQCfgMgr->Clear();
// 	if(FAILED(hr))
// 	{
// 		return hr;
// 	}
// 	for (int i=0; i<m_gridList.GetRowCount(); ++i)
// 	{
// 		CYDPaperBlockConfigure* pCfg = (CYDPaperBlockConfigure*)m_gridList.GetRow(i)->GetData();
// 		if (pCfg != NULL)
// 		{
// 			_variant_t varVal = m_gridList.GetRow(i)->GetItem(COL_QUESTION_HARDLEVEL)->GetValue();
// 			CString str = CDataHandler::VariantToString(varVal);
// 			long lLevel = CYDQuestionRef::TranslateHardLevel(str);
// 			varVal.Clear();
// 			varVal = m_gridList.GetRow(i)->GetItem(COL_QUESTION_EACH_NUM)->GetValue();
// 			long lRelCount = CDataHandler::VariantToLong(varVal);
// 			varVal.Clear();
// 			varVal = m_gridList.GetRow(i)->GetItem(COL_QUESTION_NUM)->GetValue();
// 			long lCount = CDataHandler::VariantToLong(varVal);
// 			varVal.Clear();
// 			varVal = m_gridList.GetRow(i)->GetItem(COL_QUESTION_MARK)->GetValue();
// 			double fMark = varVal;
// 			pCfg->UpdateConfigureItem(lLevel, lRelCount, lCount,fMark);
// 		}
// 	}
// 	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
// 	ASSERT(pDB);
// 	for(std::list<CYDPaperBlockConfigure*>::const_iterator itr = m_lstCfg.begin();
// 		itr != m_lstCfg.end();++itr)
// 	{
// 	CYDQuestionType QTypeObj(pDB);
// 		hr = QTypeObj.SetID((*itr)->m_QType);
// 		if(FAILED(hr))
// 		{
// 			return hr;
// 		}
// 		CQuestionCfgStruct* pStruct = new CQuestionCfgStruct();
// 		pStruct->m_QTypeID = (*itr)->m_QType;
// 		m_pSelQCfgMgr->m_lstQuestionStruct.push_back(pStruct);
// 		for(std::list<CYDPaperBlockConfigureItem*>::const_iterator itrItem = (*itr)->m_lstConfigureItem.begin();
// 			itrItem != (*itr)->m_lstConfigureItem.end();++itrItem)
// 		{
// 			CQuestionLevelNum* pQLevelNum = new CQuestionLevelNum();
// 			pQLevelNum->m_level = (HARDLEVEL)(*itrItem)->m_iLevel;
// 			pQLevelNum->m_uEachNum = (*itrItem)->m_iRelNum;
// 			pQLevelNum->m_uNumber = (*itrItem)->m_iCount;
// 			pQLevelNum->m_fMark = (*itrItem)->m_fMark;
// 			pStruct->m_lstLevelNum.push_back(pQLevelNum);
// 			
// 		}
// 	}
	return S_OK;
}
void CDlgCfgQuestion::OnCbnSelchangeComboQuestionVault()
{
	// TODO: Add your control notification handler code here
	
	HRESULT hr = FillGrid();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONEXCLAMATION);
		return ;
	}

}
