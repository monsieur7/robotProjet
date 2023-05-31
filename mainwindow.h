#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QLabel>
#include <QByteArray>
#include <QDebug>
#include <QDateTime>
#include <QLCDNumber>
#include <iostream>
#include "login.h"
#include "myrobot.h"
#include <QWebEngineView>
#include "cameramove.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionSe_connecter_triggered();

    void updateUI();




    void on_actionSe_d_connecter_triggered();
    void on_Top_pressed();

    void on_Top_released();

    void on_bottom_pressed();

    void on_bottom_released();

    void on_left_pressed();

    void on_left_released();

    void on_right_pressed();

    void on_right_released();
    void on_verticalSlider_sliderMoved(int position);

    void on_CAMERA_DOWN_pressed();

    void on_CAMERA_RIGHT_pressed();

    void on_CAMERA_LEFT_pressed();

    void on_CAMERA_UP_pressed();

    float movingAverage(float value);

private:
    Ui::MainWindow *ui;
    Login *login;
    MyRobot robot;
    QWebEngineView *webView;
    void showCamera(QUrl url);
    int _speed;
    long map(long x, long in_min, long in_max, long out_min, long out_max);
    int _speedWheelR;
    int _speedWheelL;
    int _odometryR;
    int _odometryL;
    long _oldTime;
    cameraMove * _cameraMove;
    float _movingAverage[10];

};

#endif // MAINWINDOW_H
