#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <QObject>

class serverManager : public QObject
{
    Q_OBJECT
public:
    explicit serverManager(QObject *parent = nullptr);

signals:

public slots:
};

#endif // SERVERMANAGER_H