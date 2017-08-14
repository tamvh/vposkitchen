#include "settingscontroller.h"
#include "appsetting.h"
#include "utils.h"

#include <QJsonValue>
#include <QJsonArray>

SettingsController::SettingsController(QObject *parent) : QObject(parent)
{

}

void SettingsController::testRed()
{
    Utils::turnOnRed();
}

void SettingsController::testGreen()
{
    Utils::turnOnGreen();
}

void SettingsController::testBlue()
{
    Utils::turnOnBlue();
}

void SettingsController::testOff()
{
    Utils::turnOff();
}

void SettingsController::testCrash()
{
    QObject* test = Q_NULLPTR;
    QString string = test->objectName();
}
