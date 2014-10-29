/* name: bmplib.c
* description: bmp
* author: zuokongxiao
* date: 2014-10-23
*/
#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include "deftype.h"
#include "debug.h"
#include "bmplib.h"


typedef unsigned char  U8;
typedef unsigned short U16;
typedef unsigned int   U32;

/* BMP�ļ�ͷ�ṹ�嶨�� */
typedef struct tagBITMAPFILEHEADER
{
    U16 bfType; /* ����(2 bytes)BM */
    U32 bfSize; /* �ļ���С(4 bytes) */
    U16 bfReserved1; /* ���� */
    U16 bfReserved2; /* ���� */
    U32 bfOffBits; /* �ļ�ͷ��ͼ�����ݵ�ƫ��ֵ */
} BITMAPFILEHEADER;

/* BMP�ļ���Ϣͷ�ṹ�嶨�� */
typedef struct tagBITMAPINFOHEADER
{
    U32 biSize; /* �˽ṹռ�õ��ֽ���(4 bytes) */
    U32 biWidth; /* ͼ���� */
    U32 biHeight; /* ͼ��߶� */
    U16 biPlanes; /* ��ɫƽ������ʼ��Ϊ1 */
    U16 biBitCount; /* ������/���أ�1��4��8��16��24��32 */
    U32 biCompression; /* ѹ������ */
    U32 biSizeImage; /* ͼ���С */
    U32 biXPelsPerMeter; /* ˮƽ�ֱ��� */
    U32 biYPelsPerMeter; /* ��ֱ�ֱ��� */
    U32 biClrUsed; /* ��ɫ������ */
    U32 biClrImportant; /* ��ͼ����ʾ����ҪӰ�����ɫ��������Ŀ */
} BITMAPINFOHEADER;

/* ��ɫ��ṹ�嶨�� */
typedef struct tagRGBQUAD
{
    U8 rgbBlue;
    U8 rgbGreen;
    U8 rgbRed;
    U8 rgbReserved;
} RGBQUAD;


/* ��ӡbmp�ļ�ͷ��Ϣ */
static void print_bmp_file_header(BITMAPFILEHEADER *pstBmpFileHeader)
{
    /* ���������� */
    if(INVALID_POINTER(pstBmpFileHeader))
    {
        DEBUG_MSG("E:input param error.\r\n");
        return;
    }
    DEBUG_MSG("D:bmp file header > type=0x%04x, size=%d, offbits=%d\r\n", \
              pstBmpFileHeader->bfType, \
              pstBmpFileHeader->bfSize, \
              pstBmpFileHeader->bfOffBits);
}

/* ��ӡλͼ��Ϣͷ */
static void print_bmp_info_header(BITMAPINFOHEADER *pstBmpInfoHeader)
{
    /* ���������� */
    if(INVALID_POINTER(pstBmpInfoHeader))
    {
        DEBUG_MSG("E:input param error.\r\n");
        return;
    }
    DEBUG_MSG("D:bmp info header > size=%d, width=%d, height=%d, bitcount=%d, \
biCompression=%d, biSizeImage=%d, biXPelsPerMeter=%d, biYPelsPerMeter=%d, \
biClrUsed=%d, biClrImportant=%d\r\n", \
           pstBmpInfoHeader->biSize, \
           pstBmpInfoHeader->biWidth, \
           pstBmpInfoHeader->biHeight, \
           pstBmpInfoHeader->biBitCount, \
           pstBmpInfoHeader->biCompression, \
           pstBmpInfoHeader->biSizeImage, \
           pstBmpInfoHeader->biXPelsPerMeter, \
           pstBmpInfoHeader->biYPelsPerMeter, \
           pstBmpInfoHeader->biClrUsed, \
           pstBmpInfoHeader->biClrImportant);
}


/* ��ȡBMP�ļ���λͼ����(��1bit/����) */
int get_bmp_data(bmp_t *pstBmp, const char *pcFileName)
{
    FILE *fp;
    BITMAPFILEHEADER stBmpFileHeader;
    BITMAPINFOHEADER stBmpInfoHeader;
    
    /* ���������� */
    if(INVALID_POINTER(pstBmp) || INVALID_POINTER(pcFileName))
    {
        DEBUG_MSG("E:input param error.\r\n");
        return -1;
    }
    
    /* ��bmp�ļ� */
    if((fp = fopen(pcFileName, "rb")) == NULL)
    {
        DEBUG_MSG("E:Error opening file %s.\r\n", pcFileName);
        return -1;
    }
    /* ��BMP�ļ�ͷ��Ϣ(�ֿ���ȡ) */
    fread(&(stBmpFileHeader.bfType), sizeof(stBmpFileHeader.bfType), 1, fp);
    fread(&(stBmpFileHeader.bfSize), sizeof(stBmpFileHeader.bfSize), 1, fp);
    fread(&(stBmpFileHeader.bfReserved1), sizeof(stBmpFileHeader.bfReserved1), 1, fp);
    fread(&(stBmpFileHeader.bfReserved2), sizeof(stBmpFileHeader.bfReserved2), 1, fp);
    fread(&(stBmpFileHeader.bfOffBits), sizeof(stBmpFileHeader.bfOffBits), 1, fp);
    /* ��ӡ�ļ�ͷ��Ϣ */
    print_bmp_file_header(&stBmpFileHeader);
    if(0x4d42 != stBmpFileHeader.bfType)
    {
        DEBUG_MSG("E:is not bmp file.\r\n");
        return -1;
    }
    /* ��λͼ��Ϣ */
    if(0 > fread(&stBmpInfoHeader, sizeof(stBmpInfoHeader), 1, fp))
    {
        ERROR_MSG("E:fread bmiHeader error!\r\n");
    }
    /* ��ӡλͼ��Ϣͷ */
    print_bmp_info_header(&stBmpInfoHeader);
    
    /* ������Ϣ */
    pstBmp->usBitCount = stBmpInfoHeader.biBitCount;  
    pstBmp->ulWidth = stBmpInfoHeader.biWidth;
    pstBmp->ulHeight = stBmpInfoHeader.biHeight;
    pstBmp->ulRowBytes = (((pstBmp->ulWidth)*(pstBmp->usBitCount) + 31) >> 5) << 2;
    pstBmp->pucData = (unsigned char *)malloc((pstBmp->ulHeight)*(pstBmp->ulRowBytes));  
    if(NULL == pstBmp->pucData)  
    {
        DEBUG_MSG("E:malloc error.\r\n");
        return -1;
    }
    fseek(fp, stBmpFileHeader.bfOffBits, SEEK_SET);
    fread(pstBmp->pucData, (pstBmp->ulHeight)*(pstBmp->ulRowBytes), 1, fp); 
    /* �ر��ļ� */
    fclose(fp);  
       
    return 0;
}

/* �ͷ�GetBmpData����Ŀռ� */
void free_bmp_data(unsigned char *pucData)
{  
    if(pucData)
    {  
        free(pucData);
        pucData = NULL;
    }
}

