#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include <QVariant>
#include <QJsonArray>

class MenuReader;
class MainController : public QObject
{
    Q_OBJECT
public:
    explicit MainController(QObject *parent = 0);
    ~MainController();
    Q_INVOKABLE int initialize();
    Q_INVOKABLE void uninitialize();
signals:    
    void orderNumber(const QVariant& newData);
    void statusBarCode(bool stt);
protected:
    void timerEvent(QTimerEvent *event);
private slots:
    void onOrderNumber(const QString& newData);
    void onStatusBarCode(bool stt);
private:
    void initWSClient();
    QVariant jsonData(QString number);
private:
    int _timerCheckEventHappen;
    MenuReader *m_menuReader;
    QJsonArray aJson;
};

#endif // MAINCONTROLLER_H
