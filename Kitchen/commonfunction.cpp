#include "commonfunction.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDataStream>
#include <QByteArray>
#include <QDebug>
#include <QtEndian>
#include <QFileInfo>
#include <QStandardPaths>
#include <QCryptographicHash>
#include <QMessageAuthenticationCode>
#include "encoder.h"

CommonFunction::CommonFunction()
{

}

QString CommonFunction::generateQRCode(const QString &url, const QString &_datetime, int _roomId)
{
    return NULL;
}

QString CommonFunction::formatWsSession(const QString& app_id)
{
    QJsonObject jso;
    jso["app_id"] = app_id;
    QJsonDocument jsd(jso);

    return QString::fromUtf8(jsd.toJson().data());
}

QString CommonFunction::genBarCodeFromInvoiceCode(const QString &invoiceCode)
{
    int len = invoiceCode.length();
    int i, j, x = 0, y = 0, c;

    if (len < 10)
        return "";

    j = 1;
    for (i = 0; i < len; i++)
    {
        c = (invoiceCode.at(i).toLatin1())*j;
        x = x + c;
        j += 3;
    }
    x = (x % 9 + 1) + '0';

    j = 1;
    for (i = len - 1; i >= 0; i--)
    {
        c = (invoiceCode.at(i).toLatin1())*j;
        y = y + c;
        j += 1;
    }
    y = (y % 9 + 1) + '0';

    char *temp = new char[len+2];
    char *barCode = new char[len+3];

    strncpy(temp, invoiceCode.toLatin1().data(), 6);
    temp[6] = (char)x;
    temp[7] = (char)y;
    strncpy(temp + 8, invoiceCode.toLatin1().data() + 6, len - 6);

    Encoder::encode(temp, len + 2, barCode);
    barCode[len+2] = 0;

    QString ret = QString(barCode);
    delete []barCode;
    delete []temp;

    return ret;
}

QString CommonFunction::getInvoiceCodeFromBarCode(const QString &barCode)
{
    int len = barCode.length();
    char *temp = new char[len];
    char *invoiceCode = new char[len + 1];

    if (len < 12)
        return "";

    strncpy(temp, barCode.toLatin1().data(), len);
    Encoder::decode(temp, len, invoiceCode);

    invoiceCode[6] = 0;
    invoiceCode[len] = 0;
    QString ret = QString(invoiceCode) + QString(invoiceCode + 8);
    delete []invoiceCode;
    delete []temp;

    return ret;
}
