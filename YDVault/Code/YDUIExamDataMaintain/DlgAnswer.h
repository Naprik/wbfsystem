#pragma once

#include "resource.h"
#include "../UIBase/BCGPKeyGridCtrl.h"
#include "../ObjRef/YDAnswer.h"
#include "../ObjRef/YDPaper.h"
#include "../ObjRef/YdObjWrapper.h"
#include <map>
#include "../YDExamObjRef\AnswerRecord.h"
// CDlgAnswer dialog


class BASE_DLL_API CDlgAnswer : public CDialog
{
	DECLARE_DYNAMIC(CDlgAnswer)

public:
	CDlgAnswer(CYDObjectRef* _pPaper, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgAnswer();

// Dialog Data
	enum { IDD = IDD_YDUIEXAM_DATA_MAINTAIN_DLG_EDIT_ANSWER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

private:
	HRESULT FillGrid();
	HRESULT InsertItem(CYdObjWrapper* pObjWrapper);

private:
	CBCGPKeyGridCtrl	m_AnswerGrid;
	CYDObjectRef*	m_pPaper;

	std::list<CYdObjWrapper*>	m_lstObjWrapperCache;
	
	typedef std::map<OBJID, CAnswerRecord*>		MapAnswerRecord;
	MapAnswerRecord						m_mapExistAnswer;
public:
	afx_msg void OnBnClickedBtnDmAnswerOk();
	virtual INT_PTR DoModal();
};
