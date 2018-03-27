
// 915108320104_1View.cpp : CMy915108320104_1View 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "915108320104_1.h"
#endif

#include "915108320104_1Doc.h"
#include "915108320104_1View.h"
#include "simpleDIB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMy915108320104_1View

IMPLEMENT_DYNCREATE(CMy915108320104_1View, CView)

BEGIN_MESSAGE_MAP(CMy915108320104_1View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMy915108320104_1View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CMy915108320104_1View 构造/析构

CMy915108320104_1View::CMy915108320104_1View()
{
	// TODO: 在此处添加构造代码

}

CMy915108320104_1View::~CMy915108320104_1View()
{
}

BOOL CMy915108320104_1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMy915108320104_1View 绘制

void CMy915108320104_1View::OnDraw(CDC* pDC)
{
	CMy915108320104_1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	SimpleDIB dib;
	dib.ReadDIB("1.bmp");
	int width = dib.pbih->biWidth;
	int height = dib.pbih->biHeight;
	int bitcount = dib.pbih->biBitCount;  //8bit bmp
	dib.Draw(pDC);

	if (bitcount != 8)
		return;
	BYTE* pdata = dib.pData;
	int histogram[256] = { 0 };
	int histogram2[256] = { 0 };
	for (int i = 0; i<256; i++) {
		for (int j = 0; j<256; j++) {
			BYTE c = *pdata;   //像素值的指针
			pdata++;
			histogram[c]++;
		}
	}
	for (int i = 0; i<256; i++) {
		histogram[i] /= 20;
	}
	for (int i = 0; i<256; i++) {
		pDC->MoveTo(i + 2 * width, 500);
		pDC->LineTo(i + 2 * width, 500 - histogram[i]);
	}

	pdata = dib.pData;
	// linear transfer
	for (int i = 0; i<256; i++) {
		for (int j = 0; j<256; j++) {
			BYTE c = *pdata;
			pdata++;
			if (c<50) {
				pDC->SetPixel(j, 300 + height - i, RGB(0, 0, 0));
			}
			else if (c<150) {
				BYTE tmp = BYTE(1.0 * 255 / 100 * (c - 50));
				pDC->SetPixel(j, 300 + height - i, RGB(tmp, tmp, tmp));
			}
			else {
				pDC->SetPixel(j, 300 + height - i, RGB(255, 255, 255));
			}
		}
	}



	pdata = dib.pData;
	double Sk[256] = {0};
	double Sktmp[256]={0};
	int HistEqual[256] = {0};
	//统计灰度频率
    for (int i = 0; i < 256; i++)
    {
        Sk[i] = ((double)histogram[i]*20/width/height);
    }

    //计算累计密度
    Sktmp[0] = Sk[0];
    for (int i = 1; i < 256; i++)
    {
        Sktmp[i] = Sktmp[i-1] +Sk[i];
    }
    //重新计算均衡化后的灰度值，四舍五入。参考公式：(N-1)*T+0.5
    for (int i = 0; i < 256; i++)
    {
        HistEqual[i] = int((255 * Sktmp[i] + 0.5));
    }
    //直方图均衡化,更新原图每个点的像素值
	for (int i = 0; i<256; i++) {
		for (int j = 0; j<256; j++) {
			BYTE c = *pdata;   //像素值的指针
			int tmp;
			pdata++;
            tmp = HistEqual[c];
            histogram2[tmp]++;
			pDC->SetPixel(width+200+j, height-i, RGB(tmp, tmp, tmp));
		}
	}
	for (int i = 0; i<256; i++) {
		pDC->MoveTo(i + 4 * width, 500);
		pDC->LineTo(i + 4 * width, 500 - histogram2[i]/20);
	}


	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CMy915108320104_1View 打印


void CMy915108320104_1View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMy915108320104_1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMy915108320104_1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMy915108320104_1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CMy915108320104_1View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMy915108320104_1View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMy915108320104_1View 诊断

#ifdef _DEBUG
void CMy915108320104_1View::AssertValid() const
{
	CView::AssertValid();
}

void CMy915108320104_1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMy915108320104_1Doc* CMy915108320104_1View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy915108320104_1Doc)));
	return (CMy915108320104_1Doc*)m_pDocument;
}
#endif //_DEBUG


// CMy915108320104_1View 消息处理程序
