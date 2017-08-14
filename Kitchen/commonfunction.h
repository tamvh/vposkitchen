#ifndef COMMONFUNCTION_H
#define COMMONFUNCTION_H
#include <QString>
#include <QVariant>

class CommonFunction
{
public:
    CommonFunction();
    static QString generateQRCode(const QString &url, const QString &_datetime, int _roomId);
    static QString formatWsSession(const QString& app_id);
    static QString genBarCodeFromInvoiceCode(const QString &invoiceCode);
    static QString getInvoiceCodeFromBarCode(const QString &barCode);
};

#endif // COMMONFUNCTION_H
