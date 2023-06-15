#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWebEngineView>
//QMediaPlayer
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    login = new Login(nullptr, &robot);
    // on connected : show camera ! (only one time
    connect(login, &QDialog::finished, this, [this](int result) {if(result == 1){this->showCamera(QUrl("http://192.168.1.106:8080/?action=stream"));}}); // open camera when the dialog is finished !
    connect(&robot, &MyRobot::updateUI, this, &MainWindow::updateUI); //update sensor values when data is received
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
    ui->GAUCHE_DISTANCE->display(0);
    ui->DROITE_DISTANCE->display(0);
    _cameraMove = new cameraMove(); // init camera movement
    for(int i = 0; i < 10; i++){ // moving average (todo)
        _movingAverage[i] = 0;
    }
    _enregistrerState = 0; // state of enregistrer button
    this->on_verticalSlider_sliderMoved(ui->verticalSlider->minimum()); // setting min value and color for speed color
    _movementType = STOPPED; // default value


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
    int speedR, speedL, odometryL, odometryR;
    uint8_t BatLevelL, IR1, IR2,IL1, IL2;
    //gedding values
    speedL=(int)((robot.DataReceived.data()[1] << 8) + robot.DataReceived.data()[0]);
    if (speedL > 32767) speedL=speedL-65536;

    BatLevelL=robot.DataReceived.data()[2];
    IL1=robot.DataReceived.data()[3];
    IL2=robot.DataReceived.data()[4];

    odometryL=((((long)robot.DataReceived.data()[8] << 24))+(((long)robot.DataReceived.data()[7] << 16))+(((long)robot.DataReceived.data()[6] << 8))+((long)robot.DataReceived.data()[5]));

    speedR=(int)(robot.DataReceived.data()[10] << 8) + robot.DataReceived.data()[9];
    if (speedR > 32767) speedR=speedR-65536;

    IR1=robot.DataReceived.data()[11];
    IR2=robot.DataReceived.data()[12];

    odometryR=((((long)robot.DataReceived.data()[16] << 24))+(((long)robot.DataReceived.data()[15] << 16))+(((long)robot.DataReceived.data()[14] << 8))+((long)robot.DataReceived.data()[13]));

    //getting value !
    int dt = QDateTime::currentDateTime().toMSecsSinceEpoch();
    // DIAMETRE ROUE : 12.5cm
    // circonférence roues : 0.39m

    float currentSpeedR =((( odometryR - _speedWheelR )/ 2248.0f ) * 0.39f)/ (float)(dt-_oldTime);// 2448 ticks per wheel turn
    float currentSpeedL =((( odometryL - _speedWheelL )/2448.0f) *0.39f)/ (float)(dt-_oldTime) ; // same

    float distanceL =     distanceL = (((( odometryL )/2448.0f) *0.39f)); // updating distance value !
    float distanceR =     distanceR = (((( odometryR )/2448.0f) *0.39f));



    /*if(odometryR = _speedWheelR){ // debug
        currentSpeedR = 0;
    }
    if(odometryL = _speedWheelL){
        currentSpeedL = 0;
    }
*/
    // speed = d / t => d = number of wheel turn * pi * wheel diameter
    /*qDebug() << "data received " << BatLevelL << " " << IR1 << " "<<  IR2 <<" " << IL1 << " " << IL2<< "\n";
    qDebug() << "speed " << currentSpeedR << " " << currentSpeedL << "\n";
    qDebug() << "odometry L" << (((odometryL - _speedWheelL)/2448.0f)*0.39f)/(float)dt << "odometry R" << (((odometryR - _speedWheelR)/2448.0f)*0.39f)/(float)dt  << "\n";
    qDebug() << "dt " << (float)(dt-_oldTime) << "\n";
*/
    //TODO  : moving average !
    ui->gauche_lcd->display(currentSpeedL*1000); // displaying left speed in cm/s
    ui->droite_lcd->display(currentSpeedR*1000); // displaying right speed in cm/s
    ui->batterie->setRange(0, 100); // 10* the battery voltage  (101 : 10.1v)
    ui->batterie->setValue(map(std::min<uint8_t>(BatLevelL, (uint8_t) 101), 12, 101, 0, 100)); // set batterie level
    qDebug() << "batlevel " << std::min<uint8_t>(BatLevelL, (uint8_t) 101) <<  "level" << ui->batterie->value() << "\n";
    _speedWheelL = odometryL; // update old  speed
     _speedWheelR = odometryR; // update old  speed
    _oldTime = dt; // update old time
    //setting distance :
    ui->GAUCHE_DISTANCE->display(distanceL);
    ui->DROITE_DISTANCE->display(distanceR);

    //SETTING IR :

    //AVANT
    QPalette palette;
    //THRESHOLD IS 100
    // FOR IL2, threshold is 85
    //setting color to red if value > 100, green else
    palette = ui->HAUT_DROITE->palette();
    if(IL1 > 100){
        IL1 = 100;
        palette.setColor(QPalette::Highlight, Qt::red); // Qpalette::Highlight is the progress bar color
    }else {
        IL1 = 0;
        palette.setColor(QPalette::Highlight, Qt::green); // Qpalette::Highlight is the progress bar color
    }
    ui->HAUT_DROITE->setPalette(palette); // setting palette
    ui->HAUT_DROITE->show(); // updating color !
    palette = ui->HAUT_GAUCHE->palette();
    // IR thresholds (*4)
    if(IR1 > 100){
        IR1 = 100;
        palette.setColor(QPalette::Highlight, Qt::red); // Qpalette::Highlight is the progress bar color

    }else {
        IR1 = 0;
        palette.setColor(QPalette::Highlight, Qt::green); // Qpalette::Highlight is the progress bar color

    }
    ui->HAUT_GAUCHE->setPalette(palette); // setting palette
    ui->HAUT_GAUCHE->show();// updating color !

    palette = ui->BAS_DROIT->palette();
    if(IL2 > 85){
        IL2 = 100;
        palette.setColor(QPalette::Highlight, Qt::red); // Qpalette::Highlight is the progress bar color

    }else {
        IL2 = 0;
        palette.setColor(QPalette::Highlight, Qt::green); // Qpalette::Highlight is the progress bar color

    }
    ui->BAS_DROIT->setPalette(palette); // setting palette
    ui->BAS_DROIT->show();// updating color !

    palette = ui->BAS_GAUCHE->palette();
    if(IR2 > 80){
        IR2 = 100;
        palette.setColor(QPalette::Highlight, Qt::red); // Qpalette::Highlight is the progress bar color

    }else {
        IR2 = 0;
        palette.setColor(QPalette::Highlight, Qt::green); // Qpalette::Highlight is the progress bar color

    }
    ui->BAS_GAUCHE->setPalette(palette); // setting palette
    ui->BAS_GAUCHE->show();// updating color !
    // DROITE ET GAUCHE INVERSEE !
    ui->HAUT_DROITE->setValue(IL1); // set ir level
    ui->HAUT_GAUCHE->setValue(IR1); // set ir level



    //ARRIERE

    ui->BAS_DROIT->setValue(IL2); // set ir level
    ui->BAS_GAUCHE->setValue(IR2); // set ir level


//stopping if we have an ir obstacle :
   switch(_movementType){
    case LEFT : {
        if(ui->BAS_GAUCHE->value() == 100 ||ui->HAUT_GAUCHE->value() == 100){
            this->robot.sendMovement(0, 0); // stopping robot;
            _movementType = STOPPED;

        }
        break;

    case RIGHT: {
        if(ui->BAS_DROIT->value() == 100 ||ui->HAUT_DROITE->value() == 100){
            this->robot.sendMovement(0, 0); // stopping robot;
            _movementType = STOPPED;

        }
        break;

    }
    case BOTTOM : {
        if(ui->BAS_GAUCHE->value() == 100 ||ui->BAS_DROIT->value() == 100){
            this->robot.sendMovement(0, 0); // stopping robot;
            _movementType = STOPPED;
        }
        break;

    }
    case TOP: {
        if(ui->HAUT_GAUCHE->value() == 100 ||ui->HAUT_DROITE->value() == 100){
            this->robot.sendMovement(0, 0); // stopping robot;
            _movementType = STOPPED;

        }
        break;

    }
    case STOPPED: { // stopped !
        break;
    }default :  break;

    }

}


}


long MainWindow::map(long x, long in_min, long in_max, long out_min, long out_max) { // from arduino
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void MainWindow::on_actionSe_d_connecter_triggered()
{
    if(robot.getConnected() == true){ // only disconnect when connected !
        robot.disConnect();
    }
}

void MainWindow::on_Top_pressed() //saving sequence
{

    if(_enregistrerState == 1){
        _mov = new movement;
        _mov->speedL = _speed;
        _mov->speedR = _speed;
        _mov->time = QDateTime::currentDateTime().toSecsSinceEpoch(); // setting time
    }
    else {
        //stopping robot
        robot.sendMovement(_speed,_speed);
        _movementType = TOP;

    }
}




void MainWindow::on_Top_released()
{
    if(_enregistrerState == 1){
        // saving sequence
        _mov->time = QDateTime::currentDateTime().toSecsSinceEpoch() - _mov->time;// saving time (delta)

        _sequence.push_back(_mov);
        std::cout << "time " << _mov->time << std::endl;
    }else {
        //stopping robot
        robot.sendMovement(0,0);
        _movementType = STOPPED;
    }
}




void MainWindow::on_bottom_pressed()
{
//

    std::cout << "going bottom " << std::endl;
    if(_enregistrerState == 1){ // saving sequence
        _mov = new movement;
        _mov->speedL = -_speed;
        _mov->speedR = -_speed;
        _mov->time = QDateTime::currentDateTime().toSecsSinceEpoch(); // setting time


    }
    else { //moving robot
         robot.sendMovement(-_speed,-_speed);
        _movementType = BOTTOM;
    }

}




void MainWindow::on_bottom_released()
{
    std::cout << "stop going bottom " << std::endl;
    if(_enregistrerState == 1){
    // saving sequence
    _mov->time =QDateTime::currentDateTime().toSecsSinceEpoch() - _mov->time;// saving time (delta)


    _sequence.push_back(_mov);
    }else { // stopping robot
    robot.sendMovement(0,0);
    _movementType = STOPPED;
    }
}





void MainWindow::on_left_pressed()
{
    if(_enregistrerState == 1){
        _mov = new movement;
        _mov->speedL = _speed;
        _mov->speedR = 0;
        _mov->time = QDateTime::currentDateTime().toSecsSinceEpoch(); // setting time

    }
    else {

    robot.sendMovement(_speed,0);
        _movementType = LEFT;

    }
    std::cout << "going left " << std::endl;

}


void MainWindow::on_left_released()
{
    if(_enregistrerState == 1){
        // saving sequence
        _mov->time = QDateTime::currentDateTime().toSecsSinceEpoch() - _mov->time;// saving time (delta)


        _sequence.push_back(_mov);
        _movementType = STOPPED;
    }else {
        robot.sendMovement(0,0);
    }

}

void MainWindow::on_right_pressed()
{
    if(_enregistrerState == 1){
        _mov = new movement;
        _mov->speedL = 0;
        _mov->speedR = _speed;
        _mov->time = QDateTime::currentDateTime().toSecsSinceEpoch(); // setting time

    }
    else {
     robot.sendMovement(0,_speed);
    _movementType = RIGHT;
    }

}

void MainWindow::on_right_released()

{    if(_enregistrerState == 1){
        // saving sequence
        _mov->time = QDateTime::currentDateTime().toSecsSinceEpoch() - _mov->time; // saving time (delta)

        _sequence.push_back(_mov);
    }else {
        robot.sendMovement(0,0);
        _movementType = STOPPED;

    }
}

void MainWindow::showCamera(QUrl url){

    webView = new QWebEngineView();

    // Load a webpage
    webView->load(url);
    ui->gridLayout_2->addWidget(webView);
}
// on speed slider moved
void MainWindow::on_verticalSlider_sliderMoved(int position)
{
    this->_speed = position % 241; // capping the value at 241
    std::cout << "new speed " << this->_speed << std::endl;
    double normalizedValue = map(_speed, ui->verticalSlider->minimum(), ui->verticalSlider->maximum(), 0, 255);
    int red = static_cast<int>(255-normalizedValue); // red zone
    int green = static_cast<int>(normalizedValue); // green zone

    QString styleSheet = QString("QSlider::handle:vertical { background-color: rgb(%1, %2, %3); }")
                             .arg(red).arg(green).arg(0);

    ui->verticalSlider->setStyleSheet(styleSheet); // modifying slider value

}

// arrows key for camera movement
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

float MainWindow::movingAverage(float value){ // not used
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


void MainWindow::on_enregistrer_clicked()
{
        if(_enregistrerState == 0){ // button was pressed for saving sequence
            _enregistrerState = 1; // updating state
          _sequence.clear(); // in case not done
            // changing button color
            this->ui->enregistrer->setStyleSheet("background-color: red;");
        }
        else if(_enregistrerState == 1){ // button was pressed for ending sequence saving
            _enregistrerState = 0;
            this->ui->enregistrer->setStyleSheet("background-color: green;");
        }
        //TODO : change color somewhat
        else {
            _enregistrerState = 0; // reseting state because the state value is illegal !
        }
        std::cout << "state " << _enregistrerState << std::endl;

}


void MainWindow::on_executer_clicked()

{
        if(_enregistrerState == 0){ // sending dequence
        if(this->robot.getConnected()){
        this->robot.sendSequence(_sequence);
        std::cout << "sequence has been sent !" << std::endl;
        for(movement * i : _sequence){
            delete [] i; // freeing memory
        }
        _sequence.clear(); // reseting sequence array

        this->ui->enregistrer->setStyleSheet("background-color: rgb(255, 255, 255);"); // resetting color
        }
        }
}

void MainWindow::keyPressEvent(QKeyEvent *event){
        // getting pressed event (keyboard)
        if(event->key() == Qt::Key_Z){
        if(event->type() == QEvent::KeyPress){
        this->on_Top_pressed();
        }

        }
        if(event->key() == Qt::Key_S){
        if(event->type() == QEvent::KeyPress){
        this->on_bottom_pressed();
        }

        }

        if(event->key() == Qt::Key_Q){
        if(event->type() == QEvent::KeyPress){
        this->on_left_pressed();
        }

        }

        if(event->key() == Qt::Key_D){
        if(event->type() == QEvent::KeyPress){
        this->on_right_pressed();
        }

        }
        if(event->key() == Qt::Key_Up){
        if(event->type() == QEvent::KeyPress){
        this->_cameraMove->moveCameraUp();
        }

        }
        if(event->key() == Qt::Key_Down){
        if(event->type() == QEvent::KeyPress){
        this->_cameraMove->moveCameraDown();
        }

        }
        if(event->key() == Qt::Key_Right){
        if(event->type() == QEvent::KeyPress){
        this->_cameraMove->moveCameraRight();
        }

        }
        if(event->key() == Qt::Key_Left){
        if(event->type() == QEvent::KeyPress){
        this->_cameraMove->moveCameraLeft();
        }

        }


}
void MainWindow::keyReleaseEvent(QKeyEvent *event){
    // getting released event (keyboard)
       // if the key is released (zqsd)  stopping the robot / saving it in sequence
        if(event->key() == Qt::Key_Z){
        if(event->type() == QEvent::KeyRelease){
        this->on_Top_released();
        }
        }
        if(event->key() == Qt::Key_S){
        if(event->type() == QEvent::KeyRelease){
        this->on_bottom_released();
        }
        }
        if(event->key() == Qt::Key_Q){
        if(event->type() == QEvent::KeyRelease){
        this->on_left_released();
        }
        }
        if(event->key() == Qt::Key_D){
        if(event->type() == QEvent::KeyRelease){
        this->on_right_released();
        }
    // no camera key because we move the camera when we press on the key
        }

}
