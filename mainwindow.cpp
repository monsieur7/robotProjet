#include "mainwindow.h"
#include "ui_mainwindow.h"
//QMediaPlayer
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
   QObject::connect(&robot, &MyRobot::updateUI, this, &MainWindow::updateUI);


   networkManager = new QNetworkAccessManager(this);

   QNetworkRequest request;
   request.setUrl(QUrl("http://192.168.1.106:8080/?action=stream"));
   reply = networkManager->get(request);

   connect(reply, &QNetworkReply::readyRead, this, &MainWindow::networkReplyReadyRead);

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
/*
     int speedR;
     int speedL;
     int odometryL;
     int odometryR;
     int BatLevelR;
     int BatLevelL;
     int IRR;
     int IRL;
     int CurrentL;
     int CurrentR;
     int VersionR;
     int VersionL;

      speedL=(int)((robot.DataReceived.data()[1] << 8) + robot.DataReceived.data()[0]);
      if (speedL > 32767) speedL=dataL>SpeedFront-65536;
      BatLevelL=DataReceived.data()[2];   dataL->IR=robot.DataReceived.data()[3];   dataL->IR2=DataReceived.data()[4];
      odometryL=((((long)DataReceived.data()[8] << 24))+(((long)robot.DataReceived.data()[7] << 16))+(((long)DataReceived.data()[6] << 8))+((long)DataReceived.data()[5]));

      speedR=(int)(DataReceived.data()[10] << 8) + robot.DataReceived.data()[9];
      if (speedR > 32767) speedR=dataR>SpeedFront-65536;
      BatLevelR=0;
     IRL=robot.DataReceived.data()[11];
      IRR=robot.DataReceived.data()[12];
    odometryR=((((long)robot.DataReceived.data()[16] << 24))+(((longrobot.)DataReceived.data()[15] << 16))+(((long)robot.DataReceived.data()[14] << 8))+((long)robot.DataReceived.data()[13]));
    CurrentL=robot.DataReceived.data()[17];
    CurrentR=robot.DataReceived.data()[17];
    VersionL=robot.DataReceived.data()[18];
    VersionR=robot.DataReceived.data()[18];
*/
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
#include <QImageReader>

// ...

#include <QImageReader>

// ...

void MainWindow::networkReplyReadyRead()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (reply && reply->error() == QNetworkReply::NoError)
    {
        // Append the received data to a buffer
        imageDataBuffer.append(reply->readAll());

        // Check if the full image frame was received
            int markerIndex = imageDataBuffer.indexOf(QByteArray::fromHex("FFD9"));
            if (markerIndex != -1)
            {
                // Extract the image frame data including the marker
                QByteArray imageFrameData = imageDataBuffer.left(markerIndex + 2);

                // Load the image data into a QImage
                QImageReader imageReader;
                imageReader.setDecideFormatFromContent(false);  // Disable format auto-detection
                imageReader.setFormat("JPEG");
                imageReader.setDevice(new QBuffer(&imageFrameData));  // Set the buffer containing image data
                QImage image = imageReader.read();
                std::cout << "reading image" << std::endl;
                if (!image.isNull())
                {
                    // Create a QPixmap from the loaded QImage
                    QPixmap pixmap = QPixmap::fromImage(image);
                    std::cout << "drawing image" << std::endl;


                    // Perform scaling operation on the pixmap
                    QPixmap scaledPixmap = pixmap.scaled(200, 4/3);

                    // Set the pixmap to your QLabel widget
                    ui->label_2->setPixmap(scaledPixmap);
                    ui->label_2->update();
                    // Remove the processed image frame from the buffer
                    imageDataBuffer.remove(0, markerIndex + 2);
                }
            }
            else
            {
                std::cout << " image null" << std::endl;

                return ;  // No more complete image frames in the buffer
            }
    }
}

