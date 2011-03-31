// PtrAutoClean.h: interface for the CPtrAutoClean class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PTRAUTOCLEAN_H__77C964A5_17A4_44D5_840C_2393CFA1B17B__INCLUDED_)
#define AFX_PTRAUTOCLEAN_H__77C964A5_17A4_44D5_840C_2393CFA1B17B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)
#include <list>
#include <afxTempl.h>

//ָ���ڴ��Զ��ͷ� T* p = new T;
template<class T>
class CPtrAutoClean  
{
public:
	CPtrAutoClean(T*& p):m_p(p){}
	virtual ~CPtrAutoClean()
	{
		if(m_p!=NULL)
		{
			delete m_p;
			m_p = NULL;
		}
	}

private:
	CPtrAutoClean(){}
	CPtrAutoClean(const CPtrAutoClean&){}

private:
	T*   m_p;
};

//�����ڴ��Զ��ͷ� T* p = new T[];
template<class T>
class CArrayAutoClean  
{
public:
	CArrayAutoClean(T*& p):m_p(p){}
	virtual ~CArrayAutoClean()
	{
		if(m_p != NULL)
		{
			delete[] m_p;
			m_p = NULL;
		}
	}
	
private:
	CArrayAutoClean(){}
	CArrayAutoClean(const CArrayAutoClean&){}
	
private:
	T*   m_p;
};

//��������ָ����Զ��ͷ�
template<class T>
class CRefPtrAutoClean  
{
public:
	CRefPtrAutoClean(T*& p):m_p(p){}
	virtual ~CRefPtrAutoClean()
	{
		if(m_p != NULL)
		{
			m_p->Release();
			m_p = NULL;
		}
	}
	
private:
	CRefPtrAutoClean(){}
	CRefPtrAutoClean(const CRefPtrAutoClean&){}
	
private:
	T*   m_p;
};

//�б���Զ��ͷ�std::list<T*>
//ע�⣺ֻ�ͷ��б�����ݣ��������Ϊ�ӿڣ�����б�Ϊnew std::list<T*>���������಻�����ͷ�
template<class T>
class CRefListAutoClean  
{
public:
	CRefListAutoClean(std::list<T*>& lst):m_list(lst){}
	virtual ~CRefListAutoClean()
	{
		std::list<T*>::iterator itr;
		for(itr=m_list.begin();itr!=m_list.end();++itr)
		{
			_ASSERT(*itr != NULL);
			(*itr)->Release();
			*itr = NULL;
		}
		m_list.clear();
	}
	
private:
	CRefListAutoClean(){}
	CRefListAutoClean(const CRefListAutoClean&){}
	
private:
	std::list<T*>&   m_list;
};


//�б���Զ��ͷ�std::list<T*>
//ע�⣺ֻ�ͷ��б�����ݣ�����б�Ϊnew std::list<T*>���������಻�����ͷ�
template<class T>
class CListAutoClean  
{
public:
	CListAutoClean(std::list<T*>& lst):m_list(lst){}
	virtual ~CListAutoClean()
	{
		std::list<T*>::iterator itr;
		for(itr=m_list.begin();itr!=m_list.end();++itr)
		{
			_ASSERT(*itr != NULL);
			delete (T*)*itr;
			*itr = NULL;
		}
		m_list.clear();
	}
	
private:
	CListAutoClean(){}
	CListAutoClean(const CListAutoClean&){}
	
private:
	std::list<T*>&   m_list;
};

//�б���Զ��ͷ�CPtrList<T*>
//ע�⣺ֻ�ͷ��б�����ݣ�����б�Ϊnew CPtrList<T*>���������಻�����ͷ�
template<class T>
class CPtrListAutoClean  
{
public:
	CPtrListAutoClean(CPtrList& lst):m_list(list){}
	virtual ~CPtrListAutoClean()
	{
		POSITION pos = m_list.GetHeadPosition();
		while(pos != NULL)
		{
			T* p = (T*)m_list.GetNext(pos);
			delete p;
			p = NULL;
		}
		m_list.RemoveAll();
	}
	
private:
	CPtrListAutoClean(){}
	CPtrListAutoClean(const CPtrListAutoClean&){}
	
private:
	CPtrList&   m_list;
};

class CVARIANTArrayAutoClean  
{
public:
	CVARIANTArrayAutoClean(VARIANT*& pVar,int nNum):m_pVar(pVar),m_nNum(nNum){}
	virtual ~CVARIANTArrayAutoClean()
	{
		if(m_pVar == NULL || m_nNum == 0) return;
		for(int i=0;i<m_nNum;i++)
		{
			VariantClear(&m_pVar[i]);
		}
		delete []m_pVar;
	}
	
private:
	CVARIANTArrayAutoClean(){}
	CVARIANTArrayAutoClean(const CVARIANTArrayAutoClean&){}
	
private:
	VARIANT* m_pVar;
	int      m_nNum;
};

#endif // !defined(AFX_PTRAUTOCLEAN_H__77C964A5_17A4_44D5_840C_2393CFA1B17B__INCLUDED_)
