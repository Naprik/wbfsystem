#pragma once
#include <list>
#include <map>
#include "../ObjRef/YDObjectRef.h"

class CPaperErrorResult;
class CStudev;

class BASE_DLL_API CPaperAnsysHelperResult
{
public:
	UINT	m_iRight; //��ȷ������
	UINT	m_iError; //���������
	std::map<CString, UINT>	m_mapAnswer; //���ִ𰸵ĸ���
	CString	m_strRightAnswer;//��׼��
};


class BASE_DLL_API CPaperAnsysHelper
{
public:
	CPaperAnsysHelper(void);
	~CPaperAnsysHelper(void);

	//�������ݿ��е����ݷ����Ծ�
	HRESULT Ansys(OBJID paperID, 
		std::list<CYDObjectRef*>& lstMarks, 
		std::map<UINT, CPaperAnsysHelperResult*>& mapRes);

	//���ӽ�ʦ���д�Ĵ���Ϣ�����Ծ�����
	HRESULT Ansys(OBJID paperID, 
		std::list<CString>& lstStrAnswer,  
		std::map<UINT, CPaperAnsysHelperResult*>& mapRes);

	HRESULT ErrorReport(std::list<CYDObjectRef*>& lstMarks,
		std::list<CPaperErrorResult*>& _lstErrorResult);

private:
	HRESULT AnsysOne(CYDObjectRef* pObjAnswer, 
		std::list<CYDObjectRef*>& lstMarks, 
		std::map<UINT, CPaperAnsysHelperResult*>& mapRes);
	HRESULT AnsysOne(CYDObjectRef* pObjAnswer, 
		std::list<CString>& lststrAnswer,  
		std::map<UINT, CPaperAnsysHelperResult*>& mapRes);
	HRESULT AnsysOne(CYDObjectRef* pObjAnswer,//��׼��
					std::map<int, CString> mapAnswer,/*�����𰸵�ӳ��*/
					std::map<UINT, CPaperAnsysHelperResult*>& mapRes/*�����Ľ��*/);
	HRESULT CreateAnsysHelperResultByPaperID(OBJID paperID, 
											std::list<CYDObjectRef*> &lstAnswer,
											std::map<UINT, CPaperAnsysHelperResult*>& mapRes);
};
