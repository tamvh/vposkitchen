#include "server.h"
#include <QtWebSockets/qwebsocketserver.h>
#include <QtWebSockets/qwebsocket.h>
#include <QtCore/QDebug>
#include <QJsonObject>
#include <QUuid>
QT_USE_NAMESPACE

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

Server::Server(quint16 port, QObject *parent):
    QObject(parent),
    m_pWebSocketServer(new QWebSocketServer(QStringLiteral("Central Server"), QWebSocketServer::NonSecureMode, this)),
    m_clients()
{
     wsClient = 0;

    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {
        qDebug() << "start server...";
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection, this, &Server::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &Server::closed);
    }
    initialWebSocket();
}

Server::~Server() {
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}

void Server::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();
    connect(pSocket, &QWebSocket::textMessageReceived, this, &Server::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &Server::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &Server::socketDisconnected);

    m_clients << pSocket;
}

void Server::processTextMessage(QString message)
{
    qDebug() << message;
    int appid = 0;
    int action  = 0;
    int to_appid = 0;
    QString data = "";
    WS::iterator iLookup;

    try {
        QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
        QJsonDocument jsd = QJsonDocument::fromJson(message.toUtf8());
        qDebug() << "data: " << jsd;
        if(jsd.isObject()) {
            QJsonObject jso = jsd.object();
            action = jso["action"].toInt();
            appid = jso["appid"].toInt();
            //LOGIN
            if(action == packet_content_type::CLIENT_LOGIN) {
                // add client to map
                M_WS ws_item;
                iLookup = list_client.find(appid);
                if(iLookup != list_client.end()) {
                    ws_item = iLookup.value();
                }
                if(appid > 0) {
                    ws_item.push_back(pClient);
                    list_client[appid] = ws_item;
                }
            } else if (action == packet_content_type::CLIENT_LOGOUT) {
            } else if (action == packet_content_type::CHAT_TEXT_MSG) {
                //send msg
                M_WS l_ws;
                data = jso["data"].toString();
                to_appid = jso["to_appid"].toInt();
                iLookup = list_client.find(to_appid);
                if(iLookup != list_client.end()) {
                    l_ws = iLookup.value();
                }

                for(int i = 0;i < l_ws.size(); i++)
                {
                    if(l_ws.at(i)) {
                        l_ws.at(i)->sendTextMessage(data);
                    } else {
                        l_ws.removeAll(l_ws.at(i));
                        pClient->deleteLater();
                    }
                }
                list_client[to_appid] = l_ws;
            }
        }
    } catch (const std::bad_alloc &) {
    }
}

void Server::send_message(int to_appid, const QString &message) {
    M_WS l_ws;
    WS::iterator iLookup;
    iLookup = list_client.find(to_appid);
    if(iLookup != list_client.end()) {
        l_ws = iLookup.value();
    }

    for(int i = 0;i < l_ws.size(); i++)
    {
        if(l_ws.at(i)) {
            l_ws.at(i)->sendTextMessage(message);
        } else {
            l_ws.removeAll(l_ws.at(i));
        }
    }
}

void Server::processBinaryMessage(QByteArray message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient) {
        // send all client
        QWebSocket *client;
        for(int i = 0;i < m_clients.size(); i++) {
            client = m_clients.at(i);
            client->sendBinaryMessage(message);
        }
    }
}

void Server::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient) {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
    QList<int> mapKeys = list_client.keys();
    for (int i = 0; i < mapKeys.size(); ++i) {
        M_WS ws_item;
        ws_item = list_client[mapKeys[i]];
        if(pClient) {
            ws_item.removeAll(pClient);
            pClient->deleteLater();
            list_client[mapKeys[i]] = ws_item;
        }
    }
    QString str = "";
}

void Server::initialWebSocket(bool forceStop)
{
    qDebug() << "initialWebSocket ... forcestop=" << forceStop << "\n";
    QString deviceId = QString(QUuid::createUuid().toRfc4122().toHex());
//    QString deviceId = "ffffffffffffffffffffffffffffffff";

    if( forceStop && wsClient )
    {
        wsClient->stop();
        wsClient->wait(10000);

        delete wsClient;
        wsClient = 0;
    }

    if (wsClient == 0)
    {
        QUrl wsUrl = QUrl("wss://gbcstaging.zing.vn/vpos/ntf/");
        QUrl httpUrl = QUrl("https://gbcstaging.zing.vn/vpos/api/common/");
        QString merchant_code = "maybanhangtudong";

        wsClient = new WSClient(wsUrl, httpUrl, merchant_code, deviceId, true, 0);

        QObject::connect(wsClient, SIGNAL(textMessageReceived(QString)), this, SLOT(onCloudNotify(const QString&)));
        QObject::connect(wsClient, &WSClient::connected, this, [=](){qDebug() << "cloud Connected";});
        QObject::connect(wsClient, &WSClient::closed, this, [=](){qDebug() << "cloud Disconnected";});

        wsClient->start();
    }
}

void Server::onNotify(const QString &message)
{
    QString notifymsg = CommonFunction::getNotifyValue(message, "msg");
    qDebug() << "notifymsg: " << notifymsg<< ", data: " << message;

}


void Server::onCloudNotify(const QString &message)
{
    QString notifymsg = CommonFunction::getNotifyValue(message, "msg");
    QString notifydt = CommonFunction::getNotifyValue(message, "dt");
    qDebug() << "notifymsg: " << notifymsg<< ", data: " << notifydt;
    this->send_message(APPID::PRINTER, notifydt);
}
