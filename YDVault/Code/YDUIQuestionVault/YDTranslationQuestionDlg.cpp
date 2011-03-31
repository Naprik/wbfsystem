// YDTranslationQuestionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "YDTranslationQuestionDlg.h"
#include "../ObjRef/YDChoiceQuestionRef.h"
#include "../Include/ShowErr.h"
#include "../ObjRef/YdObjWrapper.h"
#include "../DBBase/DBTransactionRef.h"
#include "../YDFormUIBase/ListCtrlOperate.h"
#include "../Base/DataHandler.h"
#include "../base/AutoClean.h"
#include "../ObjRef/YDQuestionType.h"

// CYDTranslationQuestionDlg dialog

IMPLEMENT_DYNAMIC(CYDTranslationQuestionDlg, CYDQuestionDlg)

CYDTranslationQuestionDlg::CYDTranslationQuestionDlg(CWnd* pParent /*=NULL*/)
	: CYDQuestionDlg(CYDTranslationQuestionDlg::IDD, pParent)
	, m_strArticle(_T(""))
	, m_strAnswer(_T(""))
	, m_iHardLevel(0)
	, m_strCreateDate(_T(""))
	, m_strCreator(_T(""))
{

}

CYDTranslationQuestionDlg::~CYDTranslationQuestionDlg()
{
	CListAutoClean<CYdObjWrapper> clean1(m_lstAddKPs);
}

void CYDTranslationQuestionDlg::DoDataExchange(CDataExchange* pDX)
{
	CYDQuestionDlg::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_RICHEDIT_TITLE, m_strArticle);
	DDX_Text(pDX, IDC_RICHEDIT_ANSWER, m_strAnswer);
	DDX_CBIndex(pDX, IDC_COMBO_AQ_HARDLEVEL, m_iHardLevel);
	DDX_Text(pDX, IDC_EDIT_AQ_CREATE_DATE, m_strCreateDate);
	DDX_Text(pDX, IDC_EDIT_AQ_CREATOR, m_strCreator);
	DDX_Control(pDX, IDC_LIST_KPALL, m_listCtrlKpAll);
	DDX_Control(pDX, IDC_LIST_KPRELATED, m_listCtrlKpRelated);
}


BEGIN_MESSAGE_MAP(CYDTranslationQuestionDlg, CYDQuestionDlg)
	ON_BN_CLICKED(IDC_BUTTON_CQ_ADD, &CYDTranslationQuestionDlg::OnBnClickedButtonCqAdd)
	ON_BN_CLICKED(IDC_BUTTON_CQ_MODIFY, &CYDTranslationQuestionDlg::OnBnClickedButtonCqModify)
	ON_BN_CLICKED(IDC_BUTTON_ADDKP, &CYDTranslationQuestionDlg::OnBnClickedButtonAddkp)
	ON_BN_CLICKED(IDC_BUTTON_REMOVEKP, &CYDTranslationQuestionDlg::OnBnClickedButtonRemovekp)
END_MESSAGE_MAP()


// CYDTranslationQuestionDlg message handlers

void CYDTranslationQuestionDlg::OnBnClickedButtonCqAdd()
{
	// TODO: Add your control notification handler code here
	CListAutoClean<CYdObjWrapper> clean1(m_lstAddKPs);
	UpdateData();
	if(!ValidateData(OP_NEW))
	{
		return;
	}
	HRESULT hr = E_FAIL;
	//保存一个翻译题
	CYDChoiceQuestionRef* pRef = NULL;
	hr = CreateQuestionRef(pRef);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	CYDLinkRef* pLinkRef = NULL;
	hr = CreateLinkRef(pLinkRef);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	ASSERT(m_pQVault);
	hr = pLinkRef->PutObjRef((CYDObjectRef*)m_pQVault,pRef);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	CYdObjWrapper* pObjWrapper = new CYdObjWrapper();
	pObjWrapper->m_pObjRef = pRef;
	pObjWrapper->m_pLinkRef = pLinkRef;
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	CDBTransactionRef TransRef(pDB,TRUE);
	hr = pObjWrapper->Save();
	if(FAILED(hr))
	{
		TransRef.Rollback();
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	hr = PersistKnowledge(pObjWrapper->m_pObjRef);
	if(FAILED(hr))
	{
		delete pObjWrapper;
		pObjWrapper = NULL;
		TransRef.Rollback();
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	hr = TransRef.Commit();
	if(FAILED(hr))
	{
		delete pObjWrapper;
		pObjWrapper = NULL;
		TransRef.Rollback();
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	if(m_bIsInPaperGenerate)
	{
		m_pInPaperGenrateObjWrapper = pObjWrapper;
		OnOK();
		return ;
	}
	if (m_pListOperate)
	{
		hr = m_pListOperate->AddItem(pObjWrapper);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}

	}
	//清空控件中的数据
	hr = UpdateQuestionArea();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	UpdateData(FALSE);

	AfxMessageBox(L"新建成功！继续新增题目或完成题目录入");
}

void CYDTranslationQuestionDlg::OnBnClickedButtonCqModify()
{
	// TODO: Add your control notification handler code here
	CListAutoClean<CYdObjWrapper> clean1(m_lstAddKPs);
	UpdateData();
	if(!ValidateData(OP_EDIT))
	{
		return;
	}
	ASSERT(m_pObjWrapper);
	HRESULT hr = E_FAIL;
	ASSERT(m_pObjWrapper->m_pObjRef->IsKindOf(RUNTIME_CLASS(CYDChoiceQuestionRef)));
	CYDChoiceQuestionRef* pRef = (CYDChoiceQuestionRef*)m_pObjWrapper->m_pObjRef;
	hr = UpdateQuestionRef(pRef);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	CDBTransactionRef TransRef(pDB,TRUE);
	hr = m_pObjWrapper->Update();
	if(FAILED(hr))
	{
		TransRef.Rollback();
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	hr = PersistKnowledge(m_pObjWrapper->m_pObjRef);
	if(FAILED(hr))
	{
		TransRef.Rollback();
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	hr = TransRef.Commit();
	if(FAILED(hr))
	{
		TransRef.Rollback();
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	hr = m_pListOperate->ModifyItem(m_index,m_pObjWrapper);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	OnOK();
}

BOOL CYDTranslationQuestionDlg::ValidateData(OPERATION op)
{
	if(m_strArticle.IsEmpty())
	{
		AfxMessageBox(_T("题干不能为空！"),MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	if(m_strAnswer.IsEmpty())
	{
		AfxMessageBox(_T("答案不能为空！"),MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	return TRUE;
}

HRESULT CYDTranslationQuestionDlg::CreateQuestionRef(CYDChoiceQuestionRef* &_pRef)
{
	HRESULT hr = E_FAIL;
	ASSERT(_pRef == NULL);
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	_pRef = new CYDChoiceQuestionRef(pDB);
	hr = UpdateQuestionRef(_pRef);
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

HRESULT CYDTranslationQuestionDlg::CreateLinkRef(CYDLinkRef* &_pLinkRef)
{
	HRESULT hr = E_FAIL;
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	_pLinkRef = new CYDLinkRef(pDB,DB_VAULTQUESTION);
	hr = _pLinkRef->AddPropDef(_T("ID_TYPE"),VT_UINT);
	if(FAILED(hr))
	{
		return hr ;
	}
	CComVariant valTypeID;
	hr = CreateQTypeID(&valTypeID);
	if(FAILED(hr))
	{
		return hr;
	}
	hr = _pLinkRef->SetPropVal(L"ID_TYPE",&valTypeID);
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

HRESULT CYDTranslationQuestionDlg::UpdateQuestionArea()
{
	HRESULT hr = E_FAIL;
	if (m_uType == OP_NEW)
	{
		m_strArticle =_T("");
		m_strAnswer = _T("");
		m_iHardLevel = 0;
		CComVariant varCreateDate;
		COleDateTime dt = COleDateTime::GetCurrentTime();
		varCreateDate.vt = VT_DATE;
		varCreateDate.date = dt;
		m_strCreateDate = CDataHandler::VariantToString(varCreateDate);
		m_strCreator = DEFAULT_CREATOR;
		while(m_listCtrlKpRelated.GetItemCount() > 0)
		{
			m_listCtrlKpRelated.DeleteItem(0);
		}
	}
	else
	{
		ASSERT(m_pObjWrapper->m_pObjRef->IsKindOf(RUNTIME_CLASS(CYDChoiceQuestionRef)));
		CYDChoiceQuestionRef* pRef = (CYDChoiceQuestionRef*)m_pObjWrapper->m_pObjRef;
		CComVariant var;
		HRESULT hr = pRef->GetPropVal(L"TITLE", &var);
		if (FAILED(hr))
		{
			return hr;
		}
		m_strArticle  = CDataHandler::VariantToString(var);
		var.Clear();
		hr = pRef->GetPropVal(L"HARDLEVEL", &var);
		if (FAILED(hr))
		{
			return hr;
		}
		int iLevel = CDataHandler::VariantToLong(var);
		if (iLevel == EASY)
		{
			m_iHardLevel = 0 ;//简单
		}
		else if(iLevel == HARD)
		{
			m_iHardLevel = 1;//困难
		}
		else
		{
			ASSERT(FALSE);
		}
		var.Clear();

	
		hr = pRef->GetPropVal(L"ANSWER", &var);
		if (FAILED(hr))
		{
			return hr;
		}
		m_strAnswer =  CDataHandler::VariantToString(var);
		var.Clear();
		hr = pRef->GetPropVal(L"CREATOR", &var);
		if (FAILED(hr))
		{
			return hr;
		}
		m_strCreator = CDataHandler::VariantToString(var);
		var.Clear();
		hr = pRef->GetPropVal(L"CREATEDATE", &var);
		if (FAILED(hr))
		{
			return hr;
		}
		m_strCreateDate = CDataHandler::VariantToString(var);
	}

	UpdateData(FALSE);
	return S_OK;
}

HRESULT CYDTranslationQuestionDlg::EnableQuestionArea(BOOL bEnable)
{
	HRESULT hr = E_FAIL;
	((CEdit*)(GetDlgItem(IDC_RICHEDIT_TITLE)))->SetReadOnly(!bEnable);
	((CEdit*)(GetDlgItem(IDC_RICHEDIT_ANSWER)))->SetReadOnly(!bEnable);
	GetDlgItem(IDC_COMBO_AQ_HARDLEVEL)->EnableWindow(bEnable);
	return S_OK;
}

HRESULT CYDTranslationQuestionDlg::UpdateQuestionRef(CYDChoiceQuestionRef* _pRef)
{
	HRESULT hr = E_FAIL;
	CComVariant valArticle(m_strArticle);
	hr = _pRef->SetPropVal(L"TITLE",&valArticle);
	if(FAILED(hr))
	{
		return hr;
	}
	CComVariant valAnswer(m_strAnswer);
	hr = _pRef->SetPropVal(L"ANSWER",&valAnswer);
	if(FAILED(hr))
	{
		return hr;
	}
	if(m_uType == OP_NEW)
	{
		//只有在新建的时候，才要设置试题的类型
		CComVariant valTypeID;
		hr = CreateQTypeID(&valTypeID);
		if(FAILED(hr))
		{
			return hr;
		}
		hr = _pRef->SetPropVal(L"TYPEID",&valTypeID);
		if(FAILED(hr))
		{
			return hr;
		}
		CComVariant valCreator;
		hr = CDataHandler::StringToVariant(m_strCreator,VT_BSTR,&valCreator);
		if(FAILED(hr))
		{
			return hr;
		}
		hr = _pRef->SetPropVal(L"CREATOR",&valCreator);
		if(FAILED(hr))
		{
			return hr;
		}
		CComVariant valCrateDate;
		hr = CDataHandler::StringToVariant(m_strCreateDate,VT_DATE,&valCrateDate);
		if(FAILED(hr))
		{
			return hr;
		}
		hr = _pRef->SetPropVal(L"CREATEDATE",&valCrateDate);
		if(FAILED(hr))
		{
			return hr;
		}
	}
	CComVariant valHardLevel;
	valHardLevel.vt = VT_I4;
	valHardLevel.lVal = m_iHardLevel == 0 ? EASY : HARD;
	hr = _pRef->SetPropVal(L"HARDLEVEL",&valHardLevel);
	if(FAILED(hr))
	{
		return hr;
	}

	return S_OK;
}
BOOL CYDTranslationQuestionDlg::OnInitDialog()
{
	CYDQuestionDlg::OnInitDialog();

	// TODO:  Add extra initialization here
	HRESULT hr = E_FAIL;
	if(m_uType == OP_NEW)
	{
		//新增
		GetDlgItem(IDC_BUTTON_CQ_ADD)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_CQ_MODIFY)->EnableWindow(FALSE);
		EnableQuestionArea(TRUE);
	}
	else if(m_uType == OP_EDIT)
	{
		//修改
		GetDlgItem(IDC_BUTTON_CQ_ADD)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CQ_MODIFY)->EnableWindow(TRUE);

		EnableQuestionArea(TRUE);
	}
	else if (m_uType = OP_VIEW)
	{
		//查看
		GetDlgItem(IDC_BUTTON_CQ_ADD)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CQ_MODIFY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ADDKP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_REMOVEKP)->EnableWindow(FALSE);
		EnableQuestionArea(FALSE);
	}
	hr = UpdateQuestionArea();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	hr = InitKnowledge();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	CString strTitle;
	m_pQType->GetLabel(&strTitle);
	SetWindowText(strTitle);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CYDTranslationQuestionDlg::IfNewKPRemoveFromCache(CYdObjWrapper* pObjWrapper)
{
	std::list<CYdObjWrapper*>::const_iterator itr = m_lstAddKPs.begin();
	for (; itr != m_lstAddKPs.end(); ++itr)
	{
		if ((*itr)->m_pObjRef->IsEqual(pObjWrapper->m_pObjRef))
		{
			delete (*itr);
			m_lstAddKPs.erase(itr);
			return TRUE;
		}
	}
	return FALSE;
}

HRESULT CYDTranslationQuestionDlg::InitKnowledge()
{
	HRESULT hr = E_FAIL;
	CRect rectKP;
	m_listCtrlKpAll.GetWindowRect(rectKP);
	m_listCtrlKpAll.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_listCtrlKpAll.InsertColumn(0,_T("序号"),LVCFMT_LEFT, rectKP.Width()/5 -1);
	m_listCtrlKpAll.InsertColumn(1,_T("名称"),LVCFMT_LEFT, rectKP.Width()*4/5 -1);

	m_listCtrlKpRelated.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_listCtrlKpRelated.InsertColumn(0,_T("名称"),LVCFMT_LEFT, rectKP.Width() -1);

	int iCount = 1;
	for(std::list<CYdKnowledge*>::const_iterator itr = GetAllKnowledgPoint().begin();
		itr != GetAllKnowledgPoint().end();++itr,++iCount)
	{
		CString strIndex;
		strIndex.Format(_T("%d"),iCount);
		int index = m_listCtrlKpAll.InsertItem(iCount,strIndex);
		CString strLabel;
		hr = (*itr)->GetLabel(&strLabel);
		if(FAILED(hr))
		{
			return hr;
		}
		m_listCtrlKpAll.SetItemText(index,1,strLabel);
		m_listCtrlKpAll.SetItemData(index,(DWORD_PTR)(*itr));
	}
	iCount = 1;
	if (m_uType == OP_EDIT)
	{
		for(std::list<CYdObjWrapper*>::const_iterator itr = GetQuestionRelatedKnowledgePoint().begin();
			itr != GetQuestionRelatedKnowledgePoint().end();++itr,++iCount)
		{

			CString strLabel;
			hr = (*itr)->m_pObjRef->GetLabel(&strLabel);
			if(FAILED(hr))
			{
				return hr;
			}
			int index = m_listCtrlKpRelated.InsertItem(iCount,strLabel);
			m_listCtrlKpRelated.SetItemData(index,(DWORD_PTR)(*itr));
		}
	}
	return S_OK;
}
void CYDTranslationQuestionDlg::OnBnClickedButtonAddkp()
{
	// TODO: Add your control notification handler code here
	HRESULT hr = E_FAIL;
	int nCount = m_listCtrlKpAll.GetSelectedCount();
	if(nCount <= 0)
	{
		AfxMessageBox(_T("请在左边列表中选择一条知识点增加！"),MB_OK|MB_ICONWARNING);
		return;
	}
	POSITION pos = m_listCtrlKpAll.GetFirstSelectedItemPosition();
	ASSERT(pos);

	while (pos)
	{
		int nItem = m_listCtrlKpAll.GetNextSelectedItem(pos);
		CYdKnowledge* pKnowledge = (CYdKnowledge*)m_listCtrlKpAll.GetItemData(nItem);
		ASSERT(pKnowledge);
		//判断是否在右边列表中
		for(int i =0 ;i < m_listCtrlKpRelated.GetItemCount();++i)
		{
			CYdObjWrapper* pObjWrapper = (CYdObjWrapper*)m_listCtrlKpRelated.GetItemData(i);
			ASSERT(pObjWrapper);
			if(pObjWrapper->m_pObjRef->IsEqual(pKnowledge))
			{
				CString strLabel;
				pObjWrapper->m_pObjRef->GetLabel(&strLabel);
				CString strFormat = L"该知识点:%s已经被关联!";
				CString strInfo;
				strInfo.Format(strFormat, strLabel);
				AfxMessageBox(strInfo);
				return;
			}
		}
		int iCount = m_listCtrlKpRelated.GetItemCount();

		CString strLabel;
		hr = pKnowledge->GetLabel(&strLabel);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		}
		int index = m_listCtrlKpRelated.InsertItem(iCount,strLabel);
		CYdObjWrapper* pData = new CYdObjWrapper;
		m_lstAddKPs.push_back(pData);
		CYdKnowledge* pObjRef = new CYdKnowledge(pKnowledge->m_pDb);
		OBJID idObj;
		pKnowledge->GetID(&idObj);
		pObjRef->SetID(idObj);
		pData->m_pObjRef = pObjRef;
		CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
		ASSERT(pDB);
		CYDLinkRef* pLink = new CYDLinkRef(pDB, DB_QUESTIONKPOINT);
		pData->m_pLinkRef = pLink;
		hr = pLink->AddPropDef(_T("ID_TYPE"),VT_UINT);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		}
		CComVariant valTypeID;
		hr = CreateQTypeID(&valTypeID);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		}
		hr = pLink->SetPropVal(L"ID_TYPE",&valTypeID);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);;
		}
		pData->m_state = S_NEED_SAVE_LINK;
		m_listCtrlKpRelated.SetItemData(index,DWORD_PTR(pData));
	}
}

void CYDTranslationQuestionDlg::OnBnClickedButtonRemovekp()
{
	// TODO: Add your control notification handler code here
	int nCount = m_listCtrlKpRelated.GetSelectedCount();
	if(nCount <= 0)
	{
		AfxMessageBox(_T("请在右边列表中选择一条知识点删除！"),MB_OK|MB_ICONWARNING);
		return;
	}
	POSITION pos = m_listCtrlKpRelated.GetFirstSelectedItemPosition();
	ASSERT(pos);
	std::list<int> lstDelItem;
	while (pos)
	{
		int nItem = m_listCtrlKpRelated.GetNextSelectedItem(pos);
		lstDelItem.push_front(nItem);
		CYdObjWrapper* pRelatedData = (CYdObjWrapper*)m_listCtrlKpRelated.GetItemData(nItem);
		ASSERT(pRelatedData != NULL);
		if (!IfNewKPRemoveFromCache(pRelatedData))
		{
			pRelatedData->m_state = S_NEED_DELETE_LINK;//需要解除关系
			m_lstDeleteKPs.push_back(pRelatedData);
		}
	}

	for (std::list<int>::const_iterator itr = lstDelItem.begin();
		itr != lstDelItem.end(); ++itr)
	{
		m_listCtrlKpRelated.DeleteItem(*itr);
	}
}

HRESULT CYDTranslationQuestionDlg::PersistKnowledge(CYDObjectRef* _pParentRef)
{
	HRESULT hr = E_FAIL;
	std::list<CYdObjWrapper*>::iterator itr = m_lstDeleteKPs.begin();
	for (; itr != m_lstDeleteKPs.end(); ++itr)
	{
		hr = (*itr)->Persist();
		if (FAILED(hr))
		{
			return hr;
		}
	}
	itr = m_lstAddKPs.begin();
	for (; itr != m_lstAddKPs.end(); ++itr)
	{
		hr = (*itr)->m_pLinkRef->PutObjRef(_pParentRef, (*itr)->m_pObjRef);
		if (FAILED(hr))
		{
			return hr;
		}
		hr = (*itr)->Persist();
		if (FAILED(hr))
		{
			return hr;
		}
	}
	return S_OK;
}
INT_PTR CYDTranslationQuestionDlg::DoModal()
{
	// TODO: Add your specialized code here and/or call the base class
	CExtDllState state;
	return CYDQuestionDlg::DoModal();
}
