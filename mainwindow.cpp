#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "downloadmanager.h"
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // paso 1: Creacion de directorio temporal
    mkdirTemp(true);

    // paso 2: Descargar el archivo de actualizacion
    downComponents("https://github.com/dkmpos89/softEGM_updates/raw/master/Tesis.exe");

    // paso 3: Ejecutar el Batch mediante QProcess

    // paso 4: Eliminar directorio temporal
    // mkdirTemp(false);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mkdirTemp(bool f)
{
    QString path = QDir::currentPath()+"/temp";

    if(f){
        if(!QDir(path).exists())
            QDir().mkdir(path);
    }else
        if(QDir(path).exists())
            QDir().rmdir(path);
}

void MainWindow::downComponents(QString url)
{
    // paso 2: Descargar la actualizacion
    DownloadManager *downManager = new DownloadManager(this);
    const QUrl iUrl(url);
    downManager->doDownload(iUrl);
    connect(downManager, SIGNAL(downFinished(bool)), this, SLOT(execBatchFile(bool)) );
}

void MainWindow::execBatchFile(bool b)
{
    // paso 3:
    if(b) QMessageBox::information(this, tr("Success"),tr("Archivo descargado con exito!") );
    else  QMessageBox::information(this, tr("Failed"),tr("Error al descargar el archivo.") );

    initProcess();
}

void MainWindow::initProcess()
{
    batProcess = new QProcess(this);
    batProcess->setProcessChannelMode( QProcess::SeparateChannels );
    batProcess->start(UPDATER, ARGUMENTS, QProcess::ReadWrite);
    batProcess->waitForStarted(5000);

    connect(batProcess, SIGNAL( readyReadStandardOutput() ), this, SLOT(readOutput()) );
    connect(batProcess, SIGNAL( readyReadStandardError() ), this, SLOT(readError()) );

    batProcess->write("cd C:/Users/diego.campos/Desktop/soft\n");
    batProcess->waitForFinished(5000);
    batProcess->write("update.bat\n");

}

void MainWindow::readOutput()
{
    QString buff = QString::fromLatin1(batProcess->readAllStandardOutput());
    ui->terminal->putData(buff);
    ui->terminal->putData("\n");
}

void MainWindow::readError()
{
    QPalette p = ui->terminal->palette();
    p.setColor(QPalette::Text, Qt::red);
    ui->terminal->setPalette(p);

    QString error = QString::fromLocal8Bit(batProcess->readAllStandardError());
    ui->terminal->putData(error);
    ui->terminal->putData("\n");

    p.setColor(QPalette::Text, Qt::green);
    ui->terminal->setPalette(p);
}

