
#include <QtGui>
#include <QDebug>
#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this); //��ʼ������

    //this->setWindowFlags(Qt::FramelessWindowHint); //ȥ��������
    this->resize(480, 272); //��ʾ��С
}

//MainWindow::~MainWindow()
//{

//}


