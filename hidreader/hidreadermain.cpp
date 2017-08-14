#include "hidreadermain.h"
#include "menureader.h"
#include "commonfunction.h"
#include "../common/wscssclient.h"
#include <QJsonObject>
#include <QJsonDocument>

HidReaderMain::HidReaderMain(QObject *parent)
    : QObject(parent)
{
    m_menuReader = new MenuReader();
    connect(m_menuReader, SIGNAL(orderNumber(QString)), this, SLOT(onBarcode(QString)));

    wscssClient = new WsCssClient( APPID::HIDREDER, APPID::KITCHEN_VIEW );
    connect(wscssClient, SIGNAL(connected()), m_menuReader, SLOT(start()));
}

void HidReaderMain::onBarcode(const QString &bc)
{
    QString data = "";
    QString invoiceCode = CommonFunction::getInvoiceCodeFromBarCode(bc);
    if(invoiceCode.size()>=4) {
        data = invoiceCode.right(4);
    } else {
        data = invoiceCode;
    }

    qDebug() << "New data:" << data;

    QString json_data = jsonSend2VposView(data);
    wscssClient->sendTextMessage( json_data );
}

void HidReaderMain::onVssConnected()
{
    qDebug() << "onVssConnected ...";
}

QString HidReaderMain::jsonSend2VposView(const QString& data)
{
    QJsonObject jso;
    jso["action"] = packet_content_type::CHAT_TEXT_MSG;
    jso["appid"] = APPID::HIDREDER;
    jso["to_appid"] = APPID::KITCHEN_VIEW;
    jso["data"] = data;

    QJsonDocument doc(jso);
    return doc.toJson(QJsonDocument::Compact);
}
