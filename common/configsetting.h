#ifndef CONFIGSETTING_H
#define CONFIGSETTING_H

#include <QObject>

class ConfigSetting : public QObject
{
    Q_OBJECT
public:
    static ConfigSetting *instance();
    explicit ConfigSetting(bool autosave = false, QObject *parent = 0);
    ~ConfigSetting();

    void Load();
    void Save();
    void setDocServer(const QString& docsrv);
    bool setCenterService(const QString& centerSrvIp);
    void setDockVersion(const QString &ver);
    void setPrintersize(const QString &pagesize);
    void setSobanin(int num);
    void setTenbanin(const QString &ten);
    void setBillPreinf(const QString &info);
    void setBillPostinf(const QString &info);
    void setUserenableConfig(bool enableconf);
    void setUsername(const QString &uname, const QString &upwd);
    void setSearchadvance(bool bsearchadvance);
    void setPrinter(const QString &prnName);
    void setAppName(const QString& appname);
    void setAppTitle(const QString& apptitle);
    void setRecharge(const int recharge);
    void setRefreshTimer(const int refreshTimer);
    void setBillAlertTimer(const int alertTimer);
    void setTransProtocol(const QString transpro);
    void setShowThanhtoancunghoadon(const bool bshow);
    bool setPaymentName(const QString& pmName);
    void setPaymentMethod(int pmMethod);
    void setPaymentMask(int pmMask);
    void setQrcodeRatio(const float ratio);
    void setFoodItemFontsize(const int fntsize);
    void setBillItemFontsize(const int fntsize);

    QString transProtocol;  // http / https
    QString appName;        // "canteenvng", "vpos", ...
    QString appTitle;       // "CĂN TIN"
    QString printerName;    // "xpriner58"
    QString paymentName;    // "104.199.170.120", "payment.vng.com.vn"
    QString dockServer;     // "192.168.1.1"
    QString dockVersion;    // DockServerService version; 1.0 is original, 2.0 support extent command
    QString centerService;  // local vpos center server service IP 192.168.1.1
    QString printerSize;    // small/large
    int     sobanin;        // so luong ban in 1..n
    QString tenbanin;       // Prefix-sobanin
    QString billPreinf;     // Bill prefix sau Title "Đ/c: 182 Lê Đại Hành"
    QString billPostinf;    // Bill postfix
    bool    userEnableConf;
    QString userName;       // đăng nhập login
    QString userPwd;        // đăng nhập
    bool    searchAdvance;  // tìm kiếm nâng cao các từ viết tắt bb = 'bun bo'

    int     idRecharge;     // id recharge machine: 0..20
    int     paymentMethod;  // 1:zalo; 2:card; 4:cash
    int     paymentMask;    // zalo | card | cash
    int     foodRefreshTimer;// time to refresh food data in second
    int     billAlertTimer; // time waiting to alert user get print bill
    float   qrcodeRatio;    // qr-code image ratio

    int     foodItemFontsize;
    int     billItemFontsize;

    bool    showThanhtoancunghoadon; // hiển thị thanh toán cùng hóa đơn ra màn hình
    bool    showBaocaoDate; // show textbox chọn ngày in báo cáo
    bool    networkDevice;

private:
    static ConfigSetting *m_instance;

    QString getTitle_version1(const QString &appname);
    bool    m_autoSave;

signals:

public slots:
};

#endif // CONFIGSETTING_H
