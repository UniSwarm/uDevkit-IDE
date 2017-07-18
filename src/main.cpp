#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::setDesktopSettingsAware(false);
    QApplication a(argc, argv);
    MainWindow w;

    w.openFile("../../rtprog/support/driver/periph/uart/uart.h");
    w.show();

    return a.exec();
}
