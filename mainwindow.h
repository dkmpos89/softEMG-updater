#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QDir>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum {compVersion,execBatch,other};
    enum {msg_alert,msg_notify,msg_info};
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void downComponents(QString url, int postOp);
    void initProcess();
    void mkdirTemp(bool f);
    void writeText(QString text, int color);
public slots:
    void execBatchFile(bool b);
    void readOutput();
    void readError();
    QString getVersionXML(QString file);
    void compararVersiones(bool downSucc, QString strError);
private:
    Ui::MainWindow *ui;
    QProcess *batProcess;
    bool result_state = false;

    const QString UPDATER = QString("cmd");
    const QStringList ARGUMENTS = ( QStringList()<<"" );
    const QString WORKING_DIR = QString("'"+QDir::currentPath()+"'\n");
};

#endif // MAINWINDOW_H
