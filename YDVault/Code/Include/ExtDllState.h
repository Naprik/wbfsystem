
#ifndef		EXTDLLSTATE_H  
#define		EXTDLLSTATE_H
/******************************************************************
** �ļ���:	EXTDLLSTATE.h


** Ŀ�ļ���Ҫ���ܣ�	dll�еĶԻ���ʹ��dll����Դ
** ������:			����
** ��������:		2005-7-22
** �޸���:		
** �޸�����:
** �� ��:
** �� ��:


** ˵������dll�д���������ʵ�֣�
		�磺
		CExtDllState::CExtDllState()
		{
			m_hInstOld = AfxGetResourceHandle(); 
			AfxSetResourceHandle(TestdllDLL.hModule); 
		}

		CExtDllState::~CExtDllState()
		{
			AfxSetResourceHandle(m_hInstOld); 
		}

		//�ڶԻ����ʵ�ִ���
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

