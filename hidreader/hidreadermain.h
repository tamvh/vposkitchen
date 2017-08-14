#ifndef HIDREADERMAIN_H
#define HIDREADERMAIN_H

#include <QObject>

class MenuReader;
class WsCssClient;

class HidReaderMain : public QObject
{
    Q_OBJECT
public:
    explicit HidReaderMain(QObject *parent = 0);

signals:

public slots:
    void    onBarcode(const QString &bc);
    void    onVssConnected();

private:
    MenuReader      *m_menuReader;
    WsCssClient     *wscssClient;

    QString jsonSend2VposView(const QString& data);

};

#endif // HIDREADERMAIN_H
