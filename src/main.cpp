#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

#include "project/project.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // apply dark style
    QFile f(":qdarkstyle/style.qss");
    if (f.exists())
    {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }

    Project *project = nullptr;
    if (argc > 1)
        project = new Project(argv[1]);

    MainWindow w(project);
    w.show();

    return a.exec();
}
