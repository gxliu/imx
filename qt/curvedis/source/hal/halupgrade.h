
#ifndef __HALUPGRADE_H
#define __HALUPGRADE_H


namespace HalUpgrade
{
    /* ��λ���� */
    int upgradeReset(int node);
    /* ��������ģʽ */
    int enterUpgradeMode(int node);
    /* ��ʼ���� */
    int upgradeStart(int node, quint32 startAddr);
    /* ������������ */
    int upgradeData(int node, quint32 addr, quint8 *pbuf, int len);
    /* �������� */
    int upgradeFinish(int node);
};

#endif // __HALUPGRADE_H

