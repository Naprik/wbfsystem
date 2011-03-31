#pragma once
#include "ydobjectref.h"

class BASE_DLL_API CYDMediaRef :
	public CYDObjectRef
{
public:
	CYDMediaRef(CDatabaseEx* pDB);
	~CYDMediaRef(void);

public:
	virtual HRESULT GetLabel(CString* pStr);
	HRESULT CreateVaultName(CString& _strVaultName);//�õ��ڿ���������ļ���,/Vault/+MP3/GUID�ļ���
	virtual HRESULT Remove();
};
