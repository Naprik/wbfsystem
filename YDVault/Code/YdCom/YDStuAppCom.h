#pragma once
#include "YDAppCom.h"
#include <list>

class CStudev;

class AFX_EXT_CLASS CYDStuAppCom //ѧ������������Com�ӿ�ͨ�ŵĿ���
	:public CYDAppCom
{
public:
	CYDStuAppCom(CString _strCom);
	~CYDStuAppCom(void);
public:
	//�Եõ��Ĵ𰸽��н��룬��ŵ�ѧ����̬������
	HRESULT    LinkRolling(CString &_str);
	HRESULT    Linksuccess();
	virtual HRESULT    OpenCom();
	virtual HRESULT    Sendexaminfo(); 
	BOOL		m_bBeginExam;//�Ƿ񿪿�
	BOOL		m_bRolling;//�Ƿ񽻾�
	COleDateTime m_timeBeginExam;//����ʱ��
	COleDateTime m_timeRolling;//����ʱ��
public:
	static HRESULT   FindStucom(std::list<CYDStuAppCom*>& _lstStuAppCom);
};
