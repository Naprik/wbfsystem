#pragma once
#include "YDCom.h"

/*
	CYDAppCom:包括了CYDCom：
	主要用来控制端口的读写命令
*/
class AFX_EXT_CLASS CYDAppCom
{
public:
	CYDAppCom(CString _strComPort);
	virtual ~CYDAppCom(void);
public:
	virtual HRESULT    OpenCom(DWORD _dwBaudRate = 9600);
	virtual HRESULT    Close();
	HRESULT GetState(BOOL &_bOpen);
	//返回当前机器的MAC地址，_iType=0,说明是教师机
	//_iType = 1,说明是学生机
	HRESULT GetDevMac(CString& _strMac,int& _iType);
	//开考,发送考试信息指令给教师机或学生机
	virtual HRESULT    Sendexaminfo(); 
	virtual HRESULT	   ReadData(CString &_str);
protected:
	//发送指令，iTime为时间秒，如果在这个秒数内没有反应，则返回_str=为空
	HRESULT    contrl(CString cmd, int iTime,CString &_str);
	CYDCom	   m_com;
protected:
	HRESULT    Decode(CString input,BYTE *out,int min,int max);
	//解析成1A2A3BC4A的形式的答案
	HRESULT    TransferAnswer(BYTE* out,CString &_strAnswer);
	//根据全局变量生成考试信息
	HRESULT    CreateExamInfo(CString& _strExamInfo);
public:
	CString		m_strComPort;	//端口号
	CString		m_strMac;   //Mac地址
private:
};