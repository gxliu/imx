
#ifndef __NODECOMM_H
#define __NODECOMM_H

#include <QObject>

class NodeCommPrivate;

class NodeComm : public QObject
{
    Q_OBJECT
public:
    NodeComm();
    ~NodeComm();
    void start();
    int query(int node, quint8 cmdType, quint8 cmdNum, quint8 cmdContent, \
              quint8 *cmdParam, quint8 paramLen, quint8 *retData, int retLen);
    /* ͬ������ */
    int syncSend(int node, quint8 cmdType, quint8 cmdNum, quint8 cmdContent, \
                 quint8 *cmdParam, quint8 paramLen, int timeout);
    int syncSend(int node, quint8 cmdType, quint8 cmdNum, quint8 cmdContent, \
                 quint8 *cmdParam, quint8 paramLen, quint8 *retData, int retLen, \
                 int timeout);
    int syncSend(int node, quint8 ctlCode, quint8 cmdType, quint8 cmdNum, \
                 quint8 cmdContent, quint8 *cmdParam, quint8 paramLen, int timeout);
    /* �첽���� */
    int asyncSend(int node, quint8 cmdType, quint8 cmdNum, quint8 cmdContent, \
                  quint8 *cmdParam, quint8 paramLen, int timeout);
    int asyncSend(int node, quint8 cmdType, quint8 cmdNum, quint8 cmdContent, \
                  quint8 *cmdParam, quint8 paramLen, quint8 *retData, int retLen, \
                  int timeout);
    int asyncSend(int node, quint8 ctlCode, quint8 cmdType, quint8 cmdNum, \
                  quint8 cmdContent, quint8 *cmdParam, quint8 paramLen, int timeout);
signals:

private slots:
    void frameReady(int node, quint8 *data, int datalen);

private:
    NodeCommPrivate *d;
};

#endif

