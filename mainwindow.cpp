#include "mainwindow.h"
#include <QApplication>
#include <QProcess>
#include <QFileDialog>
#include <QFile>
#include <QInputDialog>
#include <QStandardPaths>
#include <QPalette>
#include <QPainter>
#include <QScreen>
#include <QPushButton>
#include <QLabel>
#include <QMouseEvent>
#include <QComboBox>
#include <QFontDatabase>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::FramelessWindowHint);

    QRect screen = QApplication::primaryScreen()->geometry();
    int windowHeight = screen.height()/2;
    int windowWidth = screen.width()/2;
    setFixedSize(windowWidth, windowHeight);

    int id = QFontDatabase::addApplicationFont(":/res/mechabold.ttf");
    QString mechabold = QFontDatabase::applicationFontFamilies(id).at(0);

    QPushButton *launch_button = new QPushButton(this);
    launch_button->setFixedSize(windowWidth/3, windowHeight/12);
    launch_button->setGeometry((windowWidth - launch_button->width())/2, windowHeight - (windowHeight/5), launch_button->width(), launch_button->height());
    launch_button->setText("Launch");
    launch_button->setFont(QFont(mechabold, launch_button->height()/2));
    launch_button->setStyleSheet("background-color: #4CAF50; color: white; border: 0px; border-radius: 5px;");

    QPushButton *add_button = new QPushButton(this);
    add_button->setFixedSize(windowWidth/9 - 5, windowHeight/18);
    add_button->setGeometry(launch_button->geometry().left(), windowHeight - (windowHeight/5) - add_button->height() - 3, add_button->width(), add_button->height());
    add_button->setText("Add");
    add_button->setFont(QFont(mechabold, add_button->height()/2));
    add_button->setStyleSheet("background-color: #E7E7E7; color: #4CAF50; border: 2px solid #4CAF50;");

    QPushButton *remove_button = new QPushButton(this);
    remove_button->setFixedSize(windowWidth/9, windowHeight/18);
    remove_button->setGeometry((windowWidth - remove_button->width())/2, windowHeight - (windowHeight/5) - remove_button->height() - 3, remove_button->width(), remove_button->height());
    remove_button->setText("Remove");
    remove_button->setFont(QFont(mechabold, add_button->height()/2));
    remove_button->setStyleSheet("background-color: #E7E7E7; color: #A91409; border: 2px solid #A91409;");

    this->gec_button = new QPushButton(this);
    this->gec_button->setFixedSize(windowWidth/9 - 5, windowHeight/18);
    this->gec_button->setGeometry(launch_button->geometry().right() - this->gec_button->width(), windowHeight - (windowHeight/5) - this->gec_button->height() - 3, this->gec_button->width(), this->gec_button->height());
    this->gec_button->setText("GEC");
    this->gec_button->setFont(QFont(mechabold, this->gec_button->height()/2));
    this->gec_button->setStyleSheet("background-color: #A91409; color: white; border: 2px solid black;");

    this->selection_box = new QComboBox(this);
    this->selection_box->setFixedSize(windowWidth/3, windowHeight/18);
    this->selection_box->setFont(QFont(mechabold, this->selection_box->height()/2));
    this->selection_box->setEditable(true);
    QLineEdit *selection_edit = this->selection_box->lineEdit();
    selection_edit->setAlignment(Qt::AlignCenter);
    selection_edit->setReadOnly(true);
    this->selection_box->setGeometry((windowWidth - this->selection_box->width())/2, windowHeight - (windowHeight/5) - this->selection_box->height() - add_button->height() - 6, this->selection_box->width(), this->selection_box->height());

    this->selection_frame = new QFrame(this);
    selection_frame->setFixedSize(windowWidth/3 + 20, launch_button->geometry().bottom() - this->selection_box->geometry().top() + 20);
    selection_frame->setGeometry(this->selection_box->geometry().left() - 10, this->selection_box->geometry().top() - 10, selection_frame->width(), selection_frame->height());
    selection_frame->setStyleSheet("QFrame{background-color: rgba(0,0,0,90); border-radius: 5px;}");
    selection_frame->lower();

//    this->frame = new QFrame(this);
//    this->frame->setGeometry(0, 5, windowWidth, windowHeight/14);
//    this->frame->setStyleSheet("background-color: rgba(0,0,0,90);");

//    QLabel *label = new QLabel(this);
//    label->setText("PokeMMO Launcher");
//    label->setFont(QFont(mechabold, windowHeight/14 - 10));
//    label->adjustSize();
//    label->setStyleSheet("color: rgb(255, 255, 255);");
//    label->setGeometry((windowWidth-label->width())/2, 5, label->width(), windowHeight/14);

    QPushButton *folder_button = new QPushButton(this);
    folder_button->setGeometry(windowWidth-(3*(windowHeight/14))-5, 5, windowHeight/14, windowHeight/14);
    folder_button->setFlat(true);
    QPixmap folder_pixmap(":/res/folder_icon.png");
    QIcon folder_button_icon(folder_pixmap);
    folder_button->setIcon(folder_button_icon);
    folder_button->setIconSize(QSize(windowHeight/15, windowHeight/15));
    folder_button->setStyleSheet("QPushButton:pressed{background-color: rgba(0,0,0,0);}");

    QPushButton *background_button = new QPushButton(this);
    background_button->setGeometry(windowWidth-(2*(windowHeight/14))-5, 5, windowHeight/14, windowHeight/14);
    background_button->setFlat(true);
    QPixmap background_pixmap(":/res/background_icon.svg");
    QIcon background_button_icon(background_pixmap);
    background_button->setIcon(background_button_icon);
    background_button->setIconSize(QSize(windowHeight/15, windowHeight/15));
    background_button->setStyleSheet("QPushButton:pressed{background-color: rgba(0,0,0,0);}");

    QPushButton *close_button = new QPushButton(this);
    close_button->setGeometry(windowWidth-(windowHeight/14)-5, 5, windowHeight/14, windowHeight/14);
    close_button->setFlat(true);
    close_button->setFont(QFont(mechabold, windowHeight/14 - 10));
    close_button->setText("X");
    close_button->setStyleSheet("QPushButton:pressed{background-color: rgba(0,0,0,0);}");

    connect(launch_button, SIGNAL(clicked()), this, SLOT(on_launch_button_clicked()));
    connect(add_button, SIGNAL(clicked()), this, SLOT(on_add_button_clicked()));
    connect(remove_button, SIGNAL(clicked()), this, SLOT(on_remove_button_clicked()));
    connect(this->gec_button, SIGNAL(clicked()), this, SLOT(on_gec_button_clicked()));
    connect(folder_button, SIGNAL(clicked()), this, SLOT(on_folder_button_clicked()));
    connect(background_button, SIGNAL(clicked()), this, SLOT(on_background_button_clicked()));
    connect(close_button, SIGNAL(clicked()), this, SLOT(on_close_button_clicked()));
    connect(this->selection_box, SIGNAL(clicked()), this, SLOT(mouseReleaseEvent()));

    QDir().mkdir("files");
    LoadFiles();
    LoadConfig();
}

MainWindow::~MainWindow()
{
//    delete ui;
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
    this->selection_box->clear();
    QDir directory("files");
    QStringList files = directory.entryList(QStringList() << "*.cred", QDir::Files);
    foreach(QString filename, files) {
        this->selection_box->addItem(filename.remove(".cred"));
    }
}

bool MainWindow::LoadConfig()
{
    QFile file("files/config.txt");
    if(!file.open(QIODevice::ReadOnly)){
        QTextStream(stdout) << "Cannot open the file. \n" << file.errorString() << "\n";
        _pixmapBg.load(":/res/background.png");
        return false;
    }
    QTextStream stream(&file);
    QString path = stream.readLine();
    if (path != "")
        this->pokemmo_path = new QString(path);
    this->selection_box->setCurrentText(stream.readLine());
    if (stream.readLine().toInt())
    {
        this->isGEC = true;
        this->gec_button->setStyleSheet("background-color: #4CAF50; color: white; border: 2px solid black;");
    }
    QString bgd(stream.readLine());
    if (bgd != "")
    {
        this->background_image = new QString(bgd);
        _pixmapBg.load(QString("imgs/%1").arg(bgd));
    } else {
        this->background_image = new QString("");
        _pixmapBg.load(":/res/background.png");
    }

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
    stream << this->pokemmo_path->toLatin1() << "\n";
    stream << this->selection_box->currentText() << "\n";
    stream << this->isGEC << "\n";
    stream << this->background_image->toLatin1() << "\n";

    file.close();
    return true;
}

void MainWindow::on_launch_button_clicked()
{
    QString selected = this->selection_box->currentText();
    if (QFile::exists(this->pokemmo_path->toLatin1() + "/config/savedcredentials.properties")){
        QFile::remove(this->pokemmo_path->toLatin1() + "/config/savedcredentials.properties");
    }
    QFile::copy(QString("files/%1.cred").arg(selected), this->pokemmo_path->toLatin1() + "/config/savedcredentials.properties");

    QProcess::startDetached(this->pokemmo_path->toLatin1() + "/PokeMMO.exe");

    if (this->isGEC){
        QString GECpath = this->pokemmo_path->toLatin1() + "/data/mods/Gilans Encounter Counter/GEC";
        QProcess *child = new QProcess();
        child->setWorkingDirectory(GECpath);
        child->start(GECpath + "/GEC.exe");
    }

    saveConfig();
    QApplication::quit();
}

void MainWindow::on_add_button_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Select Credential File", this->pokemmo_path->toLatin1() + "/config");
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
    saveConfig();
}

void MainWindow::on_remove_button_clicked()
{
    QString selected = this->selection_box->currentText();
    if (QFile::exists(QString("files/%1.cred").arg(selected)))
    {
        QFile::remove(QString("files/%1.cred").arg(selected));
    }
    LoadFiles();
}

void MainWindow::on_gec_button_clicked()
{
    this->isGEC = !isGEC;

    if (this->isGEC)
        this->gec_button->setStyleSheet("background-color: #4CAF50; color: white; border: 2px solid black;");
    else
        this->gec_button->setStyleSheet("background-color: #A91409; color: white; border: 2px solid black;");
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    this->pressed = false;
    if (this->selection_frame->x() > event->position().x() || event->position().x() > (this->selection_frame->x() + this->selection_frame->width()) || (this->selection_frame->y() > event->position().y() || event->position().y() > (this->selection_frame->y() + this->selection_frame->height()))){
        this->pressed = true;
        mouseClick_X_Coordinate = event->position().x();
        mouseClick_Y_Coordinate = event->position().y();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    this->pressed = false;
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (this->pressed)
        move(event->globalPosition().x()-mouseClick_X_Coordinate,event->globalPosition().y()-mouseClick_Y_Coordinate);
}

void MainWindow::on_folder_button_clicked()
{
    QString file_path = QFileDialog::getExistingDirectory(this, "Select PokeMMO Folder", QDir::homePath());

    if (file_path != "")
        this->pokemmo_path = new QString(file_path);

    saveConfig();
}

void MainWindow::on_background_button_clicked()
{
    QString file_path = QFileDialog::getOpenFileName(this, "New Background Image", QDir::homePath() + "/Pictures");

    if (file_path != ""){
        QDir().mkdir("imgs");
        QString fileName = file_path.split('/').last();

        if (QFile::exists(QString("imgs/%1").arg(fileName))){
            QFile::remove(QString("imgs/%1").arg(fileName));
        }

        QFile::copy(file_path, QString("imgs/%1").arg(fileName));

        if (_pixmapBg.load(QString("imgs/%1").arg(fileName))){
            this->background_image = new QString(fileName);
        }

    } else {
        this->background_image = new QString("");
        _pixmapBg.load(":/res/background.png");
    }
    saveConfig();
}

void MainWindow::on_close_button_clicked()
{
//    saveConfig();
    QApplication::quit();
}
