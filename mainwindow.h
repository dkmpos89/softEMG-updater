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
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void downComponents(QString url);
    void initProcess();
    void mkdirTemp(bool f);
public slots:
    void execBatchFile(bool b);
    void readOutput();
    void readError();
private:
    Ui::MainWindow *ui;
    QProcess *batProcess;
    bool result_state = false;

    const QString UPDATER = QString("cmd");
    const QStringList ARGUMENTS = ( QStringList()<<"" );
    const QString WORKING_DIR = QString("'"+QDir::currentPath()+"'\n");
};

#endif // MAINWINDOW_H
