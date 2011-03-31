#include "stdafx.h"
#include "FormImageread.h"
#include "../Base\AutoClean.h"


IMPLEMENT_DYNCREATE(CFormImageRead, CYdFormView)
int * psaved = NULL;

CFormImageRead::CFormImageRead()
	: CYdFormView(CFormImageRead::IDD)
{
	//{{AFX_DATA_INIT(CFormImageRead)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pBmpDat = NULL;
}

CFormImageRead::~CFormImageRead()
{
	CPtrAutoClean<CBmpDat> clr(m_pBmpDat);
}

void CFormImageRead::DoDataExchange(CDataExchange* pDX)
{
	CYdFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormImageRead)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormImageRead, CYdFormView)
	//{{AFX_MSG_MAP(CFormImageRead)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_OPNE, OnOpne)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_BN_CLICKED(IDC_REVO, OnRevo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormImageRead message handlers

void CFormImageRead::OnInitialUpdate() 
{
	CYdFormView::OnInitialUpdate();
	
	CPaintDC dc(this);
	m_pBmpDat = new CBmpDat(&dc);
	m_pBmpDat->New(800,400);

	saved = -1;
	contyp = 0;
	cnum = 6;
	lbd = FALSE;
	rbd = FALSE;
	tx[5] = 0;
	psaved = &saved;	
	
	return ;  
}

void CFormImageRead::OnPaint() 
{
	if(m_pBmpDat)
	{
		CPaintDC dc(this);
		dc.BitBlt(100,0,
			m_pBmpDat->GetW(),
			m_pBmpDat->GetH(),
			m_pBmpDat->GetDC(),
			0,0,SRCCOPY);
	}
	
}

void CFormImageRead::OnOpne() 
{
	CPaintDC pdc(this);
	CFileDialog ff(true,NULL,NULL,
					OFN_OVERWRITEPROMPT,
					_T("*.bmp|*.bmp|"),NULL);
	if(ff.DoModal()==IDOK)
	{
		CString path = ff.GetPathName();
		m_pBmpDat->Open(path);
//		saved = 0;
		Invalidate(TRUE);
	}	
}

void CFormImageRead::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(rbd){//如果右键也按下的
		rbd = FALSE;
		m_pBmpDat->Cancel();//撤消所作的工作
		Invalidate(FALSE);
		tx[5] = 0;
	}
	else if(point.x<100){
		for(int i = 0;i<cnum;i++){
			if(con[i].InIt(point.x,point.y)){//选择画图工具
				if(contyp == 4&&i!=4){
					m_pBmpDat->BeiFen();
					saved = 1;
					tx[5] = 0;
				}
				if(i == 5&&contyp!=5){//选择的是文字工具时,要打开文字工具对话框
/*					texttool.Set(&contyp,contyp,bmp);
					texttool.SetXY(0,0);
					tx[9] = contyp;contyp = i;
					Invalidate(FALSE);
					texttool.ShowWindow(SW_RESTORE);*/
				}
				else if(i != 5){
					if(contyp == 5){
						//texttool.Close();
					}
					contyp = i;
					Invalidate(FALSE);
				}
				return;
			}
		}
		if(point.x>30&&point.x<50&&point.y>310&&point.y<330){
			CColorDialog cscd(m_pBmpDat->pecol);//选择前景颜色
			if(cscd.DoModal() == IDOK)
			{
				m_pBmpDat->SetPen(cscd.GetColor());
				Invalidate(FALSE);
			}
		}
		if(point.x > 55 && 
			point.x < 75 &&
			point.y> 320 && 
			point.y<340)
		{
			CColorDialog cscd(m_pBmpDat->bgcol);//选择背景颜色
			if(cscd.DoModal() == IDOK){
				m_pBmpDat->bgcol = cscd.GetColor();
				Invalidate(FALSE);
			}
		}
	}
	else{
		if(point.x<m_pBmpDat->GetW()+100 && 
			point.y<m_pBmpDat->GetH())
		{
			//绘图区域
			lbd = TRUE;
			if(contyp == 3){//填充
				m_pBmpDat->TC(point.x-100,point.y);
				Invalidate(FALSE);
			}
			else if(contyp == 2){//擦除
				m_pBmpDat->Clear(point.x-100,point.y);
				Invalidate(FALSE);
			}
			else if(contyp == 4)
			{//曲线
				if(tx[5]==0){//第一点(第二点在释放时记录)
					lbd = TRUE;
					tx[2] = tx[1] = tx[0] = point.x-100;
					ty[2] = ty[1] = ty[0] = point.y;
				}
				else if(tx[5] == 1){//第三点
					tx[1] = tx[2] = point.x - 100;
					ty[1] = ty[2] = point.y;
					m_pBmpDat->ReBezier(tx,ty);
					Invalidate(FALSE);
				}
				else{//第四点
					tx[2] = point.x - 100;
					ty[2] = point.y;
					m_pBmpDat->ReBezier(tx,ty);
					Invalidate(FALSE);
				}
			}
			else if(contyp == 5){//文字
/*				if(texttool.Select(point.x-100,point.y)){
					tx[0] = point.x-100;
					ty[0] = point.y;
				}
				else{
					lbd = FALSE;
				}*/
			}
			else{
				lbd = TRUE;
				tx[1] = tx[0] = point.x-100;
				ty[1] = ty[0] = point.y;
			}
		}
	}	
	CYdFormView::OnLButtonDown(nFlags, point);
}

void CFormImageRead::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(lbd){
		lbd = FALSE;
		if(contyp==0){//画笔
			m_pBmpDat->Line(tx[1],ty[1],point.x-100,point.y);
			m_pBmpDat->Darw(point.x-100,point.y);
			m_pBmpDat->BeiFen();
			saved = 1;
		}
		else if(contyp == 1){//画直线
			m_pBmpDat->ReLine(tx[0],ty[0],point.x-100,point.y,1,0);
			m_pBmpDat->Darw(point.x-100,point.y);
			m_pBmpDat->BeiFen();
			saved = 1;
		}
		else if(contyp == 2){//擦除
			m_pBmpDat->Clear(point.x-100,point.y);
			m_pBmpDat->BeiFen();
			saved = 1;
		}
		else if(contyp == 3){//填充
			m_pBmpDat->BeiFen();
			saved = 1;
		}
		else if(contyp == 4){//曲线
			tx[5] ++;
			if(tx[5] == 2){
				tx[2] = tx[1] = point.x - 100;
				ty[2] = ty[1] = point.y;
			}
			else if(tx[5] == 3){
				tx[2] = point.x-100;
				ty[2] = point.y;
				tx[5] = 0;
			}
			m_pBmpDat->ReBezier(tx,ty);
			if(tx[5]==0){
				m_pBmpDat->BeiFen();
				saved = 1;
			}
		}
		else if(contyp == 5){//文字
//			texttool.Move(point.x-100-tx[0],point.y-ty[0]);
//			Invalidate(FALSE);
		}
		Invalidate(FALSE);
	}	
	CYdFormView::OnLButtonUp(nFlags, point);
}

void CFormImageRead::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(lbd){
		if(contyp == 0){//画笔工具
			m_pBmpDat->Line(tx[1],ty[1],point.x-100,point.y);
			tx[1] = point.x-100;
			ty[1] = point.y;
			Invalidate(FALSE);
		}
		else if(contyp == 1){//画直线
			m_pBmpDat->ReLine(tx[0],ty[0],point.x-100,point.y,0,0);
			tx[1] = point.x-100;
			ty[1] = point.y;
			Invalidate(FALSE);
		}
		else if(contyp == 2){//擦除
			m_pBmpDat->Clear(point.x-100,point.y);
			tx[1] = point.x-100;
			ty[1] = point.y;
			Invalidate(FALSE);
		}
		else if(contyp == 4){//曲线
			if(tx[5] == 0){
				tx[3] = point.x - 100;ty [3] = point.y;
			}
			else if(tx[5] == 1){
				tx[1] = tx[2] = point.x - 100;
				ty[1] = ty[2] = point.y;
			}
			else{
				tx[2] = point.x - 100;
				ty[2] = point.y;
			}
			m_pBmpDat->ReBezier(tx,ty);
			Invalidate(FALSE);
		}
		else if(contyp == 5){//文字
/*			texttool.Move(point.x-100-tx[0],point.y-ty[0]);
			Invalidate(FALSE);
			tx[0] = point.x-100;
			ty[0] = point.y;*/
		}
	}
	else if(rbd){
		if(contyp == 0){
			m_pBmpDat->Line(tx[1],ty[1],point.x-100,point.y,1);
			Invalidate(FALSE);
			tx[1] = point.x-100;
			ty[1] = point.y;
		}
		else if(contyp == 1){
			m_pBmpDat->ReLine(tx[0],ty[0],point.x-100,point.y,0,1);
			Invalidate(FALSE);
			tx[1] = point.x-100;
			ty[1] = point.y;
		}
	}
	CYdFormView::OnMouseMove(nFlags, point);
}

void CFormImageRead::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if(lbd){
		if(contyp==5){
			return;
		}
		lbd = FALSE;
		m_pBmpDat->Cancel();
		Invalidate(FALSE);
		tx[5] = 0;
	}
	else if(point.x < m_pBmpDat->GetW()+100 && 
			point.y<m_pBmpDat->GetH() && 
			point.x>=100)
	{
		rbd = TRUE;
		if(contyp == 3){
			m_pBmpDat->TC(point.x-100,point.y,1);
			Invalidate(FALSE);
		}
		else if(contyp == 2){
		}
		else{
			tx[1] = tx[0] = point.x-100;
			ty[1] = ty[0] = point.y;
		}
	}	
	CYdFormView::OnRButtonDown(nFlags, point);
}

void CFormImageRead::OnRButtonUp(UINT nFlags, CPoint point) 
{
	if(rbd){
		rbd = FALSE;
		if(contyp == 0)
		{
			m_pBmpDat->Line(tx[1],ty[1],point.x-100,point.y,1);
			m_pBmpDat->Darw(point.x-100,point.y,1);
		}
		else if(contyp == 1)
		{
			m_pBmpDat->ReLine(tx[0],ty[0],point.x-100,point.y,1,1);
			m_pBmpDat->Darw(point.x-100,point.y,1);
		}
		m_pBmpDat->BeiFen();
		Invalidate(FALSE);
		saved = 1;
	}	
	CYdFormView::OnRButtonUp(nFlags, point);
}

int CFormImageRead::StringEnd(CString &ts,TCHAR e[])
{
	TCHAR s[500];
	lstrcpy(s,ts);
	int l = lstrlen(s);
	int n = lstrlen(e);
	int i = 0;
	if(l>n){
		for(i=0;i<n;i++){
			if(s[l-1-i]!=e[n-1-i])
				break;
		}
	}
	if(i<n||s[l-i-1]==_T('\\'))
	{
		ts.Format(_T("%s%s"),s,e);
		return 0;
	}
	return 1;
}

void CFormImageRead::OnSave() 
{
  	CFileDialog ff(false,
					NULL,NULL,
					OFN_OVERWRITEPROMPT,
					_T("*.bmp|*.bmp|"),NULL);
	if(ff.DoModal()==IDOK)
	{
		CString path = ff.GetPathName();
		StringEnd(path,_T(".bmp"));
		m_pBmpDat->Save(path);
		saved = 0;
		Invalidate(TRUE);
	}	
}

void CFormImageRead::OnRevo() 
{
	if(m_pBmpDat->UnDoOK()){
		m_pBmpDat->UnDo();
		saved = 1;
		Invalidate();
	}	
}
BOOL CFormImageRead::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
											 DWORD dwRequestedStyle, const RECT& rect, CWnd* pParentWnd, UINT nID,
											 CCreateContext* pContext)
{
	CExtDllState state;
	return CFormView::Create(lpszClassName,
		lpszWindowName,
		dwRequestedStyle,
		rect,
		pParentWnd,
		nID,
		pContext);
}