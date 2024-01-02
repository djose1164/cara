#ifndef ORDERREPOSITORY_H
#define ORDERREPOSITORY_H

#include <QObject>
#include <QRestAccessManager>
#include <QRestReply>

namespace Business
{
class OrderRepository : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;

    virtual ~OrderRepository();

public slots:
    virtual void add(const QVariantMap &payload) = 0;

signals:
    void errorOccurred();

    void resourceAdded(const QJsonObject &data);
};
}

#endif // ORDERREPOSITORY_H
