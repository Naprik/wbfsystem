#include "stdafx.h"

	CString resetCmd = "81 00 00 00 0d 0d 0a";//
	CString searchCmd = "82 00 00 00 0d 0d 0a"; //
	CString getaddrCmd = "83 00 00 00 0d 0d 0a";
	CString querysendexammessageCmd = "86 00 00 00 0d 0d 0a";//
	CString startexmaCmd = "89 00 00 00 0d 0d 0a";//
	CString statCmd = "8b 00 00 00 0d 0d 0a";
	CString queryexamstatCmd = "a4 00 00 00 0d 0d 0a";
	CString queryrollingCmd = "a3 00 00 00 0d 0d 0a";
	CString getanswerCmd = "8d 00 00 00 0d 0d 0a";
	CString	getteacherdevCmd = "a8 00 00 00 0d 0d 0a";
	CString recvResetCmd = "91 00 00 00 0d 0d 0a ";
	CString recvSearchCmd = "92 00 00 00 0d 0d 0a ";
	CString recvSendlegaladdrCmd = "94 00 00 00 0d 0d 0a ";
	CString	recvSendexammessageCmd = "95 00 00 00 0d 0d 0a ";
	CString recvSendstepexammessageCmd = "96 00 00 00 0d 0d 0a ";
	CString recvUpdatestudev = "97 00 00 00 0d 0d 0a ";
	CString	recvAddmac = "98 00 00 00 0d 0d 0a ";
	CString recvSinglemacsuccess = "99 00 00 00 0d 0d 0a ";
	CString recvSinglemacfail = "9c 00 00 00 0d 0d 0a ";
	CString recvRollingCmd = "9c 00 00 00 0d 0d 0a ";
	CString recvSendsingleinfoCmd = "9a 00 00 00 0d 0d 0a ";

	BOOL    b_isSetexam = FALSE;
	int		i_examMode = 0;