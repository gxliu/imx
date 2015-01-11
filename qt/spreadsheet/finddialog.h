
#ifndef _FINDDIALOG_H
#define _FINDDIALOG_H

#include <QDialog>

/* ǰ������ */
class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;

/* �ඨ�� */
class FindDialog : public QDialog
{
    Q_OBJECT
    
public:
    FindDialog(QWidget *parent = 0);
    
signals:
    void findNext(const QString &str, Qt::CaseSensitivity cs);
    void findPrevious(const QString &str, Qt::CaseSensitivity cs);

private slots:
    void findClicked();
    void enableFindButton(const QString &text);

private:
    QLabel *label;
    QLineEdit *lineEdit;
    QCheckBox *caseCheckBox;
    QCheckBox *backwardCheckBox;
    QPushButton *findButton;
    QPushButton *closeButton;
};


#endif

