#include "recievenotify.h"
#include "commonfunction.h"
#include "cmn.h"
#include "../common/wscssclient.h"
#include "../common/prntdataformat.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QUuid>


RecieveNotify::RecieveNotify(QObject *parent)
    : QObject(parent)
{
    wsClient = 0;
    initialWebSocket();
    wscssClient = new WsCssClient( APPID::VPOS, APPID::PRINTER );
}

RecieveNotify::~RecieveNotify() {
    wsClient->stop();
    delete wsClient;
    delete wscssClient;
}

void RecieveNotify::onVssConnected()
{
    qDebug() << "onVssConnected ...";
}

QString RecieveNotify::jsonSend2VposView(const QString& data)
{
    QJsonObject jso;
    jso["action"] = packet_content_type::CHAT_TEXT_MSG;
    jso["appid"] = APPID::HIDREDER;
    jso["to_appid"] = APPID::KITCHEN_VIEW;
    jso["data"] = data;

    QJsonDocument doc(jso);
    return doc.toJson(QJsonDocument::Compact);
}

void RecieveNotify::initialWebSocket(bool forceStop)
{
    qDebug() << "initialWebSocket ... forcestop=" << forceStop << "\n";
    QString deviceId = QString(QUuid::createUuid().toRfc4122().toHex());

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
        QString merchant_code = "vpos";

        wsClient = new WSClient(wsUrl, httpUrl, merchant_code, deviceId, true, 0);

        QObject::connect(wsClient, SIGNAL(textMessageReceived(QString)), this, SLOT(onCloudNotify(const QString&)));
        QObject::connect(wsClient, &WSClient::connected, this, [=](){qDebug() << "cloud Connected";});
        QObject::connect(wsClient, &WSClient::closed, this, [=](){qDebug() << "cloud Disconnected";});
        wsClient->start();
    }
}

void RecieveNotify::onCloudNotify(const QString &message) {
    QString notifymsg = CommonFunction::getNotifyValue(message, "msg");
    QString notifydt = CommonFunction::getNotifyValue(message, "dt");
    qDebug() << "notifymsg: " << notifymsg<< ", data: " << message;
    QJsonDocument jsd = QJsonDocument::fromJson(QByteArray((message.toUtf8())));
    if (!jsd.isObject()) {
        return ;
    }
    QJsonObject jso = jsd.object();
    QJsonObject dt = jso["dt"].toObject();
    qDebug() << "dt: " << dt;
    this->printBill(dt);
}

void RecieveNotify::printBill(const QJsonObject &jso) {
    int PAGESZ, TAB1, TAB2, TAB3, TAB4;
    PAGESZ = 48;
    TAB1 = 12;
    TAB2 = 27;
    TAB3 = 38;
    TAB4 = 48;

    QString invoice_code = "";
    QString invoice_datetime = "";
    QString barcode = "";
    QString total_money = "";

    invoice_code = jso["invoice_code"].toString();
    invoice_datetime = jso["invoice_datetime"].toString();
    barcode = jso["barcode"].toString();
    total_money = this->moneyMoney(jso["payment"].toVariant().toString());

    Prntdataformat pdf;
    pdf.codeFontType(pdf.font_A);
    pdf.codeFontBold(true);
    pdf.codeLine("BEP CANTINVNG", pdf.Justify_center);
    pdf.codeFontBold(false);

    pdf.codeLine("182 Le Dai Hanh, P.15, Q.11", pdf.Justify_center);
    QString invoice_code_right_4 = "0000";
    if(invoice_code.length() >= 4) {
        invoice_code_right_4 = invoice_code.right(4);
    }
    pdf.codeFontType(pdf.font_A);
    pdf.codeFontBold(true);
    pdf.codeLine("#" + invoice_code_right_4, pdf.Justify_center);
    pdf.codeFontBold(false);

    pdf.codeLine(QString("May:\t%1").arg("MAY-1"), pdf.Justify_left, TAB1);
    pdf.codeLine(QString("Hoa don:\t%1").arg(invoice_code));
    pdf.codeLine(QString("Ngay:\t%1").arg(invoice_datetime));
    pdf.codeLine(QString("Vi tri:\t%1").arg("TABLE: 1-3"));

    pdf.codeChars('-', PAGESZ, pdf.Justify_center);

    pdf.codeText("TEN", pdf.Justify_left);
    pdf.codeLine("\tSO LUONG", pdf.Justify_right, TAB4);
    pdf.codeChars('-', PAGESZ, pdf.Justify_center);


    if (jso["items"].isArray())
    {
        QJsonArray receiptItem = jso["items"].toArray();
        for (int i = 0; i < receiptItem.size(); i++)
        {
            QJsonObject item = receiptItem[i].toObject();

            QString name = Cmn::ViettoLatin(item["item_name"].toString());
            if (name.length() >= TAB2)
                pdf.codeLine(name, pdf.Justify_left);
            else
                pdf.codeText(name, pdf.Justify_left);

            pdf.codeText("\t", pdf.Justify_left, TAB2);
            pdf.codeText("\t", pdf.Justify_right, TAB3);
            pdf.codeLine("\t" + item["quantity"].toVariant().toString(), pdf.Justify_right, TAB4);
        }
    }


    pdf.codeChars('-', PAGESZ, pdf.Justify_left);

//    pdf.codeLine(QString("TONG:%1").arg(total_money), pdf.Justify_left, TAB4-total_money.length());

//    pdf.codeChars('-', PAGESZ, pdf.Justify_left);
//    pdf.codeFeedline(1);

    pdf.codeBarcode(barcode, 2, 80, pdf.Justify_center);
    pdf.codeFeedline(1);

    pdf.codeLine("Quet ma qrcode ben duoi khi lam xong mon");


    pdf.codeFeedline(PAGESZ < 40 ? 2 : 1);
    pdf.codeCutpage(true);
    QByteArray prntdatacode = pdf.getPrintDeviceCode();
    int prnId = 4;
    if( wscssClient ) {
        wscssClient->sendMsg( Cmn::broadcastPrinter(prnId, prntdatacode) );
    }
}

long long RecieveNotify::getMoneyValue(const  QString &moneyString)
{
    QString money = moneyString;
    money = money.replace(QString(","),QString::null);
    if (money.isEmpty())
    {
        return 0;
    }
    else
    {
        return money.toLongLong();
    }
}

QString RecieveNotify::formatMoney(long long moneyValue)
{
    long long absVal = abs(moneyValue);
    QString money = QString::number(absVal);
    int loop = money.length() / 3;
    if (loop > 0)
    {
        if (money.length() % 3 == 0)
        {
            loop = loop - 1;
        }

        int index = money.length();
        for (int i = 0; i < loop ; i++)
        {
            index = index - 3;
            money.insert(index,  QString(","));
        }
    }

    if (moneyValue < 0)
        money.insert(0,  QString("-"));

    return money;
}

QString RecieveNotify::moneyMoney(QString money)
{
    // convert money string "12345" >> "12,345"
    return formatMoney(getMoneyValue(money));
}
