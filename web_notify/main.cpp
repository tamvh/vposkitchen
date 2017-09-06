#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>
#include "recievenotify.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    RecieveNotify* hrm = new RecieveNotify();
    return a.exec();
}
