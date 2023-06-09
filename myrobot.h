#ifndef MYROBOT_H
#define MYROBOT_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>
#include <QTimer>
#include <QMutex>
#include <iostream>
#include <thread>
#include <QEventLoop>
#include <QDateTime>

typedef struct movement {
    int speedL;
    int speedR;
    long time;
    long time_between_press;
}movement;
enum MOVEMENT_TYPE {

    STOPPED,
    LEFT,
    RIGHT,
    TOP,
    BOTTOM
};
class MyRobot : public QObject {
    Q_OBJECT
public:
    explicit MyRobot(QObject *parent = 0);
    bool doConnect();
    bool disConnect();
    QByteArray DataToSend;
    QByteArray DataReceived;
    QMutex Mutex;
signals:
    void updateUI(const QByteArray Data);
public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();
    void MyTimerSlot();
    void setIpAddress(QString ip);
    void setPort(QString port);
    void sendMovement(int left, int right);
    bool getConnected();
    void sendSequence(std::vector<movement *> sequence);
private:
    QTcpSocket *socket;
    QTimer *TimerEnvoi;
    QString _ip;
    QString _port;
    bool _connected = false;
    int Crc16(unsigned char *Adresse_tab , unsigned char Taille_max);
};

#endif // MYROBOT_H
