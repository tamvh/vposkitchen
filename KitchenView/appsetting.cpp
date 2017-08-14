#include "appsetting.h"

#include <QSettings>

AppSetting* AppSetting::_instance = Q_NULLPTR;

AppSetting::AppSetting(QObject *parent) : QObject(parent)
{
    _settings = new QSettings();
    load();
}

AppSetting::~AppSetting()
{
    delete _settings;
}

void AppSetting::load()
{
    _serverUrl = _settings->value("settings/serverurl", "ws://localhost:1234").toString();
    _serverUrl = "ws://192.168.7.2:1234";
    _serverUrl = "ws://127.0.0.1:1234";
}

void AppSetting::save()
{
    _settings->setValue("settings/serverurl", _serverUrl);
    _settings->sync();
}

QString AppSetting::serverUrl() const
{
    qDebug() << "server url:" << _serverUrl;
    return _serverUrl;
}

void AppSetting::setServerUrl(const QString &serverUrl)
{
    _serverUrl = serverUrl;
}
