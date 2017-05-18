#ifndef APPSETTING_H
#define APPSETTING_H

#include <QObject>
#include <QDebug>

class QSettings;
class AppSetting : public QObject
{
    Q_OBJECT
public:
    static AppSetting* getInstance(){
      if (Q_NULLPTR == _instance) {
          _instance = new AppSetting();
      }
      return _instance;
    }

    explicit AppSetting(QObject *parent = 0);
    ~AppSetting();

    void load();
    Q_INVOKABLE void save();   
    Q_INVOKABLE QString serverUrl() const;
    Q_INVOKABLE void setServerUrl(const QString &serverUrl);

//    Q_INVOKABLE int getVID() const;
//    Q_INVOKABLE void setVID(int vid);
//    Q_INVOKABLE int getPID() const;
//    Q_INVOKABLE void setPID(int pid);

signals:

public slots:

private:
    static AppSetting* _instance;
    QSettings* _settings;
//    int _updateVID;
//    int _updatePID;

    QString _serverUrl;
};

#endif // APPSETTING_H
