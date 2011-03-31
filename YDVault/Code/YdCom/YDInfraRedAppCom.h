#pragma once
#include "YDAppCom.h"
#include <list>
#include <afxmt.h>



//����Ŀ��ԵĻ���
class AFX_EXT_CLASS CYDInfraRedAppCom
	:public CYDAppCom
{
public:
	CYDInfraRedAppCom(CString _strCom);
	virtual ~CYDInfraRedAppCom(void);
public:
	HRESULT DecodData(std::list<std::pair<CString,CString> > &_lstMacAnswer);//��ȡ�𰸣�������
	HRESULT ReadData();
	virtual HRESULT	   ReadData(CString &_str);        
private:
	HRESULT DecodeAnswer(CString _str,CString &_strAnswer,CString &_strMac);
	HRESULT ValidateData(CString _strData,VARIANT_BOOL* _bValid);
	int		Str2Dec(CString str);
	CString	   FormatNO(CString strNO);
protected:
	CString		m_strReadData;
	CCriticalSection m_cs;
};
