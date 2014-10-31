/* name: thermalprinterlib.h
 * description: ������ӡ����
 * author: zuokongxiao
 * date: 2014-10-30
 */

#ifndef __THERMALPRINTERLIB_H
#define __THERMALPRINTERLIB_H


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


/* ������ӡ���� */
class thermalprinter
{
    public:
        int open_dev(const char *pcTtyName);
        int close_dev(void);
        int init(void); /* ��ʼ������ */
        int set_linespacing(unsigned char ucLineNum); /* �����м�� */
        int paper_feed(unsigned char ucLineNum); /* ��ֽ */
        int get_state(unsigned char *pucState); /* ��ȡ��ӡ��״̬ */
        int print_picture(unsigned char *pucBuf, int lSize); /* ��ӡͼ�� */
        int print_1bit_bmp(const char *pcBmpName);
    private:
        int lPrinterFd; /* ��ӡ���ļ������� */
};


#endif

