// DlgVocabularyInputPreview.cpp : implementation file
//

#include "stdafx.h"
#include "DlgVocabularyInputPreview.h"
#include "afxdialogex.h"
#include "../DBBase/DBTransactionRef.h"
#include "../ObjRef/YDChoiceQuestionRef.h"
#include "../Base/DataHandler.h"
#include "../ObjHelper/FactorInfoHelper.h"
#include "../Base/AutoClean.h"
#include "../UIBase\PdemWait.h"


// CDlgVocabularyInputPreview dialog

const int cColSerialNo = 0;//序号
const int cColCaption = 1;//题目
const int cColOptionName = 2;//选项名
const int cColOption = 3;//选项
const int cColAnswer = 4;//答案
const int cColFactorName = 5;//指标名
const int cColFactor = 6;//指标值

const int cPageNum = 100;//每页显示的行数

IMPLEMENT_DYNAMIC(CDlgVocabularyInputPreview, CDialogEx)

CDlgVocabularyInputPreview::CDlgVocabularyInputPreview(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgVocabularyInputPreview::IDD, pParent)
{
	m_plstVocabularyQuestion = NULL;
	m_pVault = NULL;
	m_pType = NULL;
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
	ON_BN_CLICKED(IDOK, &CDlgVocabularyInputPreview::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_MORE, &CDlgVocabularyInputPreview::OnBnClickedButtonMore)
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
	m_Grid.InsertColumn(cColSerialNo, L"序号", 50);
	m_Grid.InsertColumn(cColCaption, L"题目", 200);
	m_Grid.InsertColumn(cColOptionName, L"选项名", 50);
	m_Grid.InsertColumn(cColOption, L"选项", 80);
	m_Grid.InsertColumn(cColAnswer, L"答案", 80);
	m_Grid.InsertColumn(cColFactorName, L"指标名", 50);
	m_Grid.InsertColumn(cColFactor, L"指标值", 80);


	m_ItrCur = m_plstVocabularyQuestion->begin();
	ASSERT(m_plstVocabularyQuestion);
	m_iIndex = 0;
	OnBnClickedButtonMore();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


HRESULT CDlgVocabularyInputPreview::InsertRowByVocabularyQuestion(CVocabularyQuestion* _pVocabularyQuestion,int _index)
{
	HRESULT hr = E_FAIL;
	CString strSerial;
	strSerial.Format(_T("%d"),_index+1);
	CBCGPGridRow* pRow = m_Grid.CreateRow(m_Grid.GetColumnCount());
	pRow->GetItem(cColSerialNo)->SetValue(CComVariant(strSerial));
	pRow->SetData((DWORD_PTR)_pVocabularyQuestion);
	//插入题目
	pRow->GetItem(cColCaption)->SetValue(CComVariant(_pVocabularyQuestion->m_strCaption));
	pRow->AllowSubItems();
	m_Grid.AddRow(pRow);

	//插入选项
	int index = 0;
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
	//插入答案
	pRow->GetItem(cColAnswer)->SetValue(CComVariant(_pVocabularyQuestion->m_strAnswer));
	//插入指标
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

void CDlgVocabularyInputPreview::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	HRESULT hr = E_FAIL;
	for(int i = 0; i < m_Grid.GetRowCount();i++)
	{
		CBCGPGridRow* pRow = m_Grid.GetRow(i);
		ASSERT(pRow);
		CBCGPGridRow* pParentRow = pRow->GetParent();
		if(pParentRow == NULL)
		{
			//当前是根节点，是一条选择题
			hr = UpdateQuestionByRow(pRow);
			if(FAILED(hr))
			{
				DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
				return;
			}
		}

	}
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	CPdemWait	wait(_T("请稍候，正在导入数据..."),FALSE,m_plstVocabularyQuestion->size());
	wait.BeginWait();
	CDBTransactionRef trans(pDB, TRUE);
	for(auto itr = m_plstVocabularyQuestion->begin();itr != m_plstVocabularyQuestion->end();++itr)
	{
		hr = InsertQuertionByVocabularyQuestion(*itr);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
			return;
		}
		wait.StepIt();
	}
	
	wait.Close();
	trans.Commit();
	CDialogEx::OnOK();
}

HRESULT CDlgVocabularyInputPreview::UpdateQuestionByRow(CBCGPGridRow* _pRootRow)
{
	HRESULT hr = E_FAIL;
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	ASSERT(_pRootRow);
	CVocabularyQuestion* pVocabularyQuestion = (CVocabularyQuestion*)(_pRootRow->GetData());
	ASSERT(pVocabularyQuestion);

	//提干
	CComVariant valTitle = _pRootRow->GetItem(cColCaption)->GetValue();
	pVocabularyQuestion->m_strCaption = CDataHandler::VariantToString(valTitle);
	pVocabularyQuestion->m_lstOption.clear();
	//选项
	{
		CString strOptionName,strOptionVal;
		GetOption(_pRootRow,strOptionName,strOptionVal);
		if(!strOptionVal.IsEmpty())
		{
			pVocabularyQuestion->m_lstOption.push_back(std::make_pair(strOptionName,strOptionVal));
		}
		for(int i = 0; i < _pRootRow->GetSubItemsCount();i++)
		{
			CBCGPGridRow* pChildRow = _pRootRow->GetSubItem(i);
			ASSERT(pChildRow);
			CString strSubOptionName,strSubOptionVal;
			GetOption(pChildRow,strSubOptionName,strSubOptionVal);
			if(!strSubOptionVal.IsEmpty())
			{
				pVocabularyQuestion->m_lstOption.push_back(std::make_pair(strSubOptionName,strSubOptionVal));
			}
		}
	}
	CComVariant valAnswer = _pRootRow->GetItem(cColAnswer)->GetValue();
	pVocabularyQuestion->m_strAnswer = CDataHandler::VariantToString(valAnswer);
	//设置指标
	pVocabularyQuestion->m_lstFactor.clear();
	{
		CString strFactorName,strFactorVal;
		GetFactor(_pRootRow,strFactorName,strFactorVal);
		if(!strFactorName.IsEmpty())
		{
			pVocabularyQuestion->m_lstFactor.push_back(std::make_pair(strFactorName,strFactorVal));
		}
		for(int i = 0; i < _pRootRow->GetSubItemsCount();i++)
		{
			CBCGPGridRow* pChildRow = _pRootRow->GetSubItem(i);
			ASSERT(pChildRow);
			CString strSubFactorName,strSubFactorVal;
			GetFactor(pChildRow,strSubFactorName,strSubFactorVal);
			if(!strSubFactorName.IsEmpty())
			{
				pVocabularyQuestion->m_lstFactor.push_back(std::make_pair(strFactorName,strFactorVal));
			}
		}
	}
	return S_OK;
}

HRESULT CDlgVocabularyInputPreview::InsertQuertionByVocabularyQuestion(CVocabularyQuestion* _pVocabularyQuestion)
{
	HRESULT hr = E_FAIL;
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	ASSERT(_pVocabularyQuestion);
	CYDChoiceQuestionRef* pRef = new CYDChoiceQuestionRef(pDB);
	CPtrAutoClean<CYDChoiceQuestionRef> clr(pRef);
	//提干
	CComVariant valTitle = _pVocabularyQuestion->m_strCaption;
	hr = pRef->SetPropVal(FIELD_CHOICEQUESTION_TITLE,&valTitle);
	if(FAILED(hr))
	{
		return hr;
	}
	std::list<CString> lstChoices;
	//选项
	{
		for(auto itr = _pVocabularyQuestion->m_lstOption.begin();itr != _pVocabularyQuestion->m_lstOption.end();++itr)
		{
			lstChoices.push_back((*itr).second);
		}
	}
	hr = pRef->SetOptionList(&lstChoices);
	if(FAILED(hr))
	{
		return hr;
	}
	CComVariant valAnswer = _pVocabularyQuestion->m_strAnswer;
	hr = pRef->SetPropVal(FIELD_CHOICEQUESTION_ANSWER,&valAnswer);
	if(FAILED(hr))
	{
		return hr;
	}
	//设置为选择题
	CComVariant valTypeID(QTYPE_VOCABULARY);
	hr = pRef->SetPropVal(FIELD_CHOICEQUESTION_TYPEID,&valTypeID);
	if(FAILED(hr))
	{
		return hr;
	}
	CComVariant valCreator = DEFAULT_CREATOR;
	hr = pRef->SetPropVal(FIELD_CHOICEQUESTION_CREATOR,&valCreator);
	if(FAILED(hr))
	{
		return hr;
	}
	COleDateTime dt = COleDateTime::GetCurrentTime();
	CComVariant valCrateDate;
	valCrateDate.vt = VT_DATE;
	valCrateDate.date = dt;
	hr = pRef->SetPropVal(FIELD_CHOICEQUESTION_CREATEDATE,&valCrateDate);
	if(FAILED(hr))
	{
		return hr;
	}
	// 	CComVariant valHardLevel;
	// 	valHardLevel.vt = VT_I4;
	// 	valHardLevel.lVal = m_iHardLevel == 0 ? EASY : HARD;
	// 	hr = _pRef->SetPropVal(L"HARDLEVEL",&valHardLevel);
	// 	if(FAILED(hr))
	// 	{
	// 		return hr;
	// 	}
	//设置指标
	CFactorInfoHelper helper;
	std::list<CYDObjectRef*> lstFactorInfo;
	hr = helper.GetFactorInfoByVaultQType(pDB,m_pVault,m_pType,&lstFactorInfo);
	if(FAILED(hr))
	{
		return hr;
	}
	CListAutoClean<CYDObjectRef> clr2(lstFactorInfo);
	{
		for(auto itr = _pVocabularyQuestion->m_lstFactor.begin();itr != _pVocabularyQuestion->m_lstFactor.end();++itr)
		{
			CString strFactorName,strFactorVal;
			strFactorName = (*itr).first;
			strFactorVal = (*itr).second;
			if(!strFactorName.IsEmpty())
			{
				hr = SetFactorProp(&lstFactorInfo,strFactorName,strFactorVal,pRef,&helper);
				if(FAILED(hr))
				{
					return hr;
				}
			}
		}
	}
	hr = pRef->Save();
	if(FAILED(hr))
	{
		return hr;
	}
	CYDLinkRef* pLinkRef = new CYDLinkRef(pDB,DB_VAULTQUESTION);
	CPtrAutoClean<CYDLinkRef> clr3(pLinkRef);
	hr = pLinkRef->AddPropDef(_T("ID_TYPE"),VT_UINT);
	if(FAILED(hr))
	{
		return hr ;
	}
	OBJID idType = 0;
	hr= m_pType->GetID(&idType);
	if(FAILED(hr))
	{
		return hr ;
	}
	CComVariant valIDType(idType);;
	hr = pLinkRef->SetPropVal(L"ID_TYPE",&valTypeID);
	if(FAILED(hr))
	{
		return hr;
	}
	hr = pLinkRef->PutObjRef(m_pVault,pRef);
	if(FAILED(hr))
	{
		return hr;
	}
	hr = pLinkRef->Save();
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

HRESULT CDlgVocabularyInputPreview::SetFactorProp(std::list<CYDObjectRef*> *_plstFactorInfo,
					CString _strFactorName,CString _strFactor,CYDObjectRef* _pQRef,CFactorInfoHelper* _phelper)
{
	HRESULT hr = E_FAIL;
	ASSERT(_plstFactorInfo);
	for(std::list<CYDObjectRef*>::const_iterator itr = _plstFactorInfo->begin();
		itr != _plstFactorInfo->end();++itr)
	{
		CString strItrFactorName;
		hr = (*itr)->GetPropVal(FIELD_YDFACTORINFOITEM_FACTORNAME,strItrFactorName);
		if(FAILED(hr))
		{
			return hr;
		}
		if(strItrFactorName.CompareNoCase(_strFactorName) == 0)
		{
			CString strFieldName;
			hr = (*itr)->GetPropVal(FIELD_YDFACTORINFOITEM_FIELDNAME,strFieldName);
			if(FAILED(hr))
			{
				return hr;
			}
			CComVariant val(_strFactor);
			if(_phelper->IsNumberFieldName(strFieldName))
			{
				long lVal = CDataHandler::VariantToLong(val);
				val = lVal;
			}
			hr = _pQRef->SetPropVal(CComBSTR(strFieldName),&val);
			if(FAILED(hr))
			{
				return hr;
			}
			break;
		}
		
		
	}
	return S_OK;
}

void CDlgVocabularyInputPreview::GetOption(CBCGPGridRow* _pRow,CString &_strOptionName,CString &_strOption)
{
	CComVariant valChoiceName = _pRow->GetItem(cColOptionName)->GetValue();
	_strOptionName = CDataHandler::VariantToString(valChoiceName);
	CComVariant valChoice = _pRow->GetItem(cColOption)->GetValue();
	_strOption = CDataHandler::VariantToString(valChoice);
}

void CDlgVocabularyInputPreview::GetFactor(CBCGPGridRow* _pRow,CString &_strFactorName,CString &_strFactor)
{
	CComVariant valFactorName = _pRow->GetItem(cColFactorName)->GetValue();
	_strFactorName = CDataHandler::VariantToString(valFactorName);
	CComVariant valFactor = _pRow->GetItem(cColFactor)->GetValue();
	_strFactor = CDataHandler::VariantToString(valFactor);
}

void CDlgVocabularyInputPreview::OnBnClickedButtonMore()
{
	// TODO: Add your control notification handler code here
	CPdemWait	wait(_T("请稍候，正在生成数据..."),FALSE,cPageNum);
	wait.BeginWait();
	m_Grid.LockWindowUpdate();
	m_Grid.SetRedraw(FALSE);
	int index = 0;
	for(;m_ItrCur != m_plstVocabularyQuestion->end() && index < cPageNum;++m_ItrCur,++m_iIndex,++index)
	{
		InsertRowByVocabularyQuestion(*m_ItrCur,m_iIndex);
		wait.StepIt();
	}
	wait.Close();
	m_Grid.UnlockWindowUpdate();
	m_Grid.SetRedraw(TRUE);
	if(m_ItrCur == m_plstVocabularyQuestion->end())
	{
		GetDlgItem(IDC_BUTTON_MORE)->EnableWindow(FALSE);
	}
}
