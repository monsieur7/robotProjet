#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include "login.h"
#include "myrobot.h"

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


    void on_left_clicked();

    void on_right_clicked();

<<<<<<< HEAD
    void on_actionSe_d_connecter_triggered();
=======
    void on_Top_pressed();

    void on_Top_released();

    void on_bottom_pressed();

    void on_bottom_released();

    void on_left_pressed();

    void on_left_released();

    void on_right_pressed();

    void on_right_released();
>>>>>>> a98ef2bc1131fcc0fc55fef2a2b50cba72d93790

private:
    Ui::MainWindow *ui;
    Login *login;
    MyRobot robot;
};

#endif // MAINWINDOW_H
