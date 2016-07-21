#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "downloadmanager.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    downComponents();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::downComponents()
{
    DownloadManager *downManager = new DownloadManager(this);
    const QUrl iUrl("https://github.com/dkmpos89/softEGM_updates/raw/master/Tesis.exe");
    downManager->doDownload(iUrl);
}

void MainWindow::initProcess()
{
    //
}

void MainWindow::execBatchFile()
{
    //
}
