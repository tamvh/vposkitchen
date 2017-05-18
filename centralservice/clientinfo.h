#ifndef CLIENTINFO_H
#define CLIENTINFO_H
#include <QString>
class ClientInfo
{
public:
    ClientInfo();
    ~ClientInfo();
public:
    int getAppId();
    int getType();
    QString getDesc();
    void setAppId(int value);
    void setType(int value);
    void setDesc(const QString& value);
private:
    int _appid;
    int _type;
    QString _desc;
};

#endif // CLIENTINFO_H
