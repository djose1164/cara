#ifndef SESSION_H
#define SESSION_H

#include <QObject>
#include <QQmlEngine>

class Session
{
    QML_INTERFACE
public:
    virtual ~Session();

    virtual void update() = 0;

    virtual void clear() = 0;
};
Q_DECLARE_INTERFACE(Session, "SessionInterface")
#endif // SESSION_H
