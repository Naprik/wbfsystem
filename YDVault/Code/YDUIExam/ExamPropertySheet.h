#pragma once

class CExamPropertySheet
{
public:
	CExamPropertySheet(void);
	~CExamPropertySheet(void);
	HRESULT ShowExam(CString _strText);
private:
	std::map<CString,CFormView*> m_mapView;
};
