#pragma once

class BASE_DLL_API CCrypt
{
public:
	CCrypt(void);
	~CCrypt(void);
public:
	CString Encrypt(CString S, WORD Key); // ���ܺ���
	CString Decrypt(CString S, WORD Key); // ���ܺ���
};
