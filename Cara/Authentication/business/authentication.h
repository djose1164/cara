#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include "Utils/httpstatuscodes.h"
#include <QQmlEngine>

namespace Authentication::Business
{
class Authentication : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("This' an abstract class. Use an implementation instead.")
public:
    using QObject::QObject;

    virtual ~Authentication();

public slots:
    virtual void authenticate() = 0;

signals:
    void succeeded(const QByteArray &data);
    void errorOccurred(HttpStatus::Code error, const QString &errorString);
    void tokenReady(const QString &token);
};
}

#endif // AUTHENTICATION_H
