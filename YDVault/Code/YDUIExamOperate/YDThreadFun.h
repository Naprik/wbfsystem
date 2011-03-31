#pragma once
class CProgressThreadDlg;
class CInvigilateInfo;


ULONG SearchThread(LPVOID pParam);
ULONG BeginExamThread(LPVOID pParam);	//开考
ULONG RollingThread(LPVOID pParam);     // 收卷

ULONG RollingThreadByTeachcom(LPVOID pParam);//对单独一个教师机进行收卷线程，在RollingThread中启动
ULONG BeginExamThreadByTeachcom(LPVOID pParam);//对单独一个教师机进行开考线程，在BeginExamThread中启动


ULONG SpecRollThreadByUse(LPVOID pParam,CProgressThreadDlg* pProgressDlg);//特殊单独通过USB点名

