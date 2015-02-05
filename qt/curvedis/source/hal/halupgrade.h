
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
    /* ��ʼ����FPGA */
    int fpgaUpgradeStart(int node, quint8 ucType, quint32 startAddr);
    /* FPGA������������ */
    int fpgaUpgradeData(int node, quint32 addr, quint8 *pbuf, int len);
    /* FPGA�������� */
    int fpgaUpgradeFinish(int node);
};

#endif // __HALUPGRADE_H

