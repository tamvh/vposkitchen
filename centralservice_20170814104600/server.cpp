#include "server.h"
#include <QtWebSockets/qwebsocketserver.h>
#include <QtWebSockets/qwebsocket.h>
#include <QtCore/QDebug>
#include <QJsonObject>
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
    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {
        qDebug() << "start server...";
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection, this, &Server::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &Server::closed);
    }
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
