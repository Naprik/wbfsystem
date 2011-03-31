#pragma once

#include "Resource.h"
#include "../ObjHelper/PaperAnsysHelper.h"
#include "afxcmn.h"
#include "../UIBase\BCGPSortListCtrl.h"
// CChoiceAnsys dialog

class CChoiceAnsys : public CBCGPDialog
{
	DECLARE_DYNAMIC(CChoiceAnsys)

public:
	CChoiceAnsys(CWnd* pParent = NULL);   // standard constructor
	virtual ~CChoiceAnsys();

// Dialog Data
	enum { IDD = IDD_YDUIEXAM_DLG_CHOICEANSYS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

public:
	CPaperAnsysHelperResult* m_pRes;
	CBCGPSortListCtrl m_ChoiceAnsysList;
};
