#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include <QVariant>

class WSClient;
class WsCssClient;
class MainController : public QObject
{
    Q_OBJECT
public:
    explicit MainController(QObject *parent = 0);
    ~MainController();
    Q_INVOKABLE int initialize();
    Q_INVOKABLE void uninitialize();
    Q_INVOKABLE void restartWsConnect();
signals:    
    void orderNumber(const QVariant& newData);
private slots:
    void onOrderNumber(const QVariant& newData);
    void onServerFound(const QString &ip, const int &port);
private:
    void initWSClient(bool forceStop=false);
private:
    WSClient * _wsclient;
    WsCssClient *wscssClient;
};

#endif // MAINCONTROLLER_H
