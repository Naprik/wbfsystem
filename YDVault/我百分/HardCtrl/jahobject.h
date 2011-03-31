/*************************************************************************
类名：    CJahObject
完成时间：2004-9-10
作者：    谢文伟
父类：    无
关键字：  系统，对象，类
功能说明：对当前内存中的对象进行管理，得到运行时的类名、对象大小，头指针
使用说明：
          
注意点：  只对由本类派生的对象可用，
          派生类的定义必须在定义的时候加上对象申明的宏
		  DECLARE_DYNCREATE(CJahObject)        
		  在实现文件中加上,创建的宏
		  IMPLEMENT_DYNCREATE(CJahObject, CObject)
*************************************************************************/
//##ModelId=4142B5B20002
#pragma once
class CJahObject : public CObject  
{
public:
	DECLARE_DYNCREATE(CJahObject)        //OBJ申明
	//##ModelId=4142B5B20042
	CJahObject();
	//##ModelId=4142B5B20041
	~CJahObject();	
public:
	//##ModelId=4142B5B20040
	int             GetObjSize();        //得到对象大小
	//##ModelId=4142B5B2003F
	CString         GetObjName();        //得到对象的类名
	//##ModelId=4142B5B2003E
	CRuntimeClass * GetObjClass();       //得到当前运行的类指针如：RUNTIME_CLASS(CJahObject)
};
