
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
    //int 
    syncSend(); //ͬ������
    asyncSend(); //�첽����
signals:
    
    
private slots:

private:
    NodeCommPrivate *d;
};

#endif

