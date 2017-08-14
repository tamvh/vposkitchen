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
    _serverUrl = _settings->value("settings/serverurl", "http://localhost:1234").toString();
    _updatePID = _settings->value("settings/pid", 0).toByteArray().toUShort();
    _updateVID = _settings->value("settings/vid", 0).toByteArray().toUShort();
}

void AppSetting::save()
{
    _settings->setValue("settings/serverurl", _serverUrl);
    _settings->setValue("settings/vid", _updateVID);
    _settings->setValue("settings/pid", _updatePID);
    _settings->sync();
}

QString AppSetting::serverUrl() const
{
    return _serverUrl;
}

void AppSetting::setServerUrl(const QString &serverUrl)
{
    _serverUrl = serverUrl;
}

unsigned short AppSetting::getVID() const
{
    return _updateVID;
}

void AppSetting::setVID(unsigned short vid)
{
    _updateVID = vid;
}

unsigned short AppSetting::getPID() const
{
    return _updatePID;
}

void AppSetting::setPID(unsigned short pid)
{
    _updatePID = pid;
}
