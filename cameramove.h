#ifndef CAMERAMOVE_H
#define CAMERAMOVE_H
 #include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
class cameraMove
{
public:
    cameraMove();
    int moveCameraLeft();
    int moveCameraRight();
    int moveCameraUp();
    int moveCameraDown();
private:
    QNetworkAccessManager * _networkManager;
    bool _finishedConnect = false;

};

#endif // CAMERAMOVE_H
