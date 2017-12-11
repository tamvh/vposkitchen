#ifndef CMN_H
#define CMN_H

#include <QObject>
#include <QJsonObject>

class Cmn
{
public:
    Cmn();

    static int      getErrMsg(const QString &httprest, QString *msg=nullptr);
    static int      getQRInfo(const QString &respone, QString &zptoken, int &appid);

    static QString  buildMsg(int err, const QString &msg);
    static QString  buildQRPicture(const QString &zptoken, int appId);

    static QString  ViettoLatin(const QString& strIn);

    static QByteArray parseDataToRemoteReaderDevice(quint32 amount, quint16 appId, const QString &token);
    static quint16  makeCrc16Kermit(const QByteArray &data);

    static QString  broadcastPrinter(int prnId, QByteArray data);


    static QString  fakeFootItemorder(int &amount, QJsonObject &foodItems);
    static QString  fakeTableData();
};

#endif // CMN_H
