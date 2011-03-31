#pragma once
#include "YDCom.h"

class CYDAppCom
{
public:
	CYDAppCom(void);
	~CYDAppCom(void);

	BOOL Close();
	int TestCom(CString &strCom, CString &strMac);

	BOOL OpenCome(CString strCom);

	BOOL GetStuMac(CString &strMac);
	BOOL SendStuNo(int iNo);
	BOOL DecodeAnswer(CString strRecv, CString &strAnswer, CString &strMac);

	CString ReadData();

private:
	CString		contrl(CString cmd, int iTime);
	CYDCom		m_com;
	CString		DectoHexstr(int iDec);
	CString		FormatLength(CString strHexMaclength);
	BOOL        proofdata(CString strData);
	HRESULT		TransferAnswer(BYTE* out,CString &_strAnswer);

protected:
	HRESULT    Decode(CString input,BYTE *out,int min,int max);
	int		   String2Hex(CString str, char *SendOut);
	TCHAR	   ConvertHexData(TCHAR ch);
	CString	   FormatNO(CString strNO);
	int		   Str2Dec(CString str);

};
