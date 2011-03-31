// SelFolderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SelFolderDlg.h"
#include "../Base/AutoClean.h"
#include "../Base/DataHandler.h"
#include "../ObjRef/YDPaperFolder.h"
#include "../ObjRef/YdObjWrapper.h"
#include "Resource.h"

// CSelFolderDlg dialog

IMPLEMENT_DYNAMIC(CSelFolderDlg, CDialog)

CSelFolderDlg::CSelFolderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelFolderDlg::IDD, pParent)
{

}

CSelFolderDlg::~CSelFolderDlg()
{
	CListAutoClean<CYdObjWrapper> autoClean(m_lstClear);
}

void CSelFolderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_PAPERFOLDER, m_FolderTree);
}


BEGIN_MESSAGE_MAP(CSelFolderDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSelFolderDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSelFolderDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CSelFolderDlg message handlers

void CSelFolderDlg::OnBnClickedOk()
{
	HTREEITEM hitem = m_FolderTree.GetSelectedItem();
	if (hitem == NULL)
	{
		AfxMessageBox(L"请选择归档试卷夹");
		return;
	}
	CYdObjWrapper* pObjWrapper = (CYdObjWrapper*)m_FolderTree.GetItemData(hitem);
	if (pObjWrapper == NULL)
	{
		AfxMessageBox(L"请选择归档试卷夹");
		return;
	}
	OBJID objID;
	pObjWrapper->m_pObjRef->GetID(&objID);
	if (objID == ID_EMPTY)
	{
		AfxMessageBox(L"请选择归档试卷夹");
		return;
	}

	m_pPaperFolder = pObjWrapper->m_pObjRef;

	OnOK();
}

void CSelFolderDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

BOOL CSelFolderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	BOOL bHiColorIcons = (BOOL)AfxGetMainWnd()->SendMessage(WM_YD_GET_BHICOLORICONS);
	UINT uiBmpId = bHiColorIcons ? IDB_CLASS_VIEW_24 : IDB_CLASS_VIEW;
	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("Can't load bitmap: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return FALSE;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_PaperFolderViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_PaperFolderViewImages.Add(&bmp, RGB(255, 0, 0));

	m_FolderTree.SetImageList(&m_PaperFolderViewImages, TVSIL_NORMAL);
	//生成根节点
	CString strRootName;
	strRootName.LoadString(IDS_YDUIQUESTIONVAULT_PAPERMGR_ROOT);
	HTREEITEM hItem =  m_FolderTree.InsertItem(strRootName, NULL, NULL);
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	CYDPaperFolder* pRootPF = new CYDPaperFolder(pDB);
	HRESULT hr = pRootPF->SetID(0);//设根节点的对象ID为0
	if(FAILED(hr))
	{
		return FALSE;
	}
	CYdObjWrapper* pObjWrapper = new CYdObjWrapper();
	pObjWrapper->m_pObjRef = pRootPF;
	pObjWrapper->m_pRelatedWnd = &m_FolderTree;
	pObjWrapper->m_hRelatedItem = hItem;
	m_FolderTree.SetItemData(hItem, (DWORD_PTR)pObjWrapper);
	m_lstClear.push_back(pObjWrapper);

	hr = RefreshNode(hItem);
	if(FAILED(hr))
	{
		return FALSE;
	}
	m_FolderTree.Expand(hItem, TVE_EXPAND);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


HRESULT CSelFolderDlg::RefreshNode(HTREEITEM _hItem)//刷新_hItem数据
{
	HRESULT hr = E_FAIL;
	hr = DelAllChildNode(_hItem);
	if(FAILED(hr))
	{
		return hr;
	}
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);

	CYdObjWrapper* pObjWrapper = (CYdObjWrapper*)m_FolderTree.GetItemData(_hItem);
	ASSERT(pObjWrapper != NULL);

	CYDPaperFolder* pPF= dynamic_cast<CYDPaperFolder*>(pObjWrapper->m_pObjRef);
	ASSERT(pPF != NULL);

	std::list<CYDPaperFolder*> lstChild;
	std::list<CYDLinkRef*> lstLinkRef;
	hr = pPF->GetChildPaperFolder(&lstChild, &lstLinkRef);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(lstChild.size() == lstLinkRef.size());
	std::list<CYDPaperFolder*>::const_iterator itrChild = lstChild.begin();
	std::list<CYDLinkRef*>::const_iterator itrLink = lstLinkRef.begin();
	for(;itrChild != lstChild.end() && itrLink != lstLinkRef.end();++itrChild,++itrLink)
	{
		hr = InsertYDObject(_hItem, (*itrChild), (*itrLink));
		if(FAILED(hr))
		{
			return hr;
		}
	}

	return S_OK;
}

HRESULT CSelFolderDlg::DelAllChildNode(HTREEITEM _hItem)//删除_hItem的所有子节点
{
	HRESULT hr = E_FAIL;
	while(m_FolderTree.ItemHasChildren(_hItem))
	{
		HTREEITEM hTemp = m_FolderTree.GetChildItem(_hItem);
		m_FolderTree.DeleteItem(hTemp);
	}
	return S_OK;
}

HRESULT CSelFolderDlg::InsertYDObject(HTREEITEM _hParent,
									   CYDObjectRef* _pObj,
									   CYDLinkRef* _pLink /*= NULL*/)
{
	HRESULT hr = E_FAIL;
	hr = _pObj->Restore();
	if (FAILED(hr))
	{
		return hr;
	}
	_variant_t valName;
	hr = _pObj->GetPropVal(L"NAME", &valName);
	if(FAILED(hr))
	{
		return hr;
	}

	CString strName = CDataHandler::VariantToString(valName);

	HTREEITEM hItem = m_FolderTree.InsertItem(strName,_hParent,NULL);
	CYdObjWrapper* pObjWrapper = new CYdObjWrapper();
	pObjWrapper->m_pObjRef = _pObj;
	pObjWrapper->m_pLinkRef = _pLink;
	pObjWrapper->m_pRelatedWnd = &m_FolderTree;
	pObjWrapper->m_hRelatedItem = hItem;
	m_lstClear.push_back(pObjWrapper);
	m_FolderTree.SetItemData(hItem, (DWORD_PTR)pObjWrapper);

	return S_OK;
}



INT_PTR CSelFolderDlg::DoModal()
{
	// TODO: Add your specialized code here and/or call the base class
	CExtDllState state;
	return CDialog::DoModal();
}
