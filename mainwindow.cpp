#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWebEngineView>
//QMediaPlayer
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{



    ui->setupUi(this);
    this->showCamera(QUrl("http://192.168.1.106:8080/?action=stream"));
    login = new Login(nullptr, &robot);
    connect(&robot, &MyRobot::updateUI, this, &MainWindow::updateUI);
    _speedWheelL = 0;
    _speedWheelR = 0;
    _odometryL = 0;
    _odometryR = 0;
    _oldTime = 0;
    ui->batterie->setValue(0); // init batterie level;
    ui->BAS_DROIT->setValue(0); // init IR progress bar
    ui->BAS_GAUCHE->setValue(0);
    ui->HAUT_DROITE->setValue(0);
    ui->HAUT_GAUCHE->setValue(0);
    _cameraMove = new cameraMove(); // init camera movement
    for(int i = 0; i < 10; i++){
        _movingAverage[i] = 0;
    }

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
// code taken from docs
    int speedR, speedL, odometryL, odometryR, BatLevelR,CurrentL, CurrentR, VersionR, VersionL, dataL, dataR;
    uint8_t BatLevelL, IR1, IR2,IL, IL2;;



    speedL=(int)((robot.DataReceived.data()[1] << 8) + robot.DataReceived.data()[0]);
    if (speedL > 32767) speedL=speedL-65536;

    BatLevelL=robot.DataReceived.data()[2];
    IL=robot.DataReceived.data()[3];
    IL2=robot.DataReceived.data()[4];

    odometryL=((((long)robot.DataReceived.data()[8] << 24))+(((long)robot.DataReceived.data()[7] << 16))+(((long)robot.DataReceived.data()[6] << 8))+((long)robot.DataReceived.data()[5]));

    speedR=(int)(robot.DataReceived.data()[10] << 8) + robot.DataReceived.data()[9];
    if (speedR > 32767) speedR=speedR-65536;

    BatLevelR=0;
    IR1=robot.DataReceived.data()[11];
    IR2=robot.DataReceived.data()[12];

    odometryR=((((long)robot.DataReceived.data()[16] << 24))+(((long)robot.DataReceived.data()[15] << 16))+(((long)robot.DataReceived.data()[14] << 8))+((long)robot.DataReceived.data()[13]));

    CurrentL=robot.DataReceived.data()[17];
    CurrentR=robot.DataReceived.data()[17];

    VersionL=robot.DataReceived.data()[18];
    VersionR=robot.DataReceived.data()[18];
    int dt = QDateTime::currentDateTime().toMSecsSinceEpoch();
    // DIAMETRE ROUE : 12.5cm
    // circonférence roues : 0.39m

    float currentSpeedR =((( odometryR - _speedWheelR )/ 2248.0f ) * 0.39f)/ (float)(dt-_oldTime);// 2448 ticks per wheel turn
    float currentSpeedL =((( odometryL - _speedWheelL )/2448.0f) *0.39f)/ (float)(dt-_oldTime) ; // same

    /*if(odometryR = _speedWheelR){ // debug
        currentSpeedR = 0;
    }
    if(odometryL = _speedWheelL){
        currentSpeedL = 0;
    }
*/
    // speed = d / t => d = number of wheel turn * pi * wheel diameter
    qDebug() << "data received " << BatLevelL << " " << IR1 << " "<<  IR2 <<" " << IL << " " << IL2<< "\n";
    qDebug() << "speed " << currentSpeedR << " " << currentSpeedL << "\n";
    qDebug() << "odometry L" << (((odometryL - _speedWheelL)/2448.0f)*0.39f)/(float)dt << "odometry R" << (((odometryR - _speedWheelR)/2448.0f)*0.39f)/(float)dt  << "\n";
    qDebug() << "dt " << (float)(dt-_oldTime) << "\n";
    ui->gauche_lcd->display(movingAverage(currentSpeedL)*1000); // displaying left speed in cm/s
    ui->droite_lcd->display(currentSpeedR*1000); // displaying right speed in cm/s
    ui->batterie->setValue(map(BatLevelL, 0, 255, 0, 100)); // set batterie level

    _speedWheelL = odometryL; // update old  speed
     _speedWheelR = odometryR; // update old  speed
    _oldTime = dt; // update old time


    //SETTING IR :

    //AVANT
    ui->HAUT_DROITE->setValue(map(IL, 0, 255, 0, 100)); // set batterie level
    ui->HAUT_GAUCHE->setValue(map(IR1, 0, 255, 0, 100)); // set batterie level



    //ARRIERE

    ui->BAS_DROIT->setValue(map(IL2, 0, 255, 0, 100)); // set batterie level
    ui->BAS_GAUCHE->setValue(map(IR2, 0, 255, 0, 100)); // set batterie level

    //value level : 90


}


long MainWindow::map(long x, long in_min, long in_max, long out_min, long out_max) { // from arduino
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void MainWindow::on_actionSe_d_connecter_triggered()
{
    robot.disConnect();
}

void MainWindow::on_Top_pressed()
{
    std::cout << "going top " << std::endl;

    robot.sendMovement(_speed,_speed);
}




void MainWindow::on_Top_released()
{
    std::cout << "stop going top " << std::endl;

    robot.sendMovement(0,0);
}




void MainWindow::on_bottom_pressed()
{
    std::cout << "going bottom " << std::endl;

    robot.sendMovement(-_speed,-_speed);

}




void MainWindow::on_bottom_released()
{
    std::cout << "stop going bottom " << std::endl;

    robot.sendMovement(0,0);
}





void MainWindow::on_left_pressed()
{
    std::cout << "going left " << std::endl;

    robot.sendMovement(_speed,0);
}


void MainWindow::on_left_released()
{
    std::cout << "stop going left " << std::endl;

    robot.sendMovement(0,0);
}

void MainWindow::on_right_pressed()
{
    std::cout << "going right " << std::endl;

    robot.sendMovement(0,_speed);
}

void MainWindow::on_right_released()
{
    std::cout << "stop going right " << std::endl;

    robot.sendMovement(0,0);
}

void MainWindow::showCamera(QUrl url){

    webView = new QWebEngineView();

    // Load a webpage
    webView->load(url);
    ui->gridLayout_2->addWidget(webView);
}

void MainWindow::on_verticalSlider_sliderMoved(int position)
{
    this->_speed = position % 128;
    std::cout << "new speed " << this->_speed << std::endl;
}


void MainWindow::on_CAMERA_DOWN_pressed()
{
    _cameraMove->moveCameraDown();

}


void MainWindow::on_CAMERA_RIGHT_pressed()
{
    _cameraMove->moveCameraRight();

}


void MainWindow::on_CAMERA_LEFT_pressed()
{
    _cameraMove->moveCameraLeft();

}


void MainWindow::on_CAMERA_UP_pressed()
{
    _cameraMove->moveCameraUp();

}

float MainWindow::movingAverage(float value){
    for(int i = 1; i <= 9; i++){
        _movingAverage[i] = _movingAverage[i-1]; // shift every value by 1 pos
    }
    _movingAverage[0] = value; // add new value
    float sum = 0; // average
    for(int i = 0; i < 9; i++){
        sum += _movingAverage[i];
    }return sum/10;
//TODO
}

