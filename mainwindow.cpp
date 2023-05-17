#include "mainwindow.h"
#include "ui_mainwindow.h"

//QMediaPlayer
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{



    ui->setupUi(this);

    webView = new QWebEngineView();

    // Load a webpage
    webView->load(QUrl("http://192.168.1.106:8080/?action=stream"));
    ui->gridLayout_2->addWidget(webView);
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

     int speedR, speedL, odometryL, odometryR, BatLevelR, IR1, IR2, CurrentL, CurrentR, VersionR, VersionL, dataL,
        SpeedFront, dataR, IL, IL2;
    unsigned int BatLevelL;



      speedL=(int)((robot.DataReceived.data()[1] << 8) + robot.DataReceived.data()[0]);
      if (speedL > 32767) speedL=dataL>SpeedFront-65536;

      BatLevelL=robot.DataReceived.data()[2];   IL=robot.DataReceived.data()[3];   IL2=robot.DataReceived.data()[4];
      odometryL=((((long)robot.DataReceived.data()[8] << 24))+(((long)robot.DataReceived.data()[7] << 16))+(((long)robot.DataReceived.data()[6] << 8))+((long)robot.DataReceived.data()[5]));

      speedR=(int)(robot.DataReceived.data()[10] << 8) + robot.DataReceived.data()[9];
      if (speedR > 32767) speedR=dataR>SpeedFront-65536;
      BatLevelR=0;
     IR1=robot.DataReceived.data()[11];
      IR2=robot.DataReceived.data()[12];
     odometryR=((((long)robot.DataReceived.data()[16] << 24))+(((long)robot.DataReceived.data()[15] << 16))+(((long)robot.DataReceived.data()[14] << 8))+((long)robot.DataReceived.data()[13]));
    CurrentL=robot.DataReceived.data()[17];
    CurrentR=robot.DataReceived.data()[17];
    VersionL=robot.DataReceived.data()[18];
    VersionR=robot.DataReceived.data()[18];
    std::cout << "speedR: " << speedR << " \n speedL: " << speedL << " \n odometryL: " << odometryL << " \n odometryR: " << odometryR << "\n BatLevelL:  " << BatLevelL << " \n BatLevelR: " << BatLevelR
    << " \n IR1: " << IR1 << " \n IR2: " << IR2 << " \n CurrentL:" << CurrentL << " \n CurrentR: " << CurrentR << "\nVersionR: " << VersionR << "\nVersionL: " << VersionL << "\ndataL: " << dataL
              << "\nSpeedFront: " << SpeedFront << "\ndataR: " << dataR << "\nIL: " << IL << "\nIL2: " << IL2;
}




void MainWindow::on_actionSe_d_connecter_triggered()
{
    robot.disConnect();
}

void MainWindow::on_Top_pressed()
{
    std::cout << "going top " << std::endl;

    robot.sendMovement(120,120);
}




void MainWindow::on_Top_released()
{
    std::cout << "stop going top " << std::endl;

    robot.sendMovement(0,0);
}




void MainWindow::on_bottom_pressed()
{
    std::cout << "going bottom " << std::endl;

    robot.sendMovement(-120,-120);

}




void MainWindow::on_bottom_released()
{
    std::cout << "stop going bottom " << std::endl;

    robot.sendMovement(0,0);
}





void MainWindow::on_left_pressed()
{
    std::cout << "going left " << std::endl;

    robot.sendMovement(0,80);
}


void MainWindow::on_left_released()
{
    std::cout << "stop going left " << std::endl;

    robot.sendMovement(0,0);
}


void MainWindow::on_right_pressed()
{
    std::cout << "going right " << std::endl;

    robot.sendMovement(80,0);
}

void MainWindow::on_right_released()
{
    std::cout << "stop going right " << std::endl;

    robot.sendMovement(0,0);
}

