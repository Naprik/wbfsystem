#pragma once
class CProgressThreadDlg;
class CInvigilateInfo;


ULONG SearchThread(LPVOID pParam);
ULONG BeginExamThread(LPVOID pParam);	//����
ULONG RollingThread(LPVOID pParam);     // �վ�

ULONG RollingThreadByTeachcom(LPVOID pParam);//�Ե���һ����ʦ�������վ��̣߳���RollingThread������
ULONG BeginExamThreadByTeachcom(LPVOID pParam);//�Ե���һ����ʦ�����п����̣߳���BeginExamThread������


ULONG SpecRollThreadByUse(LPVOID pParam,CProgressThreadDlg* pProgressDlg);//���ⵥ��ͨ��USB����

