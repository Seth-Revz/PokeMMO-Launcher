#include "settingswindow.h"
#include <QDebug>

SettingsWindow::SettingsWindow(QWidget *parent)
    : QDialog{parent}
{
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);

    QRect parentGeo = this->parentWidget()->geometry();
    QSize parentSize = this->parentWidget()->size();

    frame_size = new QRect(parentGeo.left() + qRound(parentSize.width()/3.0), parentGeo.top() + parentSize.height()/4, parentSize.width()/3, parentSize.height()/4 * 2);
    setGeometry(*frame_size);

    LoadSettings();

    SetupUI();

}

void SettingsWindow::SetupUI(){

    QGridLayout *layout = new QGridLayout;

    settings_frame = new QFrame(this);
    settings_frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    settings_frame->setStyleSheet(".QFrame{background-color: #242424; border: 2px solid black; border-radius: 20px;}");
    layout->addWidget(settings_frame, 0, 0);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    int mecha_id = QFontDatabase::addApplicationFont(":/res/mechabold.ttf");
    QString mechabold = QFontDatabase::applicationFontFamilies(mecha_id).at(0);

    pokemmo_title_label = new QLabel(this);
    pokemmo_title_label->setGeometry(this->width() / 20, this->height() - (this->height()/8) * 6.7, this->width() / 3, this->height()/10);
    pokemmo_title_label->setText("PokeMMO Location: ");
    pokemmo_title_label->setFont(QFont(mechabold, this->parentWidget()->geometry().height()/60));
    pokemmo_title_label->setStyleSheet("color: white; font-weight: bold;");

    pokemmo_location_label = new QLabel(this);
    pokemmo_location_label->setGeometry(this->width() / 20, this->height() - (this->height()/8) * 6.3, this->width() / 2, this->height()/10);
    pokemmo_location_label->setText(pokemmo_path);
    pokemmo_location_label->setFont(QFont(mechabold, this->parentWidget()->geometry().height()/60));
    pokemmo_location_label->setStyleSheet("color: white;");
    pokemmo_location_label->setToolTip(pokemmo_path);
\
    pokemmo_button = new QPushButton(this);
    pokemmo_button->setFont(QFont(mechabold, this->parentWidget()->geometry().height()/60));
    pokemmo_button->setText("Change Location");
    pokemmo_button->setGeometry((this->width()) - this->width() / 2.5, this->height() - (this->height()/8) * 6.5, this->width() / 3, this->height()/10);
    pokemmo_button->setStyleSheet("background-color: #4a4d50; color: white; border: 0px; border-radius: 5px;");

    counter_title_label = new QLabel(this);
    counter_title_label->setGeometry(this->width() / 20, this->height() - (this->height()/8) * 5.2, this->width() / 3, this->height()/10);
    counter_title_label->setText("Counter Location: ");
    counter_title_label->setFont(QFont(mechabold, this->parentWidget()->geometry().height()/60));
    counter_title_label->setStyleSheet("color: white; font-weight: bold;");

    counter_location_label = new QLabel(this);
    counter_location_label->setGeometry(this->width() / 20, this->height() - (this->height()/8) * 4.8, this->width() / 2, this->height()/10);
    counter_location_label->setText(counter_path);
    counter_location_label->setFont(QFont(mechabold, this->parentWidget()->geometry().height()/60));
    counter_location_label->setStyleSheet("color: white;");
    counter_location_label->setToolTip(counter_path);

    counter_button = new QPushButton(this);
    counter_button->setFont(QFont(mechabold, this->parentWidget()->geometry().height()/60));
    counter_button->setText("Change Location");
    counter_button->setGeometry((this->width()) - this->width() / 2.5, this->height() - (this->height()/8) * 5, this->width() / 3, this->height()/10);
    counter_button->setStyleSheet("background-color: #4a4d50; color: white; border: 0px; border-radius: 5px;");

    cycle_button = new QPushButton(this);
    cycle_button->setFont(QFont(mechabold, this->parentWidget()->geometry().height()/50));
    cycle_button->setText("Toggle Cycles");
    cycle_button->setGeometry((this->width()/2) - this->width() / 2 / 2, this->height() - (this->height()/8) * 3.5, this->width() / 2, this->height()/10);
    if (cycle_bool){
        cycle_button->setStyleSheet("background-color: #4CAF50; color: white; border: 2px solid black;");
    }else{
        cycle_button->setStyleSheet("background-color: #A91409; color: white; border: 2px solid black;");
    }

    background_button = new QPushButton(this);
    background_button->setFont(QFont(mechabold, this->parentWidget()->geometry().height()/50));
    background_button->setText("Change Background");
    background_button->setGeometry((this->width()/2) - this->width() / 2 / 2, this->height() - (this->height()/8) * 2.5, this->width() / 2, this->height()/10);
    background_button->setStyleSheet("background-color: #4a4d50; color: white; border: 0px; border-radius: 5px;");

    save_button = new QPushButton(this);
    save_button->setFont(QFont(mechabold, this->parentWidget()->geometry().height()/30));
    save_button->setText("Save");
    save_button->setGeometry((this->width()/2) - this->width() / 3 / 2, this->height() - (this->height()/8) * 1.5, this->width() / 3, this->height()/10);
    save_button->setStyleSheet("background-color: #1f6aa5; color: white; border: 0px; border-radius: 5px;");

    close_button = new QPushButton(this);
    close_button->setFlat(true);
    close_button->setFont(QFont(mechabold, this->parentWidget()->geometry().height()/14 - 10));
    close_button->setText("X");
    close_button->setGeometry(this->width() - (this->geometry().height()/6), 0, this->geometry().width()/6, this->geometry().height()/6);
//    close_button->setStyleSheet("QPushButton:pressed{background-color: rgba(0,0,0,0);}");
    close_button->setStyleSheet("color: white; border: 0px;");

    connect(pokemmo_button, SIGNAL(clicked()), this, SLOT(on_pokemmo_button_clicked()));
    connect(counter_button, SIGNAL(clicked()), this, SLOT(on_counter_button_clicked()));
    connect(cycle_button, SIGNAL(clicked()), this, SLOT(on_cycle_button_clicked()));
    connect(background_button, SIGNAL(clicked()), this, SLOT(on_background_button_clicked()));
    connect(close_button, SIGNAL(clicked()), this, SLOT(on_close_button_clicked()));
    connect(save_button, SIGNAL(clicked()), this, SLOT(on_save_button_clicked()));
}

void SettingsWindow::SaveSettings()
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

void SettingsWindow::LoadSettings()
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

void SettingsWindow::mousePressEvent(QMouseEvent *event)
{
    pressed = true;
    mouseClick_X_Coordinate = event->position().x();
    mouseClick_Y_Coordinate = event->position().y();
}

void SettingsWindow::mouseReleaseEvent(QMouseEvent *event)
{
    (void)event;
    pressed = false;
}

void SettingsWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (pressed)
        move(event->globalPosition().x()-mouseClick_X_Coordinate,event->globalPosition().y()-mouseClick_Y_Coordinate);
}

void SettingsWindow::on_pokemmo_button_clicked()
{
    QString file_path = QFileDialog::getOpenFileName(this, "Select PokeMMO Application (.exe)", QDir::homePath());

    pokemmo_path = QString(file_path);
    pokemmo_location_label->setText(pokemmo_path);
    pokemmo_location_label->setToolTip(pokemmo_path);
}

void SettingsWindow::on_counter_button_clicked()
{
    QString file_path = QFileDialog::getOpenFileName(this, "Select Counter Application (.exe)", pokemmo_path.toLatin1());

    counter_path = QString(file_path);
    counter_location_label->setText(counter_path);
    counter_location_label->setToolTip(counter_path);
}

void SettingsWindow::on_cycle_button_clicked()
{
    cycle_bool = !cycle_bool;

    if (cycle_bool){
        cycle_button->setStyleSheet("background-color: #4CAF50; color: white; border: 2px solid black;");
    }else{
        cycle_button->setStyleSheet("background-color: #A91409; color: white; border: 2px solid black;");
    }
}

void SettingsWindow::on_background_button_clicked()
{
    QString file_path = QFileDialog::getOpenFileName(this, "New Background Image/GIF", QDir::homePath() + "/Pictures");

    if (file_path != ""){
        QString file_name = file_path.split('/').last();
        if (QFile::exists(QString("launcher-backgrounds/%1").arg(file_name))){
            QFile::copy(file_path, QString("launcher-backgrounds/temp_image"));
            QFile::remove(QString("launcher-backgrounds/%1").arg(file_name));
            QFile::rename("launcher-backgrounds/temp_image", "launcher-backgrounds/" + file_name);
        }else{
            QFile::copy(file_path, QString("launcher-backgrounds/%1").arg(file_name));
        }

        active_background = *new QString(file_name);

    } else {
        active_background = "";
    }
}

void SettingsWindow::on_save_button_clicked()
{
    SaveSettings();
    QDialog::close();
}

void SettingsWindow::on_close_button_clicked()
{
    QDialog::close();
}
