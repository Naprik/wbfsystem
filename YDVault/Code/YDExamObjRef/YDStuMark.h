#pragma once
#include "..\objref\ydobjectref.h"
#include "..\DBBase\DatabaseEx.h"
#include "../YDExamObjRef/YdStudentRef.h"
#include "../YDExamObjRef/YDDevice.h"
#include "../ObjRef/YDPaper.h"
#include "../YDExamObjRef/YDSubjectRef.h"

class BASE_DLL_API CYDStuMark : public CYDObjectRef
{
public:
	CYDStuMark(CDatabaseEx* pDB);
	~CYDStuMark(void);

	HRESULT GetStudent(CYdStudentRef*& pStu);
	HRESULT GetPaper(CYDPaper*& pPaper);
	HRESULT GetDevice(CYDDevice*& pDevice);
	HRESULT GetSubject(CYDSubjectRef*& pSubject);

	HRESULT CalMark(double* pMark);
	HRESULT CalMark(std::list<CYDObjectRef*>* pLstStdAns, double* pMark);
	HRESULT CalMarkByString(double* pMark);
	HRESULT CalMarkByString(std::list<CYDObjectRef*>* pLstStdAns, double* pMark);

	HRESULT GetCountBySelfProp(std::list<CPropQueryContidition*> *_lstCondition, long* pCount);
	HRESULT GetUnitsBySelfProp(int _iPage,
		std::list<CPropQueryContidition*> *_lstCondition, 
		std::list<CYDObjectRef*>* pListObjs);

	HRESULT GetCountByStu(std::list<CPropQueryContidition*> *_lstCondition, long* pCount);
	HRESULT GetUnitsByStu(int _iPage,
		std::list<CPropQueryContidition*> *_lstCondition, 
		std::list<CYDObjectRef*>* pListObjs);

	HRESULT GetCountByCls(std::list<OBJID> *_lstClass,std::list<CPropQueryContidition*> *_lstCondition, long* pCount);
	HRESULT GetUnitsByCls(int _iPage,
		std::list<OBJID> *_lstClass, 
		std::list<CPropQueryContidition*> *_lstCondition, 
		std::list<CYDObjectRef*>* pListObjs);
	virtual HRESULT GetObjectTotalNumber(std::list<CPropQueryContidition*> *_lstCondition,
		long* _lCount);
	HRESULT CreateParamter(std::list<CPropQueryContidition*> *_lstCondition);
protected:
	CYDObjectRef* CreateObject();
	HRESULT CreateSQLFromQueryCondition(std::list<CPropQueryContidition*> *_lstCondition,
		CString &_strSQL);

private:
	HRESULT CreateSQLFromQueryConditionByStu(std::list<CPropQueryContidition*> *_lstCondition,
		CString &_strSQL);

	HRESULT CreateSQLFromQueryConditionByCls(std::list<OBJID> *_lstClass, std::list<CPropQueryContidition*> *_lstCondition,
		CString &_strSQL);
public:
	HRESULT TransferAnswer(std::map<int, CString>* pMapAnswer);
	HRESULT TransferAnswerByString(std::map<int, CString>* pMapAnswer);//直接从属性中解析，不读数据库
};
