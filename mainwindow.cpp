#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->Top, &QPushButton::clicked, this, &MainWindow::on_Top_clicked);
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

void MainWindow::on_Top_clicked()
{
    std::cout << "going top " << std::endl;

    robot.sendMovement(120, 120);
}
