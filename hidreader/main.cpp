#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>
#include "hidreadermain.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    HidReaderMain* hrm = new HidReaderMain();
    return a.exec();
}
