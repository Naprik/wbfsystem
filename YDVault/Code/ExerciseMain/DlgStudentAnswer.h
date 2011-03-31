#pragma once
#include "../UIBase/BCGPKeyGridCtrl.h"

class CQuestionRecord;
class CQuestionRecordStruct;

// CDlgStudentAnswer dialog

class CDlgStudentAnswer : public CDialog
{
	DECLARE_DYNAMIC(CDlgStudentAnswer)

public:
	CDlgStudentAnswer(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgStudentAnswer();

// Dialog Data
	enum { IDD = IDD_DIALOG_STUDENT_ANSWER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CQuestionRecord* m_pQuestionRecord;
private:
	CBCGPKeyGridCtrl	m_listGrid;
	HRESULT InsertList(CQuestionRecordStruct* _pQRecordStuct,
		CBCGPGridRow* _pParentRow,
		CBCGPGridRow*& _pInsertRow);
};
