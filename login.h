#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <iostream>
#include "myrobot.h"
namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr, MyRobot * robot = nullptr);
    ~Login();

private slots:
    void on_connect_clicked();

private:
    Ui::Login *ui;
    MyRobot * _robot;
};

#endif // LOGIN_H
