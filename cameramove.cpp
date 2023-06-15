#include "cameramove.h"
cameraMove::cameraMove()
{
    _networkManager = new QNetworkAccessManager(); // in order to send network requests

}
void cameraMove::moveCameraDown(){
    _networkManager->get(QNetworkRequest((QUrl("http://192.168.1.106:8080/?action=command&dest=0&plugin=0&id=10094853&group=1&value=200"))));

}
//TODO : replay finished
void cameraMove::moveCameraUp(){
    _networkManager->get(QNetworkRequest((QUrl("http://192.168.1.106:8080/?action=command&dest=0&plugin=0&id=10094853&group=1&value=-200"))));

}

void cameraMove::moveCameraLeft(){
    _networkManager->get(QNetworkRequest((QUrl("http://192.168.1.106:8080/?action=command&dest=0&plugin=0&id=10094852&group=1&value=200"))));

}
void cameraMove::moveCameraRight(){
    _networkManager->get(QNetworkRequest((QUrl("http://192.168.1.106:8080/?action=command&dest=0&plugin=0&id=10094852&group=1&value=-200"))));

}
