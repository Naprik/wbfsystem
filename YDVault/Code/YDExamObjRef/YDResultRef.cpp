#include "StdAfx.h"
#include "YDResultRef.h"

CYDResultRef::CYDResultRef(CDatabaseEx* pDB)
:CYDObjectRef(pDB)
{
	m_strDBName =  DB_YDRESULT ;//ѧ������Ϣ��
	AddPropDef(FIELD_YDRESULT_STUDID,VT_I4);//ѧ����Ϣ��OBJID
	AddPropDef(FIELD_YDRESULT_PAPERID,VT_I4);//�Ծ���
	AddPropDef(FIELD_YDRESULT_START,VT_DATE);//����ʱ��
	AddPropDef(FIELD_YDRESULT_END,VT_DATE);//����ʱ��
	AddPropDef(FIELD_YDRESULT_ASNSWER,VT_BSTR); //ѧ����
	AddPropDef(FIELD_YDRESULT_ADDRID,VT_BSTR); //�������

}

CYDResultRef::~CYDResultRef(void)
{
}
