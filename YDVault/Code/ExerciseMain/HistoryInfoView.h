#pragma once

#include "resource.h"
#include "../YDFormUIBase/YdFormView.h"
#include "../UIBase/BCGPSortListCtrl.h"

// CHistoryInfoView form view

class CHistoryInfoView : public CYdFormView
{
	DECLARE_DYNCREATE(CHistoryInfoView)

protected:
	CHistoryInfoView();           // protected constructor used by dynamic creation
	virtual ~CHistoryInfoView();

public:
	enum { IDD = IDD_DLG_PERSIONINFO_HISTORY };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
private:
	CBCGPSortListCtrl	m_ExerciseListCtrl;
	CBCGPSortListCtrl	m_ExamListCtrl;
public:
	virtual void OnInitialUpdate();
	virtual HRESULT UpdateProp(BOOL bUpdate);

private:
	bool InsertExerciseLog();
	bool InsertExamLog();
public:
	afx_msg void OnNMDblclkListExam(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListExercise(NMHDR *pNMHDR, LRESULT *pResult);
};


