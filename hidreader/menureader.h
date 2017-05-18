#ifndef MENUREADER_H
#define MENUREADER_H

#include <QObject>
#include <QThread>
#include <string>
#include <iostream>
#include <iomanip>
#include <hidapi/hidapi/hidapi.h>
#include "utils.h"

class MenuReader : public QThread
{
    Q_OBJECT
    void run() Q_DECL_OVERRIDE {
        read();
    }
signals:
    void orderNumber(QString newData);
    void statusBarCode(bool stt);
private slots:

protected:
    bool mappingChar(uint8_t *buffer, int len, char &out);
    bool checkHid(unsigned short vid, unsigned short pid);
public:
    void read();
private:
   int res = 0;
};

#endif // MENUREADER_H
