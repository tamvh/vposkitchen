#include "maincontroller.h"
//#include "wsclient.h"
#include "menureader.h"
#include "commonfunction.h"

#include "appsetting.h"
#include "utils.h"

#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>
#include <QJsonArray>
#include <QJsonObject>
#include <QDateTime>
#include <QTimerEvent>
#include <QDebug>


MainController::MainController(QObject *parent) : QObject(parent)
{
    _timerCheckEventHappen = 1000;
}

MainController::~MainController()
{

}

int MainController::initialize()
{
    initWSClient();
    return 0;
}

void MainController::initWSClient() {
    m_menuReader = new MenuReader();
    connect(m_menuReader, SIGNAL(orderNumber(QString)), this, SLOT(onOrderNumber(QString)));
    connect(m_menuReader, SIGNAL(statusBarCode(bool)), this, SLOT(onStatusBarCode(bool)));
    m_menuReader->start();
}

void MainController::onOrderNumber(const QString &newData) {
    qDebug() << "new data: " << newData;
    emit orderNumber(jsonData(newData));
}
void MainController::onStatusBarCode(bool stt) {
    qDebug() << "barcode status: " << stt;
    emit statusBarCode(stt);
}

QVariant MainController::jsonData(QString number) {
    QString data = "";
    qDebug() << "number: " << number;
    QString invoiceCode = CommonFunction::getInvoiceCodeFromBarCode(number);
    qDebug() << "Invoice Code: " << invoiceCode;
    if(invoiceCode.size()>=4) {
        data = invoiceCode.right(4);
    }
    QJsonObject jso;
    bool ck = false;
    jso["number"] = data;
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

void MainController::uninitialize()
{
    killTimer(_timerCheckEventHappen);
}

void MainController::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == _timerCheckEventHappen) {
        qDebug() << "check status barcode";
    }
}
