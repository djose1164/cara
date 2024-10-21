#ifndef ORDERQUEUEMODEL_H
#define ORDERQUEUEMODEL_H

#include "orderqueue.h"
#include "queuerepository.h"
#include <QAbstractListModel>
#include <QQmlEngine>

namespace Billing
{

class OrderQueueModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QueueRepository *repository MEMBER repository_ BINDABLE bindableRepository FINAL)
    QML_ELEMENT
public:
    enum class RoleNames
    {
        CommentRole = Qt::UserRole,
        CreatedAtRole,
        SalespersonRole,
        OrderRole,
        QueueStatusIdRole,
        QueueStatusStringRole,
        DetailsRole,
        PaymentRole,
        DateRole,
        StatusRole,
        CustomerRole,
        TakenRole,
        DoneRole,
        OrderIdRole,
        ProductQuantityRole,
        TotalToPayRole
    };
    explicit OrderQueueModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QHash<int, QByteArray> roleNames() const override;

    QBindable<QueueRepository *> bindableRepository();

    Q_INVOKABLE QString statusIdToString(OrderQueue::Status s) const;
    Q_INVOKABLE OrderQueue::Status statusStringToId(const QString &s);

private slots:
    void queueFetched(const QJsonArray &queue);

private:
    std::vector<std::unique_ptr<OrderQueue>> data_;
    QProperty<QueueRepository *> repository_;
    QPropertyNotifier repositoryNotifier_;
    QList<QMetaObject::Connection> repositoryConnections_;
};

} // namespace Billing

#endif // ORDERQUEUEMODEL_H
