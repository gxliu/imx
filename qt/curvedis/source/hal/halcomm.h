
#ifndef __HALCOMM_H
#define __HALCOMM_H


class NodeComm;

namespace HalComm
{
    NodeComm *nodeCommInstance();

    int nodeQuery(int node, quint8 cmdType, quint8 cmdNum, quint8 cmdContent, \
                 quint8 *cmdParam, quint8 paramLen, quint8 *retData, int retLen);
    /* ͬ������ */
    int nodeSyncSend(int node, quint8 cmdType, quint8 cmdNum, quint8 cmdContent, \
                 quint8 *cmdParam, quint8 paramLen, int timeout);
    int nodeSyncSend(int node, quint8 cmdType, quint8 cmdNum, quint8 cmdContent, \
                 quint8 *cmdParam, quint8 paramLen, quint8 *retData, int retLen, \
                 int timeout);
    int nodeSyncSend(int node, quint8 ctlCode, quint8 cmdType, quint8 cmdNum, \
                 quint8 cmdContent, quint8 *cmdParam, quint8 paramLen, int timeout);
    /* �첽���� */
    int nodeAsyncSend(int node, quint8 cmdType, quint8 cmdNum, quint8 cmdContent, \
                  quint8 *cmdParam, quint8 paramLen, int timeout);
    int nodeAsyncSend(int node, quint8 cmdType, quint8 cmdNum, quint8 cmdContent, \
                  quint8 *cmdParam, quint8 paramLen, quint8 *retData, int retLen, \
                  int timeout);
    int nodeAsyncSend(int node, quint8 ctlCode, quint8 cmdType, quint8 cmdNum, \
                  quint8 cmdContent, quint8 *cmdParam, quint8 paramLen, int timeout);
};

#endif // __HALCOMM_H

