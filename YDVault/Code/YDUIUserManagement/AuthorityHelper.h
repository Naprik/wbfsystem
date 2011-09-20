#pragma once

class BASE_DLL_API CAuthorityHelper
{
public:
	CAuthorityHelper(void);
	~CAuthorityHelper(void);
public:
	//����Ȩ��ֵ�ж�Ȩ��
 	HRESULT ConvertByVal(long _lAuthority,
 						BOOL& _bVault,/*��������*/
						BOOL& _bPaper,/*�Ծ����*/
						BOOL& _bOperate,/*���Բ���*/
						BOOL& _bBlue,/*����*/
						BOOL& _bRedOut,/*����*/
						BOOL& _bDataMaintain/*����ά��*/,
						BOOL& _bLogin/*��¼*/);
	//����Ȩ��ֵ
	HRESULT CreateVal(BOOL _bVault,/*��������*/
		BOOL _bPaper,/*�Ծ����*/
		BOOL _bOperate,/*���Բ���*/
		BOOL& _bBlue,/*����*/
		BOOL& _bRedOut,/*����*/
		BOOL _bDataMaintain/*����ά��*/,
		BOOL& _bLogin,
		long& _lAuthority);
	//����Ȩ��ֵ�����ַ�������ʾ���û�����
	HRESULT ConvertString(long _lAuthority,
						  CString& _strVal);
	//�жϵ�ǰ�û����в���_type��Ȩ��
	HRESULT CheckSecurity(SECURITY_TYPE _type,VARIANT_BOOL* _bRight);
};
