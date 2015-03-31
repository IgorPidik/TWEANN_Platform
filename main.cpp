#include <QCoreApplication>
#include "ExampleController.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTime time = QTime::currentTime();
    qsrand((int)time.msec());



    Controller controller;

    return a.exec();
}
