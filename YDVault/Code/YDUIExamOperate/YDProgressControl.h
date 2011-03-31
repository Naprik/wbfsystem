#pragma once


class CYDProgressControl //点名用来控制进度条显示和提示信息
{
public:
	CYDProgressControl(CWnd* _pWnd,BOOL bShowCancel = FALSE);
	~CYDProgressControl(void);
public:
	//如果bTime为True，则会自动根据时间来控制进度条
	HRESULT SetRange(UINT uRange,BOOL bTime = FALSE);
	HRESULT StepIt();
private:
	CWnd*	m_pWnd;
};
