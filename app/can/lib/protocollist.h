/* name: protocollist.h
 * description: Э���������
 * author: zuokongxiao
 * date: 2014-09-04
 */

#ifndef __PROTOCOLLIST_H
#define __PROTOCOLLIST_H

#include "./list.h"

/* �ź�������ֵ���� */
enum sem_ret_e
{
    SEM_NORMAL_RET = 0,
    SEM_TIMEOUT_RET,
    SEM_ERROR_RET
};

/* Э������ṹ�嶨�� */
struct protocol_list
{
    sem_t semfd; /* �ź�����ʶ�� */
    int semret; /* �ź�������ֵ */
    uint8 ucCmdFlag; /* �����־ */
    uint8 ucCmdType; /* �������� */
    uint8 ucCmdNum; /* ������� */
    uint8 ucCmdContent; /* �������� */
    uint8 ucCmdParamLen; /* �������� */
    uint8 aucCmdParam[32]; /* ������� */
    struct list_head list;
};

int plist_create(struct protocol_list *pstHead);
int plist_add(char cmdtype, char cmdnum, char cmdcontent, struct protocol_list *pstHead);
int plist_del(char cmdtype, char cmdnum, char cmdcontent, struct protocol_list *pstHead);
int plist_match(char cmdtype, char cmdnum, char cmdcontent, char *param, char paramlen, struct protocol_list *pstHead);

#endif

