#pragma once
#include "../UIBase/BCGPKeyGridCtrl.h"

class CQuestionRecord;
class CQuestionRecordStruct;

// CDlgExsiceMark dialog

class CDlgExsiceMark : public CDialog
{
	DECLARE_DYNAMIC(CDlgExsiceMark)

public:
	CDlgExsiceMark(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgExsiceMark();

// Dialog Data
	enum { IDD = IDD_DLG_EXISICE_MARK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strMark;
	virtual BOOL OnInitDialog();
	CQuestionRecord* m_pQuestionRecord;
private:
	CBCGPKeyGridCtrl	m_listGrid;
	HRESULT InsertList(CQuestionRecordStruct* _pQRecordStuct,
		CBCGPGridRow* _pParentRow,
		CBCGPGridRow*& _pInsertRow,
		int _iItemNo /*Ã‚∫≈*/);

public:
	afx_msg void OnBnClickedOk();
	int		GetAccuracy();
public:
	int m_accuracy;
	CString m_strLevel;
};
