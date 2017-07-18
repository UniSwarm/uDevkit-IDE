#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.openFile(QApplication::applicationDirPath()+"/../../rtprog/support/driver/periph/uart/uart.h");
    w.show();

    return a.exec();
}
