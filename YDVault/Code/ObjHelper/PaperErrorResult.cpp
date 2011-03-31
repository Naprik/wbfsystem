#include "StdAfx.h"
#include "PaperErrorResult.h"
#include "../ObjRef\YDPaper.h"
#include "../Base\AutoClean.h"

CPaperErrorResult::CPaperErrorResult(void)
{

}

CPaperErrorResult::~CPaperErrorResult(void)
{
	CPtrAutoClean<CYDPaper> clr1(m_pPaper);
}
