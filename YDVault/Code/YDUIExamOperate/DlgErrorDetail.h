#pragma once
#include "afxcmn.h"
#include "resource.h"
#include "../UIBase\BCGPSortListCtrl.h"
#include "../UIBase/BCGPKeyGridCtrl.h"

// CDlgErrorDetail dialog
class CPaperErrorResult;
class CYDStuMark;
class CDlgErrorDetail : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgErrorDetail)

public:
	CDlgErrorDetail(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgErrorDetail();

// Dialog Data
	enum { IDD = IDD_YDUIEXAM_DLG_ERROR_DETAIL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	CPaperErrorResult* m_pResult;
	virtual INT_PTR DoModal();
private:
	CBCGPKeyGridCtrl						m_Grid;
	HRESULT InsertListByStuMark(CYDStuMark* _pMark);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
