#pragma once


class CYDProgressControl //�����������ƽ�������ʾ����ʾ��Ϣ
{
public:
	CYDProgressControl(CWnd* _pWnd,BOOL bShowCancel = FALSE);
	~CYDProgressControl(void);
public:
	//���bTimeΪTrue������Զ�����ʱ�������ƽ�����
	HRESULT SetRange(UINT uRange,BOOL bTime = FALSE);
	HRESULT StepIt();
private:
	CWnd*	m_pWnd;
};
