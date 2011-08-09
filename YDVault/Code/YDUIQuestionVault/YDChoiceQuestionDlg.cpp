// YDChoiceQuestionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "YDChoiceQuestionDlg.h"
#include "../ObjRef/YDChoiceQuestionRef.h"
#include "../ObjRef/YDLinkRef.h"
#include "../ObjRef/YdObjWrapper.h"
#include "../Include/ShowErr.h"
#include "../Base/DataHandler.h"
#include "../ObjRef/YDQuestionType.h"
#include "../DBBase/DBTransactionRef.h"
#include "../YDFormUIBase/ListCtrlOperate.h"
#include "../Base/AutoClean.h"
#include "Resource.h"


// CYDChoiceQuestionDlg dialog



IMPLEMENT_DYNAMIC(CYDChoiceQuestionDlg, CYDQuestionDlg)

CYDChoiceQuestionDlg::CYDChoiceQuestionDlg(CWnd* pParent /*=NULL*/)
	: CYDQuestionDlg(CYDChoiceQuestionDlg::IDD, pParent)
	, m_strCode(_T(""))
	, m_strTitle(_T(""))
	, m_strAnswer(_T(""))
	, m_iHardLevel(0)
	, m_strCreator(_T(""))
	, m_strCreateDate(_T(""))
	, m_strAnswerDesc(_T(""))
	, m_bCheckSelPic(FALSE)
{
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	m_pCQ = new CYDChoiceQuestionRef(pDB);
	m_pParentQuestionRef = NULL;
}

CYDChoiceQuestionDlg::~CYDChoiceQuestionDlg()
{
	delete m_pCQ;
	m_pCQ = NULL;
	CListAutoClean<CYdObjWrapper> clean1(m_lstAddKPs);

}

void CYDChoiceQuestionDlg::DoDataExchange(CDataExchange* pDX)
{
	CYDQuestionDlg::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CQ_CODE, m_strCode);
	DDX_Text(pDX, IDC_EDIT_CQ_TITLE, m_strTitle);
	DDX_Text(pDX, IDC_EDIT_CQ_ANSWER, m_strAnswer);
	DDX_CBIndex(pDX, IDC_COMBO_CQ_HARDLEVEL, m_iHardLevel);
	DDX_Text(pDX, IDC_EDIT_CQ_CREATOR, m_strCreator);
	DDX_Text(pDX, IDC_EDIT_CQ_CREATE_DATE, m_strCreateDate);
	DDX_Control(pDX, IDC_LIST_CQ_KPALL, m_listCtrlKpAll);
	DDX_Control(pDX, IDC_LIST_CQ_KPRELATED, m_listCtrlKpRelated);
	DDX_Text(pDX, IDC_EDIT_CQ_ANSWER_DESC, m_strAnswerDesc);
	DDX_Check(pDX, IDC_CHECK_SEL_PIC, m_bCheckSelPic);
}


BEGIN_MESSAGE_MAP(CYDChoiceQuestionDlg, CYDQuestionDlg)
	ON_BN_CLICKED(IDC_BUTTON_CQ_ADD, &CYDChoiceQuestionDlg::OnBnClickedButtonCqAdd)
	ON_BN_CLICKED(IDC_BUTTON_CQ_CLOSE, &CYDChoiceQuestionDlg::OnBnClickedButtonCqClose)
	ON_BN_CLICKED(IDC_BUTTON_CQ_MODIFY, &CYDChoiceQuestionDlg::OnBnClickedButtonCqModify)
	ON_BN_CLICKED(IDC_BUTTON_CQ_ADDKP, &CYDChoiceQuestionDlg::OnBnClickedButtonCqAddkp)
	ON_BN_CLICKED(IDC_BUTTON_CQ_REMOVEKP, &CYDChoiceQuestionDlg::OnBnClickedButtonCqRemovekp)
	ON_BN_CLICKED(IDC_CHECK_SEL_PIC, &CYDChoiceQuestionDlg::OnBnClickedCheckSelPic)
END_MESSAGE_MAP()


// CYDChoiceQuestionDlg message handlers

void CYDChoiceQuestionDlg::OnBnClickedButtonCqAdd()
{
	// TODO: Add your control notification handler code here

	CListAutoClean<CYdObjWrapper> clean1(m_lstAddKPs);

	UpdateData();
	QTYPE iQType;
	GetQuestionType(&iQType);
	if (iQType == QTYPE_VOCABULARY)//选择题
	{
		if (!VocabularyValid())
		{
			return;
		}
	}
	else
	{
		if (!ValidOther())
		{
			return;
		}
	}

	HRESULT hr = E_FAIL;
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
	CYdObjWrapper* pObjWrapper = new CYdObjWrapper();
	pObjWrapper->m_pObjRef = pRef;
	pObjWrapper->m_pLinkRef = pLinkRef;

	if(iQType == QTYPE_VOCABULARY)//选择题保存
	{
		//当前选择题是挂到题库下面的，要保存到数据库
		hr = pLinkRef->PutObjRef((CYDObjectRef*)m_pQVault, pRef);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
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
		//保存知识点
		std::list<CYdObjWrapper*>::iterator itrKp = m_lstAddKPs.begin();
		for (; itrKp != m_lstAddKPs.end(); ++itrKp)
		{
			(*itrKp)->m_pLinkRef->PutObjRef(pRef, (*itrKp)->m_pObjRef);
			hr = (*itrKp)->Persist();
			if(FAILED(hr))
			{
				TransRef.Rollback();
				DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
				return ;
			}
		}
		hr = TransRef.Commit();
		if(FAILED(hr))
		{
			TransRef.Rollback();
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
	}
	else
	{
		pObjWrapper->m_state = S_NEED_SAVE_BOTH;
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
	UpdateQuestionArea();

	UpdateData(FALSE);
	AfxMessageBox(L"新建成功！继续新增题目或完成题目录入");
}

BOOL CYDChoiceQuestionDlg::VocabularyValid()
{
	if (m_strTitle.GetLength() <= 0)
	{
		AfxMessageBox(L"提干不能为空！", MB_OK, MB_ICONWARNING);
		GetDlgItem(IDC_EDIT_CQ_TITLE)->SetFocus();
		return FALSE;
	}
	if (m_strAnswer.GetLength() <= 0)
	{
		AfxMessageBox(L"答案不能为空！", MB_OK, MB_ICONWARNING);
		GetDlgItem(IDC_EDIT_CQ_ANSWER)->SetFocus();
		return FALSE;
	}
	std::list<CString> lst;
	GetOption(&lst);
	if (lst.size() <= 0)
	{
		AfxMessageBox(L"选项不能为空！", MB_OK, MB_ICONWARNING);
		return FALSE;
	}
	if(!ValidateOption())
	{
		return FALSE;
	}
	if(!ValidateAsnwer())
	{
		return FALSE;
	}
	if(!ValidateIndicator(&m_GridIndicator))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CYDChoiceQuestionDlg::ValidOther()
{
	if (m_strAnswer.GetLength() <= 0)
	{
		AfxMessageBox(L"答案不能为空！", MB_OK, MB_ICONWARNING);
		GetDlgItem(IDC_EDIT_CQ_ANSWER)->SetFocus();
		return FALSE;
	}
	if(!ValidateOption())
	{
		return FALSE;
	}
	if(!ValidateAsnwer())
	{
		return FALSE;
	}
// 	std::list<CString> lst;
// 	GetOption(&lst);
// 	if (lst.size() <= 0)
// 	{
// 		AfxMessageBox(L"选项不能为空！", MB_OK, MB_ICONWARNING);
// 		return FALSE;
// 	}
	return TRUE;
}

BOOL CYDChoiceQuestionDlg::IfNewKPRemoveFromCache(CYdObjWrapper* pObjWrapper)
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

void CYDChoiceQuestionDlg::OnBnClickedButtonCqClose()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

HRESULT CYDChoiceQuestionDlg::CreateQuestionRef(CYDChoiceQuestionRef* &_pRef)
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

HRESULT CYDChoiceQuestionDlg::CreateLinkRef(CYDLinkRef* &_pLinkRef)
{
	HRESULT hr = E_FAIL;
	QTYPE qtype;
	GetQuestionType(&qtype);
	if (qtype == QTYPE_VOCABULARY)
	{
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
	}
	
	
	return S_OK;
}

HRESULT CYDChoiceQuestionDlg::UpdateQuestionRef(CYDChoiceQuestionRef* _pRef)
{
	HRESULT hr = E_FAIL;
	CComVariant valCode;
	hr = CDataHandler::StringToVariant(m_strCode,VT_BSTR,&valCode);
	if(FAILED(hr))
	{
		return hr;
	}
	hr = _pRef->SetPropVal(L"Code",&valCode);
	if(FAILED(hr))
	{
		return hr;
	}
	CComVariant valTitle;
	hr = CDataHandler::StringToVariant(m_strTitle,VT_BSTR,&valTitle);
	if(FAILED(hr))
	{
		return hr;
	}
	hr = _pRef->SetPropVal(L"TITLE",&valTitle);
	if(FAILED(hr))
	{
		return hr;
	}
	
	std::list<CString> lstChoices;
	GetOption(&lstChoices);
	hr = _pRef->SetOptionList(&lstChoices);
	if(FAILED(hr))
	{
		return hr;
	}
	CComVariant valAnswer;
	hr = CDataHandler::StringToVariant(m_strAnswer,VT_BSTR,&valAnswer);
	if(FAILED(hr))
	{
		return hr;
	}
	hr = _pRef->SetPropVal(L"ANSWER",&valAnswer);
	if(FAILED(hr))
	{
		return hr;
	}

	CComVariant valAnswerDesc;
	hr = CDataHandler::StringToVariant(m_strAnswerDesc,VT_BSTR,&valAnswerDesc);
	if(FAILED(hr))
	{
		return hr;
	}
	hr = _pRef->SetPropVal(L"ANSWERDESC",&valAnswerDesc);
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
	//图像的数据

	hr = UpdateIndicator(_pRef,&m_GridIndicator);
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

HRESULT CYDChoiceQuestionDlg::CreateChoice(VARIANT* _valChoice)
{
	HRESULT hr = E_FAIL;
	return S_OK;
}

BOOL CYDChoiceQuestionDlg::OnInitDialog()
{
	CYDQuestionDlg::OnInitDialog();

	// TODO:  Add extra initialization here
	//初始化试题选项列表

	CRect gridOptionLocal;
	GetDlgItem(IDC_STATIC_GRID)->GetWindowRect(&gridOptionLocal);
	ScreenToClient(&gridOptionLocal);
	if (!m_gridOption.Create(WS_CHILD|WS_VISIBLE|WS_BORDER, gridOptionLocal, 
		this, IDC_CQ_GCL_OPTION))
	{
		TRACE0("Failed to m_listctrl.Create!\n");
		return FALSE;
	}
	m_gridOption.EnableDragHeaderItems(FALSE);
	m_gridOption.EnableColumnAutoSize (TRUE);
	m_gridOption.EnableGroupByBox (FALSE);
	m_gridOption.SetWholeRowSel(FALSE);
	m_gridOption.EnableHeader (TRUE, 0);

	CBCGPGridColors colors;
	colors.m_LeftOffsetColors.m_clrBackground = globalData.clrWindow;
	m_gridOption.SetColorTheme (colors);

	//指标
	CreateIndicatorGridCtrl(IDC_STATIC_INDICATOR,&m_GridIndicator);

	//初始化表头
	CString strName = L"序号";
	m_gridOption.InsertColumn(0, strName, gridOptionLocal.Width()/8 - 1);
	CString strOption = L"选项";
	m_gridOption.InsertColumn(1, strOption, gridOptionLocal.Width()*7/8 -1);
	for (int iColumn = 0; iColumn < m_gridOption.GetColumnCount(); iColumn++)
	{
		m_gridOption.SetHeaderAlign(iColumn, HDF_CENTER);
		m_gridOption.SetColumnAlign(iColumn, HDF_LEFT);
	}
	Fill();
	m_gridOption.AdjustLayout();
	if(m_uType == OP_NEW)
	{
		//新增
		GetDlgItem(IDC_BUTTON_CQ_ADD)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_CQ_MODIFY)->EnableWindow(FALSE);
		EnableQuestionArea(TRUE);
		GetDlgItem(IDC_BUTTON_CQ_ADDKP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_CQ_REMOVEKP)->EnableWindow(TRUE);
	}
	else if(m_uType == OP_EDIT)
	{
		//修改
		GetDlgItem(IDC_BUTTON_CQ_ADD)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CQ_MODIFY)->EnableWindow(TRUE);

		GetDlgItem(IDC_BUTTON_CQ_ADDKP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_CQ_REMOVEKP)->EnableWindow(TRUE);
		EnableQuestionArea(TRUE);
	}
	else if (m_uType = OP_VIEW)
	{
		//查看
		EnableQuestionArea(FALSE);
		GetDlgItem(IDC_BUTTON_CQ_ADD)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CQ_MODIFY)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_CQ_ADDKP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CQ_REMOVEKP)->EnableWindow(FALSE);
	}
	else
	{
		ASSERT(FALSE);
	}
	UpdateQuestionArea();

	//知识点初始化
	QTYPE iQType;
	GetQuestionType(&iQType);
	HRESULT hr = E_FAIL;
	if (iQType == QTYPE_VOCABULARY)//选择题
	{
		
		CRect rectKP;
		GetDlgItem(IDC_LIST_CQ_KPALL)->GetWindowRect(rectKP);
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
				DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
				return FALSE;
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
					DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
					return FALSE;
				}
				int index = m_listCtrlKpRelated.InsertItem(iCount,strLabel);
				m_listCtrlKpRelated.SetItemData(index,(DWORD_PTR)(*itr));
			}
		}
	}
	else
	{
		EnableCtrl(iQType);
	}
	UpdateData(FALSE);
	CString strTitle;
	m_pQType->GetLabel(&strTitle);
	if(m_uType == OP_NEW)
	{
		//新增
		strTitle = _T("新增-") + strTitle;
	}
	else if(m_uType == OP_EDIT)
	{
		//修改
		strTitle = _T("修改-") + strTitle;
	}
	else if (m_uType = OP_VIEW)
	{
		//查看
		strTitle = _T("修改-") + strTitle;
	}
	else
	{
		ASSERT(FALSE);
	}
	SetWindowText(strTitle);



	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



BOOL CYDChoiceQuestionDlg::EnableCtrl(QTYPE type)
{
	GetDlgItem(IDC_BUTTON_CQ_ADDKP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CQ_REMOVEKP)->EnableWindow(FALSE);
	if (type == QTYPE_LISTENINGCOMPREHENSION ||
		type == QTYPE_LISTENINGVOCABULARY ||
		type == QTYPE_READINGVOCABULARY ||
		type == QTYPE_LISTENINGLONG ||
		type == QTYPE_LISTENINGSHORT ||
		type == QTYPE_ERRORCORRECTION)
	{
		((CEdit*)(GetDlgItem(IDC_EDIT_CQ_TITLE)))->SetReadOnly(TRUE);
		if (type == QTYPE_LISTENINGVOCABULARY ||
			type == QTYPE_ERRORCORRECTION)
		{
			GetDlgItem(IDC_CQ_GCL_OPTION)->EnableWindow(FALSE);
		}
	}

	return TRUE;
}

HRESULT CYDChoiceQuestionDlg::Fill()
{
	std::list<CString>::const_iterator itr = m_pCQ->m_LstSequence.begin();
	for (; itr != m_pCQ->m_LstSequence.end(); ++itr)
	{
		CBCGPGridRow* pRow = m_gridOption.CreateRow(m_gridOption.GetColumnCount());

		CComBSTR bstr = *itr;
		CComVariant varVal = bstr;
		pRow->GetItem(0)->SetValue(varVal);
		pRow->GetItem(0)->SetBackgroundColor(RGB(110,180,200));
		pRow->GetItem(0)->Enable(FALSE);
		//选项内容
		pRow->ReplaceItem(1, new CBCGPGridItem(L""));
		m_gridOption.AddRow(pRow);
	}

	return S_OK;
}

HRESULT CYDChoiceQuestionDlg::EnableQuestionArea(BOOL bEnable)
{
	HRESULT hr = E_FAIL;
	((CEdit*)(GetDlgItem(IDC_EDIT_CQ_TITLE)))->SetReadOnly(!bEnable);
	GetDlgItem(IDC_COMBO_CQ_HARDLEVEL)->EnableWindow(bEnable);
	m_gridOption.EnableWindow(bEnable);
	((CEdit*)(GetDlgItem(IDC_EDIT_CQ_ANSWER)))->SetReadOnly(!bEnable);
	((CEdit*)(GetDlgItem(IDC_EDIT_CQ_CODE)))->SetReadOnly(!bEnable);
	return S_OK;
}

HRESULT CYDChoiceQuestionDlg::UpdateQuestionArea()
{
	HRESULT hr = E_FAIL;
	m_GridIndicator.RemoveAll();
	if (m_uType == OP_NEW)
	{
		m_strTitle = L"";
		m_strAnswer = L"";
		m_strCode = L"";
		m_strAnswerDesc = L"";
		m_iHardLevel = 0;
		for (int i=0; i<m_gridOption.GetRowCount(); ++i)
		{
			m_gridOption.GetRow(i)->GetItem(1)->SetValue(L"");
		}
		CComVariant varCreateDate;
		COleDateTime dt = COleDateTime::GetCurrentTime();
		varCreateDate.vt = VT_DATE;
		varCreateDate.date = dt;
		m_strCreateDate = CDataHandler::VariantToString(varCreateDate);
		m_strCreator = DEFAULT_CREATOR;
		m_listCtrlKpRelated.DeleteAllItems();
		QTYPE iQType = QTYPE(-1);
		GetQuestionType(&iQType);
		if (iQType == QTYPE_VOCABULARY)//选择题
		{
			hr = CreateIndicator(NULL,&m_GridIndicator);
			if (FAILED(hr))
			{
				return hr;
			}
		}
	}
	else
	{
		ASSERT(m_pObjWrapper->m_pObjRef->IsKindOf(RUNTIME_CLASS(CYDChoiceQuestionRef)));
		CYDChoiceQuestionRef* pRef = (CYDChoiceQuestionRef*)m_pObjWrapper->m_pObjRef;
		CComVariant var;
		 hr = pRef->GetPropVal(L"TITLE", &var);
		if (FAILED(hr))
		{
			return hr;
		}
		m_strTitle = CDataHandler::VariantToString(var);
		var.Clear();
		hr = pRef->GetPropVal(L"HARDLEVEL", &var);
		if (FAILED(hr))
		{
			return hr;
		}
		LONG lHardLevel = CDataHandler::VariantToLong(var);
		m_iHardLevel = lHardLevel == 0 ? EASY : HARD;
		var.Clear();
		std::list<CString> lstChoices;
		hr = pRef->GetOptionList(&lstChoices);
		if (FAILED(hr))
		{
			return hr;
		}
		FillOption(&lstChoices);
		//没有写
		hr = pRef->GetPropVal(L"ANSWER", &var);
		if (FAILED(hr))
		{
			return hr;
		}
		m_strAnswer = CDataHandler::VariantToString(var);
		var.Clear();
		hr = pRef->GetPropVal(L"ANSWERDESC", &var);
		if (FAILED(hr))
		{
			return hr;
		}
		m_strAnswerDesc = CDataHandler::VariantToString(var);
		var.Clear();
		hr = pRef->GetPropVal(L"CODE", &var);
		if (FAILED(hr))
		{
			return hr;
		}
		m_strCode = CDataHandler::VariantToString(var);
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
		QTYPE iQType = QTYPE(-1);
		GetQuestionType(&iQType);
		if (iQType == QTYPE_VOCABULARY)//选择题
		{
			hr = CreateIndicator(pRef,&m_GridIndicator);
			if (FAILED(hr))
			{
				return hr;
			}
		}
		
	}

	UpdateData(FALSE);
	return S_OK;
}


HRESULT CYDChoiceQuestionDlg::GetOption(std::list<CString>* pListOption)
{
	HRESULT hr = E_FAIL;

	for (int i=0; i<m_gridOption.GetRowCount(); ++i)
	{
		CComVariant var = m_gridOption.GetRow(i)->GetItem(1)->GetValue();
		CString str = CDataHandler::VariantToString(var);
		if (str.GetLength() > 0)
		{
			pListOption->push_back(str);
		}
	}

	return S_OK;
}

BOOL CYDChoiceQuestionDlg::ValidateOption()
{
	//选项的列表智能从上往下填，如果A，C已经填了选项，而B为填则不允许
	CString strLastNotEmpty;//最下面一个已经填充的选项
	for (int i=m_gridOption.GetRowCount() -1; i >= 0; --i)
	{
		CComVariant var = m_gridOption.GetRow(i)->GetItem(1)->GetValue();
		CString str = CDataHandler::VariantToString(var);
		if (str.GetLength() > 0 && strLastNotEmpty.IsEmpty())
		{
			strLastNotEmpty.Format(_T("%c"),_T('A')+i);
			continue;
		}
		if(str.IsEmpty() && !strLastNotEmpty.IsEmpty())
		{
			CString strMsg;
			strMsg.Format(_T("%c选项不能为空，因为%s选项已经填写！"),_T('A')+i,strLastNotEmpty);
			AfxMessageBox(strMsg);
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CYDChoiceQuestionDlg::ValidateAsnwer()
{
	//要判断答案是否为已经填写的选项中的
	CString strAllAnswer;
	for (int i = 0; i <m_gridOption.GetRowCount() ; ++i)
	{
		CComVariant var = m_gridOption.GetRow(i)->GetItem(1)->GetValue();
		CString str = CDataHandler::VariantToString(var);
		if (str.GetLength() > 0)
		{
			CString strIndex;
			strIndex.Format(_T("%c"),_T('A')+i);
			strAllAnswer += strIndex;
		}
	}
	strAllAnswer.MakeUpper();
	CString strAnswer = m_strAnswer;
	strAnswer.MakeUpper();
	for(int i = 0 ; i < strAnswer.GetLength();i++)
	{
		TCHAR chAnswer = strAnswer.GetAt(i);
		if(strAllAnswer.Find(chAnswer) == -1)
		{
			AfxMessageBox(_T("答案不在已经填写的选项中！"));
			return FALSE;
		}
	}
	return TRUE;
}

HRESULT CYDChoiceQuestionDlg::FillOption(std::list<CString>* pListOption)
{
	HRESULT hr = E_FAIL;

	std::list<CString>::const_iterator itr = pListOption->begin();
	int i = 0;
	for (; itr != pListOption->end(); ++itr)
	{
		m_gridOption.GetRow(i++)->GetItem(1)->SetValue(CComVariant(*itr));
	}

	return S_OK;
}


void CYDChoiceQuestionDlg::OnBnClickedButtonCqModify()
{
	// TODO: Add your control notification handler code here
	CListAutoClean<CYdObjWrapper> clean1(m_lstAddKPs);
	UpdateData();
	QTYPE iQType;
	GetQuestionType(&iQType);
	if (iQType == QTYPE_VOCABULARY)//选择题
	{
		if (!VocabularyValid())
		{
			return;
		}
	}
	else
	{
		if (!ValidOther())
		{
			return;
		}
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
	if (iQType == QTYPE_VOCABULARY)
	{
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
		std::list<CYdObjWrapper*>::iterator itr = m_lstDeleteKPs.begin();
		for (; itr != m_lstDeleteKPs.end(); ++itr)
		{
			hr = (*itr)->Persist();
			if (FAILED(hr))
			{
				TransRef.Rollback();
				DISPLAY_YDERROR(hr, MB_OK | MB_ICONINFORMATION);
				return ;
			}
		}
		itr = m_lstAddKPs.begin();
		for (; itr != m_lstAddKPs.end(); ++itr)
		{
			(*itr)->m_pLinkRef->PutObjRef(pRef, (*itr)->m_pObjRef);
			hr = (*itr)->Persist();
			if (FAILED(hr))
			{
				TransRef.Rollback();
				DISPLAY_YDERROR(hr, MB_OK | MB_ICONINFORMATION);
				return ;
			}
		}
		hr = TransRef.Commit();
		if(FAILED(hr))
		{
			TransRef.Rollback();
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
	}
	else
	{
		if (m_pObjWrapper->m_pLinkRef != NULL)
		{
			m_pObjWrapper->m_state = S_NEED_UPDATE_OBJECT;
		}
	}

	hr = m_pListOperate->ModifyItem(m_index,m_pObjWrapper);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	OnOK();
}


void CYDChoiceQuestionDlg::OnBnClickedButtonCqAddkp()
{
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

void CYDChoiceQuestionDlg::OnBnClickedButtonCqRemovekp()
{
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


INT_PTR CYDChoiceQuestionDlg::DoModal()
{
	// TODO: Add your specialized code here and/or call the base class
	CExtDllState state;
	return CYDQuestionDlg::DoModal();
}


void CYDChoiceQuestionDlg::OnBnClickedCheckSelPic()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(!m_bCheckSelPic)
	{
		return;
	}
	if (m_uType == OP_NEW)
	{
	}
	else
	{
		CYDChoiceQuestionRef* pRef = (CYDChoiceQuestionRef*)m_pObjWrapper->m_pObjRef;
		ASSERT(pRef);
	}
	
}
