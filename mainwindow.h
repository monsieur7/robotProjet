#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaContent>
#include <QtNetwork>
#include <QLabel>
#include <QPixmap>
#include <QByteArray>


#include <iostream>
#include "login.h"
#include "myrobot.h"
#include <QVideoWidget>
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
    void networkReplyReadyRead();

private:
    Ui::MainWindow *ui;
    Login *login;
    MyRobot robot;
    QNetworkAccessManager * networkManager;
    QNetworkReply * reply;
    QByteArray imageDataBuffer;
};

#endif // MAINWINDOW_H
