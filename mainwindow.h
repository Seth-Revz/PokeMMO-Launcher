#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFrame>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QDir>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow(){};

private slots:
    void on_launch_button_clicked();
    void on_add_button_clicked();
    void on_remove_button_clicked();
    void on_counter_button_clicked();
    void on_settings_button_clicked();
    void on_close_button_clicked();

private:
    void SetupUI(int, int);
    void LoadCredFiles();
    void LoadBackground(int, int);
    void ReloadBackground();
    QPixmap _pixmapBg;
    QPoint current;
    QFrame *frame;
    QFrame *selection_frame;
    QComboBox *selection_box;
    QLabel *movie_label;
    QPushButton *launch_button;
    QPushButton *add_button;
    QPushButton *remove_button;
    QPushButton *counter_button;
    QLineEdit *selection_edit;
    QPushButton *settings_button;
    QPushButton *close_button;
    QString *background_image;
    QMovie *background_movie;

    void SaveSettings();
    void LoadSettings();

    QString pokemmo_path;
    QString counter_path;
    bool counter_bool = false;
    bool cycle_bool = false;
    QString active_login;
    QString active_background;


    bool pressed = false;
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    int mouseClick_X_Coordinate;
    int mouseClick_Y_Coordinate;
};
#endif // MAINWINDOW_H
