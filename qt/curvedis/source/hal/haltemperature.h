
#ifndef __HALTEMPERATURE_H
#define __HALTEMPERATURE_H


namespace HalTemperature
{
    /* ��ѯ��ǰ�¶� */
    float quireCurTemperature(int node, quint8 ch);
    /* �¶ȿ��� */
    int temperatureCtl(int node, quint8 ch, quint8 state, float temp);
    /* ���¶ȿ��Ʋ�����Ŀ��ֵ */
    int openTemperatureCtl(int node, quint8 ch, float tarTemp);
    /* �ر��¶ȿ��� */
    int closeTemperatureCtl(int node, quint8 ch);
    /* ����PID���� */
    int setTemperaturePidParam(int node, quint8 ch, float kp, float ki, float kd);
    /* ��ȡPID���� */
    int getTemperaturePidParam(int node, quint8 ch, float *kp, float *ki, float *kd);
};

#endif // __HALTEMPERATURE_H

