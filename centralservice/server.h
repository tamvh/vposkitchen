#ifndef SERVER_H
#define SERVER_H
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <QString>
#include <QMap>
#include <QJsonDocument>
#include "wsclient.h"

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)
class Server : public QObject

{
    Q_OBJECT
public:
    explicit Server(quint16 port, QObject *parent = Q_NULLPTR);
    ~Server();
Q_SIGNALS:
    void closed();
private Q_SLOTS:
    void onNewConnection();
    void processTextMessage(QString message);
    void processBinaryMessage(QByteArray message);
    void socketDisconnected();
    void initialWebSocket(bool forceStop=false);
    void onNotify(const QString &message);
    void onCloudNotify(const QString &message);
    void send_message(int to_appid, const QString &message);
private:
    QWebSocketServer *m_pWebSocketServer;
    QList<QWebSocket *> m_clients;
    typedef QList<QWebSocket *> M_WS;
    typedef QMap<int, M_WS> WS;
    WS list_client;
    WSClient            *wsClient;
};

#endif // SERVER_H
