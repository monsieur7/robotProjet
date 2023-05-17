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

    void on_actionSe_d_connecter_triggered();

private:
    Ui::MainWindow *ui;
    Login *login;
    MyRobot robot;
};

#endif // MAINWINDOW_H