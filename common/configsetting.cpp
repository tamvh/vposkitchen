#include <QMutex>
#include <QSettings>
#include <QDebug>
#include "configsetting.h"

ConfigSetting* ConfigSetting::m_instance = NULL;

ConfigSetting::ConfigSetting(bool autosave, QObject *parent) : QObject(parent)
{
    m_autoSave = autosave;

    Load();

    showBaocaoDate = false;
}

ConfigSetting::~ConfigSetting()
{
    if (m_autoSave) {
        Save();
    }
}

ConfigSetting *ConfigSetting::instance()
{
    static QMutex mutex;

    if (m_instance == NULL)
    {
        mutex.lock();

        if (m_instance == NULL) {
            m_instance = new ConfigSetting();
        }

        mutex.unlock();
    }

    return (m_instance);
}

#include <QStandardPaths>
void ConfigSetting::Load()
{
    QSettings sets("vng", "vpos");

    appName         = sets.value(QString("option/appname"), QString("")).toString();
    appTitle        = sets.value(QString("option/apptitle"), QString("")).toString();
    if (appTitle.isEmpty()) {
        appTitle = getTitle_version1(appName); // support title of privious-version
    }

    paymentName     = sets.value(QString("option/payment"),         QString("vpos.zing.vn/vpos")).toString();
    transProtocol   = sets.value(QString("option/transferprotocol"),QString("https")).toString();
    printerName     = sets.value(QString("option/printer"),         QString("xpriner58")).toString();
    dockServer      = sets.value(QString("option/dock"),            QString("192.168.1.1")).toString();
    dockVersion     = sets.value(QString("option/dockversion"),     QString("2.0")).toString();
    centerService   = sets.value(QString("option/centerService"),   "").toString();
    centerService   = "";
    printerSize     = sets.value(QString("option/printersize"),     QString("large")).toString();
    sobanin         = sets.value(QString("option/sobanin"), 1).toInt();
    tenbanin        = sets.value(QString("option/tenbanin"),        QString("Lien Bep;Lien Chinh;Lien Khach")).toString();
    billPreinf      = sets.value(QString("option/billpreinf"),      QString("TTTM FLEMINGTON;184 Le Dai Hanh, P.15, Q.11")).toString();
    billPostinf     = sets.value(QString("option/billpostinf"),     QString("Cam on Quy khach, hen gap lai")).toString();
    userEnableConf  = sets.value(QString("option/userenableconf"),  true).toBool();
    userName        = sets.value(QString("option/username"),        QString("")).toString();
    userPwd = "";
    searchAdvance   = sets.value(QString("option/searchadvance"),   true).toBool();
#ifdef QT_DEBUG
    userPwd         = sets.value(QString("option/userpwd"),         QString("")).toString();
#endif
    networkDevice   = sets.value(QString("option/networkdevice"),   true).toBool();
    idRecharge      = sets.value(QString("option/machine"),         0).toInt();
    paymentMethod   = sets.value(QString("option/paymentmethod"),   1).toInt(); //zalopay
    paymentMask     = sets.value(QString("option/paymentmask"),     1).toInt();
    foodRefreshTimer= sets.value(QString("option/foodrefreshtimer"),300).toInt();
    billAlertTimer  = sets.value(QString("option/billalerttimer"),  5).toInt();
    qrcodeRatio     = sets.value(QString("option/qrcoderatio"),     2.2).toFloat();
    showThanhtoancunghoadon = sets.value(QString("option/showthanhtoancunghoadon"), false).toBool();
    foodItemFontsize = sets.value(QString("option/fooditemfontsize"), 14).toInt();
    billItemFontsize = sets.value(QString("option/billitemfontsize"), 14).toInt();

    if( m_autoSave && !sets.contains("option/appname") )
    {
        Save();
    }
}

QString ConfigSetting::getTitle_version1(const QString &appname)
{
    if (appname == "canteenvng" ) {
        return QString("CANTEEN VNG");
    }
    else if (appname == "ffbanhmique" ) {
        return QString("BANH MI QUE");
    }
    else if (appname == "ffcomngon" ) {
        return QString("COM NGON");
    }
    else if (appname == "ffcomtamcali" ) {
        return QString("COM TAM CALI");
    }
    else if (appname == "ffcomnieu" ) {
        return QString("CƠM NIÊU");
    }
    else if (appname == "fftapicocup" ) {
        return QString("TAPICO CUP");
    }
    else if (appname == "fftokyobowl" ) {
        return QString("TOKYO BOWL");
    }
    else if (appname == "ffvietthai" ) {
        return QString("VIỆT THÁI");
    }
    else if (appname == "ffphongoon" ) {
        return QString("PHỞ NGOON");
    }
    else if (appname == "ffcool" ) {
        return QString("COOL");
    }
    else if (appname == "ffcobonla" ) {
        return QString("CO BON LA");
    }
    else if (appname == "ffdorayaki" ) {
        return QString("DORAYAKI");
    }
    else if (appname == "maybanhangtudong" ) {
        return QString("BAN HANG TU DONG");
    }
    else if (appname == "mqshops" ) {
        return QString("MQ shops");
    }
    else if (appname == "vpos" ) {
        return QString("MÁY BÁN HÀNG");
    }

    QString s = appname.toUpper().replace(QRegExp("FF|HV"), "");
    return s;
}

void ConfigSetting::Save()
{
    QSettings sets("vng", "vpos");

    sets.setValue(QString("option/appname"), appName);
    sets.sync();
    sets.setValue(QString("option/apptitle"), appTitle);
    sets.sync();
    sets.setValue(QString("option/printer"), printerName);
    sets.sync();
    sets.setValue(QString("option/payment"), paymentName);
    sets.sync();
    sets.setValue(QString("option/vposserver"), dockServer);
    sets.sync();
    sets.setValue(QString("option/dock"), dockServer);
    sets.sync();
    //sets.setValue(QString("option/centerService"), centerService);
    //sets.sync();
    sets.setValue(QString("option/dockversion"), dockVersion);
    sets.sync();
    sets.setValue(QString("option/printersize"), printerSize);
    sets.sync();
    sets.setValue(QString("option/sobanin"), sobanin);
    sets.sync();
    sets.setValue(QString("option/tenbanin"), tenbanin);
    sets.sync();
    sets.setValue(QString("option/billpreinf"), billPreinf);
    sets.sync();
    sets.setValue(QString("option/billpostinf"), billPostinf);
    sets.sync();
    sets.setValue(QString("option/userenableconf"), userEnableConf);
    sets.sync();
    sets.setValue(QString("option/username"), userName);
    sets.sync();
#ifdef QT_DEBUG
    sets.setValue(QString("option/userpwd"), userPwd);
    sets.sync();
#endif
    sets.setValue(QString("option/searchadvance"), searchAdvance);
    sets.sync();
    sets.setValue(QString("option/networkdevice"), networkDevice);
    sets.sync();
    sets.setValue(QString("option/machine"), idRecharge);
    sets.sync();
    sets.setValue(QString("option/paymentmethod"), paymentMethod);
    sets.sync();
    sets.setValue(QString("option/paymentmask"), paymentMask);
    sets.sync();
    sets.setValue(QString("option/foodrefreshtimer"), foodRefreshTimer);
    sets.sync();
    sets.setValue(QString("option/billalerttimer"), billAlertTimer);
    sets.sync();
    sets.setValue(QString("option/showthanhtoancunghoadon"), showThanhtoancunghoadon);
    sets.sync();
    sets.setValue(QString("option/fooditemfontsize"), foodItemFontsize);
    sets.sync();
    sets.setValue(QString("option/billitemfontsize"), billItemFontsize);
    sets.sync();
}

void ConfigSetting::setDocServer(const QString& docsrv)
{
    if( dockServer.compare(docsrv) ) {
        dockServer = docsrv;

        QSettings sets("vng", "vpos");
        sets.setValue(QString("option/dock"), dockServer);
        sets.sync();
    }
}

bool ConfigSetting::setCenterService(const QString& centerSrvIp)
{
    if( centerService.compare(centerSrvIp) ) {
        centerService = centerSrvIp;

        //QSettings sets("vng", "vpos");
        //sets.setValue(QString("option/centerService"), centerService);
        //sets.sync();

        return true;
    }

    return false;
}

void ConfigSetting::setDockVersion(const QString &ver)
{
    if (dockVersion != ver) {
        dockVersion = ver;

        QSettings sets("vng", "vpos");
        sets.setValue(QString("option/dockversion"), dockVersion);
        sets.sync();
    }
}

void ConfigSetting::setPrintersize(const QString &pagesize)
{
    if (printerSize != pagesize) {
        printerSize = pagesize;

        QSettings sets("vng", "vpos");
        sets.setValue(QString("option/printersize"), printerSize);
        sets.sync();
    }
}

void ConfigSetting::setSobanin(int num)
{
    if (sobanin != num) {
        sobanin = num;

        QSettings sets("vng", "vpos");
        sets.setValue(QString("option/sobanin"), sobanin);
        sets.sync();
    }
}

void ConfigSetting::setTenbanin(const QString &ten)
{
    if (tenbanin != ten) {
        tenbanin = ten;

        QSettings sets("vng", "vpos");
        sets.setValue(QString("option/tenbanin"), tenbanin);
        sets.sync();
    }
}

void ConfigSetting::setBillPreinf(const QString &info)
{
    if (billPreinf != info) {
        billPreinf = info;

        QSettings sets("vng", "vpos");
        sets.setValue(QString("option/billpreinf"), billPreinf);
        sets.sync();
    }
}

void ConfigSetting::setBillPostinf(const QString &info)
{
    if (billPostinf != info) {
        billPostinf = info;

        QSettings sets("vng", "vpos");
        sets.setValue(QString("option/billpostinf"), billPostinf);
        sets.sync();
    }
}

void ConfigSetting::setUserenableConfig(bool enableconf)
{
    if (userEnableConf != enableconf) {
        userEnableConf = enableconf;

        QSettings sets("vng", "vpos");
        sets.setValue(QString("option/userenableconf"), userEnableConf);
        sets.sync();
    }
}

void ConfigSetting::setUsername(const QString &uname, const QString &upwd)
{
    if (userName != uname) {
        userName = uname;

        QSettings sets("vng", "vpos");
        sets.setValue(QString("option/username"), userName);
        sets.sync();
    }

    if (userPwd != upwd) {
        userPwd = upwd;
#ifdef QT_DEBUG
        QSettings sets("vng", "vpos");
        sets.setValue(QString("option/userpwd"), userPwd);
        sets.sync();
#endif
    }
}

void ConfigSetting::setSearchadvance(bool bsearchadvance)
{
    if (searchAdvance != bsearchadvance) {
        searchAdvance = bsearchadvance;

        QSettings sets("vng", "vpos");
        sets.setValue(QString("option/searchadvance"), searchAdvance);
        sets.sync();
    }
}

void ConfigSetting::setPrinter(const QString &prnName)
{
    if (printerName != prnName) {
        printerName = prnName;

        QSettings sets("vng", "vpos");
        sets.setValue(QString("option/printer"), printerName);
        sets.sync();
    }
}

void ConfigSetting::setAppName(const QString &appname)
{
    if( appName.compare(appname) ) {
        appName = appname;

        QSettings sets("vng", "vpos");
        sets.setValue(QString("option/appname"), appName);
        sets.sync();

        setAppTitle( getTitle_version1(appName) );
    }
}

void ConfigSetting::setAppTitle(const QString& apptitle)
{
    if( appTitle.compare(apptitle) ) {
        appTitle = apptitle;

        QSettings sets("vng", "vpos");
        sets.setValue(QString("option/apptitle"), appTitle);
        sets.sync();
    }
}

void ConfigSetting::setRecharge(const int recharge)
{
    if (idRecharge != recharge) {
        idRecharge = recharge;

        QSettings sets("vng", "vpos");
        sets.setValue(QString("option/machine"), idRecharge);
        sets.sync();
    }
}

void ConfigSetting::setRefreshTimer(const int refreshTimer)
{
    if (foodRefreshTimer != refreshTimer) {
        foodRefreshTimer = refreshTimer;

        QSettings sets("vng", "vpos");
        sets.setValue(QString("option/foodrefreshtimer"), foodRefreshTimer);
        sets.sync();
    }
}

void ConfigSetting::setBillAlertTimer(const int alertTimer)
{
    if (billAlertTimer != alertTimer) {
        billAlertTimer = alertTimer;

        QSettings sets("vng", "vpos");
        sets.setValue(QString("option/billalerttimer"), billAlertTimer);
        sets.sync();
    }
}

void ConfigSetting::setTransProtocol(const QString transpro)
{
    if( transProtocol.compare(transpro) ) {
        transProtocol = transpro;

        QSettings sets("vng", "vpos");
        sets.setValue(QString("option/transferprotocol"), transProtocol);
        sets.sync();
    }
}

void ConfigSetting::setShowThanhtoancunghoadon(const bool bshow)
{
    if( showThanhtoancunghoadon != bshow ) {
        showThanhtoancunghoadon = bshow;

        QSettings sets("vng", "vpos");
        sets.setValue(QString("option/showthanhtoancunghoadon"), showThanhtoancunghoadon);
        sets.sync();
    }
}

bool ConfigSetting::setPaymentName(const QString& pmName)
{
    if( paymentName.compare(pmName) ) {
        paymentName = pmName;

        QSettings sets("vng", "vpos");
        sets.setValue(QString("option/payment"), paymentName);
        sets.sync();

        return true;
    }

    return false;
}

void ConfigSetting::setPaymentMethod(int pmMethod)
{
    if( paymentMethod != pmMethod ) {
        paymentMethod = pmMethod;

        QSettings sets("vng", "vpos");
        sets.setValue(QString("option/paymentmethod"), paymentMethod);
        sets.sync();
   }
}

void ConfigSetting::setPaymentMask(int pmMask)
{
    if (paymentMask != pmMask) {
        paymentMask = pmMask;

        QSettings sets("vng", "vpos");
        sets.setValue(QString("option/paymentmask"), paymentMask);
        sets.sync();
    }
}

void ConfigSetting::setQrcodeRatio(const float ratio)
{
    if (ratio != qrcodeRatio) {
        qrcodeRatio = ratio;

        QSettings sets("vng", "vpos");
        sets.setValue(QString("option/qrcoderatio"), qrcodeRatio);
        sets.sync();
    }
}

void ConfigSetting::setFoodItemFontsize(const int fntsize)
{
    if (foodItemFontsize != fntsize) {
        foodItemFontsize = fntsize;

        QSettings sets("vng", "vpos");
        sets.setValue(QString("option/fooditemfontsize"), foodItemFontsize);
        sets.sync();
    }
}

void ConfigSetting::setBillItemFontsize(const int fntsize)
{
    if (billItemFontsize != fntsize) {
        billItemFontsize = fntsize;

        QSettings sets("vng", "vpos");
        sets.setValue(QString("option/billitemfontsize"), billItemFontsize);
        sets.sync();
    }
}
