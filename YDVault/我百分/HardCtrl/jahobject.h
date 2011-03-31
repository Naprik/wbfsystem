/*************************************************************************
������    CJahObject
���ʱ�䣺2004-9-10
���ߣ�    л��ΰ
���ࣺ    ��
�ؼ��֣�  ϵͳ��������
����˵�����Ե�ǰ�ڴ��еĶ�����й����õ�����ʱ�������������С��ͷָ��
ʹ��˵����
          
ע��㣺  ֻ���ɱ��������Ķ�����ã�
          ������Ķ�������ڶ����ʱ����϶��������ĺ�
		  DECLARE_DYNCREATE(CJahObject)        
		  ��ʵ���ļ��м���,�����ĺ�
		  IMPLEMENT_DYNCREATE(CJahObject, CObject)
*************************************************************************/
//##ModelId=4142B5B20002
#pragma once
class CJahObject : public CObject  
{
public:
	DECLARE_DYNCREATE(CJahObject)        //OBJ����
	//##ModelId=4142B5B20042
	CJahObject();
	//##ModelId=4142B5B20041
	~CJahObject();	
public:
	//##ModelId=4142B5B20040
	int             GetObjSize();        //�õ������С
	//##ModelId=4142B5B2003F
	CString         GetObjName();        //�õ����������
	//##ModelId=4142B5B2003E
	CRuntimeClass * GetObjClass();       //�õ���ǰ���е���ָ���磺RUNTIME_CLASS(CJahObject)
};
