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
    enum {compVersion,execBatch,actComboBox,other};
    enum {msg_alert,msg_notify,msg_info};
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void downComponents(QString url, int postOp);
    void initProcess();
    void mkdirTemp(bool f);
    void writeText(QString text, int color);
    void setLogoEMG();
public slots:
    void execBatchFile(bool dowSucc, QString msg);
    void readOutput();
    void readError();
    QStringList getFromXML(QString file, QString token, int cant);
    void compararVersiones(bool downSucc, QString strError);
    void progressBarSetValue(int val);
    void actualizarComboBox(bool flag, QString msg);
private slots:
    void on_actionStart_triggered();
    void on_btnActualizarInfo_clicked();

    void on_actionClean_triggered();
    void on_actionStop_triggered();
    void on_actionCMD_triggered();

    void on_btnAplicarProxy_clicked();

signals:
    errorGeneral(int);
private:
    Ui::MainWindow *ui;
    QProcess *batProcess;
    bool result_state = false;

    const QString UPDATER = QString("cmd");
    const QStringList ARGUMENTS = ( QStringList()<<"" );
    const QString WORKING_DIR = QString(QDir::currentPath());

    bool soft_actualizando = false; // indica si se inicio alguna actualizacion del software
    QTimer *mtimer;

    QString gitrepository = "https://github.com/dkmpos89/softEGM_updates/";
    QString version_to_updated = "Latest";

    QString proxyHostName = "";
    qint16 proxyPort = -1;

};

#endif // MAINWINDOW_H
