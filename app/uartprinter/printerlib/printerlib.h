/* name: printerlib.h
 * description: ��ӡ����
 * author: zuokongxiao
 * date: 2014-10-23
 */

#ifndef __PRINTERLIB_H
#define __PRINTERLIB_H


/* ��ӡ��״̬���� */
typedef enum 
{
    PRINTER_NORMAL = 0x00, /* ��ӡ������  */
    PRINTER_ONLINE = 0x00, /* ���� */
    PRINTER_OFFLINE = 0x08, /* �ѻ� */
    PRINTER_NOT_OUT_OF_PAPER = 0x00, /* ��ӡ����ȱֽ */
    PRINTER_OUT_OF_PAPER = 0x20, /* ��ӡ��ȱֽ */
    PRINTER_HEAD_NORMAL = 0x00, /* ��ӡ����ͷ���� */
    PRINTER_HEAD_OVERHEAT = 0x40, /* ��ӡ����ͷ���� */
} printer_state_e;


void printer_fildes(int fd);
int printer_init(void);
int set_printer_linespacing(unsigned char ucLineNum);
int printer_paper_feed(unsigned char ucLineNum);
int get_printer_state(unsigned char *pucState);
int printer_picture_print(unsigned char *pucBuf, int lSize);
int printer_bmp_print(char *pcBmpName);

#endif

