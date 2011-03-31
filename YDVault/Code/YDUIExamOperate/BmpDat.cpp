// BmpDat.cpp: implementation of the CBmpDat class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "BmpDat.h"
#include "resource.h"

#include <time.h>
#include <stdlib.h>
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CStatusBar * mmcsb;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBmpDat::CBmpDat(CDC * dc){
	undo = 0;
	redo = 2;
	width = 0;
	height = 0;
	path = _T("");
	bgcol = RGB(255,255,255);
	pecol = RGB(255,0,0);
	pdc.CreateCompatibleDC(dc);
	bfdc.CreateCompatibleDC(dc);
	mdc[0].CreateCompatibleDC(dc);
	mdc[1].CreateCompatibleDC(dc);
	CBitmap *obp;
//	bp.CreateCompatibleBitmap(dc,1024,786);
	bp.LoadBitmap(IDB_BBB);
	////////////////////////////////////将位图选入DC
	obp = pdc.SelectObject(&bp);
	obp->DeleteObject();
	//////////////////////////////////////创建一个白色画笔
//	CPen pen,*op;
//	pen.CreatePen(PS_SOLID,796,RGB(255,255,255));
	//////////////////////////////////////使用白色画笔
//	op = pdc.SelectObject(&pen);
	//////////////////////////////////////
//	pdc.MoveTo(0,393);
//	pdc.LineTo(1024,393);
	////////////////////////////////////////
//	pdc.SelectObject(op);
	pdc.SetTextColor(pecol);
	pdc.SetBkMode(111);

	//////////////////////////
	CBitmap *obp2;
//	bp2.CreateCompatibleBitmap(dc,1024,786);
	bp2.LoadBitmap(IDB_BBB);
	////////////////////////////////////将位图选入DC
	obp2 = bfdc.SelectObject(&bp2);
	obp2->DeleteObject();

	CBitmap *obp3;
//	bp3.CreateCompatibleBitmap(dc,1024,786);
	bp3.LoadBitmap(IDB_BBB);
	////////////////////////////////////将位图选入DC
	obp3 = mdc[0].SelectObject(&bp3);
	obp3->DeleteObject();

	CBitmap *obp4;
//	bp4.CreateCompatibleBitmap(dc,1024,786);
	bp4.LoadBitmap(IDB_BBB);
	////////////////////////////////////将位图选入DC
	obp4 = mdc[1].SelectObject(&bp4);
	obp4->DeleteObject();

//	pen.DeleteObject();
}

CBmpDat::~CBmpDat(){
}

//////////////
void CBmpDat::Cancel(){
	pdc.BitBlt(0,0,width,height,&bfdc,0,0,SRCCOPY);
	for(int i = 0;i<width;i++){
		for(int j = 0;j<height;j++){
			pix[0][i][j] = bfpix[0][i][j];
			pix[1][i][j] = bfpix[1][i][j];
			pix[2][i][j] = bfpix[2][i][j];
		}
	}
}
///////////////////////////////////////////////
void CBmpDat::BeiFen(){
	int i,j;
	if(undo==2){
		undo--;
		mdc[0].BitBlt(0,0,width,height,&mdc[1],0,0,SRCCOPY);
		for(i = 0;i<width;i++){
			for(j = 0;j<height;j++){
				mpix[0][0][i][j] = mpix[1][0][i][j];
				mpix[0][1][i][j] = mpix[1][1][i][j];
				mpix[0][2][i][j] = mpix[1][2][i][j];
			}
		}
		lastw[0] = lastw[1];
		lasth[0] = lasth[1];
	}
	mdc[undo].BitBlt(0,0,width,height,&bfdc,0,0,SRCCOPY);
	for(i = 0;i<width;i++){
		for(j = 0;j<height;j++){
			mpix[undo][0][i][j] = bfpix[0][i][j];
			mpix[undo][1][i][j] = bfpix[1][i][j];
			mpix[undo][2][i][j] = bfpix[2][i][j];
		}
	}
	bfdc.BitBlt(0,0,width,height,&pdc,0,0,SRCCOPY);
	for(i = 0;i<width;i++){
		for(j = 0;j<height;j++){
			bfpix[0][i][j] = pix[0][i][j];
			bfpix[1][i][j] = pix[1][i][j];
			bfpix[2][i][j] = pix[2][i][j];
		}
	}
	lastw[undo] = width;
	lasth[undo] = height;
	undo ++;
	redo = 2;
}

void CBmpDat::UnDo(){
	if(!UnDoOK())
		return;
	int i,j;
	int ow,oh;
	undo --;
	redo --;
	ow = width;
	oh = height;
	width = lastw[undo];
	height = lasth[undo];
	int tw,th;
	tw = ow>width?ow:width;
	th = oh>height?oh:height;
	pdc.BitBlt(0,0,tw,th,&mdc[undo],0,0,SRCCOPY);
	for(i = 0;i<tw;i++){
		for(j = 0;j<th;j++){
			pix[0][i][j] = mpix[undo][0][i][j];
			pix[1][i][j] = mpix[undo][1][i][j];
			pix[2][i][j] = mpix[undo][2][i][j];
		}
	}
	mdc[undo].BitBlt(0,0,tw,th,&bfdc,0,0,SRCCOPY);
	for(i = 0;i<tw;i++){
		for(j = 0;j<th;j++){
			mpix[undo][0][i][j] = bfpix[0][i][j];
			mpix[undo][1][i][j] = bfpix[1][i][j];
			mpix[undo][2][i][j] = bfpix[2][i][j];
		}
	}
	bfdc.BitBlt(0,0,tw,th,&pdc,0,0,SRCCOPY);
	for(i = 0;i<tw;i++){
		for(j = 0;j<th;j++){
			bfpix[0][i][j] = pix[0][i][j];
			bfpix[1][i][j] = pix[1][i][j];
			bfpix[2][i][j] = pix[2][i][j];
		}
	}
	lastw[undo] = ow;
	lasth[undo] = oh;
//	oldwidth = width;
//	oldheight = height;
}
void CBmpDat::ReDo(){
	if(!ReDoOK())
		return;
	int i,j;
	int ow,oh;
	ow = width;
	oh = height;
	width = lastw[undo];
	height = lasth[undo];
	int tw,th;
	tw = ow>width?ow:width;
	th = oh>height?oh:height;
	pdc.BitBlt(0,0,tw,th,&mdc[undo],0,0,SRCCOPY);
	for(i = 0;i<tw;i++){
		for(j = 0;j<th;j++){
			pix[0][i][j] = mpix[undo][0][i][j];
			pix[1][i][j] = mpix[undo][1][i][j];
			pix[2][i][j] = mpix[undo][2][i][j];
		}
	}
	mdc[undo].BitBlt(0,0,tw,th,&bfdc,0,0,SRCCOPY);
	for(i = 0;i<tw;i++){
		for(j = 0;j<th;j++){
			mpix[undo][0][i][j] = bfpix[0][i][j];
			mpix[undo][1][i][j] = bfpix[1][i][j];
			mpix[undo][2][i][j] = bfpix[2][i][j];
		}
	}
	bfdc.BitBlt(0,0,tw,th,&pdc,0,0,SRCCOPY);
	for(i = 0;i<tw;i++){
		for(j = 0;j<th;j++){
			bfpix[0][i][j] = pix[0][i][j];
			bfpix[1][i][j] = pix[1][i][j];
			bfpix[2][i][j] = pix[2][i][j];
		}
	}
	lastw[undo] = ow;
	lasth[undo] = oh;
	undo ++;
	redo ++;
}
////////////////////////////////////////////////////
int StringEnd(CString &ts,TCHAR e[])
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
	if(i<n||s[l-i-1]==_T('\\')){
		ts.Format(_T("%s%s"),s,e);
		return 0;
	}
	return 1;
}

int CBmpDat::Save(CString p){
	if(p.GetLength()>0){
		path = p;
	}
	if(path.GetLength()==0){
		CFileDialog ff(false,NULL,NULL,
					OFN_OVERWRITEPROMPT,
					_T("*.bmp|*.bmp|"),NULL);
		if(ff.DoModal()==IDOK){
			path = ff.GetPathName();
			StringEnd(path,_T(".bmp"));
		}
		else{
			return -1;
		}
	}
	CDC memDC;
	memDC.CreateCompatibleDC(&pdc);
	CBitmap bm;
	bm.CreateCompatibleBitmap(&pdc, width, height);
	CBitmap*  pOld = memDC.SelectObject(&bm);
	memDC.BitBlt(0, 0, width, height, &pdc, 0, 0, SRCCOPY);
	memDC.SelectObject(pOld);
	BITMAP  btm;
	bm.GetBitmap(&btm);
	DWORD  size = btm.bmWidthBytes * btm.bmHeight;
	LPSTR lpData = (LPSTR)GlobalAlloc(GPTR, size);
	BITMAPFILEHEADER   bfh;
	/////////////////////////////////////////////
	BITMAPINFOHEADER  bih;
	bih.biBitCount = btm.bmBitsPixel;
	bih.biClrImportant = 0;
	bih.biClrUsed = 0;
	bih.biCompression = 0;
	bih.biHeight = btm.bmHeight;
	bih.biPlanes = 1;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biSizeImage = size;
	bih.biWidth = btm.bmWidth;
	bih.biXPelsPerMeter = 0;
	bih.biYPelsPerMeter = 0;
	GetDIBits(pdc,bm,0,bih.biHeight,lpData,(BITMAPINFO*)&bih,DIB_RGB_COLORS);
	//bm.GetBitmapBits(size,lpData);//此函数在处理5-5-5模式的16位色下会出现颜色混乱
	bfh.bfReserved1 = bfh.bfReserved2 = 0;
	bfh.bfType = ((WORD)('M'<< 8)|'B');
	bfh.bfSize = 54 + size;
	bfh.bfOffBits = 54;

	CFile  bf;
	if(bf.Open(path,CFile::modeCreate|CFile::modeWrite))	{
		bf.Write(&bfh, sizeof(BITMAPFILEHEADER));
		bf.Write(&bih, sizeof(BITMAPINFOHEADER));
		bf.Write(lpData, size);
		bf.Close();
	}
	GlobalFree (lpData);
	return 0;
}

void CBmpDat::New(int w,int h){
	int i,j;
	/////////////////////////////////////创建一个DC
	/////////////////////////////////////创建一个位图
	CBitmap *bp,*obp;
	bp = new CBitmap;
//	bp->CreateCompatibleBitmap(&pdc,1024,786);
	bp->LoadBitmap(IDB_BBB);
	////////////////////////////////////将位图选入DC
	obp = pdc.SelectObject(bp);
	obp->DeleteObject();
	//////////////////////////////////////创建一个白色画笔
//	CPen pen,*op;
//	pen.CreatePen(PS_SOLID,796,RGB(255,255,255));
	//////////////////////////////////////使用白色画笔
//	op = pdc.SelectObject(&pen);
	//////////////////////////////////////
//	pdc.MoveTo(0,393);
//	pdc.LineTo(1024,393);
	////////////////////////////////////////
//	pdc.SelectObject(op);
//	pen.DeleteObject();

	///////////////////////////
	path = _T("");
	height = h>786?786:h;
	width = w>1024?1024:w;
	oldheight = h>786?786:h;
	oldwidth = w>1024?1024:w;
	for(i = 0;i<1024;i++){
		for(j = 0;j<786;j++){
			pix[0][i][j] = 255;
			pix[1][i][j] = 255;
			pix[2][i][j] = 255;
			bfpix[0][i][j] = 255;
			bfpix[1][i][j] = 255;
			bfpix[2][i][j] = 255;
			mpix[0][0][i][j] = 255;
			mpix[0][1][i][j] = 255;
			mpix[0][2][i][j] = 255;
			mpix[1][0][i][j] = 255;
			mpix[1][1][i][j] = 255;
			mpix[1][2][i][j] = 255;
		}
	}
	BeiFen();
	undo = 0;
	redo = 2;
}


void CBmpDat::Open(CString p){
	/////读取位图文件
	int i,j;
	HBITMAP bitmap;
	CFile file;
	if(file.Open(p,CFile::modeRead|CFile::shareDenyNone)){//打开文件
		j = width;
		BITMAPINFO bmpinfo;
		BITMAPFILEHEADER bmphead;
		file.Read(&bmphead,sizeof(BITMAPFILEHEADER));
		file.Read(&bmpinfo,sizeof(BITMAPINFO));
		width = bmpinfo.bmiHeader.biWidth;
		height = bmpinfo.bmiHeader.biHeight;
		oldwidth = width;
		oldheight = height;
		file.Close();
	}
	else{
		AfxMessageBox(_T("打开文件失败！"));
		return;
	}
	bitmap=(HBITMAP)LoadImage(AfxGetInstanceHandle(),p,IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	ASSERT(bitmap);

	New(width,height);
	CDC mdc;
	mdc.CreateCompatibleDC(&pdc);
	mdc.SelectObject(bitmap);
	pdc.BitBlt(0,0,width,height,&mdc,0,0,SRCCOPY);

	path = p;
	COLORREF clr;
	CString str;
	for(i = 0;i<width;i++){
		for(j = 0;j<height;j++){
			clr = pdc.GetPixel(i,j);
			pix[0][i][j] = GetRValue(clr);
			pix[1][i][j] = GetGValue(clr);
			pix[2][i][j] = GetBValue(clr);
			bfpix[0][i][j] = pix[0][i][j];
			bfpix[1][i][j] = pix[1][i][j];
			bfpix[2][i][j] = pix[2][i][j];
		}
		str.Format(_T("已完成%d%%"),i*100/width);
//		mmcsb->SetWindowText(str);
	}
	BeiFen();
//	mmcsb->SetWindowText("就绪");
	undo = 0;
	redo = 2;
}

//////////////////////边缘提取///////////////////////////////
void CBmpDat::BY(){
	BeiFen();
	HDTX(0);
	CString str;
	int i,j;
	for(i = 0;i<width;i++){
		for(j = 0;j<height;j++){
			pix[0][i][j] = BY(0,i,j);
			pix[1][i][j] = BY(1,i,j);
			pix[2][i][j] = BY(2,i,j);
			pdc.SetPixel(i,j,RGB(pix[0][i][j],pix[1][i][j],pix[2][i][j]));
		}
		str.Format(_T("已完成%d%%"),i*100/width);
		mmcsb->SetWindowText(str);
	}
	for(i = 0;i<width;i++){
		for(j = 0;j<height;j++){
			bfpix[0][i][j] = pix[0][i][j];
			bfpix[1][i][j] = pix[1][i][j];
			bfpix[2][i][j] = pix[2][i][j];
		}
	}
	bfdc.BitBlt(0,0,width,height,&pdc,0,0,SRCCOPY);
	mmcsb->SetWindowText(_T("就绪"));
}
BYTE CBmpDat::BY(int i,int x,int y){
	int t1 = 0,t2 = 0,t3 = 0,t4 = 0,t;
	if(x>0){
		t1 = bfpix[i][x-1][y]-bfpix[i][x][y];
		if(t1<0){t1 = -t1;}
		if(y>0){
			t2 = bfpix[i][x-1][y-1]-bfpix[i][x][y];
			if(t2<0){t2 = -t2;}
		}
	}
	if(x<width-1){
		t3 = bfpix[i][x+1][y]-pix[i][x][y];
		if(t3<0){t3 = -t3;}
		if(y<height-1){
			t4 = bfpix[i][x+1][y+1]-bfpix[i][x][y];
			if(t4<0){t4 = -t4;}
		}
	}
	t = t1+t2+t3+t4;
	return t>255?255:t;
}

///////////////////////////浮雕/////////////////////////////////
void CBmpDat::FD(){
	BeiFen();
	HDTX(0);
	CString str;
	int i,j;
	for(i = 0;i<width;i++){
		for(j = 0;j<height;j++){
			pix[0][i][j] = FD(0,i,j);
			pix[1][i][j] = FD(1,i,j);
			pix[2][i][j] = FD(2,i,j);
			pdc.SetPixel(i,j,RGB(pix[0][i][j],pix[1][i][j],pix[2][i][j]));
		}
		str.Format(_T("已完成%d%%"),i*100/width);
		mmcsb->SetWindowText(str);
	}
	for(i = 0;i<width;i++){
		for(j = 0;j<height;j++){
			bfpix[0][i][j] = pix[0][i][j];
			bfpix[1][i][j] = pix[1][i][j];
			bfpix[2][i][j] = pix[2][i][j];
		}
	}
	bfdc.BitBlt(0,0,width,height,&pdc,0,0,SRCCOPY);
	mmcsb->SetWindowText(_T("就绪"));
}
BYTE CBmpDat::FD(int i,int x,int y){
	int t = 0;
	if(x<width-2){
		if(y<height){
			t = bfpix[i][x][y] - bfpix[i][x+2][y+1] + 125;
		}
		else{
			t = bfpix[i][x][y-1] - bfpix[i][x+2][y] + 125;
		}
	}
	else if(x<width-1){
		if(y<height){
			t = bfpix[i][x][y] - bfpix[i][x+1][y+1] + 125;
		}else{
			t = bfpix[i][x][y-1] - bfpix[i][x+1][y] + 125;
		}
	}
	else{
		if(y<height){
			t = bfpix[i][x-1][y] - bfpix[i][x][y+1] + 125;
		}else{
			t = bfpix[i][x-1][y-1] - bfpix[i][x][y] + 125;
		}
	}
	if(t<0){t = 0;}
	if(t>255){t = 255;}
	return t;
}


//////////////////////////去噪////////////////////////////
int Mid(int n,int c[]){
	int m = n / 2;
	for(int i = 0;i<m+1;i++){
		for(int j = 0;j<n-i-1;j++){
			if(c[j]>c[j+1]){
				int t = c[j];c[j] = c[j+1];c[j+1] = t;
			}
		}
	}
	return c[m];
}
BYTE CBmpDat::LB(int i,int x,int y,int z){
	int tn[9];
	int n = 0;
	if(x>0){
		if(y>0){
			tn[n++] = bfpix[i][x-1][y-1];
		}
		tn[n++] = bfpix[i][x-1][y];
		if(y<height-1){
			tn[n++] = bfpix[i][x-1][y+1];
		}
	}
	if(y>0){
		tn[n++] = bfpix[i][x][y-1];
	}
	tn[n++] = bfpix[i][x][y];
	if(y<height-1){
		tn[n++] = bfpix[i][x][y+1];
	}
	if(x<width-1){
		if(y>0){
			tn[n++] = bfpix[i][x+1][y-1];
		}
		tn[n++] = bfpix[i][x+1][y];
		if(y<height-1){
			tn[n++] = bfpix[i][x+1][y+1];
		}
	}
	int t1,t2;
	t1 = Mid(n,tn);
	t2 = t1 - bfpix[i][x][y];
	if(t2<-z||t2>z){
		return t1;
	}
	else{
		return bfpix[i][x][y];
	}
}
///////////////////模糊////////////////////////
BYTE CBmpDat::MH(int i,int x,int y){
	int temp = 2*(int)(bfpix[i][x][y]);
	int n = 2;
	if(x>0){
		if(y>0){
			temp += (int)bfpix[i][x-1][y-1];
			n++;
		}
		temp += int(bfpix[i][x-1][y]);
		n ++;
		if(y<height-1){
			temp += int(bfpix[i][x-1][y+1]);
			n++;
		}
	}
	if(y>0){
		temp += int(bfpix[i][x][y-1]);
		n ++;
	}
	if(y<height-1){
		temp += int(bfpix[i][x][y+1]);
		n ++;
	}
	if(x<width-1){
		if(y>0){
			temp += int(bfpix[i][x+1][y-1]);
			n ++;
		}
		temp += int(bfpix[i][x+1][y]);
		n ++;
		if(y<height-1){
			temp += int(bfpix[i][x+1][y+1]);
			n ++;
		}
	}
	return BYTE(temp/n);
}

//////////////////////////亮度变化//////////////////////////////
BYTE CBmpDat::LA(int i,int x,int y,int z,int c){//c==1增加，c==0减少
	int n = 255 - z;
	int t = bfpix[i][x][y];
	if(c){
		t = z + t*(255 - z)/255;
	}
	else{
		t = t*z/255;
	}
	return t;
}

///////////////////////////设置画笔/////////////////////
void CBmpDat::SetPen(COLORREF col){
	pecol = col;
	CPen * p, * op;
	p = new CPen;
	p->CreatePen(PS_SOLID,1,pecol);
	op = pdc.SelectObject(p);
	op->DeleteObject();
	pdc.SetTextColor(col);
}
///////////////////////////////////种子填充算法//////////////////////////////////////
void CBmpDat::TC(int x,int y,int m){
	BYTE ** sp;
	int i,j;
	/////////////作标记//////////////
	sp = new BYTE*[width];
	for(i = 0;i<width;i++){
		sp[i] = new BYTE[height];
		for(j = 0;j<height;j++){
			sp[i][j] = 1;
		}
	}
	////////////////////////////////
	COLORREF col = pecol;
	CPen pen,*open;
	if(m)
		col = bgcol;
	pen.CreatePen(PS_SOLID,1,col);
	open = pdc.SelectObject(&pen);
	
	int sx[100],sy[100],top = 0;//栈
	int tr,tg,tb;//待填充色，
	int fr,fg,fb;//填充色
	fr = GetRValue(col);
	fg = GetGValue(col);
	fb = GetBValue(col);
	tr = pix[0][x][y];
	tg = pix[1][x][y];
	tb = pix[2][x][y];
	if(tr == fr&&tg == fg&&tb == fb){//颜色相等则不用填充
		return;
	}

	///////////////////////////////
	for(i = x;i>=0&&tr==pix[0][i][y]&&tg==pix[1][i][y]&&tb==pix[2][i][y];i--);//找到最左端的点
	sx[top] = i+1;sy[top] = y;top++;
	int lx,ly;
	while(top>0){//栈非空时填充
		////////////////////出栈
		lx = sx[top - 1];
		ly = sy[top - 1];
		top --;
		///////////////////填充
		if(sp[lx][ly]){
			for(i = lx;i<width&&pix[0][i][ly]==tr&&pix[1][i][ly]==tg&&pix[2][i][ly]==tb;i++){
				pix[0][i][ly] = fr;
				pix[1][i][ly] = fg;
				pix[2][i][ly] = fb;
//				bfpix[0][i][ly] = fr;
//				bfpix[1][i][ly] = fg;
//				bfpix[2][i][ly] = fb;
				sp[i][ly] = 0;
			}
			pdc.MoveTo(lx,ly);
			pdc.LineTo(i,ly);
			//////////////////////////////////////上看
			for(j = i-1;ly>=0&&j>=lx;j--){
				if(pix[0][j][ly-1]==tr&&pix[1][j][ly-1]==tg&&pix[2][j][ly-1]==tb){
					for(;j>=0&&pix[0][j][ly-1]==tr&&pix[1][j][ly-1]==tg&&pix[2][j][ly-1]==tb;j--);
					sx[top] = j+1;
					sy[top] = ly-1;
					top ++;
				}
			}
			//////////////////////////////////////下看
			for(j = i-1;ly<height&&j>=lx;j--){
				if(pix[0][j][ly+1]==tr&&pix[1][j][ly+1]==tg&&pix[2][j][ly+1]==tb){
					for(;j>=0&&pix[0][j][ly+1]==tr&&pix[1][j][ly+1]==tg&&pix[2][j][ly+1]==tb;j--);
					sx[top] = j+1;
					sy[top] = ly+1;
					top ++;
				}
			}
		}
	}
//	bfdc.BitBlt(0,0,width,height,&pdc,0,0,SRCCOPY);
	pdc.SelectObject(open);
}
///////////////////////////以下是基本操作////////////////////////////////////////

////////////////////////////Bezier曲线///////////////////////////////
int End(float x1,float y1,float x2,float y2,float x3,float y3){
	if(x3-x2<1.0000 && x3-x2>-1.00 && y3-y2<1.0000 && y3-y2>-1.0){
		if(x1-x3<1.0000 && x1-x3>-1.0 && y1-y3<1.0 && y1-y3>-1.0){
			return 1;
		}
	}
	return 0;
}
////////////////////////
void CBmpDat::DrawBezier(float x[4],float y[4],COLORREF col){
	int rpix = GetRValue(col),gpix = GetGValue(col),bpix = GetBValue(col);
	float tx1,ty1;
	float tx2,ty2;
	float tx3,ty3;
	float tx4,ty4;
	float tx5,ty5;
	float tx6,ty6;
	tx1 = (x[0]+x[1])/2;	ty1 = (y[0]+y[1])/2;
	tx2 = (x[2]+x[1])/2;	ty2 = (y[2]+y[1])/2;
	tx3 = (x[2]+x[3])/2;	ty3 = (y[2]+y[3])/2;
	tx4 = (tx1+tx2)/2;		ty4 = (ty1+ty2)/2;
	tx5 = (tx3+tx2)/2;		ty5 = (ty3+ty2)/2;
	tx6 = (tx4+tx5)/2;		ty6 = (ty4+ty5)/2;
	int xx = int(tx6+0.5f),yy = int(ty6+0.5f);
	if(End(x[0],y[0],x[3],y[3],tx6,ty6)){
		if(pdc.GetPixel(xx+1,yy)==col&&pdc.GetPixel(xx,yy+1)==col){
			return;
		}
		if(pdc.GetPixel(xx-1,yy)==col&&pdc.GetPixel(xx,yy+1)==col){
			return;
		}
		if(pdc.GetPixel(xx+1,yy)==col&&pdc.GetPixel(xx,yy-1)==col){
			return;
		}
		if(pdc.GetPixel(xx-1,yy)==col&&pdc.GetPixel(xx,yy-1)==col){
			return;
		}
		if(xx>=0&&xx<width&&yy>=0&&yy<=height){
			pdc.SetPixel(xx,yy,col);
			pix[0][xx][yy] = rpix;
			pix[1][xx][yy] = gpix;
			pix[2][xx][yy] = bpix;
		}
	}
	else{
		float tx[4],ty[4];
		if(xx>=0&&xx<width&&yy>=0&&yy<=height){
			pdc.SetPixel(xx,yy,col);
			pix[0][xx][yy] = rpix;
			pix[1][xx][yy] = gpix;
			pix[2][xx][yy] = bpix;
		}
		tx[0] = x[0];tx[1] = tx1;tx[2] = tx4;tx[3] = tx6;
		ty[0] = y[0];ty[1] = ty1;ty[2] = ty4;ty[3] = ty6;
		DrawBezier(tx,ty,col);
		tx[0] = tx6;tx[1] = tx5;tx[2] = tx3;tx[3] = x[3];
		ty[0] = ty6;ty[1] = ty5;ty[2] = ty3;ty[3] = y[3];
		DrawBezier(tx,ty,col);
	}
}
void CBmpDat::Bezier(int sx[],int sy[],int m){
	COLORREF col;
	if(m){
		col = bgcol;
	}
	else{
		col = pecol;
	}
	float x[4],y[4];
	for(int i = 0;i<4;i++){
		x[i] = (float)sx[i];
		y[i] = (float)sy[i];
	}
	DrawBezier(x,y,col);
}
void CBmpDat::ReBezier(int sx[],int sy[],int m){
	pdc.BitBlt(0,0,width,height,&bfdc,0,0,SRCCOPY);
	for(int i = 0;i<width;i++){
		for(int j = 0;j<height;j++){
			pix[0][i][j] = bfpix[0][i][j];
			pix[1][i][j] = bfpix[1][i][j];
			pix[2][i][j] = bfpix[2][i][j];
		}
	}
	Bezier(sx,sy,m);
}
//////////////////////////////////////////////////////////////////////////////
void CBmpDat::Line(int sx,int sy,int tx,int ty,int m){
	COLORREF col = pecol;
	if(m){
		col = bgcol;
	}
	if(tx>sx){
		if(ty>sy){
			if(ty-sy>tx-sx){
				Line(sy,sx,ty,tx,1,1,1,col);
			}
			else{
				Line(sx,sy,tx,ty,0,1,1,col);
			}
		}
		else{
			if(sy-ty>tx-sx){
				Line(-sy,sx,-ty,tx,1,-1,1,col);
			}
			else{
				Line(sx,-sy,tx,-ty,0,1,-1,col);
			}
		}
	}
	else{
		if(ty>sy){
			if(ty-sy>sx-tx){
				Line(sy,-sx,ty,-tx,1,1,-1,col);
			}
			else{
				Line(-sx,sy,-tx,ty,0,-1,1,col);
			}
		}
		else{
			if(sy-ty>sx-tx){
				Line(-sy,-sx,-ty,-tx,1,-1,-1,col);
			}
			else{
				Line(-sx,-sy,-tx,-ty,0,-1,-1,col);
			}
		}
	}
}

//////////////////////////中点画线算法////////////////////////////////
void CBmpDat::Line(int sx,int sy,int tx,int ty,int c,int dx,int dy,COLORREF col){
//	pdc->MoveTo(sx,sy);
//	pdc->LineTo(tx,ty);
	int tr,tg,tb;
	tr = GetRValue(col);
	tg = GetGValue(col);
	tb = GetBValue(col);
	int a,b,d1,d2,d,x,y;
	a = sy - ty;
	b = tx - sx;
	d = a+a+b;
	d1 = a+a;
	d2 = a+a+b+b;
	x = sx;
	y = sy;
	if(c){
		if(dy*y>=0&&dx*x>=0&&dx*x<height&&dy*y<width){
			pdc.SetPixel(dy*y,dx*x,col);
			pix[0][dy*y][dx*x] = tr;
			pix[1][dy*y][dx*x] = tg;
			pix[2][dy*y][dx*x] = tb;
		}
	}
	else{
		if(dy*y>=0&&dx*x>=0&&dx*x<width&&dy*y<height){
			pdc.SetPixel(dx*x,dy*y,col);
			pix[0][dx*x][dy*y] = tr;
			pix[1][dx*x][dy*y] = tg;
			pix[2][dx*x][dy*y] = tb;
		}
	}
	int ddd,ddd2;
	if(c){
		ddd2 = width;
		ddd = height;
	}
	else{
		ddd = width;
		ddd2 = height;
	}
	while(x<tx){
		if(d<0){
			x++;y++;
			d += d2;
		}
		else{
			x++;
			d += d1;
		}
		if(dx*x>=(ddd)||dy*y>=(ddd2)||dx*x<0||dy*y<0){
			break;
		}
		if(c){
			if(dy*y>=0&&dx*x>=0&&dx*x<ddd&&dy*y<ddd2){
				pdc.SetPixel(dy*y,dx*x,col);
				pix[0][dy*y][dx*x] = tr;
				pix[1][dy*y][dx*x] = tg;
				pix[2][dy*y][dx*x] = tb;
			}
		}
		else{
			if(dy*y>=0&&dx*x>=0&&dx*x<ddd&&dy*y<ddd2){
				pdc.SetPixel(dx*x,dy*y,col);
				pix[0][dx*x][dy*y] = tr;
				pix[1][dx*x][dy*y] = tg;
				pix[2][dx*x][dy*y] = tb;
			}
		}
	}
}

void CBmpDat::ReLine(int sx,int sy,int tx,int ty,int c,int m){
	pdc.BitBlt(0,0,width,height,&bfdc,0,0,SRCCOPY);
	if(c){
		for(int i = 0;i<width;i++){
			for(int j = 0;j<height;j++){
				pix[0][i][j] = bfpix[0][i][j];
				pix[1][i][j] = bfpix[1][i][j];
				pix[2][i][j] = bfpix[2][i][j];
			}
		}
	}
	Line(sx,sy,tx,ty,m);
}

void CBmpDat::Darw(int x,int y,int m){
	int tr,tg,tb;
	COLORREF col = pecol;
	if(m)
		col = bgcol;
	tr = GetRValue(col);
	tg = GetGValue(col);
	tb = GetBValue(col);
	pix[0][x][y] = tr;
	pix[1][x][y] = tg;
	pix[2][x][y] = tb;
	bfpix[0][x][y] = tr;
	bfpix[1][x][y] = tg;
	bfpix[2][x][y] = tb;
	pdc.SetPixel(x,y,col);
	bfdc.SetPixel(x,y,col);
}

/////////////////////////////////////////////////
void CBmpDat::Clear(int x,int y){
	int i,j;
	int bi,ei,bj,ej;
	BYTE tr,tg,tb;
	tr = GetRValue(bgcol);
	tg = GetGValue(bgcol);
	tb = GetBValue(bgcol);
	CPen pen1,*op1;
//	CPen pen2,*op2;
	pen1.CreatePen(PS_SOLID,1,bgcol);
	op1 = pdc.SelectObject(&pen1);
//	pen2.CreatePen(PS_SOLID,1,bgcol);
//	op2 = bfdc.SelectObject(&pen2);
	bi = x-5<0?0:x-5;
	ei = x+6>width?width:x+6;
	bj = y-5<0?0:y-5;
	ej = y+6>height?height:y+6;
	for(i = bi;i<ei;i++){
		for(j = bj;j<ej;j++){
			pix[0][i][j] = tr;
			pix[1][i][j] = tg;
			pix[2][i][j] = tb;
//			bfpix[0][i][j] = tr;
//			bfpix[1][i][j] = tg;
//			bfpix[2][i][j] = tb;
		}
		pdc.MoveTo(i,bj);
		pdc.LineTo(i,ej);
//		bfdc.MoveTo(i,bj);
//		bfdc.LineTo(i,ej);
	}
	pdc.SelectObject(op1);
//	pdc.SelectObject(op2);
}

////////////////////////////////////////////////////////////
void CBmpDat::CZ(int cn,int a[],int cr,int cg,int cb){
	int i,j;
	CString str;
	for(i = 0;i<width;i++){
		for(j = 0;j<height;j++){
			if(cr)
				pix[0][i][j] = XZ(0,i,j,cn,a);
			if(cg)
				pix[1][i][j] = XZ(1,i,j,cn,a);
			if(cb)
				pix[2][i][j] = XZ(2,i,j,cn,a);
			pdc.SetPixel(i,j,RGB(pix[0][i][j],pix[1][i][j],pix[2][i][j]));
		}
		str.Format(_T("已完成%d%%"),i*100/width);
		mmcsb->SetWindowText(str);
	}
	mmcsb->SetWindowText(_T("就绪"));
	BeiFen();
}
BYTE CBmpDat::XZ(int i,int x,int y,int cn,int a[]){
	switch(cn){
//	case 0:return BY(i,x,y);//边缘
//	case 1:return FD(i,x,y);//浮雕
	case 2:return LB(i,x,y,a[0]);//滤波
	case 3:return MH(i,x,y);//模糊
	case 4:return LA(i,x,y,a[0],1);//亮度增加
	case 5:return LA(i,x,y,a[0],0);//亮度减少
	}
	return 0;
}
///////////////////////////////////////////////////////
void CBmpDat::DBD(){
	int i,j;
	int maxr = 0,maxg = 0,maxb = 0;
	int minr = 255,ming = 255,minb = 255;
	int maxp = 0,minp = 255;
	CString str;
	for(i = 0;i<width;i++){
		for(j = 0;j<height;j++){
			if(pix[0][i][j]>maxr){
				maxr = pix[0][i][j];
			}
			if(pix[0][i][j]<minr){
				minr = pix[0][i][j];
			}
			if(pix[1][i][j]>maxg){
				maxg = pix[1][i][j];
			}
			if(pix[1][i][j]<ming){
				ming = pix[1][i][j];
			}
			if(pix[2][i][j]>maxb){
				maxb = pix[2][i][j];
			}
			if(pix[2][i][j]<minb){
				minb = pix[2][i][j];
			}//*/
		}
		str.Format(_T("已完成%d%%"),i*20/width);
		mmcsb->SetWindowText(str);
	}
	////////////
//	maxr = 125;
//	maxg = 125;
//	maxb = 125;
	////////////
	int dr = maxr - minr;
	int dg = maxg - ming;
	int db = maxb - minb;
	for(i = 0;i<width;i++){
		for(j = 0;j<height;j++){
			if(dr)pix[0][i][j] = ((int)pix[0][i][j] - minr)*255/dr;
			if(dg)pix[1][i][j] = ((int)pix[1][i][j] - ming)*255/dg;
			if(db)pix[2][i][j] = ((int)pix[2][i][j] - minb)*255/db;
			pdc.SetPixel(i,j,RGB(pix[0][i][j],pix[1][i][j],pix[2][i][j]));
		}
		str.Format(_T("已完成%d%%"),i*80/width+20);
		mmcsb->SetWindowText(str);
	}
	mmcsb->SetWindowText(_T("就绪"));
	BeiFen();
}
//////////////////////////
void CBmpDat::SMXG(int bl,int bw,int dx,int dy){
	bl++;
	int cou = 0;
	int tot = width*height;
	int i,j,k;
	int len = 0,ln;
	HDTX(0);
	srand((UINT)(time(NULL)));
	CString str;
	COLORREF tcl;
	if(dy){
		for(i = 0;i<width;i++){
			for(j = i,k = 0;j<width&&j>=0&&k<height&&k>=0;j+=dx,k+=dy){
				if(len == 0){
					len = rand()%bl+1;
					if(bw ==0){
						ln = 0;
					}
					else if(bw == 100){
						ln = 256;
					}
					else if(bw<11){
						ln = rand()%(bw*10);
					}
					else if(bw<95){
						ln = rand()%110+bw-10;
					}
					else{
						ln = rand()%100+(bw+14*(bw-95));
					}
				}
				tcl = SMXG(j,k,ln);
				pix[0][j][k] = GetRValue(tcl);
				pix[1][j][k] = GetGValue(tcl);
				pix[2][j][k] = GetBValue(tcl);
				pdc.SetPixel(j,k,tcl);
				len --;
				cou++;
			}
			str.Format(_T("已完成%d%%"),cou*100/tot);
			mmcsb->SetWindowText(str);
		}
	}
	if(dx){
		for(i = 1;i<height;i++){
			if(dx*dy==-1){
				j = width-1;
			}
			else{
				j = 0;
			}
			for(k = i;j<width&&j>=0&&k<height&&k>=0;j+=dx,k+=dy){
				if(len == 0){
					len = rand()%bl+1;
					if(bw ==0){
						ln = 0;
					}
					else if(bw == 100){
						ln = 256;
					}
					else if(bw<11){
						ln = rand()%(bw*10);
					}
					else if(bw<95){
						ln = rand()%110+bw-10;
					}
					else{
						ln = rand()%100+(bw+14*(bw-95));
					}
				}
				tcl = SMXG(j,k,ln);
				pix[0][j][k] = GetRValue(tcl);
				pix[1][j][k] = GetGValue(tcl);
				pix[2][j][k] = GetBValue(tcl);
				pdc.SetPixel(j,k,tcl);
				len --;
				cou++;
			}
			str.Format(_T("已完成%d%%"),cou*100/tot);
			mmcsb->SetWindowText(str);
		}
	}
	mmcsb->SetWindowText(_T("就绪"));
	BeiFen();
}
COLORREF CBmpDat::SMXG(int x,int y,int ln){
	int t = bfpix[0][x][y],n = 1;
	if(x>0){
		if(y>0){
			t += bfpix[0][x-1][y-1];
			n++;
		}
		if(y<height-1){
			t += bfpix[0][x-1][y+1];
			n++;
		}
		t += bfpix[0][x-1][y];
		n++;
	}
	if(x<width-1){
		if(y>0){
			t += bfpix[0][x+1][y-1];
			n++;
		}
		if(y<height-1){
			t += bfpix[0][x+1][y+1];
			n++;
		}
		t += bfpix[0][x+1][y-1];
		n++;
	}
	return (t/n)>=(ln)?bgcol:pecol;
}
//////////////////灰度图像
void CBmpDat::HDTX(int s){
	int i,j;
	int temp;
	CString str;
	for(i = 0;i<width;i++){
		for(j = 0;j<height;j++){
			temp = (30*int(pix[0][i][j]) + 59*int(pix[1][i][j]) + 11*int(pix[2][i][j]))/100;
			pix[0][i][j] = temp;
			pix[1][i][j] = temp;
			pix[2][i][j] = temp;
			bfpix[0][i][j] = temp;
			bfpix[1][i][j] = temp;
			bfpix[2][i][j] = temp;
			if(s){
				pdc.SetPixel(i,j,RGB(pix[0][i][j],pix[1][i][j],pix[2][i][j]));
			}
		}
		if(s){
			str.Format(_T("已完成%d%%"),i*100/width);
			mmcsb->SetWindowText(str);
		}
	}
	if(s){

		mmcsb->SetWindowText(_T("就绪"));
		BeiFen();
	}
}
//////////////////缩放///////////////////////////
void CBmpDat::Zoom(int w,int h){
	BeiFen();
	int i,j,oi;
	float temp;
	int f1,f2;
	CString str;
	////////////////////////////////////
	//宽度进行缩放
	for(j = 0;j<height;j++){
		bfpix[0][w-1][j] = pix[0][width-1][j];
		bfpix[1][w-1][j] = pix[1][width-1][j];
		bfpix[2][w-1][j] = pix[2][width-1][j];
//		pix[0][w-1][j] = bfpix[0][width-1][j];
//		pix[1][w-1][j] = bfpix[1][width-1][j];
//		pix[2][w-1][j] = bfpix[2][width-1][j];
//		pdc.SetPixel(w-1,j,RGB(pix[0][w-1][j],pix[1][w-1][j],pix[2][w-1][j]));
	}
	for(i = 0;i<w-1;i++){
		temp = float(i * (width-1)) / float(w-1);
		temp += 0.00001f;//结果校正
		oi = int(temp);
		f2 = int(temp*100) - oi*100;
		f1 = 100 - f2;
		for(j = 0;j<height;j++){
			bfpix[0][i][j] = (pix[0][oi][j]*f1 + pix[0][oi+1][j]*f2)/100;
			bfpix[1][i][j] = (pix[1][oi][j]*f1 + pix[1][oi+1][j]*f2)/100;
			bfpix[2][i][j] = (pix[2][oi][j]*f1 + pix[2][oi+1][j]*f2)/100;
//			pix[0][i][j] = (bfpix[0][oi][j]*f1 + bfpix[0][oi+1][j]*f2)/100;
//			pix[1][i][j] = (bfpix[1][oi][j]*f1 + bfpix[1][oi+1][j]*f2)/100;
//			pix[2][i][j] = (bfpix[2][oi][j]*f1 + bfpix[2][oi+1][j]*f2)/100;
//			pdc.SetPixel(i,j,RGB(pix[0][i][j],pix[1][i][j],pix[2][i][j]));
		}
	}
	////////////////////////////////////*/
	//高度进行缩放
	for(i = 0;i<w;i++){
		pix[0][i][h-1] = bfpix[0][i][height-1];
		pix[1][i][h-1] = bfpix[1][i][height-1];
		pix[2][i][h-1] = bfpix[2][i][height-1];
		pdc.SetPixel(i,h-1,RGB(pix[0][i][h-1],pix[1][i][h-1],pix[2][i][h-1]));
	}
	for(j = 0;j<h-1;j++){
		temp = float(j * (height-1)) / float(h-1);
		temp += 0.00001f;//结果校正
		oi = int(temp);
		f2 = int(temp*100) - oi*100;
		f1 = 100 - f2;
		for(i = 0;i<w;i++){
			pix[0][i][j] = (bfpix[0][i][oi]*f1 + bfpix[0][i][oi+1]*f2)/100;
			pix[1][i][j] = (bfpix[1][i][oi]*f1 + bfpix[1][i][oi+1]*f2)/100;
			pix[2][i][j] = (bfpix[2][i][oi]*f1 + bfpix[2][i][oi+1]*f2)/100;
			pdc.SetPixel(i,j,RGB(pix[0][i][j],pix[1][i][j],pix[2][i][j]));
		}
		str.Format(_T("已完成%d%%"),j*100/h);
		mmcsb->SetWindowText(str);
	}//*/
	for(i = 0;i<w;i++){
		for(j = 0;j<h;j++){
			bfpix[0][i][j] = pix[0][i][j];
			bfpix[1][i][j] = pix[1][i][j];
			bfpix[2][i][j] = pix[2][i][j];
		}
	}
	bfdc.BitBlt(0,0,w,h,&pdc,0,0,SRCCOPY);
	oldwidth = width;
	width = w;
	oldheight = height;
	height = h;
	mmcsb->SetWindowText(_T("就绪"));
}
///////////////////平移/////////////////////
void CBmpDat::Move(int dx,int dy){
	int i,j;
	int tx,ty;
	CPen pen,*op;
	pen.CreatePen(PS_SOLID,796,RGB(255,255,255));
	//////////////////////////////////////使用白色画笔
	op = pdc.SelectObject(&pen);
	//////////////////////////////////////
	pdc.MoveTo(0,393);
	pdc.LineTo(1024,393);
	////////////////////////////////////////
	pdc.SelectObject(op);

	for(i = 0;i<width;i++){
		for(j = 0;j<height;j++){
			tx = i - dx;
			ty = j - dy;
			if(tx>=width||tx<0||ty>=height||ty<0){
				pix[0][i][j] = 255;
				pix[1][i][j] = 255;
				pix[2][i][j] = 255;
			}
			else{
				pix[0][i][j] = bfpix[0][tx][ty];
				pix[1][i][j] = bfpix[1][tx][ty];
				pix[2][i][j] = bfpix[2][tx][ty];
			}
		}
	}
	pdc.BitBlt(dx,dy,width,height,&bfdc,0,0,SRCCOPY);
	BeiFen();
}
/////////////////////////////////
double Max(double n[]){
	double n1,n2;
	n1 = n[0]>n[1]?n[0]:n[1];
	n2 = n[2]>n[3]?n[2]:n[3];
	return n1>n2?n1:n2;
}
double Min(double n[]){
	double n1,n2;
	n1 = n[0]<n[1]?n[0]:n[1];
	n2 = n[2]<n[3]?n[2]:n[3];
	return n1<n2?n1:n2;
}
///////////////旋转//////////////
void CBmpDat::Turn(int de){
	BeiFen();
	double pdec = double(de) * 3.1415926 / 180.0;
	double sind = sin(pdec);
	double cosd = cos(pdec);
	double nx[4] = {0,0,0,0};
	double ny[4] = {0,0,0,0};
	nx[1] = width*cosd - height*sind;
	ny[1] = width*sind + height*cosd;
	nx[2] = -height*sind;
	ny[2] = height*cosd;
	nx[3] = width*cosd;
	ny[3] = width*sind;
	double maxy,miny,maxx,minx;
	maxy = Max(ny);
	maxx = Max(nx);
	miny = Min(ny);
	minx = Min(nx);
	int dx = int(maxx - minx + 0.5);	//新的宽度
	int dy = int(maxy - miny + 0.5);	//新的高度
	int mx = int(minx - 0.5);		//转换后应偏移的横坐标
	int my = int(miny - 0.5);		//转换后应偏移的纵坐标
	sind = -sind;

	int i,j;
	int fx1,fx2,fy1,fy2;
	int tx,ty,tp1[3],tp2[3],tp3[3],tp4[3];
	for(i = 0;i<dx;i++){
		for(j = 0;j<dy;j++){
			minx = (i + mx)*cosd - (j + my)*sind + 0.00001;
			miny = (i + mx)*sind + (j + my)*cosd + 0.00001;
			fx1 = int(minx);
			fy1 = int(miny);
			if(minx<0)
				fx1 --;
			if(miny<0)
				fy1--;
			fx2 = fx1 + 1;
			fy2 = fy1 + 1;
			tx = int(minx*100)-int(minx)*100;
			if(tx<0)
				tx = 100+tx;
			ty = int(miny*100)-int(miny)*100;
			if(ty<0)
				ty = 100+ty;

				if(fx1>=0&&fx1<width){
					if(fy1>=0&&fy1<height){
						tp1[0] = bfpix[0][fx1][fy1];
						tp1[1] = bfpix[1][fx1][fy1];
						tp1[2] = bfpix[2][fx1][fy1];
					}
					else{
						tp1[0] = 255;
						tp1[1] = 255;
						tp1[2] = 255;
					}
					if(fy2<height&&fy2>=0){
						tp2[0] = bfpix[0][fx1][fy2];
						tp2[1] = bfpix[1][fx1][fy2];
						tp2[2] = bfpix[2][fx1][fy2];
					}
					else{
						tp2[0] = 255;
						tp2[1] = 255;
						tp2[2] = 255;
					}
				}
				else{
					tp1[0] = 255;
					tp1[1] = 255;
					tp1[2] = 255;
					tp2[0] = 255;
					tp2[1] = 255;
					tp2[2] = 255;
				}
				if(fx2>=0&&fx2<width){
					if(fy1>=0&&fy1<height){
						tp3[0] = bfpix[0][fx2][fy1];
						tp3[1] = bfpix[1][fx2][fy1];
						tp3[2] = bfpix[2][fx2][fy1];
					}
					else {
						tp3[0] = 255;
						tp3[1] = 255;
						tp3[2] = 255;
					}
					if(fy2>=0&&fy2<height){
						tp4[0] = bfpix[0][fx2][fy2];
						tp4[1] = bfpix[1][fx2][fy2];
						tp4[2] = bfpix[2][fx2][fy2];
					}
					else{
						tp4[0] = 255;
						tp4[1] = 255;
						tp4[2] = 255;
					}
				}
				else{
					tp3[0] = 255;
					tp3[1] = 255;
					tp3[2] = 255;
					tp4[0] = 255;
					tp4[1] = 255;
					tp4[2] = 255;
				}
				pix[0][i][j] = ((100-ty)*(((100-tx)*tp1[0]+tx*tp3[0])/100)+ty*(((100-tx)*tp2[0]+tx*tp4[0])/100))/100;
				pix[1][i][j] = ((100-ty)*(((100-tx)*tp1[1]+tx*tp3[1])/100)+ty*(((100-tx)*tp2[1]+tx*tp4[1])/100))/100;
				pix[2][i][j] = ((100-ty)*(((100-tx)*tp1[2]+tx*tp3[2])/100)+ty*(((100-tx)*tp2[2]+tx*tp4[2])/100))/100;
				pdc.SetPixel(i,j,RGB(pix[0][i][j],pix[1][i][j],pix[2][i][j]));

		}
	}
	for(i = 0;i<dx;i++){
		for(j = 0;j<dy;j++){
			bfpix[0][i][j] = pix[0][i][j];
			bfpix[1][i][j] = pix[1][i][j];
			bfpix[2][i][j] = pix[2][i][j];
		}
	}
	bfdc.BitBlt(0,0,dx,dy,&pdc,0,0,SRCCOPY);
	width = dx;
	height = dy;
}

/////////////////////////////////////////////
void CBmpDat::RH(){
	int i,j;
	CString str;
	for(i = 0;i<width;i++){
		for(j = 0;j<height;j++){
			pix[0][i][j] = RH(0,i,j);
			pix[1][i][j] = RH(1,i,j);
			pix[2][i][j] = RH(2,i,j);
			pdc.SetPixel(i,j,RGB(pix[0][i][j],pix[1][i][j],pix[2][i][j]));
		}
		str.Format(_T("已完成%d%%"),i*100/width);
		mmcsb->SetWindowText(str);
	}
	mmcsb->SetWindowText(_T("就绪"));
	BeiFen();
}
BYTE CBmpDat::RH(int i,int x,int y){
	int temp = 0;
	int count = 1;
	if(x>0){
		temp += int(bfpix[i][x-1][y]);
		count ++;
		if(y>0){
			temp += int(bfpix[i][x-1][y-1]);
			count ++;
		}
		if(y<height-1){
			temp += int(bfpix[i][x-1][y+1]);
			count ++;
		}
	}
	if(y>0){
		temp += int(bfpix[i][x][y-1]);
		count ++;
	}
	if(y<height-1){
		temp += int(bfpix[i][x][y+1]);
		count ++;
	}
	if(x<width){
		temp += int(bfpix[i][x+1][y]);
		count ++;
		if(y>0){
			temp += int(bfpix[i][x+1][y-1]);
			count ++;
		}
		if(y<height-1){
			temp += int(bfpix[i][x+1][y+1]);
			count ++;
		}
	}
	temp = ((count+3) * int(bfpix[i][x][y]) - temp)/4;
	return temp;
}
//////////////////////////////////////////////////////////
void CBmpDat::NOT(){
	int i,j;
	CString str;
	for(i = 0;i<width;i++){
		for(j = 0;j<height;j++){
			pix[0][i][j] = 255-pix[0][i][j];
			pix[1][i][j] = 255-pix[1][i][j];
			pix[2][i][j] = 255-pix[2][i][j];
			pdc.SetPixel(i,j,RGB(pix[0][i][j],pix[1][i][j],pix[2][i][j]));
		}
		str.Format(_T("已完成%d%%"),i*100/width);
		mmcsb->SetWindowText(str);
	}
	mmcsb->SetWindowText(_T("就绪"));
	BeiFen();
}
/////////////////////////////////////////////////////////

void CBmpDat::Copy(){
	int i,j;
	COLORREF clr;
	for(i = 0;i<width;i++){
		for(j = 0;j<height;j++){
			clr = pdc.GetPixel(i,j);
			pix[0][i][j] = GetRValue(clr);
			pix[1][i][j] = GetGValue(clr);
			pix[2][i][j] = GetBValue(clr);
		}
	}
}

////////////////////////////
void CBmpDat::TextTemp(int x,int y,CString s[],int n,int dy){
	pdc.BitBlt(0,0,width,height,&bfdc,0,0,SRCCOPY);
	for(int i = 0;i<n;i++){
		pdc.TextOut(x,y+i*dy,s[i]);
	}
}
void CBmpDat::TextOK(int x,int y,CString s[],int n,int dx[],int dy){
	pdc.BitBlt(0,0,width,height,&bfdc,0,0,SRCCOPY);
	int j,k;
	COLORREF col;
	mmcsb->SetWindowText(_T("文字提交中，请等待..."));
	for(int i = 0;i<n;i++){
		pdc.TextOut(x,y+i*dy,s[i]);
		dx[i] += x;
		for(j = x<0?0:x;j<width||j<dx[i];j++){
			for(k = y<0?0:y;k<height||k<y+i*dy;k++){
				col = pdc.GetPixel(j,k);
				pix[0][j][k] = GetRValue(col);
				pix[1][j][k] = GetGValue(col);
				pix[2][j][k] = GetBValue(col);
			}
		}
	}
	mmcsb->SetWindowText(_T("就绪"));
	BeiFen();
}

void CBmpDat::SetFont(LOGFONT & lpc){
	CFont * cf,*ocf;
	cf = new CFont;
	cf->CreateFontIndirect(&lpc);
	ocf = pdc.SelectObject(cf);
	ocf->DeleteObject();
}
void CBmpDat::GetFont(LOGFONT &lpc){
	pdc.GetCurrentFont()->GetLogFont(&lpc);
}
///////////////////////////
void CBmpDat::Size(int w,int h){
	BeiFen();
	int minw,maxw;
	int minh,maxh;
	if(w>width){
		maxw = w;
		minw = width;
	}
	else{
		maxw = width;
		minw = w;
	}
	if(h>height){
		maxh = h;
		minh = height;
	}
	else{
		minh = h;
		maxh = height;
	}
	int i,j;
	for(i = minw;i<maxw;i++){
		for(j = 0;j<minh;j++){
			pix[0][i][j] = 255;
			pix[1][i][j] = 255;
			pix[2][i][j] = 255;
			bfpix[0][i][j] = 255;
			bfpix[1][i][j] = 255;
			bfpix[2][i][j] = 255;
		}
	}
	for(j = minh;j<maxh;j++){
		for(i = 0;i<maxw;i++){
			pix[0][i][j] = 255;
			pix[1][i][j] = 255;
			pix[2][i][j] = 255;
			bfpix[0][i][j] = 255;
			bfpix[1][i][j] = 255;
			bfpix[2][i][j] = 255;
		}
	}
	CDC mdc;
	mdc.CreateCompatibleDC(&pdc);
	CBitmap tb,*ob;
	tb.LoadBitmap(IDB_BBB);
	ob = mdc.SelectObject(&tb);
	pdc.BitBlt(minw,0,maxw-minw+1,maxh,&mdc,0,0,SRCCOPY);
	pdc.BitBlt(0,minh,maxw,maxh-minh+1,&mdc,0,0,SRCCOPY);
	bfdc.BitBlt(minw,0,maxw-minw+1,maxh,&mdc,0,0,SRCCOPY);
	bfdc.BitBlt(0,minh,maxw,maxh-minh+1,&mdc,0,0,SRCCOPY);
	mdc.SelectObject(ob);
	width = w;
	height = h;
}
///////////////////////////
void CBmpDat::TEST(int n){
	int i,j;
	COLORREF clr;
	CString str;
	HDTX(0);
	int x;int y;
	for(i = 0;i<width;i++){
		for(j = 0;j<height;j++){
			x = i/n*n,y = j/n*n;
			clr = (bfpix[0][x][y]>255-((i%n)*(i%n)+(j%n)*(j%n)+1)*245/((n-1)*(n-1)+1)+5)?bgcol:pecol;
			pdc.SetPixel(i,j,clr);
			pix[0][i][j] = GetRValue(clr);
			pix[1][i][j] = GetGValue(clr);
			pix[2][i][j] = GetBValue(clr);
		}
		str.Format(_T("已完成%d%%"),i*100/width);
		mmcsb->SetWindowText(str);
	}
	mmcsb->SetWindowText(_T("就绪"));
	BeiFen();
}
