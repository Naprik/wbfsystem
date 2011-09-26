#pragma once

class CYDQuestionRef;
// CStdAnswerDetialDlg dialog

class CStdAnswerDetialDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStdAnswerDetialDlg)

public:
	CStdAnswerDetialDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CStdAnswerDetialDlg();

// Dialog Data
	enum { IDD = IDD_DLG_STDANSWER_DETIAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

private:
	HRESULT FillList();

public:
	CYDQuestionRef* m_pQuestionRef;
private:
	CBCGPGridCtrl	m_GridCtrlDetial;
};
