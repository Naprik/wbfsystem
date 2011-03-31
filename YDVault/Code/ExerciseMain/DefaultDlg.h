#pragma once
#include "QuestionDlg.h"

// CDefaultDlg dialog

class CDefaultDlg : public CQuestionDlg
{
	DECLARE_DYNAMIC(CDefaultDlg)

public:
	CDefaultDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDefaultDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_DEFAULT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
