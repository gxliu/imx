
#ifndef __NODECOMM_H
#define __NODECOMM_H

#include <QObject>

class NodeComm : public QOBject
{
    Q_OBJECT
public:
    NodeComm();
    ~NodeComm();
    
    syncSend(); //ͬ������
    asyncSend(); //�첽����
signals:
    
    
private slots:
    
};

#endif

