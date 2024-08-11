#ifndef QUEUEREPOSITORY_H
#define QUEUEREPOSITORY_H

#include <QQmlEngine>

namespace Billing
{

class QueueRepository : public QObject
{
    Q_OBJECT
    QML_ANONYMOUS

public:
    using SalespersonId = int;
    using QObject::QObject;

    virtual ~QueueRepository() = default;

    virtual void fetchOrdersInQueue(SalespersonId spId) = 0;

signals:
    void queueFetched(const QJsonArray &queue);
};

}

#endif // QUEUEREPOSITORY_H
