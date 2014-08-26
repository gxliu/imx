/* name: uarttest.c
 * description: ���ڲ��Գ���
 * author: zuokongxiao
 * date: 2014-08-26
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
#include "deftype.h"
#include "debug.h"


/* gps���ݽṹ�嶨�� */
typedef struct gps_info
{ 
    char utc_time[32];
    char status; 
    float latitude_value;
    char latitude; 
    float longtitude_value;
    char longtitude;
    float speed;
    float azimuth_angle; 
    char utc_data[32]; 
} gps_info_t;

/* ��ȡgps��Ϣ */
static int gps_get_rmc(gps_info_t *pstGpsInfo, char *pcBuf) 
{
    int i = 0, res = 0;
    char *pcGprmcString = NULL;

    /* find "$GPRMC" from gps_buf */ 
    if((pcGprmcString = strstr(pcBuf, "$GPRMC")) != NULL) 
    { 
        for(i=0; i<strlen(pcGprmcString); i++) 
        { 
            if(pcGprmcString[i] == '\n') 
            { 
                pcGprmcString[i] = '\0'; //replace ��\n�� with null 
            }
        } 
        /*  */
        res = sscanf(pcGprmcString, "$GPRMC,%[^,],%c,%f,%c,%f,%c,%f,%f,%[^,]",
              pstGpsInfo->utc_time, &(pstGpsInfo->status), &(pstGpsInfo->latitude_value),
              &(pstGpsInfo->latitude), &(pstGpsInfo->longtitude_value),&(pstGpsInfo->longtitude),
              &(pstGpsInfo->speed), &(pstGpsInfo->azimuth_angle), pstGpsInfo->utc_data);
        if(0 > res)
        {
            return -1;
        }
    } 
    else
    {
        return -1;
    }

    return 0;
}

/* ��ӡGPS��Ϣ */
static int print_gps_info(gps_info_t *pstGpsInfo)
{
    if(INVALID_POINTER(pstGpsInfo))
    {
        DEBUG_MSG("E:input param error!\r\n");
        return -1;
    }
    printf("GPS INFO: ");
    printf("utc_date=%s ", pstGpsInfo->utc_data);
    printf("time=%s ", pstGpsInfo->utc_time);
    printf("status=%c ", pstGpsInfo->status);
    printf("latitude=%f ", pstGpsInfo->latitude_value);
    printf("N/S=%c ", pstGpsInfo->latitude);
    printf("longtitude=%f ", pstGpsInfo->longtitude_value);
    printf("E/W=%c ", pstGpsInfo->longtitude);
    printf("speed=%f ", pstGpsInfo->speed);
    printf("azimuth_angle=%f ", pstGpsInfo->azimuth_angle);
    printf("\r\n");
}

/* �򿪴��� */
static int openSerial(char *cSerialName)
{
    int iFd;
    struct termios opt; 

    DEBUG_MSG("D:name=%s\r\n", cSerialName);
    /* �򿪴��� */
    iFd = open(cSerialName, O_RDWR|O_NOCTTY|O_NDELAY);
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
    cfsetispeed(&opt, B4800);
    cfsetospeed(&opt, B4800);
    /* �������� */
    opt.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    opt.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    opt.c_oflag &= ~(OPOST);
    opt.c_cflag &= ~(CSIZE | PARENB);
    opt.c_cflag |= CS8;
    /* VMIN:���ȡ����С�ֽ��� VTIME:�޶��Ĵ����ȴ����ʱ�� */
    opt.c_cc[VMIN] = 16;
    opt.c_cc[VTIME] = 150;
    /* ���� */
    if(0 > tcsetattr(iFd, TCSANOW, &opt))
    {
        ERROR_MSG("E:tcsetattr error!\r\n");
        return -1;
    }

    return iFd;
}

/* ������ */
int main(int argc, char *argv[])
{
    char tmp[1024] = "test uart!\r\n";
    char gpsbuf[1024];
    int len;
    int fd, i;
    gps_info_t stGpsInfo;
    int wrbufindex = 0;

    /* �򿪴��� */
    fd = openSerial("/dev/ttySP0");
    if(0 > fd)
    {
        DEBUG_MSG("D:open uart error!\r\n");
        return -1;
    }
    write(fd, tmp, 16);
    /* */
    while(1)
    {
        len = read(fd, tmp, 0x01);
        for(i=0; i<len; i++)
        {
            //printf("%c", tmp[i]);
            if('\n' == tmp[i])
            {
                gpsbuf[wrbufindex] = '\n';
                wrbufindex++;
                if(1024 > wrbufindex)
                {
                    gpsbuf[wrbufindex] = '\0';
                    if(0 == gps_get_rmc(&stGpsInfo, gpsbuf))
                    {
                        print_gps_info(&stGpsInfo);
                    }
                }
                wrbufindex = 0;
            }
            else
            {
                gpsbuf[wrbufindex] = tmp[i];
                wrbufindex++;
                if(1024 <= wrbufindex)
                {
                    wrbufindex = 0;
                }
            }
        }
    }
}

