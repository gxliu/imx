/* name: bmplib.h
 * description: BMP��
 * author: zuokongxiao
 * date: 2014-10-23
 */
#ifndef __BMPLIB_H
#define __BMPLIB_H


/* BMP�ļ��ṹ�嶨�� */
typedef struct bmp_file_t
{
    unsigned int ulWidth; /* ͼ���� */
    unsigned int ulHeight; /* ͼ��߶� */
    unsigned int ulRowBytes; /* ÿ���ֽ��� */
    unsigned short usBitCount; /* λ��/���� */
    unsigned char *pucData; /* ָ��ͼ������ */
} bmp_t;


/* ��ȡBMP�ļ���λͼ���� */
int get_bmp_data(bmp_t *pstBmp, const char *pcFileName);
/* �ͷ�get_bmp_data������ڴ� */
void free_bmp_data(unsigned char *pucData);


#endif

