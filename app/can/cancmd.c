/* name: cancmd.c
 * description: CAN�������
 * author: zuokongxiao
 * date: 2014-08-27
 */
#include <stdio.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <linux/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include "deftype.h"
#include "debug.h"
#include "can.h"


/* �����ַ���ת�� */
static int cmd_string_convert(char *pcCmd, const char *pcCmdBuf)
{
    int ret = 0;
    int i = 0, cmdi = 0, hib = 0, lob = 0;

    if(INVALID_POINTER(pcCmdBuf) || INVALID_POINTER(pcCmd))
    {
        DEBUG_MSG("E:input param error!\r\n");
        return -1;
    }
    for(i=0; i<1021; i=i+3)
    {
        if('\0' == pcCmdBuf[i])
        {
            ret = cmdi;
            break;
        }
        /* ��λ */
        if(('0' <= pcCmdBuf[i]) && ('9' >= pcCmdBuf[i]))
        {
            hib = pcCmdBuf[i] - '0';
        }
        else if(('a' <= pcCmdBuf[i]) && ('f' >= pcCmdBuf[i]))
        {
            hib = pcCmdBuf[i] - 'a' + 10;
        }
        else if(('A' <= pcCmdBuf[i]) && ('F' >= pcCmdBuf[i]))
        {
            hib = pcCmdBuf[i] - 'A' + 10;
        }
        else
        {
            DEBUG_MSG("E:high 4 bits error!\r\n");
            ret = -1;
            break;
        }
        /* ��λ */
        if(('0' <= pcCmdBuf[i+1]) && ('9' >= pcCmdBuf[i+1]))
        {
            lob = pcCmdBuf[i+1] - '0';
        }
        else if(('a' <= pcCmdBuf[i+1]) && ('f' >= pcCmdBuf[i+1]))
        {
            lob = pcCmdBuf[i+1] - 'a' + 10;
        }
        else if(('A' <= pcCmdBuf[i+1]) && ('F' >= pcCmdBuf[i+1]))
        {
            lob = pcCmdBuf[i+1] - 'A' + 10;
        }
        else
        {
            DEBUG_MSG("E:low 4 bits error!\r\n");
            ret = -1;
            break;
        }
        hib *= 16;
        hib = hib + lob;
        pcCmd[cmdi] = (char)hib;
        cmdi++;
        if('\0' == pcCmdBuf[i+2])
        {
            ret = cmdi;
            break;
        }
        if((' ' != pcCmdBuf[i+2]) && ('\0' != pcCmdBuf[i+2]))
        {
            DEBUG_MSG("E:space error! i=%d cmdi=%d\r\n", i, cmdi);
            ret = -1;
            break;
        }
    }
    
    return ret;
}

/* �������ݵ�CAN */
static int send_data_to_can(int lBoardType, char *pcCmd, int lCmdLen)
{
    struct can_frame astCanFrame[43];
    int i = 0;
    
    if(INVALID_POINTER(pcCmd))
    {
        DEBUG_MSG("E:input param error!\r\n");
        return -1;
    }
    if(8 >= lCmdLen)
    {
        
        
    }
    else
    {
    
    
    }

    
    
    
//int can_write(struct can_frame *pstCanFrame)

}

/* ������ */
int main(int argc, char *argv[])
{
    int lBoardType = 0, i = 0, lCmdLen = 0;
    char acCmdBuf[1024] = {0};
    char acCmd[340] = {0};
    
    /* ��CAN�豸 */
    if(0 > can_open())
    {
        return -1;
    }
    /* ���������߳� */
    //todo
   
    while(1)
    {
        /* ����忨���� */
        //printf("\r\n***************************\r\n");
        printf("Please input board type: ");
        scanf("%d", &lBoardType);
        printf("board type = %d\r\n", lBoardType);
        if((0 > lBoardType) || (3 < lBoardType))
        {
            DEBUG_MSG("E:input boardtype < 0 or boardtype >3!\r\n");
            continue;
        }
        /* ���������ַ��� */
        setbuf(stdin, NULL);
        printf("Please input cmd: ");
        //gets(acCmdBuf);
        for(i=0; i<1023; i++)
        {
            acCmdBuf[i] = getchar();
            if((0x00 == acCmdBuf[i]) || ('\n' == acCmdBuf[i]))
            {
                acCmdBuf[i] = '\0';
                break;
            }
        }
        acCmdBuf[i+1] = '\0';
        printf("%s\r\n", acCmdBuf);
        
        /* �����ַ���ת�� */
        lCmdLen = cmd_string_convert(acCmd, acCmdBuf);
        if(0 >= lCmdLen)
        {
            continue;
        }
        printf("cmd: ", acCmdBuf);
        for(i=0; i<lCmdLen; i++)
        {
            printf("0x%02x ", acCmd[i]);
        }
        printf("\r\n");
        
        /* cmd��ʽ��� */
        //todo...
        
        /* can�������� */
        
    }
}


