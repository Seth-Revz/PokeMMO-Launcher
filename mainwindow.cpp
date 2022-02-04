#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>
#include <QFileDialog>
#include <QFile>
#include <QInputDialog>
#include <QStandardPaths>
#include <QPalette>
#include <QPainter>
#include <QScreen>
#include <QLabel>
#include <QMouseEvent>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::FramelessWindowHint);

    QRect screen = QApplication::primaryScreen()->geometry();
    int windowHeight = screen.height()/4;
    int windowWidth = screen.width()/4;

    setFixedSize(windowWidth, windowHeight);

    _pixmapBg.load(":/res/background.jpeg");

    this->frame = new QFrame(this);
    this->frame->setGeometry(0, 5, 1000, windowHeight/14);
    this->frame->setStyleSheet("background-color: rgba(0,0,0,90);");
    this->frame->installEventFilter(this);

    QLabel *label = new QLabel(this);
    label->setText("PokeMMO Launcher");
    label->setFont(QFont("Times", windowHeight/14 - 6));
    label->adjustSize();
    label->setStyleSheet("color: rgb(255, 255, 255);");
    label->setGeometry((windowWidth-label->width())/2, 5, label->width(), windowHeight/14);

    QPushButton *close_button = new QPushButton(this);
    close_button->setGeometry(windowWidth-(windowHeight/14)-5, 5, windowHeight/14, windowHeight/14);
    close_button->setFlat(true);
    QPixmap pixmap(":/res/icon_close.png");
    QIcon ButtonIcon(pixmap);
    close_button->setIcon(ButtonIcon);
    close_button->setIconSize(QSize(windowHeight/14, windowHeight/14));
    close_button->setStyleSheet("QPushButton:pressed{background-color: rgba(0,0,0,100);}");


    connect(close_button, SIGNAL(clicked()), this, SLOT(on_close_button_clicked()));

    QDir().mkdir("files");
    LoadFiles();
    LoadConfig();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *pe)
{
    QPainter painter(this);

    auto winSize = size();
    auto pixmapRatio = (float)_pixmapBg.width() / _pixmapBg.height();
    auto windowRatio = (float)winSize.width() / winSize.height();

    if(pixmapRatio > windowRatio)
    {
      auto newWidth = (int)(winSize.height() * pixmapRatio);
      auto offset = (newWidth - winSize.width()) / -2;
      painter.drawPixmap(offset, 0, newWidth, winSize.height(), _pixmapBg);
    }
    else
    {
      auto newHeight = (int)(winSize.width() / pixmapRatio);
      auto offset = (newHeight - winSize.height()) / -2;
      painter.drawPixmap(0, offset, winSize.width(), newHeight, _pixmapBg);
    }
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
//    ui->checkBox->setChecked(stream.readLine().toInt());

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
//    stream << ui->checkBox->isChecked();

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

//    if (ui->checkBox->isChecked()){
//        QString GECpath = path + "/AppData/Local/Programs/PokeMMO/data/mods/Gilans Encounter Counter/GEC";
//        QProcess *child = new QProcess();
//        child->setWorkingDirectory(GECpath);
//        child->start(GECpath + "/GEC.exe");
//    }

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

void MainWindow::mousePressEvent(QMouseEvent *event) {
    mouseClick_X_Coordinate = event->position().x();
    mouseClick_Y_Coordinate = event->position().y();
}

/**
    Works with mousePressEvent to allow the window to be dragged by clicking and holding.
    @param the mouse event observed by QT
*/
void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    move(event->globalPosition().x()-mouseClick_X_Coordinate,event->globalPosition().y()-mouseClick_Y_Coordinate);
}

void MainWindow::on_close_button_clicked()
{
//    saveConfig();
    QApplication::quit();
}
