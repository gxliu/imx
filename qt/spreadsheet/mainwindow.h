
#ifndef _MAINWINDOW_H
#define _MAINWINDOW_H


#include <QMainWindow>

/* 前置声明 */
class QAction;
class QLabel;
class FindDialog;
class Spreadsheet;

/* 类定义 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow();
    
protected:
    void closeEvent(QCloseEvent *event); //QWidget虚函数
    
private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void find();
    void goToCell();
    void sort();
    void about();
    void openRecentFile();
    void updateStatusBar();
    void spreadsheetModified();
    
private:
    void createActions();
    void createMenus();
    void createContextMenu();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool okToContinue();
    bool loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    void updateRecentFileActions();
    QString strippedName(const QString &fullFileName);

    Spreadsheet *spreadsheet;
    FindDialog *findDialog;
    QLabel *locationLael;
    QLabel *formulaLabel;
    QStringList recentFiles;
    QString curFile;
    
    enum{MaxRecentFiles = 5};
    QAction *recentFileActions[MaxRecentFiles];
    QAction *separatorAction;
    
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *selectSubMenu;
    QMenu *toolsMenu;
    QMenu *optionsMenu;
    QMenu *helpMenu;
    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QAction *newAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *exitAction;
    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *deleteAction;
    QAction *selectRowAction;
    QAction *selectColumnAction;
    QAction *selectAllAction;
    QAction *findAction;
    QAction *goToCellAction;
    QAction *recalculateAction;
    QAction *sortAction;
    QAction *showGridAction;
    QAction *autoRecalcAction;
    QAction *aboutAction;
    QAction *aboutQtAction;
};

#endif

