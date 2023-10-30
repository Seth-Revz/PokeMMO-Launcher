#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>
#include <QSettings>
#include <QMouseEvent>
#include <QPushButton>
#include <QFontDatabase>
#include <QGridLayout>
#include <QFrame>
#include <QLabel>
#include <QFileDialog>


class SettingsWindow : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsWindow(QWidget *parent = nullptr);

private:
    void SetupUI();

    QString pokemmo_path;
    QString counter_path;
    bool counter_bool;
    bool cycle_bool;
    QString active_login;
    QString active_background;

    QRect *frame_size;
    QFrame *settings_frame;

    QLabel *pokemmo_title_label;
    QLabel *pokemmo_location_label;
    QLabel *counter_title_label;
    QLabel *counter_location_label;

    QPushButton *pokemmo_button;
    QPushButton *counter_button;
    QPushButton *cycle_button;
    QPushButton *background_button;
    QPushButton *save_button;
    QPushButton *close_button;

    void SaveSettings();
    void LoadSettings();

    bool pressed = false;
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    int mouseClick_X_Coordinate;
    int mouseClick_Y_Coordinate;

private slots:
    void on_pokemmo_button_clicked();
    void on_counter_button_clicked();
    void on_cycle_button_clicked();
    void on_background_button_clicked();
    void on_save_button_clicked();
    void on_close_button_clicked();


signals:

};

#endif // SETTINGSWINDOW_H
