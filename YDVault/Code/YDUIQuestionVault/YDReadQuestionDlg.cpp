// YDReadQuestionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "YDReadQuestionDlg.h"
#include "YDChoiceQuestionDlg.h"
#include "../DBBase/DBTransactionRef.h"
#include "../ObjRef/YdObjWrapper.h"
#include "../Include/ShowErr.h"
#include "../YDFormUIBase/ListCtrlOperate.h"
#include "../Base/AutoClean.h"
#include "../base/DataHandler.h"
#include "../ObjRef/YDObjectRef.h"
#include "../ObjRef/YDArticleQuestionRef.h"
#include "../ObjRef/YDLinkRef.h"
#include "../ObjRef/YDMediaRef.h"
#include "../ObjRef/YDQuestionType.h"
#include "../FtpBase/FtpRef.h"
// CYDReadQuestionDlg dialog

IMPLEMENT_DYNAMIC(CYDReadQuestionDlg, CYDQuestionDlg)

CYDReadQuestionDlg::CYDReadQuestionDlg(CWnd* pParent /*=NULL*/)
	: CYDQuestionDlg(CYDReadQuestionDlg::IDD, pParent)
	, m_strArticle(_T(""))
	, m_iHardLevel(0)
	, m_strCreator(_T(""))
	, m_strCreateDate(_T(""))
	, m_strMediaFile(_T(""))
{
	m_pCQListOperate = NULL;
	m_bModifyMedia = FALSE;
}

CYDReadQuestionDlg::~CYDReadQuestionDlg()
{
	CPtrAutoClean<CListCtrlOperate> clr(m_pCQListOperate);
	CListAutoClean<CYdObjWrapper> clean1(m_lstAddKPs);
	CListAutoClean<CYdObjWrapper> clr2(m_lstMedia);
}

void CYDReadQuestionDlg::DoDataExchange(CDataExchange* pDX)
{
	CYDQuestionDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CHOICE_QUESTION, m_lstChoiceQuestion);
	DDX_Text(pDX, IDC_RICHEDIT_AC_ARTICLE, m_strArticle);
	DDX_CBIndex(pDX, IDC_COMBO_AQ_HARDLEVEL, m_iHardLevel);
	DDX_Text(pDX, IDC_EDIT_AQ_CREATOR, m_strCreator);
	DDX_Text(pDX, IDC_EDIT_AQ_CREATE_DATE, m_strCreateDate);
	DDX_Control(pDX, IDC_LIST_CQ_KPALL2, m_listCtrlKpAll);
	DDX_Control(pDX, IDC_LIST_CQ_KPRELATED2, m_listCtrlKpRelated);
	DDX_Text(pDX, IDC_EDIT_FILE, m_strMediaFile);
}


BEGIN_MESSAGE_MAP(CYDReadQuestionDlg, CYDQuestionDlg)
	ON_BN_CLICKED(IDC_BUTTON_NEW_QUESTION, &CYDReadQuestionDlg::OnBnClickedButtonNewQuestion)
	ON_BN_CLICKED(IDC_BUTTON_CQ_ADD, &CYDReadQuestionDlg::OnBnClickedButtonCqAdd)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY_QUESTION, &CYDReadQuestionDlg::OnBnClickedButtonModifyQuestion)
	ON_BN_CLICKED(IDC_BUTTON_DEL_QUESTION, &CYDReadQuestionDlg::OnBnClickedButtonDelQuestion)
	ON_BN_CLICKED(IDC_BUTTON_CQ_MODIFY, &CYDReadQuestionDlg::OnBnClickedButtonCqModify)
	ON_BN_CLICKED(IDC_BUTTON_ADDKP, &CYDReadQuestionDlg::OnBnClickedButtonAddkp)
	ON_BN_CLICKED(IDC_BUTTON_REMOVEKP, &CYDReadQuestionDlg::OnBnClickedButtonRemovekp)
	ON_BN_CLICKED(IDC_BUTTON_SEL_FILE, &CYDReadQuestionDlg::OnBnClickedButtonSelFile)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CHOICE_QUESTION, &CYDReadQuestionDlg::OnNMDblclkListQuestion)
END_MESSAGE_MAP()


// CYDReadQuestionDlg message handlers

void CYDReadQuestionDlg::OnBnClickedButtonNewQuestion()
{
	// TODO: Add your control notification handler code here
	CYDChoiceQuestionDlg dlg;
	dlg.SetYdQType(m_pQType);
	dlg.SetOpType(OP_NEW);
	dlg.SetListOperate(m_pCQListOperate);
	dlg.DoModal();
}

BOOL CYDReadQuestionDlg::OnInitDialog()
{
	CYDQuestionDlg::OnInitDialog();

	// TODO:  Add extra initialization here
	m_lstChoiceQuestion.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	CRect lstQLocate;
	m_lstChoiceQuestion.GetWindowRect(lstQLocate);
	m_lstChoiceQuestion.InsertColumn(0,_T("序号"),LVCFMT_LEFT, lstQLocate.Width()/7 -1 );
	m_lstChoiceQuestion.InsertColumn(1,_T("提干"),LVCFMT_LEFT, lstQLocate.Width()*6/7 - 1);
	m_pCQListOperate = new CListCtrlOperate(&m_lstChoiceQuestion);
	
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
		GetDlgItem(IDC_BUTTON_SEL_FILE)->EnableWindow(FALSE);
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

void CYDReadQuestionDlg::OnBnClickedButtonCqAdd()
{
	// TODO: Add your control notification handler code here
	CListAutoClean<CYdObjWrapper> clean1(m_lstAddKPs);
	UpdateData();
	if(m_strArticle.IsEmpty())
	{
		AfxMessageBox(_T("阅读文章内容不能为空！"),MB_OK|MB_ICONINFORMATION);
		return;
	}
	if (m_lstChoiceQuestion.GetItemCount() <= 0)
	{
		AfxMessageBox(_T("问题不能为空！"),MB_OK|MB_ICONINFORMATION);
		return;
	}
	HRESULT hr = E_FAIL;
	//保存一个阅读理解题
	CYDArticleQuestionRef* pRef = NULL;
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
	hr = PersistMedia(pObjWrapper->m_pObjRef);
	if(FAILED(hr))
	{
		TransRef.Rollback();
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	hr = PersistRelateQuestion(pObjWrapper->m_pObjRef);
	if(FAILED(hr))
	{
		TransRef.Rollback();
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	hr = PersistKnowledge(pObjWrapper->m_pObjRef);
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
	if(m_bIsInPaperGenerate)
	{
		m_pInPaperGenrateObjWrapper = pObjWrapper;
		OnOK();
		return ;
	}
	hr = m_pListOperate->AddItem(pObjWrapper);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	//清空控件中的数据
	hr = UpdateQuestionArea();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	UpdateData(FALSE);

	m_pCQListOperate->ClearDataCache();//删除list中的对象

	AfxMessageBox(L"新建成功！继续新增题目或完成题目录入");
}


HRESULT CYDReadQuestionDlg::ClearData()
{
	HRESULT hr = E_FAIL;
	m_strArticle = _T("");
	m_iHardLevel = 0;
	while(m_lstChoiceQuestion.GetItemCount() > 0)
	{
		m_lstChoiceQuestion.DeleteItem(0);
	}
	
	return S_OK;
}

void CYDReadQuestionDlg::OnBnClickedButtonModifyQuestion()
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_lstChoiceQuestion.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		AfxMessageBox(_T("请选择要修改的题目！"));
		return;
	}
	int nItem = m_lstChoiceQuestion.GetNextSelectedItem(pos);
	HRESULT hr = E_FAIL;
	CYdObjWrapper* pObjWrapper = (CYdObjWrapper*)m_lstChoiceQuestion.GetItemData(nItem);
	ASSERT(pObjWrapper);
	CYDChoiceQuestionDlg dlg;
	hr = dlg.SetYdQType(m_pQType);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	hr = dlg.SetObjWrapper(pObjWrapper);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	hr = dlg.SetOpType(OP_EDIT);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	hr = dlg.SetListOperate(m_pCQListOperate,nItem);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	dlg.DoModal();
}

void CYDReadQuestionDlg::OnBnClickedButtonDelQuestion()
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_lstChoiceQuestion.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		AfxMessageBox(_T("请选择要修改的题目！"));
		return;
	}
	if(AfxMessageBox(_T("你确定要删除当前选择的题目吗？"),MB_YESNO) != IDYES)
	{
		return;
	}
	int nItem = m_lstChoiceQuestion.GetNextSelectedItem(pos);
	HRESULT hr = E_FAIL;
	CYdObjWrapper* pObjWrapper = (CYdObjWrapper*)m_lstChoiceQuestion.GetItemData(nItem);
	ASSERT(pObjWrapper);
	if (pObjWrapper->m_pLinkRef != NULL)//新建的题目link为NULL
	{
		pObjWrapper->m_state = S_NEED_DELETE_BOTH;
		m_lstDeleteQuestions.push_back(pObjWrapper);
	}

	m_lstChoiceQuestion.DeleteItem(nItem);
}

HRESULT CYDReadQuestionDlg::UpdateQuestionArea()
{
	HRESULT hr = E_FAIL;
	if (m_uType == OP_NEW)
	{
		m_strArticle =_T("");
		m_iHardLevel = 0;
		CComVariant varCreateDate;
		COleDateTime dt = COleDateTime::GetCurrentTime();
		varCreateDate.vt = VT_DATE;
		varCreateDate.date = dt;
		m_strCreateDate = CDataHandler::VariantToString(varCreateDate);
		m_strCreator = DEFAULT_CREATOR;
		while(m_lstChoiceQuestion.GetItemCount() > 0 )
		{
			m_lstChoiceQuestion.DeleteItem(0);
		}
		while(m_listCtrlKpRelated.GetItemCount() > 0 )
		{
			m_listCtrlKpRelated.DeleteItem(0);
		}
		m_strMediaFile = _T("");
	}
	else
	{
		ASSERT(m_pObjWrapper->m_pObjRef->IsKindOf(RUNTIME_CLASS(CYDArticleQuestionRef)));
		CYDArticleQuestionRef* pRef = (CYDArticleQuestionRef*)m_pObjWrapper->m_pObjRef;
		CComVariant var;
		HRESULT hr = pRef->GetPropVal(L"ARTICLE", &var);
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
		
		hr = InitListChoiceQuest(pRef);
		if (FAILED(hr))
		{
			return hr;
		}
		hr = InitMedia(pRef);
		if (FAILED(hr))
		{
			return hr;
		}
		hr = pRef->GetPropVal(L"ANSWER", &var);
		if (FAILED(hr))
		{
			return hr;
		}
	
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

HRESULT CYDReadQuestionDlg::EnableQuestionArea(BOOL bEnable)
{
	HRESULT hr = E_FAIL;
	((CEdit*)(GetDlgItem(IDC_RICHEDIT_AC_ARTICLE)))->SetReadOnly(!bEnable);
	GetDlgItem(IDC_COMBO_AQ_HARDLEVEL)->EnableWindow(bEnable);
	((CEdit*)(GetDlgItem(IDC_EDIT_AQ_CREATOR)))->SetReadOnly(!bEnable);
	GetDlgItem(IDC_BUTTON_NEW_QUESTION)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_MODIFY_QUESTION)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_DEL_QUESTION)->EnableWindow(bEnable);
	return S_OK;
}

HRESULT CYDReadQuestionDlg::InitListChoiceQuest(CYDArticleQuestionRef* _pRef)
{
	HRESULT hr = E_FAIL;
	ASSERT(_pRef);
	std::list<CYDQuestionRef*> lstRef;
	std::list<CYDLinkRef*> lstLinkRef;
	hr = _pRef->GetChildQueston(&lstRef,&lstLinkRef);
	if (FAILED(hr))
	{
		return hr;
	}
	ASSERT(lstRef.size() == lstLinkRef.size());
	std::list<CYDQuestionRef*> ::const_iterator itr = lstRef.begin();
	std::list<CYDLinkRef*>::const_iterator itrLink = lstLinkRef.begin();
	for(;itr != lstRef.end() && itrLink != lstLinkRef.end();
		++itr,++itrLink)
	{
		CYdObjWrapper* pObjWrapper = new CYdObjWrapper();
		pObjWrapper->m_pObjRef = (*itr);
		pObjWrapper->m_pLinkRef = (*itrLink);
		hr = m_pCQListOperate->AddItem(pObjWrapper);
		if (FAILED(hr))
		{
			return hr;
		}
	}
	return S_OK;
}

HRESULT CYDReadQuestionDlg::InitMedia(CYDArticleQuestionRef* _pRef)
{
	HRESULT hr = E_FAIL;
	ASSERT(_pRef);
	std::list<CYDMediaRef*> lstRef;
	std::list<CYDLinkRef*> lstLinkRef;
	hr = _pRef->GetAllMedia(&lstRef,&lstLinkRef);
	if (FAILED(hr))
	{
		return hr;
	}
	ASSERT(lstRef.size() == lstLinkRef.size());
	std::list<CYDMediaRef*> ::const_iterator itr = lstRef.begin();
	std::list<CYDLinkRef*>::const_iterator itrLink = lstLinkRef.begin();
	for(;itr != lstRef.end() && itrLink != lstLinkRef.end();
		++itr,++itrLink)
	{
		CYdObjWrapper* pObjWrapper = new CYdObjWrapper();
		pObjWrapper->m_pObjRef = (*itr);
		pObjWrapper->m_pLinkRef = (*itrLink);
		m_lstMedia.push_back(pObjWrapper);
		CString strName;
		hr = (*itr)->GetLabel(&strName);
		if (FAILED(hr))
		{
			return hr;
		}
		m_strMediaFile = strName;
		_variant_t valTYPE;
		hr = (*itr)->GetPropVal(L"TYPE",&valTYPE);
		if (FAILED(hr))
		{
			return hr;
		}
		
	}
	return S_OK;
}

HRESULT CYDReadQuestionDlg::CreateQuestionRef(CYDArticleQuestionRef* &_pRef)
{
	HRESULT hr = E_FAIL;
	ASSERT(_pRef == NULL);
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	_pRef = new CYDArticleQuestionRef(pDB);
	hr = UpdateQuestionRef(_pRef);
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

HRESULT CYDReadQuestionDlg::CreateLinkRef(CYDLinkRef* &_pLinkRef)
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

HRESULT CYDReadQuestionDlg::UpdateQuestionRef(CYDArticleQuestionRef* _pRef)
{
	HRESULT hr = E_FAIL;
	CComVariant valArticle(m_strArticle);
	hr = _pRef->SetPropVal(L"ARTICLE",&valArticle);
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
	CComVariant valQNum;
	valQNum.vt = VT_I4;
	valQNum.lVal = m_lstChoiceQuestion.GetItemCount();
	hr = _pRef->SetPropVal(L"QNUM",&valQNum);
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}
void CYDReadQuestionDlg::OnBnClickedButtonCqModify()
{
	// TODO: Add your control notification handler code here
	CListAutoClean<CYdObjWrapper> clean1(m_lstAddKPs);
	UpdateData();
	if(m_strArticle.IsEmpty())
	{
		AfxMessageBox(_T("阅读文章内容不能为空！"),MB_OK|MB_ICONINFORMATION);
		return;
	}
	if (m_lstChoiceQuestion.GetItemCount() <= 0)
	{
		AfxMessageBox(_T("问题不能为空！"),MB_OK|MB_ICONINFORMATION);
		return;
	}
	ASSERT(m_pObjWrapper);
	HRESULT hr = E_FAIL;
	ASSERT(m_pObjWrapper->m_pObjRef->IsKindOf(RUNTIME_CLASS(CYDArticleQuestionRef)));
	CYDArticleQuestionRef* pRef = (CYDArticleQuestionRef*)m_pObjWrapper->m_pObjRef;
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
	hr = PersistRelateQuestion(m_pObjWrapper->m_pObjRef);
	if(FAILED(hr))
	{
		TransRef.Rollback();
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	hr = PersistMedia(m_pObjWrapper->m_pObjRef);
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



HRESULT CYDReadQuestionDlg::PersistRelateQuestion(CYDObjectRef* _pParentRef)
{
	HRESULT hr = E_FAIL;
	for(int i = 0; i < m_lstChoiceQuestion.GetItemCount();i++)
	{
		CYdObjWrapper* pObjWrapper = (CYdObjWrapper*)m_lstChoiceQuestion.GetItemData(i);
		ASSERT(pObjWrapper);
		if (pObjWrapper->m_pLinkRef == NULL)
		{
			CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
			ASSERT(pDB);
			pObjWrapper->m_pLinkRef = new CYDLinkRef(pDB, DB_ARTICLEQCHOICEQ);

			hr = pObjWrapper->m_pLinkRef->PutObjRef(_pParentRef,pObjWrapper->m_pObjRef);
			if(FAILED(hr))
			{
				return hr;
			}
		}
		
		hr = pObjWrapper->Persist();
		if(FAILED(hr))
		{
			return hr;
		}
	}
	std::list<CYdObjWrapper*>::iterator itr = m_lstDeleteQuestions.begin();
	for (; itr != m_lstDeleteQuestions.end(); ++itr)
	{
		hr = (*itr)->Persist();
		if (FAILED(hr))
		{
			return hr;
		}
	}
	m_lstDeleteQuestions.clear();
	return S_OK;
}
void CYDReadQuestionDlg::OnBnClickedButtonAddkp()
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

void CYDReadQuestionDlg::OnBnClickedButtonRemovekp()
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

HRESULT CYDReadQuestionDlg::PersistKnowledge(CYDObjectRef* _pParentRef)
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

BOOL CYDReadQuestionDlg::IfNewKPRemoveFromCache(CYdObjWrapper* pObjWrapper)
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

HRESULT CYDReadQuestionDlg::InitKnowledge()
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
void CYDReadQuestionDlg::OnBnClickedButtonSelFile()
{
	// TODO: Add your control notification handler code here
	MEDIATYPE MType = (MEDIATYPE)-1;
	HRESULT hr = E_FAIL;
	hr = GetMediaType(&MType);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	CString strFileFilter;
	hr = GetFileTypeFilter(MType,strFileFilter);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,strFileFilter);
	if(dlg.DoModal() != IDOK)
	{
		return ;
	}
	UpdateData();
	m_strMediaFile = dlg.GetPathName();
	m_bModifyMedia = TRUE;
	UpdateData(FALSE);
}

HRESULT CYDReadQuestionDlg::GetFileTypeFilter(MEDIATYPE _MType,CString& _strFiltTypeFilter)
{
	HRESULT hr = E_FAIL;
	if(_MType == M_IMAGE)
	{
		_strFiltTypeFilter = _T("图片文件(*.jpg)|*.jpg|所有文件(*.*)|*.*||");
	}
	else if(_MType == M_MP3)
	{
		_strFiltTypeFilter = _T("音频文件(*.Mp3)|*.Mp3|所有文件(*.*)|*.*||");
	}
	else if(_MType == M_VEDIO)
	{
		_strFiltTypeFilter = _T("视频文件(*.rmvb)|*.rmvb|所有文件(*.*)|*.*||");
	}
	else
	{
		_strFiltTypeFilter = _T("所有文件(*.*)|*.*||");
	}
	return S_OK;
}
HRESULT CYDReadQuestionDlg::GetMediaType(MEDIATYPE* _pMType)
{
	HRESULT hr= E_FAIL;
	*_pMType = M_MP3;
	return S_OK;
	if(((CButton*)GetDlgItem(IDC_RADIO_PICTURE))->GetCheck())
	{
		*_pMType = M_IMAGE;
	}
	else if(((CButton*)GetDlgItem(IDC_RADIO_SOUND))->GetCheck())
	{
		*_pMType = M_MP3;
	}
	else if(((CButton*)GetDlgItem(IDC_RADIO_VOD))->GetCheck())
	{
		*_pMType = M_VEDIO;
	}
	else 
	{
		ASSERT(FALSE);
	}
	return S_OK;
}

HRESULT CYDReadQuestionDlg::CreateMediaRef(CYDMediaRef*& _pRef)
{
	HRESULT hr = E_FAIL;
	_pRef = NULL;
	if(m_strMediaFile.IsEmpty())
	{
		return S_OK;
	}
	if(!PathFileExists(m_strMediaFile))
	{
		//文件不存在
		return S_OK;
	}
	if (!m_bModifyMedia)
	{
		//没有修改音频文件
		return S_OK;
	}
	MEDIATYPE MType = (MEDIATYPE)-1;
	hr = GetMediaType(&MType);
	if(FAILED(hr))
	{
		return hr;
	}
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	_pRef = new CYDMediaRef(pDB);
	int nPos = m_strMediaFile.ReverseFind(_T('\\'));
	CString strName = m_strMediaFile.Right(m_strMediaFile.GetLength()-nPos-1);
	_variant_t valName(strName);
	hr = _pRef->SetPropVal(L"Name",&valName);
	if(FAILED(hr))
	{
		return hr;
	}
	GUID guid;
	hr = CoCreateGuid(&guid);
	if(FAILED(hr))
	{
		return hr;
	}
	nPos = m_strMediaFile.ReverseFind(_T('.'));
	CString strFileExt = m_strMediaFile.Right(m_strMediaFile.GetLength()-nPos-1);
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
	_variant_t valGuidName(strGUID);
	hr = _pRef->SetPropVal(L"GUIDNAME",&valGuidName);
	if(FAILED(hr))
	{
		return hr;
	}
	_variant_t valType((long)MType);
	hr = _pRef->SetPropVal(L"TYPE",&valType);
	if(FAILED(hr))
	{
		return hr;
	}
	_variant_t valUseFor((long)M_FORARTICLE);
	hr = _pRef->SetPropVal(L"USEFOR",&valUseFor);
	if(FAILED(hr))
	{
		return hr;
	}
	_variant_t valCreator(m_strCreator);
	hr = _pRef->SetPropVal(L"CREATOR",&valCreator);
	if(FAILED(hr))
	{
		return hr;
	}
	_variant_t valCreateDate(m_strCreateDate);
	hr = _pRef->SetPropVal(L"CREATEDATE",&valCreateDate);
	if(FAILED(hr))
	{
		return hr;
	}
	_variant_t valFileName(m_strMediaFile);
	hr = _pRef->SetPropVal(L"*FileName",&valFileName);
	if(FAILED(hr))
	{
		return hr;
	}
	hr = CreateMediaFile(_pRef);
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

HRESULT CYDReadQuestionDlg::PersistMedia(CYDObjectRef* _pParentRef)
{
	HRESULT hr = E_FAIL;
	CYDMediaRef* pMeiaRef = NULL;
	hr = CreateMediaRef(pMeiaRef);
	if(FAILED(hr))
	{
		return hr;
	}
	if(pMeiaRef == NULL)
	{
		return S_OK;
	}
	if(m_uType == OP_EDIT)
	{
		//要将原来的Media删除
		for(std::list<CYdObjWrapper*>::iterator itr = m_lstMedia.begin();
			itr != m_lstMedia.end();++itr)
		{
			(*itr)->m_state = S_NEED_DELETE_BOTH;
			CYDMediaRef* pMediaRef = (CYDMediaRef*)((*itr)->m_pObjRef);
			hr = RemoveMediaFile(pMediaRef);
			if(FAILED(hr))
			{
				return hr;
			}
			hr = (*itr)->Persist();
			if(FAILED(hr))
			{
				return hr;
			}
		}
		CListAutoClean<CYdObjWrapper> clr(m_lstMedia);
	}
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	CYDLinkRef* pLinkref = new CYDLinkRef(pDB,DB_QUESTIONMEDIA);
	if(FAILED(hr))
	{
		return hr;
	}
	hr = pLinkref->AddPropDef(L"ID_TYPE",VT_I4);
	if(FAILED(hr))
	{
		return hr;
	}
	_variant_t valIDType;
	hr = CreateQTypeID(&valIDType);
	if(FAILED(hr))
	{
		return hr;
	}
	hr = pLinkref->SetPropVal(L"ID_TYPE",&valIDType);
	if(FAILED(hr))
	{
		return hr;
	}
	hr = pLinkref->PutObjRef(_pParentRef,pMeiaRef);
	if(FAILED(hr))
	{
		return hr;
	}
	CYdObjWrapper* pObjWrapper = new CYdObjWrapper();
	pObjWrapper->m_pObjRef = pMeiaRef;
	pObjWrapper->m_pLinkRef = pLinkref;
	pObjWrapper->m_state = S_NEED_SAVE_BOTH;
	hr = pObjWrapper->Persist();
	if(FAILED(hr))
	{
		return hr;
	}
	m_lstMedia.push_back(pObjWrapper);
	return S_OK;
}

HRESULT CYDReadQuestionDlg::CreateMediaFile(CYDMediaRef* _pRef)
{
	HRESULT hr = E_FAIL;
	_variant_t valFileName;
	hr = _pRef->GetPropVal(L"*FileName",&valFileName);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strFileName = CDataHandler::VariantToString(valFileName);
	CString strVaultName;
	hr = _pRef->CreateVaultName(strVaultName);
	if(FAILED(hr))
	{
		return hr;
	}
	CFtpRef* pFtpRef = NULL;
	CWnd* pWnd = AfxGetMainWnd();
	if(pWnd != NULL && pWnd->GetSafeHwnd() != NULL)
	{
		pFtpRef = (CFtpRef*)pWnd->SendMessage(WM_YD_GET_FTPREF);
	}
	ASSERT(pFtpRef);
	hr = pFtpRef->Upload(strFileName,strVaultName);
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}
HRESULT CYDReadQuestionDlg::RemoveMediaFile(CYDMediaRef* _pRef)
{
	HRESULT hr = E_FAIL;
	CString strVaultName;
	hr = _pRef->CreateVaultName(strVaultName);
	if(FAILED(hr))
	{
		return hr;
	}
	CFtpRef* pFtpRef = NULL;
	CWnd* pWnd = AfxGetMainWnd();
	if(pWnd != NULL && pWnd->GetSafeHwnd() != NULL)
	{
		pFtpRef = (CFtpRef*)pWnd->SendMessage(WM_YD_GET_FTPREF);
	}
	ASSERT(pFtpRef);
	hr = pFtpRef->Remove(strVaultName);
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

void CYDReadQuestionDlg::OnNMDblclkListQuestion(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem <0)
		return;
	HRESULT hr = E_FAIL;
	CYdObjWrapper* pObjWrapper = (CYdObjWrapper*)m_lstChoiceQuestion.GetItemData(pNMListView->iItem);
	ASSERT(pObjWrapper);
	CYDChoiceQuestionDlg dlg;
	hr = dlg.SetYdQType(m_pQType);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	hr = dlg.SetObjWrapper(pObjWrapper);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	hr = dlg.SetOpType(OP_VIEW);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
// 	hr = dlg.SetListOperate(m_pCQListOperate,pNMListView->iItem);
// 	if(FAILED(hr))
// 	{
// 		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
// 		return;
// 	}
	dlg.DoModal();
}


INT_PTR CYDReadQuestionDlg::DoModal()
{
	// TODO: Add your specialized code here and/or call the base class
	CExtDllState state;
	return CYDQuestionDlg::DoModal();
}
