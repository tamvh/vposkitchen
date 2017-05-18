#include "udpbrdcastclient.h"
#include <QTimer>
#include <QtNetwork>
#include <QJsonObject>
#include <QJsonDocument>

UdpBrdcastClient::UdpBrdcastClient(int appid, QObject *parent)
    : QObject(parent)
    , appId(appid)
{
    udpSocket = new QUdpSocket(this);
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    brdcastTimer = new QTimer();
    connect(brdcastTimer, SIGNAL(timeout()), this, SLOT(ontimeBroadcast()));

    msgNo = 1;

    startBroadcasting();
}

void UdpBrdcastClient::startBroadcasting()
{
    brdcastTimer->start(5000);
}

void UdpBrdcastClient::ontimeBroadcast()
{
    QByteArray datagram = "Vpos client (" + QByteArray::number(appId) + ") detecting, msgNo:" + QByteArray::number(msgNo);

    udpSocket->writeDatagram(datagram.data(), datagram.size(),
                             QHostAddress::Broadcast, UDP_BRDCAST_PORT);

    ++msgNo;
}

void UdpBrdcastClient::readyRead()
{
    while (udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());

        udpSocket->readDatagram(datagram.data(), datagram.size());

        QString ip;
        int port;
        if( parseIp(datagram, ip, port) )
        {
            brdcastTimer->stop();

            qDebug() << "Udp Client, found server ip:" << ip << ", port:" << port;

            emit foundServer(ip, port);
        }
    }
}

bool UdpBrdcastClient::parseIp(const QByteArray &datagram, QString &ip, int &port)
{
    QJsonDocument jsd = QJsonDocument::fromJson(datagram);
    if( jsd.isObject() )
    {
        QJsonObject jso = jsd.object();
        if( jso.contains("ip")
            && jso.contains("port")
            && jso.contains("srvname")
            && jso["srvname"].toString().compare("Vpos") == 0 )
        {
            ip = jso["ip"].toString();
            port = jso["port"].toInt();

            return true;
        }
    }

    return false;
}
