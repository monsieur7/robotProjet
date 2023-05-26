#include "cameramove.h"
cameraMove::cameraMove()
{
    _networkManager = new QNetworkAccessManager();

}
int cameraMove::moveCameraDown(){
    _networkManager->get(QNetworkRequest((QUrl("http://192.168.1.106:8080/?action=command&dest=0&plugin=0&id=10094853&group=1&value=200"))));

}
//TODO : replay finished
int cameraMove::moveCameraUp(){
    _networkManager->get(QNetworkRequest((QUrl("http://192.168.1.106:8080/?action=command&dest=0&plugin=0&id=10094853&group=1&value=-200"))));

}

int cameraMove::moveCameraLeft(){
    _networkManager->get(QNetworkRequest((QUrl("http://192.168.1.106:8080/?action=command&dest=0&plugin=0&id=10094852&group=1&value=200"))));

}
int cameraMove::moveCameraRight(){
    _networkManager->get(QNetworkRequest((QUrl("http://192.168.1.106:8080/?action=command&dest=0&plugin=0&id=10094852&group=1&value=-200"))));

}
