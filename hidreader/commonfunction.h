#ifndef COMMONFUNCTION_H
#define COMMONFUNCTION_H

#include <QString>

class CommonFunction
{
public:
    CommonFunction();
    static QString genBarCodeFromInvoiceCode(const QString &invoiceCode);
    static QString getInvoiceCodeFromBarCode(const QString &barCode);
};

#endif // COMMONFUNCTION_H
