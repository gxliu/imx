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
#include "thermalprinterlib.h"


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
        printf("E:input param error.\r\n");
        return -1;
    }
    
    /* ��������ӡ�� */
    thermalprinter cPrinter;
    if(0 > cPrinter.open_dev(argv[1]))
    {
        printf("D:open thermalprinter error!\r\n");
        return -1;
    }
    cPrinter.init();
    cPrinter.get_state(&ucPrinterState);
    /* ͼƬ��ӡ */
    cPrinter.set_linespacing(0);
    cPrinter.paper_feed(20);
    cPrinter.print_1bit_bmp(argv[2]);
    cPrinter.paper_feed(50);
    
    /* ��ѭ�� */
    printf("D:enter main loop.\r\n");
    while(1)
    {
        sleep(1);
        cPrinter.get_state(&ucPrinterState);
    } /* end of while(1)... */
}

