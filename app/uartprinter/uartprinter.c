/* name: uartprinter.c
 * description: ���ڴ�ӡ��
 * author: zuokongxiao
 * date: 2014-10-23
 */
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <unistd.h>  
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <termios.h>
#include <errno.h>   
#include <limits.h> 
#include <asm/ioctls.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>
#include "deftype.h"
#include "debug.h"
#include "printerlib.h"


/* ���ڲ����ʶ��� */
#define BAUDRATE        B57600


/* �򿪴��� */
static int open_uart(char *pcSerialName)
{
    int iFd;
    struct termios opt; 

    DEBUG_MSG("D:name=%s\r\n", pcSerialName);
    /* �򿪴��� */
    iFd = open(pcSerialName, O_RDWR|O_NOCTTY|O_NDELAY);
    if(iFd < 0)
    {
        ERROR_MSG("E:open error!\r\n");
        return -1;
    }
    /* ��ȡ�豸termios�ṹ */
    if(0 > tcgetattr(iFd, &opt))
    {
        ERROR_MSG("E:tcgetattr error!\r\n");
        return -1;
    }
    /* ���ò����� */
    cfsetispeed(&opt, BAUDRATE);
    cfsetospeed(&opt, BAUDRATE);
    /* ���Ʊ�־��8λ����λ����У��λ */
    opt.c_cflag &= ~(CSTOPB | CSIZE | PARENB);
    opt.c_cflag |= CS8;
    opt.c_cflag |= (CLOCAL | CREAD);
    /* �����־ */
    opt.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    /* �����־ */
    opt.c_oflag &= ~(OPOST);
    /* ���ر�־��ԭʼģʽ */
    opt.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    /* VMIN:���ȡ����С�ֽ��� VTIME:�޶��Ĵ����ȴ����ʱ�� */
    opt.c_cc[VMIN] = 16; //һ��read�����������ص���С�ֽ���
    opt.c_cc[VTIME] = 2; //150��ʾ15��
    /* ���� */
    if(0 > tcsetattr(iFd, TCSANOW, &opt))
    {
        ERROR_MSG("E:tcsetattr error!\r\n");
        return -1;
    }

    return iFd;
}

/* 
 * ������ 
 * �÷���uartprinter + �����豸��"/dev/ttySP0 + bmp�ļ���"
 */
int main(int argc, char *argv[])
{
    int uartfd;
    unsigned char ucPrinterState = 0;
    
    /* ���������� */
    if(3 != argc)
    {
        DEBUG_MSG("E:input param error.\r\n");
        return -1;
    }
    
    /* �򿪴��� */
    uartfd = open_uart(argv[1]);
    if(0 > uartfd)
    {
        DEBUG_MSG("D:open uart error!\r\n");
        return -1;
    }
    
    /* ���ô��ڴ�ӡ�� */
    printer_fildes(uartfd);
    printer_init();
    get_printer_state(&ucPrinterState);
    /* ͼƬ��ӡ */
    printer_set_linespacing(0);
    printer_paper_feed(20);
    printer_bmp_print(argv[2]);
    printer_paper_feed(50);
    
    /* ��ѭ�� */
    DEBUG_MSG("D:enter main loop.\r\n");
    while(1)
    {
        sleep(1);
        get_printer_state(&ucPrinterState);
    } /* end of while(1)... */
}

