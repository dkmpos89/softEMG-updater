#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "downloadmanager.h"
#include <QMessageBox>

#include <QXmlStreamReader>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setLogoEMG();
    connect(this, SIGNAL(errorGeneral(int)), ui->progressBar, SLOT(setValue(int)) );


    // paso 1: Creacion de directorio temporal
    mkdirTemp(true);

    getFromXML("C:/Users/diego.campos/Desktop/softEMG/soft-auto-updater/temp/soft-emg-version.xml", "nversion", 2);

    // paso 2: Descargar el archivo XML para verificar la version actual
    // downComponents("https://raw.githubusercontent.com/dkmpos89/softEGM_updates/master/soft-emg-version.xml", compVersion);

    // paso 3: verificar si el software necesita actualizarce
    // getFromXML(QDir::currentPath()+"/temp/soft-emg-version.xml");

    // paso 4: Descargar el archivo ZIP que contiene las actualizaciones
    // downComponents("https://github.com/dkmpos89/softEGM_updates/raw/master/soft-updates.zip", execBatch);
    //** Esta URL incluye el id del commit de github **/
    // url : https://github.com/dkmpos89/softEGM_updates/raw/d591415f3ed9361fec49dd22276f81e1dcc2e3da/soft-updates.zip


    // paso 5: Ejecutar el Batch mediante QProcess

    // paso 6: Eliminar directorio temporal
    // mkdirTemp(false);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mkdirTemp(bool f)
{
    writeText("Directorio temporal ../../temp", msg_notify);
    QString path = QDir::currentPath()+"/temp";

    if(f){
        if(!QDir(path).exists())
            QDir().mkdir(path);
        writeText("^ [OK]", msg_info);
    }else{
        if(QDir(path).exists())
            QDir().rmdir(path);
        writeText("^ [NO]", msg_info);
    }
}

void MainWindow::downComponents(QString url, int postOp)
{
    writeText("Descargando en ../temp/soft-emg-version.xml", msg_notify);
    // paso 2: Descargar la actualizacion
    DownloadManager *downManager = new DownloadManager(this);
    const QUrl iUrl(url);
    downManager->doDownload(iUrl);
    switch(postOp)
    {
    case 0:
        connect(downManager, SIGNAL(downFinished(bool, QString)), this, SLOT(compararVersiones(bool,QString)) );
        break;

    case 1:
        connect(downManager, SIGNAL(downFinished(bool, QString)), this, SLOT(execBatchFile(bool, QString)) );
        break;

    case 2:
        connect(downManager, SIGNAL(downFinished(bool, QString)), this, SLOT(actualizarComboBox(bool, QString)) );
        break;

    default:
        break;
    }
}




//==========================================================================================//
//                                       SLOTS                                              //

void MainWindow::execBatchFile(bool b, QString msg)
{
    // paso 3:
    if(b) QMessageBox::information(this, tr("Success"),tr("Archivo descargado con exito!") );
    else  QMessageBox::information(this, tr("Failed"),tr("Error al descargar el archivo.") );

    //initProcess();
}

void MainWindow::readOutput()
{
    QString buff = QString::fromLatin1(batProcess->readAllStandardOutput());
    writeText(buff, Qt::green);
}

void MainWindow::readError()
{
    QString error = QString::fromLocal8Bit(batProcess->readAllStandardError());
    writeText(error, Qt::red);
}

QStringList MainWindow::getFromXML(QString file, QString token, int cant)
{
    int cont = 0;
    QStringList valtokens;
    QFile archivo(file);

    if (archivo.open(QFile::ReadOnly | QFile::Text))
    {
        QXmlStreamReader reader;
        reader.setDevice(&archivo);
        while (!reader.atEnd())
        {
            reader.readNext();
            if (reader.isStartElement())
            {
                QString name = reader.name().toString();
                QTextStream(stdout)<<name<<endl;
                if( name==token ){
                    if(cont >= cant) break;
                    valtokens << reader.readElementText();
                    cont++;
                }
            }
        }
        if (reader.hasError())
        {
            writeText(reader.errorString(), msg_alert);
        }

    }else{
        writeText("Error al abrir el archivo xml: "+file, msg_alert);
    }

    archivo.close();
    return valtokens;

}

void MainWindow::compararVersiones(bool downSucc, QString strError)
{
    if(downSucc){
        writeText("Comparando con la version instalada del software ../soft-emg-version.xml", msg_info);
        /** cdUp subir hasta el directorio raiz del programa principal **/
        QDir path = QDir::currentPath();
        path.cdUp();

        QString rootPath = path.absolutePath();
        QString newFile = QDir::currentPath()+"/temp/soft-emg-version.xml";
        QString oldFile = rootPath+"/soft-emg-version.xml";

        if(QFile(newFile).exists() && QFile(oldFile).exists()){
            QStringList new_version = getFromXML(newFile, "nversion", 1);
            QStringList old_version = getFromXML(oldFile, "nversion", 1);

            // este bloque se debe cambiar por lo q hara finalmente el software!::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::>
            if(new_version[0] != old_version[0] && old_version.size()>0 && new_version.size()>0){
                writeText("Hay actualizaciones disponibles", msg_notify);
                writeText("Version actual: "+old_version[0]+"", msg_info);
                writeText("Version disponible: "+new_version[0]+"", msg_info);
            }
            else
                writeText("Software en su version mas  reciente!", msg_notify);
        }else{
            writeText("Archivos XML no encontrado...", msg_alert);
            writeText(oldFile, msg_alert);
            writeText(newFile, msg_alert);
        }
    }else{
        writeText("^ ["+strError+"]", msg_alert);
        emit errorGeneral(100);
    }
}

void MainWindow::progressBarSetValue(int val)
{
    if(val==1){
        int cv = ui->progressBar->value()+1;

        if(cv>=99){
            disconnect(mtimer, SIGNAL(timeout()), this, SLOT(progressBarSetValue(int)));
            mtimer->stop();
            mtimer->deleteLater();
            soft_actualizando = false;
            return;
        }
        ui->progressBar->setValue(cv);
    }else{
        ui->progressBar->setValue(val);
    }

}

void MainWindow::actualizarComboBox(bool flag, QString msg)
{
    if(flag) {
        writeText("^ [OK]", msg_info);
        ui->comboB_versiones->clear();
        ui->comboB_versiones->addItem("Latest");
        QString file = QDir::currentPath()+"/temp/soft-emg-version.xml";
        ui->comboB_versiones->addItems(getFromXML(file, "nversion", 10));
        ui->tabWidget->setCurrentIndex(1);
        writeText("^^ [Lista de versiones actualizadas]", msg_info);
    }
    else{
        writeText("^ ["+msg+"]", msg_alert);
    }

    ui->tabWidget->setTabEnabled(1, true);
    soft_actualizando = false;
}




//==========================================================================================//
//                                       FUNCIONES                                          //

void MainWindow::writeText(QString text, int color)
{
    QString line = text;

    QTextCursor cursor = ui->terminal->textCursor();
    QString alertHtml = "<font color=\"red\">";
    QString notifyHtml = "<font color=\"blue\">";
    QString infoHtml = "<font color=\"green\">";
    QString endHtml = "</font><br>";

    switch(color)
    {
        case msg_alert: line.prepend(alertHtml.toLatin1()); break;
        case msg_notify: line.prepend(notifyHtml.toLatin1()); break;
        case msg_info: line.prepend(infoHtml.toLatin1()); break;
        default: line.prepend(infoHtml.toLatin1()); break;
    }

    line = line.append(endHtml.toLatin1());
    ui->terminal->putData(line);
    cursor.movePosition(QTextCursor::End);
    ui->terminal->setTextCursor(cursor);

}

void MainWindow::setLogoEMG()
{
    ui->terminal->clear();
    QTextCursor imageCursor = ui->terminal->textCursor();
    imageCursor.movePosition(QTextCursor::NextWord, QTextCursor::KeepAnchor);
    QPixmap icon = QPixmap(":/images/logo.png");
    QImage image = icon.toImage();
    imageCursor.insertImage(image);
    ui->terminal->setTextCursor(imageCursor);

    QTextCursor cursor = ui->terminal->textCursor();
    cursor.movePosition(QTextCursor::NextWord, QTextCursor::KeepAnchor);
    cursor.insertText("\n");
    cursor.movePosition(QTextCursor::End);
    ui->terminal->setTextCursor(cursor);
}

void MainWindow::on_actionStart_triggered()
{
    if(!soft_actualizando)
    {
        soft_actualizando = true;
        mtimer = new QTimer(this);
        connect(mtimer, SIGNAL(timeout()), this, SLOT(progressBarSetValue(int val=1)));
        mtimer->start(2000);

        mkdirTemp(true);
        downComponents("https://raw.githubusercontent.com/dkmpos89/softEGM_updates/master/soft-emg-version.xml", compVersion);
    }else{
        QMessageBox::information(this, "Importante_", QString("Actualizacion en curso %1%").arg(ui->progressBar->value()));
    }
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

void MainWindow::on_btnActualizarInfo_clicked()
{
    if(!soft_actualizando)
    {
        soft_actualizando = true;
        ui->tabWidget->setTabEnabled(1, false);
        QString url = "https://raw.githubusercontent.com/dkmpos89/softEGM_updates/master/soft-emg-version.xml";
        downComponents(url, actComboBox);
    }else{
        QMessageBox::information(this, "Advertencia_", "No se puede editar la configuracion durante una actualizacion!");
    }

}




