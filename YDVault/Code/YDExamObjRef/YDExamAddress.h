#pragma once
#include "../ObjRef/YDObjectRef.h"
#include "../ObjRef/YDLinkRef.h"


//������Ϣ
class BASE_DLL_API CYDExamAddress :
	public CYDObjectRef
{
public:
	CYDExamAddress(CDatabaseEx* pDB);
	~CYDExamAddress(void);

	
	//�õ����������п��Ե�Ԫ
	HRESULT GetAllUnits(std::list<CYDObjectRef*>* _lstUnits, std::list<CYDLinkRef*>* _links = NULL);
	HRESULT Remove();

protected:
	CYDObjectRef* CreateObject();
};
