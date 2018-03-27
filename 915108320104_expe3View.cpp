
// 915108320104_expe3View.cpp : CMy915108320104_expe3View ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMy915108320104_expe3View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CMy915108320104_expe3View ����/����

CMy915108320104_expe3View::CMy915108320104_expe3View()
{
	// TODO: �ڴ˴���ӹ������

}

CMy915108320104_expe3View::~CMy915108320104_expe3View()
{
}

BOOL CMy915108320104_expe3View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CMy915108320104_expe3View ����

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
			BYTE c = *pdata;   //���ؾ����ָ��
			pdata++;
			histogram[c]++;
			sum += c;
		}
	}
	//�Զ���ֵ������
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
			BYTE c = *pdata;   //����ֵ��ָ��
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
		//����ֵΪnTʱ������c0��ľ�ֵ�͸���   
		u0 = u1 = cnt0 = 0;
		for (int i = 0; i <= tmpT; i++)
		{
			u0 += i * histogram[i];
			cnt0 += histogram[i];
		}
		u0 /= cnt0;
		w0 = (double)cnt0 / 256 / 256;
		//����ֵΪnTʱ������c1��ľ�ֵ�͸���   
		for (int i = tmpT + 1; i < 256; i++)
		{
			u1 += i * histogram[i];
		}
		u1 /= (sum - cnt0);
		w1 = 1.0 - w0;
		tmpVaria = w0*w1*(u0 - u1)*(u0 - u1);//���������ķ���      
		if (tmpVaria > ansVaria)//��¼��󷽲�������ֵ   
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
			BYTE c = *pdata;   //����ֵ��ָ��
			pdata++;
			if (int(c) > ansT)
				pDC->SetPixel(width * 2 + 200 + j, height - i, RGB(255, 255, 255));
			else
				pDC->SetPixel(width * 2 + 200 + j, height - i, RGB(0, 0, 0));
		}
	}
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CMy915108320104_expe3View ��ӡ


void CMy915108320104_expe3View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMy915108320104_expe3View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CMy915108320104_expe3View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CMy915108320104_expe3View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
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


// CMy915108320104_expe3View ���

#ifdef _DEBUG
void CMy915108320104_expe3View::AssertValid() const
{
	CView::AssertValid();
}

void CMy915108320104_expe3View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMy915108320104_expe3Doc* CMy915108320104_expe3View::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy915108320104_expe3Doc)));
	return (CMy915108320104_expe3Doc*)m_pDocument;
}
#endif //_DEBUG


// CMy915108320104_expe3View ��Ϣ�������
