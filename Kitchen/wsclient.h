#ifndef WSCLIENT_H
#define WSCLIENT_H
#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QString>
#include <QVariant>
#include <QThread>
#include <QTimer>

class WSClient : public QThread
{
    Q_OBJECT
public:
    explicit WSClient(const QUrl &url, QObject *parent = Q_NULLPTR);
    ~WSClient();
    void sendMsg(const QString &msg);
    void start();
    void stop();

    void timerEvent(QTimerEvent *event);
private:

signals:
    void connected();
    void closed();
    void reconnect();
    void orderNumber(const QVariant& newData);
public slots:
    void onConnected();
    void onReconnect();
    void onTextMessageReceived(QString message);
    void onDisconnected();
private:
    QString initJson();
    QVariant jsonData(QString number);
private:
    QWebSocket m_webSocket;
    QUrl m_url;
    QUrl m_httpUrl;
    bool m_start;
    bool m_getSession;
    QJsonArray aJson;
    int timerPing;
    int pingFlag;
    bool        isConnected;
    QTimer      m_reconnTimer;
};

#endif // WSCLIENT_H
