// myrobot.cpp

#include "myrobot.h"

MyRobot::MyRobot(QObject *parent) : QObject(parent) {
    DataToSend.resize(9);
    DataToSend[0] = 0xFF;
    DataToSend[1] = 0x07;
    DataToSend[2] = 0x0;
    DataToSend[3] = 0x0;
    DataToSend[4] = 0x0;
    DataToSend[5] = 0x0;
    DataToSend[6] = 0x0;
    DataToSend[7] = 0x0;
    DataToSend[8] = 0x0;
    DataReceived.resize(21);
    TimerEnvoi = new QTimer();
    // setup signal and slot
    connect(TimerEnvoi, SIGNAL(timeout()), this, SLOT(MyTimerSlot())); //Send data to wifibot timer
}


bool MyRobot::doConnect() {
    socket = new QTcpSocket(this); // socket creation
    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));
    qDebug() << "connecting..."; // this is not blocking call
    //socket->connectToHost("LOCALHOST", 15020);
    socket->connectToHost(_ip, _port.toInt()); // connection to wifibot
    // we need to wait...
    if(!socket->waitForConnected(5000)) {
        qDebug() << "Error: " << socket->errorString();
        return false;
    }
    TimerEnvoi->start(75);
    _connected = true; // setting connected state
    return true;
}

bool MyRobot::disConnect() {
    _connected = false; // setting connected state
    TimerEnvoi->stop(); // stopping event loop
    socket->close();
    return true;
}

void MyRobot::connected() {
    qDebug() << "connected..."; // Hey server, tell me about you.
}
bool MyRobot::getConnected(){
    return _connected;
}
void MyRobot::disconnected() {
    qDebug() << "disconnected...";
}

void MyRobot::bytesWritten(qint64 bytes) {
    qDebug() << bytes << " bytes written...";
}

void MyRobot::readyRead() { //ready to read value
    if(_connected == true){
  //  qDebug() << "reading..."; // read the data from the socket
    DataReceived = socket->readAll(); // inform main window that we have value to parse
    emit updateUI(DataReceived);
    qDebug() << DataReceived[0] << DataReceived[1] << DataReceived[2];
    }
}

void MyRobot::MyTimerSlot() { // event loop (sending data every 2 seconds)
    qDebug() << "Timer...";
    while(Mutex.tryLock());
    socket->write(DataToSend);
    Mutex.unlock();
}

void MyRobot::setPort(QString port){
    _port = port;
}

void MyRobot::setIpAddress(QString ip){
    _ip = ip;
}

int MyRobot::Crc16(unsigned char *Adresse_tab , unsigned char Taille_max) {
    unsigned int Crc = 0xFFFF;
    unsigned int Polynome = 0xA001;
    unsigned int CptOctet = 0;
    unsigned int CptBit = 0;
    unsigned int Parity= 0;

 Crc = 0xFFFF;
 Polynome = 0xA001;
 for ( CptOctet= 1 ; CptOctet < Taille_max ; CptOctet++)  {
     Crc ^= *( Adresse_tab + CptOctet);

        for ( CptBit = 0; CptBit <= 7 ; CptBit++)   {
            Parity= Crc;
            Crc >>= 1;
            if (Parity%2 == true)
                Crc ^= Polynome;
        }
 }  return(Crc);

}

void MyRobot::sendMovement(int left, int right){

 if(_connected == true){
        // sending movement
    DataToSend[2] = abs(left) % 241; // limiting value and speed (> 0) because <0 MEANS opposed direction
    DataToSend[3] = 0x0;
    DataToSend[4] = abs(right) % 241;// limiting value and speed (> 0) because <0 MEANS opposed direction
    DataToSend[5] = 0x0;
    if(left > 0){
    DataToSend[6] = DataToSend[6] | 1 << 6; // motor direction
    }
    else {
        DataToSend[6] =DataToSend[6] & ~( 1 << 6);
    }
    if(right > 0){// motor direction
    DataToSend[6] =  DataToSend[6] | 1 << 4;
    }
    else {
        DataToSend[6] =  DataToSend[6] & ~( 1 << 4);

    }
    int crc = Crc16((unsigned char *)DataToSend.data(), 7); // CRC: IMPORTANT
    std::cout << std::hex << "crc" << crc << std::endl;
    DataToSend[7] = crc;
    DataToSend[8] = crc >> 8;
 }
}

void MyRobot::sendSequence(std::vector<movement *> sequence){ // sending sequence
    for(auto i : sequence){
    std::cout << "speed : " << i->speedL << " " << i->speedR <<  "time " << i->time << std::endl;
    sendMovement(i->speedL, i->speedR);
        // sending data
        MyTimerSlot();

        QEventLoop loop;
        QTimer::singleShot(i->time * 1000, &loop, SLOT(quit())); // blocking delay
        loop.exec();  // Blocking delay

        // After the specified time, stop the movement
        std::cout << "stopping robot" << std::endl;
        sendMovement(0, 0);

        // sending data
        MyTimerSlot();

        QEventLoop loop2;
        QTimer::singleShot(100, &loop2, SLOT(quit())); // blocking delay
        loop2.exec();  // Blocking delay
    }
}

