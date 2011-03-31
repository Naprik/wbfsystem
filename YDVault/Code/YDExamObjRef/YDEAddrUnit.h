#pragma once
#include "..\objref\ydobjectref.h"
#include "..\DBBase\DatabaseEx.h"
#include "../YDExamObjRef/YdStudentRef.h"
#include "../YDExamObjRef/YDDevice.h"
#include "../ObjRef/YDPaper.h"

class BASE_DLL_API CYDEAddrUnit : public CYDObjectRef
{
public:
	CYDEAddrUnit(CDatabaseEx* pDB);
	~CYDEAddrUnit(void);

	HRESULT GetStudent(CYdStudentRef*& pStu);

	HRESULT GetDevice(CYDDevice*& pDevice);
	HRESULT GetDeviceMac(CString& _strMac);
	virtual HRESULT Remove();
private:
	//�õ������Ŀ���
	HRESULT GetInAddress(std::list<CYDObjectRef*>* _lstAddress,
				std::list<CYDLinkRef*>* _links = NULL);
};
