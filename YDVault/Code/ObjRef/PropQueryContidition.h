#pragma once

class CDatabaseEx;
class BASE_DLL_API CPropQueryContidition
{
public:
	CPropQueryContidition(void);
	~CPropQueryContidition(void);
public:
	CString			m_strFiledName;//�ֶ���
	UINT			m_uFieldType;//�ֶε�����
	QUERYOPERATE	m_uOpType;  //�����ıȽϣ��е��ڣ������ںͰ�����
	CString			m_strConVal;//����ֵ

	HRESULT	CreateContidition(CString &_strContidition);//�����������ʽ
	HRESULT CreateParamter(CDatabaseEx* _pDB);
};
