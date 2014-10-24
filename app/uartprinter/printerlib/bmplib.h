/* name: bmplib.h
 * description: BMP��
 * author: zuokongxiao
 * date: 2014-10-23
 */

#ifndef __BMPLIB_H
#define __BMPLIB_H

typedef unsigned char  U8;
typedef unsigned short U16;
typedef unsigned int   U32;
 
typedef struct tagBITMAPFILEHEADER
{
 
 U16 bfType;
 U32 bfSize;
 U16 bfReserved1;
 U16 bfReserved2;
 U32 bfOffBits;
} BITMAPFILEHEADER;
 
typedef struct tagBITMAPINFOHEADER
{
 U32 biSize;
 U32 biWidth;
 U32 biHeight;
 U16 biPlanes;
 U16 biBitCount;
 U32 biCompression;
 U32 biSizeImage;
 U32 biXPelsPerMeter;
 U32 biYPelsPerMeter;
 U32 biClrUsed;
 U32 biClrImportant;
} BITMAPINFOHEADER;
 
typedef struct tagRGBQUAD
{
 U8 rgbBlue;
 U8 rgbGreen;
 U8 rgbRed;
 U8 rgbReserved;
} RGBQUAD;
 
typedef struct tagBITMAPINFO
{
 BITMAPINFOHEADER bmiHeader;
 RGBQUAD bmiColors[1];
} BITMAPINFO;
 
 
typedef struct tagBITMAP
{
 BITMAPFILEHEADER bfHeader;
 BITMAPINFO biInfo;
}BITMAPFILE;


//����BMPͼƬ(����ɫ���λͼ):��RGB(A)λͼ���ݵĻ����ϼ����ļ���Ϣͷ��λͼ��Ϣͷ
int GenBmpFile(U8 *pData, U8 bitCountPerPix, U32 width, U32 height, const char *filename);
//��ȡBMP�ļ���λͼ����(����ɫ���λͼ):����BMP�ļ����ļ���Ϣͷ��λͼ��Ϣͷ����ȡ��RGB(A)λͼ����
U8* GetBmpData(U8 *bitCountPerPix, U32 *width, U32 *height, const char* filename);
//�ͷ�GetBmpData����Ŀռ�
void FreeBmpData(U8 *pdata);
//����BMPͼƬ(����ɫ���λͼ):��RGB(A)λͼ���ݵĻ����ϼ����ļ���Ϣͷ��λͼ��Ϣͷ  

#endif

