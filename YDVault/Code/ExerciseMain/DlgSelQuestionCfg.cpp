// DlgSelQuestionCfg.cpp : implementation file
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "DlgSelQuestionCfg.h"
#include "../Base/AutoClean.h"
#include "../Base/DataHandler.h"
#include "../ObjHelper/FactorInfoHelper.h"
#include "../ObjHelper/StaticObjHelper.h"
#include "../ObjRef/YDQuestionRef.h"
#include "../ObjRef/YDQuestionType.h"
#include "../ObjRef/YDQuestionVault.h"
#include "ExerciseMain.h"
#include "SelectQuestionCfgMgr.h"
#include "QuestionCfgStruct.h"

// CDlgSelQuestionCfg dialog
namespace 
{
const int COL_QUESTION_TYPE = 0;		//题型那一列
const int COL_QUESTION_CONDITION = 1;	//难度
}
IMPLEMENT_DYNAMIC(CDlgSelQuestionCfg, CDialogEx)

CDlgSelQuestionCfg::CDlgSelQuestionCfg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSelQuestionCfg::IDD, pParent)
	, m_qnum(0)
	, m_mark(0)
{

}

CDlgSelQuestionCfg::~CDlgSelQuestionCfg()
{
	CListAutoClean<CYDObjectRef> clr1(m_lstQuestionVault);
	CListAutoClean<CYDQuestionType> clr2(m_lstClean);
	CListAutoClean<CQuestionCfgStruct> clr3(m_lstCfg);
}

void CDlgSelQuestionCfg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_QVAULT, m_cmbQuestionVault);
	DDX_Control(pDX, IDC_CMB_QTYPE, m_cmbQType);
	DDX_Text(pDX, IDC_EDIT_QNUM, m_qnum);
	DDX_Text(pDX, IDC_EDIT_QMARK, m_mark);
}


BEGIN_MESSAGE_MAP(CDlgSelQuestionCfg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_CMB_QVAULT, &CDlgSelQuestionCfg::OnCbnSelchangeCmbQvault)
	ON_CBN_SELCHANGE(IDC_CMB_QTYPE, &CDlgSelQuestionCfg::OnCbnSelchangeCmbQtype)
	ON_BN_CLICKED(IDC_BTN_DEL, &CDlgSelQuestionCfg::OnBnClickedBtnDel)
	ON_BN_CLICKED(IDC_BTN_UPDATE, &CDlgSelQuestionCfg::OnBnClickedBtnUpdate)
	ON_BN_CLICKED(IDC_BTN_ADD, &CDlgSelQuestionCfg::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_OK, &CDlgSelQuestionCfg::OnBnClickedBtnOk)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CDlgSelQuestionCfg::OnBnClickedBtnCancel)
END_MESSAGE_MAP()


// CDlgSelQuestionCfg message handlers


void CDlgSelQuestionCfg::OnCbnSelchangeCmbQvault()
{
	CWaitCursor wati;
	HRESULT hr = E_FAIL;
	if(m_cmbQuestionVault.GetCurSel() == CB_ERR)
	{
		return;
	}
	int iSel = m_cmbQuestionVault.GetCurSel();
	m_pVault = (CYDQuestionVault*)m_cmbQuestionVault.GetItemData(iSel);
	ASSERT(m_pVault);
	{
		CListAutoClean<CYDQuestionType> clean1(m_lstClean);
	}
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	hr = m_pVault->GetAllQuestionType(&m_lstClean);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	m_cmbQType.ResetContent();
	for(std::list<CYDQuestionType*>::const_iterator itr = m_lstClean.begin();
		itr != m_lstClean.end();++itr)
	{
		CString strName;
		hr = (*itr)->GetPropVal(L"NAME",strName);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		int index = m_cmbQType.InsertString(0,strName);
		m_cmbQType.SetItemData(index,(DWORD_PTR)(*itr));
	}

	m_gridFactors.RemoveAll();
	
}


BOOL CDlgSelQuestionCfg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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
	HRESULT hr = InitQuestionVault();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	CRect rectBK;
	GetDlgItem(IDC_LIST_CON)->GetWindowRect(&rectBK);
	int iWidth = rectBK.Width();
	GetDlgItem(IDC_LIST_CON)->ShowWindow(SW_HIDE);
	ScreenToClient(&rectBK);
	if (!m_gridConditions.Create(WS_CHILD | WS_TABSTOP | WS_VISIBLE, rectBK, this,
		-1))
	{
		return FALSE;
	}
	m_gridConditions.EnableDragHeaderItems(FALSE);
	m_gridConditions.EnableGroupByBox (FALSE);
	m_gridConditions.SetWholeRowSel(TRUE);
	m_gridConditions.EnableHeader (TRUE, 0);

	CBCGPGridColors colors;
	colors.m_LeftOffsetColors.m_clrBackground = globalData.clrWindow;
	m_gridConditions.SetColorTheme (colors);

	CString strName = L"题型";
	m_gridConditions.InsertColumn(COL_QUESTION_TYPE, strName, iWidth/4);

	CString strHardLevel = L"选题条件";
	m_gridConditions.InsertColumn(COL_QUESTION_CONDITION, strHardLevel, iWidth* 3/4 -2);

	for (int iColumn = 0; iColumn < m_gridConditions.GetColumnCount(); iColumn++)
	{
		m_gridConditions.SetHeaderAlign(iColumn,HDF_CENTER);
		m_gridConditions.SetColumnAlign(iColumn, HDF_LEFT);
	}
	ASSERT(m_pSelQCfgMgr);
	std::list<CQuestionCfgStruct*>::iterator itr = m_pSelQCfgMgr->m_lstQuestionStruct.begin();
	
	for (; itr != m_pSelQCfgMgr->m_lstQuestionStruct.end(); ++itr)
	{
		CQuestionCfgStruct* pcfg = new CQuestionCfgStruct(**itr);
		m_lstCfg.push_back(pcfg);
		InsertCondition(pcfg);
	}

	//指标
	GetDlgItem(IDC_LIST_FACTORS)->GetWindowRect(&rectBK);
	iWidth = rectBK.Width();
	GetDlgItem(IDC_LIST_FACTORS)->ShowWindow(SW_HIDE);
	ScreenToClient(&rectBK);
	if (!m_gridFactors.Create(WS_CHILD | WS_TABSTOP | WS_VISIBLE, rectBK, this,
		-1))
	{
		return FALSE;
	}
	m_gridFactors.EnableDragHeaderItems(FALSE);
	m_gridFactors.EnableGroupByBox (FALSE);
	m_gridFactors.SetWholeRowSel(TRUE);
	m_gridFactors.EnableHeader (TRUE, 0);

	m_gridFactors.SetColorTheme (colors);

	CString factorname = L"指标名称";
	m_gridFactors.InsertColumn(COL_QUESTION_TYPE, factorname, iWidth/4);

	CString factorvalue = L"指标值";
	m_gridFactors.InsertColumn(COL_QUESTION_CONDITION, factorvalue, iWidth* 3/4 -2);

	for (int iColumn = 0; iColumn < m_gridFactors.GetColumnCount(); iColumn++)
	{
		m_gridFactors.SetHeaderAlign(iColumn,HDF_CENTER);
		m_gridFactors.SetColumnAlign(iColumn, HDF_LEFT);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

HRESULT CDlgSelQuestionCfg::InitQuestionVault()
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
	OnCbnSelchangeCmbQvault();
	return S_OK;
}

HRESULT CDlgSelQuestionCfg::InsertCondition(CQuestionCfgStruct* pCondition)
{
	CBCGPGridRow* pRow = m_gridConditions.CreateRow(m_gridConditions.GetColumnCount());
	BOOL bColor = (m_gridConditions.GetRowCount()%2 == 0) ? TRUE : FALSE;
	CString str;
	pCondition->GetQuestionType(&str);
	pRow->GetItem(COL_QUESTION_TYPE)->SetValue(CComVariant(CComBSTR(str)));
	if (bColor)
	{
		pRow->GetItem(COL_QUESTION_TYPE)->SetBackgroundColor(RGB(110,180,200));
	}
	pRow->GetItem(COL_QUESTION_TYPE)->Enable(FALSE);
	pCondition->GetDescription(&str);
	pRow->GetItem(COL_QUESTION_CONDITION)->SetValue(CComVariant(CComBSTR(str)));
	if (bColor)
	{
		pRow->GetItem(COL_QUESTION_CONDITION)->SetBackgroundColor(RGB(110,180,200));
	}
	pRow->GetItem(COL_QUESTION_CONDITION)->Enable(FALSE);
	pRow->SetData(DWORD_PTR(pCondition));
	m_gridConditions.AddRow(pRow);

	return S_OK;
}

void CDlgSelQuestionCfg::OnCbnSelchangeCmbQtype()
{
	m_gridFactors.RemoveAll();
	CWaitCursor wati;
	HRESULT hr = E_FAIL;
	if(m_cmbQType.GetCurSel() == CB_ERR)
	{
		return;
	}
	int selindex = m_cmbQType.GetCurSel();
	CYDQuestionType* pquestiontype = (CYDQuestionType*)m_cmbQType.GetItemData(selindex);	
	std::list<CYDObjectRef*> lstfactor;
	CListAutoClean<CYDObjectRef> clr(lstfactor);
	CFactorInfoHelper helper;
	helper.GetFactorInfoByVaultQType(theApp.m_pDatabase, m_pVault, pquestiontype, &lstfactor);
	auto itr = lstfactor.begin();
	for (; itr != lstfactor.end(); ++itr)
	{
		CYDObjectRef* pfactor = *itr;
		CBCGPGridRow* pRow = m_gridFactors.CreateRow(m_gridFactors.GetColumnCount());

		CString factorname;
		pfactor->GetPropVal(FIELD_YDFACTORINFOITEM_FACTORNAME, factorname);
		pRow->GetItem(COL_QUESTION_TYPE)->SetValue(CComVariant(CComBSTR(factorname)));
		pRow->GetItem(COL_QUESTION_TYPE)->Enable(FALSE);
		if (factorname.Find(L'C') > 0)
		{
			pRow->GetItem(COL_QUESTION_CONDITION)->SetValue(CComVariant(CComBSTR(L"")));
		}
		else
		{
			pRow->GetItem(COL_QUESTION_CONDITION)->SetValue(CComVariant((long)0));
		}
		
		m_gridFactors.AddRow(pRow);
	}
	m_qnum = 0;
	m_mark = 0.0;
	UpdateData(FALSE);

}


void CDlgSelQuestionCfg::OnBnClickedBtnDel()
{
	if (m_gridConditions.GetSelectionCount() <= 0)
	{
		AfxMessageBox(L"请选择要删除的选题条件！");
		return;
	}
	if (AfxMessageBox(L"您确定要删除选择的选题条件？", MB_OKCANCEL|MB_ICONQUESTION) == IDOK)
	{
		CUIntArray rows;
		CList<CBCGPGridItem*,CBCGPGridItem*> selitems;
		m_gridConditions.GetSelectedItems(selitems);
		POSITION pos = selitems.GetHeadPosition();
		while(pos!=NULL)
		{
			CBCGPGridItem* pitem = selitems.GetNext(pos);
			CBCGPGridItemID &itemid = pitem->GetGridItemID();
			BOOL bfound = FALSE;
			for(int i=0;i<rows.GetSize();i++)
			{
				if(itemid.m_nRow == rows[i])
				{
					bfound = TRUE;
					break;
				}
			}
			if(!bfound) rows.Add(itemid.m_nRow);
		}
		for(int i=rows.GetSize()-1;i>=0;i--)
		{
			m_gridConditions.RemoveRow(rows[i],TRUE);
		}
	}
}


void CDlgSelQuestionCfg::OnBnClickedBtnUpdate()
{
	
}


void CDlgSelQuestionCfg::OnBnClickedBtnAdd()
{
	UpdateData(TRUE);
	if(m_cmbQuestionVault.GetCurSel() == CB_ERR)
	{
		AfxMessageBox(L"请选题库");
		return;
	}

	if(m_cmbQType.GetCurSel() == CB_ERR)
	{
		AfxMessageBox(L"请选择题型");
		return;
	}
	if (m_qnum <= 0)
	{
		AfxMessageBox(L"请正确输入选题数");
		GetDlgItem(IDC_EDIT_QNUM)->SetFocus();
		return;
	}
	int selindex = m_cmbQType.GetCurSel();
	CYDQuestionType* pquestiontype = (CYDQuestionType*)m_cmbQType.GetItemData(selindex);
	CQuestionCfgStruct* pcfg = new CQuestionCfgStruct();
	m_lstCfg.push_back(pcfg);

	m_pVault->GetID(&pcfg->m_vaultID);
	pquestiontype->GetID(&pcfg->m_QTypeID);
	pcfg->m_dMark = m_mark;
	pcfg->m_cNum = m_qnum;
	for (int i=0; i<m_gridFactors.GetRowCount(); ++i)
	{
		CBCGPGridRow* prow = m_gridFactors.GetRow(i);
		_variant_t var = prow->GetItem(COL_QUESTION_CONDITION)->GetValue();
		CString factorvalue = CDataHandler::VariantToString(var);
		if (factorvalue.GetLength() > 0)
		{
			var = prow->GetItem(COL_QUESTION_TYPE)->GetValue();
			CString factorname = CDataHandler::VariantToString(var);
			pcfg->m_lstFactors.push_back(std::make_pair(factorname, factorvalue));
		}
	}
	InsertCondition(pcfg);
}


void CDlgSelQuestionCfg::OnBnClickedBtnOk()
{
	{
		CListAutoClean<CQuestionCfgStruct> clr(m_pSelQCfgMgr->m_lstQuestionStruct);
	}
	for (int i=0; i<m_gridConditions.GetRowCount(); ++i)
	{
		CBCGPGridRow* prow = m_gridConditions.GetRow(i);
		CQuestionCfgStruct* pcfg = (CQuestionCfgStruct*)prow->GetData();
		CQuestionCfgStruct* pnewcfg = new CQuestionCfgStruct(*pcfg);
		m_pSelQCfgMgr->m_lstQuestionStruct.push_back(pnewcfg);
	}
	m_pSelQCfgMgr->Save();
	return OnOK();
}


void CDlgSelQuestionCfg::OnBnClickedBtnCancel()
{
	return OnCancel();
}