#include "mainwindow.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QList<QScreen *> screenList = QGuiApplication::screens();
    qDebug("screen size: %d;", screenList.size());
    QMainWindow window;
    if (screenList.size() > 1) {
        MainWindow w;
        w.init(0);
        w.show();
        MainWindow w2;
        w2.init(1);
        w2.show();
        return a.exec();
    } else {
        MainWindow w;
        w.init(0);
        w.show();
        return a.exec();
    }
}
