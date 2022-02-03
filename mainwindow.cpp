#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>
#include <QFileDialog>
#include <QFile>
#include <QInputDialog>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QDir().mkdir("files");
    LoadFiles();
    LoadConfig();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::LoadFiles()
{
    ui->comboBox->clear();
    QDir directory("files");
    QStringList files = directory.entryList(QStringList() << "*.cred", QDir::Files);
    foreach(QString filename, files) {
        ui->comboBox->addItem(filename.remove(".cred"));
    }
}

bool MainWindow::LoadConfig()
{
    QFile file("files/config.txt");
    if(!file.open(QIODevice::ReadOnly)){
        QTextStream(stdout) << "Cannot open the file. \n" << file.errorString() << "\n";
        return false;
    }
    QTextStream stream(&file);
    ui->comboBox->setCurrentText(stream.readLine());
    ui->checkBox->setChecked(stream.readLine().toInt());

    file.close();
    return true;
}

bool MainWindow::saveConfig(){
    QFile file("files/config.txt");
    if(!file.open(QIODevice::WriteOnly)){
        QTextStream(stdout) << "Cannot open the file. \n" << file.errorString() << "\n";
        return false;
    }
    QTextStream stream(&file);
    stream << ui->comboBox->currentText() << "\n";
    stream << ui->checkBox->isChecked();

    file.close();
    return true;
}

void MainWindow::on_pushButton_clicked()
{
    QString path = QDir::homePath();
    QString selected = ui->comboBox->currentText();
    if (QFile::exists(path + "/AppData/Local/Programs/PokeMMO/config/savedcredentials.properties")){
        QFile::remove(path + "/AppData/Local/Programs/PokeMMO/config/savedcredentials.properties");
    }
    QFile::copy(QString("files/%1.cred").arg(selected), path + "/AppData/Local/Programs/PokeMMO/config/savedcredentials.properties");

    QProcess::startDetached(path + "/AppData/Local/Programs/PokeMMO/PokeMMO.exe");

    if (ui->checkBox->isChecked()){
        QString GECpath = path + "/AppData/Local/Programs/PokeMMO/data/mods/Gilans Encounter Counter/GEC";
        QProcess *child = new QProcess();
        child->setWorkingDirectory(GECpath);
        child->start(GECpath + "/GEC.exe");
    }

    saveConfig();
    QApplication::quit();
}

void MainWindow::on_pushButton_2_clicked()
{
    QString path = QDir::homePath();
    QString fileName = QFileDialog::getOpenFileName(this, "Select Credential File", path + "/AppData/Local/Programs/PokeMMO/config");
    QTextStream(stdout) << fileName;
    if (fileName != ""){
        bool ok;
        QString text = QInputDialog::getText(this, tr("Save as"),
                                             tr("Save file as:"), QLineEdit::Normal,
                                             "", &ok);
        if (ok && !text.isEmpty()){
            if (QFile::exists(QString("files/%1.cred").arg(text)))
            {
                QFile::remove(QString("files/%1.cred").arg(text));
            }
            QFile::copy(fileName, QString("files/%1.cred").arg(text));
        }
        LoadFiles();
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    QString selected = ui->comboBox->currentText();
    if (QFile::exists(QString("files/%1.cred").arg(selected)))
    {
        QFile::remove(QString("files/%1.cred").arg(selected));
    }
    LoadFiles();
}
