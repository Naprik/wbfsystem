#pragma once
#include "stdafx.h"
#include "Resource.h"
#include "../UIBase/BCGPSortListCtrl.h"
#include "../ObjRef/PropQueryContidition.h"
#include "../include/YDVaultComm.h"
#include "../UIBase/BCGPKeyGridCtrl.h"

// CQueryByStudent dialog

class CQueryByStudent : public CDialog
{
	DECLARE_DYNAMIC(CQueryByStudent)

public:
	CQueryByStudent(CWnd* pParent = NULL);   // standard constructor
	virtual ~CQueryByStudent();

// Dialog Data
	enum { IDD = IDD_YDUIEXAM_DATA_MAINTAIN_DLG_QMARKBYSTU_SHEET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual INT_PTR DoModal();
	virtual BOOL OnInitDialog();

private:
	CBCGPKeyGridCtrl						m_QueryGrid;
	std::list<CQueryProp*>				m_lstQueryProp;
public:
	afx_msg void OnBnClickedBtnDmStuAddqc();
	afx_msg void OnBnClickedBtnDmStuDelqc();
	afx_msg void OnBnClickedBtnDmStuClearqc();
	HRESULT CreatePropQueryContidion(std::list<CPropQueryContidition*>& lstPropQuery, BOOL* pbFromStu);

private:
	HRESULT CreateQuestionQueryProp();
};
