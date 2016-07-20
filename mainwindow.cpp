#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "downloadmanager.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    DownloadManager *downManager = new DownloadManager(this);
    const QUrl imageUrl("http://www.cuentamealgobueno.com/wp-content/uploads/2016/04/apadrina20160422.png");
    downManager->doDownload(imageUrl);
}

MainWindow::~MainWindow()
{
    delete ui;
}
