#ifndef HIDREADERMAIN_H
#define HIDREADERMAIN_H

#include <QObject>
#include <QJsonObject>
#include <wsclient.h>
#include <QString>
class WsCssClient;

class RecieveNotify : public QObject
{
    Q_OBJECT
public:
    explicit RecieveNotify(QObject *parent = 0);
    virtual ~RecieveNotify();
signals:

public slots:
    void    onVssConnected();
    void initialWebSocket(bool forceStop=false);
    void onCloudNotify(const QString &message);
private:
    void printBill(const QJsonObject& data);
    QString moneyMoney(QString money);
    long long getMoneyValue(const QString &moneyString);
    QString formatMoney(long long moneyValue);
private:
    WsCssClient     *wscssClient;
    WSClient        *wsClient;
    QString jsonSend2VposView(const QString& data);

};

#endif // HIDREADERMAIN_H
