#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>
#include "server.h"
#include "udpbrdcastserver.h"
#include "wsclient.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    int port = 1234;
    UdpBrdcastServer *_udp = new UdpBrdcastServer();
    Server *server = new Server(port);
    QObject::connect(server, &Server::closed, &a, &QCoreApplication::quit);
    return a.exec();
}


