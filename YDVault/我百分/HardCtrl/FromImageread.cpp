#include "stdafx.h"
#include "hardctrl.h"
#include "MainFrm.h"
#include "FromImageread.h"

extern CHardCtrlApp theApp;
IMPLEMENT_DYNCREATE(CFromImageread, CDialog)
int * psaved = NULL;

CFromImageread::CFromImageread(CWnd* pParent /*=NULL*/)
	: cdxCSizingDialog(CFromImageread::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFromImageread)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFromImageread::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFromImageread)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFromImageread, cdxCSizingDialog)
	//{{AFX_MSG_MAP(CFromImageread)
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
// CFromImageread message handlers

BOOL CFromImageread::OnInitDialog() 
{
	cdxCSizingDialog::OnInitDialog();
	
	CPaintDC dc(this);
	bmp = new CBmpDat(&dc);
	bmp->New(800,400);

	saved = -1;
	contyp = 0;
	cnum = 6;
	lbd = FALSE;
	rbd = FALSE;
	tx[5] = 0;
	psaved = &saved;	
	
	return TRUE;  
}

void CFromImageread::OnPaint() 
{
	CPaintDC dc(this);
	dc.BitBlt(100,0,bmp->GetW(),bmp->GetH(),bmp->GetDC(),0,0,SRCCOPY);
}

void CFromImageread::OnOpne() 
{
	CPaintDC pdc(this);
	CFileDialog ff(true,NULL,NULL,OFN_OVERWRITEPROMPT,"*.bmp|*.bmp|",NULL);
	if(ff.DoModal()==IDOK){
		CString path = ff.GetPathName();
		bmp->Open(path);
//		saved = 0;
		Invalidate(TRUE);
	}	
}

void CFromImageread::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(rbd){//如果右键也按下的
		rbd = FALSE;
		bmp->Cancel();//撤消所作的工作
		Invalidate(FALSE);
		tx[5] = 0;
	}
	else if(point.x<100){
		for(int i = 0;i<cnum;i++){
			if(con[i].InIt(point.x,point.y)){//选择画图工具
				if(contyp == 4&&i!=4){
					bmp->BeiFen();
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
			CColorDialog cscd(bmp->pecol);//选择前景颜色
			if(cscd.DoModal() == IDOK){
				bmp->SetPen(cscd.GetColor());
				Invalidate(FALSE);
			}
		}
		if(point.x>55&&point.x<75&&point.y>320&&point.y<340){
			CColorDialog cscd(bmp->bgcol);//选择背景颜色
			if(cscd.DoModal() == IDOK){
				bmp->bgcol = cscd.GetColor();
				Invalidate(FALSE);
			}
		}
	}
	else{
		if(point.x<bmp->GetW()+100&&point.y<bmp->GetH()){
			//绘图区域
			lbd = TRUE;
			if(contyp == 3){//填充
				bmp->TC(point.x-100,point.y);
				Invalidate(FALSE);
			}
			else if(contyp == 2){//擦除
				bmp->Clear(point.x-100,point.y);
				Invalidate(FALSE);
			}
			else if(contyp == 4){//曲线
				if(tx[5]==0){//第一点(第二点在释放时记录)
					lbd = TRUE;
					tx[2] = tx[1] = tx[0] = point.x-100;
					ty[2] = ty[1] = ty[0] = point.y;
				}
				else if(tx[5] == 1){//第三点
					tx[1] = tx[2] = point.x - 100;
					ty[1] = ty[2] = point.y;
					bmp->ReBezier(tx,ty);
					Invalidate(FALSE);
				}
				else{//第四点
					tx[2] = point.x - 100;
					ty[2] = point.y;
					bmp->ReBezier(tx,ty);
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
	cdxCSizingDialog::OnLButtonDown(nFlags, point);
}

void CFromImageread::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(lbd){
		lbd = FALSE;
		if(contyp==0){//画笔
			bmp->Line(tx[1],ty[1],point.x-100,point.y);
			bmp->Darw(point.x-100,point.y);
			bmp->BeiFen();
			saved = 1;
		}
		else if(contyp == 1){//画直线
			bmp->ReLine(tx[0],ty[0],point.x-100,point.y,1,0);
			bmp->Darw(point.x-100,point.y);
			bmp->BeiFen();
			saved = 1;
		}
		else if(contyp == 2){//擦除
			bmp->Clear(point.x-100,point.y);
			bmp->BeiFen();
			saved = 1;
		}
		else if(contyp == 3){//填充
			bmp->BeiFen();
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
			bmp->ReBezier(tx,ty);
			if(tx[5]==0){
				bmp->BeiFen();
				saved = 1;
			}
		}
		else if(contyp == 5){//文字
//			texttool.Move(point.x-100-tx[0],point.y-ty[0]);
//			Invalidate(FALSE);
		}
		Invalidate(FALSE);
	}	
	cdxCSizingDialog::OnLButtonUp(nFlags, point);
}

void CFromImageread::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(lbd){
		if(contyp == 0){//画笔工具
			bmp->Line(tx[1],ty[1],point.x-100,point.y);
			tx[1] = point.x-100;
			ty[1] = point.y;
			Invalidate(FALSE);
		}
		else if(contyp == 1){//画直线
			bmp->ReLine(tx[0],ty[0],point.x-100,point.y,0,0);
			tx[1] = point.x-100;
			ty[1] = point.y;
			Invalidate(FALSE);
		}
		else if(contyp == 2){//擦除
			bmp->Clear(point.x-100,point.y);
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
			bmp->ReBezier(tx,ty);
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
			bmp->Line(tx[1],ty[1],point.x-100,point.y,1);
			Invalidate(FALSE);
			tx[1] = point.x-100;
			ty[1] = point.y;
		}
		else if(contyp == 1){
			bmp->ReLine(tx[0],ty[0],point.x-100,point.y,0,1);
			Invalidate(FALSE);
			tx[1] = point.x-100;
			ty[1] = point.y;
		}
	}
	cdxCSizingDialog::OnMouseMove(nFlags, point);
}

void CFromImageread::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if(lbd){
		if(contyp==5){
			return;
		}
		lbd = FALSE;
		bmp->Cancel();
		Invalidate(FALSE);
		tx[5] = 0;
	}
	else if(point.x<bmp->GetW()+100&&point.y<bmp->GetH()&&point.x>=100){
		rbd = TRUE;
		if(contyp == 3){
			bmp->TC(point.x-100,point.y,1);
			Invalidate(FALSE);
		}
		else if(contyp == 2){
		}
		else{
			tx[1] = tx[0] = point.x-100;
			ty[1] = ty[0] = point.y;
		}
	}	
	cdxCSizingDialog::OnRButtonDown(nFlags, point);
}

void CFromImageread::OnRButtonUp(UINT nFlags, CPoint point) 
{
	if(rbd){
		rbd = FALSE;
		if(contyp == 0){
			bmp->Line(tx[1],ty[1],point.x-100,point.y,1);
			bmp->Darw(point.x-100,point.y,1);
		}
		else if(contyp == 1){
			bmp->ReLine(tx[0],ty[0],point.x-100,point.y,1,1);
			bmp->Darw(point.x-100,point.y,1);
		}
		bmp->BeiFen();
		Invalidate(FALSE);
		saved = 1;
	}	
	cdxCSizingDialog::OnRButtonUp(nFlags, point);
}

int CFromImageread::StringEnd(CString &ts,char e[]){
	char s[500];
	strcpy(s,ts);
	int l = strlen(s);
	int n = strlen(e);
	int i = 0;
	if(l>n){
		for(i=0;i<n;i++){
			if(s[l-1-i]!=e[n-1-i])
				break;
		}
	}
	if(i<n||s[l-i-1]=='\\'){
		ts.Format("%s%s",s,e);
		return 0;
	}
	return 1;
}

void CFromImageread::OnSave() 
{
  	CFileDialog ff(false,NULL,NULL,OFN_OVERWRITEPROMPT,"*.bmp|*.bmp|",NULL);
	if(ff.DoModal()==IDOK){
		CString path = ff.GetPathName();
		StringEnd(path,".bmp");
		bmp->Save(path);
		saved = 0;
		Invalidate(TRUE);
	}	
}

void CFromImageread::OnRevo() 
{
	if(bmp->UnDoOK()){
		bmp->UnDo();
		saved = 1;
		Invalidate();
	}	
}
