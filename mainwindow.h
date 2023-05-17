#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QLabel>
#include <QByteArray>


#include <iostream>
#include "login.h"
#include "myrobot.h"
#include <QWebEngineView>
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

private:
    Ui::MainWindow *ui;
    Login *login;
    MyRobot robot;
    QWebEngineView *webView;
    void showCamera(QUrl url);
    int _speed;

};

#endif // MAINWINDOW_H
