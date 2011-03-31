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
	//�õ�ѧ�������༶,ѧ���������Զ���༶
	HRESULT GetClass(std::list<CYDObjectRef*>* pListClasses, std::list<CYDLinkRef*>* pLinks = NULL);
	

	//���������õ���Ӧ��ѧ����Ϣ
	static HRESULT GetStudents(CDatabaseEx* pDB,
		std::list<CYdStudentRef*>* _lstref,
		std::list<CPropQueryContidition*>* _lstQueryCondition = NULL);

	//�õ��Ա�
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
	//�õ���ǰѧ�������Ŀ��Ե�Ԫ
	HRESULT GetInAddrUnit(std::list<CYDObjectRef*>& _lstUnit);
};
