#pragma once
#include <list>
class AFX_EXT_CLASS CSearchDevCom	//ͨ��ע����ȡ��ǰ��ʦ����ѧ���������Ӷ˿�
{
public:
	CSearchDevCom(void);
	~CSearchDevCom(void);
public:
	//��ȡע���õ����е�����Com�Ļ���
	HRESULT SearchDev(std::list<CString> &_lstCom);
};
