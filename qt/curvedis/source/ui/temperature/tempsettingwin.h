
#ifndef __TEMPSETTINGWIN_H
#define __TEMPSETTINGWIN_H

#include <QDialog>
#include "../../../build/source/ui/temperature/ui_tempsettingwin.h"


class TempSettingWin : public QDialog, public Ui::TempSettingWin
{
    Q_OBJECT

public:
    TempSettingWin(QWidget *parent=0, int node=1, quint8 ch=1);
    ~TempSettingWin();

private slots:
    void on_setPidParamButton_clicked();
    void on_getPidParamButton_clicked();
    void on_openButton_clicked();
    void on_closeButton_clicked();

private:
    int node; //�ڵ�
    quint8 ch; //ͨ��
};


#endif // __TEMPSETTINGWIN_H

