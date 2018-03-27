#ifndef __SIMPLE_DIB__
#define __SIMPLE_DIB__

#include <fstream>
using namespace std;

//����ֻ������ȡ���洢8λ�Ҷ�ͼ��24λ���ɫͼ
class SimpleDIB
{
public:
	SimpleDIB()
	{
		pbfh = NULL;
		pbi = NULL;//BITMAPINFO
		pbih = NULL;
		pQuad = NULL;
		pData = NULL;
	}

	~SimpleDIB()
	{
		clear();
	}

	//������ٵ��ڴ�
	void clear()
	{
		if(pbfh != NULL)
			delete pbfh;
		if(pbi != NULL)
			delete[] pbi;
		if(pData != NULL)
			delete[] pData;
		
		pbfh = NULL;
		pbi = NULL;//BITMAPINFO
		pbih = NULL;
		pQuad = NULL;
		pData = NULL;

	}

	//�жϵ�ǰ��û����������һ��ͼƬ
	BOOL valid()
	{
		if(pbfh == NULL)
			return FALSE;
		if(pbi == NULL)
			return FALSE;
		if(pbih == NULL)
			return FALSE;
		if(pQuad == NULL)
			return FALSE;
		if(pData == NULL)
			return FALSE;

		return TRUE;
	}


	//��λͼ�ж�ȡ����
	BOOL ReadDIB(char* fileName)
	{
		clear();
		//��Ҫ��д���ļ�
		ifstream bmpFile;
		bmpFile.open(fileName,ios::in|ios::binary);
		if(bmpFile.fail())
			return FALSE;
		
		pbfh = new BITMAPFILEHEADER;

		//����BITMAPINFOHEADER+RGBQUAD������Ҫ�Ŀռ䡣��Ϊ����ֻ��8λ��24λλͼ���ж�ȡ
		//����8λλͼ����256����ɫ�����24λλͼû�е�ɫ��
		//����ֻ��Ϊ�˷��㣬���������Ŀռ䣬��ʵ�ʳ����������ʵ����Ҫ���ٵ�ɫ������ڴ�ռ䣡��
		pbi = new char[sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD)];
		
		//pbi�������ʼ�ĵط�ΪBITMAPINFOHEADER������ΪRGBQUAD
		pbih = (BITMAPINFOHEADER*) pbi;
		pQuad = (RGBQUAD*)(pbi+sizeof(BITMAPINFOHEADER));
		

		//��ȡ�ļ�ͷ��Ϣ
		bmpFile.read((char*)pbfh,sizeof(BITMAPFILEHEADER));
		//�ж��ǲ���λͼ�������˳�
		if(pbfh->bfType != 0x4d42) //'BM"
			return FALSE;

		//��ȡλͼ��Ϣͷ
		bmpFile.read((char*)pbih,sizeof(BITMAPINFOHEADER));

		//ֻ��8λ��24λλͼ
		if(pbih->biBitCount != 8 && pbih->biBitCount != 24)
			return FALSE;

		//�����8λλͼ�����ֶ�biClrUsed==0����ʱ��Ҫ����ɫ����Ϣ
		if(pbih->biBitCount == 8 && pbih->biClrUsed == 0)
			bmpFile.read((char*)pQuad,256*sizeof(RGBQUAD));

		//��ȡλͼ���ݣ���Ϊpbih->biSizeImage�����ɿ������Ի���Ҫ�Լ�����һ��
		int lwidth = widthAlign(pbih->biWidth,pbih->biBitCount);
		int sizeImage = lwidth*pbih->biHeight;
		pData = new BYTE[sizeImage];
		bmpFile.read((char*)pData,sizeImage);

		//�ر��ļ�
		bmpFile.close();

		return TRUE;
	}

	//�˺������ڽ�pbmpdata��λͼ����ʽд���ļ�fileName��
	//��Ҫ�Ĳ����У�pbmpdata��λͼ���ݵ�λ��bitcount��������ֻ֧��8λ��24λ
	//pbmpdata��λͼ���ݵĿ��width��pbmpdata��λͼ���ݵĸ߶�height
	//ע�⣬pbmpdata����Ϊ��λͼ���ݣ�������ÿ��4�ֽڶ����������ݣ��˺��������4�ֽڶ����������书��
	static BOOL WriteDIB(char* fileName,BYTE* pbmpdata,int bitcount,int width,int height)
	{
		if(pbmpdata == NULL)
			return FALSE;
		//ֻ֧��д��8λ��24λλͼ
		if(bitcount != 8 && bitcount != 24)
			return FALSE;

		//����һ������ʵ�ʴ����ļ���ʱ���ռ�ö����ֽ�
		int lwidth = widthAlign(width,bitcount);
		//�����ļ����ܳ���
		int sizeImage = lwidth*height;

		//�����ɫ���������Ĵ�С
		int rgbQuadSize = 0;
		if(bitcount == 8)
			rgbQuadSize = 256*sizeof(RGBQUAD);

		//�����ļ�ͷ
		BITMAPFILEHEADER bfh;
        bfh.bfType = 0x4d42;	//'BM'
        //�ļ��ĳ���
		bfh.bfSize = 0;//sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+rgbQuadSize+sizeImage;
        bfh.bfReserved1 = 0;
        bfh.bfReserved2 = 0;
        //������ļ�ͷ��λͼ����֮���ƫ����
		bfh.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+rgbQuadSize;

		
		//����λͼ��Ϣͷ
		BITMAPINFOHEADER bih;
		bih.biSize = sizeof(BITMAPINFOHEADER);
        bih.biWidth = width;
        bih.biHeight = height;
        bih.biPlanes = 1;//����Ϊ1
        bih.biBitCount = bitcount;
        bih.biCompression = BI_RGB;//δѹ����BMP����
        bih.biSizeImage = 0;//sizeImage;������0
        bih.biXPelsPerMeter = 0;//������0
        bih.biYPelsPerMeter = 0;//������0
        bih.biClrUsed = 0;
        bih.biClrImportant = 0;
		
		ofstream bmpFile(fileName,ios::out|ios::binary);
		bmpFile.write((char*)&bfh,sizeof(bfh));//д���ļ���Ϣͷ
		bmpFile.write((char*)&bih,sizeof(bih));//д��λͼ��Ϣͷ

		//�����8λλͼ����Ҫд���ɫ����Ϣ,ע�⣬�˴���ɫ��ĸ�ֵ����ֻ������8λ�Ҷ�ͼ��
		if(bitcount == 8)
		{
			for(int i=0;i<256;i++)
			{
				RGBQUAD rgbQuad;
				rgbQuad.rgbBlue = i;
				rgbQuad.rgbGreen = i;
				rgbQuad.rgbRed = i;
				rgbQuad.rgbReserved = 0;

				bmpFile.write((char*)&rgbQuad,sizeof(RGBQUAD));
			}
		}

		//����Ϊ��λд��λͼ���ݱ���
		BYTE* lineData = new BYTE[lwidth];//ÿһ��ʵ��д���ļ���ʱ��Ҫ�����ֽڣ��Ǿ���4�ֽڶ����������
		BYTE* ptmp = pbmpdata;	//��ʱָ�룬����ָ��д���ļ���λͼ����
		int bytesPerLine = width*bitcount/8; //ʵ�ʵ�λͼ���ݣ�û�о���4�ֽڶ������
		
		for(int i=0;i<height;i++)//����Ϊ��λдλͼ����
		{
			memset(lineData,0,lwidth);
			memcpy(lineData,ptmp,bytesPerLine);
			ptmp += bytesPerLine;

			bmpFile.write((char*)lineData,lwidth);
		}

		delete[] lineData;

		bmpFile.close();

		return TRUE;
	}

#ifdef _WINDOWS
	//left,top,width,heightȷ�����Ƶ�����,����Ĵ�С��ͼ���С��ͬ
	//���width,height����Ϊ-1������ͼ����Ŀ���
	void Draw(CDC* pDC,int left=0,int top=0)
	{
		if(valid() == FALSE)
			return;

		int	width= pbih->biWidth;

		int	height = pbih->biHeight;

		StretchDIBits(pDC->GetSafeHdc(),left,top,width,height,
			0,0,pbih->biWidth,pbih->biHeight,pData,
			(BITMAPINFO*)pbi,DIB_RGB_COLORS,SRCCOPY); 
	}

	//�����صĻ���ͼƬ�����ڳ�����ԣ�ʵ�ʻ�ͼ��ʱ�򲻻���ô��
	void DrawByPixel(CDC* pDC,int left=0,int top=0)
	{
		if(valid() == FALSE)
			return;

		//����һ������ʵ�ʴ����ļ���ʱ���ռ�ö����ֽ�
		int lwidth = widthAlign(pbih->biWidth,pbih->biBitCount);
		BYTE* lineData = new BYTE[lwidth];
		
		if(pbih->biBitCount == 8)//�Ҷ�ͼ
		{
			for(int y=0;y<pbih->biHeight;y++)
			{
				memcpy(lineData, pData+lwidth*y, lwidth);
				BYTE* ptmp = lineData;

				for(int x=0;x<pbih->biWidth;x++)
				{
					BYTE c = *ptmp++;
					RGBQUAD rgbQuad = pQuad[c];
					//��Ϊλͼ�洢��ʱ�������µߵ��ģ����������ʱ��yֵΪheight-y
					pDC->SetPixel(x+left,pbih->biHeight-y-1+top,RGB(rgbQuad.rgbRed,rgbQuad.rgbGreen,rgbQuad.rgbBlue));
				}
			}
		}

		if(pbih->biBitCount == 24)//���ɫͼ
		{
			for(int y=0;y<pbih->biHeight;y++)
			{
				memcpy(lineData, pData+lwidth*y, lwidth);
				BYTE* ptmp = lineData;

				for(int x=0;x<pbih->biWidth;x++)
				{
					BYTE b = *ptmp++;
					BYTE g = *ptmp++;
					BYTE r = *ptmp++;

					pDC->SetPixel(x+left,pbih->biHeight-y-1+top,RGB(r,g,b));
				}
			}
		}

		delete[] lineData;

	}
#endif

	//ÿ�������ڴ����ļ���ʱ�������4�ֽڶ���ģ�������4�ֽڱ�����ʱ�������䣬��ʹ�ܳ���Ϊ4��λ��
	//��һ����3�����أ�ÿ������24λ��3�ֽڣ������ܳ���Ϊ9�ֽڣ�ʵ�ʴ����ļ���ʱ�����Ϊ12�ֽ�
	static int widthAlign(int width,int bitcount)
	{
		int lwidth = (width*bitcount/8+3)/4*4;
		return lwidth;
	}

public:	//ע�⣬�˴���publicֻ��Ϊ�˳�����÷��㣬��ʵ�ʱ����Ӧ����private��protected
	BITMAPFILEHEADER* pbfh;
	char* pbi;//BITMAPINFO
	BITMAPINFOHEADER* pbih;
	RGBQUAD*	pQuad;
	BYTE*		pData;
};

#endif //__SIMPLE_DIB__