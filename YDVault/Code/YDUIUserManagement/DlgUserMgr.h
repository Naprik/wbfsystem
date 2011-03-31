#pragma once
#include "Resource.h"
#include "afxcmn.h"
#include "../UIBase\SortListCtrl.h"

// CDlgUserMgr dialog
class CYDObjectRef;

class AFX_EXT_CLASS CDlgUserMgr : public CDialog
{
	DECLARE_DYNAMIC(CDlgUserMgr)

public:
	CDlgUserMgr(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgUserMgr();

// Dialog Data
	enum { IDD = IDD_DIALOG_USERMGR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonUsermgrAdd();
	afx_msg void OnBnClickedButtonUsermgrModify();
	afx_msg void OnBnClickedButtonUsermgrDel();
	afx_msg void OnBnClickedButtonUsermgrOutput();
	CSortListCtrl m_lstUser;
	virtual BOOL OnInitDialog();
private:
	HRESULT InitUser();
	//���б��в���һ����¼
	HRESULT AddUser(CYDObjectRef* _pUser);
	std::list<CYDObjectRef*>	m_lstUserRef;
	//�����б��ϵ�_nItem����¼
	HRESULT UpdateUser(int _nItem);
public:
	virtual INT_PTR DoModal();
};
