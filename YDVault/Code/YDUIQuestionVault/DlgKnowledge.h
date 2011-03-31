#pragma once

class CYDObjectRef;
class CYDLinkRef;
class CYdObjWrapper;

// CDlgKnowledge dialog

class CDlgKnowledge : public CDialog
{
	DECLARE_DYNAMIC(CDlgKnowledge)

public:
	CDlgKnowledge(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgKnowledge();

// Dialog Data
	enum { IDD = IDD_YDUIQUESTIONVAULT_DIALOG_KNOWLEDGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strName;
	CString m_strCode;
	CString m_strCreator;
	CString m_strDescription;
	CString m_strCreateDate;
public:
	CYdObjWrapper*			m_pObjWrapper;                     //知识点
	CYDObjectRef*			m_pParentObjRef;               //试题库
	OPERATION				m_uType;                        // 
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
public:
	virtual HRESULT FillProp(CYDObjectRef* _pObjRef);
private:
	HRESULT ReadOnlyDlg(BOOL _bReadOnly);
public:

	virtual INT_PTR DoModal();
};
