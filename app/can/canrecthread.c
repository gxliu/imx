/* name: canrecthread.c
* description: CAN�����߳�
* author: zuokongxiao
* date: 2014-08-28
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
#include <linux/can.h>
#include <linux/can/error.h>
#include <linux/can/raw.h>
#include <pthread.h>
#include <semaphore.h>
#include "deftype.h"
#include "debug.h"
#include "can.h"
#include "protocol.h"

/* ���ջ������� */
#define REC_BUF_NUM         4u

/* ���Ķλ������ṹ�嶨�� */
struct seg_msg_buf
{
    uint8 aucSegMsgBuf[PROTOCOL_FRAME_MAX_SIZE*2]; /* ֡���������ֶα������黺������ */
    uint8 ucSegMsgCnt; /* ֡���� */
    uint8 ucLastSegNum; /* ��һ�ηֶκ� */
};

/* ���Ķ����� */
static int can_seg_msg_recombine(frame_check_t *pstFrame, \
                                 struct seg_msg_buf *pstSegMsg, \
                                 struct can_frame *pstCanFrame)
{
    int ret = -1;
    int i = 0;
    uint8 ucSegNum = 0;
    
    do
    {
        if(INVALID_POINTER(pstFrame) || INVALID_POINTER(pstSegMsg) \
           || INVALID_POINTER(pstCanFrame))
        {
            DEBUG_MSG("E:input param error!\r\n");
            break;
        }
        /* ��ȡЭ��ֶκ� */
        ucSegNum = (uint8)((pstCanFrame->can_id & SEG_CODE_NUM_MASK)>>8);
        
        /*! Э��֡�޷ֶα��� */
        if(FRAME_NONE_SEG == (pstCanFrame->can_id & SEG_CODE_FLAG_MASK))
        {
            /*! ֱ�ӽ����ݸ��Ƶ�CAN���ջ����� */
            for(i=0; i<pstCanFrame->can_dlc; i++)
            {
                pstFrame->stRecBuf.aucBuf[pstFrame->stRecBuf.unWrIndex] \
                    = pstCanFrame->data[i];
                (pstFrame->stRecBuf.unWrIndex)++;
                if(REC_BUF_SIZE <= pstFrame->stRecBuf.unWrIndex)
                {
                    pstFrame->stRecBuf.unWrIndex = 0;
                }
            }
        }
        /*! Э��֡��һ���ֶα��ģ��ֶα��ı������η��ͣ� */
        else if(FRAME_FIRST_SEG == (pstCanFrame->can_id & SEG_CODE_FLAG_MASK))
        {
            if((0 != ucSegNum) || (8 != pstCanFrame->can_dlc))
            {
                DEBUG_MSG("E: ucSegNum != 0 or can_dlc != 8 !\r\n");
                break;
            }
            /*! ���������ݸ��Ƶ�Э��֡������ */
            pstSegMsg->ucSegMsgCnt = 0;
            for(i=0; i<8; i++)
            {
                pstSegMsg->aucSegMsgBuf[pstSegMsg->ucSegMsgCnt] \
                    = pstCanFrame->data[i];
                (pstSegMsg->ucSegMsgCnt)++;
            }
            pstSegMsg->ucLastSegNum = ucSegNum;
        }
        /*! Э��֡�м�ֶα��� */
        else if(FRAME_MIDDLE_SEG == (pstCanFrame->can_id & SEG_CODE_FLAG_MASK))
        {
            (pstSegMsg->ucLastSegNum)++;
            if((pstSegMsg->ucLastSegNum != ucSegNum) \
                || (8 != pstCanFrame->can_dlc) \
                || (1 > ucSegNum) || (48 < ucSegNum))
            {
                DEBUG_MSG("E: SegNum != LastSegNum or can_dlc != 8 or ucSegNum value is invalid!\r\n");
                pstSegMsg->ucLastSegNum = 0xff; /*!< �м�ֶα��ĸ�ʽ���� */
                break;
            }
            /*! ���������ݸ��Ƶ�Э��֡������ */
            for(i=0; i<8; i++)
            {
                pstSegMsg->aucSegMsgBuf[pstSegMsg->ucSegMsgCnt] \
                    = pstCanFrame->data[i];
                (pstSegMsg->ucSegMsgCnt)++;
            }
            pstSegMsg->ucLastSegNum = ucSegNum;
        }
        /*! Э��֡�����ֶα��� */
        else if(FRAME_END_SEG == (pstCanFrame->can_id & SEG_CODE_FLAG_MASK))
        {
            if((0 != ucSegNum) || (0xff == pstSegMsg->ucLastSegNum))
            {
                DEBUG_MSG("E: SegNum != 0 or ucLastSegNum value is invalid(0xff)!\r\n");
                break;
            }
            /*! ���������ݸ��Ƶ�Э��֡������ */
            for(i=0; i<pstCanFrame->can_dlc; i++)
            {
                pstSegMsg->aucSegMsgBuf[pstSegMsg->ucSegMsgCnt] \
                    = pstCanFrame->data[i];
                (pstSegMsg->ucSegMsgCnt)++;
            }
            /*! ���յ����ֶα��ĺ󣬽�Э��֡�����������ݸ��Ƶ�CAN���ջ����� */
            for(i=0; i<(pstSegMsg->ucSegMsgCnt); i++)
            {
                pstFrame->stRecBuf.aucBuf[pstFrame->stRecBuf.unWrIndex] \
                    = pstSegMsg->aucSegMsgBuf[i];
                (pstFrame->stRecBuf.unWrIndex)++;
                if(REC_BUF_SIZE <= pstFrame->stRecBuf.unWrIndex)
                {
                    pstFrame->stRecBuf.unWrIndex = 0;
                }
            }
        }
        else
        {
            break;
        }
        ret = 0;
    }while(0);
    
    return ret;
}

/* can�����߳� */
void *can_rec_thread(void *arg)
{
    struct can_frame stCanFrame;
    uint8 ucSrcId = 0, ucDestId = 0;
    frame_check_t astFrameBuf[REC_BUF_NUM] = { 0 };
    struct seg_msg_buf astSegMsgBuf[REC_BUF_NUM] = { 0 };

    while(1)
    {
        /* ������ȡCAN֡ */
        if(0 > can_read(&stCanFrame))
        {
            DEBUG_MSG("E:can read error!\r\n");
            continue;
        }

        /*! ����֡�ж� */
        if((0x80000000 != (stCanFrame.can_id & 0x80000000)) \
            || (1 > stCanFrame.can_dlc))
        {
            DEBUG_MSG("E:can frame error(not data frame or dlc < 1)!\r\n");
            continue;
        }
        /*! ͨ������֡��ʶ����ȡԴ�ڵ㡢Ŀ��ڵ� */
        ucSrcId = (uint8)((stCanFrame.can_id & SRC_ID_MASK) >> 22);
        ucDestId = (uint8)((stCanFrame.can_id & DEST_ID_MASK) >> 16);
        //ucSegNum = (uint8)((stCanFrame.can_id & SEG_CODE_NUM_MASK) >> 8);
        if((3 < ucSrcId) || (3 < ucDestId))
        {
            DEBUG_MSG("E:srcid > 3 or destid > 3! srcid=%d desti=%d\r\n", \
                      ucSrcId, ucDestId);
            continue;
        }

        /* ���Ķ����� */
        if(0 > can_seg_msg_recombine(&(astFrameBuf[ucSrcId]), \
                                     &(astSegMsgBuf[ucSrcId]), &stCanFrame))
        {
            DEBUG_MSG("E:can_seg_msg_recombine() return error!\r\n");
            continue;
        }
        
        /* Э����� */
        if(EXIT_FAILURE == protocol_process(ucSrcId, &(astFrameBuf[ucSrcId])))
        {
            DEBUG_MSG("E:Protocol process error!\r\n");
            continue;
        }
        
        /* �ж��������Ƿ��еȴ��Ķ��� */
        /* �߳��� */
        /* �У��ͷ��ź��� */
        
    } /* end of while(1)... */
}





