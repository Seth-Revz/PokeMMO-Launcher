#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFrame>
#include <QComboBox>
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
    ~MainWindow();
    void paintEvent(QPaintEvent *pe);
    void LoadFiles();
    bool LoadConfig();
    bool saveConfig();
    QPixmap _pixmapBg;
    bool pressed;
    QPoint current;
    QFrame *frame;
    QFrame *selection_frame;
    QComboBox *selection_box;
    QPushButton *gec_button;
    QString *background_image;
    QString *pokemmo_path = new QString(QDir::homePath() + "/AppData/Local/Programs/PokeMMO");
    bool isGEC = false;

private slots:
    void on_launch_button_clicked();
    void on_add_button_clicked();
    void on_remove_button_clicked();
    void on_gec_button_clicked();
    void on_folder_button_clicked();
    void on_background_button_clicked();
    void on_close_button_clicked();

private:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    int mouseClick_X_Coordinate;
    int mouseClick_Y_Coordinate;
};
#endif // MAINWINDOW_H
