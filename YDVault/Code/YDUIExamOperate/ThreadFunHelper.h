#pragma once

class CStudev;
class CDatabaseEx;
class CInvigilateInfo;
class CFormExamBlue;

//�����޸�studev��Ϣ�Ķ�������������У���Ϊ�߳��޸�Ҫ����ͨ���ٽ��
class CThreadFunHelper//�߳��л���������һЩ����
{
public:
	CThreadFunHelper(void);
	~CThreadFunHelper(void);
public:
	//�����Ǹ���_pStuDev�޸��б��ϵ�״̬
	//bError = True:��ʾ�������ɹ�ҲҪ��ʾ״̬�����򣺿������ɹ�����ʾ
	HRESULT UpdateListBeginExamByStudev(CStudev* _pStuDev,CFormExamBlue* _pForm,BOOL bError);
	//����ʱ����_pStuDev�޸��б��ϵ�״̬
	HRESULT UpdateListRollingByStudev(CStudev* _pStuDev,CFormExamBlue* _pForm,BOOL bError);
	//����ʱ����𰸵����ݿ���
	HRESULT SaveStudevAnswer(CStudev* _pStuDev,CFormExamBlue* _pForm,
		CInvigilateInfo* _pInvigilateInfo,CDatabaseEx* _pDB);
	//�����Ѿ�������ѧ��������
	HRESULT GetBeginExamStudev(CFormExamBlue* _pForm,std::list<CStudev*>& _lstStudev);

	//�õ��Ѿ������ѧ����
	HRESULT GetRollingStudev(CFormExamBlue* _pForm,std::list<CStudev*>& _lstStudev);

};
