
// 915108320104_expe2View.cpp : CMy915108320104_expe2View 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "915108320104_expe2.h"
#endif

#include "915108320104_expe2Doc.h"
#include "915108320104_expe2View.h"
#include "simpleDIB.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMy915108320104_expe2View

IMPLEMENT_DYNCREATE(CMy915108320104_expe2View, CView)

BEGIN_MESSAGE_MAP(CMy915108320104_expe2View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMy915108320104_expe2View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CMy915108320104_expe2View 构造/析构

CMy915108320104_expe2View::CMy915108320104_expe2View()
{
	// TODO: 在此处添加构造代码

}

CMy915108320104_expe2View::~CMy915108320104_expe2View()
{
}

BOOL CMy915108320104_expe2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMy915108320104_expe2View 绘制

void CMy915108320104_expe2View::OnDraw(CDC* pDC)
{
	CMy915108320104_expe2Doc* pDoc = GetDocument();
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
	int mapp[256][256] = { 0 };
	int newmapp[256][256] = { 0 };
	int allnewmapp[256][256] = { 0 };
	for (int i = 0; i<256; i++) {
		for (int j = 0; j<256; j++) {
			BYTE c = *pdata;   //像素值的指针
			pdata++;
			histogram[c]++;
			allnewmapp[i][j] = newmapp[i][j] = mapp[i][j] = int(c);
		}
	}
	//设置梯度阈值为20  梯度锐化法
	for (int i = 0; i<255; i++) {
		for (int j = 0; j<255; j++) {
			if ( (mapp[i][j]+120) < 255 )
			{
				int tmp = abs(mapp[i][j] - mapp[i + 1][j + 1]) + abs(mapp[i + 1][j] - mapp[i][j + 1]);
				if (tmp >= 20)
				{
					newmapp[i][j] = 0;
				}
				else {
					newmapp[i][j] = 255;   //突显出物体的轮廓
				}
			}
		}
	}

	for (int i = 0; i<256; i++) {
		for (int j = 0; j<256; j++) {
			int tmp = newmapp[i][j];
			pDC->SetPixel(j, 300 + height - i, RGB(tmp, tmp, tmp));
		}
	}


	//laplacian增强算子
	for (int i = 1; i<255; i++) {
		for (int j = 1; j<255; j++) {
			allnewmapp[i][j] = 5 * mapp[i][j] - mapp[i + 1][j] - mapp[i - 1][j] - mapp[i][j + 1] - mapp[i][j - 1];
		}
	}

	for (int i = 0; i<256; i++) {
		for (int j = 0; j<256; j++) {
			int tmp = allnewmapp[i][j];
			pDC->SetPixel(width + 5 + j, 300 + height - i, RGB(tmp, tmp, tmp));
		}
	}

	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CMy915108320104_expe2View 打印


void CMy915108320104_expe2View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMy915108320104_expe2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMy915108320104_expe2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMy915108320104_expe2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CMy915108320104_expe2View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMy915108320104_expe2View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMy915108320104_expe2View 诊断

#ifdef _DEBUG
void CMy915108320104_expe2View::AssertValid() const
{
	CView::AssertValid();
}

void CMy915108320104_expe2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMy915108320104_expe2Doc* CMy915108320104_expe2View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy915108320104_expe2Doc)));
	return (CMy915108320104_expe2Doc*)m_pDocument;
}
#endif //_DEBUG


// CMy915108320104_expe2View 消息处理程序
