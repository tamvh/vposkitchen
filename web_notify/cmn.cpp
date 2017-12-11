#include "cmn.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDataStream>
#include <QTextCodec>
#include <QDebug>

Cmn::Cmn()
{
}

int Cmn::getErrMsg(const QString &httprest, QString *msg)
{
    int err = -1;

    QJsonDocument jsd = QJsonDocument::fromJson(httprest.toUtf8());
    if( jsd.isObject() )
    {
        QJsonObject jso = jsd.object();

        if (jso.contains("err")) {
            err = jso["err"].toInt();
        }

        if (msg && jso.contains("msg")) {
            *msg = jso["msg"].toString();
        }
    }

    return err;
}

int Cmn::getQRInfo(const QString &respone, QString &zptoken, int &appid)
{
    int err = -1;

    QJsonDocument jsd = QJsonDocument::fromJson(respone.toUtf8());
    if( jsd.isObject() ) {
        QJsonObject jso = jsd.object();

        if( jso.contains("err") ) {
            err = jso["err"].toInt();
        }

        if( jso.contains("dt") ) {
            QJsonObject jdt = jso["dt"].toObject();

            if( jdt.contains("zptranstoken") ) {
                zptoken = jdt["zptranstoken"].toString();
            }

            if( jdt.contains("appid") ) {
                appid = jdt["appid"].toInt();
            }
        }
    }

    return err;
}

QString Cmn::buildMsg(int err, const QString &msg)
{
    QJsonObject jso;
    jso["err"]  = err;
    jso["msg"]  = msg;

    QJsonDocument jsd( jso );
    return QString::fromUtf8( jsd.toJson(QJsonDocument::Compact).data() );
}

QString Cmn::buildQRPicture(const QString &zptoken, int appId)
{
    QJsonObject jso;
    jso["zptranstoken"] = zptoken;
    jso["appid"] = appId;

    QJsonDocument jsd(jso);

    return QString::fromUtf8(jsd.toJson().data());
}

QString Cmn::ViettoLatin(const QString& strIn)
{
    QString strOut;
    strOut=strIn;
    strOut=strOut.replace(QRegExp("à|á|ạ|ả|ã|â|ầ|ấ|ậ|ẩ|ẫ|ă|ằ|ắ|ặ|ẳ|ẵ"),"a");
    strOut=strOut.replace(QRegExp("À|Á|Ạ|Ả|Ã|Ă|Ằ|Ắ|Ặ|Ẳ|Ẵ|Â|Ấ|Ầ|Ậ|Ẩ|Ẫ"),"A");
    strOut=strOut.replace(QRegExp("è|é|ẹ|ẻ|ẽ|ê|ề|ế|ệ|ể|ễ"),"e");
    strOut=strOut.replace(QRegExp("Ề|Ế|Ệ|Ể|Ê|Ễ|É|È|Ẻ|Ẽ|Ẹ"),"E");
    strOut=strOut.replace(QRegExp("ì|í|ị|ỉ|ĩ"),"i");
    strOut=strOut.replace(QRegExp("Ì|Í|Ị|Ỉ|Ĩ"),"I");
    strOut=strOut.replace(QRegExp("ò|ó|ọ|ỏ|õ|ô|ồ|ố|ộ|ổ|ỗ|ơ|ờ|ớ|ợ|ở|ỡ"),"o");
    strOut=strOut.replace(QRegExp("Ò|Ó|Ọ|Ỏ|Õ|Ơ|Ờ|Ớ|Ợ|Ở|Ỡ|Ô|Ố|Ổ|Ộ|Ồ|Ỗ"),"O");
    strOut=strOut.replace(QRegExp("ù|ú|ụ|ủ|ũ|ư|ừ|ứ|ự|ử|ữ"),"u");
    strOut=strOut.replace(QRegExp("Ừ|Ứ|Ự|Ử|Ư|Ữ|Ù|Ú|Ụ|Ủ|Ũ"),"U");
    strOut=strOut.replace(QRegExp("ỳ|ý|ỵ|ỷ|ỹ"),"y");
    strOut=strOut.replace(QRegExp("Ỳ|Ý|Ỵ|Ỷ|Ỹ"),"y");
    strOut=strOut.replace(QRegExp("đ"),"d");
    strOut=strOut.replace(QRegExp("Đ"),"D");
    return strOut;
}

QByteArray Cmn::parseDataToRemoteReaderDevice(quint32 amount, quint16 appId, const QString &token)
{
    QByteArray dataArray;
    QDataStream dataStream(&dataArray, QIODevice::WriteOnly);
    dataStream.setByteOrder(QDataStream::LittleEndian);

    // Header
    //dataStream << (qint8)0xA0;

    // Len
    //dataStream << (qint8)0x1C; //28

    // Cmd
    //dataStream << (qint8)0x01;

    // SeqID
    //dataStream << (qint8)0x01;

    // Direction Flag
    dataStream << (qint8)0x07;

    dataStream << amount;

    dataStream << appId;
    //dataArray.append((char*)outAppId,2);
    //dataStream << QByteArray::fromBase64(token.toUtf8(), QByteArray::Base64UrlEncoding);

    dataArray.append(QByteArray::fromBase64(token.toLocal8Bit(), QByteArray::Base64UrlEncoding));

    QByteArray dataArray2;
    QDataStream dataStream2(&dataArray2, QIODevice::WriteOnly);
    dataStream2.setByteOrder(QDataStream::LittleEndian);
    //quint16 checksum = deviceDataChecksum(dataArray);
    quint16 checksum = makeCrc16Kermit(dataArray.right(dataArray.length()-1));
    dataStream2 << checksum;

    //dataStream2 << (qint8)0xFF;

    dataArray.append(dataArray2);

    return dataArray;
}

quint16 Cmn::makeCrc16Kermit(const QByteArray &data)
{
    quint16         valuehex = 0;
    quint16         CRC = 0;
    int             size = data.size();
    const quint8    *pData  = reinterpret_cast<const quint8*>(data.constData());

    CRC = 0;
    for(int i=0; i<size; ++i) {
        valuehex = ((*pData ^ CRC) & 0x0fu) * 0x1081;
        CRC >>= 4;
        CRC ^= valuehex;
        valuehex = ( (*pData >> 4) ^ (CRC & 0x00ffu) ) & 0x0fu;
        CRC >>= 4;
        CRC ^= (valuehex * 0x1081u);

        ++pData;
    }
    quint16 ret = ( (CRC & 0x00ffu) << 8) | ((CRC & 0xff00u) >> 8);

    return ret;
}

QString Cmn::broadcastPrinter(int prnId, QByteArray data)
{
    QString str = QTextCodec::codecForMib(1015)->toUnicode(data);

    QJsonObject jso;
    jso["msg"]      = "broadcast";
    jso["prnid"]    = prnId;
    jso["prncode"]  = str; //QString::fromUtf16(data.da);  //QString::fromUtf8(data);

    QJsonDocument jsd(jso);
    return QString::fromUtf8( jsd.toJson(QJsonDocument::Compact) );
}

QString Cmn::fakeFootItemorder(int &amount, QJsonObject &foodItems)
{
    static int order = 0;

    if( order == 0 )
    {
        QJsonObject i1;
        i1["item_id"] = 1;
        i1["item_name"] = "Bun Bo Hue";
        i1["quantity"] = 1;
        i1["price"] = 10000;
        i1["amount"] = 10000;

        QJsonObject i2;
        i2["item_id"] = 2;
        i2["item_name"] = "Bun Moc";
        i2["quantity"] = 1;
        i2["price"] = 20000;
        i2["amount"] = 20000;

        QJsonArray jarr;
        jarr.append(i1);
        jarr.append(i2);

        foodItems["items"] = jarr;

        amount = 30000;
    }
    else
    {
        QJsonObject i1;
        i1["item_id"] = 3;
        i1["item_name"] = "Tra sua tran chau";
        i1["quantity"] = 3;
        i1["price"] = 10000;
        i1["amount"] = 30000;

        QJsonObject i2;
        i2["item_id"] = 2;
        i2["item_name"] = "Bun Moc";
        i2["quantity"] = 1;
        i2["price"] = 20000;
        i1["amount"] = 20000;

        QJsonArray jarr;
        jarr.append(i1);
        jarr.append(i2);

        foodItems["items"] = jarr;

        amount = 50000;
    }

    order++;

    return 0;
}

QString Cmn::fakeTableData()
{
    QJsonArray jarr;
    for( int i=1; i<21; i++ ) {
        QJsonObject jtab;
        jtab["table_id"] = i;
        jtab["table_name"] = QString("Bàn %1").arg(i);
        jtab["status"] = (i%3)+1;

        jarr.append(jtab);
    }

    QJsonObject jdt;
    jdt["tables"] = jarr;

    QJsonObject jso;
    jso["err"] = 0;
    jso["msg"] = "";
    jso["dt"] = jdt;

    QJsonDocument jsd(jso);

    return QString::fromUtf8(jsd.toJson(QJsonDocument::Compact).data());
}
