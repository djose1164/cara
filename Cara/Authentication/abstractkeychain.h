#ifndef ABSTRACTKEYCHAIN_H
#define ABSTRACTKEYCHAIN_H

#include <QQmlEngine>

namespace Authentication
{

class AbstractKeyChain : public QObject
{
    Q_OBJECT
    QML_ANONYMOUS
public:
    using QObject::QObject;
    virtual ~AbstractKeyChain() = default;

    virtual void readKey(const QString &key) = 0;
    virtual void writeKey(const QString &key, const QString &value) = 0;
    virtual void deleteKey(const QString &key) = 0;

signals:
    void keyStored(const QString &key);
    void keyRestored(const QString &key, const QString &value);
    void keyDeleted(const QString &key);
    void errorOccurred(const QString &errorString);
};

} // namespace Authentication

#endif // ABSTRACTKEYCHAIN_H
