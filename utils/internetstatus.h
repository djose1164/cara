#ifndef INTERNETSTATUS_H
#define INTERNETSTATUS_H

#include <QObject>
#include <QQmlEngine>

class InternetStatus : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit InternetStatus(QObject *parent = nullptr);

    Q_INVOKABLE bool hasInternetConnection() const;
signals:

};

#endif // INTERNETSTATUS_H
