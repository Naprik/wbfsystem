// BmpDat.h: interface for the CBmpDat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BMPDAT_H__62268881_55E3_41BB_95B7_2B43B869E36A__INCLUDED_)
#define AFX_BMPDAT_H__62268881_55E3_41BB_95B7_2B43B869E36A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBmpDat  
{
public:
	CBmpDat(CDC * dc);
	virtual ~CBmpDat();
	void New(int w,int h);
	void Open(CString p);
	CDC * GetDC(){return &pdc;}
	int GetH(){return height;}
	int GetW(){return width;}
	void SetPen(COLORREF col);
	int Save(CString p = "");
	void Cancel();
	void BeiFen();
	void TC(int x,int y,int m = 0);//填充
	void CZ(int cn,int a[],int cr,int cg,int cb);//cn处理类型，a参数,cr,cg,cb三个分量
	BYTE XZ(int i,int x,int y,int cn,int a[]);
	BYTE BY(int i,int x,int y);
	BYTE FD(int i,int x,int y);
	BYTE MH(int i,int x,int y);
	BYTE LB(int i,int x,int y,int z);
	BYTE LA(int i,int x,int y,int z,int c);
	void BY();
	void FD();
	void HDTX(int s = 1);//灰度图像
	void DBD();//对比度增加
	void SMXG(int pl,int bw,int dx,int dy);//素描
	COLORREF SMXG(int x,int y,int ln);
	void RH();//锐化
	BYTE RH(int i,int x,int y);
	void Clear(int x,int y);
	void Darw(int x,int y,int m = 0);
	void Line(int sx,int sy,int tx,int ty,int m = 0);
	void ReLine(int sx,int sy,int tx,int ty,int c,int m);
	void Line(int sx,int sy,int tx,int ty,int c,int dx,int dy,COLORREF col);
	void DrawBezier(float x[4],float y[4],COLORREF col);	////
	void Bezier(int sx[4],int sy[4],int m = 0);				//   贝塞尔
	void ReBezier(int sx[4],int sy[4],int m = 0);			////
	BOOL UnDoOK(){return undo>0?TRUE:FALSE;}
	BOOL ReDoOK(){return redo<2?TRUE:FALSE;}
	void UnDo();
	void ReDo();
	void NOT();//反色
	/////////////////
	void Zoom(int w,int h);
	void Move(int dx,int dy);
	void Turn(int de);
	void Size(int w,int h);
	/////////////////
	void GetFont(LOGFONT & lpc);//获取字体
	void SetFont(LOGFONT & lpc);//设置字体
	void TextTemp(int x,int y,CString s[],int n,int dy);//输入字符过程中
	void TextOK(int x,int y,CString s[],int n,int dx[],int dy);//输入字符完成
	////////////////////////
	void TEST(int n);
	///////////////
	void Copy();
public:
	COLORREF bgcol;
	COLORREF pecol;
private:
	CDC pdc;//用于显示的DC
	CDC bfdc;//备份的DC
	int undo;//撤消
	int redo;//重做
	CDC mdc[2];//用于undo and redo
	int lastw[2];
	int lasth[2];
	BYTE pix[3][1024][786];//图像像素
	BYTE bfpix[3][1024][786];//备份图像像素
	BYTE mpix[2][3][1024][786];
	int width;//图像宽
	int height;//图像高
	int oldwidth;
	int oldheight;
	CString path;//路径
	CBitmap bp,bp2,bp3,bp4;
};

#endif // !defined(AFX_BMPDAT_H__62268881_55E3_41BB_95B7_2B43B869E36A__INCLUDED_)
