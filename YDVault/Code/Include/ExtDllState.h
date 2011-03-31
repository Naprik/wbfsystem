
#ifndef		EXTDLLSTATE_H  
#define		EXTDLLSTATE_H
/******************************************************************
** 文件名:	EXTDLLSTATE.h


** 目的及主要功能：	dll中的对话框使用dll的资源
** 创建人:			刘凯
** 创建日期:		2005-7-22
** 修改人:		
** 修改日期:
** 描 述:
** 版 本:


** 说明：在dll中创建这个类的实现，
		如：
		CExtDllState::CExtDllState()
		{
			m_hInstOld = AfxGetResourceHandle(); 
			AfxSetResourceHandle(TestdllDLL.hModule); 
		}

		CExtDllState::~CExtDllState()
		{
			AfxSetResourceHandle(m_hInstOld); 
		}

		//在对话框的实现处：
		Class CMyDialog: public CDialog
		CMyDialog::DoModal()
		{
			CExtDllState state;
			CDialog::DoModal();
		}


******************************************************************/
class CExtDllState  
{
public: 
	CExtDllState(); 
	~CExtDllState(); 
protected: 
	HINSTANCE m_hInstOld; 
	
};

#endif

