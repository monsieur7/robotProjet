#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent, MyRobot * robot) :
    QDialog(parent),
    ui(new Ui::Login),
    _robot(robot)
{
    ui->setupUi(this);

}

Login::~Login()
{
    delete ui;
}

void Login::on_connect_clicked()
{
  //  this->ui->connect->setStyleSheet(""); // see for timer !

    QString ip = ui->ip->text();
    QString port = ui->port->text();
    std::cout << "ip " << ip.toStdString() << " " << port.toInt() << std::endl;
    _robot->setPort(port);
    _robot->setIpAddress(ip);
    bool res = _robot->doConnect();
    if(res == false){
        std::cout << "could not connect" << std::endl;
        // put button in red
        this->ui->connect->setStyleSheet("background-color: red;");
    }
    else {
        // quit window
        finished(1);
    }
}
