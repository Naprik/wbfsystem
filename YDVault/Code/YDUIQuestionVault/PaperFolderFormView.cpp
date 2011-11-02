// PaperFolderFormView.cpp : implementation file
//

#include "stdafx.h"
#include "../ObjRef/YDObjectRef.h"
#include "../ObjRef/YDLinkRef.h"
#include "../ObjRef/YdObjWrapper.h"
#include "../ObjRef/YDPaperFolder.h"
#include "PaperFolderFormView.h"
#include "../Base/AutoClean.h"
#include "../DBBase/DBTransactionRef.h"
#include "../Base/DataHandler.h"
#include "../Base/FilePathHelper.h"

// CPaperFolderFormView

IMPLEMENT_DYNCREATE(CPaperFolderFormView, CYdFormView)

CPaperFolderFormView::CPaperFolderFormView()
	: CYdFormView(CPaperFolderFormView::IDD)
	, m_strName(_T(""))
	, m_strCreator(_T(""))
	, m_strDescription(_T(""))
{

}

CPaperFolderFormView::~CPaperFolderFormView()
{
	CListAutoClean<CYdObjWrapper> clr(m_lstPaper);
}

HRESULT CPaperFolderFormView::ClearPaper()
{
	CListAutoClean<CYdObjWrapper> clr(m_lstPaper);
	return S_OK;
}

void CPaperFolderFormView::DoDataExchange(CDataExchange* pDX)
{
	CYdFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PFNAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_PFCREATOR, m_strCreator);
	DDX_Text(pDX, IDC_EDIT_PFCREATEDATE, m_strCreateDate);
	DDX_Text(pDX, IDC_EDIT_PFDESC, m_strDescription);
	DDX_Control(pDX, IDC_LIST_PAPERS, m_PaperListCtrl);
}

BEGIN_MESSAGE_MAP(CPaperFolderFormView, CYdFormView)
	ON_BN_CLICKED(IDC_BTN_OUTPUT, &CPaperFolderFormView::OnBnClickedBtnOutput)
	ON_BN_CLICKED(IDC_BTN_DEL, &CPaperFolderFormView::OnBnClickedBtnDel)
//	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_PAPERS, &CPaperFolderFormView::OnLvnItemchangedListPapers)
END_MESSAGE_MAP()


// CPaperFolderFormView diagnostics

#ifdef _DEBUG
void CPaperFolderFormView::AssertValid() const
{
	CYdFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CPaperFolderFormView::Dump(CDumpContext& dc) const
{
	CYdFormView::Dump(dc);
}
#endif
#endif //_DEBUG
void CPaperFolderFormView::OnInitialUpdate()
{
	CYdFormView::OnInitialUpdate();
	m_PaperListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	CRect rect;
	m_PaperListCtrl.GetWindowRect(rect);
	int iWidth = rect.Width();
	m_PaperListCtrl.InsertColumn(0, L"序号", LVCFMT_LEFT, iWidth/10);
	m_PaperListCtrl.InsertColumn(1, L"名称", LVCFMT_LEFT,iWidth*3/10);
	m_PaperListCtrl.InsertColumn(2, L"编号", LVCFMT_LEFT,iWidth*1/5);
	m_PaperListCtrl.InsertColumn(3, L"创建人", LVCFMT_LEFT,iWidth*1/5);
	m_PaperListCtrl.InsertColumn(4, L"创建时间", LVCFMT_LEFT,iWidth*1/5 -1);
	// TODO: Add your specialized code here and/or call the base class
	showdata();
}

void CPaperFolderFormView::showdata()
{
	m_PaperListCtrl.DeleteAllItems();
	HRESULT hr =E_FAIL;
	ClearPaper();
	
	ASSERT(m_pSheet);
	CYdObjWrapper* pObjWrapper = m_pSheet->GetCurObjWrapper();
	ASSERT(pObjWrapper);
	CYDObjectRef* pObjRef = pObjWrapper->m_pObjRef;
	ASSERT(pObjRef);
	CYDPaperFolder* pFolder = dynamic_cast<CYDPaperFolder*>(pObjRef);
	std::list<CYDPaper*> lstPapers;
	std::list<CYDLinkRef*> lstLinks;
	pFolder->GetAllPapers(&lstPapers, &lstLinks);
	std::list<CYDLinkRef*>::iterator itrLink = lstLinks.begin();
	std::list<CYDPaper*>::iterator itrPaper = lstPapers.begin();
	for(; itrPaper != lstPapers.end(); ++itrPaper, ++itrLink)
	{
		CYdObjWrapper* pObjWrapper = new CYdObjWrapper();
		pObjWrapper->m_pObjRef = *itrPaper;
		pObjWrapper->m_pLinkRef = *itrLink;
		hr = AddItem(pObjWrapper);
		if(FAILED(hr))
		{
			
			return;
		}
	}
}

HRESULT CPaperFolderFormView::AddItem(CYdObjWrapper* _pWrapper)
{
	HRESULT hr = E_FAIL;

	CYDPaper* pPaper = (CYDPaper*)_pWrapper->m_pObjRef;

	CString strName;
	hr = pPaper->GetPropVal(FIELD_YDPAPER_NAME, strName);
	if(FAILED(hr))
	{
		return hr;
	}
	
	CString strNO;
	hr = pPaper->GetPropVal(FIELD_YDPAPER_CODE,strNO);
	if(FAILED(hr))
	{
		return hr;
	}
	
	CString strCreator;
	hr = pPaper->GetPropVal(FIELD_YDPAPER_CREATOR, strCreator);
	if(FAILED(hr))
	{
		return hr;
	}
	
	CString strDate;
	hr = pPaper->GetPropVal(FIELD_YDPAPER_CREATEDATE,strDate);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strIndex(_T(""));
	strIndex.Format(_T("%d"), m_PaperListCtrl.GetItemCount()+1);
	int index = m_PaperListCtrl.InsertItem(m_PaperListCtrl.GetItemCount(), strIndex);
	m_PaperListCtrl.SetItemText(index,1, strName);
	m_PaperListCtrl.SetItemText(index,2, strNO);
	m_PaperListCtrl.SetItemText(index,3, strCreator);
	m_PaperListCtrl.SetItemText(index,4, strDate);
	m_PaperListCtrl.SetItemData(index, (DWORD_PTR)_pWrapper);

	m_lstPaper.push_back(_pWrapper);
	return S_OK;
}


// CPaperFolderFormView message handlers
void CPaperFolderFormView::ReadonlyPage(BOOL bReadonly)
{
	((CEdit*)(GetDlgItem(IDC_EDIT_PFNAME)))->SetReadOnly(bReadonly);
	((CEdit*)(GetDlgItem(IDC_EDIT_PFDESC)))->SetReadOnly(bReadonly);
}

HRESULT CPaperFolderFormView::UpdateProp(BOOL bUpdate) 
{

	HRESULT hr = E_FAIL;
	ASSERT(m_pSheet);
	CYdObjWrapper* pObjWrapper = m_pSheet->GetCurObjWrapper();
	ASSERT(pObjWrapper);
	CYDObjectRef* pObjRef = pObjWrapper->m_pObjRef;
	ASSERT(pObjRef);
	if(bUpdate)
	{
		//将属性放到控件中
		_variant_t valName;
		hr = pObjRef->GetPropVal(L"NAME",&valName);
		if(FAILED(hr))
		{
			return hr;
		}
		m_strName = CDataHandler::VariantToString(valName);
		_variant_t valCreator;
		hr = pObjRef->GetPropVal(L"CREATOR",&valCreator);
		if(FAILED(hr))
		{
			return hr;
		}
		m_strCreator = CDataHandler::VariantToString(valCreator);
		if (m_strCreator.GetLength() <= 0)
		{
			m_strCreator = DEFAULT_CREATOR;
		}
		_variant_t valCreateDate;
		hr = pObjRef->GetPropVal(L"CREATEDATE",&valCreateDate);
		if(FAILED(hr))
		{
			return hr;
		}

		if (valCreateDate.vt == VT_EMPTY)
		{
			COleDateTime date = COleDateTime::GetCurrentTime();
			valCreateDate.vt = VT_DATE;
			valCreateDate.date = date;
		}
		
		m_strCreateDate = CDataHandler::VariantToString(valCreateDate);

		_variant_t valDescription;
		hr = pObjRef->GetPropVal(L"DESCRIPTION",&valDescription);
		if(FAILED(hr))
		{
			return hr;
		}
		m_strDescription = CDataHandler::VariantToString(valDescription);

		UpdateData(FALSE);
	}
	else
	{
		//将控件的值填到属性中
		UpdateData(TRUE);
		_variant_t valName(m_strName);
		hr = pObjRef->SetPropVal(L"NAME",&valName);
		if(FAILED(hr))
		{
			return hr;
		}
		_variant_t valCreater(m_strCreator);
		hr = pObjRef->SetPropVal(L"CREATOR",&valCreater);
		if(FAILED(hr))
		{
			return hr;
		}

		_variant_t valCreateDate;
		hr = CDataHandler::StringToVariant(m_strCreateDate, VT_DATE, &valCreateDate);
		if (FAILED(hr))
		{
			return hr;
		}
		hr = pObjRef->SetPropVal(L"CREATEDATE",&valCreateDate);
		if(FAILED(hr))
		{
			return hr;
		}
		_variant_t valDesc(m_strDescription);
		hr = pObjRef->SetPropVal(L"DESCRIPTION",&valDesc);
		if(FAILED(hr))
		{
			return hr;
		}
	}

	return S_OK;
}

BOOL CPaperFolderFormView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
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
void CPaperFolderFormView::OnBnClickedBtnOutput()
{
	int nCount = m_PaperListCtrl.GetSelectedCount();
	if(nCount <= 0)
	{
		AfxMessageBox(_T("请在列表中选择要导出的试卷！"),MB_OK|MB_ICONWARNING);
		return;
	}
	POSITION pos = m_PaperListCtrl.GetFirstSelectedItemPosition();
	ASSERT(pos);
	int nItem = m_PaperListCtrl.GetNextSelectedItem(pos);
	CYdObjWrapper* pObjWrapper = (CYdObjWrapper*)m_PaperListCtrl.GetItemData(nItem);
	ASSERT(pObjWrapper);
	CYDPaper* pPaper = dynamic_cast<CYDPaper*>(pObjWrapper->m_pObjRef);
	CComVariant varName;
	pPaper->GetPropVal(L"NAME", &varName);
	CString strPaper = CDataHandler::VariantToString(varName);
	CString strPaperAnswer = strPaper + L"答案";
	CString strPaperVault, strPaperAnswerVault;
	pPaper->CreateVaultName(strPaperVault, strPaperAnswerVault);

	CString strPath;
	CFilePathHelper::GetMainCfgPath(strPath);
	strPath += L"//temp//paper//";

	CFilePathHelper pathHelper;
	CString strFolderPath = strPath + strPaper;
	CString s = strFolderPath + L"/";
	if (!pathHelper.CreateFolder(s))
	{
		AfxMessageBox(L"创建文件夹失败！");
		return;
	}
	CFtpRef* pFtpRef = NULL;
	CWnd* pWnd = AfxGetMainWnd();
	if(pWnd != NULL && pWnd->GetSafeHwnd() != NULL)
	{
		pFtpRef = (CFtpRef*)pWnd->SendMessage(WM_YD_GET_FTPREF);
	}
	CPdemWait wait(L"正在下载试卷请稍候...", FALSE, 2);
	wait.BeginWait();
	CString strPaperPath = strFolderPath + L"/" + strPaper + L".doc";
	HRESULT hr = pFtpRef->DownLoad(strPaperPath, strPaperVault, TRUE);
	if (FAILED(hr))
	{
		wait.Close();
		CString strFormat = L"试卷<%s>下载失败!";
		CString strInfo;
		strInfo.Format(strFormat, strPaperPath);
		AfxMessageBox(strInfo);
		return ;
	}
	wait.StepIt();
	wait.ResetMessage(L"正在下载试卷答案请稍候...");
	CString strPaperAnswerPath = strFolderPath + L"/" +  strPaper + L"答案.doc";
	hr = pFtpRef->DownLoad(strPaperAnswerPath, strPaperAnswerVault, TRUE);
	if (FAILED(hr))
	{
		wait.Close();
		CString strFormat = L"试卷答案<%s>下载失败!";
		CString strInfo;
		strInfo.Format(strFormat, strPaperAnswerPath);
		AfxMessageBox(strInfo);
		return;
	}
	wait.StepIt();
	wait.Close();
	DWORD_PTR dw = (DWORD_PTR)ShellExecute(NULL, L"open", strPaperPath, NULL, NULL , SW_SHOWNORMAL);
}

void CPaperFolderFormView::OnBnClickedBtnDel()
{
	int nCount = m_PaperListCtrl.GetSelectedCount();
	if(nCount <= 0)
	{
		AfxMessageBox(_T("请在列表中选择要删除的试卷！"),MB_OK|MB_ICONWARNING);
		return;
	}
	POSITION pos = m_PaperListCtrl.GetFirstSelectedItemPosition();
	ASSERT(pos);
	int nItem = m_PaperListCtrl.GetNextSelectedItem(pos);
	CYdObjWrapper* pObjWrapper = (CYdObjWrapper*)m_PaperListCtrl.GetItemData(nItem);
	ASSERT(pObjWrapper);
	CString msg;
	msg.Format(_T("确定删除选中的试卷吗？"));
	if ( MessageBox(msg,_T("确认"),MB_OKCANCEL) != IDOK)
		return;
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	HRESULT hr = E_FAIL;
	CDBTransactionRef TransRef(pDB,TRUE);
	hr = pObjWrapper->Remove();
	if(FAILED(hr))
	{
		TransRef.Rollback();
		return;
	}
	hr = TransRef.Commit();
	if(FAILED(hr))
	{
		return;
	}
	m_PaperListCtrl.DeleteItem(nItem);
}

//void CPaperFolderFormView::OnLvnItemchangedListPapers(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
//	// TODO: 在此添加控件通知处理程序代码
//	*pResult = 0;
//}
