#include "maincontroller.h"
#include "../common/wscssclient.h"
#include "appsetting.h"
#include "utils.h"

//#include <QtCore/QCommandLineParser>
//#include <QtCore/QCommandLineOption>
//#include <QDateTime>
//#include <QTimerEvent>
//#include <QDebug>

MainController::MainController(QObject *parent)
    : QObject(parent)
    , wscssClient(Q_NULLPTR)
{
}

MainController::~MainController()
{
}

int MainController::initialize()
{
    _wsclient = 0;
    initWSClient();
}

void MainController::initWSClient(bool forceStop)
{
    if( wscssClient ) {
        delete wscssClient;
        wscssClient = NULL;
    }

    wscssClient = new WsCssClient( APPID::KITCHEN_VIEW, APPID::KITCHEN_VIEW );
    QObject::connect(wscssClient, SIGNAL(orderNumber(QVariant)), this, SLOT(onOrderNumber(QVariant)));
    QObject::connect(wscssClient, SIGNAL(foundServer(QString,int)), this, SLOT(onServerFound(QString,int)));
}

void MainController::onOrderNumber(const QVariant &newData)
{
    emit orderNumber(newData);
}

void MainController::onServerFound(const QString &ip, const int &port)
{
    AppSetting::getInstance()->setServerUrl(QString("ws://%1:%2").arg(ip).arg(port));
}

void MainController::uninitialize()
{
}

void MainController::restartWsConnect()
{
    initWSClient(true);
}
