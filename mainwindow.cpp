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
#include <QMovie>
#include <QMouseEvent>
#include <QComboBox>
#include <QFontDatabase>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::FramelessWindowHint);

    QRect screen = QApplication::primaryScreen()->geometry();
    int windowWidth = screen.width()/2;
    int windowHeight = screen.height()/2;
    setFixedSize(windowWidth, windowHeight);

    SetupUI(windowWidth, windowHeight);

    QDir().mkdir("files");
    LoadCredFiles();
    LoadConfig();
    LoadBackground(windowWidth, windowHeight);
}

void MainWindow::SetupUI(int windowWidth, int windowHeight){
    int mecha_id = QFontDatabase::addApplicationFont(":/res/mechabold.ttf");
    QString mechabold = QFontDatabase::applicationFontFamilies(mecha_id).at(0);

    movie_label = new QLabel(this);
    movie_label->setGeometry(0, 0, windowWidth, windowHeight);

    launch_button = new QPushButton(this);
    launch_button->setFixedSize(windowWidth/3, windowHeight/12);
    launch_button->setGeometry((windowWidth - launch_button->width())/2, windowHeight - (windowHeight/5), launch_button->width(), launch_button->height());
    launch_button->setText("Launch");
    launch_button->setFont(QFont(mechabold, launch_button->height()/2));
    launch_button->setStyleSheet("background-color: #4CAF50; color: white; border: 0px; border-radius: 5px;");

    add_button = new QPushButton(this);
    add_button->setFixedSize(windowWidth/9 - 5, windowHeight/18);
    add_button->setGeometry(launch_button->geometry().left(), windowHeight - (windowHeight/5) - add_button->height() - 3, add_button->width(), add_button->height());
    add_button->setText("Add");
    add_button->setFont(QFont(mechabold, add_button->height()/2));
    add_button->setStyleSheet("background-color: #E7E7E7; color: #4CAF50; border: 2px solid #4CAF50;");

    remove_button = new QPushButton(this);
    remove_button->setFixedSize(windowWidth/9, windowHeight/18);
    remove_button->setGeometry((windowWidth - remove_button->width())/2, windowHeight - (windowHeight/5) - remove_button->height() - 3, remove_button->width(), remove_button->height());
    remove_button->setText("Remove");
    remove_button->setFont(QFont(mechabold, add_button->height()/2));
    remove_button->setStyleSheet("background-color: #E7E7E7; color: #A91409; border: 2px solid #A91409;");

    counter_button = new QPushButton(this);
    counter_button->setFixedSize(windowWidth/9 - 5, windowHeight/18);
    counter_button->setGeometry(launch_button->geometry().right() - counter_button->width(), windowHeight - (windowHeight/5) - counter_button->height() - 3, counter_button->width(), counter_button->height());
    counter_button->setText("Counter");
    counter_button->setFont(QFont(mechabold, counter_button->height()/2));
    counter_button->setStyleSheet("background-color: #A91409; color: white; border: 2px solid black;");

    selection_box = new QComboBox(this);
    selection_box->setFixedSize(windowWidth/3, windowHeight/18);
    selection_box->setFont(QFont(mechabold, selection_box->height()/2));
    selection_box->setEditable(true);
    selection_edit = selection_box->lineEdit();
    selection_edit->setAlignment(Qt::AlignCenter);
    selection_edit->setReadOnly(true);
    selection_box->setGeometry((windowWidth - selection_box->width())/2, windowHeight - (windowHeight/5) - selection_box->height() - add_button->height() - 6, selection_box->width(), selection_box->height());

    selection_frame = new QFrame(this);
    selection_frame->setFixedSize(windowWidth/3 + 20, launch_button->geometry().bottom() - selection_box->geometry().top() + 20);
    selection_frame->setGeometry(selection_box->geometry().left() - 10, selection_box->geometry().top() - 10, selection_frame->width(), selection_frame->height());
    selection_frame->setStyleSheet("QFrame{background-color: rgba(0,0,0,90); border-radius: 5px;}");
    selection_frame->lower();
    // Put movie back on bottom
    movie_label->lower();

    counter_folder_button = new QPushButton(this);
    counter_folder_button->setGeometry(windowWidth-(4*(windowHeight/14)) - 5, 5, windowHeight/14, windowHeight/14);
    counter_folder_button->setFlat(true);
    QPixmap counter_folder_pixmap(":/res/counter_icon.png");
    QIcon counter_folder_icon(counter_folder_pixmap);
    counter_folder_button->setIcon(counter_folder_icon);
    counter_folder_button->setIconSize(QSize(windowHeight/15, windowHeight/15));
    counter_folder_button->setStyleSheet("QPushButton:pressed{background-color: rgba(0,0,0,0);}");

    pokemmo_folder_button = new QPushButton(this);
    pokemmo_folder_button->setGeometry(windowWidth-(3*(windowHeight/14)) - 5, 5, windowHeight/14, windowHeight/14);
    pokemmo_folder_button->setFlat(true);
    QPixmap pokemmo_folder_pixmap(":/res/pokemmo_icon.png");
    QIcon pokemmo_folder_icon(pokemmo_folder_pixmap);
    pokemmo_folder_button->setIcon(pokemmo_folder_icon);
    pokemmo_folder_button->setIconSize(QSize(windowHeight/15, windowHeight/15));
    pokemmo_folder_button->setStyleSheet("QPushButton:pressed{background-color: rgba(0,0,0,0);}");

    background_button = new QPushButton(this);
    background_button->setGeometry(windowWidth-(2*(windowHeight/14)) - 5, 5, windowHeight/14, windowHeight/14);
    background_button->setFlat(true);
    QPixmap background_pixmap(":/res/background_icon.svg");
    QIcon background_button_icon(background_pixmap);
    background_button->setIcon(background_button_icon);
    background_button->setIconSize(QSize(windowHeight/15, windowHeight/15));
    background_button->setStyleSheet("QPushButton:pressed{background-color: rgba(0,0,0,0);}");

    close_button = new QPushButton(this);
    close_button->setGeometry(windowWidth-(windowHeight/14) - 5, 5, windowHeight/14, windowHeight/14);
    close_button->setFlat(true);
    close_button->setFont(QFont(mechabold, windowHeight/14 - 10));
    close_button->setText("X");
    close_button->setStyleSheet("QPushButton:pressed{background-color: rgba(0,0,0,0);}");

    connect(launch_button, SIGNAL(clicked()), this, SLOT(on_launch_button_clicked()));
    connect(add_button, SIGNAL(clicked()), this, SLOT(on_add_button_clicked()));
    connect(remove_button, SIGNAL(clicked()), this, SLOT(on_remove_button_clicked()));
    connect(counter_button, SIGNAL(clicked()), this, SLOT(on_counter_button_clicked()));
    connect(counter_folder_button, SIGNAL(clicked()), this, SLOT(on_counter_folder_button_clicked()));
    connect(pokemmo_folder_button, SIGNAL(clicked()), this, SLOT(on_pokemmo_folder_button_clicked()));
    connect(background_button, SIGNAL(clicked()), this, SLOT(on_background_button_clicked()));
    connect(close_button, SIGNAL(clicked()), this, SLOT(on_close_button_clicked()));
}

void MainWindow::LoadCredFiles()
{
    selection_box->clear();
    QDir directory("files");
    QStringList files = directory.entryList(QStringList() << "*.cred", QDir::Files);
    foreach(QString filename, files) {
        selection_box->addItem(filename.remove(".cred"));
    }
}

bool MainWindow::LoadConfig()
{
    QFile file("files/config.txt");
    if(!file.open(QIODevice::ReadOnly)){
        QTextStream(stdout) << "Cannot open the file. \n" << file.errorString() << "\n";
        _pixmapBg.load(":/res/background.png");

        if (QFile::exists(QDir::rootPath() + "/Program Files/PokeMMO/PokeMMO.exe")){
            pokemmo_path = new QString(QDir::rootPath() + "/Program Files/PokeMMO/PokeMMO.exe");
        }else if(QFile::exists(QDir::homePath() + "/AppData/Local/Programs/PokeMMO/PokeMMO.exe")){
            pokemmo_path = new QString(QDir::homePath() + "/AppData/Local/Programs/PokeMMO");
        }else{
            pokemmo_path = new QString("");
            QMessageBox::warning(this, "Warning: Set PokeMMO Directory", "Could not find PokeMMO installation.\nPlease use the folder icon to set PokeMMO's path.", QMessageBox::Ok);
        }

        counter_path = new QString("");
        background_image = new QString("");

        return false;
    }
    QTextStream stream(&file);
    QString path = stream.readLine();
    if (path != ""){
        pokemmo_path = new QString(path);
    }else{
        if (QFile::exists(QDir::rootPath() + "/Program Files/PokeMMO/PokeMMO.exe")){
            pokemmo_path = new QString(QDir::rootPath() + "/Program Files/PokeMMO");
        }else if(QFile::exists(QDir::homePath() + "/AppData/Local/Programs/PokeMMO/PokeMMO.exe")){
            pokemmo_path = new QString(QDir::homePath() + "/AppData/Local/Programs/PokeMMO");
        }
    }

    counter_path = new QString(stream.readLine());

    selection_box->setCurrentText(stream.readLine());
    if (stream.readLine().toInt())
    {
        counter = true;
        counter_button->setStyleSheet("background-color: #4CAF50; color: white; border: 2px solid black;");
    }
    QString bgd(stream.readLine());
    if (bgd != "")
    {
        background_image = new QString(bgd);
    } else {
        background_image = new QString("");
    }

    file.close();
    return true;
}

bool MainWindow::saveConfig()
{
    QFile file("files/config.txt");
    if(!file.open(QIODevice::WriteOnly)){
        QTextStream(stdout) << "Cannot open the file. \n" << file.errorString() << "\n";
        return false;
    }

    QTextStream stream(&file);
    stream << pokemmo_path->toLatin1() << "\n";
    stream << counter_path->toLatin1() << "\n";
    stream << selection_box->currentText() << "\n";
    stream << counter << "\n";
    stream << background_image->toLatin1() << "\n";

    file.close();
    return true;
}

void MainWindow::LoadBackground(int windowWidth, int windowHeight){
    if(!background_image->isEmpty()){
        background_movie = new QMovie("launcher-backgrounds/" + *background_image);
    } else {
        background_movie = new QMovie(":/res/background.png");
    }
    background_movie->setScaledSize({windowWidth, windowHeight});
    background_movie->start();
    movie_label->setAttribute(Qt::WA_NoSystemBackground);
    movie_label->setMovie(background_movie);
}

void MainWindow::on_launch_button_clicked()
{
    QString selected = selection_box->currentText();
    if (QFile::exists(pokemmo_path->toLatin1() + "/config/savedcredentials.properties")){
        QFile::remove(pokemmo_path->toLatin1() + "/config/savedcredentials.properties");
    }
    QFile::copy(QString("files/%1.cred").arg(selected), pokemmo_path->toLatin1() + "/config/savedcredentials.properties");

    QProcess::startDetached(pokemmo_path->toLatin1() + "/PokeMMO.exe");

    if (counter){
        QProcess *child = new QProcess();
        QString file_name = counter_path->split('/').last();
        child->setWorkingDirectory(counter_path->toLatin1().left(counter_path->size() - file_name.size()));
        child->start(counter_path->toLatin1());
    }

    saveConfig();
    QApplication::quit();
}

void MainWindow::on_add_button_clicked()
{
    QString fileName = pokemmo_path->toLatin1() + "/config/savedcredentials.properties";
    if (QFile::exists(fileName)){
        bool ok;
        QString text = QInputDialog::getText(this, tr("Save as"),
                                             tr("Save current login as:"), QLineEdit::Normal,
                                             "", &ok);
        if (ok && !text.isEmpty()){
            if (QFile::exists(QString("files/%1.cred").arg(text)))
            {
                QFile::remove(QString("files/%1.cred").arg(text));
            }
            QFile::copy(fileName, QString("files/%1.cred").arg(text));
        }
        LoadCredFiles();
    } else {
        QMessageBox::critical(this, "No savedcredentials.properties file", "No savedcredentials.properties file was found. Log into PokeMMO first.");
    }
    saveConfig();
}

void MainWindow::on_remove_button_clicked()
{
    QString selected = selection_box->currentText();
    if (selected == ""){
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Are you sure?", "Are you sure you want to remove " + selected + "?");
    if (reply == QMessageBox::Yes){
        if (QFile::exists(QString("files/%1.cred").arg(selected)))
        {
            QFile::remove(QString("files/%1.cred").arg(selected));
        }
        LoadCredFiles();
    }
}

void MainWindow::on_counter_button_clicked()
{
    counter = !counter;

    if (counter){
        if(counter_path->toLatin1() == ""){
            QMessageBox mb;
            mb.warning(this, "Warning: No Counter Program Set", "There is no counter program set.\nPlease select your counter's executable (.exe) in the next window.", QMessageBox::Ok);
            QString file_path = QFileDialog::getOpenFileName(this, "Counter Program", pokemmo_path->toLatin1());
            if(file_path != ""){
                counter_path = new QString(file_path);
            }else{
                counter = !counter;
                return;
            }
        }
        counter_button->setStyleSheet("background-color: #4CAF50; color: white; border: 2px solid black;");
    }else{
        counter_button->setStyleSheet("background-color: #A91409; color: white; border: 2px solid black;");
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    pressed = false;
    if (selection_frame->x() > event->position().x() || event->position().x() > (selection_frame->x() + selection_frame->width()) || (selection_frame->y() > event->position().y() || event->position().y() > (selection_frame->y() + selection_frame->height()))){
        pressed = true;
        mouseClick_X_Coordinate = event->position().x();
        mouseClick_Y_Coordinate = event->position().y();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    (void)event;
    pressed = false;
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (pressed)
        move(event->globalPosition().x()-mouseClick_X_Coordinate,event->globalPosition().y()-mouseClick_Y_Coordinate);
}

void MainWindow::on_counter_folder_button_clicked()
{
    QString file_path = QFileDialog::getOpenFileName(this, "Select the .exe of your Counter Program", pokemmo_path->toLatin1());

    if (file_path != "")
        counter_path = new QString(file_path);

    saveConfig();
}

void MainWindow::on_pokemmo_folder_button_clicked()
{
    QString file_path = QFileDialog::getExistingDirectory(this, "Select PokeMMO FOLDER", QDir::homePath());

    if (file_path != "")
        pokemmo_path = new QString(file_path);

    saveConfig();
}

void MainWindow::on_background_button_clicked()
{
    QString file_path = QFileDialog::getOpenFileName(this, "New Background Image/GIF", QDir::homePath() + "/Pictures");

    if (file_path != ""){
        QDir().mkdir("launcher-backgrounds");
        QString file_name = file_path.split('/').last();

        if (QFile::exists(QString("launcher-backgrounds/%1").arg(file_name))){
            QFile::copy(file_path, QString("launcher-backgrounds/temp_image"));
            QFile::remove(QString("launcher-backgrounds/%1").arg(file_name));
            QFile::rename("launcher-backgrounds/temp_image", "launcher-backgrounds/" + file_name);
        }else{
            QFile::copy(file_path, QString("launcher-backgrounds/%1").arg(file_name));
        }

        background_image = new QString(file_name);
        background_movie->stop();
        background_movie->setFileName("launcher-backgrounds/" + *background_image);
        background_movie->start();

    } else {
        background_image = new QString("");

        background_movie->stop();
        background_movie->setFileName(":/res/background.png");
        background_movie->start();
    }
    saveConfig();
}

void MainWindow::on_close_button_clicked()
{
    saveConfig();
    QApplication::quit();
}
