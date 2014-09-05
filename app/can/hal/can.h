/* name: can.h
 * description: CAN�����ӿ�
 * author: zuokongxiao
 * date: 2014-08-21
 */
#ifndef __CAN_H
#define __CAN_H

/* CAN���ı�ʶ������ */
/* Դ�ڵ��ַmask */
#define SRC_ID_MASK             0x07C00000
/* Ŀ��ڵ��ַmask */
#define DEST_ID_MASK            0x001F0000
/* �ֶ���mask */
#define SEG_CODE_MASK           0x0000FF00
/* �ֶα�־mask */
#define SEG_CODE_FLAG_MASK      0x0000C000
/* �ֶκ�mask */
#define SEG_CODE_NUM_MASK       0x00003F00
/* �ֶα�־ */
#define FRAME_NONE_SEG          0x00000000 // �޷ֶ�
#define FRAME_FIRST_SEG         0x00004000 // ��һ���ֶ�
#define FRAME_MIDDLE_SEG        0x00008000 // �м�ֶ�
#define FRAME_END_SEG           0x0000C000 // ���ֶ�


int can_open(void);
int can_close(void);
int can_read(struct can_frame *pstCanFrame);
int can_write(struct can_frame *pstCanFrame);
int can_write_test(uint8 ucBoardType);
int send_data_to_can(int lBoardType, char *pcCmd, int lCmdLen);

#endif
