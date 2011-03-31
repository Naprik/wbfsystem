#pragma once
#include <afxmt.h>

class CXmlNode;
class CXmlWriter;

//ѧ�����豸
class AFX_EXT_CLASS CStudev  
{
public:
	CStudev();
	virtual ~CStudev();
private:
	CString				m_com;
	int					m_index;					//���
	CString				m_mac;					   //ѧ������ַ
	CString				m_stuid;				   //ѧ��ѧ��
	int					m_rollcall;					//�Ƿ����
	int					m_rolling;                  //�Ƿ񽻾�
	BOOL				m_bFetchAnswer;             //�Ƿ��Ѿ���ȡ��
	BOOL				m_islate;					//�Ƿ�ٵ�
	CString				m_answer;				    //ѧ����
	COleDateTime		m_start;					//��ʼ����ʱ��
	COleDateTime		m_end;						//��������ʱ��
	BOOL				m_islinkroll;               //�ǲ�����������  ����ͨ��USB����
	BOOL				m_islinkroling;             //�ǲ��������վ�����ͨ��USB�վ�
	BOOL				m_bWriteDB;					//�Ƿ��Ѿ�д�����ݿ�
public:
	HRESULT		LoadXml(CXmlNode& _node);
	HRESULT     SaveXml(CXmlWriter& _writer);
public:
	HRESULT		SetCom(CString _strCom);
	HRESULT		GetCom(CString& _strCom);
	HRESULT		SetIndex(int _index);
	HRESULT		GetIndex(int& _index);
	HRESULT		SetMac(CString _strMac);
	HRESULT		GetMac(CString& _strMac);
	HRESULT		SetStuID(CString _strStuID);
	HRESULT		GetSudID(CString& _strStuID);
	HRESULT		SetRollCall(int _iRollCall);
	HRESULT		GetRollCall(int &_iRollCall);
	HRESULT		SetRolling(int _iRolling);
	HRESULT		GetRolling(int &_iRolling);
	HRESULT		SetFetchAnswer(BOOL _bFetchAnswer);
	HRESULT		GetFetchAnswer(BOOL& _bFetchAnswer);
	HRESULT     SetIsLate(BOOL _bIsLate);
	HRESULT		GetIsLate(BOOL &_bIsLate);
	HRESULT		SetAnswer(CString _strAnswer);
	HRESULT     GetAnswer(CString& _strAnswer);
	HRESULT		SetStart(COleDateTime _tStart);
	HRESULT		GetStart(COleDateTime& _tStart);
	HRESULT		SetEnd(COleDateTime _tEnd);
	HRESULT		GetEnd(COleDateTime &_tEnd);
	HRESULT		SetIsLinkRoll(BOOL _bIsLinkRoll);
	HRESULT		GetIsLinkRoll(BOOL& _bIsLinkRoll);
	HRESULT		SetIsLinkRolling(BOOL _bIsLinkRolling);
	HRESULT		GetIsLinkRolling(BOOL& _bIsLinkRolling);
	HRESULT		SetIsWriteDB(BOOL _bWriteDB);
	HRESULT		GetIsWriteDB(BOOL &_bWriteDB);
public:
	CString  Get_Roll_text(); //��ȡ��ǰ����״̬
	CString  Get_Rolling_text(); //��ȡ��ǰ�վ�״̬
	//BOOL     Setanswer(CString strAnswer, int start, int end);
private:
	CCriticalSection m_cs; //�ٽ����ƣ����ڶ��߳��п���
};

