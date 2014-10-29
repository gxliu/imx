/* name: printerlib.c
* description: ���ڴ�ӡ����(RG-E487����������ӡ��)
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
#include "printerlib.h"

/* �����ַ����� */
#define CHAR_ESC                0x1b
#define CHAR_LF                 0x0a
#define CHAR_CR                 0x0d
#define CHAR_NUL                0x00
/* ��ӡ������ȶ��� */
#define PRINTER_WIDTH_SIZE      384

/* λ���� */
const unsigned char aucByteBit[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

/* ��ӡ���ļ������� */
int lPrinterFd = 0;


/* ��д�ӿ� */
void printer_fildes(int fd)
{
    lPrinterFd = fd;
}

/* ��ʼ����ӡ�� */
int printer_init(void)
{
    unsigned char aucBuf[3] = {CHAR_ESC, 0x40, 0x00};
    
    return write(lPrinterFd, aucBuf, 3);
}

/* �����м�� */
int printer_set_linespacing(unsigned char ucLineNum)
{
    unsigned char aucBuf[3] = {CHAR_ESC, '1', 0x00};
    
    aucBuf[2] = ucLineNum;
    return write(lPrinterFd, aucBuf, 3);
}

/* ִ��ָ������ֽ */
int printer_paper_feed(unsigned char ucLineNum)
{
    unsigned char aucBuf[3] = {CHAR_ESC, 'J', 0x00};
    
    aucBuf[2] = ucLineNum;
    return write(lPrinterFd, aucBuf, 3);
}

/* ��ȡ��ӡ��״̬ */
int get_printer_state(unsigned char *pucState)
{
    unsigned char aucBuf[3] = {0x10, 0x04, 0x01};
    
    /* ���������� */
    if(INVALID_POINTER(pucState))
    {
        DEBUG_MSG("E:input param error.\r\n");
        return -1;
    }
    /* �����ӡ��״̬ */
    if(0 > write(lPrinterFd, aucBuf, 3))
    {
        DEBUG_MSG("E:write cmd error(get printer state)!\r\n");
        return -1;
    }
    /* ��ȡ״̬ */
    if(1 != read(lPrinterFd, aucBuf, 1))
    {
        DEBUG_MSG("E:read serial error!\r\n");
        return -1;
    }
    /* ������������ */
    if(0x12 != (aucBuf[0] & 0x97))
    {
        DEBUG_MSG("E:return data error!\r\n");
        return -1;
    }
    //DEBUG_MSG("D:printer state = 0x%02x.\r\n", aucBuf[0]);
    *pucState = PRINTER_NORMAL;
    if(0x12 == aucBuf[0])
    {
        //DEBUG_MSG("D:printer normal.\r\n");
    }
    else
    {
        /* ����״̬ */
        if(PRINTER_OFFLINE == (aucBuf[0]&PRINTER_OFFLINE))
        {
            DEBUG_MSG("D:printer offline.\r\n");
            *pucState |= PRINTER_OFFLINE;
        }
        /* �Ƿ�ȱֽ */
        if(PRINTER_OUT_OF_PAPER == (aucBuf[0]&PRINTER_OUT_OF_PAPER))
        {
            DEBUG_MSG("D:printer out of paper.\r\n");
            *pucState |= PRINTER_OUT_OF_PAPER;
        }
        /* ��ͷ״̬ */
        if(PRINTER_HEAD_OVERHEAT == (aucBuf[0]&PRINTER_HEAD_OVERHEAT))
        {
            DEBUG_MSG("D:printer head overhead.\r\n");
            *pucState |= PRINTER_HEAD_OVERHEAT;
        }
    }
    
    return 0;
}

/* ͼ�δ�ӡ */
int printer_picture_print(unsigned char *pucBuf, int lSize)
{
    unsigned char aucBuf[390] = {CHAR_ESC, 'K'};
    unsigned char ucState = 0;
    
    /* ���������� */
    if(INVALID_POINTER(pucBuf) || (PRINTER_WIDTH_SIZE < lSize))
    {
        DEBUG_MSG("E:input param error.\r\n");
        return -1;
    }
    /* ����ӡ��״̬ */
    if(0 > get_printer_state(&ucState))
    {
        DEBUG_MSG("E:get printer state error.\r\n");
        return -1;
    }
    if(PRINTER_NORMAL != ucState)
    {
        DEBUG_MSG("E:printer abnormal.\r\n");
        return -1;
    }
    
    /* ���� */
    aucBuf[2] = (unsigned char)lSize;
    aucBuf[3] = (unsigned char)(lSize>>8);
    /* ���� */
    memcpy(&(aucBuf[4]), pucBuf, lSize);
    aucBuf[4+lSize] = CHAR_LF;
    aucBuf[4+lSize+1] = CHAR_CR;
#if 1
    unsigned int i = 0;
    unsigned char n = 0;
    /* д���� */
    //printer_set_linespacing(0);
    unsigned int ulCnt = (lSize+6+15)/16 - 1;
    for(i=0; i<ulCnt; i++)
    {
        write(lPrinterFd, (aucBuf+i*16), 16);
    }
    n = (lSize+6)%16;
    if(0 == n)
    {
        write(lPrinterFd, (aucBuf+ulCnt*16), 16);
    }
    else
    {
        write(lPrinterFd, (aucBuf+ulCnt*16), n);
    }
    
    return 0;
#endif
    //return write(lPrinterFd, aucBuf, lSize+6);
}

/* bmpͼ�δ�ӡ */
int printer_bmp_print(char *pcBmpName)
{
    bmp_t stBmp;
    int res = 0;
    
    /* ���������� */
    if(INVALID_POINTER(pcBmpName))
    {
        DEBUG_MSG("E:input param error.\r\n");
        return -1;
    }
    /* �м������Ϊ0 */
    printer_set_linespacing(0);
    
    /* ��ȡbmp�ļ����� */
    if(0 > get_bmp_data(&stBmp, pcBmpName))
    {
        DEBUG_MSG("E:get bmp file data error.\r\n");
        return -1;
    }
    if((1 != stBmp.usBitCount) || (0 > stBmp.ulHeight))
    {
        DEBUG_MSG("E:bmp iamge data format is error.\r\n");
        return -1;
    }
    DEBUG_MSG("D:ulRowbytes=%d.\r\n", stBmp.ulRowBytes);
    
    /* ����ͼ������ */
    unsigned char *pucBuf;
    unsigned char ucTemp = 0, ucDot = 0, ucBit = 0;
    unsigned int ulRowCnt = 0, i = 0, j = 0, ulPrintWidth = 0;
    ulPrintWidth = stBmp.ulWidth;
    if(PRINTER_WIDTH_SIZE < stBmp.ulWidth)
    {
        ulPrintWidth = PRINTER_WIDTH_SIZE;
    }
    pucBuf = (unsigned char *)malloc(ulPrintWidth);
#if 0 //��ӡͼ������
    for(j=stBmp.ulHeight; j!=0; j--)
    {
        for(i=0; i<stBmp.ulRowBytes; i++)
        {
            printf("%02x ", *(stBmp.pucData+(j-1)*(stBmp.ulRowBytes)+i));
        }
        printf("\r\n");
    }
    printf("\r\n");
#endif
    ucBit = 8;
    memset(pucBuf, 0, ulPrintWidth);
    for(j=stBmp.ulHeight; j!=0; j--) //ͼ������½������Ͻǽ��б����
    {
        /* ͼ���д��� */
        //DEBUG_MSG("D:row num=%d.\r\n", j);
        ulRowCnt = 0;
        for(i=0; i<stBmp.ulRowBytes; i++) //���ص����෴
        {
            ucTemp = *(stBmp.pucData+(j-1)*(stBmp.ulRowBytes)+i);
            for(ucDot=8; ucDot>0; ucDot--)
            {
                if(ulRowCnt < ulPrintWidth) //if(ulRowCnt < stBmp.ulWidth)
                {
                    if(aucByteBit[ucDot-1] != (aucByteBit[ucDot-1] & ucTemp))
                    {
                        *(pucBuf+ulRowCnt) |= (aucByteBit[ucBit-1]);
                    }
                }
                else
                {
                    break;
                }
                ulRowCnt++;
            }
        }
        ucBit--;
        /* ��ӡͼ�� */
        if(0 == ucBit)
        {
            ucBit = 8;
            if(0 > printer_picture_print(pucBuf, ulPrintWidth))
            {
                DEBUG_MSG("E:picture print error1.\r\n");
                /* ����ջ����� */
                res = -1;
                break;
            }
            memset(pucBuf, 0, ulPrintWidth);
        }
    }
    /* ��ӡʣ��ͼ�� */
    if((0 < ucBit) && (8 > ucBit))
    {
        if(0 > printer_picture_print(pucBuf, ulPrintWidth))
        {
            DEBUG_MSG("E:picture print error2.\r\n");
            /* ����ջ����� */
            res = -1;
        }
    }
    /* �ͷ��ڴ� */
    free(pucBuf);
    free_bmp_data(stBmp.pucData);  
    
    return res;
}



