#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--no-sandbox");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
