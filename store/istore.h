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
    QML_NAMED_ELEMENT(Store)
    QML_UNCREATABLE("Store is an abstract class.")
public:
    using QObject::QObject;

    virtual ~IStore();

    virtual void save(const QByteArray &data) = 0;
    virtual void restore() = 0;
    virtual void clear() = 0;

signals:
    void saved();
    void errorOccurred();
};
}

#endif // ISTORE_H
