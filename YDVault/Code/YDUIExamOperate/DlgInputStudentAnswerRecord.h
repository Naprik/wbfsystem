#pragma once
#include "../uibase/BCGPKeyGridCtrl.h"
#include "Resource.h"


class CInputStudentAnswerRecord;


// CDlgInputStudentAnswerRecord dialog

class CDlgInputStudentAnswerRecord : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgInputStudentAnswerRecord)

public:
	CDlgInputStudentAnswerRecord(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgInputStudentAnswerRecord();

// Dialog Data
	enum { IDD = IDD_YDUIEXAM_DLG_INPUT_STUDENT_ANSWER_RECORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CBCGPKeyGridCtrl						m_Grid;
public:
	std::list<CInputStudentAnswerRecord*> *m_plstRecord;
	afx_msg void OnBnClickedOk();
private:
	HRESULT ValidateIsExist(CDatabaseEx* _pDB,int iRow,VARIANT_BOOL* _bValid);
};
