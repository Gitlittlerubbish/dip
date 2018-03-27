
// 915108320104_expe3View.cpp : CMy915108320104_expe3View 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "915108320104_expe3.h"
#endif

#include "915108320104_expe3Doc.h"
#include "915108320104_expe3View.h"
#include "simpleDIB.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMy915108320104_expe3View

IMPLEMENT_DYNCREATE(CMy915108320104_expe3View, CView)

BEGIN_MESSAGE_MAP(CMy915108320104_expe3View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMy915108320104_expe3View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CMy915108320104_expe3View 构造/析构

CMy915108320104_expe3View::CMy915108320104_expe3View()
{
	// TODO: 在此处添加构造代码

}

CMy915108320104_expe3View::~CMy915108320104_expe3View()
{
}

BOOL CMy915108320104_expe3View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMy915108320104_expe3View 绘制

void CMy915108320104_expe3View::OnDraw(CDC* pDC)
{
	CMy915108320104_expe3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	SimpleDIB dib;
	dib.ReadDIB("1.bmp");
	int width = dib.pbih->biWidth;
	int height = dib.pbih->biHeight;
	int bitcount = dib.pbih->biBitCount;  //8bit bmp
	dib.Draw(pDC);
	CString str;
	str = "origin";
	pDC->TextOut(width / 2, height + 5, str);
	if (bitcount != 8)
		return;
	BYTE* pdata = dib.pData;
	int histogram[256] = { 0 };
	int ave = 0, ave2 = 0, ave1 = 0, sum = 0, sum1, sum11, sum2, sum22;

	for (int i = 0; i<256; i++) {
		for (int j = 0; j<256; j++) {
			BYTE c = *pdata;   //像素矩阵的指针
			pdata++;
			histogram[c]++;
			sum += c;
		}
	}
	//自动阈值迭代法
	ave = sum / width / height;
	int tmp;
	while (1)
	{
		sum1 = sum2 = sum11 = sum22 = 0;
		for (int i = 1; i < ave; i++)
		{
			sum1 += (histogram[i] * i);
			sum11 += histogram[i];
		}
		ave1 = sum1 / sum11;
		for (int i = ave; i < 256; i++)
		{
			sum2 += (histogram[i] * i);
			sum22 += histogram[i];
		}
		ave2 = sum2 / sum22;
		tmp = (ave1 + ave2) / 2;
		if (tmp == ave)
			break;
		ave = tmp;
	}
	str.Format(_T("%d"), ave);
	pDC->TextOut(width, height + 5, str);

	pdata = dib.pData;
	for (int i = 0; i<256; i++) {
		for (int j = 0; j<256; j++) {
			BYTE c = *pdata;   //像素值的指针
			pdata++;
			if (int(c) > ave)
				pDC->SetPixel(width + 200 + j, height - i, RGB(255, 255, 255));
			else
				pDC->SetPixel(width + 200 + j, height - i, RGB(0, 0, 0));
		}
	}


	//Otsu method
	double w0, w1, u0, u1;
	int tmpT, ansT, cnt0;
	double tmpVaria, ansVaria = 0;
	for (tmpT = 1; tmpT<256; tmpT++)
	{
		//当阈值为nT时，计算c0组的均值和概率   
		u0 = u1 = cnt0 = 0;
		for (int i = 0; i <= tmpT; i++)
		{
			u0 += i * histogram[i];
			cnt0 += histogram[i];
		}
		u0 /= cnt0;
		w0 = (double)cnt0 / 256 / 256;
		//当阈值为nT时，计算c1组的均值和概率   
		for (int i = tmpT + 1; i < 256; i++)
		{
			u1 += i * histogram[i];
		}
		u1 /= (sum - cnt0);
		w1 = 1.0 - w0;
		tmpVaria = w0*w1*(u0 - u1)*(u0 - u1);//计算两组间的方差      
		if (tmpVaria > ansVaria)//记录最大方差和最佳阈值   
		{
			ansVaria = tmpVaria;
			ansT = tmpT;
		}
	}
	str.Format(_T("%d"), ansT);
	pDC->TextOut(width / 2, height + 5, str);
	pdata = dib.pData;
	for (int i = 0; i<256; i++) {
		for (int j = 0; j<256; j++) {
			BYTE c = *pdata;   //像素值的指针
			pdata++;
			if (int(c) > ansT)
				pDC->SetPixel(width * 2 + 200 + j, height - i, RGB(255, 255, 255));
			else
				pDC->SetPixel(width * 2 + 200 + j, height - i, RGB(0, 0, 0));
		}
	}
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CMy915108320104_expe3View 打印


void CMy915108320104_expe3View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMy915108320104_expe3View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMy915108320104_expe3View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMy915108320104_expe3View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CMy915108320104_expe3View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMy915108320104_expe3View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMy915108320104_expe3View 诊断

#ifdef _DEBUG
void CMy915108320104_expe3View::AssertValid() const
{
	CView::AssertValid();
}

void CMy915108320104_expe3View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMy915108320104_expe3Doc* CMy915108320104_expe3View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy915108320104_expe3Doc)));
	return (CMy915108320104_expe3Doc*)m_pDocument;
}
#endif //_DEBUG


// CMy915108320104_expe3View 消息处理程序
