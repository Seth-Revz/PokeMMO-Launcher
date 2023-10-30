#include "mainwindow.h"
#include "settingswindow.h"
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
#include <QFileInfo>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::FramelessWindowHint);
    QDir().mkdir("files");
    QDir().mkdir("launcher-backgrounds");

    QRect screen = QApplication::primaryScreen()->geometry();
    int window_width = screen.width()/2;
    int window_height = screen.height()/2;
    setFixedSize(window_width, window_height);

    LoadSettings();
    SetupUI(window_width, window_height);
    _pixmapBg.load(":/res/background.png");

    LoadCredFiles();
    LoadBackground(window_width, window_height);
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
    if (counter_bool){
        counter_button->setStyleSheet("background-color: #4CAF50; color: white; border: 2px solid black;");
    }else{
        counter_button->setStyleSheet("background-color: #A91409; color: white; border: 2px solid black;");
    }

    selection_box = new QComboBox(this);
    selection_box->setFixedSize(windowWidth/3, windowHeight/18);
    selection_box->setFont(QFont(mechabold, selection_box->height()/2));
    selection_box->setEditable(true);
    selection_edit = selection_box->lineEdit();
    selection_edit->setAlignment(Qt::AlignCenter);
    selection_edit->setReadOnly(true);
    selection_box->setGeometry((windowWidth - selection_box->width())/2, windowHeight - (windowHeight/5) - selection_box->height() - add_button->height() - 6, selection_box->width(), selection_box->height());
    selection_box->setCurrentText(active_login);

    selection_frame = new QFrame(this);
    selection_frame->setFixedSize(windowWidth/3 + 20, launch_button->geometry().bottom() - selection_box->geometry().top() + 20);
    selection_frame->setGeometry(selection_box->geometry().left() - 10, selection_box->geometry().top() - 10, selection_frame->width(), selection_frame->height());
    selection_frame->setStyleSheet("QFrame{background-color: rgba(0,0,0,90); border-radius: 5px;}");
    selection_frame->lower();
    // Put movie back on bottom
    movie_label->lower();

    QLabel *black = new QLabel(this);
    black->setGeometry(windowWidth-(2*(windowHeight/12.5)), 0, windowHeight/6, windowHeight/11);
    black->setStyleSheet("background-color: black; border: 0px; border-radius: 5px;");

    settings_button = new QPushButton(this);
    settings_button->setGeometry(windowWidth-(2*(windowHeight/14)) - 5, 5, windowHeight/14, windowHeight/14);
    settings_button->setFlat(true);
    QPixmap pokemmo_folder_pixmap(":/res/settings_icon.png");
    QIcon pokemmo_folder_icon(pokemmo_folder_pixmap);
    settings_button->setIcon(pokemmo_folder_icon);
    settings_button->setIconSize(QSize(windowHeight/15, windowHeight/15));
    settings_button->setStyleSheet("QPushButton:pressed{background-color: rgba(0,0,0,0);}");

    close_button = new QPushButton(this);
    close_button->setGeometry(windowWidth-(windowHeight/14) - 5, 5, windowHeight/14, windowHeight/14);
    close_button->setFlat(true);
    close_button->setFont(QFont(mechabold, windowHeight/14 - 10));
    close_button->setText("X");
    close_button->setStyleSheet("color: white; border: 0px;");
//    close_button->setStyleSheet("QPushButton:pressed{background-color: rgba(0,0,0,0);}");

    connect(launch_button, SIGNAL(clicked()), this, SLOT(on_launch_button_clicked()));
    connect(add_button, SIGNAL(clicked()), this, SLOT(on_add_button_clicked()));
    connect(remove_button, SIGNAL(clicked()), this, SLOT(on_remove_button_clicked()));
    connect(counter_button, SIGNAL(clicked()), this, SLOT(on_counter_button_clicked()));
    connect(settings_button, SIGNAL(clicked()), this, SLOT(on_settings_button_clicked()));
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
    int index = selection_box->findText(active_login);
    if (index != -1){
        selection_box->setCurrentIndex(index);
    }
}

void MainWindow::SaveSettings()
{
    QSettings settings("Revz", "PokeMMO-Launcher");
    settings.beginGroup("MainSettings");
    settings.setValue("PokeMMO-Application", pokemmo_path);
    settings.setValue("Counter-Application", counter_path);
    settings.setValue("Counter-Bool", counter_bool);
    settings.setValue("Cycle-Bool", cycle_bool);
    settings.setValue("Active_Login", active_login);
    settings.setValue("Active-Background", active_background);
    settings.endGroup();
}

void MainWindow::LoadSettings()
{
    QSettings settings("Revz", "PokeMMO-Launcher");
    settings.beginGroup("MainSettings");
    pokemmo_path = settings.value("PokeMMO-Application", "").toString();
    counter_path = settings.value("Counter-Application", "").toString();
    counter_bool = settings.value("Counter-Bool", false).toBool();
    cycle_bool = settings.value("Cycle-Bool", false).toBool();
    active_login = settings.value("Active_Login", "").toString();
    active_background = settings.value("Active-Background", "").toString();
    settings.endGroup();
}

void MainWindow::LoadBackground(int windowWidth, int windowHeight){
    QFileInfo check_file("launcher-backgrounds/" + active_background);
    if(!active_background.isEmpty() && check_file.exists()){
        background_movie = new QMovie("launcher-backgrounds/" + active_background);
    } else {
        background_movie = new QMovie(":/res/background.png");
    }
    background_movie->setScaledSize({windowWidth, windowHeight});
    background_movie->start();
    movie_label->setAttribute(Qt::WA_NoSystemBackground);
    movie_label->setMovie(background_movie);
}

void MainWindow::ReloadBackground(){
    if (active_background != ""){
        background_movie->stop();
        background_movie->setFileName("launcher-backgrounds/" + active_background);
        background_movie->start();
    } else {
        background_movie->stop();
        background_movie->setFileName(":/res/background.png");
        background_movie->start();
    }
}

void MainWindow::on_launch_button_clicked()
{
    QString selected = selection_box->currentText();
    QString pokemmo_folder = QFileInfo(pokemmo_path).absolutePath().toLatin1();
    active_login = QString(selected);

    if (QFile::exists(pokemmo_folder + "/config/savedcredentials.properties")){
        QFile::remove(pokemmo_folder + "/config/savedcredentials.properties");
    }
    QFile::copy(QString("files/%1.cred").arg(selected), pokemmo_folder + "/config/savedcredentials.properties");

    QProcess::startDetached(pokemmo_folder + "/PokeMMO.exe");

    if (counter_bool){
        QProcess *child = new QProcess();
        QString file_name = counter_path.split('/').last();
        child->setWorkingDirectory(counter_path.toLatin1().left(counter_path.size() - file_name.size()));
        child->start(counter_path.toLatin1());
    }

    SaveSettings();
    QApplication::quit();
}

void MainWindow::on_add_button_clicked()
{
    QString fileName = QFileInfo(pokemmo_path).absolutePath().toLatin1() + "/config/savedcredentials.properties";
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
    SaveSettings();
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
    counter_bool = !counter_bool;

    if (counter_bool){
        if(counter_path.toLatin1() == ""){
            QMessageBox mb;
            mb.warning(this, "Warning: No Counter Program Set", "There is no counter program set.\nPlease select your counter's executable (.exe) in the next window.", QMessageBox::Ok);
            QString file_path = QFileDialog::getOpenFileName(this, "Counter Program", pokemmo_path.toLatin1());
            if(file_path != ""){
                counter_path = *new QString(file_path);
            }else{
                counter_bool = !counter_bool;
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

void MainWindow::on_settings_button_clicked()
{
    SettingsWindow settings(this);
    settings.setModal(true);
    settings.exec();

    LoadSettings();
    ReloadBackground();
}

void MainWindow::on_close_button_clicked()
{
    SaveSettings();
    QApplication::quit();
}
