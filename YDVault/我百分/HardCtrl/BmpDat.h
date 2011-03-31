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
	void TC(int x,int y,int m = 0);//���
	void CZ(int cn,int a[],int cr,int cg,int cb);//cn�������ͣ�a����,cr,cg,cb��������
	BYTE XZ(int i,int x,int y,int cn,int a[]);
	BYTE BY(int i,int x,int y);
	BYTE FD(int i,int x,int y);
	BYTE MH(int i,int x,int y);
	BYTE LB(int i,int x,int y,int z);
	BYTE LA(int i,int x,int y,int z,int c);
	void BY();
	void FD();
	void HDTX(int s = 1);//�Ҷ�ͼ��
	void DBD();//�Աȶ�����
	void SMXG(int pl,int bw,int dx,int dy);//����
	COLORREF SMXG(int x,int y,int ln);
	void RH();//��
	BYTE RH(int i,int x,int y);
	void Clear(int x,int y);
	void Darw(int x,int y,int m = 0);
	void Line(int sx,int sy,int tx,int ty,int m = 0);
	void ReLine(int sx,int sy,int tx,int ty,int c,int m);
	void Line(int sx,int sy,int tx,int ty,int c,int dx,int dy,COLORREF col);
	void DrawBezier(float x[4],float y[4],COLORREF col);	////
	void Bezier(int sx[4],int sy[4],int m = 0);				//   ������
	void ReBezier(int sx[4],int sy[4],int m = 0);			////
	BOOL UnDoOK(){return undo>0?TRUE:FALSE;}
	BOOL ReDoOK(){return redo<2?TRUE:FALSE;}
	void UnDo();
	void ReDo();
	void NOT();//��ɫ
	/////////////////
	void Zoom(int w,int h);
	void Move(int dx,int dy);
	void Turn(int de);
	void Size(int w,int h);
	/////////////////
	void GetFont(LOGFONT & lpc);//��ȡ����
	void SetFont(LOGFONT & lpc);//��������
	void TextTemp(int x,int y,CString s[],int n,int dy);//�����ַ�������
	void TextOK(int x,int y,CString s[],int n,int dx[],int dy);//�����ַ����
	////////////////////////
	void TEST(int n);
	///////////////
	void Copy();
public:
	COLORREF bgcol;
	COLORREF pecol;
private:
	CDC pdc;//������ʾ��DC
	CDC bfdc;//���ݵ�DC
	int undo;//����
	int redo;//����
	CDC mdc[2];//����undo and redo
	int lastw[2];
	int lasth[2];
	BYTE pix[3][1024][786];//ͼ������
	BYTE bfpix[3][1024][786];//����ͼ������
	BYTE mpix[2][3][1024][786];
	int width;//ͼ���
	int height;//ͼ���
	int oldwidth;
	int oldheight;
	CString path;//·��
	CBitmap bp,bp2,bp3,bp4;
};

#endif // !defined(AFX_BMPDAT_H__62268881_55E3_41BB_95B7_2B43B869E36A__INCLUDED_)
