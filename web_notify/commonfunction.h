#ifndef COMMONFUNCTION_H
#define COMMONFUNCTION_H

#include <QString>
#include <QJsonArray>
class CommonFunction
{
public:
    CommonFunction();
    static QString gVPOS1();

    static QString formatMoney(int money);
    static QString formatMoney(const QString& money);
    static QString formatLogin(const QString &usrname, const QString &pwd);
    static QString formatPayDone(const QString &invce, const QString &devid, int type, const QString &appid);
    static QString formatInvoiceCancel(const QString& invoiceCode, const QString &appUser);
    static QString formatRequestFoods(const QString& appUser);
    static QString formatUpdateInventory(const QString& appUser, const QString &itemName, const int &inventory);

    static QString formatRequestCreateInvoice(const QString &merchantCode,
                                              const QString &machineName,
                                              long amount,
                                              const QString& deviceId,
                                              const QString& biId,
                                              const QJsonArray& billItems,
                                              const QString& appTitle,
                                              int paymentMethod);

    static QString formatSession(const QString& merchantCode, const QString& devId, const QString &biId);
    static QString formatWsSession(const QString& merchantCode, const QString& devId);
    static QString formatCheckin(const QString &cardId);
    static int     parsePmsid(const QString &respone, QString *msg, QString *sid, QString *biid);
    static int     parseWssid(const QString &respone, QString *sid);

    static QString formatRequestRecharge(const QString& appUser,const QString& cardCode, const QString& amount, const QString &machineName, const QString& sid);
    static QString formatRequestGetStaffInfo(const QString& appUser,const QString& cardCode);

    static int parseResponseZalopay(const QString& response,
                                        QString& qrcode, QString* token, quint16* appId,
                                        QString* ivceCode, QString *ivceSess, QString* ivceDate,
                                        QString* oripay, int *discount, QString* discpay);

    static QString generateQRCode(const QString &token, int appId);


    static bool parseResponseCreateInvoice2(const QString& response, QString& qrcode,
                                            QString* token, quint16* appId,
                                            QString* invoiceCode, QString *invoiceSess, QString* invceDate,
                                            QString* oripay, QString* realpay, QString* discpay);
    static int parseResponsePayCash(const QString &respone, QString *msg,
                                            QString *ivceCode,
                                            QString *ivceTime,
                                            QString *barcode,
                                            QString *balance);

    static QByteArray parseDataToReaderDevice(quint32 amount, quint16 appId, const QString& token);
    static QByteArray parseDataToRemoteReaderDevice(quint32 amount, quint16 appId, const QString& token);
    static QByteArray parseCmdStopToReaderDevice();
    static quint16 deviceDataChecksum(const QByteArray& data);
    static quint16 makeCrc16Kermit(const QByteArray &data);
    static QString getNotifyValue(const QString& msg, const QString& key);
    static bool    parseNotifyInfo(const QString &notify, QString *dt, QString *token);
    static bool    parseNotifyInfo(const QString &responeMsg, QString* ivceCode,
                                   QString* ivceTime, QString *barcode,
                                   QString* discount, QString *amount, QString* payment);

    static QString ViettoLatin(const QString& strIn);
   static void getLastUpdateTime(const QString &qstrData, qulonglong& qullLUT);

    static QString updatePrintBillTime(const QString& billDetail, const QString& datetime, const QString &barcode);

    static QString formatWSConnectString(const QString& url, const QString& merchantCode,
                                        const QString& deviceId, const QString& wsSession);

    static QString HMacSha256HexStringEncode(const QString& data, const QString& key);
};

#endif // COMMONFUNCTION_H
