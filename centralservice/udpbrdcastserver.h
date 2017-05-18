#ifndef UDPBRDCASTSERVER_H
#define UDPBRDCASTSERVER_H

#include <QObject>
#include <QUdpSocket>

#define UDP_BRDCAST_PORT    45459
#define WS_SERVER_PORT      1234 //54549

class UdpBrdcastServer : public QObject
{
    Q_OBJECT
public:
    explicit UdpBrdcastServer(QObject *parent = 0);

signals:

public slots:
    void    readyRead();

private:
    QUdpSocket *udpSocket;

    QString     getLocalIp();
    QString     formatBroadcastInfo();
};

#endif // UDPBRDCASTSERVER_H
