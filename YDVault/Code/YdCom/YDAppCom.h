#pragma once
#include "YDCom.h"

/*
	CYDAppCom:������CYDCom��
	��Ҫ�������ƶ˿ڵĶ�д����
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
	//���ص�ǰ������MAC��ַ��_iType=0,˵���ǽ�ʦ��
	//_iType = 1,˵����ѧ����
	HRESULT GetDevMac(CString& _strMac,int& _iType);
	//����,���Ϳ�����Ϣָ�����ʦ����ѧ����
	virtual HRESULT    Sendexaminfo(); 
	virtual HRESULT	   ReadData(CString &_str);
protected:
	//����ָ�iTimeΪʱ���룬��������������û�з�Ӧ���򷵻�_str=Ϊ��
	HRESULT    contrl(CString cmd, int iTime,CString &_str);
	CYDCom	   m_com;
protected:
	HRESULT    Decode(CString input,BYTE *out,int min,int max);
	//������1A2A3BC4A����ʽ�Ĵ�
	HRESULT    TransferAnswer(BYTE* out,CString &_strAnswer);
	//����ȫ�ֱ������ɿ�����Ϣ
	HRESULT    CreateExamInfo(CString& _strExamInfo);
public:
	CString		m_strComPort;	//�˿ں�
	CString		m_strMac;   //Mac��ַ
private:
};