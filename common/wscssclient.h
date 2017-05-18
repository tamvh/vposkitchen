#ifndef WSCSSCLIENT_H
#define WSCSSCLIENT_H
#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QString>
#include <QVariant>
#include <QTimer>
#include "udpbrdcastclient.h"

enum packet_content_type{
    NONE,
    CLIENT_LOGIN,
    CLIENT_LOGOUT,
    NEW_CONNECTED_CLIENTS,
    NEW_DISCONNECTED_CLIENTS,
    CHAT_TEXT_MSG
};

enum APPID {
    UNKNOWN,
    KITCHEN_VIEW,
    VPOS,
    HIDREDER,
    PRINTER
};

class WsCssClient : public QObject
{
    Q_OBJECT
public:
    explicit WsCssClient(int fromId=APPID::UNKNOWN, int toId=APPID::UNKNOWN, QObject *parent = Q_NULLPTR);
    ~WsCssClient();

    void sendMsg(const QString &msg);
    void sendTextMessage(const QString &msg);

signals:
    void connected();
    void disconnected();
    void textMessageReceived(QString msg);
    void foundServer(const QString &ip, const int &port);
    void orderNumber(const QVariant& newData);

public slots:
    void onServer(const QString &ips, const int &port);
    void onConnected();
    void onClosed();
    void onReconnect();
    void onTextMessageReceived(QString message);

private:
    QString initJson(const int fromId, const int toId);
    QVariant jsonData(QString number);

private:

    UdpBrdcastClient *brdcastClient;

    QWebSocket  m_webSocket;
    QUrl        m_url;
    bool        isConnected;
    int         m_id;
    int         m_toId;

    QJsonArray aJson;
};

#endif // WSCSSCLIENT_H
