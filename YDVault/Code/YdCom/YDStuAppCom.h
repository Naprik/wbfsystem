#pragma once
#include "YDAppCom.h"
#include <list>

class CStudev;

class AFX_EXT_CLASS CYDStuAppCom //学生机，用来与Com接口通信的控制
	:public CYDAppCom
{
public:
	CYDStuAppCom(CString _strCom);
	~CYDStuAppCom(void);
public:
	//对得到的答案进行解码，存放到学生动态数组中
	HRESULT    LinkRolling(CString &_str);
	HRESULT    Linksuccess();
	virtual HRESULT    OpenCom();
	virtual HRESULT    Sendexaminfo(); 
	BOOL		m_bBeginExam;//是否开考
	BOOL		m_bRolling;//是否交卷
	COleDateTime m_timeBeginExam;//开考时间
	COleDateTime m_timeRolling;//交卷时间
public:
	static HRESULT   FindStucom(std::list<CYDStuAppCom*>& _lstStuAppCom);
};
