#pragma once

class CFormExamBlue;

class CBOOLControl
{
public:
	CBOOLControl(BOOL* _pbVal)
	{
		m_pbVal = _pbVal;
	}
	~CBOOLControl()
	{
		*m_pbVal = TRUE;
	}
private:
	BOOL* m_pbVal;
};
class CThreadFunOperate
{
public:
	CThreadFunOperate(CFormExamBlue* _pForm);
	~CThreadFunOperate(void);
public:
	HRESULT SearchThread();
	HRESULT BeginExamThread();
	HRESULT RollingThread();
private:
	CFormExamBlue*	m_pForm;
};
