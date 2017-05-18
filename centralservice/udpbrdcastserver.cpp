#include "udpbrdcastserver.h"
#include <QDebug>
#include <QNetworkInterface>
#include <QJsonObject>
#include <QJsonDocument>

UdpBrdcastServer::UdpBrdcastServer(QObject *parent) : QObject(parent)
{
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(UDP_BRDCAST_PORT, QUdpSocket::ShareAddress);

    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void UdpBrdcastServer::readyRead()
{
    while (udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());

        QHostAddress senderHost;
        quint16 senderPort;

        udpSocket->readDatagram(datagram.data(), datagram.size(),
                                                &senderHost, &senderPort);

        qDebug() << QString("received from %1:%2, msg=%3)")
                    .arg(senderHost.toString())
                    .arg(senderPort)
                    .arg(datagram.data());

        if( 1 ) // check if client send broadcast
        {
            QString msgdata = formatBroadcastInfo();
            int wrtcount = udpSocket->writeDatagram(msgdata.toUtf8(), senderHost, senderPort);

            qDebug() << "Respone" << wrtcount << "bytes of:" << msgdata;
        }
    }
}

QString UdpBrdcastServer::getLocalIp()
{
    QString ip;
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses())
    {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
        {
            if( ip.isEmpty() ) {
                ip = address.toString();
            }
            else {
                ip += "," + address.toString();
            }
        }
    }

    return ip;
}

QString UdpBrdcastServer::formatBroadcastInfo()
{
    QJsonObject jso;
    jso["srvname"] = "Vpos";
    jso["ip"] = getLocalIp();
    jso["port"] = (int)WS_SERVER_PORT;

    QJsonDocument jsd(jso);

    return QString::fromUtf8(jsd.toJson(QJsonDocument::Compact).data());
}
