#include "stdafx.h"
#include "ThreadTeacherComJump.h"
#include "../YdCom\SearchDevCom.h"

ULONG JumpTeacherComThread(LPVOID pParam)
{
	//������ͣ�ļ���ʦ��״̬
	HRESULT hr = E_FAIL;
	while(TRUE)
	{
		CGExamStruct* pGExamStruct = NULL;
		hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return -1;
		}
		if(!pGExamStruct->m_bThreadJump)
		{
			return 0L;//�߳��˳�
		}
		CSearchDevCom SearchDevCom;
		std::list<CString> lstSearchedCom;
		hr = SearchDevCom.SearchDev(lstSearchedCom);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return -1;
		}
		for(std::list<CYDTeacherAppCom*>::const_iterator itr = pGExamStruct->m_lstTacherAppCom.begin();
			itr != pGExamStruct->m_lstTacherAppCom.end();++itr)
		{
			BOOL bOpen = FALSE;
			hr = (*itr)->GetState(bOpen);
			if(FAILED(hr))
			{
				DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
				return -1;
			}
			if(!bOpen)
			{
				continue;
			}
			BOOL bFind = FALSE;
			CString strItrCom = (*itr)->m_strComPort;
			for(std::list<CString>::const_iterator itrCom = lstSearchedCom.begin();
				itrCom != lstSearchedCom.end();++itrCom)
			{
				CString strPort = (*itrCom);
				if (strPort.GetLength() > 4) //����˿ںŴ���"com10"����Ҫ�ڶ˿�����ǰ�����ӡ�\\\\.\\��
				{
					strPort = _T("\\\\.\\") + strPort;
				}
				if(strItrCom.CompareNoCase(strPort) == 0)
				{
					bFind = TRUE;
					break;
				}
			}
			if(!bFind)
			{
				AfxGetMainWnd()->SendMessage(WM_YD_TEACHERCOM_BREAK,WPARAM(*itr));
				break;
			}
		}
		Sleep(500);
	}
	return 0;
}