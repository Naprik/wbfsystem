#pragma once
#include "Stuanswer.h"

class CStudev  
{
public:
	CStudev();
	virtual ~CStudev();

	int					m_index;					//���
	char				m_mac[20];					//ѧ������ַ
	char				m_stuid[20];				//ѧ��ѧ��
	int					m_rollcall;					//�Ƿ����
	int					m_rolling;                  //�Ƿ񽻾�
	BOOL				m_islate;					//�Ƿ�ٵ�
	//CArray<CStuanswer, CStuanswer>  m_answer;
	char				m_answer[500];				//ѧ����
	COleDateTime		m_start;					//��ʼ����ʱ��
	COleDateTime		m_end;						//��������ʱ��
	BOOL				m_islinkroll;
	BOOL				m_islinkroling;

public:
	CString  Get_Roll_text();
	CString  Get_Rolling_text();
	//BOOL     Setanswer(CString strAnswer, int start, int end);
};

