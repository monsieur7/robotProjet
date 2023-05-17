#ifndef MYROBOT_H
#define MYROBOT_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>
#include <QTimer>
#include <QMutex>
#include <iostream>
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
private:
    QTcpSocket *socket;
    QTimer *TimerEnvoi;
    QString _ip;
    QString _port;
    int Crc16(unsigned char *Adresse_tab , unsigned char Taille_max);
};

#endif // MYROBOT_H
