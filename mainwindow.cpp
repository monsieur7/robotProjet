#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->Top, &QPushButton::clicked, this, &MainWindow::on_Top_clicked);
   //QObject::connect(ui->, &QPushButton::clicked, this, &MainWindow::on_bottom_clicked);
   QObject::connect(&robot, &MyRobot::updateUI, this, &MainWindow::updateUI);

    login = new Login(nullptr, &robot);
}
MainWindow::~MainWindow()
{
    delete ui;
    robot.disConnect();
}

void MainWindow::on_actionSe_connecter_triggered()
{
    std::cout << "Menu clicked" << std::endl;
    login->show();

}


void MainWindow::updateUI(){
  //  robot.DataReceived;

}
