#pragma once
#include "../ObjRef/YDObjectRef.h"
#include "../ObjRef/YDLinkRef.h"
class CYDClassRef;
class CPropQueryContidition;
class BASE_DLL_API CYdStudentRef : public CYDObjectRef
{
public:
	CYdStudentRef(CDatabaseEx* pDB);
	~CYdStudentRef(void);
public:
	//得到学生所属班级,学生可能属性多个班级
	HRESULT GetClass(std::list<CYDObjectRef*>* pListClasses, std::list<CYDLinkRef*>* pLinks = NULL);
	

	//根据条件得到对应的学生信息
	static HRESULT GetStudents(CDatabaseEx* pDB,
		std::list<CYdStudentRef*>* _lstref,
		std::list<CPropQueryContidition*>* _lstQueryCondition = NULL);

	//得到性别
	HRESULT GetGender(CString* pStrGender);
	HRESULT SetGender(const CString& pStrGender);
	HRESULT GetStudentByConditionFromDeptAndCls(int _iPage, 
										OBJID deptID, 
										OBJID classID,
										std::list<CPropQueryContidition*> *_lstCondition,
										std::list<CYDObjectRef*>* _lstObjects);
	HRESULT GetStudentTotalNumber(OBJID deptID, 
									OBJID classID,
									std::list<CPropQueryContidition*> *_lstCondition,
									long* _lCount);
	virtual HRESULT Restore(BOOL bReload = FALSE);
	virtual HRESULT Update();
	virtual HRESULT Remove();
protected:
	CYDObjectRef* CreateObject();
	virtual HRESULT OracleSave();
	virtual HRESULT SQLServerSave();
	virtual HRESULT AccessSave();
	virtual HRESULT AddNewRecord();

private:
	HRESULT GetStudentByConditionFromDept(int _iPage, 
											OBJID clsD, 
											std::list<CPropQueryContidition*> *_lstCondition,
											std::list<CYDObjectRef*>* _lstObjects);
	HRESULT GetStudentByConditionFromCls(int _iPage, 
											OBJID deptID, 
											std::list<CPropQueryContidition*> *_lstCondition,
											std::list<CYDObjectRef*>* _lstObjects);

	HRESULT CreateSQLFromQueryConditionByCls(std::list<CPropQueryContidition*> *_lstCondition,
														CString &_strSQL);

	HRESULT GetStudentTotalNumberOfClsr(OBJID classID,
									std::list<CPropQueryContidition*> *_lstCondition,
									long* _lCount);
	HRESULT GetStudentTotalNumberOfDept(OBJID deptID,
								std::list<CPropQueryContidition*> *_lstCondition,
								long* _lCount);
	//得到当前学生所属的考试单元
	HRESULT GetInAddrUnit(std::list<CYDObjectRef*>& _lstUnit);
};
