#ifndef ISTORE_H
#define ISTORE_H

#include <QQmlEngine>

namespace Store
{
/**
 * @brief The UserStore class saves the user's information.
 */
class IStore : public QObject
{
    Q_OBJECT
    QML_ANONYMOUS
public:
    using QObject::QObject;

    virtual ~IStore() = default;

    virtual bool save(const QByteArray &data) = 0;
    virtual void restore() = 0;
    virtual void clear() = 0;
    virtual QString refresh() const = 0;
};
}

#endif // ISTORE_H
