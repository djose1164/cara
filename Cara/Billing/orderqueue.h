#ifndef ORDERQUEUE_H
#define ORDERQUEUE_H

#include "Salesperson/salesperson.h"
#include "bill.h"
#include <QProperty>
#include <QQmlEngine>

namespace Billing
{

class OrderQueue : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString comment MEMBER comment_ BINDABLE bindableComment FINAL)
    Q_PROPERTY(Entity::Bill *bill MEMBER bill_ BINDABLE bindableBill FINAL)
    Q_PROPERTY(QDateTime createdAt MEMBER createdAt_ BINDABLE bindableCreatedAt FINAL)
    Q_PROPERTY(int statusId MEMBER statusId_ BINDABLE bindableStatusId FINAL)
    Q_PROPERTY(QString statusString READ default BINDABLE bindableStatusString FINAL)
    Q_PROPERTY(Entity::Salesperson salesperson READ default BINDABLE bindableSalesperson FINAL)
    QML_ELEMENT

public:
    enum class Status
    {
        Unassigned,
        Assigned,
        Completed,
        Cancelled,
        Returned,
        Refund,
        Any
    };
    Q_ENUM(Status)

    static QString statusIdToString(Status statudId);

    static Status statusStringToId(QAnyStringView s);

    static std::unique_ptr<OrderQueue> fromJson(const QJsonObject &json);

    OrderQueue(QObject *parent = nullptr);

    QBindable<QString> bindableComment() const;

    void setBill(Entity::Bill *newBill);
    QBindable<Entity::Bill *> bindableBill() const;

    QBindable<QDateTime> bindableCreatedAt() const;

    QBindable<int> bindableStatusId() const;
    QBindable<QString> bindableStatusString() const;

    QBindable<Entity::Salesperson> bindableSalesperson() const;

private:
    QProperty<QString> comment_;
    QProperty<Entity::Bill *> bill_;
    QProperty<QDateTime> createdAt_;
    QProperty<int> statusId_;
    QProperty<QString> statusString_;
    QProperty<Entity::Salesperson> salesperson_;
};

} // namespace Billing

#endif // ORDERQUEUE_H
