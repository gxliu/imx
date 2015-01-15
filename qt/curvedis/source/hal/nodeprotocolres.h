
#ifndef __NODEPROTOCOLRES_H
#define __NODEPROTOCOLRES_H

/*! Э��֡��󳤶�*/
#define PROTOCOL_FRAME_MAX_SIZE                 (250u)
/*! ACK֡��󳤶� */
#define ACK_FRAME_MAX_SIZE                      (10u)
/*! REP֡��󳤶� */
#define REP_FRAME_MAX_SIZE                      (32u)
/*! EVT֡��󳤶� */
#define EVT_FRAME_MAX_SIZE                      (10u)
/*! ��ѯ��������ݵ���󳤶� */
#define QUERY_CMD_STATE_DATA_MAX_SIZE           (PROTOCOL_FRAME_MAX_SIZE-8)

/*! Э��֡��ʼ�� */
#define PROTOCOL_FRAME_START_CODE               (0xAA)
/*! Э��ֹ֡ͣ�� */
#define PROTOCOL_FRAME_STOP_CODE                (0x55)

/*! Э��֡��ʽ�±� */
#define FRAME_START_INDEX                       (0u)
#define FRAME_CONTROL_INDEX                     (1u)
#define FRAME_DATALEN_INDEX                     (2u)
#define FRAME_COMMAND_INDEX                     (3u)
#define FRAME_COMMAND_TYPE_INDEX                (3u)
#define FRAME_COMMAND_NUM_INDEX                 (4u)
#define FRAME_COMMAND_CONTENT_INDEX             (5u)
#define FRAME_COMMAND_PARAM_INDEX               (6u)

/*! ��������� */
#define FRAME_CONTROL_CODE_COMMAND_REP          (0x00) /*!< �´�����֡(REP) */
#define FRAME_CONTROL_CODE_COMMAND_NO_REP       (0x40) /*!< �´�����֡(NO REP) */
#define FRAME_CONTROL_CODE_TS_HEAD              (0x41)
#define FRAME_CONTROL_CODE_TS_BODY              (0x42)
#define FRAME_CONTROL_CODE_TS_TAIL              (0x43)
#define FRAME_CONTROL_CODE_REP                  (0xC0) /*!< ��վ��Ӧ֡ */
#define FRAME_CONTROL_CODE_ACK                  (0xD4) /*!< ��վӦ��֡ */
#define FRAME_CONTROL_CODE_EVT                  (0xF8) /*!< ��վ�쳣֡ */
#define FRAME_CONTROL_CODE_UPLOAD               (0xD8) /*!< ��վ�����ϴ�֡ */

/*! ������mask */
#define CONTROL_CODE_TRANSDIR_MASK              (0x80)
#define CONTROL_CODE_RETURNFLAG_MASK            (0x40)
#define CONTROL_CODE_ABNORMALFLAG_MASK          (0x20)
#define CONTROL_CODE_FUNCTION_MASK              (0x1C)
#define CONTROL_CODE_IDENT_MASK                 (0x03)

/*! �����빦�ܱ��� */
#define CONTROL_CODE_FUNCTION_CMD               (0x00) /*!< ���� */
#define CONTROL_CODE_FUNCTION_ACK               (0x14) /*!< Ӧ��֡ */
#define CONTROL_CODE_FUNCTION_UPLOAD            (0x18) /*!< �����ϴ�֡ */

/*! ������ʶ����� */
#define CONTROL_CODE_IDENT_CMD                  (0x00) /*!< ���� */
#define CONTROL_CODE_IDENT_TS_HEAD              (0x01) /*!< ʱ�����ͷ */
#define CONTROL_CODE_IDENT_TS_BODY              (0x02) /*!< ʱ������� */
#define CONTROL_CODE_IDENT_TS_TAIL              (0x03) /*!< ʱ�����β */

/*! ֡���践��REP */
#define FRAME_NOT_RETURN_REP(x) (CONTROL_CODE_RETURNFLAG_MASK == ((x)&CONTROL_CODE_RETURNFLAG_MASK))
/*! ֡�践��REP */
#define FRAME_RETURN_REP(x)     (CONTROL_CODE_RETURNFLAG_MASK != ((x)&CONTROL_CODE_RETURNFLAG_MASK))

/*! ����֡״̬ */
typedef enum
{
    NONE_RETURN = 0x00, /*! ������ */
    NEED_ACK, /*!< ��Ҫ����ACK */
    NEED_REP, /*!< ��Ҫ����REP */
    NEED_EVT, /*!< ��Ҫ����EVT */
    NEED_UPLOAD, /*!< �����ϴ� */
} return_frame_state_e;

/*! ֡��ʽ���״̬���� */
typedef enum
{
    FRAME_CHECK_START = 0x00u,
    FRAME_CHECK_CONTROL,
    FRAME_CHECK_DATALEN,
    FRAME_CHECK_DATA,
    FRAME_CHECK_CHECKSUM,
    FRAME_CHECK_STOP,
    FRAME_CHECK_OK,
    FRAME_CHECK_ERROR
} check_state_e;

#endif // __NODEPROTOCOLRES_H
