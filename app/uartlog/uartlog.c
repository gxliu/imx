/* name: uartlog.c
 * description: �������ݱ���
 * author: zuokongxiao
 * date: 2014-10-20
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

/* ��������С���� */
#define BUF_SIZE        (10240)
/* ���ڲ����ʶ��� */
#define BAUDRATE        B115200

/* �ļ����� */
static char acFileName[64];
/* ������д���� */
static int lWrIndex = 0;
/* ���������� */
static char acBuf[BUF_SIZE];


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

/* ����uartlog�ļ� */
static int create_uartlog_file(char *pcFileName)
{
    struct timeval cur_time;
    struct tm *pstTm;
    int fd = 0, len = 0;
    int ret = 0;
    
    /* ���������� */
    if(INVALID_POINTER(pcFileName))
    {
        DEBUG_MSG("E:input param error!\r\n");
        return -1;
    }
    /* ��ȡ��ǰʱ�䲢ת��Ϊ�ļ��� */
    gettimeofday(&cur_time, NULL);
    pstTm = localtime(&(cur_time.tv_sec));
    sprintf(pcFileName, "/home/uartlog_%04d%02d%02d%02d%02d%02d.txt", \
            pstTm->tm_year+1900, pstTm->tm_mon+1, pstTm->tm_mday, \
            pstTm->tm_hour, pstTm->tm_min, pstTm->tm_sec);
    /* �����ļ� */
    fd = open(pcFileName, O_CREAT|O_RDWR|O_APPEND, S_IRWXU|S_IRWXG|S_IRWXO);
    if(0 > fd)
    {
        ERROR_MSG("create uart log file error\r\n");
        return -1;
    }
    /* д�ļ� */
    len = write(fd, "create log file is ok.\r\n", 24);
    if(len < 24)
    {
        ERROR_MSG("write file len < 24!\r\n");
        ret = -1;
    }
    else
    {
        DEBUG_MSG("write file ok!\r\n");
    }
    /* �ر��ļ� */
    close(fd);
    
    return ret;
}

/* дuartlog�ļ� */
static int write_uartlog_file(char *pcFileName, char *pcBuf, int lSize)
{
    int fd = 0, len = 0;
    int ret = 0;
    
    /* ���������� */
    if(INVALID_POINTER(pcFileName) || INVALID_POINTER(pcBuf))
    {
        DEBUG_MSG("E:input param error!\r\n");
        return -1;
    }
    /* ���ļ� */
    fd = open(pcFileName, O_RDWR|O_APPEND, S_IRWXU|S_IRWXG|S_IRWXO);
    if(0 > fd)
    {
        ERROR_MSG("open uart log file error\r\n");
        return -1;
    }
    /* д�ļ� */
    len = write(fd, pcBuf, lSize);
    if(len < 24)
    {
        ERROR_MSG("write file len < lSize!\r\n");
        ret = -1;
    }
    else
    {
        //DEBUG_MSG("write file ok!\r\n");
    }
    /* �ر��ļ� */
    close(fd);
    
    return ret;
}

/* alrm�źŴ����� */
void sigalrm_fn(int sig)
{
    if(0 < lWrIndex)
    {
        write_uartlog_file(acFileName, acBuf, lWrIndex);
        lWrIndex = 0;
    }
}

/* 
 * ������ 
 * �÷���uartlog + �����豸��"/dev/ttySAC1"
 */
int main(int argc, char *argv[])
{
    int uartfd;
    int i = 0, len = 0;
    char tmp[32];
    
    /* ���������� */
    if(2 != argc)
    {
        DEBUG_MSG("E:input param error.\r\n");
        return -1;
    }
    DEBUG_MSG("D:dev name = %s!\r\n", argv[1]);
    /* �򿪴��� */
    uartfd = open_uart(argv[1]);
    if(0 > uartfd)
    {
        DEBUG_MSG("D:open uart error!\r\n");
        return -1;
    }
    /* ����log�ļ� */
    if(0 > create_uartlog_file(acFileName))
    {
        DEBUG_MSG("D:create log file error!\r\n");
        close(uartfd);
        return -1;
    }
    /* �ź� */
    signal(SIGALRM, sigalrm_fn);
    
    /* ��ѭ�� */
    while(1)
    {
        len = read(uartfd, tmp, 16);
        for(i=0; i<len; i++)
        {
            alarm(5);
            printf("%c", tmp[i]);
            acBuf[lWrIndex] = tmp[i];
            lWrIndex++;
            if(BUF_SIZE <= lWrIndex)
            {
                lWrIndex = 0;
                write_uartlog_file(acFileName, acBuf, BUF_SIZE);
            }
        }
    } /* end of while(1)... */
}

