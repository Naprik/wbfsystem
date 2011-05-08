#include "StdAfx.h"
#include "YDQuestionDlg.h"
#include "ListCtrlOperate.h"
#include "../ObjRef/YDQuestionType.h"
#include "../Base/DataHandler.h"
#include "../ObjRef/YdObjWrapper.h"
#include "../Base/AutoClean.h"
#include "../ObjRef/YDQuestionVault.h"
#include "../ObjRef/YDQuestionRef.h"
#include "../ObjHelper/FactorInfoHelper.h"
#include "../Include/ShowErr.h"

const int cColPropName = 0;//属性名
const int cColPropVal = 1;//属性值

CYDQuestionDlg::CYDQuestionDlg(UINT nIDTemplate,CWnd* _pParent)
:CDialog(nIDTemplate,_pParent)
{
	m_pListOperate = NULL;
	m_index = 0;
	m_pObjWrapper= NULL;
	m_pQVault = NULL;
	m_pQType = NULL;
	m_uType = OP_NEW;
	m_bIsInPaperGenerate = FALSE;
	m_pInPaperGenrateObjWrapper = NULL;
}

CYDQuestionDlg::~CYDQuestionDlg(void)
{
	CListAutoClean<CYdKnowledge> clean1(m_allKPs);
	CListAutoClean<CYdObjWrapper> clean2(m_relatedKPs);
	CListAutoClean<CYDObjectRef> clr2(m_lstFactorInfo);
}

HRESULT CYDQuestionDlg::SetListOperate(CListCtrlOperate* _pListOperate,int _index /*= 0*/)
{
	m_pListOperate = _pListOperate;
	m_index = _index;
	return S_OK;
}


HRESULT CYDQuestionDlg::SetObjWrapper(CYdObjWrapper* _pObjWrapper)
{
	m_pObjWrapper = _pObjWrapper;
	return S_OK;
}

HRESULT CYDQuestionDlg::SetYdQValut(CYDQuestionVault* _pVault)
{
	m_pQVault = _pVault;
	return S_OK;
}

HRESULT CYDQuestionDlg::SetYdQType(CYDQuestionType* _pQType)
{
	m_pQType = _pQType;
	return S_OK;
}

HRESULT CYDQuestionDlg::SetOpType(OPERATION _uType /*= 0*/)
{
	m_uType = _uType;
	return S_OK;
}


HRESULT CYDQuestionDlg::CreateQTypeID(VARIANT* _valTypeID)
{
	HRESULT hr = E_FAIL;
	ASSERT(m_pQType);
	OBJID idType = ID_EMPTY;
	hr = m_pQType->GetID(&idType);
	if(FAILED(hr))
	{
		return hr;
	}
	_valTypeID->vt = VT_I4;
	_valTypeID->lVal = idType;
	return S_OK;
}
HRESULT CYDQuestionDlg::GetQuestionType(QTYPE* pType)
{	
	HRESULT hr = E_FAIL;
	
	CComVariant var;
	if (m_uType == OP_NEW)
	{
		hr = m_pQType->GetPropVal(FIELD_QUESTIONTYPE_TYPE, &var);
		if (FAILED(hr))
		{
			return hr;
		}
	}
	else
	{
		CComVariant varID;
		hr = m_pObjWrapper->m_pObjRef->GetPropVal(L"TYPEID", &varID);
		if (FAILED(hr))
		{
			return hr;
		}
		OBJID idType = CDataHandler::VariantToLong(varID);
		CYDQuestionType qtype(m_pObjWrapper->m_pObjRef->m_pDb);
		qtype.SetID(idType);
		hr = qtype.GetPropVal(FIELD_QUESTIONTYPE_TYPE, &var);
		if (FAILED(hr))
		{
			return hr;
		}
	}
	*pType = (QTYPE)CDataHandler::VariantToLong(var);
	return S_OK;
}

std::list<CYdKnowledge*>& CYDQuestionDlg::GetAllKnowledgPoint()
{
	if (m_allKPs.size() <= 0)
	{
		m_pQVault->GetAllKnowledgePoint(&m_allKPs);
	}
	
	return m_allKPs;
}

std::list<CYdObjWrapper*>& CYDQuestionDlg::GetQuestionRelatedKnowledgePoint()
{
	if (m_relatedKPs.size() <= 0 &&
		m_uType == OP_EDIT)
	{
		CYDQuestionRef* question = dynamic_cast<CYDQuestionRef*>(m_pObjWrapper->m_pObjRef);
		ASSERT(question != NULL);	
		std::list<CYdKnowledge*> lstKPs;
		std::list<CYDLinkRef*> lstLinks;
		question->GetAllRelatedKnowledgePoints(&lstKPs, &lstLinks);
		std::list<CYdKnowledge*>::const_iterator itrKP = lstKPs.begin();
		std::list<CYDLinkRef*>::const_iterator itrLink = lstLinks.begin();
		for (; itrKP != lstKPs.end(); ++itrKP, ++itrLink)
		{
			CYdObjWrapper*  pObjWrapper = new CYdObjWrapper();
			pObjWrapper->m_pObjRef = *itrKP;
			pObjWrapper->m_pLinkRef = *itrLink;
			m_relatedKPs.push_back(pObjWrapper);
		}
	}

	return m_relatedKPs;
}

HRESULT CYDQuestionDlg::CreateIndicatorGridCtrl(UINT _idBK,CBCGPGridCtrl* _pGrid)
{
	HRESULT hr = E_FAIL;
	CRect rectBK;
	GetDlgItem(_idBK)->GetWindowRect(&rectBK);
	// TODO:  Add extra initialization here
	ScreenToClient(&rectBK);
	rectBK.top += 30;
	rectBK.left += 10;
	rectBK.right -= 10;
	rectBK.bottom -= 10;
	if (!_pGrid->Create(WS_CHILD | WS_TABSTOP | WS_VISIBLE, rectBK, this,
		-1))
	{
		return S_FALSE;
	}
	_pGrid->EnableDragHeaderItems(FALSE);
	_pGrid->EnableGroupByBox (FALSE);
	_pGrid->SetWholeRowSel(FALSE);
	_pGrid->EnableHeader (TRUE, 0);

	_pGrid->InsertColumn(cColPropName, L"属性名", 100);
	_pGrid->InsertColumn(cColPropVal, L"属性值", 100);
	return S_OK;
}

HRESULT CYDQuestionDlg::CreateIndicator(CYDObjectRef* _pQuestionRef,CBCGPGridCtrl* _pGrid)
{
	HRESULT hr = E_FAIL;
	{
		CListAutoClean<CYDObjectRef> clr(m_lstFactorInfo);
	}
	ASSERT(m_pQVault);
	ASSERT(m_pQType);
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	CFactorInfoHelper helper;
	hr = helper.GetFactorInfoByVaultQType(pDB,(CYDObjectRef*)m_pQVault,m_pQType,&m_lstFactorInfo);
	if(FAILED(hr))
	{
		return hr;
	}
	for(std::list<CYDObjectRef*>::const_iterator itr = m_lstFactorInfo.begin();
		itr != m_lstFactorInfo.end();++itr)
	{
		CString strFactorName;
		hr = (*itr)->GetPropVal(FIELD_YDFACTORINFOITEM_FACTORNAME,strFactorName);
		if(FAILED(hr))
		{
			return hr;
		}
		CString strFieldName;
		hr = (*itr)->GetPropVal(FIELD_YDFACTORINFOITEM_FIELDNAME,strFieldName);
		if(FAILED(hr))
		{
			return hr;
		}
		CComVariant val;
		hr = _pQuestionRef->GetPropVal(CComBSTR(strFieldName),&val);
			if(FAILED(hr))
		{
			return hr;
		}
		if(helper.IsNumberFieldName(strFieldName))
		{
			//数值型，如果val为空，要将其修改为0,否则在Grid中不能输入值
			long lVal = CDataHandler::VariantToLong(val);
			val = lVal;
		}
		else 
		{
			//字符串型
			CString strVal = CDataHandler::VariantToString(val);
			CDataHandler::StringToVariant(strVal,VT_BSTR,&val);
		}
		CBCGPGridRow* pRow = _pGrid->CreateRow(_pGrid->GetColumnCount());
		//题型
		pRow->GetItem(cColPropName)->SetValue(CComVariant(strFactorName));
		pRow->GetItem(cColPropName)->Enable(FALSE);
		pRow->GetItem(cColPropName)->SetBackgroundColor( RGB(110,180,200));
		pRow->GetItem(cColPropVal)->SetValue(val);

		_pGrid->AddRow(pRow);
	}
	return S_OK;
}

HRESULT CYDQuestionDlg::UpdateIndicator(CYDObjectRef* _pQuestionRef,CBCGPGridCtrl* _pGrid)
{
	HRESULT hr = E_FAIL;
	CFactorInfoHelper helper;
	for(int i = 0; i < _pGrid->GetRowCount();i++)
	{
		CBCGPGridRow* pRow = _pGrid->GetRow(i);
		ASSERT(pRow);
		CComVariant valFactorName = pRow->GetItem(cColPropName)->GetValue();
		CString strFactorName = CDataHandler::VariantToString(valFactorName);
		for(std::list<CYDObjectRef*>::const_iterator itr = m_lstFactorInfo.begin();
		itr != m_lstFactorInfo.end();++itr)
		{
			CString strItrFactorName;
			hr = (*itr)->GetPropVal(FIELD_YDFACTORINFOITEM_FACTORNAME,strItrFactorName);
			if(FAILED(hr))
			{
				return hr;
			}
			if(strItrFactorName.CompareNoCase(strFactorName) == 0)
			{
				CString strFieldName;
				hr = (*itr)->GetPropVal(FIELD_YDFACTORINFOITEM_FIELDNAME,strFieldName);
				if(FAILED(hr))
				{
					return hr;
				}
				CComVariant val  = pRow->GetItem(cColPropVal)->GetValue();
				if(helper.IsNumberFieldName(strFieldName))
				{
					long lVal = CDataHandler::VariantToLong(val);
					val = lVal;
				}
				hr = _pQuestionRef->SetPropVal(CComBSTR(strFieldName),&val);
				if(FAILED(hr))
				{
					return hr;
				}
				break;
			}
		
		}
	}
	return S_OK;
}

BOOL CYDQuestionDlg::ValidateIndicator(CBCGPGridCtrl* _pGrid)
{
	HRESULT hr = E_FAIL;
	CFactorInfoHelper helper;
	for(int i = 0; i < _pGrid->GetRowCount();i++)
	{
		CBCGPGridRow* pRow = _pGrid->GetRow(i);
		ASSERT(pRow);
		CComVariant valFactorName = pRow->GetItem(cColPropName)->GetValue();
		CString strFactorName = CDataHandler::VariantToString(valFactorName);
		for(std::list<CYDObjectRef*>::const_iterator itr = m_lstFactorInfo.begin();
		itr != m_lstFactorInfo.end();++itr)
		{
			CString strItrFactorName;
			hr = (*itr)->GetPropVal(FIELD_YDFACTORINFOITEM_FACTORNAME,strItrFactorName);
			if(FAILED(hr))
			{
				DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
				return FALSE;
			}
			if(strItrFactorName.CompareNoCase(strFactorName) == 0)
			{
				CString strFieldName;
				hr = (*itr)->GetPropVal(FIELD_YDFACTORINFOITEM_FIELDNAME,strFieldName);
				if(FAILED(hr))
				{
					DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
					return FALSE;
				}
				if(helper.IsNumberFieldName(strFieldName))
				{
					//要验证数值型在最大值和最小值之间
					long lMin = 0;
					hr = (*itr)->GetPropVal(FIELD_YDFACTORINFOITEM_MIN,&lMin);
					if(FAILED(hr))
					{
						DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
						return FALSE;
					}
					long lMax = 0;
					hr = (*itr)->GetPropVal(FIELD_YDFACTORINFOITEM_MAX,&lMax);
					if(FAILED(hr))
					{
						DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
						return FALSE;
					}
					CComVariant val  = pRow->GetItem(cColPropVal)->GetValue();
					long lVal = CDataHandler::VariantToLong(val);
					if(lVal < lMin || lVal > lMax)
					{
						CString strMsg;
						strMsg.Format(_T("%s的属性值应该在%d和%d之间!"),strItrFactorName,lMin,lMax);
						AfxMessageBox(strMsg);
						return FALSE;
					}
				}
				break;
			}
		
		}
	}
	return TRUE;
}