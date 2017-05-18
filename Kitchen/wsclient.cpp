#include "wsclient.h"
#include "commonfunction.h"

#include <QtCore/QDebug>
#define PING_TIMER 3000

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
    HIDREDER
};

WSClient::WSClient(const QUrl &url, QObject *parent) :
    QThread(parent),
    m_url(url),
    isConnected(false)
{
    connect(&m_webSocket, &QWebSocket::connected, this, &WSClient::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &WSClient::onDisconnected);
    connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &WSClient::onTextMessageReceived);
    connect(&m_reconnTimer, SIGNAL(timeout()), this, SLOT(onReconnect()) );
    m_webSocket.open(QUrl(m_url));
}

WSClient::~WSClient()
{
    qDebug() << "close websocket client!";
}

void WSClient::start()
{
    timerPing = startTimer(PING_TIMER);
    m_start = true;
    QThread::start();
}

void WSClient::stop()
{
    m_start = false;
    this->terminate();
}

void WSClient::onConnected()
{
    qDebug() << "onConnected";
    if( m_reconnTimer.isActive() )
    {
        qDebug() << "stopTimer...";
        m_reconnTimer.stop();
    }
    isConnected = true;
    QString json_client_login = initJson();
    m_webSocket.sendTextMessage(json_client_login);
}

void WSClient::sendMsg(const QString &msg)
{
     qint64 sent = m_webSocket.sendTextMessage(msg);
     qDebug() << "Sent: " << sent;
}

#include <QTimerEvent>
void WSClient::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == timerPing) {
        qDebug() << "Timer ping event";
        if (pingFlag > 2) {
            return;
        }
        if (m_webSocket.isValid()) {
            qDebug() << "Send ping";
            m_webSocket.ping(QString("ping").toLocal8Bit());
            pingFlag++;
        } else {
            qDebug() << "Websocket not valid";
        }
    }
}

void WSClient::onTextMessageReceived(QString message)
{
    qDebug() << "Message received:" << message;
    if(QString::compare(message, "CONNECTED") != 0) {
        emit orderNumber(jsonData(message));
    }
}

void WSClient::onDisconnected()
{
    qDebug() << "onDisconnected";
    isConnected = false;

    if( !m_reconnTimer.isActive() )
    {
        qDebug() << "start retry connection...";
        m_reconnTimer.start(5000);
    }
}

void WSClient::onReconnect()
{
    qDebug() << "onReconnect";
    m_webSocket.open(QUrl(m_url));
}



QString WSClient::initJson() {
    QJsonObject jso;
    jso["action"] = packet_content_type::CLIENT_LOGIN;
    jso["appid"] = APPID::KITCHEN_VIEW;
    jso["to_appid"] = APPID::UNKNOWN;
    QJsonDocument doc(jso);
    return doc.toJson(QJsonDocument::Compact);
}

QVariant WSClient::jsonData(QString number) {
    QJsonObject jso;
    bool ck = false;
    jso["number"] = number.toInt();
    for (int i = 0; i < aJson.size(); ++i) {
        if(QJsonValue(jso) == (aJson.at(i)))
        {
            ck = true;
            break;
        }
    }
    if(!ck) {
        aJson.insert(0, jso);
    }
    return aJson;
}
