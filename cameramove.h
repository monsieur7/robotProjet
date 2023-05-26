#ifndef CAMERAMOVE_H
#define CAMERAMOVE_H
 #include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
class cameraMove
{
public:
    cameraMove();
    void moveCameraLeft();
    void moveCameraRight();
    void moveCameraUp();
    void moveCameraDown();
private:
    QNetworkAccessManager * _networkManager;
    bool _finishedConnect = false;

};

#endif // CAMERAMOVE_H
