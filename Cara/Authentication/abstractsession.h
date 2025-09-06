#ifndef ABSTRACTSESSION_H
#define ABSTRACTSESSION_H

#include <QObject>
#include <QQmlEngine>

namespace Authentication
{

class Session;

class AbstractSession : public QObject
{
    Q_OBJECT
    QML_ANONYMOUS
public:
    explicit AbstractSession(QObject *parent = nullptr);
    virtual ~AbstractSession() = default;

    virtual bool isAuthenticated() const = 0;
    virtual void startSession(const Authentication::Session &session) = 0;
    virtual void clearSession() = 0;

signals:
    void sessionStarted();
    void sessionCleared();
};

} // namespace Authentication

#endif // ABSTRACTSESSION_H
