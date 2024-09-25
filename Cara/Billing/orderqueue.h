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
    Q_PROPERTY(QString lastComment MEMBER lastComment_ BINDABLE bindableLastComment FINAL)
    Q_PROPERTY(Entity::Bill *bill MEMBER bill_ BINDABLE bindableBill FINAL)
    Q_PROPERTY(QDateTime createdAt MEMBER createdAt_ BINDABLE bindableCreatedAt FINAL)
    Q_PROPERTY(Status status MEMBER status_ BINDABLE bindableStatus FINAL)
    Q_PROPERTY(QString statusString READ default BINDABLE bindableStatusString FINAL)
    Q_PROPERTY(Entity::Salesperson salesperson READ default BINDABLE bindableSalesperson FINAL)
    Q_PROPERTY(bool taken READ default BINDABLE bindableTaken FINAL)
    QML_ELEMENT

public:
    enum class Status
    {
        Unassigned = 1,
        Assigned,
        Completed,
        Cancelled,
        Returned,
        Refund,
        Any
    };
    Q_ENUM(Status)

    static QString statusToString(Status statudId);

    static Status statusStringToId(QAnyStringView s);

    static std::unique_ptr<OrderQueue> fromJson(const QJsonObject &json);

    OrderQueue(QObject *parent = nullptr);

    void setBill(Entity::Bill *newBill);
    QBindable<Entity::Bill *> bindableBill() const;

    QBindable<QDateTime> bindableCreatedAt() const;

    QBindable<Status> bindableStatus();

    QBindable<QString> bindableStatusString() const;

    QBindable<Entity::Salesperson> bindableSalesperson() const;

    QBindable<bool> bindableTaken();

    QBindable<QString> bindableLastComment();

private:
    QProperty<Entity::Bill *> bill_;
    QProperty<QDateTime> createdAt_;
    QProperty<Status> status_;
    QProperty<QString> statusString_;
    QProperty<Entity::Salesperson> salesperson_;
    QProperty<bool> taken_;
    QProperty<QString> lastComment_;
};

} // namespace Billing

#endif // ORDERQUEUE_H
