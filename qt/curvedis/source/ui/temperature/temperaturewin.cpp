
#include <QDebug>
#include <QtGui>
#include "temperaturewin.h"
#include "qcustomplot.h"
#include "haltemperature.h"
#include "tempsettingwin.h"


/* ���߻��������� 30min*60 */
#define CURVE_BUF_SIZE           1800
/* ���ڴ�С���� 5min*60 */
#define CURVE_DIS_SIZE           300

class TemperatureWinPrivate
{
public:
    TemperatureWinPrivate();
    ~TemperatureWinPrivate();
    int updateCurveData(QVector<double> &x, QVector<double> &y, double value);
    int viewCurveLeft(int steps);
    int viewCurveRight(int steps);
public:
    QCustomPlot *customPlot;
    int timerid; //��ʱ��
    int updateCnt; //���¼�����
    int updatePos; //���ݸ���λ��
    int disStartPos; //��ʾ������ʼλ��
    int disEndPos; //��ʾ���ڽ���λ��
    int viewStartPos; //�鿴������ʼλ��
    double xData[CURVE_BUF_SIZE]; //���ߺ���������
    double yData[CURVE_BUF_SIZE]; //��������������
    double curValue; //��ǰֵ
    int node; //�ڵ��
    int ch; //ͨ����
};

TemperatureWinPrivate::TemperatureWinPrivate()
{
    int i;
    for(i = 0; i<CURVE_BUF_SIZE; i++)
    {
        xData[i] = i;
        yData[i] = 0.0;
    }
    updateCnt = 0;
    updatePos = 0;
    disStartPos = 0;
    viewStartPos = 0;
    disEndPos = CURVE_DIS_SIZE-1;
    curValue = 30.0;
}

TemperatureWinPrivate::~TemperatureWinPrivate()
{

}

int TemperatureWinPrivate::updateCurveData(QVector<double> &x, \
                                           QVector<double> &y, double value)
{
    int i = 0;
    /* �������� */
    updateCnt++;
    if((CURVE_BUF_SIZE-1) == updatePos)
    {
        for(i=0; i<(CURVE_BUF_SIZE-1); i++)
        {
            xData[i] = xData[i+1];
            yData[i] = yData[i+1];
        }
        yData[updatePos] = value;
        xData[updatePos] = updateCnt-1;
        //double temp = updateCnt-1;
        //yData[updatePos] = 30 + qSin(temp/5.0) * 8; //����
    }
    else
    {
        yData[updatePos] = value;
        //double temp = updateCnt;
        //yData[updatePos] = 30 + qSin(temp/5.0) * 8; //����
        updatePos++;
    }
    /* ��ȡ���� */
    qDebug() << " ";
    qDebug("dis_startpos:%d dis_endpos:%d", disStartPos, disEndPos);
    for(i=0; i<CURVE_DIS_SIZE; i++)
    {
        x[i] = xData[disStartPos+i];
        y[i] = yData[disStartPos+i];
    }
    qDebug("endx:%f endy:%f", x[CURVE_DIS_SIZE-1], y[CURVE_DIS_SIZE-1]);
    if((CURVE_BUF_SIZE-CURVE_DIS_SIZE) == disStartPos)
    {
        disStartPos = CURVE_BUF_SIZE-CURVE_DIS_SIZE;
        disEndPos = CURVE_BUF_SIZE-1;
    }
    else if(300 >= updatePos)
    {
        disStartPos = 0;
        disEndPos = CURVE_DIS_SIZE-1;
    }
    else
    {
        disStartPos++;
        disEndPos++;
    }
    //qDebug("next startpos:%d endpos:%d", disStartPos, disEndPos);
    return disStartPos;
}

int TemperatureWinPrivate::viewCurveLeft(int steps)
{
    if(0 == viewStartPos)
    {
        return 0;
    }
    if(viewStartPos < steps)
    {
        viewStartPos = 0;
    }
    else
    {
        viewStartPos = viewStartPos - steps;
    }
    QVector<double> x(CURVE_DIS_SIZE), y(CURVE_DIS_SIZE);
    int i;
    for(i=0; i<CURVE_DIS_SIZE; i++)
    {
        x[i] = xData[viewStartPos+i];
        y[i] = yData[viewStartPos+i];
    }
    customPlot->xAxis->setRange(x[0], x[CURVE_DIS_SIZE-1]);
    customPlot->graph(0)->setData(x, y);
    customPlot->replot();
    return viewStartPos;
}

int TemperatureWinPrivate::viewCurveRight(int steps)
{
    if(disStartPos <= viewStartPos)
    {
        viewStartPos = disStartPos;
        return viewStartPos;
    }
    if(disStartPos <= (viewStartPos+steps))
    {
        viewStartPos = disStartPos;
    }
    else
    {
        viewStartPos = viewStartPos + steps;
    }
    QVector<double> x(CURVE_DIS_SIZE), y(CURVE_DIS_SIZE);
    int i;
    for(i=0; i<CURVE_DIS_SIZE; i++)
    {
        x[i] = xData[viewStartPos+i];
        y[i] = yData[viewStartPos+i];
    }
    customPlot->xAxis->setRange(x[0], x[CURVE_DIS_SIZE-1]);
    customPlot->graph(0)->setData(x, y);
    customPlot->replot();
    return viewStartPos;
}

/*
 * public
 */
TemperatureWin::TemperatureWin(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this); //��ʼ������
    this->setWindowFlags(Qt::FramelessWindowHint); //ȥ��������
    this->resize(480, 272); //��ʾ��С
    /* ���� */
    d = new TemperatureWinPrivate;
    d->customPlot = new QCustomPlot(this);
    d->customPlot->resize(480, 190);
    QVector<double> x(CURVE_DIS_SIZE), y(CURVE_DIS_SIZE);
    for(int i=0; i<CURVE_DIS_SIZE; ++i)
    {
        x[i] = (double)i;
        y[i] = 0.0;
    }
    d->customPlot->addGraph();
    d->customPlot->graph(0)->setData(x, y);
    d->customPlot->xAxis->setRange(0, CURVE_DIS_SIZE);
    d->customPlot->yAxis->setRange(20, 45);
    d->customPlot->show();

    startstopButton->setEnabled(true);
    startstopButton->setText(tr("START"));
    leftButton->setEnabled(false);
    leftButton->setAutoRepeat(true);
    leftButton->setAutoRepeatInterval(100);
    rightButton->setEnabled(false);
    rightButton->setAutoRepeat(true);
    rightButton->setAutoRepeatInterval(100);

    nodeComboBox->addItem("1");
    nodeComboBox->addItem("3");
    chComboBox->addItem("0");
    chComboBox->addItem("1");
    chComboBox->addItem("2");
}

TemperatureWin::~TemperatureWin()
{
    if(d->timerid != 0)
    {
        killTimer(d->timerid);
        d->timerid = 0;
    }
    delete d;
}

/* ��ʱ���� */
void TemperatureWin::timerEvent(QTimerEvent *event)
{
    //qDebug("timer event, id %d", event->timerId());
    if(d->timerid != event->timerId())
    {
        return;
    }
    /* ��ȡ�¶�ֵ */
    d->curValue = HalTemperature::quireCurTemperature(d->node, (quint8)(d->ch));
    /* ��ʾ */
    QVector<double> x(CURVE_DIS_SIZE), y(CURVE_DIS_SIZE);
    d->updateCurveData(x, y, d->curValue);
    //qDebug("startpos:%f endpos:%f", (x[0]), (x[CURVE_DIS_SIZE-1]));
    d->customPlot->xAxis->setRange(x[0], x[CURVE_DIS_SIZE-1]);
    d->customPlot->graph(0)->setData(x, y);
    d->customPlot->replot();
}

/* ���� */
void TemperatureWin::on_returnButton_clicked()
{
    this->close();
}

/* ����ֹͣ */
void TemperatureWin::on_startstopButton_clicked()
{
    if(tr("START") == startstopButton->text())
    {
        d->timerid = startTimer(1000); //��ʱ��
        startstopButton->setText(tr("STOP"));
        leftButton->setEnabled(false);
        rightButton->setEnabled(false);
    }
    else //ֹͣ
    {
        if(d->timerid != 0)
        {
            killTimer(d->timerid);
            d->timerid = 0;
        }
        d->viewStartPos = d->disStartPos;
        startstopButton->setText(tr("START"));
        if(0 < d->viewStartPos)
        {
            leftButton->setEnabled(true);
        }
    }
}

/* ����鿴 */
void TemperatureWin::on_leftButton_clicked()
{
    d->viewCurveLeft(5);
    if(0 == d->viewStartPos)
    {
        leftButton->setEnabled(false);
    }
    if(d->viewStartPos != d->disStartPos)
    {
        rightButton->setEnabled(true);
    }
}

/* ���Ҳ鿴 */
void TemperatureWin::on_rightButton_clicked()
{
    d->viewCurveRight(5);
    if(d->disStartPos == d->viewStartPos)
    {
        rightButton->setEnabled(false);
    }
    if(d->viewStartPos != d->disStartPos)
    {
        leftButton->setEnabled(true);
    }
}

void TemperatureWin::on_nodeComboBox_currentIndexChanged(const QString & text )
{
    d->node = text.toInt();
    qDebug("node=%d", d->node);
}

void TemperatureWin::on_chComboBox_currentIndexChanged(const QString & text )
{
    d->ch = text.toInt();
    qDebug("ch=%d", d->ch);
}

/* ���� */
void TemperatureWin::on_settingButton_clicked()
{
    startstopButton->setEnabled(false);
    returnButton->setEnabled(false);
    settingButton->setEnabled(false);
    TempSettingWin *settingWin = new TempSettingWin(this, d->node, d->ch);
    settingWin->show();
    settingWin->exec();
    delete settingWin;
    startstopButton->setEnabled(true);
    returnButton->setEnabled(true);
    settingButton->setEnabled(true);
}


