#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFrame>

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


private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_close_button_clicked();

private:
    Ui::MainWindow *ui;

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    int mouseClick_X_Coordinate;
    int mouseClick_Y_Coordinate;
};
#endif // MAINWINDOW_H
