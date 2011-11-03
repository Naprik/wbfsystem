#pragma once


// CDlgSelInputType dialog

class CDlgSelInputType : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSelInputType)

public:
	CDlgSelInputType(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSelInputType();

// Dialog Data
	enum { IDD = IDD_YDUIQUESTIONVAULT_DIALOG_INPUT_SEL_TYPE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_iSelType;//0:txt 1:rtf
	afx_msg void OnBnClickedOk();
};
