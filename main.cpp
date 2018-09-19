#include "mainwindow.h"
#include <QApplication>
#include "test.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    Test t;
    t.setup();
    t.loop();

    return a.exec();
}
