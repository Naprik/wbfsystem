#pragma once
#include "afxcmn.h"

#include "../ObjRef/YDLinkRef.h"
#include "../ObjRef/YDObjectRef.h"
// CSelFolderDlg dialog
class CYdObjWrapper;

class CSelFolderDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelFolderDlg)

public:
	CSelFolderDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelFolderDlg();

// Dialog Data
	enum { IDD = IDD_YDUIQUESTIONVAULT_DIALOG_PAPERFOLDER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	
	virtual BOOL OnInitDialog();
private:
	HRESULT RefreshNode(HTREEITEM _hItem);
	HRESULT DelAllChildNode(HTREEITEM _hItem);
	HRESULT InsertYDObject(HTREEITEM _hParent,
							CYDObjectRef* _pObj,
							CYDLinkRef* _pLink = NULL);
private:
	std::list<CYdObjWrapper*> m_lstClear;//”√”⁄ Õ∑≈
	CTreeCtrl m_FolderTree;
	CImageList				 m_PaperFolderViewImages;

public:
	CYDObjectRef*			 m_pPaperFolder;
	virtual INT_PTR DoModal();
};
