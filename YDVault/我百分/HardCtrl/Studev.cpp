#include "stdafx.h"
#include "hardctrl.h"
#include "Studev.h"


CStudev::CStudev()
{
	m_index = -1;					//���
	memset(m_mac, 0, sizeof(m_mac));
	memset(m_stuid, 0, sizeof(m_stuid));
	m_rollcall = -1;
	m_rolling = -1;
	m_islate = FALSE;
	memset(m_answer, 0, sizeof(m_answer));
	m_islinkroll = FALSE;
	m_islinkroling = FALSE;
}

CStudev::~CStudev()
{
	
}

CString  CStudev::Get_Roll_text()
{
	if(m_rollcall == 0)
		return "δ����";
	else if(m_rollcall == 1)
		return "���뿼��״̬";
	else if(m_rollcall == 2)
		return  "����ʧ��";
	else
		return "���뿼��״̬(�ٵ�)";
}

CString CStudev::Get_Rolling_text()
{
	if(m_rolling == 0)
		return "δ�վ�";
	else if(m_rolling == 1)
		return "���վ�";
	else if(m_rolling == 2)
		return "�վ�ʧ��";
	else
		return "���վ�(��ǰ����)";
}

/*BOOL CStudev::Setanswer(CString strAnswer, int start, int end)
{
	for(int i = 0; i < strAnswer.GetLength(); i++)
	{
		CStuanswer  sa;
		memset(sa.m_key, 0, sizeof(sa.m_key));
		strcpy(sa.m_key, strAnswer.Mid(i,i+1));
		sa.m_no = start;
		start++;
		m_answer.Add(sa);
	}
	return TRUE;
}*/
